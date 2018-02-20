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
    void setVertexShaderText(const QString& text) { textBoxVert->setPlainText(text); }
    void setFragmentShaderText(const QString& text) { textBoxFrag->setPlainText(text); }
    QString getVertexShaderText() const { return textBoxVert->toPlainText(); }
    QString getFragmentShaderText() const { return textBoxFrag->toPlainText(); }
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
    bool m_haveCompiled, m_haveLinked;
    QPlainTextEdit *textBoxVert, *textBoxFrag;
    QTextEdit* textBoxInfo;
    QTabWidget* notebook;
};
