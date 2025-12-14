#include "Game.h"

#include <cstdio>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <rapidjson/document.h>
#include <algorithm>
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "Renderer.h"
#include "InputSystem.h"
#include "PhysWorld.h"
#include "Actor.h";
#include "Font.h"
#include "Skeleton.h"
#include "Animation.h"
#include "UIScreen.h"

#include "Game/GroundActor.h"
#include "Game/WallActor.h"
#include "Game/FollowActor.h"
#include "Game/AnimTestActor.h"
#include "Game/PauseMenu.h"

namespace jLab
{
	Game::Game()
	{
		mTicksCount = 0;
		mUpdatingActors = false;
		mGameState = GameState::Gameplay;
	}

	bool Game::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		{
			printf("ERROR: Failed to Initialize SDL\n\t'%s'\n", SDL_GetError());
			return false;
		}
		if (TTF_Init() != 0)
		{
			printf("Failed to Initialize SDL TTF: %s", TTF_GetError());
			return false;
		}

		mRenderer = new Renderer(this);
		if (!mRenderer->Init(1280, 720))
		{
			printf("ERROR: Failed to Initialize Renderer\n");
			return false;
		}

		mInputSystem = new InputSystem(this);
		mInputSystem->Init();
		mPhysWorld = new PhysWorld(this);

		LoadData();

		return true;
	}

	void Game::Shutdown()
	{
		UnloadData();
		mRenderer->Shutdown();
		delete mRenderer;
		mInputSystem->Shutdown();
		delete mInputSystem;

		SDL_Quit();
	}

	void Game::AddActor(Actor* actor)
	{
		if (mUpdatingActors)
			mPendingActors.push_back(actor);
		else
			mActors.push_back(actor);
	}

	void Game::RemoveActor(Actor* actor)
	{
		auto it = std::find(mActors.begin(), mActors.end(), actor);
		if (it != mActors.end())
		{
			std::iter_swap(it, mActors.end());
			mActors.pop_back();
		}

		it = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
		if (it != mPendingActors.end())
		{
			std::iter_swap(it, mPendingActors.end());
			mPendingActors.pop_back();
		}
	}

	void Game::PushUI(UIScreen* screen)
	{
		mUIStack.emplace_back(screen);
	}

	Skeleton* Game::GetSkeleton(const std::string& filename)
	{
		auto it = mSkeletons.find(filename);
		if (it != mSkeletons.end())
			return it->second;

		Skeleton* skeleton = new Skeleton();
		if (!skeleton->Load(filename))
			return nullptr;

		mSkeletons.emplace(filename, skeleton);
		return skeleton;
	}

	Animation* Game::GetAnimation(const std::string& filename, class Skeleton* skeleton)
	{
		auto it = mAnimations.find(filename);
		if (it != mAnimations.end())
			return it->second;

		Animation* animation = new Animation();
		if (!animation->Load(filename, skeleton))
			return nullptr;

		mAnimations.emplace(filename, animation);
		return animation;
	}

	Font* Game::GetFont(const std::string& filename)
	{
		auto it = mFonts.find(filename);
		if (it != mFonts.end())
			return it->second;

		Font* font = new Font(this);
		if (!font->Load(filename))
			return nullptr;

		mFonts.emplace(filename, font);
		return font;
	}

	void Game::LoadText(const std::string& filename)
	{
		mTexts.clear();

		std::ifstream file(filename.c_str());
		if (!file.is_open())
		{
			printf("ERROR: Failed to open Text File '%s'", filename.c_str());
			return;
		}

		std::stringstream sstream;
		sstream << file.rdbuf();
		std::string fileContent = sstream.str();

		rapidjson::Document doc;
		doc.Parse(fileContent.c_str());
		if (!doc.IsObject())
		{
			printf("ERROR: Text File '%s' is not a valid JSON", filename.c_str());
			return;
		}

		rapidjson::Value& values = doc["TextMap"];
		for (auto it = values.MemberBegin(); it != values.MemberEnd(); it++)
			if (it->name.IsString() && it->value.IsString())
				mTexts.emplace(it->name.GetString(), it->value.GetString());
	}

	const std::string& Game::GetText(const std::string& key)
	{
		std::string errMsg = "**KEY NOT FOUND**";

		auto it = mTexts.find(key);
		if (it != mTexts.end())
			return mTexts[key];
		else
			return errMsg;
	}

	void Game::Run()
	{
		while (mGameState != GameState::Quit)
		{
			ProcessInput();
			UpdateGame();
			GenerateOutput();
		}
	}

	void Game::ProcessInput()
	{
		SDL_Event event;

		mInputSystem->PreUpdate();
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				mGameState = GameState::Quit;
				break;
			default:
				break;
			}
		}
		mInputSystem->Update();
		InputState inputState = mInputSystem->GetState();

		if (inputState.Keyboard.GetKeyDown(SDL_SCANCODE_ESCAPE))
			new PauseMenu(this);

		if (mGameState == GameState::Gameplay)
		{
			mUpdatingActors = true;
			for (Actor* actor : mActors)
				actor->ProcessInput(inputState);
			mUpdatingActors = false;
		}
		else if (!mUIStack.empty())
				mUIStack.back()->Input(inputState);
	}

	void Game::UpdateGame()
	{
		float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
		if (deltaTime > 0.05f)
			deltaTime = 0.05f;
		mTicksCount = SDL_GetTicks();

		if (mGameState == GameState::Gameplay)
		{
			// Update actors
			mUpdatingActors = true;
			for (Actor* actor : mActors)
				actor->ProcessUpdate(deltaTime);
			mUpdatingActors = false;

			// Move Pending Actors to Actors
			for (Actor* actor : mPendingActors)
				mActors.emplace_back(actor);
			mPendingActors.clear();

			// Move dead actors to temp list
			std::vector<Actor*> deadActors;
			for (Actor* actor : mActors)
				if(actor->GetState() == Actor::State::Dead) deadActors.emplace_back(actor);

			// Delete dead Actors
			for (Actor* actor : deadActors)
				delete actor;
			deadActors.clear();
		}

		// Update UIStack
		for (UIScreen* screen : mUIStack)
			if (screen->GetState() == UIScreen::State::Active) screen->Update(deltaTime);

		// Delete closed UIScreen
		auto iter = mUIStack.begin();
		while (iter != mUIStack.end())
		{
			if ((*iter)->GetState() == UIScreen::State::Closing)
			{
				delete* iter;
				iter = mUIStack.erase(iter);
			}
			else
				iter++;
		}
	}

	void Game::GenerateOutput()
	{
		mRenderer->Draw();
	}

	void Game::LoadData()
	{
		mInputSystem->SetCursorLocked(true);

		LoadText("Assets/Texts/English.jatxt");

		FollowActor* playerController = new FollowActor(this);

		GroundActor* ground = new GroundActor(this);

		WallActor* wa1 = new WallActor(this);
		WallActor* wa2 = new WallActor(this);
		WallActor* wa3 = new WallActor(this);
		WallActor* wa4 = new WallActor(this);

		wa1->SetPosition(glm::vec3(10, 0, 0));
		wa2->SetPosition(glm::vec3(-10, 0, 0));
		wa3->SetPosition(glm::vec3(0, 0, 10));
		wa4->SetPosition(glm::vec3(0, 0, -10));

		wa3->SetRotation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));
		wa4->SetRotation(glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)));

		mWallPlanes.emplace_back(wa1);
		mWallPlanes.emplace_back(wa2);
		mWallPlanes.emplace_back(wa3);
		mWallPlanes.emplace_back(wa4);

		AnimTestActor* animTest = new AnimTestActor(this);
		animTest->SetScale(glm::vec3(0.005f));
		animTest->SetPosition(glm::vec3(0, 0.25f, -5));
	}
	
	void Game::UnloadData()
	{
	}
}