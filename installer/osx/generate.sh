#!/bin/bash

cd ../..

qmake -spec macx-g++ -r "CONFIG+=release"

make clean && make

cd build

#macdeployqt ShaderGen.app -dmg -no-plugins
macdeployqt ShaderGen.app -dmg -no-plugins

mv ShaderGen.dmg ShaderGen-3.1.1.dmg
