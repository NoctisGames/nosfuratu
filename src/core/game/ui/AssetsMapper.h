//
//  AssetsMapper.h
//  nosfuratu
//
//  Created by Stephen Gowen on 11/9/16.
//  Copyright © 2016 Noctis Games. All rights reserved.
//

#ifndef __nosfuratu__AssetsMapper__
#define __nosfuratu__AssetsMapper__

#include "Assets.h"

#define ASSETS_MAPPER (AssetsMapper::getInstance())

class AssetsMapper
{
public:
    static AssetsMapper* getInstance()
    {
        static AssetsMapper *instance = new AssetsMapper();
        return instance;
    }
    
    void initializeAssetsMapper();
    
    TextureRegion& findTextureRegion(std::string key, float stateTime)
    {
        auto q = m_textureRegions.find(key);
        
        if (q != m_textureRegions.end())
        {
            TextureRegion* tr = q->second;
            
            return *tr;
        }
        else
        {
            auto q2 = m_animations.find(key);
            
            assert(q2 != m_animations.end());
            
            Animation* anim = q2->second;
            
            return anim->getTextureRegion(stateTime);
        }
    }
    
    TextureRegion& findTextureRegion(std::string key)
    {
        auto q = m_textureRegions.find(key);
        
        assert(q != m_textureRegions.end());
        
        TextureRegion* tr = q->second;
        
        return *tr;
    }
    
    Animation& findAnimation(std::string key)
    {
        auto q = m_animations.find(key);
        
        assert(q != m_animations.end());
        
        Animation* anim = q->second;
        
        return *anim;
    }
    
    std::map<std::string, TextureRegion*>& getTextureRegionMap()
    {
        return m_textureRegions;
    }
    
    std::map<std::string, Animation*>& getAnimationsMap()
    {
        return m_animations;
    }
    
private:
    std::map<std::string, TextureRegion*> m_textureRegions;
    std::map<std::string, Animation*> m_animations;
    
    static Animation* createAnimation(std::string textureName, int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, int numFrames)
    {
        if (ASSETS->isUsingCompressedTextureSet()
            && textureWidth > TEXTURE_SIZE_2048
            && textureHeight > TEXTURE_SIZE_2048)
        {
            x /= 2.0;
            y /= 2.0;
            regionWidth /= 2.0;
            regionHeight /= 2.0;
            animationWidth /= 2.0;
            animationHeight /= 2.0;
            textureWidth /= 2.0;
            textureHeight /= 2.0;
        }
        
        return new Animation(textureName, x, y, regionWidth, regionHeight, animationWidth, animationHeight, textureWidth, textureHeight, looping, numFrames);
    }
    
    static Animation* createAnimation(std::string textureName, int x, int y, int regionWidth, int regionHeight, int animationWidth, int animationHeight, int textureWidth, int textureHeight, bool looping, float frameTime, int numFrames, int firstLoopingFrame = 0, int yPadding = 0)
    {
        if (ASSETS->isUsingCompressedTextureSet()
            && textureWidth > TEXTURE_SIZE_2048
            && textureHeight > TEXTURE_SIZE_2048)
        {
            x /= 2.0;
            y /= 2.0;
            regionWidth /= 2.0;
            regionHeight /= 2.0;
            animationWidth /= 2.0;
            animationHeight /= 2.0;
            textureWidth /= 2.0;
            textureHeight /= 2.0;
            yPadding /= 2.0;
        }
        
        return new Animation(textureName, x, y, regionWidth, regionHeight, animationWidth, animationHeight, textureWidth, textureHeight, looping, frameTime, numFrames, firstLoopingFrame, yPadding);
    }
    
    static TextureRegion* createTextureRegion(std::string textureName, int x, int y, int regionWidth, int regionHeight, int textureWidth, int textureHeight)
    {
        if (ASSETS->isUsingCompressedTextureSet()
            && textureWidth > TEXTURE_SIZE_2048
            && textureHeight > TEXTURE_SIZE_2048)
        {
            x /= 2.0;
            y /= 2.0;
            regionWidth /= 2.0;
            regionHeight /= 2.0;
            textureWidth /= 2.0;
            textureHeight /= 2.0;
        }
        
        return new TextureRegion(textureName, x, y, regionWidth, regionHeight, textureWidth, textureHeight);
    }
    
    // ctor, copy ctor, and assignment should be private in a Singleton
    AssetsMapper() {}
    AssetsMapper(const AssetsMapper&);
    AssetsMapper& operator=(const AssetsMapper&);
};

#endif /* defined(__nosfuratu__AssetsMapper__) */
