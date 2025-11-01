#pragma once

#include <SDL/SDL.h>
#include <unordered_map>
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/mat3x3.hpp>

#include "Texture.h"

namespace jLab
{
	class Renderer
	{
	public:
		Renderer(class Game* game);

		bool Init(int screenWidth, int screenHeight);
		void Shutdown();
		void Draw();

		void AddMeshComponent(class MeshComponent* mesh);
		void RemoveMeshComponent(class MeshComponent* mesh);

		Texture* GetTexture(const std::string& filename, bool flipVertically = false, Texture::Type type = Texture::Type::Diffuse);
		Texture* GetTexture(const std::string& filename, const aiTexture* textureData, bool flipVertically = false, Texture::Type type = Texture::Type::Diffuse);
		class Model* GetModel(const std::string& filename, bool flipUVs = true, class Skeleton* skeleton = nullptr);

		int GetScreenWidth() const { return mScreenWidth; }
		int GetScreenHeight() const { return mScreenHeight; }

		SDL_Window* GetWindow() const { return mWindow; }

		void SetSetViewMatrix(const glm::mat4& view) { mView = view; }

	private:
		void SetShaderUniforms(const class Shader* shader);

	private:
		class Game* mGame;

		int mScreenWidth;
		int mScreenHeight;

		SDL_Window* mWindow;
		SDL_GLContext mContext;

		glm::mat4 mProjection;
		glm::mat4 mView;
		glm::mat4 mOrtho;

		class Shader* mMeshShader;

		std::unordered_map<std::string, Texture*> mTextures;
		std::unordered_map<std::string, class Model*> mModels;
		std::vector<class MeshComponent*> mMeshes;
	};
}