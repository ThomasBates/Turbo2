#pragma once

#include<pch.h>

#include <ITurboSceneObjectTexture.h>
#include <ITurboScenePixelShader.h>
#include <ITurboSceneVertexShader.h>

#include <Vector4D.h>

class ITurboSceneObjectMaterial
{
	virtual Vector4D MeshColor() = 0;
	virtual void MeshColor(Vector4D meshColor) = 0;

	virtual Vector4D DiffuseColor() = 0;
	virtual void DiffuseColor(Vector4D diffuseColor) = 0;

	virtual Vector4D SpecularColor() = 0;
	virtual void SpecularColor(Vector4D specularColor) = 0;

	virtual float SpecularExponent() = 0;
	virtual void SpecularExponent(float specularExponent) = 0;

	virtual std::shared_ptr<ITurboSceneObjectTexture> Texture() = 0;
	virtual void Texture(std::shared_ptr<ITurboSceneObjectTexture> texture) = 0;

	virtual std::shared_ptr<ITurboSceneVertexShader> VertexShader() = 0;
	virtual void VertexShader(std::shared_ptr<ITurboSceneVertexShader> vertexShader) = 0;

	virtual std::shared_ptr<ITurboScenePixelShader> PixelShader() = 0;
	virtual void PixelShader(std::shared_ptr<ITurboScenePixelShader> pixelShader) = 0;
};

