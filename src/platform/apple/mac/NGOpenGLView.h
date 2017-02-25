//
//  NGOpenGLView.h
//  nosfuratu
//
//  Created by Stephen Gowen on 2/25/17.
//  Copyright (c) 2016 Noctis Games. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <QuartzCore/CVDisplayLink.h>

@interface NGOpenGLView : NSOpenGLView <NSWindowDelegate>
{
	CVDisplayLinkRef displayLink;
}

@end
