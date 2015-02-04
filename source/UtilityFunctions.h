#pragma once

#include <QColor>

#include "Vector.h"
/******************************************************
The following two functions are used to convert color
    data back and forth between OpenGL and Qt.
******************************************************/

inline const QColor ToQtColor( const vec4& fpColor)
{
    return QColor::fromRgbF(fpColor.x, fpColor.y, fpColor.z);
}

inline const vec4 ToGLFPColor(const QColor& color)
{
    return vec4(color.redF(), color.greenF(), color.blueF(), 1.0f);
}
