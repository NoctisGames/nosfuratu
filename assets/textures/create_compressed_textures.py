#!/usr/bin/python
from PIL import Image
import os, sys

path = "../../../assets/textures/"
dirs = os.listdir(path)

if not os.path.exists("../../../assets/textures/compressed"):
    os.makedirs("../../../assets/textures/compressed")

def resize():
    for item in dirs:
        if os.path.isfile(path+item):
            f, e = os.path.splitext(path+item)
            if e == ".png":
                fn, ne = os.path.splitext(item)
                img = Image.open(path+item)
                size = int(img.size[1] / 2);
                imResize = img.resize((size, size), Image.ANTIALIAS)
                imResize.save(path + 'compressed/' + 'c_' + item, 'png', quality=100)

resize()




#import os
#import PIL
#from PIL import Image
#
#print('Creating compressed textures for phones...\n')
#
#print('\nResizing ../../../assets/textures/game_objects.png down to 1024x1024')
#
#basewidth = 1024
#img = Image.open('../../../assets/textures/game_objects.png')
#wpercent = (basewidth/float(img.size[0]))
#hsize = int((float(img.size[1])*float(wpercent)))
#img = img.resize((basewidth,hsize), PIL.Image.ANTIALIAS)
#img.save('../../../assets/textures/c_game_objects.png')