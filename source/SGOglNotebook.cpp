#include "SGOglNotebook.h"
#include "SGFixedGLState.h"
#include "SGFrame.h"
#include "SGOglFogNBPage.h"
#include "SGOglLightNBPage.h"
#include "SGOglMaterialNBPage.h"
#include "SGOglTextureCoordNBPage.h"
#include "SGOglTextureEnvNBPage.h"
#include "SGTextures.h"

SGOglNotebook::SGOglNotebook(SGFixedGLState* glState, QWidget* parent)
  : QTabWidget(parent)
{
    lightPage = new SGOglLightNBPage(glState, this);
    materialPage = new SGOglMaterialNBPage(glState, this);
    fogPage = new SGOglFogNBPage(glState, this);
    textureCoordPage = new SGOglTextureCoordNBPage(glState, this);
    textureEnvPage = new SGOglTextureEnvNBPage(glState, this);

    connect(lightPage, SIGNAL(valueChanged()), SLOT(onValueChange()));
    connect(materialPage, SIGNAL(valueChanged()), SLOT(onValueChange()));
    connect(fogPage, SIGNAL(valueChanged()), SLOT(onValueChange()));
    connect(textureCoordPage, SIGNAL(valueChanged()), SLOT(onValueChange()));
    connect(textureEnvPage, SIGNAL(valueChanged()), SLOT(onValueChange()));

    addTab(lightPage, tr("LIGHT"));
    addTab(materialPage, tr("MATERIAL"));
    addTab(fogPage, tr("FOG"));
    addTab(textureCoordPage, tr("TEXTURE COORDINATE SET"));
    addTab(textureEnvPage, tr("TEXTURE ENVIRONMENT SET"));
}

SGOglNotebook::~SGOglNotebook()
{
}

void
SGOglNotebook::setup()
{
    lightPage->setup();
    materialPage->setup();
    fogPage->setup();
    textureCoordPage->setup();
    textureEnvPage->setup();
}

void
SGOglNotebook::onValueChange()
{
    emit valueChanged();
}
