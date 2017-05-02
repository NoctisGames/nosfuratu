cd $SRCROOT/../../../assets/textures
rm *.h
for img in *.png; do
    filename=${img%.*}
    ./xd -d$filename $filename.png $filename.h
done

cd compressed
rm *.h
for img in *.png; do
    filename=${img%.*}
    ../xd -d$filename $filename.png $filename.h
done
cd ..

cd desktop
rm *.h
for img in *.png; do
    filename=${img%.*}
    ../xd -d$filename $filename.png $filename.h
done
cd ..

cd level_editor
rm *.h
for img in *.png; do
    filename=${img%.*}
    ../xd -d$filename $filename.png $filename.h
done
cd ..
