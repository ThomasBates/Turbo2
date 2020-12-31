
#include <pch.h>

#include <chrono>
#include <thread>
#include <cstring>

#include <OpenGLESRenderer.h>

#include <ITurboGroupView.h>
#include <ITurboCanvas.h>
#include <ITurboImage.h>

#include <TurboBitmap.h>
#include <TurboCanvasRGBA32.h>
#include <TurboCoreHelpers.h>
#include <TurboSceneMesh.h>
#include <TurboSceneTexture.h>
#include <TurboSceneArialFont.h>


using namespace Turbo::Core;
using namespace Turbo::Graphics;
using namespace Turbo::Platform::OpenGLES;

//  Constructors & Destructors  ----------------------------------------------------------------------------------------

OpenGLESRenderer::OpenGLESRenderer(
        android_app* app,
        std::shared_ptr<ITurboDebug> debug,
        std::shared_ptr<ITurboGameIOService> ioService) :
        _android_app(app),
        _debug(debug),
        _ioService(ioService)
{
//    _rendererAccess = std::shared_ptr<ITurboViewRendererAccess>(this);

    _gl_context = OpenGLESContext::GetInstance();
}

OpenGLESRenderer::~OpenGLESRenderer()
{
    ReleaseViewResources();
}

//	ITurboGameRenderer Methods -----------------------------------------------------------------------------------------

void OpenGLESRenderer::RegisterFont(std::shared_ptr<ITurboSceneFont> font)
{
    _sceneFonts[font->Name()] = font;
}

void OpenGLESRenderer::InitializeLoading()
{
    if (!_display_updated)
    {
        UpdateDisplayInformation();
        _display_updated = true;
    }

    if (_sceneFonts.empty())
    {
        RegisterFont(std::shared_ptr<ITurboSceneFont>(new TurboSceneArialFont(_debug)));
//        RegisterFont(std::shared_ptr<ITurboSceneFont>(new TurboSceneConsolasFont()));
    }
}

void OpenGLESRenderer::FinalizeLoading()
{
    CreateShaders();
}

void OpenGLESRenderer::Reset()
{
    ReleaseViewResources();

    _gl_context->Invalidate();

    _resources_initialized = false;
    _display_updated = false;
}

//	ITurboViewRendererAccess Methods ----------------------------------------------------------------------------------

void OpenGLESRenderer::LoadScene(std::shared_ptr<ITurboScene> scene)
{
    if (scene == nullptr)
        return;

    for (auto& sceneObject : scene->SceneObjects())
    {
        LoadSceneObject(sceneObject);
        LoadChildSceneObjects(sceneObject);
    }

    for (auto& sceneSprite : scene->SceneSprites())
        LoadSceneSprite(sceneSprite);

    for (auto& sceneText : scene->SceneTexts())
        LoadSceneText(sceneText);
}

void OpenGLESRenderer::LoadSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject)
{
    if (sceneObject == nullptr)
        return;

    LoadSceneObjectTexture(sceneObject);
    LoadSceneObjectVertices(sceneObject);
}

void OpenGLESRenderer::LoadChildSceneObjects(std::shared_ptr<ITurboSceneObject> sceneObject)
{
    if (sceneObject == nullptr)
        return;

    //	Iterate over child scene objects. Call this method recursively.
    for (auto &childSceneObject : sceneObject->ChildSceneObjects())
    {
        LoadSceneObject(childSceneObject);
        LoadChildSceneObjects(childSceneObject);
    }
}

void OpenGLESRenderer::LoadSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite)
{
    if (sceneSprite == nullptr)
        return;

    LoadSceneSpriteTexture(sceneSprite);
    LoadSceneSpriteVertices();
}

void OpenGLESRenderer::LoadSceneText(std::shared_ptr<ITurboSceneText> sceneText)
{
    if (sceneText == nullptr)
        return;

    LoadSceneFontTexture(sceneText);
    LoadSceneTextVertices(sceneText);

}

void OpenGLESRenderer::RenderScene(std::shared_ptr<ITurboScene> scene)
{
    if (scene == nullptr)
        return;

    UpdateProjectionMatrix();
    UpdateViewMatrix(scene->CameraPlacement(), scene->LightHack());

    for (auto& sceneObject : scene->SceneObjects())
    {
        RenderSceneObject(sceneObject);
        RenderChildSceneObjects(sceneObject);
    }

    for (auto& sceneSprite : scene->SceneSprites())
        RenderSceneSprite(sceneSprite);

    for (auto& sceneText : scene->SceneTexts())
        RenderSceneText(sceneText);
}

void OpenGLESRenderer::RenderSceneObject(std::shared_ptr<ITurboSceneObject> sceneObject)
{
    if (sceneObject == nullptr)
        return;

    auto mesh = sceneObject->Mesh();
    if (mesh == nullptr)
        return;

    auto textureName = sceneObject->Material()->Texture()->Name();
    auto model = sceneObject->Placement()->Transform();

    RenderSceneMesh(&(_sceneMeshInfo[mesh]), textureName, 0, &_projectionMatrix, &_viewMatrix, &model);
}

void OpenGLESRenderer::RenderChildSceneObjects(std::shared_ptr<ITurboSceneObject> sceneObject)
{
    if (sceneObject == nullptr)
        return;

    //	Iterate over child scene objects. Call this method recursively.
    for (auto &childSceneObject : sceneObject->ChildSceneObjects())
    {
        RenderSceneObject(childSceneObject);
        RenderChildSceneObjects(childSceneObject);
    }
}

void OpenGLESRenderer::RenderSceneSprite(std::shared_ptr<ITurboSceneSprite> sceneSprite)
{
    if (sceneSprite == nullptr)
        return;

    auto textureName = sceneSprite->Texture()->Name();

    TurboMatrix4x4 projection {};

    if (sceneSprite->UseRectangle())
    {
        int32_t viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        auto width = static_cast<float>(viewport[2]);
        auto height = static_cast<float>(viewport[3]);

        auto r = sceneSprite->Rectangle();

        projection = projection.Translate(1, 1, 0);
        projection = projection.Scale((r.X2 - r.X1) / width, (r.Y2 - r.Y1) / height, 1);
        projection = projection.Translate((2 * r.X1 / width) - 1, 1 - (2 * r.Y2 / height), 0);
    }

    RenderSceneMesh(&_spriteMeshInfo, textureName, 1, &projection, NULL, NULL);
}

void OpenGLESRenderer::RenderSceneText(std::shared_ptr<ITurboSceneText> sceneText)
{
    if (sceneText == nullptr)
        return;

    auto fontName = sceneText->FontName();
    if (fontName.empty())
        return;

    //  Font name not registered?
    if (_sceneFonts.find(fontName) == _sceneFonts.end())
        return;

    //  Font texture not loaded?
    if (_sceneTextureInfo.find(fontName) == _sceneTextureInfo.end())
        return;

    TurboMatrix4x4 projection {};

    if (sceneText->UseRectangle())
    {
        int32_t viewport[4];
        glGetIntegerv(GL_VIEWPORT, viewport);
        auto width = static_cast<float>(viewport[2]);
        auto height = static_cast<float>(viewport[3]);

        auto r = sceneText->Rectangle();

        projection = projection.Translate(1, 1, 0);
        projection = projection.Scale((r.X2 - r.X1) / width, (r.Y2 - r.Y1) / height, 1);
        projection = projection.Translate((2 * r.X1 / width) - 1, 1 - (2 * r.Y2 / height), 0);
    }

    RenderSceneMesh(&(_sceneTextInfo[sceneText]), fontName, 1, &projection, NULL, NULL);

    sceneText->PropertyChanged(false);
}

void OpenGLESRenderer::RenderSceneMesh(
        MeshInfo *meshInfo,
        std::string textureName,
        int isSprite,
        TurboMatrix4x4 *projection,
        TurboMatrix4x4 *view,
        TurboMatrix4x4 *model)
{
    meshInfo->Used = true;

    // Bind the Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, meshInfo->VertexBufferName);

    int32_t stride = sizeof(SHADER_VERTEX);
    // Pass the vertex data
    glVertexAttribPointer(ATTRIB_VERTEX, 3, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(ATTRIB_VERTEX);

    glVertexAttribPointer(ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(ATTRIB_NORMAL);

    glVertexAttribPointer(ATTRIB_COLOR,  4, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(ATTRIB_COLOR);

    glVertexAttribPointer(ATTRIB_UV,     2, GL_FLOAT, GL_FALSE, stride, BUFFER_OFFSET(10 * sizeof(GLfloat)));
    glEnableVertexAttribArray(ATTRIB_UV);

    // Bind the Index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshInfo->IndexBufferName);

    _sceneTextureInfo[textureName].Used = true;
    GLuint textureBufferName = _sceneTextureInfo[textureName].TextureBufferName;
    glBindTexture(GL_TEXTURE_2D, textureBufferName);

    glUseProgram(_shaderParams.program);

//    std::shared_ptr<ITurboSceneMaterial> material = sceneObject->Material();

//    TurboColor ambientColor = material->AmbientColor();
//    TurboColor specularColor = material->SpecularColor();
//    float specularExponent = material->SpecularExponent();

    // Update uniforms
    if (projection)
        glUniformMatrix4fv(_shaderParams.ProjectionMatrix, 1, GL_FALSE, projection->Ptr());
    if (view)
        glUniformMatrix4fv(_shaderParams.ViewMatrix,       1, GL_FALSE, view->Ptr());
    if (model)
        glUniformMatrix4fv(_shaderParams.ModelMatrix,      1, GL_FALSE, model->Ptr());

    glUniform1i(_shaderParams.LightCount, _lightCount);
    glUniform1i(_shaderParams.IsSprite, isSprite);

    // (using glUniform3fv here was troublesome..)
//    glUniform3f(_shaderParams.material_ambient_,  ambientColor.R,  ambientColor.G,  ambientColor.B);
//    glUniform4f(_shaderParams.material_specular_, specularColor.R, specularColor.G, specularColor.B, specularExponent);
//    glUniform3f(_shaderParams.light0_, 100.f, -200.f, -600.f);

    //  TODO: Optimize to use instancing
    //  Loop by mesh first, then all scene objects that use that mesh, then render them as a batch.

    glDrawElements(GL_TRIANGLES, meshInfo->IndexCount, GL_UNSIGNED_SHORT, BUFFER_OFFSET(0));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//	--------------------------------------------------------------------------------------------------------------------

void OpenGLESRenderer::UpdateDisplayInformation()
{
    if (!_resources_initialized)
    {
        //  Give Android a chance to update its ANativeWindow to show the correct dimensions.
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        _gl_context->Init(_android_app->window);
        _resources_initialized = true;
    }
    else if(_android_app->window != _gl_context->GetANativeWindow())
    {
        // Re-initialize ANativeWindow.
        // On some devices, ANativeWindow is re-created when the app is resumed
        assert(_gl_context->GetANativeWindow());
        ReleaseViewResources();
        _gl_context->Invalidate();
        _gl_context->Init(_android_app->window);
        _resources_initialized = true;
    }
    else
    {
        // initialize OpenGL ES and EGL
        if (EGL_SUCCESS == _gl_context->Resume(_android_app->window))
            ReleaseViewResources();
        else
            assert(0);
    }

    // Initialize GL state.
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // Note that screen size might have been changed
    int width = _gl_context->GetScreenWidth();
    int height = _gl_context->GetScreenHeight();
    glViewport(0, 0, width, height);

    InitializeViewResources();
}

void OpenGLESRenderer::InitializeViewResources()
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

void OpenGLESRenderer::ReleaseViewResources()
{
    DeleteBuffers();
}

//	Scene Vertex Resources ---------------------------------------------------------------------------------------------

void OpenGLESRenderer::LoadSceneObjectVertices(std::shared_ptr<ITurboSceneObject> sceneObject)
{
    auto mesh = sceneObject->Mesh();
    if (mesh == nullptr)
        return;

    //  Already loaded this mesh? don't reload it.
    if (_sceneMeshInfo.find(mesh) != _sceneMeshInfo.end())
        return;

    _debug->Send(debugDebug, debugRenderer) << "Loading vertices: " << "\n";

    MeshInfo meshInfo {};

    LoadMeshInfo(mesh, &meshInfo);

    _sceneMeshInfo[mesh] = meshInfo;
}

void OpenGLESRenderer::LoadSceneSpriteVertices()
{
    if (_spriteMeshInfo.Used)
        return;

    auto mesh = std::shared_ptr<ITurboSceneMesh>(new TurboSceneMesh());

    TurboVector3D normal = TurboVector3D( 0.0,  0.0,  1.0);
    mesh->AddVertex(TurboVector3D(-1, -1,  0), normal, TurboVector2D(0, 0));
    mesh->AddVertex(TurboVector3D(-1,  1,  0), normal, TurboVector2D(0, 1));
    mesh->AddVertex(TurboVector3D( 1,  1,  0), normal, TurboVector2D(1, 1));
    mesh->AddVertex(TurboVector3D( 1, -1,  0), normal, TurboVector2D(1, 0));

    mesh->AddTriangle(0, 1, 2);
    mesh->AddTriangle(2, 3, 0);

    MeshInfo meshInfo {};

    LoadMeshInfo(mesh, &meshInfo);

    _spriteMeshInfo = meshInfo;
}

void OpenGLESRenderer::LoadSceneTextVertices(std::shared_ptr<ITurboSceneText> sceneText)
{
    auto fontName = sceneText->FontName();
    if (fontName.empty())
        return;

    //  Font name not registered?
    if (_sceneFonts.find(fontName) == _sceneFonts.end())
        return;

    //  Already loaded this text? unload it first
    if (_sceneTextInfo.find(sceneText) != _sceneTextInfo.end())
    {
        if (!sceneText->PropertyChanged())
            return;

        auto meshInfo = _sceneTextInfo[sceneText];

        if (meshInfo.VertexBufferName)
            glDeleteBuffers(1, &meshInfo.VertexBufferName);

        if (meshInfo.IndexBufferName)
            glDeleteBuffers(1, &meshInfo.IndexBufferName);
    }

    auto font = _sceneFonts[fontName];
    auto text = sceneText->Text();

    auto mesh = font->CreateMesh(sceneText);

    MeshInfo meshInfo {};

    LoadMeshInfo(mesh, &meshInfo);

    _sceneTextInfo[sceneText] = meshInfo;
}

void OpenGLESRenderer::LoadMeshInfo(std::shared_ptr<ITurboSceneMesh> mesh, MeshInfo *meshInfo)
{

    std::vector<SHADER_VERTEX> vertexList;
    std::vector<uint16_t> indexList;

    LoadVertexData(mesh, &vertexList, &indexList);


    //  Load mesh vertices ---------------------------------------------------------------------------------------------
    meshInfo->VertexCount = (GLuint)vertexList.size();
    GLsizeiptr vertexBufferSize = (GLsizeiptr)(vertexList.size() * sizeof(vertexList[0]));
    GLuint vertexBufferName;

    glGenBuffers(1, &vertexBufferName);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferName);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertexList.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    meshInfo->VertexBufferName = vertexBufferName;

    //	Load mesh indices ----------------------------------------------------------------------------------------------
    meshInfo->IndexCount = (GLuint)indexList.size();
    GLsizeiptr indexBufferSize = (GLsizeiptr)(indexList.size() * sizeof(indexList[0]));
    GLuint indexBufferName;

    glGenBuffers(1, &indexBufferName);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferName);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, indexList.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    meshInfo->IndexBufferName = indexBufferName;
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

        shaderVertex.Color[0]    = meshVertex.Color.R;
        shaderVertex.Color[1]    = meshVertex.Color.G;
        shaderVertex.Color[2]    = meshVertex.Color.B;
        shaderVertex.Color[3]    = meshVertex.Color.A;

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

void OpenGLESRenderer::LoadSceneObjectTexture(std::shared_ptr<ITurboSceneObject> sceneObject)
{
    std::shared_ptr<ITurboSceneMaterial> material = sceneObject->Material();
    if (material == nullptr)
        return;

    LoadSceneTexture(material->Texture());
}

void OpenGLESRenderer::LoadSceneSpriteTexture(std::shared_ptr<ITurboSceneSprite> sceneSprite)
{
    LoadSceneTexture(sceneSprite->Texture());
}

void OpenGLESRenderer::LoadSceneFontTexture(std::shared_ptr<ITurboSceneText> sceneText)
{
    auto fontName = sceneText->FontName();
    if (fontName.empty())
        return;

    //  Font name not registered?
    if (_sceneFonts.find(fontName) == _sceneFonts.end())
        return;

    //  Already loaded this font texture? don't reload it.
    if (_sceneTextureInfo.find(fontName) != _sceneTextureInfo.end())
        return;

    auto fontTexture = std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture(fontName));

    LoadSceneTexture(fontTexture);
}

void OpenGLESRenderer::LoadSceneTexture(std::shared_ptr<ITurboSceneTexture> texture)
{
    if (texture == nullptr)
        return;

    std::string textureName = texture->Name();
    if (textureName.empty())
        return;

    //  Already loaded this texture? don't reload it.
    if (_sceneTextureInfo.find(textureName) != _sceneTextureInfo.end())
        return;

    _debug->Send(debugDebug, debugRenderer) << "Loading texture: "  << textureName << "\n";

    GLsizei textureWidth;
    GLsizei textureHeight;
    std::vector<unsigned char> textureData;

    LoadTextureData(textureName, &textureWidth, &textureHeight, &textureData);

    _debug->Send(debugDebug, debugRenderer) << "Loading texture: "  << textureName << " (" << textureData.size() << ")\n";

    TextureInfo textureInfo {};

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

    textureInfo.TextureBufferName = textureBufferName;

    _sceneTextureInfo[textureName] = textureInfo;

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
    bitmap->RenderToCanvas();

    auto *canvasData = (unsigned char *)canvas->Data();
    auto canvasDataSize = canvas->DataSize();
    textureData->assign(canvasData, canvasData + canvasDataSize);

    *textureWidth = canvas->Width();
    *textureHeight = canvas->Height();
}

//  Shaders  -----------------------------------------------------------------------------------------------------------

void OpenGLESRenderer::CreateShaders()
{
    if (_shaderProgram)
        return;

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
    GLuint vertexShader;
    GLuint pixelShader;

    // Create shader _shaderProgram
    _shaderProgram = glCreateProgram();
    LOGI("Created Shader %d", _shaderProgram);

    // Create and compile vertex shader
    if (!CompileShader(&vertexShader, GL_VERTEX_SHADER, vertexShaderName))
    {
        LOGI("Failed to compile vertex shader");
        glDeleteProgram(_shaderProgram);
        return false;
    }

    // Create and compile fragment shader
    if (!CompileShader(&pixelShader, GL_FRAGMENT_SHADER, pixelShaderName))
    {
        LOGI("Failed to compile fragment shader");
        glDeleteProgram(_shaderProgram);
        return false;
    }

    // Attach vertex shader to program
    glAttachShader(_shaderProgram, vertexShader);

    // Attach fragment shader to program
    glAttachShader(_shaderProgram, pixelShader);

    // Bind attribute locations
    // this needs to be done prior to linking
    glBindAttribLocation(_shaderProgram, ATTRIB_VERTEX, "vsPosition");
    glBindAttribLocation(_shaderProgram, ATTRIB_NORMAL, "vsNormal");
    glBindAttribLocation(_shaderProgram, ATTRIB_COLOR,  "vsColor");
    glBindAttribLocation(_shaderProgram, ATTRIB_UV,     "vsTexture");

    // Link program
    if (!LinkProgram(_shaderProgram))
    {
        LOGI("Failed to link program: %d", _shaderProgram);

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
        if (_shaderProgram)
        {
            glDeleteProgram(_shaderProgram);
            _shaderProgram = 0;
        }
        return false;
    }

    // Get uniform locations
    params->TextureSampler    = glGetUniformLocation(_shaderProgram, "uSampler");
    params->ModelMatrix       = glGetUniformLocation(_shaderProgram, "uModel");
    params->ViewMatrix        = glGetUniformLocation(_shaderProgram, "uView");
    params->ProjectionMatrix  = glGetUniformLocation(_shaderProgram, "uProjection");
    params->LightCount        = glGetUniformLocation(_shaderProgram, "uLightCount");
    params->IsSprite          = glGetUniformLocation(_shaderProgram, "uIsSprite");

    // Release vertex and fragment shaders
    if (vertexShader) glDeleteShader(vertexShader);
    if (pixelShader) glDeleteShader(pixelShader);

    params->program = _shaderProgram;
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
        auto log = (GLchar *)malloc((size_t)logLength);
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
    if (_shaderProgram)
    {
        glDeleteProgram(_shaderProgram);
        _shaderProgram = 0;
    }

    if (_spriteMeshInfo.VertexBufferName)
        glDeleteBuffers(1, &_spriteMeshInfo.VertexBufferName);
    if (_spriteMeshInfo.IndexBufferName)
        glDeleteBuffers(1, &_spriteMeshInfo.IndexBufferName);
    _spriteMeshInfo.Used = false;

    for (auto& entry : _sceneMeshInfo)
    {
        MeshInfo meshInfo = entry.second;

        if (meshInfo.VertexBufferName)
            glDeleteBuffers(1, &meshInfo.VertexBufferName);

        if (meshInfo.IndexBufferName)
            glDeleteBuffers(1, &meshInfo.IndexBufferName);
    }
    _sceneMeshInfo.clear();

    if (_sceneUniformBufferName)
    {
        glDeleteBuffers(1, &_sceneUniformBufferName);
        _sceneUniformBufferName = 0;
    }

    for (auto& entry : _sceneTextInfo)
    {
        MeshInfo meshInfo = entry.second;

        if (meshInfo.VertexBufferName)
            glDeleteBuffers(1, &meshInfo.VertexBufferName);

        if (meshInfo.IndexBufferName)
            glDeleteBuffers(1, &meshInfo.IndexBufferName);
    }
    _sceneTextInfo.clear();

    if (_shaderParams.program)
    {
        glDeleteProgram(_shaderParams.program);
        _shaderParams.program = 0;
    }

    for (auto& entry : _sceneTextureInfo)
    {
        TextureInfo textureInfo = entry.second;

        _debug->Send(debugDebug, debugRenderer) << "Unloading texture " << entry.first << " (DeleteBuffers)\n";

        if (textureInfo.TextureBufferName)
            glDeleteTextures(1, &textureInfo.TextureBufferName);
    }
    _sceneTextureInfo.clear();
}

//  RenderScene  -------------------------------------------------------------------------------------------------------

void OpenGLESRenderer::UpdateProjectionMatrix()
{
    int32_t viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    auto width  = static_cast<float>(viewport[2]);
    auto height = static_cast<float>(viewport[3]);

    const float fovAngle  =  90.0;
    const float nearPlane =   0.01;
    const float farPlane  = 100.0;

    _projectionMatrix = MakePerspectiveProjection(fovAngle, width, height, nearPlane, farPlane);
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
    _spriteMeshInfo.Used = true;

    for (auto& entry : _sceneMeshInfo)
        entry.second.Used = false;

    for (auto& entry : _sceneTextureInfo)
        entry.second.Used = false;

    // Draw objects back to front
    //glDisable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glDepthMask(GL_FALSE);


    // Just fill the screen with a color.
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLESRenderer::FinalizeRendering()
{
    // Swap
    if (EGL_SUCCESS != _gl_context->Swap())
        ReleaseViewResources();

    //RemoveUnusedResources();
}

void OpenGLESRenderer::RemoveUnusedResources()
{
    if (!_spriteMeshInfo.Used)
    {
        if (_spriteMeshInfo.VertexBufferName)
            glDeleteBuffers(1, &_spriteMeshInfo.VertexBufferName);
        if (_spriteMeshInfo.IndexBufferName)
            glDeleteBuffers(1, &_spriteMeshInfo.IndexBufferName);
    }

    //  Clean up unused meshes.
    std::vector<std::shared_ptr<ITurboSceneMesh>> unusedMeshes;
    for (auto& entry : _sceneMeshInfo)
    {
        auto mesh = entry.first;
        auto meshInfo = entry.second;

        if (!meshInfo.Used)
        {
            if (meshInfo.VertexBufferName)
                glDeleteBuffers(1, &meshInfo.VertexBufferName);

            if (meshInfo.IndexBufferName)
                glDeleteBuffers(1, &meshInfo.IndexBufferName);

            unusedMeshes.push_back(mesh);
        }
    }
    for (auto& mesh : unusedMeshes)
        _sceneMeshInfo.erase(mesh);
    unusedMeshes.clear();

    //  Clean up unused textures.
    std::vector<std::string> unusedTextures;
    for (auto& entry : _sceneTextureInfo)
    {
        auto textureName = entry.first;
        auto textureInfo = entry.second;

        if (!textureInfo.Used)
        {
            _debug->Send(debugDebug, debugRenderer) << "Unloading texture " << textureName << " (FinalizeRendering)\n";

            if (textureInfo.TextureBufferName)
                glDeleteTextures(1, &textureInfo.TextureBufferName);

            unusedTextures.push_back(textureName);
        }
    }
    for (auto& textureName : unusedTextures)
        _sceneTextureInfo.erase(textureName);
    unusedTextures.clear();
}

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
