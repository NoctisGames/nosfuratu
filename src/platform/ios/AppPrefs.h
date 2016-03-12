//
//  AppPrefs.h
//  nosfuratu
//
//  Created by Stephen Gowen on 3/11/16.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

@interface AppPrefs : NSObject
{
    
}

+ (id)getInstance;

- (void)setString:(NSString *)value forKey:(NSString *)key;

- (void)setInt:(int)value forKey:(NSString *)key;

- (void)setBool:(BOOL)value forKey:(NSString *)key;

- (NSString *)getString:(NSString *)key;

- (int)getInt:(NSString *)key;

- (BOOL)getBool:(NSString *)key;

@end