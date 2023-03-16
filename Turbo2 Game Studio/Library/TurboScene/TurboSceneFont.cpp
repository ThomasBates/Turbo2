
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
    _debug->Send(debugVerbose, debugView)
            << "TurboSceneFont::CreateMesh(\"" << sceneText->Text() << "\")\n";

    auto textRectangle = sceneText->Rectangle();
    auto fontSize = sceneText->FontSize();

    float sceneTextWidth  = textRectangle.X2 - textRectangle.X1;
    float sceneTextHeight = textRectangle.Y2 - textRectangle.Y1;

    std::vector<std::vector<TurboRectangle>> placementLists{};
    std::vector<std::vector<TurboRectangle>> textureUVLists{};

    //  just for scope.
    {
        std::vector<TurboRectangle> placementList{};
        std::vector<TurboRectangle> textureUVList{};

        TurboVector2D charPosition{};
        TurboVector2D charSize{0, 1};

        for (auto &c : sceneText->Text())
        {
            if (c == '\n')
            {
                charPosition.X = 0;
                charPosition.Y += charSize.Y;

                placementLists.push_back(placementList);
                textureUVLists.push_back(textureUVList);
                placementList.clear();
                textureUVList.clear();
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

            _debug->Send(debugVerbose, debugView)
                    << "TurboSceneFont::CreateMesh: '" << c << "': " << placement << "\n";

            placementList.push_back(placement);
            textureUVList.push_back(textureUV);

            charPosition.X += charSize.X;
        }

        if (placementLists.empty() && placementList.empty())
            return std::shared_ptr<ITurboSceneMesh>(new TurboSceneMesh());

        placementLists.push_back(placementList);
        textureUVLists.push_back(textureUVList);
    }

//    TurboVector2D factor
//    {
//        2 * (fontSize) / (textRectangle.X2 - textRectangle.X1),
//        2 * (fontSize) / (textRectangle.Y2 - textRectangle.Y1)
//    };

    _debug->Send(debugVerbose, debugView)
            << "TurboSceneFont::CreateMesh: sceneText->HorizontalAlignment = "
            << (int) (sceneText->HorizontalAlignment()) << "\n";

    float factor;
    float offset;
    float increment;

    for (auto &placementList : placementLists)
    {
        if (placementList.size() == 0)
            continue;

        float maxX = placementList.back().X2;

        switch (sceneText->HorizontalAlignment())
        {
            case horizontalLeft:
                factor = 2 * fontSize / sceneTextWidth;
                offset = -1;
                increment = 0;
                break;
            case horizontalCenter:
                factor = 2 * fontSize / sceneTextWidth;
                offset = -maxX * factor / 2;
                increment = 0;
                break;
            case horizontalRight:
                factor = 2 * fontSize / sceneTextWidth;
                offset = 1 - maxX * factor;
                increment = 0;
                break;
            case horizontalStretched:
                factor = 2 / maxX;
                offset = -1;
                increment = 0;
                break;
            case horizontalJustified:
                factor = 2 * fontSize / sceneTextWidth;
                offset = -1;
                increment = (2 - maxX * factor) / (placementList.size() - 1.0f);
                break;
            default:
                break;
        }

        _debug->Send(debugVerbose, debugView)
                << "TurboSceneFont::CreateMesh: factor = " << factor << ", offset = " << offset << ", increment = " << increment << "\n";

        for (auto &placement : placementList)
        {
            placement.X1 = placement.X1 * factor + offset;
            placement.X2 = placement.X2 * factor + offset;
            offset += increment;
            _debug->Send(debugVerbose, debugView)
                    << "TurboSceneFont::CreateMesh: " << placement << "\n";
        }

    }

    _debug->Send(debugVerbose, debugView)
            << "TurboSceneFont::CreateMesh: sceneText->VerticalAlignment = "
            << (int)(sceneText->VerticalAlignment()) << "\n";

    float maxY = placementLists.back().back().Y2;

    switch (sceneText->VerticalAlignment())
    {
        case verticalTop:
            factor = -2 * fontSize / sceneTextHeight;
            offset = 1;
            increment = 0;
            break;
        case verticalCenter:
            factor = -2 * fontSize / sceneTextHeight;
            offset = maxY * factor / 2;
            increment = 0;
            break;
        case verticalBottom:
            factor = -2 * fontSize / sceneTextHeight;
            offset = maxY * factor - 1;
            increment = 0;
            break;
        case verticalStretched:
            factor = -2 / maxY;
            offset = 1;
            increment = 0;
            break;
        case verticalJustified:
            factor = -2 * fontSize / sceneTextHeight;
            offset = 1;
            increment = (2 - maxY * factor) / (placementLists.size() - 1.0f);
            break;
        default:
            break;
    }

    _debug->Send(debugVerbose, debugView)
            << "TurboSceneFont::CreateMesh: factor = " << factor << ", offset = " << offset << ", increment = " << increment << "\n";

    for (auto &placementList : placementLists)
    {
        for (auto &placement : placementList)
        {
            placement.Y1 = placement.Y1 * factor + offset;
            placement.Y2 = placement.Y2 * factor + offset;
            _debug->Send(debugVerbose, debugView)
                    << "TurboSceneFont::CreateMesh: " << placement << "\n";
        }
    }

    auto normal = TurboVector3D( 0.0,  0.0,  1.0);
    auto color = sceneText->Color();

    auto mesh = std::shared_ptr<ITurboSceneMesh>(new TurboSceneMesh());
    int baseIndex = 0;

    for (int j = 0; j < placementLists.size(); j++)
    {
        auto placementList = placementLists[j];
        auto textureUVList = textureUVLists[j];

        for (int i = 0; i < placementList.size(); i++)
        {
            auto placement = placementList[i];
            auto textureUV = textureUVList[i];

            _debug->Send(debugVerbose, debugView)
                    << "TurboSceneFont::CreateMesh: " << placement << "\n";

            mesh->AddVertex(TurboVector3D(placement.X1, placement.Y2, 0), normal, color, TurboVector2D(textureUV.X1, textureUV.Y2));
            mesh->AddVertex(TurboVector3D(placement.X1, placement.Y1, 0), normal, color, TurboVector2D(textureUV.X1, textureUV.Y1));
            mesh->AddVertex(TurboVector3D(placement.X2, placement.Y1, 0), normal, color, TurboVector2D(textureUV.X2, textureUV.Y1));
            mesh->AddVertex(TurboVector3D(placement.X2, placement.Y2, 0), normal, color, TurboVector2D(textureUV.X2, textureUV.Y2));

            mesh->AddTriangle(baseIndex + 0, baseIndex + 1, baseIndex + 2);
            mesh->AddTriangle(baseIndex + 2, baseIndex + 3, baseIndex + 0);

            baseIndex += 4;
        }
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
