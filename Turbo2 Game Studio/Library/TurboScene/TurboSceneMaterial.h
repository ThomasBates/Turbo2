#pragma once

#include <ITurboSceneMaterial.h>
#include <ITurboScenePixelShader.h>
#include <ITurboSceneVertexShader.h>

namespace Turbo
{
	namespace Scene
	{
		class TurboSceneMaterial : public ITurboSceneMaterial
		{
		public:
			//  Constructors and Destructors -----------------------------------------------------------------------------------
			TurboSceneMaterial(
				TurboColor meshColor,
				TurboColor diffuseColor,
				TurboColor specularColor,
				float specularExponent,
				std::shared_ptr<ITurboSceneTexture> texture,
				std::shared_ptr<ITurboSceneVertexShader> vertexShader,
				std::shared_ptr<ITurboScenePixelShader> pixelShader);
			TurboSceneMaterial(
				TurboColor meshColor,
				TurboColor diffuseColor,
				TurboColor specularColor,
				float specularExponent,
				std::string textureName,
				std::string vertexShaderName,
				std::string pixelShaderName);

			//  ITurboSceneObjectMaterial Properties ---------------------------------------------------------------------------
			virtual TurboColor MeshColor() { return _meshColor; }
			virtual void MeshColor(TurboColor meshColor) { _meshColor = meshColor; }

			virtual TurboColor DiffuseColor() { return _diffuseColor; }
			virtual void DiffuseColor(TurboColor diffuseColor) { _diffuseColor = diffuseColor; }

			virtual TurboColor SpecularColor() { return _specularColor; }
			virtual void SpecularColor(TurboColor specularColor) { _specularColor = specularColor; }

			virtual float SpecularExponent() { return _specularExponent; }
			virtual void SpecularExponent(float specularExponent) { _specularExponent = specularExponent; }

			virtual std::shared_ptr<ITurboSceneTexture> Texture() { return _texture; }
			virtual void Texture(std::shared_ptr<ITurboSceneTexture> texture) { _texture = texture; }

			virtual std::shared_ptr<ITurboSceneVertexShader> VertexShader() { return _vertexShader; }
			virtual void VertexShader(std::shared_ptr<ITurboSceneVertexShader> vertexShader) { _vertexShader = vertexShader; }

			virtual std::shared_ptr<ITurboScenePixelShader> PixelShader() { return _pixelShader; }
			virtual void PixelShader(std::shared_ptr<ITurboScenePixelShader> pixelShader) { _pixelShader = pixelShader; }

			//  ITurboSceneObjectMaterial Methods ------------------------------------------------------------------------------

		private:
			TurboColor _meshColor;
			TurboColor _diffuseColor;
			TurboColor _specularColor;
			float _specularExponent;
			std::shared_ptr<ITurboSceneTexture> _texture;
			std::shared_ptr<ITurboSceneVertexShader> _vertexShader;
			std::shared_ptr<ITurboScenePixelShader> _pixelShader;
		};
	}
}