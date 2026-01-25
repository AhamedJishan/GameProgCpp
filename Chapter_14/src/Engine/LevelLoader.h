#pragma once

#include <string>
#include <rapidjson/document.h>

namespace jLab
{
	class LevelLoader
	{
	public:
		static bool LoadLevel(class Game* game, const std::string& filename);

	private:
		static bool LoadJSON(const std::string& filename, rapidjson::Document& outDoc);

	private:
		static bool GetInt(rapidjson::Value& inObject, const char* inProperty, int& outInt);
	};
}