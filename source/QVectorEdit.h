#pragma once

#include <QWidget>
#include <QVector3D>
#include <QVector4D>

class QDoubleSpinBox;

class QVectorEdit : public QWidget
{
    Q_OBJECT
public:
    QVectorEdit(const QVector3D& vec, QWidget *parent = 0);
    QVectorEdit(const QVector4D& vec, QWidget *parent = 0);

    void setValue(const QVector3D& vec);
    void setValue(const QVector4D& vec);

    QVector4D getValue() const;

    QSize sizeHint() const;
signals:
    void valueChanged();
private slots:
    void onValueChange();
private:
    void init(const QVector4D& vec);

    QDoubleSpinBox* x;
    QDoubleSpinBox* y;
    QDoubleSpinBox* z;
    QDoubleSpinBox* w;
};
