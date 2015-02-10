#include "QVectorEdit.h"
#include <QHBoxLayout>
#include <QDoubleSpinBox>

#include <limits>

QVectorEdit::QVectorEdit(const QVector3D& vec, QWidget *parent) :
    QWidget(parent)
{
    init(QVector4D(vec, 0.0f));

    w->hide();
}

QVectorEdit::QVectorEdit(const QVector4D& vec, QWidget *parent) :
    QWidget(parent)
{
    init(vec);
}

void QVectorEdit::setValue(const QVector3D& vec)
{
    setValue(QVector4D(vec, 0.0f));
}

void QVectorEdit::setValue(const QVector4D& vec)
{
    blockSignals(true);
    x->setValue(vec.x());
    y->setValue(vec.y());
    z->setValue(vec.z());
    w->setValue(vec.w());
    blockSignals(false);
}

QVector4D QVectorEdit::getValue() const
{
    return QVector4D(x->value(),
                     y->value(),
                     z->value(),
                     w->value());
}

QSize QVectorEdit::sizeHint() const
{
    QSize s = x->sizeHint();
    return QSize(4*40, s.height());
}

void QVectorEdit::onValueChange()
{
    emit valueChanged();
}

void QVectorEdit::init(const QVector4D& vec)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);

    x = new QDoubleSpinBox(this);
    x->setToolTip("X");
    x->setMinimumWidth(40);
    x->setRange(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    x->setButtonSymbols(QAbstractSpinBox::NoButtons);

    y = new QDoubleSpinBox(this);
    y->setToolTip("Y");
    y->setMinimumWidth(40);
    y->setRange(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    y->setButtonSymbols(QAbstractSpinBox::NoButtons);

    z = new QDoubleSpinBox(this);
    z->setToolTip("Z");
    z->setMinimumWidth(40);
    z->setRange(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    z->setButtonSymbols(QAbstractSpinBox::NoButtons);

    w = new QDoubleSpinBox(this);
    w->setToolTip("W");
    w->setMinimumWidth(40);
    w->setRange(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    w->setButtonSymbols(QAbstractSpinBox::NoButtons);

    layout->addWidget(x);
    layout->addWidget(y);
    layout->addWidget(z);
    layout->addWidget(w);

    setValue(vec);

    connect(x, SIGNAL(valueChanged(double)), SLOT(onValueChange()));
    connect(y, SIGNAL(valueChanged(double)), SLOT(onValueChange()));
    connect(z, SIGNAL(valueChanged(double)), SLOT(onValueChange()));
    connect(w, SIGNAL(valueChanged(double)), SLOT(onValueChange()));
}
