package com.noctisgames.nosfuratu.sound;

public final class Sound
{
    final int _rawResourceId;
    final int _fileOffset;
    final int _fileLength;

    public Sound(int rawResourceId, int fileOffset, int fileLength)
    {
        _rawResourceId = rawResourceId;
        _fileOffset = fileOffset;
        _fileLength = fileLength;
    }
}