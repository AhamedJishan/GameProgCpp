#pragma once

#include <string>
#include <glm/vec3.hpp>
#include <rapidjson/document.h>

namespace jLab
{
	class LevelLoader
	{
	public:
		static bool LoadLevel(class Game* game, const std::string& filename);

	private:
		static bool LoadJSON(const std::string& filename, rapidjson::Document& outDoc);
		static bool LoadGlobalProperties(class Game* game, const rapidjson::Value& inObject);

	private:
		static bool GetInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt);
		static bool GetFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat);
		static bool GetBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool);
		static bool GetString(const rapidjson::Value& inObject, const char* inProperty, std::string& outString);
		static bool GetVec3(const rapidjson::Value& inObject, const char* inProperty, glm::vec3& outVec);
	};
}