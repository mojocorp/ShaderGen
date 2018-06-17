#!/bin/bash

if [ ! -f "linuxdeployqt.AppImage" ]; then
  curl -L "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage" > linuxdeployqt.AppImage
  chmod a+x linuxdeployqt.AppImage
fi

cp source/ShaderGen.png build/icon.png
cp installer/linux/* build/
./linuxdeployqt.AppImage build/ShaderGen -appimage -bundle-non-qt-libs
mv Application-*.AppImage ShaderGen-linux-x86_64.AppImage
