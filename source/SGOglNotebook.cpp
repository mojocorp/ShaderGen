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
    m_lightPage = new SGOglLightNBPage(glState, this);
    m_materialPage = new SGOglMaterialNBPage(glState, this);
    m_fogPage = new SGOglFogNBPage(glState, this);
    m_textureCoordPage = new SGOglTextureCoordNBPage(glState, this);
    m_textureEnvPage = new SGOglTextureEnvNBPage(glState, this);

    connect(m_lightPage, SIGNAL(valueChanged()), SLOT(onValueChange()));
    connect(m_materialPage, SIGNAL(valueChanged()), SLOT(onValueChange()));
    connect(m_fogPage, SIGNAL(valueChanged()), SLOT(onValueChange()));
    connect(m_textureCoordPage, SIGNAL(valueChanged()), SLOT(onValueChange()));
    connect(m_textureEnvPage, SIGNAL(valueChanged()), SLOT(onValueChange()));

    addTab(m_lightPage, tr("LIGHT"));
    addTab(m_materialPage, tr("MATERIAL"));
    addTab(m_fogPage, tr("FOG"));
    addTab(m_textureCoordPage, tr("TEXTURE COORDINATE SET"));
    addTab(m_textureEnvPage, tr("TEXTURE ENVIRONMENT SET"));
}

SGOglNotebook::~SGOglNotebook()
{
}

void
SGOglNotebook::setup()
{
    m_lightPage->setup();
    m_materialPage->setup();
    m_fogPage->setup();
    m_textureCoordPage->setup();
    m_textureEnvPage->setup();
}

void
SGOglNotebook::onValueChange()
{
    emit valueChanged();
}
