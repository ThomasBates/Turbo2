#pragma once

#include<pch.h>

#include <ITurboSceneTexture.h>
#include <ITurboScenePixelShader.h>
#include <ITurboSceneVertexShader.h>

#include <TurboColor.h>

class ITurboSceneMaterial
{
public:
	virtual TurboColor MeshColor() = 0;
	virtual void MeshColor(TurboColor meshColor) = 0;

	virtual TurboColor DiffuseColor() = 0;
	virtual void DiffuseColor(TurboColor diffuseColor) = 0;

	virtual TurboColor SpecularColor() = 0;
	virtual void SpecularColor(TurboColor specularColor) = 0;

	virtual float SpecularExponent() = 0;
	virtual void SpecularExponent(float specularExponent) = 0;

	virtual std::shared_ptr<ITurboSceneTexture> Texture() = 0;
	virtual void Texture(std::shared_ptr<ITurboSceneTexture> texture) = 0;

	virtual std::shared_ptr<ITurboSceneVertexShader> VertexShader() = 0;
	virtual void VertexShader(std::shared_ptr<ITurboSceneVertexShader> vertexShader) = 0;

	virtual std::shared_ptr<ITurboScenePixelShader> PixelShader() = 0;
	virtual void PixelShader(std::shared_ptr<ITurboScenePixelShader> pixelShader) = 0;
};

