#!/bin/sh
if [ $# != 1 ] 
then
    echo "Last version: $(git describe --abbrev=0 --tags)"
    echo "Use release.sh <version-number> to release a newer version"
    exit 1
fi

echo "Releasing using version $1"
echo "VERSION=$1" > version.pri

git commit -a -m "Bump version to $1"
git push origin master

git tag $1
git push origin $1
exit 0