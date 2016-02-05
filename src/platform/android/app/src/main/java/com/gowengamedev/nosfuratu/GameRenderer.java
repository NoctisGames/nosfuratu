package com.gowengamedev.nosfuratu;

import android.app.Activity;
import android.opengl.GLSurfaceView.Renderer;
import android.os.AsyncTask;
import android.os.Environment;
import android.os.SystemClock;
import android.util.Log;
import android.widget.Toast;

import com.gowengamedev.nosfuratu.platform.PlatformAssetUtils;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public final class GameRenderer implements Renderer
{
    static
    {
        System.loadLibrary("game");
    }

    // Definitions from src/core/game/GameConstants.h
    //// Requested Action Definitions ////

    private static final short REQUESTED_ACTION_UPDATE = 0;
    // Save/Load actions are passed in this format: [1-2][1-5][0-20], where the first digit is the action, second is the world, third is the level
    private static final short REQUESTED_ACTION_LEVEL_EDITOR_SAVE = 1;
    private static final short REQUESTED_ACTION_LEVEL_EDITOR_LOAD = 2;

    //// Music Definitions ////

    private static final short MUSIC_STOP = 1;
    private static final short MUSIC_RESUME = 2;
    private static final short MUSIC_PLAY_DEMO = 3;

    //// Sound Definitions ////

    private static final short SOUND_COLLECT_CARROT = 1;
    private static final short SOUND_COLLECT_GOLDEN_CARROT = 2;
    private static final short SOUND_DEATH = 3;
    private static final short SOUND_FOOTSTEP_LEFT_GRASS = 4;
    private static final short SOUND_FOOTSTEP_RIGHT_GRASS = 5;
    private static final short SOUND_FOOTSTEP_LEFT_CAVE = 6;
    private static final short SOUND_FOOTSTEP_RIGHT_CAVE = 7;
    private static final short SOUND_JUMP_SPRING = 8;
    private static final short SOUND_LANDING_GRASS = 9;
    private static final short SOUND_LANDING_CAVE = 10;
    private static final short SOUND_BREAK_LOG = 11;
    private static final short SOUND_DESTROY_ROCK = 12;
    private static final short SOUND_SNAKE_DEATH = 13;
    private static final short SOUND_TRIGGER_TRANSFORM = 14;
    private static final short SOUND_CANCEL_TRANSFORM = 15;
    private static final short SOUND_COMPLETE_TRANSFORM = 16;
    private static final short SOUND_JUMP_SPRING_HEAVY = 17;
    private static final short SOUND_JON_RABBIT_JUMP = 18;
    private static final short SOUND_JON_VAMPIRE_JUMP = 19;
    private static final short SOUND_JON_RABBIT_DOUBLE_JUMP = 20;
    private static final short SOUND_JON_VAMPIRE_DOUBLE_JUMP = 21;
    private static final short SOUND_JON_VAMPIRE_GLIDE = 22;
    private static final short SOUND_STOP_JON_VAMPIRE_GLIDE = 1021;

    private final Activity _activity;
    private final FileHandler _fileHandler;
    private final Audio _audio;
    private Music _bgm;
    private List<Sound> _sounds = new ArrayList<Sound>();
    private FPSCounter _fpsCounter;

    private float _lastRealTimeMeasurement_ms;
    private boolean _isDoingIO = false;

    public GameRenderer(Activity activity)
    {
        _activity = activity;
        _fileHandler = new FileHandler(new File(Environment.getExternalStorageDirectory(), "NosFURatu"));
        _audio = new Audio(activity.getAssets());
        _sounds.add(_audio.newSound("collect_carrot.wav"));
        _sounds.add(_audio.newSound("collect_golden_carrot.wav"));
        _sounds.add(_audio.newSound("death.wav"));
        _sounds.add(_audio.newSound("footstep_left_grass.wav"));
        _sounds.add(_audio.newSound("footstep_right_grass.wav"));
        _sounds.add(_audio.newSound("footstep_left_cave.wav"));
        _sounds.add(_audio.newSound("footstep_right_cave.wav"));
        _sounds.add(_audio.newSound("jump_spring.wav"));
        _sounds.add(_audio.newSound("landing_grass.wav"));
        _sounds.add(_audio.newSound("landing_cave.wav"));
        _sounds.add(_audio.newSound("break_log.wav"));
        _sounds.add(_audio.newSound("destroy_rock.wav"));
        _sounds.add(_audio.newSound("snake_death.wav"));
        _sounds.add(_audio.newSound("trigger_transform.wav"));
        _sounds.add(_audio.newSound("cancel_transform.wav"));
        _sounds.add(_audio.newSound("complete_transform.wav"));
        _sounds.add(_audio.newSound("jump_spring_heavy.wav"));
        _sounds.add(_audio.newSound("jon_rabbit_jump.wav"));
        _sounds.add(_audio.newSound("jon_vampire_jump.wav"));
        _sounds.add(_audio.newSound("jon_rabbit_double_jump.wav"));
        _sounds.add(_audio.newSound("jon_vampire_double_jump.wav"));
        _sounds.add(_audio.newSound("vampire_glide_loop.wav"));

        _fpsCounter = new FPSCounter();

        PlatformAssetUtils.init_asset_manager(activity.getAssets());

        init();
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        Log.d("GameRenderer", "GL Surface created!");

        on_surface_created();

        _lastRealTimeMeasurement_ms = (float) SystemClock.uptimeMillis();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
        Log.d("GameRenderer", "GL Surface changed!");

        on_surface_changed(width, height);
        on_resume();
    }

    @Override
    public void onDrawFrame(GL10 gl)
    {
        float currTimePick_ms = (float) SystemClock.uptimeMillis();

        int requestedAction = get_requested_action();
        if (requestedAction >= 1000)
        {
            requestedAction /= 1000;
        }

        switch (requestedAction)
        {
            case REQUESTED_ACTION_UPDATE:
                if (!_isDoingIO)
                {
                    update((currTimePick_ms - _lastRealTimeMeasurement_ms) / 1000);
                }
                break;
            case REQUESTED_ACTION_LEVEL_EDITOR_SAVE:
                saveLevel(get_requested_action());
                clear_requested_action();
                break;
            case REQUESTED_ACTION_LEVEL_EDITOR_LOAD:
                loadLevel(get_requested_action());
                clear_requested_action();
                break;
            default:
                break;
        }

        render();
        handleSound();
        handleMusic();

        _lastRealTimeMeasurement_ms = currTimePick_ms;

        _fpsCounter.logFrame();
    }

    public void onResume()
    {
        Log.d("GameRenderer", "onResume");

        on_resume();
    }

    public void onPause()
    {
        Log.d("GameRenderer", "onPause");

        on_pause();

        if (_bgm != null && _bgm.isPlaying())
        {
            _bgm.pause();
        }
    }

    public void handleTouchDown(float rawX, float rawY)
    {
        on_touch_down(rawX, rawY);
    }

    public void handleTouchDragged(float rawX, float rawY)
    {
        on_touch_dragged(rawX, rawY);
    }

    public void handleTouchUp(float rawX, float rawY)
    {
        on_touch_up(rawX, rawY);
    }

    public boolean handleOnBackPressed()
    {
        return handle_on_back_pressed();
    }

    private void handleSound()
    {
        short soundId;
        while ((soundId = get_current_sound_id()) > 0)
        {
            switch (soundId)
            {
                case SOUND_COLLECT_CARROT:
                    _sounds.get(0).play(1);
                    break;
                case SOUND_COLLECT_GOLDEN_CARROT:
                    _sounds.get(1).play(1);
                    break;
                case SOUND_DEATH:
                    _sounds.get(2).play(1);
                    break;
                case SOUND_FOOTSTEP_LEFT_GRASS:
                    _sounds.get(3).play(1);
                    break;
                case SOUND_FOOTSTEP_RIGHT_GRASS:
                    _sounds.get(4).play(1);
                    break;
                case SOUND_FOOTSTEP_LEFT_CAVE:
                    _sounds.get(5).play(1);
                    break;
                case SOUND_FOOTSTEP_RIGHT_CAVE:
                    _sounds.get(6).play(1);
                    break;
                case SOUND_JUMP_SPRING:
                    _sounds.get(7).play(1);
                    break;
                case SOUND_LANDING_GRASS:
                    _sounds.get(8).play(1);
                    break;
                case SOUND_LANDING_CAVE:
                    _sounds.get(9).play(1);
                    break;
                case SOUND_BREAK_LOG:
                    _sounds.get(10).play(1);
                    break;
                case SOUND_DESTROY_ROCK:
                    _sounds.get(11).play(1);
                    break;
                case SOUND_SNAKE_DEATH:
                    _sounds.get(12).play(1);
                    break;
                case SOUND_TRIGGER_TRANSFORM:
                    _sounds.get(13).play(1);
                    break;
                case SOUND_CANCEL_TRANSFORM:
                    _sounds.get(14).play(1);
                    break;
                case SOUND_COMPLETE_TRANSFORM:
                    _sounds.get(15).play(1);
                    break;
                case SOUND_JUMP_SPRING_HEAVY:
                    _sounds.get(16).play(1);
                    break;
                case SOUND_JON_RABBIT_JUMP:
                    _sounds.get(17).play(1);
                    break;
                case SOUND_JON_VAMPIRE_JUMP:
                    _sounds.get(18).play(1);
                    break;
                case SOUND_JON_RABBIT_DOUBLE_JUMP:
                    _sounds.get(19).play(1);
                    break;
                case SOUND_JON_VAMPIRE_DOUBLE_JUMP:
                    _sounds.get(20).play(1);
                    break;
                case SOUND_JON_VAMPIRE_GLIDE:
                    _sounds.get(21).play(1, true);
                    break;
                case SOUND_STOP_JON_VAMPIRE_GLIDE:
                    _sounds.get(21).stop();
                    break;
                default:
                    break;
            }
        }
    }

    private void handleMusic()
    {
        short musicId = get_current_music_id();
        switch (musicId)
        {
            case MUSIC_STOP:
                if (_bgm != null)
                {
                    _bgm.stop();
                    _bgm = null;
                }
                break;
            case MUSIC_RESUME:
                _bgm.play();
                break;
            case MUSIC_PLAY_DEMO:
                _bgm = _audio.newMusic("bgm.wav");
                _bgm.setLooping(true);
                _bgm.setVolume(0.5f);
                _bgm.play();
                break;
            default:
                break;
        }
    }

    private void saveLevel(final int requestedAction)
    {
        final String levelFileName = getLevelName(requestedAction);

        File file = _fileHandler.getFile(levelFileName);
        _isDoingIO = true;
        final boolean result = save_level(file.getAbsolutePath());
        _isDoingIO = false;

        _activity.runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                Toast.makeText(_activity, result ? "Level saved successfully" : "Error occurred while saving level... Please try again!", Toast.LENGTH_SHORT).show();
            }
        });
    }

    private void loadLevel(final int requestedAction)
    {
        final String levelFileName = getLevelName(requestedAction);

        _isDoingIO = true;

        _activity.runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                new AsyncTask<Void, Void, String>()
                {
                    @Override
                    protected String doInBackground(Void... params)
                    {
                        return _fileHandler.readFromFile(levelFileName);
                    }

                    @Override
                    protected void onPostExecute(String result)
                    {
                        Toast.makeText(_activity, result == null ? "Error occurred while loading level..." : "Level loaded successfully", Toast.LENGTH_SHORT).show();

                        if (result != null)
                        {
                            load_level(result);
                        }

                        _isDoingIO = false;
                    }
                }.execute();
            }
        });
    }

    private String getLevelName(int requestedAction)
    {
        int world = 0;
        int level = 0;
        while (requestedAction >= 1000)
        {
            requestedAction -= 1000;
        }

        while (requestedAction >= 100)
        {
            requestedAction -= 100;
            world++;
        }

        while (requestedAction >= 1)
        {
            requestedAction--;
            level++;
        }

        if (world > 0 && level > 0)
        {
            return String.format("nosfuratu_c%d_l%d.json", world, level);
        }
        else
        {
            return "nosfuratu.json";
        }
    }

    public final class FPSCounter
    {
        long startTime = System.nanoTime();
        int frames = 0;

        public void logFrame()
        {
            frames++;
            if (System.nanoTime() - startTime >= 1000000000)
            {
                Log.d("FPSCounter", "fps: " + frames);
                frames = 0;
                startTime = System.nanoTime();
            }
        }
    }

    private static native void init();

    private static native void on_surface_created();

    private static native void on_surface_changed(int pixelWidth, int pixelHeight);

    private static native void on_resume();

    private static native void on_pause();

    private static native void update(float deltaTime);

    private static native void render();

    private static native void on_touch_down(float normalized_x, float normalized_y);

    private static native void on_touch_dragged(float normalized_x, float normalized_y);

    private static native void on_touch_up(float normalized_x, float normalized_y);

    private static native short get_current_music_id();

    private static native short get_current_sound_id();

    private static native int get_requested_action();

    private static native void clear_requested_action();

    private static native boolean handle_on_back_pressed();

    private static native void load_level(String level_json);

    private static native boolean save_level(String json_file_path);
}