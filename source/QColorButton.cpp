#include "QColorButton.h"
#include <QColorDialog>
#include <QPainter>

QColorButton::QColorButton(QWidget* parent)
  : QPushButton(parent)
{
    setFixedSize(45, 23);
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

    QPainter p(this);
    p.setPen(Qt::black);
    p.setBrush(m_color);
    p.drawRect(5, 5, 34, 12);
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
