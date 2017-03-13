package com.noctisgames.nosfuratu;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.media.AudioManager;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.LinearLayout;

public final class MainActivity extends Activity
{
    private MainRenderer _mainRenderer;
    private GLSurfaceView _glSurfaceView;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);

        setContentView(R.layout.activity_game);

        _mainRenderer = new MainRenderer(this);
        _glSurfaceView = new GLSurfaceView(this);
        _glSurfaceView.setEGLContextClientVersion(2);
        _glSurfaceView.setRenderer(_mainRenderer);

        LinearLayout gameContainer = (LinearLayout) findViewById(R.id.game);
        gameContainer.addView(_glSurfaceView);

        setVolumeControlStream(AudioManager.STREAM_MUSIC);

        _glSurfaceView.setOnTouchListener(new OnTouchListener()
        {
            @SuppressLint("ClickableViewAccessibility")
            @Override
            public boolean onTouch(View v, final MotionEvent event)
            {
                synchronized (this)
                {
                    int action = event.getAction() & MotionEvent.ACTION_MASK;
                    int pointerIndex = (event.getAction() & MotionEvent.ACTION_POINTER_INDEX_MASK) >> MotionEvent.ACTION_POINTER_INDEX_SHIFT;

                    switch (action)
                    {
                        case MotionEvent.ACTION_DOWN:
                        case MotionEvent.ACTION_POINTER_DOWN:
                            _mainRenderer.handleTouchDown(event.getX(pointerIndex), event.getY(pointerIndex));
                            break;
                        case MotionEvent.ACTION_UP:
                        case MotionEvent.ACTION_POINTER_UP:
                        case MotionEvent.ACTION_CANCEL:
                            _mainRenderer.handleTouchUp(event.getX(pointerIndex), event.getY(pointerIndex));
                            break;
                        case MotionEvent.ACTION_MOVE:
                            for (int i = 0; i < event.getPointerCount(); i++)
                            {
                                pointerIndex = i;
                                _mainRenderer.handleTouchDragged(event.getX(pointerIndex), event.getY(pointerIndex));
                            }
                            break;
                    }

                    return true;
                }
            }
        });
    }

    @Override
    protected void onResume()
    {
        super.onResume();

        _glSurfaceView.onResume();
        _mainRenderer.onResume();
    }

    @Override
    protected void onPause()
    {
        _mainRenderer.onPause();
        _glSurfaceView.onPause();

        super.onPause();
    }

    @Override
    protected void onDestroy()
    {
        _mainRenderer.onDestroy();

        super.onDestroy();
    }

    @Override
    public void onBackPressed()
    {
        if (_mainRenderer.handleOnBackPressed())
        {
            return;
        }

        super.onBackPressed();
    }
}
