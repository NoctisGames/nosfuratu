package com.noctisgames.nosfuratu;

import android.app.NativeActivity;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.RequiresApi;
import android.view.View;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;

import static com.noctisgames.nosfuratu.BuildConfig.DEBUG;

public final class MainNativeActivity extends NativeActivity
{
    private InterstitialAd mInterstitialAd;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        //Hide toolbar
        int SDK_INT = android.os.Build.VERSION.SDK_INT;
        if (SDK_INT >= 19)
        {
            setImmersiveSticky();

            View decorView = getWindow().getDecorView();
            decorView.setOnSystemUiVisibilityChangeListener(new View.OnSystemUiVisibilityChangeListener()
            {
                @RequiresApi(api = Build.VERSION_CODES.KITKAT)
                @Override
                public void onSystemUiVisibilityChange(int visibility)
                {
                    setImmersiveSticky();
                }
            });
        }
    }

    @Override
    protected void onResume()
    {
        super.onResume();

        //Hide toolbar
        int SDK_INT = android.os.Build.VERSION.SDK_INT;
        if (SDK_INT >= 11 && SDK_INT < 14)
        {
            getWindow().getDecorView().setSystemUiVisibility(View.STATUS_BAR_HIDDEN);
        }
        else if (SDK_INT >= 14 && SDK_INT < 19)
        {
            getWindow().getDecorView().setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN | View.SYSTEM_UI_FLAG_LOW_PROFILE);
        }
        else if (SDK_INT >= 19)
        {
            setImmersiveSticky();
        }
    }

    // Called from C++
    public void initializeAds()
    {
        mInterstitialAd = new InterstitialAd(this);
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
    }

    // Called from C++
    public void displayInterstitialAdIfLoaded()
    {
        runOnUiThread(new Runnable()
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

    @RequiresApi(api = Build.VERSION_CODES.KITKAT)
    private void setImmersiveSticky()
    {
        View decorView = getWindow().getDecorView();
        decorView.setSystemUiVisibility(View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_STABLE);
    }
}
