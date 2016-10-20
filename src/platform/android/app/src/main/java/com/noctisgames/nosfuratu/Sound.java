package com.noctisgames.nosfuratu;

import android.media.SoundPool;

public final class Sound
{
    private final SoundPool _soundPool;
    private final int _soundID;

    private int _streamID;

    public Sound(SoundPool soundPool, int soundID)
    {
        _soundPool = soundPool;
        _soundID = soundID;
    }

    public void play(float volume)
    {
        play(volume, false);
    }

    public void play(float volume, boolean isLooping)
    {
        _streamID = _soundPool.play(_soundID, volume, volume, 0, isLooping ? -1 : 0, 1);
    }

    public void pause()
    {
        _soundPool.pause(_streamID);
    }

    public void resume()
    {
        _soundPool.resume(_streamID);
    }

    public void stop()
    {
        _soundPool.stop(_streamID);
    }

    public void dispose()
    {
        _soundPool.unload(_soundID);
    }
}