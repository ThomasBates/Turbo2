#pragma once

#include <ITurboSceneObjectMaterial.h>
#include <ITurboScenePixelShader.h>
#include <ITurboSceneVertexShader.h>

class TurboSceneObjectMaterial : public ITurboSceneObjectMaterial
{
private:
	Vector4D _meshColor;
	Vector4D _diffuseColor;
	Vector4D _specularColor;
	float _specularExponent;
	std::shared_ptr<ITurboSceneObjectTexture> _texture;
	std::shared_ptr<ITurboSceneVertexShader> _vertexShader;
	std::shared_ptr<ITurboScenePixelShader> _pixelShader;

public:
	//  Constructors and Destructors  ----------------------------------------------------------------------------------
	TurboSceneObjectMaterial(
		Vector4D meshColor,
		Vector4D diffuseColor,
		Vector4D specularColor,
		float specularExponent,
		std::shared_ptr<ITurboSceneObjectTexture> texture,
		std::shared_ptr<ITurboSceneVertexShader> vertexShader,
		std::shared_ptr<ITurboScenePixelShader> pixelShader);

	//  ITurboSceneObjectMaterial Properties  --------------------------------------------------------------------------
	virtual Vector4D MeshColor() { return _meshColor; }
	virtual void MeshColor(Vector4D meshColor) { _meshColor = meshColor; }

	virtual Vector4D DiffuseColor() { return _diffuseColor; }
	virtual void DiffuseColor(Vector4D diffuseColor) { _diffuseColor = diffuseColor; }

	virtual Vector4D SpecularColor() { return _specularColor; }
	virtual void SpecularColor(Vector4D specularColor) { _specularColor = specularColor; }

	virtual float SpecularExponent() { return _specularExponent; }
	virtual void SpecularExponent(float specularExponent) { _specularExponent = specularExponent; }

	virtual std::shared_ptr<ITurboSceneObjectTexture> Texture() { return _texture; }
	virtual void Texture(std::shared_ptr<ITurboSceneObjectTexture> texture) { _texture = texture; }

	virtual std::shared_ptr<ITurboSceneVertexShader> VertexShader() { return _vertexShader; }
	virtual void VertexShader(std::shared_ptr<ITurboSceneVertexShader> vertexShader) { _vertexShader = vertexShader; }

	virtual std::shared_ptr<ITurboScenePixelShader> PixelShader() { return _pixelShader; }
	virtual void PixelShader(std::shared_ptr<ITurboScenePixelShader> pixelShader) { _pixelShader = pixelShader; }

	//  ITurboSceneObjectMaterial Methods  -----------------------------------------------------------------------------
};