package com.noctisgames.nosfuratu.sound;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetFileDescriptor;
import android.media.AudioManager;
import android.os.Build;
import android.util.Log;

import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public final class SoundManager
{
    public static native void init_sound_manager(String apk_path, int sample_rate, int buffer_size);

    public static native void load_and_play_sound(int rawResourceId, int fileOffset, int fileLength, float volume, boolean isLooping);

    public static native void stop_sound(int rawResourceId);

    public static native void set_music_volume(float volume);

    public static native void resume_music();

    public static native void pause_music();

    private final List<Sound> _musics = new ArrayList<>();
    private final List<Sound> _sounds = new ArrayList<>();

    public SoundManager(Activity activity)
    {
        // Get the device's sample rate and buffer size to enable low-latency Android audio output, if available.
        String sampleRateString = null;
        String bufferSizeString = null;

        if (Build.VERSION.SDK_INT >= 17)
        {
            AudioManager audioManager = (AudioManager) activity.getSystemService(Context.AUDIO_SERVICE);
            sampleRateString = audioManager.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE);
            bufferSizeString = audioManager.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER);
        }

        if (sampleRateString == null)
        {
            sampleRateString = "44100";
        }

        if (bufferSizeString == null)
        {
            bufferSizeString = "512";
        }

        String packageResourcePath = activity.getPackageResourcePath();
        int sampleRate = Integer.parseInt(sampleRateString);
        int bufferSize = Integer.parseInt(bufferSizeString);

        SoundManager.init_sound_manager(packageResourcePath, sampleRate, bufferSize);
    }

    public void loadMusic(Activity activity, int rawResourceId)
    {
        _musics.add(load(activity, rawResourceId));
    }

    public void loadSound(Activity activity, int rawResourceId)
    {
        _sounds.add(load(activity, rawResourceId));
    }

    public void playMusic(int rawResourceId, float volume, boolean isLooping)
    {
        for (Sound s : _musics)
        {
            if (s._rawResourceId == rawResourceId)
            {
                SoundManager.load_and_play_sound(s._rawResourceId, s._fileOffset, s._fileLength, volume, isLooping);

                return;
            }
        }
    }

    public void pauseMusic()
    {
        SoundManager.pause_music();
    }

    public void resumeMusic()
    {
        SoundManager.resume_music();
    }

    public void setMusicVolume(float volume)
    {
        SoundManager.set_music_volume(volume);
    }

    public void playSound(int soundIndex, float volume, boolean isLooping)
    {
        Sound sound = _sounds.get(soundIndex);
        SoundManager.load_and_play_sound(sound._rawResourceId, sound._fileOffset, sound._fileLength, volume, isLooping);
    }

    public void stopSound(int soundIndex)
    {
        Sound sound = _sounds.get(soundIndex);
        SoundManager.stop_sound(sound._rawResourceId);
    }

    private Sound load(Activity activity, int rawResourceId)
    {
        // Files under res/raw are not zipped, just copied into the APK. Get the offset and length to know where our files are located.
        AssetFileDescriptor fd = activity.getResources().openRawResourceFd(rawResourceId);
        int fileOffset = (int) fd.getStartOffset(), fileLength = (int) fd.getLength();
        try
        {
            fd.getParcelFileDescriptor().close();
        } catch (IOException e)
        {
            Log.e("NosFURatu", "Close error.");
        }

        return new Sound(rawResourceId, fileOffset, fileLength);
    }
}