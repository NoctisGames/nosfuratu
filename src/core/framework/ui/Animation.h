//
//  Animation.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 9/3/15.
//  Copyright (c) 2015 Gowen Game Dev. All rights reserved.
//

#ifndef __gowengamedev__Animation__
#define __gowengamedev__Animation__

#include <vector>

class TextureRegion;

class Animation
{
public:
    Animation(int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, int numFrames, ...);
    
    Animation(int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, float frameTime, int numFrames);
    
    TextureRegion& getTextureRegion(float stateTime, float scalar);
    
    TextureRegion& getTextureRegion(float stateTime);
    
    TextureRegion& getTextureRegion(int keyFrameNumber);
    
    ~Animation();
    
private:
    std::vector<TextureRegion> m_textureRegions;
    std::vector<float> m_frameTimes;
    float m_cycleTime;
    bool m_looping;
    
    void loadTextureRegions(int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, int numFrames);
    
    int getKeyFrameNumber(float stateTime);
};

#endif /* defined(__gowengamedev__Animation__) */
