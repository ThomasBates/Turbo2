
#include <pch.h>

#include <TurboSceneFont.h>
#include <TurboSceneMesh.h>

using namespace Turbo::Scene;

//  Constructors -------------------------------------------------------------------------------------------------------

TurboSceneFont::TurboSceneFont(
    std::shared_ptr<ITurboDebug> debug,
    std::string name) :
    _debug(debug),
    _name(name)
{
}

//  ITurboSceneFont Methods --------------------------------------------------------------------------------------------

std::shared_ptr<ITurboSceneMesh> TurboSceneFont::CreateMesh(std::shared_ptr<ITurboSceneText> sceneText)
{
    _debug->Send(debugVerbose, debugView) << "TurboSceneFont::CreateMesh(\"" << sceneText->Text() << "\")\n";

    std::vector<TurboRectangle> placementList {};
    std::vector<TurboRectangle> textureUVList {};

    TurboVector2D charPosition {};
    TurboVector2D charSize {0, 1};

    for (auto& c : sceneText->Text())
    {
        if (c == '\n')
        {
            charPosition.Y += charSize.Y;
            continue;
        }

        if (c == '\r')
        {
            charPosition.X = 0;
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

        _debug->Send(debugVerbose, debugView) << "TurboSceneFont::CreateMesh: '" << c << "': " << placement << "\n";

        placementList.push_back(placement);
        textureUVList.push_back(textureUV);

        charPosition.X += charSize.X;
    }

    if (placementList.empty())
        return std::shared_ptr<ITurboSceneMesh>(new TurboSceneMesh());

    auto textRectangle = sceneText->Rectangle();
    auto fontSize = sceneText->FontSize();

//    TurboVector2D factor
//    {
//        2 * (fontSize) / (textRectangle.X2 - textRectangle.X1),
//        2 * (fontSize) / (textRectangle.Y2 - textRectangle.Y1)
//    };

    _debug->Send(debugVerbose, debugView) << "TurboSceneFont::CreateMesh: sceneText->HorizontalAlignment = " << (int)(sceneText->HorizontalAlignment()) << "\n";

    float factor;
    float offset;
    float increment;

    switch (sceneText->HorizontalAlignment())
    {
        case horizontalLeft:
            factor = 2 * (fontSize) / (textRectangle.X2 - textRectangle.X1);
            offset = -1;
            increment = 0;
            break;
        case horizontalCenter:
            factor = 2 * (fontSize) / (textRectangle.X2 - textRectangle.X1);
            offset = -placementList.back().X2 * factor / 2;
            increment = 0;
            break;
        case horizontalRight:
            factor = 2 * (fontSize) / (textRectangle.X2 - textRectangle.X1);
            offset = 1 - placementList.back().X2 * factor;
            increment = 0;
            break;
        case horizontalStretched:
            factor = 2 / placementList.back().X2;
            offset = -1;
            increment = 0;
            break;
        case horizontalJustified:
            factor = 2 * (fontSize) / (textRectangle.X2 - textRectangle.X1);
            offset = -1;
            increment = (2 - placementList.back().X2 * factor) / (placementList.size() - 1.0f);
            break;
        default:
            break;
    }

    _debug->Send(debugVerbose, debugView) << "TurboSceneFont::CreateMesh: factor = " << factor << ", offset = " << offset << "\n";

    for (auto & placement : placementList)
    {
        placement.X1 = placement.X1 * factor + offset;
        placement.X2 = placement.X2 * factor + offset;
        offset += increment;
        _debug->Send(debugVerbose, debugView) << "TurboSceneFont::CreateMesh: " << placement << "\n";
    }

    _debug->Send(debugVerbose, debugView) << "TurboSceneFont::CreateMesh: sceneText->VerticalAlignment = " << (int)(sceneText->VerticalAlignment()) << "\n";

    switch (sceneText->VerticalAlignment())
    {
        case verticalTop:
        {
            factor = 2 * (fontSize) / (textRectangle.Y2 - textRectangle.Y1);
            _debug->Send(debugVerbose, debugView) << "TurboSceneFont::CreateMesh: case verticalTop: factor = " << factor << "\n";

            for (int i = 0; i < placementList.size(); i++)
            {
                placementList[i].Y1 = 1 - placementList[i].Y1 * factor;
                placementList[i].Y2 = 1 - placementList[i].Y2 * factor;
                _debug->Send(debugVerbose, debugView) << "TurboSceneFont::CreateMesh: case verticalTop: " << placementList[i] << "\n";
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

        _debug->Send(debugVerbose, debugView) << "TurboSceneFont::CreateMesh: " << placement << "\n";

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
