package com.noctisgames.nosfuratu;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.NativeActivity;
import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.RequiresApi;
import android.view.View;
import android.view.WindowManager;

import com.google.android.gms.ads.AdListener;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.InterstitialAd;
import com.google.android.gms.common.api.GoogleApiClient;
import com.google.android.gms.games.Games;

import static com.noctisgames.nosfuratu.BuildConfig.DEBUG;
import static com.noctisgames.nosfuratu.GameHelper.RC_UNUSED;

public final class MainNativeActivity extends NativeActivity implements GameHelper.GameHelperListener
{
    private static final int REQUESTED_CLIENTS = GameHelper.CLIENT_GAMES | GameHelper.CLIENT_PLUS;

    private InterstitialAd _interstitialAd;
    private GameHelper _helper;

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

        if (_helper == null)
        {
            getGameHelper();
        }

        _helper.setup(this);

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }

    @Override
    protected void onStart()
    {
        super.onStart();

        _helper.onStart(this);
    }

    @Override
    protected void onStop()
    {
        super.onStop();

        _helper.onStop();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data)
    {
        super.onActivityResult(requestCode, resultCode, data);

        _helper.onActivityResult(requestCode, resultCode, data);
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

    @Override
    public void onSignInFailed()
    {
        // Empty
    }

    @Override
    public void onSignInSucceeded()
    {
        // Empty
    }

    // Called from C++
    public void initializeInterstitialAds()
    {
//        runOnUiThread(new Runnable()
//        {
//            @Override
//            public void run()
//            {
//                _interstitialAd = new InterstitialAd(MainNativeActivity.this);
//                _interstitialAd.setAdUnitId(DEBUG ? "ca-app-pub-3940256099942544/1033173712" : "ca-app-pub-6017554042572989/7036617356");
//                _interstitialAd.setAdListener(new AdListener()
//                {
//                    @Override
//                    public void onAdClosed()
//                    {
//                        requestNewInterstitial();
//                    }
//                });
//
//                requestNewInterstitial();
//            }
//        });
    }

    // Called from C++
    public void displayInterstitialAdIfLoaded()
    {
//        if (_interstitialAd == null)
//        {
//            initializeInterstitialAds();
//        }
//
//        runOnUiThread(new Runnable()
//        {
//            @Override
//            public void run()
//            {
//                if (_interstitialAd.isLoaded())
//                {
//                    _interstitialAd.show();
//                }
//            }
//        });
    }

    public boolean isSignedIn()
    {
        return _helper.isSignedIn();
    }

    public void beginUserInitiatedSignIn()
    {
        _helper.beginUserInitiatedSignIn();
    }

    public void signOut()
    {
        _helper.signOut();
    }

    public void showAchievements()
    {
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                if (isSignedIn())
                {
                    startActivityForResult(Games.Achievements.getAchievementsIntent(getApiClient()), RC_UNUSED);
                }
                else
                {
                    makeSimpleDialog(getString(R.string.achievements_not_available)).show();
                }
            }
        });
    }

    public void showLeaderboards()
    {
        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                if (isSignedIn())
                {
                    startActivityForResult(Games.Leaderboards.getAllLeaderboardsIntent(getApiClient()), RC_UNUSED);
                }
                else
                {
                    makeSimpleDialog(getString(R.string.leaderboards_not_available)).show();
                }
            }
        });
    }

    public void unlockAchievement(String key)
    {
        Games.Achievements.reveal(getApiClient(), key);
        Games.Achievements.unlock(getApiClient(), key);
    }

    public void submitScore(final String key, int score)
    {
        Games.Leaderboards.submitScore(getApiClient(), key, score);
    }

    public void submitAndDisplayScore(final String key, int score)
    {
        Games.Leaderboards.submitScore(getApiClient(), key, score);

        runOnUiThread(new Runnable()
        {
            @Override
            public void run()
            {
                if (isSignedIn())
                {
                    startActivityForResult(Games.Leaderboards.getLeaderboardIntent(getApiClient(), key), RC_UNUSED);
                }
            }
        });
    }

    public String getStringResource(String key)
    {
        String packageName = getPackageName();
        int resId = getResources().getIdentifier(key, "string", packageName);
        return getString(resId);
    }

    private void requestNewInterstitial()
    {
//        AdRequest adRequest = new AdRequest.Builder()
//                .addTestDevice("SEE_YOUR_LOGCAT_TO_GET_YOUR_DEVICE_ID")
//                .build();
//
//        _interstitialAd.loadAd(adRequest);
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

    private GameHelper getGameHelper()
    {
        if (_helper == null)
        {
            _helper = new GameHelper(this, REQUESTED_CLIENTS);
            _helper.enableDebugLog(DEBUG);
        }
        return _helper;
    }

    private GoogleApiClient getApiClient()
    {
        return _helper.getApiClient();
    }

    /**
     * Create a simple {@link Dialog} with an 'OK' button and a message.
     *
     * @param text the message to display on the Dialog.
     * @return an instance of {@link AlertDialog}
     */
    private Dialog makeSimpleDialog(String text)
    {
        return new AlertDialog.Builder(this).setMessage(text).setNeutralButton(android.R.string.ok, null).create();
    }
}
