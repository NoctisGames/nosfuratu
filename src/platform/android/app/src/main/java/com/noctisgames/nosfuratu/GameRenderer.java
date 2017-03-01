package com.noctisgames.nosfuratu;

import android.app.Activity;
import android.app.ActivityManager;
import android.graphics.Point;
import android.opengl.GLSurfaceView.Renderer;
import android.view.Display;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import static com.noctisgames.nosfuratu.BuildConfig.DEBUG;

public final class GameRenderer implements Renderer
{
    // Requested Action Definitions from src/core/game/logic/GameConstants.h

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
    private static final short REQUESTED_ACTION_DISPLAY_INTERSTITIAL_AD = 8;

    private final Activity _activity;
    private final InterstitialAd mInterstitialAd;

    private long _startTime;

    public GameRenderer(Activity activity)
    {
        _activity = activity;
        mInterstitialAd = new InterstitialAd(_activity);
        mInterstitialAd.setAdUnitId(DEBUG ? "ca-app-pub-3940256099942544/1033173712" : "ca-app-pub-6017554042572989/7036617356");
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

        AndroidMain.init(activity, isLowMemoryDevice);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        AndroidMain.on_surface_created();

        _startTime = System.nanoTime();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
        AndroidMain.on_surface_changed(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl)
    {
        float deltaTime = (System.nanoTime() - _startTime) / 1000000000.0f;
        _startTime = System.nanoTime();

        int requestedAction = AndroidMain.get_requested_action();
        if (requestedAction >= 1000)
        {
            requestedAction /= 1000;
        }

        switch (requestedAction)
        {
            case REQUESTED_ACTION_LEVEL_EDITOR_SAVE:
                AndroidMain.clear_requested_action();
                break;
            case REQUESTED_ACTION_LEVEL_EDITOR_LOAD:
                AndroidMain.clear_requested_action();
                break;
            case REQUESTED_ACTION_LEVEL_COMPLETED:
                markLevelAsCompleted(AndroidMain.get_requested_action());
                AndroidMain.clear_requested_action();
                break;
            case REQUESTED_ACTION_SUBMIT_SCORE_ONLINE:
                submitScoreOnline(AndroidMain.get_requested_action());
                AndroidMain.clear_requested_action();
                break;
            case REQUESTED_ACTION_UNLOCK_LEVEL:
                unlockLevel(AndroidMain.get_requested_action());
                AndroidMain.clear_requested_action();
                break;
            case REQUESTED_ACTION_SET_CUTSCENE_VIEWED:
                setCutsceneViewedFlag(AndroidMain.get_requested_action());
                AndroidMain.clear_requested_action();
                break;
            case REQUESTED_ACTION_GET_SAVE_DATA:
                sendSaveData();
                AndroidMain.clear_requested_action();
                break;
            case REQUESTED_ACTION_DISPLAY_INTERSTITIAL_AD:
                displayInterstitialAdIfLoaded();
                AndroidMain.clear_requested_action();
                break;
            case REQUESTED_ACTION_UPDATE:
            default:
                break;
        }

        AndroidMain.update(deltaTime);

        AndroidMain.render();
    }

    public void onResume()
    {
        AndroidMain.on_resume();
    }

    public void onPause()
    {
        AndroidMain.on_pause();
    }

    public void onStop()
    {
        AndroidMain.on_stop();
    }

    public void handleTouchDown(float rawX, float rawY)
    {
        AndroidMain.on_touch_down(rawX, rawY);
    }

    public void handleTouchDragged(float rawX, float rawY)
    {
        AndroidMain.on_touch_dragged(rawX, rawY);
    }

    public void handleTouchUp(float rawX, float rawY)
    {
        AndroidMain.on_touch_up(rawX, rawY);
    }

    public boolean handleOnBackPressed()
    {
        return AndroidMain.handle_on_back_pressed();
    }

    private void unlockLevel(int requestedAction)
    {
        int world = calcWorld(requestedAction);
        int level = calcLevel(requestedAction);
        int levelStatsFlag = AndroidMain.get_level_stats_flag_for_unlocked_level();
        int numGoldenCarrots = AndroidMain.get_num_golden_carrots_after_unlocking_level();

        SaveData.setLevelStatsFlag(world, level, levelStatsFlag);

        SaveData.setNumGoldenCarrots(numGoldenCarrots);
    }

    private void markLevelAsCompleted(int requestedAction)
    {
        int world = calcWorld(requestedAction);
        int level = calcLevel(requestedAction);
        int score = AndroidMain.get_score();
        int levelStatsFlag = AndroidMain.get_level_stats_flag();
        int numGoldenCarrots = AndroidMain.get_num_golden_carrots();
        int jonUnlockedAbilitiesFlag = AndroidMain.get_jon_unlocked_abilities_flag();

        SaveData.setLevelComplete(world, level, score, levelStatsFlag, jonUnlockedAbilitiesFlag);

        SaveData.setNumGoldenCarrots(numGoldenCarrots);
    }

    private void submitScoreOnline(int requestedAction)
    {
        int world = calcWorld(requestedAction);
        int level = calcLevel(requestedAction);
        int onlineScore = AndroidMain.get_online_score();

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

        int numWorlds = 1;
        for (int i = 1; i <= numWorlds; i++)
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
            if (i < numWorlds)
            {
                usd += ",";
            }
        }
        usd += "}";

        AndroidMain.load_user_save_data(usd);
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

        return requestedAction;
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
        ActivityManager actManager = (ActivityManager) activity.getSystemService(Activity.ACTIVITY_SERVICE);
        ActivityManager.MemoryInfo memInfo = new ActivityManager.MemoryInfo();
        actManager.getMemoryInfo(memInfo);

        return memInfo.totalMem;
    }
}
