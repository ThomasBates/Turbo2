#include <pch.h>

#include <TurboSceneMesh.h>
#include "TeapotMesh.h"

//--------------------------------------------------------------------------------
// Teapot model data
//--------------------------------------------------------------------------------
#include "teapot.inl"

using namespace Turbo::Math;

//  Constructors and Destructors ---------------------------------------------------------------------------------------

TeapotMesh::TeapotMesh()
{
    int32_t num_vertices_ = sizeof(teapotPositions) / sizeof(teapotPositions[0]) / 3;
    int32_t p = 0;
    int32_t n = 0;
    int32_t t = 0;
    for (int32_t i = 0; i < num_vertices_; ++i)
    {
        AddVertex(  TurboVector3D(teapotPositions[p], teapotPositions[p+1], teapotPositions[p+2]),
                    TurboVector3D(teapotNormals[n],   teapotNormals[n+1],   teapotNormals[n+2]),
                    TurboVector2D(teapotTexCoords[t], teapotTexCoords[t+1]));
        p += 3;
        n += 3;
        t += 2;
    }

    int32_t num_indices_ = sizeof(teapotIndices) / sizeof(teapotIndices[0]) / 3;
    int32_t index = 0;
    for (int32_t i = 0; i < num_indices_; ++i)
    {
        AddTriangle(teapotIndices[index + 0], teapotIndices[index + 1], teapotIndices[index + 2]);
        index += 3;
    }
}

//  Constructors and Destructors ---------------------------------------------------------------------------------------
