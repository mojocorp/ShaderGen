#include <QMessageBox>
#include <QMenuBar>
#include <QStatusBar>
#include <QSplitter>
#include <QActionGroup>
#include <QBoxLayout>
#include <QSettings>
#include <QFileDialog>
#include <QJsonDocument>

#include "SGFrame.h"
#include "SGFixedGLState.h"
#include "SGCanvasWrapper.h"
#include "SGCanvas.h"
#include "SGShaderTextWindow.h"
#include "SGOglNotebook.h"

SGFrame *sgframe_instance = 0;

SGFrame::SGFrame(const QString& title)
    : QMainWindow()
{
    setWindowTitle(title);

    glState = new SGFixedGLState();
    textures = new SGTextures(this, glState);
    shaderGen = new SGShaderGenerator();

    createActions();
    createMenus();
    createStatusBar();

    oglNotebook = new SGOglNotebook(glState, this);
    oglNotebook->resize(800, 300);
    connect(oglNotebook, SIGNAL(valueChanged()), SLOT(setFixedGLMode()));

    canvas      = new SGCanvasWrapper(this);
    canvas->resize(400, 350);
    shaderText  = new SGShaderTextWindow(this);
    shaderText->resize(450, 400);

    topSizer = new QSplitter(Qt::Vertical);
    horizSizer = new QSplitter(Qt::Horizontal);

    horizSizer->addWidget(canvas);
    horizSizer->addWidget(shaderText);

    topSizer->addWidget(horizSizer);
    topSizer->addWidget(oglNotebook);

    setCentralWidget(topSizer);

    sgframe_instance = this;
}

SGFrame::~SGFrame()
{
    sgframe_instance = 0;

    delete shaderGen;
    delete textures;
    delete glState;
} 

void SGFrame::createActions()
{
    openAct = new QAction(tr("&Open..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAsAct = new QAction(tr("Save &As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the document under a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    perspAct = new QAction(tr("Perspective"), this);
    perspAct->setCheckable(true);
    perspAct->setChecked(true);
    connect(perspAct, SIGNAL(triggered()), SLOT(viewActionTriggered()));

    switchGLModeAct = new QAction(tr("Switch GL mode"), this);
    switchGLModeAct->setShortcut(Qt::Key_F7);
    connect(switchGLModeAct, SIGNAL(triggered()), SLOT(switchGLModeTriggered()));

    torusAct = new QAction(tr("Torus"), this);
    torusAct->setCheckable(true);
    torusAct->setChecked(true);

    sphereAct = new QAction(tr("Sphere"), this);
    sphereAct->setCheckable(true);

    trefoilAct = new QAction(tr("Trefoil"), this);
    trefoilAct->setCheckable(true);

    kleinAct = new QAction(tr("Klein"), this);
    kleinAct->setCheckable(true);

    conicAct = new QAction(tr("Conic"), this);
    conicAct->setCheckable(true);

    planeAct = new QAction(tr("Plane"), this);
    planeAct->setCheckable(true);

    QActionGroup *modelGroup = new QActionGroup(this);
    modelGroup->addAction(torusAct);
    modelGroup->addAction(sphereAct);
    modelGroup->addAction(trefoilAct);
    modelGroup->addAction(kleinAct);
    modelGroup->addAction(conicAct);
    modelGroup->addAction(planeAct);
    connect(modelGroup, SIGNAL(triggered(QAction*)), SLOT(modelActionTriggered(QAction*)));

    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    helpAct = new QAction(tr("Help"), this);
    helpAct->setStatusTip(tr("Show the help"));
    connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));
}

void SGFrame::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    menuBar()->addSeparator();

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(perspAct);
    viewMenu->addSeparator();
    viewMenu->addAction(switchGLModeAct);


    modelMenu = menuBar()->addMenu(tr("&Model"));
    modelMenu->addAction(torusAct);
    modelMenu->addAction(sphereAct);
    modelMenu->addAction(trefoilAct);
    modelMenu->addAction(kleinAct);
    modelMenu->addAction(conicAct);
    modelMenu->addAction(planeAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(helpAct);
}

void SGFrame::createStatusBar()
{
    statusBar()->showMessage(windowTitle());
}

void SGFrame::setCanvasMode(SGCanvas::GLMode a)
{
    canvas->setMode(a);
    getCanvas()->updateGL();
}

void SGFrame::setStatusText(const QString &text)
{
    statusBar()->showMessage(text);
}

bool SGFrame::isPerspective() const
{
    return perspAct->isChecked();
}

int SGFrame::printOglError(const char *file, int line)
{
    if (!sgframe_instance)
        return 0;

    int    retCode = 0;

    GLenum glErr = glGetError();

    if(DEBUG_ON)
    {
        while (glErr != GL_NO_ERROR)
        {
            QString str = QString("glError in file %1:%2: %3").arg(file).arg(line).arg( (const char*)gluErrorString(glErr));
            sgframe_instance->getShaderTextWindow()->log(str);
            retCode = 1;
            glErr = glGetError();
        }
    }
    return retCode;
}

bool SGFrame::loadFile(const QString& filename)
{
    QFile loadFile(filename);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    glState->read(loadDoc.object());
    oglNotebook->setup();
    getCanvas()->updateGL();

    return true;
}

bool SGFrame::saveFile(const QString& filename) const
{
    QFile saveFile(filename);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QJsonObject stateObject;
    glState->write(stateObject);
    QJsonDocument saveDoc(stateObject);
    saveFile.write(saveDoc.toJson());

    return true;
}

void SGFrame::readSettings()
{
    QSettings settings;
    restoreGeometry(settings.value("app/geometry").toByteArray());
    restoreState(settings.value("app/windowState").toByteArray());
    topSizer->restoreState(settings.value("app/topSizer").toByteArray());
    horizSizer->restoreState(settings.value("app/horizSizer").toByteArray());
}

void SGFrame::closeEvent(QCloseEvent * event)
{
    QSettings settings;
    settings.setValue("app/geometry", saveGeometry());
    settings.setValue("app/windowState", saveState());
    settings.setValue("app/topSizer", topSizer->saveState());
    settings.setValue("app/horizSizer", horizSizer->saveState());

    QMainWindow::closeEvent(event);
}

void SGFrame::modelActionTriggered(QAction *action)
{
    if (action == torusAct) {
        getCanvas()->setModel(SGModels::ModelTorus);
    } else if (action == sphereAct) {
        getCanvas()->setModel(SGModels::ModelSphere);
    } else if (action == trefoilAct) {
        getCanvas()->setModel(SGModels::ModelTrefoil);
    } else if (action == kleinAct) {
        getCanvas()->setModel(SGModels::ModelKlein);
    } else if (action == conicAct) {
        getCanvas()->setModel(SGModels::ModelConic);
    } else if (action == planeAct) {
        getCanvas()->setModel(SGModels::ModelPlane);
    }

    getCanvas()->updateGL();
}

void SGFrame::viewActionTriggered()
{
    getCanvas()->updateGL();
}

void SGFrame::switchGLModeTriggered()
{
    canvas->switchMode();
    getCanvas()->updateGL();
}

void SGFrame::setFixedGLMode()
{
    canvas->setMode(SGCanvas::GLModeChoiceFixed);
    getCanvas()->updateGL();
}

void SGFrame::help()
{
    QDialog help(this);
    help.setWindowTitle(tr("Help - ShaderGen"));
    QVBoxLayout *layout = new QVBoxLayout(&help);
    QTextEdit *txtEdit = new QTextEdit();
    txtEdit->setReadOnly(true);
    txtEdit->setPlainText(tr("\n"
                             "GLSL ShaderGen\n\n"
                             "INTENDED PURPOSE:\n\n"
                             "The purpose of ShaderGen is to show the user how to emulate fixed functionality\n"
                             "by using the OpenGL Shading Language.\n\n"
                             "ShaderGen is intended to be educational, with the focus being on clarity of\n"
                             "generated code rather than efficiency.\n"
                             " \n"
                             " \n"
                             "USAGE:\n\n"
                             "The tool has three major parts:\n"
                             "* OpenGL Window : Displays rendered output.\n"
                             "* OpenGL State : Configure the OpenGL State using the tabs.\n"
                             "* Shader Text Windows : Use the tabs to look through the generated shaders\n\n"
                             "Operation:\n\n"
                             "1. Using the tabs, setup the OpenGL state.  You will see your results updated"
                             " in the OpenGL Window as you make state changes.  If you make a change requiring text"
                             " entry, be sure to press the enter key to update the current state.\n"
                             "2. Click the \"GENERATE SHADERS\" button on the Shader Windows panel to generate Vertex and Fragment Shaders\n"
                             "3. Click \"COMPILE\" to compile the shaders, then check the \"Infolog\" text window for compilation results\n"
                             "4. Click \"LINK\" to link the compilers, then see the \"Infolog\" window for results\n"
                             "5. To switch the OpenGL Window to display the shaded model, click the \"Shader"
                             " Equivalent\" mode radio button.\n"
                             "6. Click \"BUILD\" to do steps 2 through 5 together.\n\n\n"
                             "Note: The text boxes only accept values when enter key is pressed\n"
                             "Note: You can edit the shaders in the text windows, and then press \"COMPILE\" and then \"LINK\" to"
                             " to see the results of your edited shaders.\n"
                             "Note: The eye coordinate distance between the model center and the viewpoint is 6.0.  The model is"
                             " drawn with it's center at (0.0,0.0,6.0) in eye-coordinates and diagonals at (-1.0, -1.0, 5.0)"
                             " and (1.0, 1.0, 7.0).\n"
                             "                 "));

    layout->addWidget(txtEdit);
    help.resize(600, 600);
    help.exec();
}

void SGFrame::about()
{
    QMessageBox::about(this, "GLSL ShaderGen", "<center>"
                                               "<p><img src=\":textures/3Dlabs.png\"></p>"
                                               "<p><b>GLSL ShaderGen</b></p>"
                                               "<p>Version 3.3.1</p>"
                                               "</center>");
}

bool SGFrame::open()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open"), QDir::homePath(),
                                                    tr("ShaderGen Files (*.json)"));
    if (fileName.isEmpty())
        return false;

    return loadFile(fileName);
}

bool SGFrame::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), QDir::homePath(),
                                                    tr("ShaderGen Files (*.json)"));
    if (fileName.isEmpty())
        return false;

    return saveFile(fileName);
}
