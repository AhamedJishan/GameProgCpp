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
		void AddSpriteComponent(class SpriteComponent* sprite);
		void RemoveSpriteComponent(class SpriteComponent* sprite);

		Texture* GetTexture(const std::string& filename, bool flipVertically = false, Texture::Type type = Texture::Type::Diffuse);
		Texture* GetTexture(const std::string& filename, const aiTexture* textureData, bool flipVertically = false, Texture::Type type = Texture::Type::Diffuse);
		class Model* GetModel(const std::string& filename, bool flipUVs = true, class Skeleton* skeleton = nullptr);

		int GetScreenWidth() const { return mScreenWidth; }
		int GetScreenHeight() const { return mScreenHeight; }

		glm::vec3 ScreenToWorldPos(const glm::vec3& screenPosition);
		void ScreenToWorldDir(glm::vec3& outStart, glm::vec3& outDirection);

		SDL_Window* GetWindow() const { return mWindow; }

		void SetSetViewMatrix(const glm::mat4& view) { mView = view; }

		// Game Specific
		class Texture* GetMirrorTexture() const { return mMirrorTexture; }
		void SetMirrorView(const glm::mat4& view) { mMirrorView = view; }

	private:
		void SetShaderUniforms(const class Shader* shader, const glm::mat4& view);
		void InitSpriteQuad();
		void DeleteSpriteQuad();
		void UseSpriteQuad();
		bool LoadShaders();
		bool CreateMirrorRenderTarget();
		void DeleteMirrorRenderTarget();
		void Draw3DScene(unsigned int framebuffer, const glm::mat4& view, float viewPortScale, bool lit);
		void DrawFromGBuffer();

	private:
		class Game* mGame;
		class GBuffer* mGBuffer;

		int mScreenWidth;
		int mScreenHeight;

		SDL_Window* mWindow;
		SDL_GLContext mContext;

		glm::mat4 mProjection;
		glm::mat4 mView;
		glm::mat4 mOrtho;

		class Shader* mMeshShader;
		class Shader* mSkinnedMeshShader;
		class Shader* mGGlobalShader;
		class Shader* mSpriteShader;

		std::unordered_map<std::string, Texture*> mTextures;
		std::unordered_map<std::string, class Model*> mModels;
		std::vector<class MeshComponent*> mMeshes;
		std::vector<class SkinnedMeshComponent*> mSkinnedMeshes;
		std::vector<class SpriteComponent*> mSprites;
		unsigned int mSpriteVAO, mSpriteVBO, mSpriteEBO;

		// Game Specific
		unsigned int mMirrorFBO;
		class Texture* mMirrorTexture;
		glm::mat4 mMirrorView;
	};
}