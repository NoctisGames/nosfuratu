/*
 * Copyright (c) 2007 Dave Dribin
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#import <Cocoa/Cocoa.h>

#import <DDHidLib/DDHidLib.h>

@interface JoystickController : NSObject
{
    NSArray * mJoysticks;
    NSMutableArray * mJoystickButtons;
    float mJoystickAxis[8];
    float m_fStateTime;
    NSUInteger mJoystickIndex;
}

- (void)scan;

- (NSArray *)joysticks;

- (NSArray *)joystickButtons;

- (NSUInteger)joystickIndex;

- (void)listenOnJoystick:(NSUInteger)index;

- (void)ddhidJoystick:(DDHidJoystick *)joystick
                 stick:(unsigned)stick
              xChanged:(int)value;

- (void)ddhidJoystick:(DDHidJoystick *)joystick
                 stick:(unsigned)stick
              yChanged:(int)value;

- (void)ddhidJoystick:(DDHidJoystick *)joystick
                 stick:(unsigned)stick
             otherAxis:(unsigned)otherAxis
          valueChanged:(int)value;

- (void)ddhidJoystick:(DDHidJoystick *)joystick
            buttonDown:(unsigned)buttonNumber;

- (void)ddhidJoystick:(DDHidJoystick *)joystick
              buttonUp:(unsigned)buttonNumber;

@end
