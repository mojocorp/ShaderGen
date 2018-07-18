#include "QColorButton.h"
#include <QColorDialog>
#include <QPainter>
#include <QStyle>
#include <QStyleOptionButton>

QColorButton::QColorButton(QWidget* parent)
  : QPushButton(parent)
{
    setFixedSize(70, 32);
}

QColor
QColorButton::color() const
{
    return m_color;
}

void
QColorButton::setColor(const QColor& color)
{
    m_color = color;

    update();
}

void
QColorButton::paintEvent(QPaintEvent* event)
{
    QPushButton::paintEvent(event);

    QStyleOptionButton option;
    option.initFrom(this);
    const QRect rect = style()->subElementRect(QStyle::SE_PushButtonContents, &option, this);
    QPainter p(this);
    p.setPen(Qt::black);
    p.setBrush(m_color);
    p.drawRect(rect.adjusted(-2, 3, 1, -3));
}

void
QColorButton::nextCheckState()
{
    QPushButton::nextCheckState();

    QColor savedColor = m_color;

    QColorDialog* dialog = new QColorDialog(this);
    dialog->setCurrentColor(m_color);
    connect(dialog, SIGNAL(currentColorChanged(QColor)), SLOT(setColor(const QColor&)));

    if (dialog->exec() == QDialog::Accepted) {
        emit selected(m_color);
    } else {
        setColor(savedColor);
    }
    delete dialog;
}
