#!/bin/bash

qmake -spec macx-g++ -r "CONFIG+=release"

make clean && make

(cd build && macdeployqt ShaderGen.app)

mkdir dist
cp -r build/ShaderGen.app License.txt dist
rm -rf dist/ShaderGen.app/Contents/PlugIns/imageformats
rm -rf dist/ShaderGen.app/Contents/PlugIns/printsupport
ln -s /Applications dist/Applications

hdiutil create \
        -volname "ShaderGen" \
        -srcfolder ./dist \
        -ov \
        ShaderGen-3.3.0.dmg
