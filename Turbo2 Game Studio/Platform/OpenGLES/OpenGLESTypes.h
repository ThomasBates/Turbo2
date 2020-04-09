
#pragma once

#include <errno.h>
#include <random>
#include <vector>

#include <EGL/egl.h>
#include <GLES/gl.h>

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

namespace Turbo
{
    namespace Platform
    {
        namespace OpenGLES
        {
            struct SHADER_VERTEX
            {
                float Position[3];
                float Normal[3];
                float Color[4];
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
                bool Used;
                GLuint VertexBufferName;
                GLuint VertexCount;
                GLuint IndexBufferName;
                GLuint IndexCount;
            };

            struct TextureInfo
            {
                bool Used;
                GLuint TextureBufferName;
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
        }
    }
}
