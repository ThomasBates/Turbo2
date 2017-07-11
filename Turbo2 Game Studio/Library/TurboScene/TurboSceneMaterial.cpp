
#include <pch.h>

#include <TurboSceneMaterial.h>
#include <TurboSceneTexture.h>
#include <TurboScenePixelShader.h>
#include <TurboSceneVertexShader.h>

Turbo::Scene::TurboSceneMaterial::TurboSceneMaterial(
	TurboColor meshColor,
	TurboColor diffuseColor,
	TurboColor specularColor,
	float specularExponent,
	std::shared_ptr<ITurboSceneTexture> texture,
	std::shared_ptr<ITurboSceneVertexShader> vertexShader,
	std::shared_ptr<ITurboScenePixelShader> pixelShader)
{
	_meshColor = meshColor;
	_diffuseColor = diffuseColor;
	_specularColor = specularColor;
	_specularExponent = specularExponent;

	_texture = texture;
	_vertexShader = vertexShader;
	_pixelShader = pixelShader;
}

Turbo::Scene::TurboSceneMaterial::TurboSceneMaterial(
	TurboColor meshColor,
	TurboColor diffuseColor,
	TurboColor specularColor,
	float specularExponent,
	std::string textureName,
	std::string vertexShaderName,
	std::string pixelShaderName)
{
	_meshColor = meshColor;
	_diffuseColor = diffuseColor;
	_specularColor = specularColor;
	_specularExponent = specularExponent;

	_texture = std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture(textureName));
	_vertexShader = std::shared_ptr<ITurboSceneVertexShader>(new TurboSceneVertexShader(vertexShaderName));
	_pixelShader = std::shared_ptr<ITurboScenePixelShader>(new TurboScenePixelShader(pixelShaderName));
}

Turbo::Scene::TurboSceneMaterial::TurboSceneMaterial(std::string textureName)
{
	//	Standard default values.
	_meshColor = TurboColor(0.5f, 0.5f, 0.5f, 1.0f);
	_diffuseColor = TurboColor(0.8f, 0.8f, 0.8f, 1.0f);
	_specularColor = TurboColor(0.3f, 0.3f, 0.3f, 1.0f);
	_specularExponent = 1.0f;

	_texture = std::shared_ptr<ITurboSceneTexture>(new TurboSceneTexture(textureName));
	_vertexShader = nullptr;
	_pixelShader = nullptr;
}

