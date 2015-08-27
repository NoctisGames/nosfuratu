package com.gowengamedev.nosfuratu;

import android.app.Activity;
import android.opengl.GLSurfaceView.Renderer;
import android.os.SystemClock;
import android.util.Log;

import com.gowengamedev.nosfuratu.platform.PlatformFileUtils;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public final class GameRenderer implements Renderer
{
    private static final float movAveragePeriod = 40; // #frames involved in average calc (suggested values 5-100)
    private static final float smoothFactor = 0.1f; // adjusting ratio (suggested values 0.01-0.5)

    static
    {
        System.loadLibrary("game");
    }

    private final Activity activity;
    private final int deviceScreenWidth;
    private final int deviceScreenHeight;

    private float smoothedDeltaRealTime_ms = 17.5f;
    private float movAverageDeltaTime_ms = smoothedDeltaRealTime_ms;
    private long lastRealTimeMeasurement_ms;
    private boolean isInitialized;

    public GameRenderer(Activity activity, int deviceScreenWidth, int deviceScreenHeight)
    {
        this.activity = activity;
        this.deviceScreenWidth = deviceScreenWidth;
        this.deviceScreenHeight = deviceScreenHeight;
        this.isInitialized = false;

        init();
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config)
    {
        Log.d("RendererWrapper", "GL Surface created!");

        if (!isInitialized)
        {
            PlatformFileUtils.init_asset_manager(activity.getAssets());
            isInitialized = true;
        }

        on_surface_created(deviceScreenWidth, deviceScreenHeight);
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
        update(smoothedDeltaRealTime_ms / 1000);
        render();
        handleSound();
        handleMusic();

        // Moving average calc
        long currTimePick_ms = SystemClock.uptimeMillis();
        float realTimeElapsed_ms;
        if (lastRealTimeMeasurement_ms > 0)
        {
            realTimeElapsed_ms = (currTimePick_ms - lastRealTimeMeasurement_ms);
        }
        else
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
        // TODO
    }

    private void handleMusic()
    {
        // TODO
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