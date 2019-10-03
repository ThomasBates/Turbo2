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
// OpenGLESRenderer.cpp
// Render teapots
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
// Include files
//--------------------------------------------------------------------------------

#include <chrono>
#include <thread>
#include <cstring>

#include <ITurboCanvas.h>
#include <ITurboImage.h>

#include <TurboBitmap.h>
#include <TurboCanvasRGBA32.h>
#include <TurboCoreHelpers.h>
#include <TurboSceneMesh.h>

#include <OpenGLESRenderer.h>
#include <ITurboGameControllerViewModel.h>

using namespace Turbo::Core;
using namespace Turbo::Graphics;
using namespace Turbo::Platform::OpenGLES;

//  Constructors & Destructors  ----------------------------------------------------------------------------------------

OpenGLESRenderer::OpenGLESRenderer(
        android_app* app,
        std::shared_ptr<ITurboDebug> debug,
        std::shared_ptr<ITurboGameIOService> ioService,
        std::shared_ptr<ITurboGameControllerViewModel> controllerViewModel) :
        _android_app(app),
        _debug(debug),
        _ioService(ioService),
        _controllerViewModel(controllerViewModel)
{
    _gl_context = OpenGLESContext::GetInstance();
}

OpenGLESRenderer::~OpenGLESRenderer()
{
    ReleaseSceneResources();
}

//	ITurboGameRenderer Methods ---------------------------------------------------------------------

void OpenGLESRenderer::UpdateDisplayInformation()
{
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

    // Initialize GL state.
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Note that screen size might have been changed
    int width = _gl_context->GetScreenWidth();
    int height = _gl_context->GetScreenHeight();
    glViewport(0, 0, width, height);
}

bool OpenGLESRenderer::LoadSceneResources(std::shared_ptr<ITurboScene> scene)
{
    if (scene == nullptr)
    {
        return false;
    }

    ReleaseSceneResources();

    InitializeSceneResources();

    CreateSceneVertexResources(scene);
    CreateSceneTextureResources(scene);

    CreateShaders();

    _sceneResourcesLoaded = true;

    return true;
}

void OpenGLESRenderer::ReleaseSceneResources()
{
    DeleteBuffers();

    _sceneMeshInfo.clear();
    _sceneTextureBufferNames.clear();

    _sceneResourcesLoaded = false;
}

bool OpenGLESRenderer::RenderScene(std::shared_ptr<ITurboScene> scene)
{
    if (scene == nullptr)
    {
        return false;
    }

    if (!_sceneResourcesLoaded)
    {
        LoadSceneResources(scene);
    }

    UpdateProjectionMatrix();
    UpdateViewMatrix(scene->CameraPlacement(), scene->LightHack());

    InitializeRendering();
    RenderSceneObjects(scene);
    FinalizeRendering();

    return false;
}

void OpenGLESRenderer::Reset()
{
    ReleaseSceneResources();

    _gl_context->Invalidate();

    _resources_initialized = false;
}

//  LoadSceneResources  ------------------------------------------------------------------------------------------------

void OpenGLESRenderer::InitializeSceneResources()
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

void OpenGLESRenderer::CreateSceneVertexResources(std::shared_ptr<ITurboScene> scene)
{
    if (scene == nullptr)
    {
        return;
    }

    //	Prepare data structures.
    _sceneMeshInfo.clear();

    auto sceneObjects = scene->SceneObjects();

    for (auto& sceneObject : sceneObjects)
    {
        LoadSceneObjectVertices(sceneObject);
    }

    LoadSceneSpriteVertices();
}

void OpenGLESRenderer::LoadSceneObjectVertices(std::shared_ptr<ITurboSceneObject> sceneObject)
{
    std::shared_ptr<ITurboSceneMesh> mesh = sceneObject->Mesh();

    if (mesh == nullptr)
    {
        return;
    }

    //  Already loaded this mesh? don't reload it.
    if (_sceneMeshInfo.find(mesh) != _sceneMeshInfo.end())
    {
        return;
    }

    std::vector<SHADER_VERTEX> vertexList;
    std::vector<uint16_t> indexList;

    LoadVertexData(mesh, &vertexList, &indexList);

    MeshInfo meshInfo {};

    //  Load mesh vertices ---------------------------------------------------------------------------------------------
    meshInfo.VertexCount = (GLuint)vertexList.size();
    GLuint vertexBufferSize = (GLuint)(vertexList.size() * sizeof(vertexList[0]));
    GLuint vertexBufferName;

    glGenBuffers(1, &vertexBufferName);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferName);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertexList.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    meshInfo.VertexBufferName = vertexBufferName;

    //	Load mesh indices ----------------------------------------------------------------------------------------------
    meshInfo.IndexCount = (GLuint)indexList.size();
    GLuint indexBufferSize = (GLuint)(indexList.size() * sizeof(indexList[0]));
    GLuint indexBufferName;

    glGenBuffers(1, &indexBufferName);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferName);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, indexList.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    meshInfo.IndexBufferName = indexBufferName;

    _sceneMeshInfo[mesh] = meshInfo;

    //	Iterate over child scene objects. Call this method recursively.
    for (auto& childSceneObject : sceneObject->ChildSceneObjects())
    {
        LoadSceneObjectTextures(childSceneObject);
    }
}

void OpenGLESRenderer::LoadSceneSpriteVertices()
{
    std::shared_ptr<ITurboSceneMesh> mesh = std::shared_ptr<ITurboSceneMesh>(new TurboSceneMesh());

    TurboVector3D normal = TurboVector3D( 0.0,  0.0,  1.0);
    mesh->AddVertex(TurboVector3D(-1.0, -1.0,  0.0), normal, TurboVector2D(0.0, 0.0));
    mesh->AddVertex(TurboVector3D(-1.0,  1.0,  0.0), normal, TurboVector2D(0.0, 1.0));
    mesh->AddVertex(TurboVector3D( 1.0,  1.0,  0.0), normal, TurboVector2D(1.0, 1.0));
    mesh->AddVertex(TurboVector3D( 1.0, -1.0,  0.0), normal, TurboVector2D(1.0, 0.0));
//    mesh->AddVertex(TurboVector3D( 0.0, -1.0,  0.0), normal, TurboVector2D(0.0, 0.0));
//    mesh->AddVertex(TurboVector3D( 0.0, -0.4,  0.0), normal, TurboVector2D(0.0, 1.0));
//    mesh->AddVertex(TurboVector3D( 1.0, -0.4,  0.0), normal, TurboVector2D(1.0, 1.0));
//    mesh->AddVertex(TurboVector3D( 1.0, -1.0,  0.0), normal, TurboVector2D(1.0, 0.0));

    mesh->AddTriangle(0, 1, 2);
    mesh->AddTriangle(2, 3, 0);

    std::vector<SHADER_VERTEX> vertexList;
    std::vector<uint16_t> indexList;

    LoadVertexData(mesh, &vertexList, &indexList);

    MeshInfo meshInfo {};

    //  Load mesh vertices ---------------------------------------------------------------------------------------------
    meshInfo.VertexCount = (GLuint)vertexList.size();
    GLuint vertexBufferSize = (GLuint)(vertexList.size() * sizeof(vertexList[0]));
    GLuint vertexBufferName;

    glGenBuffers(1, &vertexBufferName);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferName);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertexList.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    meshInfo.VertexBufferName = vertexBufferName;

    //	Load mesh indices ----------------------------------------------------------------------------------------------
    meshInfo.IndexCount = (GLuint)indexList.size();
    GLuint indexBufferSize = (GLuint)(indexList.size() * sizeof(indexList[0]));
    GLuint indexBufferName;

    glGenBuffers(1, &indexBufferName);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferName);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, indexList.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    meshInfo.IndexBufferName = indexBufferName;

    _spriteMeshInfo = meshInfo;
}

void OpenGLESRenderer::LoadVertexData(
        std::shared_ptr<ITurboSceneMesh> mesh,
        std::vector<SHADER_VERTEX> *vertexList,
        std::vector<uint16_t> *indexList)
{
    for (auto& meshVertex : mesh->Vertices())
    {
        SHADER_VERTEX shaderVertex {};

        shaderVertex.Position[0] = meshVertex.Position.X;
        shaderVertex.Position[1] = meshVertex.Position.Y;
        shaderVertex.Position[2] = meshVertex.Position.Z;

        shaderVertex.Normal[0]   = meshVertex.Normal.X;
        shaderVertex.Normal[1]   = meshVertex.Normal.Y;
        shaderVertex.Normal[2]   = meshVertex.Normal.Z;

        shaderVertex.Color[0]    = meshVertex.Color.X;
        shaderVertex.Color[1]    = meshVertex.Color.Y;
        shaderVertex.Color[2]    = meshVertex.Color.Z;

        shaderVertex.Texture[0]  = meshVertex.TextureUV.X;
        shaderVertex.Texture[1]  = meshVertex.TextureUV.Y;

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

void OpenGLESRenderer::CreateSceneTextureResources(std::shared_ptr<ITurboScene> scene)
{
    if (scene == nullptr)
    {
        return;
    }

    //	Prepare data structures.
    _sceneTextureBufferNames.clear();

    auto sceneObjects = scene->SceneObjects();

    for (auto& sceneObject : sceneObjects)
    {
        LoadSceneObjectTextures(sceneObject);
    }

    auto sceneSprites = scene->SceneSprites();

    for (auto& sceneSprite : sceneSprites)
    {
        LoadSceneSpriteTextures(sceneSprite);
    }
}

void OpenGLESRenderer::LoadSceneObjectTextures(std::shared_ptr<ITurboSceneObject> sceneObject)
{
    std::shared_ptr<ITurboSceneMaterial> material = sceneObject->Material();
    if (material == nullptr)
    {
        return;
    }

    LoadSceneTexture(material->Texture());

    //  Iterate over child scene objects. Call this method recursively.
    for (auto& childSceneObject : sceneObject->ChildSceneObjects())
    {
        LoadSceneObjectTextures(childSceneObject);
    }
}

void OpenGLESRenderer::LoadSceneSpriteTextures(std::shared_ptr<ITurboSceneSprite> sceneSprite)
{
    LoadSceneTexture(sceneSprite->Texture());
}

void OpenGLESRenderer::LoadSceneTexture(std::shared_ptr<ITurboSceneTexture> texture)
{
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
}

void OpenGLESRenderer::LoadTextureData(
        std::string textureName,
        GLsizei *textureWidth,
        GLsizei *textureHeight,
        std::vector<unsigned char> *textureData)
{
    std::shared_ptr<ITurboCanvas> canvas = std::shared_ptr<ITurboCanvas>(new TurboCanvasRGBA32());
    std::vector<unsigned char> fileData = _ioService->ReadData(ToWString("Textures/" + textureName + ".bmp"));
    std::shared_ptr<ITurboImage> bitmap = std::shared_ptr<ITurboImage>(new TurboBitmap(canvas.get(), fileData.data()));
    bitmap->Draw();

    auto *canvasData = (unsigned char *)canvas->Data();
    auto canvasDataSize = canvas->DataSize();
    textureData->assign(canvasData, canvasData + canvasDataSize);

    *textureWidth = canvas->Width();
    *textureHeight = canvas->Height();
}

//  Shaders  -----------------------------------------------------------------------------------------------------------

void OpenGLESRenderer::CreateShaders()
{
    // Load shader for GLES2.0
    LoadShaders(&_shaderParams,
                L"Shaders/VertexShader_100.vsh",
                L"Shaders/PixelShader_100.fsh");
}

bool OpenGLESRenderer::LoadShaders(SHADER_PARAMS *params,
                                   std::wstring vertexShaderName,
                                   std::wstring pixelShaderName)
{
    //
    // Shader load for GLES2
    // In GLES2.0, shader attribute locations need to be explicitly specified
    // before linking
    //
    GLuint program;
    GLuint vertexShader;
    GLuint pixelShader;

    // Create shader program
    program = glCreateProgram();
    LOGI("Created Shader %d", program);

    // Create and compile vertex shader
    if (!CompileShader(&vertexShader, GL_VERTEX_SHADER, vertexShaderName))
    {
        LOGI("Failed to compile vertex shader");
        glDeleteProgram(program);
        return false;
    }

    // Create and compile fragment shader
    if (!CompileShader(&pixelShader, GL_FRAGMENT_SHADER, pixelShaderName))
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
    if (!LinkProgram(program))
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
    params->IsSprite          = glGetUniformLocation(program, "uIsSprite");

    // Release vertex and fragment shaders
    if (vertexShader) glDeleteShader(vertexShader);
    if (pixelShader) glDeleteShader(pixelShader);

    params->program = program;
    return true;
}

bool OpenGLESRenderer::CompileShader(GLuint *shader, const GLenum type, std::wstring strFileName)
{
    auto data = _ioService->ReadData(strFileName);
    if (data.empty())
    {
        LOGI("Can not open a file:%s", Core::ToString(strFileName).data());
        return false;
    }

    const GLchar *source = (GLchar *)&data[0];
    auto size = (GLint)data.size();

    if (source == NULL || size <= 0) return false;

    *shader = glCreateShader(type);
    glShaderSource(*shader, 1, &source, &size);  // Not specifying 3rd parameter
    // (size) could be troublesome..

    glCompileShader(*shader);

//#if defined(DEBUG)
    GLint logLength;
    glGetShaderiv(*shader, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetShaderInfoLog(*shader, logLength, &logLength, log);
        LOGI("Shader compile log:\n%s", log);
        free(log);
    }
//#endif

    GLint status;
    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);
    if (status == 0)
    {
        glDeleteShader(*shader);
        return false;
    }

    return true;
}

bool OpenGLESRenderer::LinkProgram(const GLuint prog)
{
    GLint status;

    glLinkProgram(prog);

//#if defined(DEBUG)
    GLint logLength;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLength);
    if (logLength > 0)
    {
        GLchar *log = (GLchar *)malloc(logLength);
        glGetProgramInfoLog(prog, logLength, &logLength, log);
        LOGI("Program link log:\n%s", log);
        free(log);
    }
//#endif

    glGetProgramiv(prog, GL_LINK_STATUS, &status);
    if (status == 0)
    {
        LOGI("Program link failed\n");
        return false;
    }

    return true;
}

//  ReleaseSceneResources  ---------------------------------------------------------------------------------------------

void OpenGLESRenderer::DeleteBuffers()
{
    for (auto& entry : _sceneMeshInfo)
    {
        MeshInfo meshInfo = entry.second;

        if (meshInfo.VertexBufferName)
        {
            glDeleteBuffers(1, &meshInfo.VertexBufferName);
        }

        if (meshInfo.IndexBufferName)
        {
            glDeleteBuffers(1, &meshInfo.IndexBufferName);
        }
    }
    _sceneMeshInfo.clear();

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

void OpenGLESRenderer::UpdateProjectionMatrix()
{
    int32_t viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    auto width  = static_cast<float>(viewport[2]);
    auto height = static_cast<float>(viewport[3]);

    _projectionMatrix = MakePerspectiveProjection(90.0, width, height, 0.01, 100.0);
}

void OpenGLESRenderer::UpdateViewMatrix(std::shared_ptr<ITurboScenePlacement> cameraPlacement, bool lightHack)
{
    TurboVector3D position = cameraPlacement->Position();
    TurboVector3D target = cameraPlacement->Target();
    TurboVector3D up = cameraPlacement->Up();

    _viewMatrix = MakeViewProjection(position, target, up);

    _lightCount = lightHack ? 1 : 0;
}


void OpenGLESRenderer::InitializeRendering()
{
    // Draw objects back to front
    //glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glDepthMask(GL_FALSE);


    // Just fill the screen with a color.
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void OpenGLESRenderer::RenderSceneObjects(std::shared_ptr<ITurboScene> scene)
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

    auto sceneSprites = scene->SceneSprites();

    for (auto& sceneSprite : sceneSprites)
    {
        RenderSceneSprite(sceneSprite);
    }
}

void OpenGLESRenderer::RenderSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject)
{
    std::shared_ptr<ITurboSceneMesh> mesh = sceneObject->Mesh();

    if (mesh == nullptr)
    {
        return;
    }

    MeshInfo meshInfo = _sceneMeshInfo[mesh];

    // Bind the Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, meshInfo.VertexBufferName);

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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshInfo.IndexBufferName);

    std::string textureName = sceneObject->Material()->Texture()->Name();
    GLuint textureBufferName = _sceneTextureBufferNames[textureName];
    glBindTexture(GL_TEXTURE_2D, textureBufferName);

    glUseProgram(_shaderParams.program);

//    std::shared_ptr<ITurboSceneMaterial> material = sceneObject->Material();

//    TurboColor ambientColor = material->AmbientColor();
//    TurboColor specularColor = material->SpecularColor();
//    float specularExponent = material->SpecularExponent();

    // Update uniforms
    glUniformMatrix4fv(_shaderParams.ViewMatrix,       1, GL_FALSE, _viewMatrix.Ptr());
    glUniformMatrix4fv(_shaderParams.ProjectionMatrix, 1, GL_FALSE, _projectionMatrix.Ptr());

    glUniform1i(_shaderParams.LightCount, _lightCount);
    glUniform1i(_shaderParams.IsSprite, 0);

    // (using glUniform3fv here was troublesome..)
//    glUniform3f(_shaderParams.material_ambient_,  ambientColor.R,  ambientColor.G,  ambientColor.B);
//    glUniform4f(_shaderParams.material_specular_, specularColor.R, specularColor.G, specularColor.B, specularExponent);
//    glUniform3f(_shaderParams.light0_, 100.f, -200.f, -600.f);

    //  TODO: Optimize to use instancing
    //  Loop by mesh first, then all scene objects that use that mesh, then render them as a batch.

    TurboMatrix4x4 model = sceneObject->Placement()->Transform();
    glUniformMatrix4fv(_shaderParams.ModelMatrix, 1, GL_FALSE, model.Ptr());

    glDrawElements(GL_TRIANGLES, meshInfo.IndexCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //	Iterate over child scene objects. Call this method recursively.
    for (auto &childSceneObject : sceneObject->ChildSceneObjects())
    {
        RenderSceneObject(childSceneObject);
    }
}

void OpenGLESRenderer::RenderSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite)
{
    MeshInfo meshInfo = _spriteMeshInfo;

    // Bind the Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, meshInfo.VertexBufferName);

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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshInfo.IndexBufferName);

    std::string textureName = sceneSprite->Texture()->Name();
    GLuint textureBufferName = _sceneTextureBufferNames[textureName];
    glBindTexture(GL_TEXTURE_2D, textureBufferName);

    glUseProgram(_shaderParams.program);

//    std::shared_ptr<ITurboSceneMaterial> material = sceneObject->Material();

//    TurboColor ambientColor = material->AmbientColor();
//    TurboColor specularColor = material->SpecularColor();
//    float specularExponent = material->SpecularExponent();

    int32_t viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    auto width  = static_cast<float>(viewport[2]);
    auto height = static_cast<float>(viewport[3]);

    float minX = sceneSprite->Left();
    float maxX = sceneSprite->Right();
    float minY = sceneSprite->Top();
    float maxY = sceneSprite->Bottom();

    TurboMatrix4x4 projection {};
    projection = projection.Translate(1, 1, 0);
    projection = projection.Scale((maxX - minX) / width, (maxY - minY) / height, 1);
    projection = projection.Translate((2 * minX / width) - 1, 1 - (2 * maxY / height), 0);

    glUniformMatrix4fv(_shaderParams.ProjectionMatrix, 1, GL_FALSE, projection.Ptr());

    // Update uniforms
    glUniform1i(_shaderParams.IsSprite, 1);

    // (using glUniform3fv here was troublesome..)
//    glUniform3f(_shaderParams.material_ambient_,  ambientColor.R,  ambientColor.G,  ambientColor.B);
//    glUniform4f(_shaderParams.material_specular_, specularColor.R, specularColor.G, specularColor.B, specularExponent);
//    glUniform3f(_shaderParams.light0_, 100.f, -200.f, -600.f);

    //  TODO: Optimize to use instancing
    //  Loop by mesh first, then all scene objects that use that mesh, then render them as a batch.

    glDrawElements(GL_TRIANGLES, meshInfo.IndexCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void OpenGLESRenderer::FinalizeRendering()
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
TurboMatrix4x4 OpenGLESRenderer::MakePerspectiveProjection(float fovAngle,
                                                           float viewportWidth,
                                                           float viewportHeight,
                                                           float nearPlane,
                                                           float farPlane)
{
    float smallDimension = nearPlane * tan(0.5f * fovAngle * PIby180);
    float width;    //  width of near Z plane, or width of viewport in world coordinates.
    float height;   //  height of near Z plane, or height of viewport in world coordinates.

    if (viewportWidth > viewportHeight)
    {
        height = smallDimension;
        width  = height * viewportWidth / viewportHeight;
    }
    else
    {
        width  = smallDimension;
        height = width * viewportHeight / viewportWidth;
    }

    float n2 = 2 * nearPlane;
    float rcpnmf = 1 / (nearPlane - farPlane);

    TurboMatrix4x4 result;

    result.M11 = n2 / width;

    result.M22 = n2 / height;

    result.M33 = (farPlane + nearPlane) * rcpnmf;
    result.M43 = farPlane * rcpnmf * n2;

    result.M34 = -1;
    result.M44 = 0;

    return result;
}

TurboMatrix4x4 OpenGLESRenderer::MakeViewProjection(const TurboVector3D &vec_eye, const TurboVector3D &vec_at, const TurboVector3D &vec_up)
{
    TurboVector3D vec_forward;
    TurboVector3D vec_up_norm;
    TurboVector3D vec_side;

    vec_forward = vec_eye - vec_at;
    vec_forward.Normalize();

    vec_up_norm = vec_up;
    vec_up_norm.Normalize();

    vec_side = vec_up_norm % vec_forward;   // cross product
    vec_up_norm = vec_forward % vec_side;   // cross product

    TurboMatrix4x4 result;

    result.M11 = vec_side.X;
    result.M21 = vec_side.Y;
    result.M31 = vec_side.Z;

    result.M12 = vec_up_norm.X;
    result.M22 = vec_up_norm.Y;
    result.M32 = vec_up_norm.Z;

    result.M13 = vec_forward.X;
    result.M23 = vec_forward.Y;
    result.M33 = vec_forward.Z;

    result.M41 = -vec_eye.X * result.M11 + -vec_eye.Y * result.M21 + -vec_eye.Z * result.M31;
    result.M42 = -vec_eye.X * result.M12 + -vec_eye.Y * result.M22 + -vec_eye.Z * result.M32;
    result.M43 = -vec_eye.X * result.M13 + -vec_eye.Y * result.M23 + -vec_eye.Z * result.M33;

    return result;
}
