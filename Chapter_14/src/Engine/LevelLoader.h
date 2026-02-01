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
	using ComponentFunc = std::function<class Component* (class Actor*, const rapidjson::Value&)>;

	class LevelLoader
	{
	public:
		static bool LoadLevel(class Game* game, const std::string& filename);
		static void SaveLevel(class Game* game, const std::string& filename);

	private:
		static bool LoadJSON(const std::string& filename, rapidjson::Document& outDoc);
		static bool LoadGlobalProperties(class Game* game, const rapidjson::Value& inObject);
		static void LoadActors(class Game* game, const rapidjson::Value& inArray);
		static void LoadComponents(class Actor* actor, const rapidjson::Value& inArray);

		static void SaveGlobalProperties(rapidjson::Document::AllocatorType& alloc, class Game* game, rapidjson::Value& inObj);

	public:
		static bool GetInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt);
		static bool GetFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat);
		static bool GetBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool);
		static bool GetString(const rapidjson::Value& inObject, const char* inProperty, std::string& outString);
		static bool GetVec3(const rapidjson::Value& inObject, const char* inProperty, glm::vec3& outVec);
		static bool GetQuat(const rapidjson::Value& inObject, const char* inProperty, glm::quat& outQuat);

		static void AddInt(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, int value);
		static void AddFloat(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, float value);
		static void AddBool(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, bool value);
		static void AddString(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, const std::string& value);
		static void AddVec3(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, glm::vec3 value);
		static void AddQuat(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj, const char* name, glm::quat value);

	private:
		static std::unordered_map<std::string, ActorFunc> sActorFactoryMap;
		static std::unordered_map<std::string, std::pair<int, ComponentFunc>> sComponentFactoryMap;
	};
}