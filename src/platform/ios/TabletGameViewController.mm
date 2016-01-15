//
//  TabletGameViewController.m
//  nosfuratu
//
//  Created by Stephen Gowen on 1/14/16.
//  Copyright (c) 2016 Gowen Game Dev. All rights reserved.
//

#import "TabletGameViewController.h"

// C++
#include "IOSOpenGLESGameScreen.h"

@implementation TabletGameViewController

#pragma mark <Protected>

- (IOSOpenGLESGameScreen *)getGameScreen:(CGSize)size
{
    return new IOSOpenGLESGameScreen(MAX(size.width, size.height), MIN(size.width, size.height), [UIScreen mainScreen].applicationFrame.size.width, [UIScreen mainScreen].applicationFrame.size.height, false);
}

@end
