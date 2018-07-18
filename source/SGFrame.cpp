#include <QActionGroup>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QFileDialog>
#include <QGroupBox>
#include <QJsonDocument>
#include <QMenuBar>
#include <QMessageBox>
#include <QRadioButton>
#include <QSettings>
#include <QSplitter>
#include <QStatusBar>
#include <QVBoxLayout>

#include "SGCanvas.h"
#include "SGFixedGLState.h"
#include "SGFrame.h"
#include "SGOglNotebook.h"
#include "SGShaderTextWindow.h"
#include "SGTextures.h"

static SGFrame* sgframe_instance = 0;

SGFrame::SGFrame(const QString& title)
  : QMainWindow()
{
    setWindowTitle(title);

    m_glState = new SGFixedGLState();
    m_textures = new SGTextures(this, m_glState);
    m_shaderGen = new SGShaderGenerator(m_glState);

    createActions();
    createMenus();
    createStatusBar();

    m_oglNotebook = new SGOglNotebook(m_glState, this);
    m_oglNotebook->resize(800, 300);
    connect(m_oglNotebook, SIGNAL(valueChanged()), SLOT(setFixedGLMode()));

    QFrame* canvasWrapper = new QFrame(this);
    QVBoxLayout* canvasWrapperLayout = new QVBoxLayout(canvasWrapper);

    m_canvas = new SGCanvas(this);
    m_canvas->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QGroupBox* gb = new QGroupBox(tr("Select GL Mode"), this);
    gb->setLayout(new QHBoxLayout);
    QRadioButton* fixed = new QRadioButton(tr("FIXED FUNCTIONALITY MODE"), gb);
    fixed->setChecked(true);
    QRadioButton* shader = new QRadioButton(tr("EQUIVALENT SHADER MODE "), gb);
    gb->layout()->addWidget(fixed);
    gb->layout()->addWidget(shader);

    m_glModeChoice = new QButtonGroup(this);
    m_glModeChoice->addButton(fixed, 0);
    m_glModeChoice->addButton(shader, 1);
    connect(m_glModeChoice, SIGNAL(buttonClicked(int)), SLOT(onGLModeChanged(int)));

    canvasWrapperLayout->addWidget(gb);
    canvasWrapperLayout->addWidget(m_canvas);
    canvasWrapper->resize(400, 350);

    m_shaderText = new SGShaderTextWindow(this);
    m_shaderText->resize(450, 400);

    m_topSizer = new QSplitter(Qt::Vertical);
    m_horizSizer = new QSplitter(Qt::Horizontal);

    m_horizSizer->addWidget(canvasWrapper);
    m_horizSizer->addWidget(m_shaderText);

    m_topSizer->addWidget(m_horizSizer);
    m_topSizer->addWidget(m_oglNotebook);

    setCentralWidget(m_topSizer);

    sgframe_instance = this;
}

SGFrame::~SGFrame()
{
    sgframe_instance = 0;

    delete m_shaderGen;
    delete m_textures;
    delete m_glState;
}

void
SGFrame::createActions()
{
    m_openAct = new QAction(tr("&Open..."), this);
    m_openAct->setShortcuts(QKeySequence::Open);
    m_openAct->setStatusTip(tr("Open an existing file"));
    connect(m_openAct, SIGNAL(triggered()), this, SLOT(open()));

    m_saveAsAct = new QAction(tr("Save &As..."), this);
    m_saveAsAct->setShortcuts(QKeySequence::SaveAs);
    m_saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(m_saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    m_exitAct = new QAction(tr("E&xit"), this);
    m_exitAct->setShortcuts(QKeySequence::Quit);
    m_exitAct->setStatusTip(tr("Exit the application"));
    connect(m_exitAct, SIGNAL(triggered()), this, SLOT(close()));

    m_perspAct = new QAction(tr("Perspective"), this);
    m_perspAct->setCheckable(true);
    m_perspAct->setChecked(true);
    connect(m_perspAct, SIGNAL(triggered()), SLOT(viewActionTriggered()));

    m_switchGLModeAct = new QAction(tr("Switch GL mode"), this);
    m_switchGLModeAct->setShortcut(Qt::Key_F7);
    connect(m_switchGLModeAct, SIGNAL(triggered()), SLOT(switchGLModeTriggered()));

    m_torusAct = new QAction(tr("Torus"), this);
    m_torusAct->setCheckable(true);
    m_torusAct->setChecked(true);

    m_sphereAct = new QAction(tr("Sphere"), this);
    m_sphereAct->setCheckable(true);

    m_trefoilAct = new QAction(tr("Trefoil"), this);
    m_trefoilAct->setCheckable(true);

    m_kleinAct = new QAction(tr("Klein"), this);
    m_kleinAct->setCheckable(true);

    m_conicAct = new QAction(tr("Conic"), this);
    m_conicAct->setCheckable(true);

    m_planeAct = new QAction(tr("Plane"), this);
    m_planeAct->setCheckable(true);

    QActionGroup* modelGroup = new QActionGroup(this);
    modelGroup->addAction(m_torusAct);
    modelGroup->addAction(m_sphereAct);
    modelGroup->addAction(m_trefoilAct);
    modelGroup->addAction(m_kleinAct);
    modelGroup->addAction(m_conicAct);
    modelGroup->addAction(m_planeAct);
    connect(modelGroup, SIGNAL(triggered(QAction*)), SLOT(modelActionTriggered(QAction*)));

    m_aboutAct = new QAction(tr("&About"), this);
    m_aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(m_aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    m_helpAct = new QAction(tr("Help"), this);
    m_helpAct->setStatusTip(tr("Show the help"));
    connect(m_helpAct, SIGNAL(triggered()), this, SLOT(help()));
}

void
SGFrame::createMenus()
{
    m_fileMenu = menuBar()->addMenu(tr("&File"));
    m_fileMenu->addAction(m_openAct);
    m_fileMenu->addAction(m_saveAsAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAct);

    menuBar()->addSeparator();

    m_viewMenu = menuBar()->addMenu(tr("&View"));
    m_viewMenu->addAction(m_perspAct);
    m_viewMenu->addSeparator();
    m_viewMenu->addAction(m_switchGLModeAct);

    m_modelMenu = menuBar()->addMenu(tr("&Model"));
    m_modelMenu->addAction(m_torusAct);
    m_modelMenu->addAction(m_sphereAct);
    m_modelMenu->addAction(m_trefoilAct);
    m_modelMenu->addAction(m_kleinAct);
    m_modelMenu->addAction(m_conicAct);
    m_modelMenu->addAction(m_planeAct);

    m_helpMenu = menuBar()->addMenu(tr("&Help"));
    m_helpMenu->addAction(m_aboutAct);
    m_helpMenu->addAction(m_helpAct);
}

void
SGFrame::createStatusBar()
{
    statusBar()->showMessage(windowTitle());
}

void
SGFrame::setCanvasMode(SGCanvas::GLMode a)
{
    if (a == SGCanvas::GLModeChoiceFixed) {
        m_glModeChoice->button(0)->setChecked(true);
    } else {
        m_glModeChoice->button(1)->setChecked(true);
    }
    const QString vert = getVertexShader();
    const QString frag = getFragmentShader();

    getShaderTextWindow()->setFragmentShaderText(frag);
    getShaderTextWindow()->setVertexShaderText(vert);

    m_canvas->setMode(a);
    m_canvas->update();
}

void
SGFrame::setStatusText(const QString& text)
{
    statusBar()->showMessage(text);
}

bool
SGFrame::isPerspective() const
{
    return m_perspAct->isChecked();
}

int
SGFrame::printOglError(const char* file, int line)
{
    if (!sgframe_instance)
        return 0;

    int retCode = 0;

    GLenum glErr = glGetError();

    while (glErr != GL_NO_ERROR) {
        const QString str = QString("glError in file %1:%2: %3").arg(file).arg(line).arg(glErr);
        sgframe_instance->getShaderTextWindow()->log(str);
        retCode = 1;
        glErr = glGetError();
    }
    return retCode;
}

bool
SGFrame::loadFile(const QString& filename)
{
    QFile loadFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    m_glState->read(loadDoc.object());
    m_oglNotebook->setup();
    m_canvas->update();

    return true;
}

bool
SGFrame::saveFile(const QString& filename) const
{
    QFile saveFile(filename);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QJsonObject stateObject;
    m_glState->write(stateObject);
    QJsonDocument saveDoc(stateObject);
    saveFile.write(saveDoc.toJson());

    return true;
}

void
SGFrame::readSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("app/geometry").toByteArray());
    restoreState(settings.value("app/windowState").toByteArray());
    m_topSizer->restoreState(settings.value("app/topSizer").toByteArray());
    m_horizSizer->restoreState(settings.value("app/horizSizer").toByteArray());
}

void
SGFrame::closeEvent(QCloseEvent* event)
{
    QSettings settings;
    settings.setValue("app/geometry", saveGeometry());
    settings.setValue("app/windowState", saveState());
    settings.setValue("app/topSizer", m_topSizer->saveState());
    settings.setValue("app/horizSizer", m_horizSizer->saveState());

    QMainWindow::closeEvent(event);
}

void
SGFrame::onGLModeChanged(int id)
{
    setCanvasMode((SGCanvas::GLMode)id);
}

void
SGFrame::modelActionTriggered(QAction* action)
{
    if (action == m_torusAct) {
        m_canvas->setModel(SGModels::ModelTorus);
    } else if (action == m_sphereAct) {
        m_canvas->setModel(SGModels::ModelSphere);
    } else if (action == m_trefoilAct) {
        m_canvas->setModel(SGModels::ModelTrefoil);
    } else if (action == m_kleinAct) {
        m_canvas->setModel(SGModels::ModelKlein);
    } else if (action == m_conicAct) {
        m_canvas->setModel(SGModels::ModelConic);
    } else if (action == m_planeAct) {
        m_canvas->setModel(SGModels::ModelPlane);
    }

    m_canvas->update();
}

void
SGFrame::viewActionTriggered()
{
    m_canvas->update();
}

void
SGFrame::switchGLModeTriggered()
{
    if (m_canvas->getMode() == SGCanvas::GLModeChoiceFixed) {
        setCanvasMode(SGCanvas::GLModeChoiceShader);
    } else {
        setCanvasMode(SGCanvas::GLModeChoiceFixed);
    }
    m_canvas->update();
}

void
SGFrame::setFixedGLMode()
{
    setCanvasMode(SGCanvas::GLModeChoiceFixed);
    m_canvas->update();
}

void
SGFrame::help()
{
    QDialog help(this);
    help.setWindowTitle(tr("Help - ShaderGen"));
    QVBoxLayout* layout = new QVBoxLayout(&help);
    QTextEdit* txtEdit = new QTextEdit();
    txtEdit->setReadOnly(true);
    txtEdit->setPlainText(tr("\n"
                             "GLSL ShaderGen\n\n"
                             "INTENDED PURPOSE:\n\n"
                             "The purpose of ShaderGen is to show the user how to emulate fixed "
                             "functionality\n"
                             "by using the OpenGL Shading Language.\n\n"
                             "ShaderGen is intended to be educational, with the focus being on "
                             "clarity of\n"
                             "generated code rather than efficiency.\n"
                             " \n"
                             " \n"
                             "USAGE:\n\n"
                             "The tool has three major parts:\n"
                             "* OpenGL Window : Displays rendered output.\n"
                             "* OpenGL State : Configure the OpenGL State using the tabs.\n"
                             "* Shader Text Windows : Use the tabs to look through the generated "
                             "shaders\n\n"
                             "Operation:\n\n"
                             "1. Using the tabs, setup the OpenGL state.  You will see your "
                             "results updated"
                             " in the OpenGL Window as you make state changes.  If you make a "
                             "change requiring text"
                             " entry, be sure to press the enter key to update the current state.\n"
                             "2. Click the \"GENERATE SHADERS\" button on the Shader Windows panel "
                             "to generate Vertex and Fragment Shaders\n"
                             "3. Click \"COMPILE\" to compile the shaders, then check the "
                             "\"Infolog\" text window for compilation results\n"
                             "4. Click \"LINK\" to link the compilers, then see the \"Infolog\" "
                             "window for results\n"
                             "5. To switch the OpenGL Window to display the shaded model, click "
                             "the \"Shader"
                             " Equivalent\" mode radio button.\n"
                             "6. Click \"BUILD\" to do steps 2 through 5 together.\n\n\n"
                             "Note: The text boxes only accept values when enter key is pressed\n"
                             "Note: You can edit the shaders in the text windows, and then press "
                             "\"COMPILE\" and then \"LINK\" to"
                             " to see the results of your edited shaders.\n"
                             "Note: The eye coordinate distance between the model center and the "
                             "viewpoint is 6.0.  The model is"
                             " drawn with it's center at (0.0,0.0,6.0) in eye-coordinates and "
                             "diagonals at (-1.0, -1.0, 5.0)"
                             " and (1.0, 1.0, 7.0).\n"
                             "                 "));

    layout->addWidget(txtEdit);
    help.resize(600, 600);
    help.exec();
}

void
SGFrame::about()
{
    QMessageBox::about(this, "GLSL ShaderGen", QString("<center>"
                                                       "<p><img src=\":textures/3Dlabs.png\"></p>"
                                                       "<p><b>GLSL ShaderGen</b></p>"
                                                       "<p>Version %1</p>"
                                                       "</center>")
                                                 .arg(SHADERGEN_VERSION));
}

bool
SGFrame::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath(),
                                                    tr("ShaderGen Files (*.json)"));
    if (fileName.isEmpty())
        return false;

    return loadFile(fileName);
}

bool
SGFrame::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath(),
                                                    tr("ShaderGen Files (*.json)"));
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}
