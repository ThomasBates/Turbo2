
#include <pch.h>

#include <memory>

#include <TurboSceneMaterial.h>
#include <TurboSceneTexture.h>
#include <TurboScenePixelShader.h>
#include <TurboSceneVertexShader.h>

Turbo::Scene::TurboSceneMaterial::TurboSceneMaterial(
	TurboColor ambientColor,
	TurboColor diffuseColor,
	TurboColor specularColor,
	float specularExponent)
{
	_ambientColor = ambientColor;
	_diffuseColor = diffuseColor;
	_specularColor = specularColor;
	_specularExponent = specularExponent;

	_texture = nullptr;
	_vertexShader = nullptr;
	_pixelShader = nullptr;
}

Turbo::Scene::TurboSceneMaterial::TurboSceneMaterial(std::shared_ptr<ITurboSceneTexture> texture)
{
	_ambientColor = TurboColor(0.5f, 0.5f, 0.5f, 1.0f);
	_diffuseColor = TurboColor(0.8f, 0.8f, 0.8f, 1.0f);
	_specularColor = TurboColor(0.3f, 0.3f, 0.3f, 1.0f);
	_specularExponent = 1.0f;

	_texture = texture;
	_vertexShader = nullptr;
	_pixelShader = nullptr;
}

Turbo::Scene::TurboSceneMaterial::TurboSceneMaterial(std::string textureName)
{
	//	Standard default values.
	_ambientColor = TurboColor(0.5f, 0.5f, 0.5f, 1.0f);
	_diffuseColor = TurboColor(0.8f, 0.8f, 0.8f, 1.0f);
	_specularColor = TurboColor(0.3f, 0.3f, 0.3f, 1.0f);
	_specularExponent = 1.0f;

	_texture = std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture(textureName));
	_vertexShader = nullptr;
	_pixelShader = nullptr;
}

