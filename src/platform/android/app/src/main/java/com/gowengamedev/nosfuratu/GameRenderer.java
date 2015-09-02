package com.gowengamedev.nosfuratu;

import android.content.res.AssetManager;
import android.opengl.GLSurfaceView.Renderer;
import android.os.SystemClock;
import android.util.Log;

import com.gowengamedev.nosfuratu.platform.PlatformAssetUtils;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public final class GameRenderer implements Renderer
{
    static
    {
        System.loadLibrary("game");
    }

    // Definitions from src/core/game/ResourceConstants.h
    //// Music Definitions ////

    private static final short MUSIC_STOP = 1;
    private static final short MUSIC_PLAY_DEMO = 2;

    //// Sound Definitions ////

    private static final short SOUND_DEMO = 1;

    private static final float movAveragePeriod = 40; // #frames involved in average calc (suggested values 5-100)
    private static final float smoothFactor = 0.1f; // adjusting ratio (suggested values 0.01-0.5)

    private final Audio audio;
    private Music bgm;
    private Sound explosionSound;

    private float smoothedDeltaRealTime_ms = 17.5f;
    private float movAverageDeltaTime_ms = smoothedDeltaRealTime_ms;
    private long lastRealTimeMeasurement_ms;

    public GameRenderer(AssetManager assetManager)
    {
        this.audio = new Audio(assetManager);
        this.explosionSound = audio.newSound("explosion.ogg");

        PlatformAssetUtils.init_asset_manager(assetManager);

        init();
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
        int gameState = get_state();
        switch (gameState)
        {
            case 0:
                update(smoothedDeltaRealTime_ms / 1000);
                break;
            default:
                break;
        }

        render();
        handleSound();
        handleMusic();

        // Moving average calc
        long currTimePick_ms = SystemClock.uptimeMillis();
        float realTimeElapsed_ms;
        if (lastRealTimeMeasurement_ms > 0)
        {
            realTimeElapsed_ms = (currTimePick_ms - lastRealTimeMeasurement_ms);
        } else
        {
            realTimeElapsed_ms = smoothedDeltaRealTime_ms; // just the first
            // time
        }

        movAverageDeltaTime_ms = (realTimeElapsed_ms + movAverageDeltaTime_ms * (movAveragePeriod - 1)) / movAveragePeriod;

        // Calc a better aproximation for smooth stepTime
        smoothedDeltaRealTime_ms = smoothedDeltaRealTime_ms + (movAverageDeltaTime_ms - smoothedDeltaRealTime_ms) * smoothFactor;

        lastRealTimeMeasurement_ms = currTimePick_ms;
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
                case SOUND_DEMO:
                    this.explosionSound.play(1);
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
                if (bgm != null)
                {
                    bgm.stop();
                }
                break;
            case MUSIC_PLAY_DEMO:
                if (bgm != null && bgm.isPlaying())
                {
                    bgm.dispose();
                    bgm = null;
                }

                bgm = audio.newMusic("bgm.ogg");
                bgm.setLooping(true);
                bgm.play();
                break;
            default:
                break;
        }
    }

    private static native void init();

    private static native void on_surface_created(int pixelWidth, int pixelHeight);

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

    private static native int get_state();

    private static native void clear_state();

    private static native boolean handle_on_back_pressed();
}