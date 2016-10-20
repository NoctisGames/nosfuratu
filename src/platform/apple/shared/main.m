//
//  main.m
//  nosfuratu
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#ifdef NG_IOS
#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#elif defined NG_MAC
#import <Cocoa/Cocoa.h>
#endif

int main(int argc, char * argv[])
{
#ifdef NG_IOS
    @autoreleasepool
    {
        return UIApplicationMain(argc, argv, nil, NSStringFromClass([AppDelegate class]));
    }
#else
    return NSApplicationMain(argc, (const char**)argv);
#endif
}
