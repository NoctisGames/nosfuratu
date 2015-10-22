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

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public final class GameRenderer implements Renderer
{
    static
    {
        System.loadLibrary("game");
    }

    // Definitions from src/core/game/GameConstants.h
    private static final short REQUESTED_ACTION_UPDATE = 0;
    private static final short REQUESTED_ACTION_LEVEL_EDITOR_SAVE = 1;
    private static final short REQUESTED_ACTION_LEVEL_EDITOR_LOAD = 2;

    private static final short MUSIC_STOP = 1;
    private static final short MUSIC_PLAY_DEMO = 2;
    private static final short SOUND_COLLECT_CARROT = 1;
    private static final short SOUND_COLLECT_GOLDEN_CARROT = 2;

    private static final float MOV_AVERAGE_PERIOD = 5; // #frames involved in average calc (suggested values 5-100)
    private static final float SMOOTH_FACTOR = 0.1f; // adjusting ratio (suggested values 0.01-0.5)

    private final Activity _activity;
    private final FileHandler _fileHandler;
    private final Audio _audio;
    private Music _bgm;
    private Sound _collectCarrotSound;
    private Sound _collectGoldenCarrotSound;

    private float _smoothedDeltaRealTime_ms = 16.66666666667f;
    private float _movAverageDeltaTime_ms = _smoothedDeltaRealTime_ms;
    private long _lastRealTimeMeasurement_ms;
    private boolean _isDoingIO = false;

    public GameRenderer(Activity activity)
    {
        _activity = activity;
        _fileHandler = new FileHandler(new File(Environment.getExternalStorageDirectory(), "NosFURatu"));
        _audio = new Audio(activity.getAssets());
        _collectCarrotSound = _audio.newSound("collect_carrot.ogg");
        _collectGoldenCarrotSound = _audio.newSound("collect_golden_carrot.ogg");

        PlatformAssetUtils.init_asset_manager(activity.getAssets());

        init(BuildConfig.IS_LEVEL_EDITOR);
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        Log.d("RendererWrapper", "GL Surface created!");
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height)
    {
        Log.d("RendererWrapper", "GL Surface changed!");

        on_surface_changed(width, height);
        on_resume();
    }

    @Override
    public void onDrawFrame(GL10 gl)
    {
        switch (get_requested_action())
        {
            case REQUESTED_ACTION_UPDATE:
                if (!_isDoingIO)
                {
                    update(_smoothedDeltaRealTime_ms / 1000);
                }
                break;
            case REQUESTED_ACTION_LEVEL_EDITOR_SAVE:
                saveLevel();
                clear_requested_action();
                break;
            case REQUESTED_ACTION_LEVEL_EDITOR_LOAD:
                loadLevel();
                clear_requested_action();
                break;
            default:
                break;
        }

        render();
        handleSound();
        handleMusic();

        // Moving average calc
        long currTimePick_ms = SystemClock.uptimeMillis();
        float realTimeElapsed_ms = _lastRealTimeMeasurement_ms > 0 ? (currTimePick_ms - _lastRealTimeMeasurement_ms) : _smoothedDeltaRealTime_ms;

        _movAverageDeltaTime_ms = (realTimeElapsed_ms + _movAverageDeltaTime_ms * (MOV_AVERAGE_PERIOD - 1)) / MOV_AVERAGE_PERIOD;

        _smoothedDeltaRealTime_ms = _smoothedDeltaRealTime_ms + (_movAverageDeltaTime_ms - _smoothedDeltaRealTime_ms) * SMOOTH_FACTOR;

        _lastRealTimeMeasurement_ms = currTimePick_ms;
    }

    public void onResume()
    {
        on_resume();
    }

    public void onPause()
    {
        on_pause();
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
                    _collectCarrotSound.play(1);
                    break;
                case SOUND_COLLECT_GOLDEN_CARROT:
                    _collectGoldenCarrotSound.play(1);
                    break;
                default:
                    continue;
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
                }
                break;
            case MUSIC_PLAY_DEMO:
                if (_bgm != null && _bgm.isPlaying())
                {
                    _bgm.dispose();
                    _bgm = null;
                }

                _bgm = _audio.newMusic("bgm.ogg");
                _bgm.setLooping(true);
                _bgm.play();
                break;
            default:
                break;
        }
    }

    private void saveLevel()
    {
        File file = _fileHandler.getFile("nosfuratu.json");
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

    private void loadLevel()
    {
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
                        return _fileHandler.readFromFile("nosfuratu.json");
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

    private static native void init(boolean isLevelEditor);

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