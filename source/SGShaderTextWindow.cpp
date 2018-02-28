#include <QHBoxLayout>
#include <QPushButton>
#include <QVBoxLayout>

#include "QCodeEditor.h"
#include "SGCanvas.h"
#include "SGFrame.h"
#include "SGShaderTextWindow.h"

SGShaderTextWindow::SGShaderTextWindow(SGFrame* frame)
  : QFrame(frame)
  , m_frame(frame)
{
    m_notebook = new QTabWidget(this);

    m_textBoxVert = new QCodeEditor(m_notebook);
    m_textBoxFrag = new QCodeEditor(m_notebook);
    m_textBoxInfo = new QTextEdit(m_notebook);

    QFont fixedFont("Monospace");
    fixedFont.setStyleHint(QFont::TypeWriter);

    m_textBoxVert->setFont(fixedFont);
    m_textBoxFrag->setFont(fixedFont);
    m_textBoxInfo->setFont(fixedFont);

    m_notebook->addTab(m_textBoxVert, "Vertex Shader");
    m_notebook->addTab(m_textBoxFrag, "Fragment Shader");
    m_notebook->addTab(m_textBoxInfo, "InfoLog");

    QVBoxLayout* topSizer = new QVBoxLayout();
    QHBoxLayout* button_sizer = new QHBoxLayout();

    QPushButton* pb;
    pb = new QPushButton("1. GENERATE SHADERS", this);
    connect(pb, SIGNAL(clicked()), SLOT(generateClicked()));
    button_sizer->addWidget(pb);

    pb = new QPushButton("2. COMPILE", this);
    connect(pb, SIGNAL(clicked()), SLOT(compileClicked()));
    button_sizer->addWidget(pb);

    pb = new QPushButton("3. LINK", this);
    connect(pb, SIGNAL(clicked()), SLOT(linkClicked()));
    button_sizer->addWidget(pb);

    pb = new QPushButton("BUILD", this);
    connect(pb, SIGNAL(clicked()), SLOT(buildClicked()));
    button_sizer->addWidget(pb);

    pb = new QPushButton("CLEAR INFO LOG", this);
    connect(pb, SIGNAL(clicked()), SLOT(clearLog()));
    button_sizer->addWidget(pb);

    topSizer->addWidget(m_notebook);
    topSizer->addLayout(button_sizer);

    setLayout(topSizer);
}

void
SGShaderTextWindow::generateClicked()
{
    m_textBoxFrag->setPlainText(m_frame->getFragmentShader());
    m_textBoxVert->setPlainText(m_frame->getVertexShader());
}

void
SGShaderTextWindow::compileClicked()
{
    const QString vert = m_textBoxVert->toPlainText();
    const QString frag = m_textBoxFrag->toPlainText();

    m_frame->getCanvas()->compileShaders(vert, frag);

    m_notebook->setCurrentWidget(m_textBoxInfo);
}

void
SGShaderTextWindow::linkClicked()
{
    const QString vert = m_textBoxVert->toPlainText();
    const QString frag = m_textBoxFrag->toPlainText();
    const bool hasLinked = m_frame->getCanvas()->linkShaders(vert, frag);
    m_notebook->setCurrentWidget(m_textBoxInfo);
    if (hasLinked) {
        m_frame->setCanvasMode(SGCanvas::GLModeChoiceShader);
    }

    m_textBoxInfo->textCursor().movePosition(QTextCursor::End);
}

void
SGShaderTextWindow::buildClicked()
{
    const QString vert = m_textBoxVert->toPlainText();
    const QString frag = m_textBoxFrag->toPlainText();

    const bool hasLinked = m_frame->getCanvas()->linkShaders(vert, frag);
    if (hasLinked) {
        m_notebook->setCurrentWidget(m_textBoxInfo);
        m_frame->setCanvasMode(SGCanvas::GLModeChoiceShader);
    }
    m_textBoxInfo->textCursor().movePosition(QTextCursor::End);
}

void
SGShaderTextWindow::log(const QString& text)
{
    m_textBoxInfo->textCursor().movePosition(QTextCursor::End);
    m_textBoxInfo->append(text);
}

void
SGShaderTextWindow::clearLog()
{
    m_textBoxInfo->clear();
}
