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
  , m_haveCompiled(false)
  , m_haveLinked(false)
{
    notebook = new QTabWidget(this);

    textBoxVert = new QCodeEditor(notebook);
    textBoxFrag = new QCodeEditor(notebook);
    textBoxInfo = new QTextEdit(notebook);

    QFont fixedFont("Monospace");
    fixedFont.setStyleHint(QFont::TypeWriter);

    textBoxVert->setFont(fixedFont);
    textBoxFrag->setFont(fixedFont);
    textBoxInfo->setFont(fixedFont);

    notebook->addTab(textBoxVert, "Vertex Shader");
    notebook->addTab(textBoxFrag, "Fragment Shader");
    notebook->addTab(textBoxInfo, "InfoLog");

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

    topSizer->addWidget(notebook);
    topSizer->addLayout(button_sizer);

    setLayout(topSizer);
}

void
SGShaderTextWindow::generateClicked()
{
    textBoxFrag->setPlainText(m_frame->getFragmentShader());
    textBoxVert->setPlainText(m_frame->getVertexShader());
}

void
SGShaderTextWindow::compileClicked()
{
    const QString vert = m_frame->getVertexShader();
    const QString frag = m_frame->getFragmentShader();

    m_haveCompiled = m_frame->getCanvas()->compileShaders(vert, frag);

    notebook->setCurrentWidget(textBoxInfo);
}

void
SGShaderTextWindow::linkClicked()
{
    if (m_haveCompiled) {
        m_haveLinked =
          m_frame->getCanvas()->linkShaders(getVertexShaderText(), getFragmentShaderText());
        notebook->setCurrentWidget(textBoxInfo);
        if (m_haveLinked) {
            m_frame->setCanvasMode(SGCanvas::GLModeChoiceShader);
        }

        textBoxInfo->textCursor().movePosition(QTextCursor::End);
    }
}

void
SGShaderTextWindow::buildClicked()
{
    const QString vert = m_frame->getVertexShader();
    const QString frag = m_frame->getFragmentShader();

    m_haveCompiled = m_frame->getCanvas()->compileShaders(vert, frag);
    notebook->setCurrentWidget(textBoxInfo);
    if (m_haveCompiled) {
        m_haveLinked = m_frame->getCanvas()->linkShaders(vert, frag);
        if (m_haveLinked) {
            m_frame->setCanvasMode(SGCanvas::GLModeChoiceShader);
        }

        textBoxInfo->textCursor().movePosition(QTextCursor::End);
    }
}

void
SGShaderTextWindow::log(const QString& text)
{
    textBoxInfo->textCursor().movePosition(QTextCursor::End);
    textBoxInfo->append(text);
}

void
SGShaderTextWindow::clearLog()
{
    textBoxInfo->clear();
}
