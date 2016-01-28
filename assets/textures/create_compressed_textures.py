import os
import PIL
from PIL import Image

print('Creating compressed textures for phones...\n')






os.system("python3 ../../../assets/textures/merge_images.py -f --columns 2 --height 2048 --output ../../../assets/textures/compressed_jon.png ../../../assets/textures/jon.png ../../../assets/textures/jon_ability.png ../../../assets/textures/jon_ability.png ../../../assets/textures/jon_poses.png")

print('\nResizing ../../../assets/textures/compressed_jon.png down to 2048x2048')

basewidth = 2048
img = Image.open('../../../assets/textures/compressed_jon.png')
wpercent = (basewidth/float(img.size[0]))
hsize = int((float(img.size[1])*float(wpercent)))
img = img.resize((basewidth,hsize), PIL.Image.ANTIALIAS)
img.save('../../../assets/textures/compressed_jon.png')
          

          
          
          
          
os.system("python3 ../../../assets/textures/merge_images.py -f --columns 2 --height 2048 --output ../../../assets/textures/compressed_vampire.png ../../../assets/textures/vampire.png ../../../assets/textures/vampire.png ../../../assets/textures/vampire_poses.png ../../../assets/textures/vampire_transform.png")

print('\nResizing ../../../assets/textures/compressed_vampire.png down to 2048x2048')

basewidth = 2048
img = Image.open('../../../assets/textures/compressed_vampire.png')
wpercent = (basewidth/float(img.size[0]))
hsize = int((float(img.size[1])*float(wpercent)))
img = img.resize((basewidth,hsize), PIL.Image.ANTIALIAS)
img.save('../../../assets/textures/compressed_vampire.png')






os.system("python3 ../../../assets/textures/merge_images.py -f --columns 2 --height 2048 --output ../../../assets/textures/compressed_misc.png ../../../assets/textures/title_font.png ../../../assets/textures/world_1_misc.png ../../../assets/textures/world_map.png ../../../assets/textures/title_font.png")

print('\nResizing ../../../assets/textures/compressed_misc.png down to 2048x2048')

basewidth = 2048
img = Image.open('../../../assets/textures/compressed_misc.png')
wpercent = (basewidth/float(img.size[0]))
hsize = int((float(img.size[1])*float(wpercent)))
img = img.resize((basewidth,hsize), PIL.Image.ANTIALIAS)
img.save('../../../assets/textures/compressed_misc.png')

          
          
          
          
          
os.system("python3 ../../../assets/textures/merge_images.py -f --columns 2 --height 2048 --output ../../../assets/textures/compressed_world_1_ground.png ../../../assets/textures/world_1_ground_w_cave.png ../../../assets/textures/world_1_ground_wo_cave.png ../../../assets/textures/world_1_cave.png")

print('\nResizing ../../../assets/textures/compressed_world_1_ground.png down to 2048x2048')

basewidth = 2048
img = Image.open('../../../assets/textures/compressed_world_1_ground.png')
wpercent = (basewidth/float(img.size[0]))
hsize = int((float(img.size[1])*float(wpercent)))
img = img.resize((basewidth,hsize), PIL.Image.ANTIALIAS)
img.save('../../../assets/textures/compressed_world_1_ground.png')

          
         
          
          
          
os.system("python3 ../../../assets/textures/merge_images.py -f --columns 2 --height 2048 --output ../../../assets/textures/compressed_world_1_objects.png ../../../assets/textures/world_1_objects.png ../../../assets/textures/game_objects.png ../../../assets/textures/world_1_enemies.png ../../../assets/textures/world_1_snake_cave.png")

print('\nResizing ../../../assets/textures/compressed_world_1_objects.png down to 2048x2048')

basewidth = 2048
img = Image.open('../../../assets/textures/compressed_world_1_objects.png')
wpercent = (basewidth/float(img.size[0]))
hsize = int((float(img.size[1])*float(wpercent)))
img = img.resize((basewidth,hsize), PIL.Image.ANTIALIAS)
img.save('../../../assets/textures/compressed_world_1_objects.png')