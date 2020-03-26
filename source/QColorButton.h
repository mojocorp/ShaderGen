#pragma once

#include <QPushButton>

class QColorButton : public QPushButton
{
    Q_OBJECT
  public:
    QColorButton(QWidget* parent = nullptr);

    //! Returns the currently selected color.
    QColor color() const;

  public slots:
    //! Sets the currently selected color.
    void setColor(const QColor& color);

  signals:
    void selected(const QColor& color);

  protected:
    void paintEvent(QPaintEvent*);
    void nextCheckState();

  private:
    QColor m_color;
};
