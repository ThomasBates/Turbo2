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
// AndroidGLRenderer.h
// Renderer for teapots
//--------------------------------------------------------------------------------
#ifndef _MazePreviewRenderer_H
#define _MazePreviewRenderer_H

//--------------------------------------------------------------------------------
// Include files
//--------------------------------------------------------------------------------
#include <jni.h>
#include <errno.h>
#include <random>
#include <vector>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <android/native_window_jni.h>

#include <ITurboDebug.h>
#include <ITurboGameRenderer.h>
#include <ITurboGameIOService.h>

//#define CLASS_NAME "android/app/NativeActivity"
//#define APPLICATION_CLASS_NAME "com/sample/moreteapots/MoreTeapotsApplication"

#include "NDKHelper.h"

using namespace Turbo::Core::Debug;
using namespace Turbo::Game;

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

struct SHADER_VERTEX {
  float Position[3];
  float Normal[3];
  float Color[3];
  float Texture[2];
};

enum SHADER_ATTRIBUTES {
  ATTRIB_VERTEX,
  ATTRIB_NORMAL,
  ATTRIB_COLOR,
  ATTRIB_UV
};

struct SHADER_PARAMS {
  GLuint program_;
  GLint active_texture_;
  GLint light0_;
  GLint material_diffuse_;
  GLint material_ambient_;
  GLint material_specular_;

  GLint matrix_projection_;
  GLint matrix_view_;
};

struct SHADER_MATERIALS {
  float specular_color[4];
  float ambient_color[3];
};


namespace Turbo {
namespace Platform {
namespace AndroidGL {
class AndroidGLRenderer : public ITurboGameRenderer
{
public:
    AndroidGLRenderer(
            android_app *app,
            std::shared_ptr<ITurboDebug> debug,
            std::shared_ptr<ITurboGameIOService> ioService);

    virtual ~AndroidGLRenderer();

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

    //ANativeWindow *_nativeWindow;
    ndk_helper::GLContext *_gl_context;
    bool _resources_initialized = false;

    std::map<std::shared_ptr<ITurboSceneMesh>, GLuint> _sceneVertexBufferNames;
    std::map<std::shared_ptr<ITurboSceneMesh>, GLuint> _sceneVertexCount;
    std::map<std::shared_ptr<ITurboSceneMesh>, GLuint> _sceneIndexBufferNames;
    std::map<std::shared_ptr<ITurboSceneMesh>, GLuint> _sceneIndexCount;
    GLuint _sceneUniformBufferName;

    std::map<std::shared_ptr<ITurboSceneObject>, GLuint> _sceneObjectOffsets;
    GLuint _sceneObjectCount;
    std::map<std::shared_ptr<ITurboSceneMesh>, GLuint> _sceneObjectMesh;
    GLuint _sceneObjectMeshCount;
    std::map<std::string, GLuint> _sceneTextureBufferNames;
    GLuint _sceneObjectTextureCount;
    bool _sceneResourcesLoaded;

    ndk_helper::PerfMonitor _performance_monitor;

    SHADER_PARAMS shader_param_;

    ndk_helper::Mat4 _projectionMatrix;
    ndk_helper::Mat4 _viewMatrix;
    std::vector<ndk_helper::Mat4> vec_mat_models_;
    std::vector<ndk_helper::Vec3> vec_colors_;
    std::vector<ndk_helper::Vec2> vec_rotations_;
    std::vector<ndk_helper::Vec2> vec_current_rotations_;

    //  ndk_helper::TapCamera _tap_camera;

    int32_t ubo_matrix_stride_;
    int32_t ubo_vector_stride_;
    bool geometry_instancing_support_ = false;
    bool arb_support_;

    //  UpdateDisplayInformation    --------------------------------------------------------------------------------
    void ShowUI();

    //  LoadSceneResources  ----------------------------------------------------------------------------------------
    void InitializeSceneResources();

    void CreateSceneVertexResources(std::shared_ptr<ITurboScene> scene);
    void LoadSceneObjectVertices(std::shared_ptr<ITurboSceneObject> sceneObject);
    void LoadVertexData(std::shared_ptr<ITurboSceneMesh> mesh,
                        std::vector<SHADER_VERTEX> *vertexList,
                        std::vector<uint16_t> *indexList);

    void CreateSceneTextureResources(std::shared_ptr<ITurboScene> scene);
    void LoadSceneObjectTextures(std::shared_ptr<ITurboSceneObject> sceneObject);
    void LoadTextureData(std::string textureName,
                         GLsizei *textureWidth,
                         GLsizei *textureHeight,
                         std::vector<unsigned char> *textureData);

    void CreateShaders();
    bool LoadShaders(SHADER_PARAMS *params, const char *vertexShaderName,
                     const char *fragmentShaderName);
    bool LoadShadersES3(SHADER_PARAMS *params, const char *vertexShaderName,
                        const char *fragmentShaderName,
                        std::map<std::string, std::string> &shaderParameters);

    //  ReleaseSceneResources   ------------------------------------------------------------------------------------
    void DeleteBuffers();

    //  RenderScene ------------------------------------------------------------------------------------------------
    void UpdateFPS();
    void UpdateProjectionMatrix();
    void UpdateProjectionMatrix2();
    void UpdateViewMatrix(std::shared_ptr<ITurboScenePlacement> cameraPlacement);

    void InitializeRendering();
    void RenderSceneObjects(std::shared_ptr<ITurboScene> scene);
    void RenderSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject);
    void FinalizeRendering();


    //  bool Bind(ndk_helper::TapCamera* camera);
    std::string ToString(const int32_t i);
};
}}}

#endif
