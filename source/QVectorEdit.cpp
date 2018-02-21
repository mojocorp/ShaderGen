#include "QVectorEdit.h"
#include <QDoubleSpinBox>
#include <QHBoxLayout>

#include <limits>

QVectorEdit::QVectorEdit(QWidget* parent)
  : QWidget(parent)
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

    setValue(QVector4D(0.f, 0.f, 0.f, 0.f));

    connect(m_x, SIGNAL(valueChanged(double)), SLOT(onValueChange()));
    connect(m_y, SIGNAL(valueChanged(double)), SLOT(onValueChange()));
    connect(m_z, SIGNAL(valueChanged(double)), SLOT(onValueChange()));
    connect(m_w, SIGNAL(valueChanged(double)), SLOT(onValueChange()));
}

void
QVectorEdit::setNumFields(int n)
{
    m_x->setVisible(n > 0);
    m_y->setVisible(n > 1);
    m_z->setVisible(n > 2);
    m_w->setVisible(n > 3);
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
