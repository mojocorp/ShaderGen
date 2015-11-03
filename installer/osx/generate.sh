#!/bin/bash

qmake ShaderGen.pro -r -spec macx-clang CONFIG+=x86_64

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
        ShaderGen-3.3.1.dmg
