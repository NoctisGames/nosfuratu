package com.noctisgames.nosfuratu;

import android.app.Activity;
import android.app.ActivityManager;
import android.graphics.Point;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Build;
import android.util.Log;
import android.view.Display;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;
import com.noctisgames.nosfuratu.platform.PlatformAssetUtils;
import com.noctisgames.nosfuratu.sound.SoundManager;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public final class GameRenderer implements Renderer
{
    // Definitions from src/core/game/logic/GameConstants.h

    //// Requested Action Definitions ////

    private static final short REQUESTED_ACTION_UPDATE = 0;
    // Save, Load, Completed, Submit Score Online, and Unlock Level actions are passed in this format: [1-4][1-5][01-21], where the first digit is the action, second is the world, third is the level
    private static final short REQUESTED_ACTION_LEVEL_EDITOR_SAVE = 1;
    private static final short REQUESTED_ACTION_LEVEL_EDITOR_LOAD = 2;
    private static final short REQUESTED_ACTION_LEVEL_COMPLETED = 3;
    private static final short REQUESTED_ACTION_SUBMIT_SCORE_ONLINE = 4;
    private static final short REQUESTED_ACTION_UNLOCK_LEVEL = 5;

    // Set Cutscene Viewed action is passed in this format: [6][001-999], where the first digit is the action, and the rest is the cutscenes viewed flag
    private static final short REQUESTED_ACTION_SET_CUTSCENE_VIEWED = 6;

    private static final short REQUESTED_ACTION_GET_SAVE_DATA = 7;

    private static final short REQUESTED_ACTION_SHOW_MESSAGE = 8; // Passed in this format: [8][001-999], where the first digit is the action and the rest determines the actual message (defined below under //// Message Definitions ////)

    private static final short REQUESTED_ACTION_DISPLAY_INTERSTITIAL_AD = 9;

    //// Music Definitions ////

    private static final short MUSIC_STOP = 1;
    private static final short MUSIC_RESUME = 2;
    private static final short MUSIC_PLAY = 3; // Assuming music is loaded
    private static final short MUSIC_PLAY_LOOP = 4; // Assuming music is loaded
    private static final short MUSIC_LOAD_TITLE_LOOP = 5;
    private static final short MUSIC_LOAD_OPENING_CUTSCENE = 6;
    private static final short MUSIC_LOAD_LEVEL_SELECT_LOOP = 7;
    private static final short MUSIC_LOAD_WORLD_1_LOOP = 8;
    private static final short MUSIC_LOAD_MID_BOSS_LOOP = 9;
    private static final short MUSIC_LOAD_END_BOSS_LOOP = 10;
    private static final short MUSIC_SET_VOLUME = 11; // Passed in this format: [3][0-100], where the first digit is the action and the rest determines the volume (0-100)

    //// Sound Definitions ////

    private static final short SOUND_JON_VAMPIRE_GLIDE = 20;
    private static final short SOUND_SPARROW_FLY = 23;
    private static final short SOUND_SAW_GRIND = 27;
    private static final short SOUND_SPIKED_BALL_ROLLING = 60;
    private static final short STOP_SOUND_JON_VAMPIRE_GLIDE = SOUND_JON_VAMPIRE_GLIDE + 1000;
    private static final short STOP_SOUND_SPARROW_FLY = SOUND_SPARROW_FLY + 1000;
    private static final short STOP_SOUND_SAW_GRIND = SOUND_SAW_GRIND + 1000;
    private static final short STOP_SOUND_SPIKED_BALL_ROLLING = SOUND_SPIKED_BALL_ROLLING + 1000;
    private static final short STOP_ALL_SOUNDS = 9998;
    private static final short STOP_ALL_LOOPING_SOUNDS = 9999;

    private final Activity _activity;
    private final InterstitialAd mInterstitialAd;
    private SoundManager _soundManager;

    private long _startTime;

    public GameRenderer(Activity activity)
    {
        _activity = activity;
        mInterstitialAd = new InterstitialAd(_activity);

        if (BuildConfig.DEBUG)
        {
            mInterstitialAd.setAdUnitId("ca-app-pub-3940256099942544/1033173712");
        }
        else
        {
            mInterstitialAd.setAdUnitId("ca-app-pub-6017554042572989/7036617356");
        }

        mInterstitialAd.setAdListener(new AdListener()
        {
            @Override
            public void onAdClosed()
            {
                requestNewInterstitial();
            }
        });

        requestNewInterstitial();

        SaveData.init(activity);

        int[] screenDimensions = getScreenDimensions(activity);

        double ramSize = getTotalRAM(activity);
        boolean isLowMemoryDevice = screenDimensions[0] < 2048 || screenDimensions[1] < 2048 || ramSize < 2147483648.0; // 2048 MB

        Log.d("NosFURatu", "ramSize: " + ramSize);
        Log.d("NosFURatu", "isLowMemoryDevice: " + (isLowMemoryDevice ? "YES" : "NO"));

        Game.init(isLowMemoryDevice);

        initSoundEngine(activity);

        PlatformAssetUtils.init_asset_manager(activity.getAssets());
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        Log.d("GameRenderer", "GL Surface created!");

        Game.on_surface_created();

        _startTime = System.nanoTime();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
        Log.d("GameRenderer", "GL Surface changed!");

        Game.on_surface_changed(width, height);
        Game.on_resume();
    }

    @Override
    public void onDrawFrame(GL10 gl)
    {
        float deltaTime = (System.nanoTime() - _startTime) / 1000000000.0f;
        _startTime = System.nanoTime();

        int requestedAction = Game.get_requested_action();
        if (requestedAction >= 1000)
        {
            requestedAction /= 1000;
        }

        switch (requestedAction)
        {
            case REQUESTED_ACTION_LEVEL_EDITOR_SAVE:
                Game.clear_requested_action();
                break;
            case REQUESTED_ACTION_LEVEL_EDITOR_LOAD:
                Game.clear_requested_action();
                break;
            case REQUESTED_ACTION_LEVEL_COMPLETED:
                markLevelAsCompleted(Game.get_requested_action());
                Game.clear_requested_action();
                break;
            case REQUESTED_ACTION_SUBMIT_SCORE_ONLINE:
                submitScoreOnline(Game.get_requested_action());
                Game.clear_requested_action();
                break;
            case REQUESTED_ACTION_UNLOCK_LEVEL:
                unlockLevel(Game.get_requested_action());
                Game.clear_requested_action();
                break;
            case REQUESTED_ACTION_SET_CUTSCENE_VIEWED:
                setCutsceneViewedFlag(Game.get_requested_action());
                Game.clear_requested_action();
                break;
            case REQUESTED_ACTION_GET_SAVE_DATA:
                sendSaveData();
                Game.clear_requested_action();
                break;
            case REQUESTED_ACTION_SHOW_MESSAGE:
                Game.clear_requested_action();
                break;
            case REQUESTED_ACTION_DISPLAY_INTERSTITIAL_AD:
                displayInterstitialAdIfLoaded();
                Game.clear_requested_action();
                break;
            case REQUESTED_ACTION_UPDATE:
            default:
                break;
        }

        Game.update(deltaTime);

        Game.render();

        handleSound();
        handleMusic();
    }

    public void onResume()
    {
        Log.d("GameRenderer", "onResume");

        Game.on_resume();
    }

    public void onPause()
    {
        Log.d("GameRenderer", "onPause");

        Game.on_pause();

        _soundManager.pauseMusic();

        stopSound(SOUND_JON_VAMPIRE_GLIDE);
        stopSound(SOUND_SPARROW_FLY);
        stopSound(SOUND_SAW_GRIND);
        stopSound(SOUND_SPIKED_BALL_ROLLING);

    }

    public void handleTouchDown(float rawX, float rawY)
    {
        Game.on_touch_down(rawX, rawY);
    }

    public void handleTouchDragged(float rawX, float rawY)
    {
        Game.on_touch_dragged(rawX, rawY);
    }

    public void handleTouchUp(float rawX, float rawY)
    {
        Game.on_touch_up(rawX, rawY);
    }

    public boolean handleOnBackPressed()
    {
        return Game.handle_on_back_pressed();
    }

    private void handleSound()
    {
        short soundId;
        while ((soundId = Game.get_current_sound_id()) > 0)
        {
            switch (soundId)
            {
                case SOUND_JON_VAMPIRE_GLIDE:
                case SOUND_SPARROW_FLY:
                case SOUND_SAW_GRIND:
                case SOUND_SPIKED_BALL_ROLLING:
                    playSound(soundId, true);
                    break;
                case STOP_SOUND_JON_VAMPIRE_GLIDE:
                case STOP_SOUND_SPARROW_FLY:
                case STOP_SOUND_SAW_GRIND:
                case STOP_SOUND_SPIKED_BALL_ROLLING:
                    stopSound(soundId - 1000);
                    break;
                case STOP_ALL_SOUNDS:
                    stopAllSounds();
                    break;
                case STOP_ALL_LOOPING_SOUNDS:
                    stopAllLoopingSounds();
                    break;
                default:
                    playSound(soundId);
                    break;
            }
        }
    }

    private void handleMusic()
    {
        short rawMusicId;
        while ((rawMusicId = Game.get_current_music_id()) > 0)
        {
            short musicId = rawMusicId;
            if (musicId >= 1000)
            {
                musicId /= 1000;
                rawMusicId -= musicId * 1000;
            }

            switch (musicId)
            {
                case MUSIC_STOP:
                    _soundManager.pauseMusic();
                    break;
                case MUSIC_RESUME:
                    _soundManager.resumeMusic();
                    break;
                case MUSIC_PLAY:
                    _soundManager.playMusic(0.5f, false);
                    break;
                case MUSIC_PLAY_LOOP:
                    _soundManager.playMusic(0.5f, true);
                    break;
                case MUSIC_SET_VOLUME:
                {
                    float volume = rawMusicId / 100.0f;

                    _soundManager.setMusicVolume(volume);
                }
                break;
                case MUSIC_LOAD_TITLE_LOOP:
                    _soundManager.loadMusic(_activity, R.raw.title_bgm);
                    break;
                case MUSIC_LOAD_OPENING_CUTSCENE:
                    _soundManager.loadMusic(_activity, R.raw.opening_cutscene_bgm);
                    break;
                case MUSIC_LOAD_LEVEL_SELECT_LOOP:
                    _soundManager.loadMusic(_activity, R.raw.level_select_bgm);
                    break;
                case MUSIC_LOAD_WORLD_1_LOOP:
                    _soundManager.loadMusic(_activity, R.raw.world_1_bgm);
                    break;
                case MUSIC_LOAD_MID_BOSS_LOOP:
                    _soundManager.loadMusic(_activity, R.raw.mid_boss_bgm);
                    break;
                case MUSIC_LOAD_END_BOSS_LOOP:
                    _soundManager.loadMusic(_activity, R.raw.final_boss_bgm);
                    break;
                default:
                    break;
            }
        }
    }

    private void playSound(int soundId, boolean isLooping)
    {
        _soundManager.playSound(soundId - 1, 1, isLooping);
    }

    private void playSound(int soundId)
    {
        _soundManager.playSound(soundId - 1, 1, false);
    }

    private void stopSound(int soundId)
    {
        _soundManager.stopSound(soundId - 1);
    }

    private void stopAllSounds()
    {
        SoundManager.stop_all_sounds();
    }

    private void stopAllLoopingSounds()
    {
        SoundManager.stop_all_looping_sounds();
    }

    private void unlockLevel(int requestedAction)
    {
        int world = calcWorld(requestedAction);
        int level = calcLevel(requestedAction);
        int levelStatsFlag = Game.get_level_stats_flag_for_unlocked_level();
        int numGoldenCarrots = Game.get_num_golden_carrots_after_unlocking_level();

        SaveData.setLevelStatsFlag(world, level, levelStatsFlag);

        SaveData.setNumGoldenCarrots(numGoldenCarrots);
    }

    private void markLevelAsCompleted(int requestedAction)
    {
        int world = calcWorld(requestedAction);
        int level = calcLevel(requestedAction);
        int score = Game.get_score();
        int levelStatsFlag = Game.get_level_stats_flag();
        int numGoldenCarrots = Game.get_num_golden_carrots();
        int jonUnlockedAbilitiesFlag = Game.get_jon_unlocked_abilities_flag();

        SaveData.setLevelComplete(world, level, score, levelStatsFlag, jonUnlockedAbilitiesFlag);

        SaveData.setNumGoldenCarrots(numGoldenCarrots);
    }

    private void submitScoreOnline(int requestedAction)
    {
        int world = calcWorld(requestedAction);
        int level = calcLevel(requestedAction);
        int onlineScore = Game.get_online_score();

        // TODO, submit score using Google Play, on success, save the score that was pushed online

        SaveData.setScorePushedOnline(world, level, onlineScore);
    }

    private void setCutsceneViewedFlag(int requestedAction)
    {
        while (requestedAction >= 1000)
        {
            requestedAction -= 1000;
        }

        int cutsceneViewedFlag = requestedAction;

        SaveData.setViewedCutscenesFlag(cutsceneViewedFlag);
    }

    private void sendSaveData()
    {
        int numGoldenCarrots = SaveData.getNumGoldenCarrots();
        int jonUnlockedAbilitiesFlag = SaveData.getJonUnlockedAbilitiesFlag();
        int viewedCutscenesFlag = SaveData.getViewedCutscenesFlag();

        String usd = "{";
        usd += "\"num_golden_carrots\": " + numGoldenCarrots + ", ";
        usd += "\"jon_unlocked_abilities_flag\": " + jonUnlockedAbilitiesFlag + ", ";
        usd += "\"viewed_cutscenes_flag\": " + viewedCutscenesFlag + ", ";

        for (int i = 1; i <= 5; i++)
        {
            usd += "\"world_" + i + "\":[";

            for (int j = 1; j <= 21; j++)
            {
                int statsFlag = SaveData.getLevelStatsFlag(i, j);
                int score = SaveData.getLevelScore(i, j);
                int scoreOnline = SaveData.getScorePushedOnline(i, j);

                usd += "{";
                usd += "\"stats_flag\": " + statsFlag + ", ";
                usd += "\"score\": " + score + ", ";
                usd += "\"score_online\": " + scoreOnline + " ";

                usd += "}";
                if (j < 21)
                {
                    usd += ",";
                }

                usd += " ";
            }
            usd += "]";
            if (i < 5)
            {
                usd += ",";
            }
        }
        usd += "}";

        Game.load_user_save_data(usd);
    }

    private int calcWorld(int requestedAction)
    {
        int world = 0;

        while (requestedAction >= 1000)
        {
            requestedAction -= 1000;
        }

        while (requestedAction >= 100)
        {
            requestedAction -= 100;
            world++;
        }

        return world;
    }

    private int calcLevel(int requestedAction)
    {
        while (requestedAction >= 1000)
        {
            requestedAction -= 1000;
        }

        while (requestedAction >= 100)
        {
            requestedAction -= 100;
        }

        int level = requestedAction;

        return level;
    }

    private void initSoundEngine(Activity activity)
    {
        _soundManager = new SoundManager(activity);

        _soundManager.loadSound(activity, R.raw.collect_carrot, 6);
        _soundManager.loadSound(activity, R.raw.collect_golden_carrot, 1);
        _soundManager.loadSound(activity, R.raw.death, 1);
        _soundManager.loadSound(activity, R.raw.footstep_left_grass, 1);
        _soundManager.loadSound(activity, R.raw.footstep_right_grass, 1);
        _soundManager.loadSound(activity, R.raw.footstep_left_cave, 1);
        _soundManager.loadSound(activity, R.raw.footstep_right_cave, 1);
        _soundManager.loadSound(activity, R.raw.jump_spring, 1);
        _soundManager.loadSound(activity, R.raw.landing_grass, 1);
        _soundManager.loadSound(activity, R.raw.landing_cave, 1);
        _soundManager.loadSound(activity, R.raw.snake_death, 3);
        _soundManager.loadSound(activity, R.raw.trigger_transform, 2);
        _soundManager.loadSound(activity, R.raw.cancel_transform, 2);
        _soundManager.loadSound(activity, R.raw.complete_transform, 1);
        _soundManager.loadSound(activity, R.raw.jump_spring_heavy, 1);
        _soundManager.loadSound(activity, R.raw.jon_rabbit_jump, 1);
        _soundManager.loadSound(activity, R.raw.jon_vampire_jump, 1);
        _soundManager.loadSound(activity, R.raw.jon_rabbit_double_jump, 2);
        _soundManager.loadSound(activity, R.raw.jon_vampire_double_jump, 2);
        _soundManager.loadSound(activity, R.raw.vampire_glide_loop, 1);
        _soundManager.loadSound(activity, R.raw.mushroom_bounce, 3);
        _soundManager.loadSound(activity, R.raw.jon_burrow_rocksfall, 1);
        _soundManager.loadSound(activity, R.raw.sparrow_fly_loop, 3);
        _soundManager.loadSound(activity, R.raw.sparrow_die, 3);
        _soundManager.loadSound(activity, R.raw.toad_die, 1);
        _soundManager.loadSound(activity, R.raw.toad_eat, 1);
        _soundManager.loadSound(activity, R.raw.saw_grind_loop, 3);
        _soundManager.loadSound(activity, R.raw.fox_bounced_on, 1);
        _soundManager.loadSound(activity, R.raw.fox_strike, 3);
        _soundManager.loadSound(activity, R.raw.fox_death, 3);
        _soundManager.loadSound(activity, R.raw.world_1_bgm_intro, 1);
        _soundManager.loadSound(activity, R.raw.mid_boss_bgm_intro, 1);
        _soundManager.loadSound(activity, R.raw.mid_boss_owl_swoop, 1);
        _soundManager.loadSound(activity, R.raw.mid_boss_owl_tree_smash, 1);
        _soundManager.loadSound(activity, R.raw.mid_boss_owl_death, 1);
        _soundManager.loadSound(activity, R.raw.screen_transition, 1);
        _soundManager.loadSound(activity, R.raw.screen_transition_2, 1);
        _soundManager.loadSound(activity, R.raw.level_complete, 1);
        _soundManager.loadSound(activity, R.raw.title_lightning_1, 2);
        _soundManager.loadSound(activity, R.raw.title_lightning_2, 2);
        _soundManager.loadSound(activity, R.raw.ability_unlock, 1);
        _soundManager.loadSound(activity, R.raw.boss_level_clear, 1);
        _soundManager.loadSound(activity, R.raw.level_clear, 1);
        _soundManager.loadSound(activity, R.raw.level_selected, 3);
        _soundManager.loadSound(activity, R.raw.rabbit_drill, 2);
        _soundManager.loadSound(activity, R.raw.snake_jump, 3);
        _soundManager.loadSound(activity, R.raw.vampire_dash, 3);
        _soundManager.loadSound(activity, R.raw.boss_level_unlock, 1);
        _soundManager.loadSound(activity, R.raw.rabbit_stomp, 3);
        _soundManager.loadSound(activity, R.raw.final_boss_bgm_intro, 1);
        _soundManager.loadSound(activity, R.raw.button_click, 3);
        _soundManager.loadSound(activity, R.raw.level_confirmed, 1);
        _soundManager.loadSound(activity, R.raw.bat_poof, 1);
        _soundManager.loadSound(activity, R.raw.chain_snap, 1);
        _soundManager.loadSound(activity, R.raw.end_boss_snake_mouth_open, 1);
        _soundManager.loadSound(activity, R.raw.end_boss_snake_charge_cue, 1);
        _soundManager.loadSound(activity, R.raw.end_boss_snake_charge, 1);
        _soundManager.loadSound(activity, R.raw.end_boss_snake_damaged, 1);
        _soundManager.loadSound(activity, R.raw.end_boss_snake_death, 1);
        _soundManager.loadSound(activity, R.raw.spiked_ball_rolling_loop, 2);
        _soundManager.loadSound(activity, R.raw.absorb_dash_ability, 1);
        _soundManager.loadSound(activity, R.raw.footstep_left_wood, 1);
        _soundManager.loadSound(activity, R.raw.footstep_right_wood, 1);
        _soundManager.loadSound(activity, R.raw.landing_wood, 1);
    }

    private void displayInterstitialAdIfLoaded()
    {
        _activity.runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                if (mInterstitialAd.isLoaded())
                {
                    mInterstitialAd.show();
                }
            }
        });
    }

    private void requestNewInterstitial()
    {
        AdRequest adRequest = new AdRequest.Builder()
                .addTestDevice("SEE_YOUR_LOGCAT_TO_GET_YOUR_DEVICE_ID")
                .build();

        mInterstitialAd.loadAd(adRequest);
    }

    private static int[] getScreenDimensions(Activity activity)
    {
        Display display = activity.getWindowManager().getDefaultDisplay();
        Point size = new Point();
        display.getSize(size);
        int width = size.x;
        int height = size.y;

        return new int[]{width, height};
    }

    private static double getTotalRAM(Activity activity)
    {
        if (Build.VERSION.SDK_INT >= 16)
        {
            ActivityManager actManager = (ActivityManager) activity.getSystemService(Activity.ACTIVITY_SERVICE);
            ActivityManager.MemoryInfo memInfo = new ActivityManager.MemoryInfo();
            actManager.getMemoryInfo(memInfo);

            return memInfo.totalMem;
        }
        else
        {
            return 536870912; // 512 MB, which will result in compressed textures being used
        }
    }
}
