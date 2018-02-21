#pragma once

#include <QVector3D>
#include <QVector4D>
#include <QWidget>

class QDoubleSpinBox;

class QVectorEdit : public QWidget
{
    Q_OBJECT
  public:
    QVectorEdit(QWidget* parent = 0);

    QVector4D getValue() const;

    QSize sizeHint() const;
  public slots:
    void setNumFields(int n);
    void setValue(const QVector4D& vec);
  signals:
    void valueChanged();
  private slots:
    void onValueChange();

  private:
    QDoubleSpinBox* m_x;
    QDoubleSpinBox* m_y;
    QDoubleSpinBox* m_z;
    QDoubleSpinBox* m_w;
};
