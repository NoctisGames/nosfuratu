package com.gowengamedev.nosfuratu;

import android.content.res.AssetFileDescriptor;
import android.content.res.AssetManager;
import android.media.AudioManager;
import android.media.SoundPool;

import java.io.IOException;

public final class Audio
{
    private final AssetManager _assets;
    private SoundPool _soundPool;

    public Audio(AssetManager assets)
    {
        _assets = assets;
        _soundPool = new SoundPool(8, AudioManager.STREAM_MUSIC, 0);
    }

    public Music newMusic(String filename)
    {
        try
        {
            AssetFileDescriptor assetDescriptor = _assets.openFd(filename);

            return new Music(assetDescriptor);
        } catch (IOException ioException)
        {
            throw new RuntimeException("Couldn't load music '" + filename + "'" + ioException.getMessage());
        }
    }

    public Sound newSound(String filename)
    {
        try
        {
            AssetFileDescriptor assetDescriptor = _assets.openFd(filename);
            int soundID = _soundPool.load(assetDescriptor, 1);

            return new Sound(_soundPool, soundID);
        } catch (IOException ioException)
        {
            throw new RuntimeException("Couldn't load sound '" + filename + "'" + ioException.getMessage());
        }
    }

    public void reset()
    {
        _soundPool.release();
        _soundPool = null;
        _soundPool = new SoundPool(8, AudioManager.STREAM_MUSIC, 0);
    }
}