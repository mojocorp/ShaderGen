#include "QVectorEdit.h"
#include <QDoubleSpinBox>
#include <QHBoxLayout>

#include <limits>

QVectorEdit::QVectorEdit(const QVector3D& vec, QWidget* parent)
  : QWidget(parent)
{
    init(QVector4D(vec, 0.0f));

    m_w->hide();
}

QVectorEdit::QVectorEdit(const QVector4D& vec, QWidget* parent)
  : QWidget(parent)
{
    init(vec);
}

void
QVectorEdit::setValue(const QVector3D& vec)
{
    setValue(QVector4D(vec, 0.0f));
}

void
QVectorEdit::setValue(const QVector4D& vec)
{
    blockSignals(true);
    m_x->setValue(vec.x());
    m_y->setValue(vec.y());
    m_z->setValue(vec.z());
    m_w->setValue(vec.w());
    blockSignals(false);
}

QVector4D
QVectorEdit::getValue() const
{
    return QVector4D(m_x->value(), m_y->value(), m_z->value(), m_w->value());
}

QSize
QVectorEdit::sizeHint() const
{
    QSize s = m_x->sizeHint();
    return QSize(4 * 40, s.height());
}

void
QVectorEdit::onValueChange()
{
    emit valueChanged();
}

void
QVectorEdit::init(const QVector4D& vec)
{
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setSpacing(0);
    layout->setMargin(0);

    m_x = new QDoubleSpinBox(this);
    m_x->setToolTip("X");
    m_x->setMinimumWidth(40);
    m_x->setRange(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    m_x->setButtonSymbols(QAbstractSpinBox::NoButtons);

    m_y = new QDoubleSpinBox(this);
    m_y->setToolTip("Y");
    m_y->setMinimumWidth(40);
    m_y->setRange(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    m_y->setButtonSymbols(QAbstractSpinBox::NoButtons);

    m_z = new QDoubleSpinBox(this);
    m_z->setToolTip("Z");
    m_z->setMinimumWidth(40);
    m_z->setRange(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    m_z->setButtonSymbols(QAbstractSpinBox::NoButtons);

    m_w = new QDoubleSpinBox(this);
    m_w->setToolTip("W");
    m_w->setMinimumWidth(40);
    m_w->setRange(-std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
    m_w->setButtonSymbols(QAbstractSpinBox::NoButtons);

    layout->addWidget(m_x);
    layout->addWidget(m_y);
    layout->addWidget(m_z);
    layout->addWidget(m_w);

    setValue(vec);

    connect(m_x, SIGNAL(valueChanged(double)), SLOT(onValueChange()));
    connect(m_y, SIGNAL(valueChanged(double)), SLOT(onValueChange()));
    connect(m_z, SIGNAL(valueChanged(double)), SLOT(onValueChange()));
    connect(m_w, SIGNAL(valueChanged(double)), SLOT(onValueChange()));
}
