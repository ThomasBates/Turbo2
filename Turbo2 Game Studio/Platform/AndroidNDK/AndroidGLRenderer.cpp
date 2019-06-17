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
// AndroidGLRenderer.cpp
// Render teapots
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// Include files
//--------------------------------------------------------------------------------
#include "AndroidGLRenderer.h"
#include "AndroidNDKHelpers.h"

#include <chrono>
#include <thread>
#include <string.h>

#include <ITurboCanvas.h>
#include <TurboCanvasRGBA32.h>
#include <ITurboImage.h>
#include <TurboBitmap.h>

using namespace Turbo::Graphics;
using namespace Turbo::Platform::AndroidGL;
using namespace Turbo::Platform::AndroidNDK;

//  Constructors & Destructors  ----------------------------------------------------------------------------------------

AndroidGLRenderer::AndroidGLRenderer(
        android_app* app,
        std::shared_ptr<ITurboDebug> debug,
        std::shared_ptr<ITurboGameIOService> ioService) :
        _android_app(app),
        _debug(debug),
        _ioService(ioService),
        geometry_instancing_support_(false)
{
    _gl_context = ndk_helper::GLContext::GetInstance();
}

AndroidGLRenderer::~AndroidGLRenderer()
{
    ReleaseSceneResources();
}

//	ITurboGameRenderer Methods ---------------------------------------------------------------------

void AndroidGLRenderer::UpdateDisplayInformation()
{
//    ReleaseSceneResources();

    if (!_resources_initialized)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        _gl_context->Init(_android_app->window);
        _resources_initialized = true;
    }
    else if(_android_app->window != _gl_context->GetANativeWindow())
    {
        // Re-initialize ANativeWindow.
        // On some devices, ANativeWindow is re-created when the app is resumed
        assert(_gl_context->GetANativeWindow());
        ReleaseSceneResources();
        _gl_context->Invalidate();
        _gl_context->Init(_android_app->window);
        _resources_initialized = true;
    }
    else
    {
        // initialize OpenGL ES and EGL
        if (EGL_SUCCESS == _gl_context->Resume(_android_app->window))
        {
            ReleaseSceneResources();
        }
        else
        {
            assert(0);
        }
    }

    ShowUI();

    // Initialize GL state.
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Note that screen size might have been changed
    int width = _gl_context->GetScreenWidth();
    int height = _gl_context->GetScreenHeight();
    glViewport(0, 0, width, height);
    //renderer_.UpdateViewport();
}

bool AndroidGLRenderer::LoadSceneResources(std::shared_ptr<ITurboScene> scene)
{
    ReleaseSceneResources();

    InitializeSceneResources();

    CreateSceneVertexResources(scene);
    CreateSceneTextureResources(scene);

    CreateShaders();

    _sceneResourcesLoaded = true;

    return true;
}

void AndroidGLRenderer::ReleaseSceneResources()
{
    DeleteBuffers();

    _sceneVertexBufferNames.clear();
    _sceneIndexBufferNames.clear();

    _sceneObjectOffsets.clear();
    _sceneObjectCount = 0;

    _sceneObjectMesh.clear();
    _sceneObjectMeshCount = 0;

    _sceneTextureBufferNames.clear();
    _sceneObjectTextureCount = 0;

    _sceneResourcesLoaded = false;
}

bool AndroidGLRenderer::RenderScene(std::shared_ptr<ITurboScene> scene)
{
    if (!_sceneResourcesLoaded)
    {
        LoadSceneResources(scene);
    }

    // Drawing is throttled to the screen update rate, so there
    // is no need to do timing here.
    UpdateFPS();

    UpdateProjectionMatrix();
    UpdateViewMatrix(scene->CameraPlacement(), scene->LightHack());

    InitializeRendering();
    RenderSceneObjects(scene);
    FinalizeRendering();

    return false;
}

void AndroidGLRenderer::Reset()
{
    ReleaseSceneResources();

    //_gl_context->Suspend();
    _gl_context->Invalidate();

    _resources_initialized = false;
}

//  UpdateDisplayInformation  ------------------------------------------------------------------------------------------

void AndroidGLRenderer::ShowUI()
{
    JNIEnv *jni;
    _android_app->activity->vm->AttachCurrentThread(&jni, NULL);

    // Default class retrieval
    jclass clazz = jni->GetObjectClass(_android_app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "showUI", "()V");
    jni->CallVoidMethod(_android_app->activity->clazz, methodID);

    _android_app->activity->vm->DetachCurrentThread();
}

//  LoadSceneResources  ------------------------------------------------------------------------------------------------

void AndroidGLRenderer::InitializeSceneResources()
{
    if (_gl_context->GetGLVersion() >= 3.0)
    {
        //  TODO: Optimize rendering to use instancing
        //geometry_instancing_support_ = true;
    }
    else if (_gl_context->CheckExtension("GL_NV_draw_instanced") &&
             _gl_context->CheckExtension("GL_NV_uniform_buffer_object"))
    {
        LOGI("Supported via extension!");
        //_bGeometryInstancingSupport = true;
        //_bARBSupport = true; //Need to patch shaders
        // Currently this has been disabled
    }

    // Settings
    glFrontFace(GL_CCW);
}

//	Scene Vertex Resources ---------------------------------------------------------------------------------------------

void AndroidGLRenderer::CreateSceneVertexResources(std::shared_ptr<ITurboScene> scene)
{
    if (scene == nullptr)
    {
        return;
    }

    //	Prepare data structures.
    _sceneObjectOffsets.clear();
    _sceneObjectMesh.clear();
    _sceneVertexBufferNames.clear();
    _sceneIndexBufferNames.clear();

    auto sceneObjects = scene->SceneObjects();

    for (auto& sceneObject : sceneObjects)
    {
        LoadSceneObjectVertices(sceneObject);
    }

    _sceneObjectCount = (GLuint)_sceneObjectOffsets.size();
    _sceneObjectMeshCount = (GLuint)_sceneObjectMesh.size();
}

void AndroidGLRenderer::LoadSceneObjectVertices(std::shared_ptr<ITurboSceneObject> sceneObject)
{
    GLuint sceneObjectOffset = (GLuint)_sceneObjectOffsets.size();
    _sceneObjectOffsets[sceneObject] = sceneObjectOffset;

    std::shared_ptr<ITurboSceneMesh> mesh = sceneObject->Mesh();

    if (mesh == nullptr)
    {
        return;
    }

    //  Already loaded this mesh? don't reload it.
    if (_sceneObjectMesh.find(mesh) != _sceneObjectMesh.end())
    {
        return;
    }

    GLuint meshOffset = (GLuint)_sceneObjectMesh.size();
    _sceneObjectMesh[mesh] = meshOffset;

    std::vector<SHADER_VERTEX> vertexList;
    std::vector<uint16_t> indexList;

    LoadVertexData(mesh, &vertexList, &indexList);

    //  Load mesh vertices ---------------------------------------------------------------------------------------------
    _sceneVertexCount[mesh] = (GLuint)vertexList.size();
    GLuint vertexBufferSize = (GLuint)(vertexList.size() * sizeof(vertexList[0]));
    GLuint vertexBufferName;

    glGenBuffers(1, &vertexBufferName);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferName);
    //glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, reinterpret_cast<u_char*>(vertexList.data()), GL_STATIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertexList.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    _sceneVertexBufferNames[mesh] = vertexBufferName;

    //	Load mesh indices ----------------------------------------------------------------------------------------------
    _sceneIndexCount[mesh] = (GLuint)indexList.size();
    GLuint indexBufferSize = (GLuint)(indexList.size() * sizeof(indexList[0]));
    GLuint indexBufferName;

    glGenBuffers(1, &indexBufferName);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferName);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, indexList.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    _sceneIndexBufferNames[mesh] = indexBufferName;

    //	Iterate over child scene objects. Call this method recursively.
    for (auto& childSceneObject : sceneObject->ChildSceneObjects())
    {
        LoadSceneObjectTextures(childSceneObject);
    }
}

void AndroidGLRenderer::LoadVertexData(
        std::shared_ptr<ITurboSceneMesh> mesh,
        std::vector<SHADER_VERTEX> *vertexList,
        std::vector<uint16_t> *indexList)
{
    for (auto& meshVertex : mesh->Vertices())
    {
        SHADER_VERTEX shaderVertex;

        shaderVertex.Position[0] = (float)(meshVertex.Position.X);
        shaderVertex.Position[1] = (float)(meshVertex.Position.Y);
        shaderVertex.Position[2] = (float)(meshVertex.Position.Z);

        shaderVertex.Normal[0] = (float)(meshVertex.Normal.X);
        shaderVertex.Normal[1] = (float)(meshVertex.Normal.Y);
        shaderVertex.Normal[2] = (float)(meshVertex.Normal.Z);

        shaderVertex.Color[0] = (float)(meshVertex.Color.X);
        shaderVertex.Color[1] = (float)(meshVertex.Color.Y);
        shaderVertex.Color[2] = (float)(meshVertex.Color.Z);

        shaderVertex.Texture[0] = (float)(meshVertex.TextureUV.X);
        shaderVertex.Texture[1] = (float)(meshVertex.TextureUV.Y);

        vertexList->push_back(shaderVertex);
    }

    for (auto& meshTriangle : mesh->Triangles())
    {
        indexList->push_back(meshTriangle.Vertex2);
        indexList->push_back(meshTriangle.Vertex1);
        indexList->push_back(meshTriangle.Vertex3);
    }
}

//	Scene Texture Resources --------------------------------------------------------------------------------------------

void AndroidGLRenderer::CreateSceneTextureResources(std::shared_ptr<ITurboScene> scene)
{
    if (scene == nullptr)
    {
        return;
    }

    //	Prepare data structures.
    _sceneTextureBufferNames.clear();
//    _sceneTextureTargetResources.clear();
//    _sceneTextureSourceResources.clear();

    auto sceneObjects = scene->SceneObjects();

    for (auto& sceneObject : sceneObjects)
    {
        LoadSceneObjectTextures(sceneObject);
    }

    _sceneObjectTextureCount = (GLuint)_sceneTextureBufferNames.size();
}

void AndroidGLRenderer::LoadSceneObjectTextures(std::shared_ptr<ITurboSceneObject> sceneObject)
{
    std::shared_ptr<ITurboSceneMaterial> material = sceneObject->Material();
    if (material == nullptr)
    {
        return;
    }

    std::shared_ptr<ITurboSceneTexture> texture = material->Texture();
    if (texture == nullptr)
    {
        return;
    }

    std::string textureName = texture->Name();
    if (textureName.empty())
    {
        return;
    }

    //  Already loaded this texture? don't reload it.
    if (_sceneTextureBufferNames.find(textureName) != _sceneTextureBufferNames.end())
    {
        return;
    }

    _debug->Send(debugDebug, debugRenderer) << "Loading texture: "  << textureName << "\n";

    GLsizei textureWidth;
    GLsizei textureHeight;
    std::vector<unsigned char> textureData;

    LoadTextureData(textureName, &textureWidth, &textureHeight, &textureData);

    _debug->Send(debugDebug, debugRenderer) << "Loading texture: "  << textureName << " (" << textureData.size() << ")\n";

    GLuint textureBufferName;

    glGenTextures(1, &textureBufferName);
    glBindTexture(GL_TEXTURE_2D, textureBufferName);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    _debug->Send(debugDebug, debugRenderer) << "Loading texture: "  << textureName << " (" << textureData.size() << ") (" << textureBufferName << ")\n";

    glTexImage2D(GL_TEXTURE_2D, 0,  // mip level
                 GL_RGBA,
                 textureWidth, textureHeight,
                 0,                // border color
                 GL_RGBA, GL_UNSIGNED_BYTE, textureData.data());

    glBindTexture(GL_TEXTURE_2D, 0);

    _sceneTextureBufferNames[textureName] = textureBufferName;

    _debug->Send(debugDebug, debugRenderer) << "Loaded texture: "  << textureName << "\n";

    //  Iterate over child scene objects. Call this method recursively.
    for (auto& childSceneObject : sceneObject->ChildSceneObjects())
    {
        LoadSceneObjectTextures(childSceneObject);
    }
}

void AndroidGLRenderer::LoadTextureData(
        std::string textureName,
        GLsizei *textureWidth,
        GLsizei *textureHeight,
        std::vector<unsigned char> *textureData)
{
    std::shared_ptr<ITurboCanvas> canvas = std::shared_ptr<ITurboCanvas>(new TurboCanvasRGBA32());
    std::vector<unsigned char> fileData = _ioService->ReadData(ToWString("Textures/" + textureName + ".bmp"));
    std::shared_ptr<ITurboImage> bitmap = std::shared_ptr<ITurboImage>(new TurboBitmap(canvas.get(), fileData.data()));
    bitmap->Draw();

    unsigned char *canvasData = (unsigned char *)canvas->Data();
    int canvasDataSize = canvas->DataSize();
    textureData->assign(canvasData, canvasData + canvasDataSize);

    *textureWidth = canvas->Width();
    *textureHeight = canvas->Height();
}

//  Shaders  -----------------------------------------------------------------------------------------------------------

void AndroidGLRenderer::CreateShaders()
{
    if (geometry_instancing_support_)
    {
        //
        // Create parameter dictionary for shader patch
        std::map<std::string, std::string> param;
        param[std::string("%NUM_TEAPOT%")] =
                //ToString(teapot_x_ * teapot_y_ * teapot_z_);
                ToString(256);
        param[std::string("%LOCATION_VERTEX%")] = ToString(ATTRIB_VERTEX);
        param[std::string("%LOCATION_NORMAL%")] = ToString(ATTRIB_NORMAL);
        if (arb_support_)
            param[std::string("%ARB%")] = std::string("ARB");
        else
            param[std::string("%ARB%")] = std::string("");

        // Load shader
        bool shadersLoaded = LoadShadersES3(&_shaderParams,
                                            "Shaders/VS_ShaderPlainES3.vsh",
                                            "Shaders/ShaderPlainES3.fsh", param);
        if (shadersLoaded)
        {
            //
            // Create uniform buffer
            //
            GLuint bindingPoint = 1;
            GLuint blockIndex;
            blockIndex = glGetUniformBlockIndex(_shaderParams.program, "ParamBlock");
            glUniformBlockBinding(_shaderParams.program, blockIndex, bindingPoint);

            // Retrieve array stride value
            int32_t num_indices;
            glGetActiveUniformBlockiv(_shaderParams.program, blockIndex,
                                      GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &num_indices);
            GLint indices[num_indices];
            GLint stride[num_indices];
            glGetActiveUniformBlockiv(_shaderParams.program, blockIndex,
                                      GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, indices);
            glGetActiveUniformsiv(_shaderParams.program, num_indices, (GLuint *) indices,
                                  GL_UNIFORM_ARRAY_STRIDE, stride);

            ubo_matrix_stride_ = stride[0] / sizeof(float);
            ubo_vector_stride_ = stride[2] / sizeof(float);

            glGenBuffers(1, &_sceneUniformBufferName);
            glBindBuffer(GL_UNIFORM_BUFFER, _sceneUniformBufferName);
            glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, _sceneUniformBufferName);

            // Store color value which wouldn't be updated every frame
            int32_t size = _sceneObjectCount *
                           (ubo_matrix_stride_ + ubo_matrix_stride_ +
                            ubo_vector_stride_);  // Mat4 + Mat4 + Vec3 + 1 stride
            float *pBuffer = new float[size];
            float *pColor = pBuffer + _sceneObjectCount * ubo_matrix_stride_ * 2;
            for (int32_t i = 0; i < _sceneObjectCount; ++i)
            {
                memcpy(pColor, &vec_colors_[i], 3 * sizeof(float));
                pColor += ubo_vector_stride_;  // Assuming std140 layout which is 4
                // DWORD stride for vectors
            }

            glBufferData(GL_UNIFORM_BUFFER, size * sizeof(float), pBuffer,
                         GL_DYNAMIC_DRAW);
            delete[] pBuffer;
        }
        else
        {
            LOGI("Shader compilation failed!! Falls back to ES2.0 pass");
            // This happens some devices.
            geometry_instancing_support_ = false;
            // Load shader for GLES2.0
            LoadShaders(&_shaderParams,
                        "Shaders/VertexShader_100.vsh",
                        "Shaders/PixelShader_100.fsh");
        }
    }
    else
    {
        // Load shader for GLES2.0
        LoadShaders(&_shaderParams,
                    "Shaders/VertexShader_100.vsh",
                    "Shaders/PixelShader_100.fsh");
    }
}

bool AndroidGLRenderer::LoadShaders(SHADER_PARAMS *params,
                                    const char *vertexShaderName,
                                    const char *fragmentShaderName)
{
    //
    // Shader load for GLES2
    // In GLES2.0, shader attribute locations need to be explicitly specified
    // before linking
    //
    GLuint program;
    GLuint vertexShader, pixelShader;

    // Create shader program
    program = glCreateProgram();
    LOGI("Created Shader %d", program);

    // Create and compile vertex shader
    if (!ndk_helper::shader::CompileShader(&vertexShader, GL_VERTEX_SHADER,
                                           vertexShaderName))
    {
        LOGI("Failed to compile vertex shader");
        glDeleteProgram(program);
        return false;
    }

    // Create and compile fragment shader
    if (!ndk_helper::shader::CompileShader(&pixelShader, GL_FRAGMENT_SHADER,
                                           fragmentShaderName))
    {
        LOGI("Failed to compile fragment shader");
        glDeleteProgram(program);
        return false;
    }

    // Attach vertex shader to program
    glAttachShader(program, vertexShader);

    // Attach fragment shader to program
    glAttachShader(program, pixelShader);

    // Bind attribute locations
    // this needs to be done prior to linking
    glBindAttribLocation(program, ATTRIB_VERTEX, "vsPosition");
    glBindAttribLocation(program, ATTRIB_NORMAL, "vsNormal");
    glBindAttribLocation(program, ATTRIB_COLOR,  "vsColor");
    glBindAttribLocation(program, ATTRIB_UV,     "vsTexture");

    // Link program
    if (!ndk_helper::shader::LinkProgram(program))
    {
        LOGI("Failed to link program: %d", program);

        if (vertexShader)
        {
            glDeleteShader(vertexShader);
            vertexShader = 0;
        }
        if (pixelShader)
        {
            glDeleteShader(pixelShader);
            pixelShader = 0;
        }
        if (program)
        {
            glDeleteProgram(program);
        }
        return false;
    }

    // Get uniform locations
    params->TextureSampler    = glGetUniformLocation(program, "uSampler");
    params->ModelMatrix       = glGetUniformLocation(program, "uModel");
    params->ViewMatrix        = glGetUniformLocation(program, "uView");
    params->ProjectionMatrix  = glGetUniformLocation(program, "uProjection");
    params->LightCount        = glGetUniformLocation(program, "uLightCount");

//    params->light0_ = glGetUniformLocation(program, "uLight0");
//    params->material_diffuse_ = glGetUniformLocation(program, "uMaterialDiffuse");
//    params->material_ambient_ = glGetUniformLocation(program, "uMaterialAmbient");
//    params->material_specular_ = glGetUniformLocation(program, "uMaterialSpecular");

    // Release vertex and fragment shaders
    if (vertexShader) glDeleteShader(vertexShader);
    if (pixelShader) glDeleteShader(pixelShader);

    params->program = program;
    return true;
}

bool AndroidGLRenderer::LoadShadersES3(
        SHADER_PARAMS *params, const char *vertexShaderName, const char *fragmentShaderName,
        std::map<std::string, std::string> &shaderParams)
{
    //
    // Shader load for GLES3
    // In GLES3.0, shader attribute index can be described in a shader code
    // directly with layout() attribute
    //
    GLuint program;
    GLuint vertexShader, fragmentShader;

    // Create shader program
    program = glCreateProgram();
    LOGI("Created Shader %d", program);

    // Create and compile vertex shader
    if (!ndk_helper::shader::CompileShader(&vertexShader, GL_VERTEX_SHADER, vertexShaderName,
                                           shaderParams))
    {
        LOGI("Failed to compile vertex shader");
        glDeleteProgram(program);
        return false;
    }

    // Create and compile fragment shader
    if (!ndk_helper::shader::CompileShader(&fragmentShader, GL_FRAGMENT_SHADER,
                                           fragmentShaderName, shaderParams))
    {
        LOGI("Failed to compile fragment shader");
        glDeleteProgram(program);
        return false;
    }

    // Attach vertex shader to program
    glAttachShader(program, vertexShader);

    // Attach fragment shader to program
    glAttachShader(program, fragmentShader);

    // Link program
    if (!ndk_helper::shader::LinkProgram(program))
    {
        LOGI("Failed to link program: %d", program);

        if (vertexShader) {
            glDeleteShader(vertexShader);
            vertexShader = 0;
        }
        if (fragmentShader) {
            glDeleteShader(fragmentShader);
            fragmentShader = 0;
        }
        if (program) {
            glDeleteProgram(program);
        }

        return false;
    }

    // Get uniform locations
//    params->light0_ = glGetUniformLocation(program, "vLight0");
//    params->material_ambient_ = glGetUniformLocation(program, "vMaterialAmbient");
//    params->material_specular_ = glGetUniformLocation(program, "vMaterialSpecular");

    // Release vertex and fragment shaders
    if (vertexShader) glDeleteShader(vertexShader);
    if (fragmentShader) glDeleteShader(fragmentShader);

    params->program = program;
    return true;
}

//  ReleaseSceneResources  ---------------------------------------------------------------------------------------------

void AndroidGLRenderer::DeleteBuffers()
{
    for (auto& entry : _sceneVertexBufferNames)
    {
        GLuint vertexBufferName = entry.second;
        if (vertexBufferName)
        {
            glDeleteBuffers(1, &vertexBufferName);
        }
    }
    _sceneVertexBufferNames.clear();

    for (auto& entry : _sceneIndexBufferNames)
    {
        GLuint indexBufferName = entry.second;
        if (indexBufferName)
        {
            glDeleteBuffers(1, &indexBufferName);
        }
    }
    _sceneIndexBufferNames.clear();

    if (_sceneUniformBufferName)
    {
        glDeleteBuffers(1, &_sceneUniformBufferName);
        _sceneUniformBufferName = 0;
    }

    if (_shaderParams.program)
    {
        glDeleteProgram(_shaderParams.program);
        _shaderParams.program = 0;
    }

    for (auto& entry : _sceneTextureBufferNames)
    {
        GLuint textureBufferName = entry.second;
        if (textureBufferName)
        {
            glDeleteTextures(1, &textureBufferName);
        }
    }
    _sceneTextureBufferNames.clear();
}

//  RenderScene  -------------------------------------------------------------------------------------------------------

void AndroidGLRenderer::UpdateFPS()
{
    float fps;
    if (!_performance_monitor.Update(fps))
    {
        return;
    }

    JNIEnv *jni;
    _android_app->activity->vm->AttachCurrentThread(&jni, NULL);

    // Default class retrieval
    jclass clazz = jni->GetObjectClass(_android_app->activity->clazz);
    jmethodID methodID = jni->GetMethodID(clazz, "updateFPS", "(F)V");
    jni->CallVoidMethod(_android_app->activity->clazz, methodID, fps);

    _android_app->activity->vm->DetachCurrentThread();
}

void AndroidGLRenderer::UpdateProjectionMatrix()
{
    int32_t viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    float width  = static_cast<float>(viewport[2]);
    float height = static_cast<float>(viewport[3]);

    _projectionMatrix = ndk_helper::Mat4::Perspective(75.0f, width, height, 0.01f, 100.0f);
}

void AndroidGLRenderer::UpdateViewMatrix(std::shared_ptr<ITurboScenePlacement> cameraPlacement, bool lightHack)
{
    TurboVector3D position = cameraPlacement->Position();
    TurboVector3D target = cameraPlacement->Target();
    TurboVector3D up = cameraPlacement->Up();
//    TurboVector3D front = -cameraPlacement->Back();

    ndk_helper::Vec3 eyePosition   = ndk_helper::Vec3((float)(position.X), (float)(position.Y), (float)(position.Z));
    ndk_helper::Vec3 focusPosition = ndk_helper::Vec3((float)(target.X), (float)(target.Y), (float)(target.Z));
    ndk_helper::Vec3 upDirection   = ndk_helper::Vec3((float)(up.X), (float)(up.Y), (float)(up.Z));
//    ndk_helper::Vec3 eyeDirection  = ndk_helper::Vec3((float)(front.X), (float)(front.Y), (float)(front.Z));

//    const float CAM_X = 0.f;
//    const float CAM_Y = 0.f;
//    const float CAM_Z = 2000.f;

    _viewMatrix = ndk_helper::Mat4::LookAt(eyePosition, focusPosition, upDirection);

//    _tap_camera.Update();
//    _viewMatrix = _tap_camera.GetTransformMatrix() * _viewMatrix *
//                  _tap_camera.GetRotationMatrix();

    _lightCount = lightHack ? 1 : 0;
}


void AndroidGLRenderer::InitializeRendering()
{
    // Just fill the screen with a color.
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void AndroidGLRenderer::RenderSceneObjects(std::shared_ptr<ITurboScene> scene)
{
    if (scene == nullptr)
    {
        return;
    }

    auto sceneObjects = scene->SceneObjects();

    for (auto& sceneObject : sceneObjects)
    {
        RenderSceneObject(sceneObject);
    }
}

void AndroidGLRenderer::RenderSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject)
{
    std::shared_ptr<ITurboSceneMesh> mesh = sceneObject->Mesh();

    if (mesh == nullptr)
    {
        return;
    }

    GLuint vertexBufferName = _sceneVertexBufferNames[mesh];
//    GLuint vertexCount = _sceneVertexCount[mesh];
    GLuint indexBufferName = _sceneIndexBufferNames[mesh];
    GLuint indexCount = _sceneIndexCount[mesh];

    // Bind the Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferName);

    int32_t stride = sizeof(SHADER_VERTEX);
    // Pass the vertex data
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(ATTRIB_VERTEX);

    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(ATTRIB_NORMAL);

    glVertexAttribPointer(ATTRIB_COLOR,  3, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(ATTRIB_COLOR);

    glVertexAttribPointer(ATTRIB_UV,     2, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(9 * sizeof(GLfloat)));
    glEnableVertexAttribArray(ATTRIB_UV);

    // Bind the Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferName);

    std::string textureName = sceneObject->Material()->Texture()->Name();
    GLuint textureBufferName = _sceneTextureBufferNames[textureName];
    glBindTexture(GL_TEXTURE_2D, textureBufferName);

    glUseProgram(_shaderParams.program);

//    std::shared_ptr<ITurboSceneMaterial> material = sceneObject->Material();

//    TurboColor ambientColor = material->AmbientColor();
//    TurboColor specularColor = material->SpecularColor();
//    float specularExponent = material->SpecularExponent();

    // Update uniforms
    //glUniform1i(_shaderParams.TextureSampler, 0);

    glUniformMatrix4fv(_shaderParams.ViewMatrix, 1, GL_FALSE, _viewMatrix.Ptr());
    glUniformMatrix4fv(_shaderParams.ProjectionMatrix, 1, GL_FALSE, _projectionMatrix.Ptr());

    glUniform1i(_shaderParams.LightCount, _lightCount);

    // (using glUniform3fv here was troublesome..)
//    glUniform3f(_shaderParams.material_ambient_,  ambientColor.R,  ambientColor.G,  ambientColor.B);
//    glUniform4f(_shaderParams.material_specular_, specularColor.R, specularColor.G, specularColor.B, specularExponent);
//    glUniform3f(_shaderParams.light0_, 100.f, -200.f, -600.f);

    //  TODO: Optimize to use instancing
    //  Loop by mesh first, then all scene objects that use that mesh, then render them as a batch.
    if (geometry_instancing_support_)
    {
        //
        // Geometry instancing, new feature in GLES3.0
        //

        // Update UBO
        glBindBuffer(GL_UNIFORM_BUFFER, _sceneUniformBufferName);
        float *range = (float *) glMapBufferRange(GL_UNIFORM_BUFFER, 0,
                                              _sceneObjectCount * (ubo_matrix_stride_ * 2) * sizeof(float),
                                              GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);
        float *mat_mvp = range;
        float *mat_mv = range + _sceneObjectCount * ubo_matrix_stride_;
        for (int32_t i = 0; i < _sceneObjectCount; ++i)
        {
            // Rotation
            float x, y;
            vec_current_rotations_[i] += vec_rotations_[i];
            vec_current_rotations_[i].Value(x, y);
            ndk_helper::Mat4 mat_rotation = ndk_helper::Mat4::RotationX(x) * ndk_helper::Mat4::RotationY(y);

            // Feed Projection and Model View matrices to the shaders
            ndk_helper::Mat4 mat_v = _viewMatrix * vec_mat_models_[i] * mat_rotation;
            ndk_helper::Mat4 mat_vp = _projectionMatrix * mat_v;

            memcpy(mat_mvp, mat_vp.Ptr(), sizeof(mat_v));
            mat_mvp += ubo_matrix_stride_;

            memcpy(mat_mv, mat_v.Ptr(), sizeof(mat_v));
            mat_mv += ubo_matrix_stride_;
        }
        glUnmapBuffer(GL_UNIFORM_BUFFER);

        // Instanced rendering
        glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0), _sceneObjectCount);
    }
    else
    {
        //TurboColor diffuseColor = material->DiffuseColor();
        //glUniform4f(_shaderParams.material_diffuse_, diffuseColor.R, diffuseColor.G, diffuseColor.B, 1.f);


        TurboMatrix4x4 transform = sceneObject->Placement()->Transform();
        float mat[16] {(float)transform.M11, (float)transform.M12, (float)transform.M13, (float)transform.M14,
                       (float)transform.M21, (float)transform.M22, (float)transform.M23, (float)transform.M24,
                       (float)transform.M31, (float)transform.M32, (float)transform.M33, (float)transform.M34,
                       (float)transform.M41, (float)transform.M42, (float)transform.M43, (float)transform.M44};
        ndk_helper::Mat4 model = ndk_helper::Mat4(mat);
//        ndk_helper::Mat4 view = _viewMatrix * model;
//        ndk_helper::Mat4 projection = _projectionMatrix * view;
//
//        glUniformMatrix4fv(_shaderParams.ModelMatrix, 1, GL_FALSE, model.Ptr());
//        glUniformMatrix4fv(_shaderParams.ViewMatrix, 1, GL_FALSE, view.Ptr());
//        glUniformMatrix4fv(_shaderParams.ProjectionMatrix, 1, GL_FALSE, projection.Ptr());

        glUniformMatrix4fv(_shaderParams.ModelMatrix, 1, GL_FALSE, model.Ptr());

        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //	Iterate over child scene objects. Call this method recursively.
    for (auto &childSceneObject : sceneObject->ChildSceneObjects())
    {
        RenderSceneObject(childSceneObject);
    }
}

void AndroidGLRenderer::FinalizeRendering()
{
    // Swap
    if (EGL_SUCCESS != _gl_context->Swap())
    {
        ReleaseSceneResources();
    }
}

//--------------------------------------------------------------------------------
// Helper functions
//--------------------------------------------------------------------------------
std::string AndroidGLRenderer::ToString(const int32_t i)
{
    char str[64];
    snprintf(str, sizeof(str), "%d", i);
    return std::string(str);
}
