/*
 * Copyright 2013 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//--------------------------------------------------------------------------------
// OpenGLESRenderer.h
// Renderer for teapots
//--------------------------------------------------------------------------------
#pragma once

//--------------------------------------------------------------------------------
// Include files
//--------------------------------------------------------------------------------
//#include <jni.h>
#include <errno.h>
#include <random>
#include <vector>

#include <EGL/egl.h>
#include <GLES/gl.h>

//#include <android/sensor.h>
//#include <android/log.h>
//#include <android_native_app_glue.h>
//#include <android/native_window_jni.h>

#include <ITurboDebug.h>
#include <ITurboGameControllerViewModel.h>
#include <ITurboGameIOService.h>
#include <ITurboGameRenderer.h>
#include <OpenGLESContext.h>

//#define CLASS_NAME "android/app/NativeActivity"
//#define APPLICATION_CLASS_NAME "com/sample/moreteapots/MoreTeapotsApplication"

//#include "NDKHelper.h"

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;

namespace Turbo
{
    namespace Platform
    {
        namespace OpenGLES
        {

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

            struct SHADER_VERTEX
            {
                float Position[3];
                float Normal[3];
                float Color[3];
                float Texture[2];
            };

            enum SHADER_ATTRIBUTES
            {
                ATTRIB_VERTEX,
                ATTRIB_NORMAL,
                ATTRIB_COLOR,
                ATTRIB_UV
            };

            struct MeshInfo
            {
                GLuint VertexBufferName;
                GLuint VertexCount;
                GLuint IndexBufferName;
                GLuint IndexCount;
            };

            struct SHADER_PARAMS
            {
                GLuint program;

                GLint TextureSampler;
                GLint ModelMatrix;
                GLint ViewMatrix;
                GLint ProjectionMatrix;
                GLint LightCount;
                GLint IsSprite;

//                GLint light0_;
//                GLint material_diffuse_;
//                GLint material_ambient_;
//                GLint material_specular_;
            };

            struct SHADER_MATERIALS
            {
                float specular_color[4];
                float ambient_color[3];
            };

            class OpenGLESRenderer : public ITurboGameRenderer
            {
            public:
                OpenGLESRenderer(
                        android_app *app,
                        std::shared_ptr<ITurboDebug> debug,
                        std::shared_ptr<ITurboGameIOService> ioService,
                        std::shared_ptr<ITurboGameControllerViewModel> controllerViewModel);

                virtual ~OpenGLESRenderer();

                //	ITurboGameRenderer Methods ---------------------------------------------------------------------------------
                virtual void UpdateDisplayInformation();
                virtual bool LoadSceneResources(std::shared_ptr<ITurboScene> scene);
                virtual void ReleaseSceneResources();
                virtual bool RenderScene(std::shared_ptr<ITurboScene> scene);
                virtual void Reset();

            private:
                //  Private Fields  --------------------------------------------------------------------------------------------
                android_app *_android_app;

                std::shared_ptr<ITurboDebug> _debug;
                std::shared_ptr<ITurboGameIOService> _ioService;
                std::shared_ptr<ITurboGameControllerViewModel> _controllerViewModel;

                //ANativeWindow *_nativeWindow;
                OpenGLESContext *_gl_context;
                bool _resources_initialized = false;

                std::map<std::shared_ptr<ITurboSceneMesh>, MeshInfo> _sceneMeshInfo;
                MeshInfo _spriteMeshInfo;
//                std::map<std::shared_ptr<ITurboSceneMesh>, GLuint> _sceneVertexBufferNames;
//                std::map<std::shared_ptr<ITurboSceneMesh>, GLuint> _sceneVertexCount;
//                std::map<std::shared_ptr<ITurboSceneMesh>, GLuint> _sceneIndexBufferNames;
//                std::map<std::shared_ptr<ITurboSceneMesh>, GLuint> _sceneIndexCount;
                GLuint _sceneUniformBufferName;

//                std::map<std::shared_ptr<ITurboSceneObject>, GLuint> _sceneObjectOffsets;
//                GLuint _sceneObjectCount;
//                std::map<std::shared_ptr<ITurboSceneMesh>, GLuint> _sceneObjectMesh;
//                GLuint _sceneObjectMeshCount;
                std::map<std::string, GLuint> _sceneTextureBufferNames;
//                GLuint _sceneObjectTextureCount;
                bool _sceneResourcesLoaded;

                SHADER_PARAMS _shaderParams;

                TurboMatrix4x4 _viewMatrix;
                TurboMatrix4x4 _projectionMatrix;
//                ndk_helper::Mat4 _viewMatrix;
//                ndk_helper::Mat4 _projectionMatrix;
                int _lightCount;

//                std::vector<ndk_helper::Mat4> vec_mat_models_;
//                std::vector<ndk_helper::Vec3> vec_colors_;
//                std::vector<ndk_helper::Vec2> vec_rotations_;
//                std::vector<ndk_helper::Vec2> vec_current_rotations_;

                //  ndk_helper::TapCamera _tap_camera;

                //int32_t ubo_matrix_stride_;
                //int32_t ubo_vector_stride_;
                //bool geometry_instancing_support_ = false;
                //bool arb_support_;

                //  LoadSceneResources  ----------------------------------------------------------------------------------------
                void InitializeSceneResources();

                void CreateSceneVertexResources(std::shared_ptr<ITurboScene> scene);
                void LoadSceneObjectVertices(std::shared_ptr<ITurboSceneObject> sceneObject);
                void LoadVertexData(std::shared_ptr<ITurboSceneMesh> mesh,
                                    std::vector<SHADER_VERTEX> *vertexList,
                                    std::vector<uint16_t> *indexList);

                void CreateSceneTextureResources(std::shared_ptr<ITurboScene> scene);
                void LoadSceneObjectTextures(std::shared_ptr<ITurboSceneObject> sceneObject);
                void LoadSceneSpriteTextures(std::shared_ptr<ITurboSceneSprite> sceneSprite);
                void LoadSceneTexture(std::shared_ptr<ITurboSceneTexture> texture);
                void LoadTextureData(std::string textureName,
                                     GLsizei *textureWidth,
                                     GLsizei *textureHeight,
                                     std::vector<unsigned char> *textureData);

                void CreateShaders();
                bool LoadShaders(SHADER_PARAMS *params,
                                 std::wstring vertexShaderName,
                                 std::wstring pixelShaderName);
                bool CompileShader(GLuint *shader, const GLenum type, std::wstring strFileName);
                bool LinkProgram(const GLuint prog);

                //  ReleaseSceneResources   ------------------------------------------------------------------------------------
                void DeleteBuffers();

                //  RenderScene ------------------------------------------------------------------------------------------------
                void UpdateProjectionMatrix();
                void UpdateViewMatrix(std::shared_ptr<ITurboScenePlacement> cameraPlacement, bool lightHack);

                void InitializeRendering();
                void RenderSceneObjects(std::shared_ptr<ITurboScene> scene);
                void RenderSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject);
                void RenderSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite);
                void FinalizeRendering();

                TurboMatrix4x4 MakePerspectiveProjection(float fovAngle,
                                                         float viewportWidth,
                                                         float viewportHeight,
                                                         float nearPlane,
                                                         float farPlane);
                TurboMatrix4x4 MakeViewProjection(const TurboVector3D& vEye, const TurboVector3D& vAt, const TurboVector3D& vUp);

                void LoadSceneSpriteVertices();
            };
        }
    }
}
