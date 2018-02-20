#pragma once

#include <QTabWidget>

class SGOglLightNBPage;
class SGOglMaterialNBPage;
class SGOglFogNBPage;
class SGFixedGLState;
class SGOglTextureCoordNBPage;
class SGOglTextureEnvNBPage;

class SGOglNotebook : public QTabWidget
{
    Q_OBJECT
  public:
    SGOglNotebook(SGFixedGLState* glState, QWidget* parent = 0);
    ~SGOglNotebook();

    void setup();
  signals:
    void valueChanged();
  private slots:
    void onValueChange();

  private:
    SGOglLightNBPage* m_lightPage;
    SGOglMaterialNBPage* m_materialPage;
    SGOglFogNBPage* m_fogPage;
    SGOglTextureCoordNBPage* m_textureCoordPage;
    SGOglTextureEnvNBPage* m_textureEnvPage;
};
