#!/bin/bash
echo "Input directory: $1"
echo "Output directory: $2"
for f in `ls $1/friends_*.ts`
do
    filename=$(basename "$f")
    echo "Translating $filename"
    name="${filename%.*}"

    lrelease -idbased $f -qm $2/$name.qm
done
