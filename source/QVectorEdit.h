#pragma once

#include <QWidget>
#include "Vector.h"

class QDoubleSpinBox;

class QVectorEdit : public QWidget
{
    Q_OBJECT
public:
    QVectorEdit(const vec3& vec, QWidget *parent = 0);
    QVectorEdit(const vec4& vec, QWidget *parent = 0);

    void setValue(const vec3& vec);
    void setValue(const vec4& vec);

    vec4 getValue() const;

    QSize sizeHint() const;
signals:
    void valueChanged();
private slots:
    void onValueChange();
private:
    void init(const vec4& vec);

    QDoubleSpinBox* x;
    QDoubleSpinBox* y;
    QDoubleSpinBox* z;
    QDoubleSpinBox* w;
};
