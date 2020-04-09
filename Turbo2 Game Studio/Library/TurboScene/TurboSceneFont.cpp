
#include <pch.h>

#include <TurboSceneFont.h>
#include <TurboSceneMesh.h>

using namespace Turbo::Scene;

//  Constructors -------------------------------------------------------------------------------------------------------

TurboSceneFont::TurboSceneFont(std::string name) :
	_name(name)
{
}

//  ITurboSceneFont Methods --------------------------------------------------------------------------------------------

std::shared_ptr<ITurboSceneMesh> TurboSceneFont::CreateMesh(std::shared_ptr<ITurboSceneText> sceneText)
{
    std::vector<TurboRectangle> placementList {};
    std::vector<TurboRectangle> textureUVList {};

    TurboVector2D charPosition {};
    TurboVector2D charSize {0, 1};

    for (auto& c : sceneText->Text())
    {
        if (c == '\n' || c == '\r')
        {
            charPosition.Y += charSize.Y;
            continue;
        }

        if (_characterExtent.find(c) == _characterExtent.end())
            continue;

        TurboRectangle extent = _characterExtent[c];

        charSize.X = (extent.X2 - extent.X1) / (extent.Y2 - extent.Y1);

        TurboRectangle placement
        {
            charPosition.X,
            charPosition.Y,
            charPosition.X + charSize.X,
            charPosition.Y + charSize.Y
        };

        TurboRectangle textureUV
        {
            0 + extent.X1 / _textureSize.X,
            1 - extent.Y1 / _textureSize.Y,
            0 + extent.X2 / _textureSize.X,
            1 - extent.Y2 / _textureSize.Y
        };

        placementList.push_back(placement);
        textureUVList.push_back(textureUV);

        charPosition.X += charSize.X;
    }

    auto textRectangle = sceneText->Rectangle();
    auto fontSize = sceneText->FontSize();

//    TurboVector2D factor
//    {
//        2 * (fontSize) / (textRectangle.X2 - textRectangle.X1),
//        2 * (fontSize) / (textRectangle.Y2 - textRectangle.Y1)
//    };

    switch (sceneText->HorizontalAlignment())
    {
        case horizontalLeft:
        {
            float factor = 2 * (fontSize) / (textRectangle.X2 - textRectangle.X1);

            for (int i = 0; i < placementList.size(); i++)
            {
                placementList[i].X1 = placementList[i].X1 * factor - 1;
                placementList[i].X2 = placementList[i].X2 * factor - 1;
            }
            break;
        }
        case horizontalCenter:
            break;
        case horizontalRight:
            break;
        case horizontalStretched:
            break;
        case horizontalJustified:
            break;
        default:
            break;
    }

    switch (sceneText->VerticalAlignment())
    {
        case verticalTop:
        {
            float factor = 2 * (fontSize) / (textRectangle.Y2 - textRectangle.Y1);

            for (int i = 0; i < placementList.size(); i++)
            {
                placementList[i].Y1 = 1 - placementList[i].Y1 * factor;
                placementList[i].Y2 = 1 - placementList[i].Y2 * factor;
            }
            break;
        }
        case verticalCenter:
            break;
        case verticalBottom:
            break;
        case verticalStretched:
            break;
        case verticalJustified:
            break;
        default:
            break;
    }

    auto normal = TurboVector3D( 0.0,  0.0,  1.0);
    auto color = sceneText->Color();

    auto mesh = std::shared_ptr<ITurboSceneMesh>(new TurboSceneMesh());
    int baseIndex = 0;
    for (int i = 0; i < placementList.size(); i++)
    {
        auto placement = placementList[i];
        auto textureUV = textureUVList[i];

        mesh->AddVertex(TurboVector3D(placement.X1, placement.Y2,  0), normal, color, TurboVector2D(textureUV.X1, textureUV.Y2));
        mesh->AddVertex(TurboVector3D(placement.X1, placement.Y1,  0), normal, color, TurboVector2D(textureUV.X1, textureUV.Y1));
        mesh->AddVertex(TurboVector3D(placement.X2, placement.Y1,  0), normal, color, TurboVector2D(textureUV.X2, textureUV.Y1));
        mesh->AddVertex(TurboVector3D(placement.X2, placement.Y2,  0), normal, color, TurboVector2D(textureUV.X2, textureUV.Y2));

        mesh->AddTriangle(baseIndex + 0, baseIndex + 1, baseIndex + 2);
        mesh->AddTriangle(baseIndex + 2, baseIndex + 3, baseIndex + 0);

        baseIndex += 4;
    }

    return mesh;
}

//  Protected Methods --------------------------------------------------------------------------------------------------

void TurboSceneFont::SetTextureExtent(float width, float height)
{
    _textureSize = TurboVector2D(width, height);
}

void TurboSceneFont::SetCharacterExtent(char character, short left, short top, short right, short bottom)
{
    _characterExtent[character] = TurboRectangle(left, top, right, bottom);
}
