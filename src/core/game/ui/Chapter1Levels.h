//
//  Chapter1Levels.h
//  nosfuratu
//
//  Created by Stephen Gowen on 1/27/16.
//  Copyright © 2016 Gowen Game Dev. All rights reserved.
//

#ifndef __nosfuratu__Chapter1Levels__
#define __nosfuratu__Chapter1Levels__

#include "GameScreenStates.h"

class Chapter1Level1 : public GamePlay
{
public:
    static Chapter1Level1* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level1(const char* json) : GamePlay(json) {};
    Chapter1Level1(const Chapter1Level1&);
    Chapter1Level1& operator=(const Chapter1Level1&);
};

class Chapter1Level2 : public GamePlay
{
public:
    static Chapter1Level2* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level2(const char* json) : GamePlay(json) {};
    Chapter1Level2(const Chapter1Level2&);
    Chapter1Level2& operator=(const Chapter1Level2&);
};

class Chapter1Level3 : public GamePlay
{
public:
    static Chapter1Level3* getInstance();
    
private:
    // ctor, copy ctor, and assignment should be private in a Singleton
    Chapter1Level3(const char* json) : GamePlay(json) {};
    Chapter1Level3(const Chapter1Level3&);
    Chapter1Level3& operator=(const Chapter1Level3&);
};

#endif /* defined(__nosfuratu__Chapter1Levels__) */
