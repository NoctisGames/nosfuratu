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
    private static final short REQUESTED_ACTION_DISPLAY_INTERSTITIAL_AD = 1;

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

        switch (requestedAction)
        {
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
