
#pragma once

#include <errno.h>
#include <random>
#include <vector>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <ITurboGameRenderer.h>
#include <ITurboViewRendererAccess.h>
#include <ITurboDebug.h>
#include <ITurboGameIOService.h>
#include <ITurboSceneFont.h>

#include <OpenGLESContext.h>
#include <OpenGLESTypes.h>

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;
using namespace Turbo::View;

namespace Turbo
{
    namespace Platform
    {
        namespace OpenGLES
        {
            class OpenGLESRenderer : public ITurboGameRenderer, public ITurboViewRendererAccess
            {
            public:
                //	Constructors ---------------------------------------------------------------------------------------
                OpenGLESRenderer(
                        android_app *app,
                        std::shared_ptr<ITurboDebug> debug,
                        std::shared_ptr<ITurboGameIOService> ioService);

                virtual ~OpenGLESRenderer();

                //	ITurboGameRenderer Properties ----------------------------------------------------------------------
                virtual std::shared_ptr<ITurboViewRendererAccess> RendererAccess() { return std::shared_ptr<ITurboViewRendererAccess>(this); }

                //	ITurboGameRenderer Methods -------------------------------------------------------------------------
                virtual void InitializeLoading();
                virtual void FinalizeLoading();
                virtual void InitializeRendering();
                virtual void FinalizeRendering();
                virtual void Reset();

                //	ITurboViewRendererAccess Methods -------------------------------------------------------------------
                virtual void LoadScene(std::shared_ptr<ITurboScene> scene);
                virtual void LoadSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite);
                virtual void LoadSceneText(std::shared_ptr<ITurboSceneText> sceneText);

                virtual void LoadSceneSound(std::shared_ptr<ITurboSceneSound> sceneSoundEffect) {}
                virtual void LoadSceneBackground(std::shared_ptr<ITurboSceneSound> sceneBackground) {}

                virtual void RenderScene(std::shared_ptr<ITurboScene> scene);
                virtual void RenderSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite);
                virtual void RenderSceneText(std::shared_ptr<ITurboSceneText> sceneText);

                virtual void RenderSceneSound(std::shared_ptr<ITurboSceneSound> sceneSoundEffect) {}
                virtual void RenderSceneBackground(std::shared_ptr<ITurboSceneSound> sceneBackground) {}

            private:
                //  Private Fields  ------------------------------------------------------------------------------------
                android_app *_android_app;
                std::shared_ptr<ITurboDebug> _debug;
                std::shared_ptr<ITurboGameIOService> _ioService;

                OpenGLESContext *_gl_context;
                bool _resources_initialized = false;
                bool _display_updated = false;

                std::map<std::string, std::shared_ptr<ITurboSceneFont>> _sceneFonts;
                std::map<std::shared_ptr<ITurboSceneMesh>, MeshInfo> _sceneMeshInfo;
                MeshInfo _spriteMeshInfo {};
                std::map<std::shared_ptr<ITurboSceneText>, MeshInfo> _sceneTextInfo;
                GLuint _sceneUniformBufferName;
                std::map<std::string, TextureInfo> _sceneTextureInfo;

                GLuint _shaderProgram;
                SHADER_PARAMS _shaderParams;

                TurboMatrix4x4 _viewMatrix;
                TurboMatrix4x4 _projectionMatrix;
                int _lightCount;

                //  LoadSceneResources  --------------------------------------------------------------------------------
                void LoadSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject);
                void LoadChildSceneObjects(std::shared_ptr<ITurboSceneObject> sceneObject);
                void RenderSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject);
                void RenderChildSceneObjects(std::shared_ptr<ITurboSceneObject> sceneObject);

                void RegisterFont(std::shared_ptr<ITurboSceneFont> font);
                void UpdateDisplayInformation();
                void InitializeViewResources();
                void ReleaseViewResources();

                void LoadSceneObjectVertices(std::shared_ptr<ITurboSceneObject> sceneObject);
                void LoadSceneSpriteVertices();
                void LoadSceneTextVertices(std::shared_ptr<ITurboSceneText> sceneText);
                void LoadMeshInfo(std::shared_ptr<ITurboSceneMesh> mesh, MeshInfo *meshInfo);
                void LoadVertexData(std::shared_ptr<ITurboSceneMesh> mesh,
                                    std::vector<SHADER_VERTEX> *vertexList,
                                    std::vector<uint16_t> *indexList);

                void LoadSceneObjectTexture(std::shared_ptr<ITurboSceneObject> sceneObject);
                void LoadSceneSpriteTexture(std::shared_ptr<ITurboSceneSprite> sceneSprite);
                void LoadSceneFontTexture(std::shared_ptr<ITurboSceneText> sceneText);
                void LoadSceneTexture(std::shared_ptr<ITurboSceneTexture> texture);
                void LoadTextureData(std::string textureName,
                                     GLsizei *textureWidth,
                                     GLsizei *textureHeight,
                                     std::vector<unsigned char> *textureData);

                //  FinalizeLoading Support Methods --------------------------------------------------------------------
                void CreateShaders();
                bool LoadShaders(SHADER_PARAMS *params,
                                 std::wstring vertexShaderName,
                                 std::wstring pixelShaderName);
                bool CompileShader(GLuint *shader, const GLenum type, std::wstring strFileName);
                bool LinkProgram(const GLuint prog);

                //  Reset Support Methods ------------------------------------------------------------------------------
                void DeleteBuffers();

                //  RenderScene ----------------------------------------------------------------------------------------
                void UpdateProjectionMatrix();
                void UpdateViewMatrix(std::shared_ptr<ITurboScenePlacement> cameraPlacement, bool lightHack);

                TurboMatrix4x4 MakePerspectiveProjection(float fovAngle,
                                                         float viewportWidth,
                                                         float viewportHeight,
                                                         float nearPlane,
                                                         float farPlane);
                TurboMatrix4x4 MakeViewProjection(const TurboVector3D& vEye, const TurboVector3D& vAt, const TurboVector3D& vUp);

                void RemoveUnusedResources();

                void RenderSceneMesh(MeshInfo *meshInfo, std::string textureName, int isSprite,
                         TurboMatrix4x4 *projection, TurboMatrix4x4 *view, TurboMatrix4x4 *model);
            };
        }
    }
}
