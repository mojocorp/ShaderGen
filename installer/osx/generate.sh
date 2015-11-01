#!/bin/bash

qmake -spec macx-g++ -r "CONFIG+=release"

make clean && make

(cd build && macdeployqt ShaderGen.app -no-plugins)

mkdir dist
cp -r build/ShaderGen.app dist
ln -s /Applications dist/Applications
cp License.txt dist 

hdiutil create \
        -volname "ShaderGen" \
        -srcfolder ./dist \
        -ov \
        ShaderGen-3.3.0.dmg
