
#include "pch.h"

#include <TurboSceneObjectMaterial.h>

TurboSceneObjectMaterial::TurboSceneObjectMaterial(Vector4D meshColor,
	Vector4D diffuseColor,
	Vector4D specularColor,
	float specularExponent,
	std::shared_ptr<ITurboSceneObjectTexture> texture,
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

