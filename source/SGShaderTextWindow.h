#pragma once

#include <QPlainTextEdit>
#include <QTabWidget>
#include <QTextEdit>

class SGFrame;

class SGShaderTextWindow : public QFrame
{
    Q_OBJECT
  public:
    SGShaderTextWindow(SGFrame* frame);
    void setVertexShaderText(const QString& text) { m_textBoxVert->setPlainText(text); }
    void setFragmentShaderText(const QString& text) { m_textBoxFrag->setPlainText(text); }

  public slots:
    void log(const QString& text);
    void clearLog();
  private slots:
    void generateClicked();
    void compileClicked();
    void linkClicked();
    void buildClicked();

  private:
    SGFrame* m_frame;
    QPlainTextEdit *m_textBoxVert, *m_textBoxFrag;
    QTextEdit* m_textBoxInfo;
    QTabWidget* m_notebook;
};
