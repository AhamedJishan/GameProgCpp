#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <rapidjson/document.h>

namespace jLab
{
	using ActorFunc = std::function<class Actor* (class Game*, const rapidjson::Value&)>;

	class LevelLoader
	{
	public:
		static bool LoadLevel(class Game* game, const std::string& filename);

	private:
		static bool LoadJSON(const std::string& filename, rapidjson::Document& outDoc);
		static bool LoadGlobalProperties(class Game* game, const rapidjson::Value& inObject);
		static void LoadActors(class Game* game, const rapidjson::Value& inArray);

	public:
		static bool GetInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt);
		static bool GetFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat);
		static bool GetBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool);
		static bool GetString(const rapidjson::Value& inObject, const char* inProperty, std::string& outString);
		static bool GetVec3(const rapidjson::Value& inObject, const char* inProperty, glm::vec3& outVec);
		static bool GetQuat(const rapidjson::Value& inObject, const char* inProperty, glm::quat& outQuat);

	private:
		static std::unordered_map<std::string, ActorFunc> sActorFactoryMap;
	};
}