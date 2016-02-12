#!/usr/bin/python
from PIL import Image
import os, sys

path = "../../../assets/textures/"
dirs = os.listdir(path)

if not os.path.exists("../../../assets/textures/compressed"):
    os.makedirs("../../../assets/textures/compressed")

def resize():
    for item in dirs:
        if not item.startswith("trans"):
            if os.path.isfile(path + item):
                f, e = os.path.splitext(path + item)
                if e == ".png":
                    fn, ne = os.path.splitext(item)
                    img = Image.open(path+item)
                    size = int(img.size[1] / 2);
                    imResize = img.resize((size, size), Image.ANTIALIAS)
                    imResize.save(path + 'compressed/' + 'c_' + item, 'png', quality=100)

resize()