#include "LevelLoader.h"

#include <cstdio>
#include <fstream>
#include <vector>
#include <rapidjson/rapidjson.h>
#include "Game.h"
#include "Renderer.h"

#include "Actor.h"
#include "Game/TargetActor.h"

namespace jLab
{
	const int LEVEL_VERSION = 1;

	std::unordered_map<std::string, ActorFunc> LevelLoader::sActorFactoryMap
	{
		{ "Actor", &Actor::Create<Actor> },
		{ "TargetActor", &Actor::Create<TargetActor> }
	};

	bool LevelLoader::LoadLevel(Game* game, const std::string& filename)
	{
		rapidjson::Document doc;
		if (!LoadJSON(filename, doc))
		{
			printf("ERROR: Failed to Load JSON file '%s'\n", filename.c_str());
			return false;
		}

		int version;
		if (!GetInt(doc, "version", version) || version != LEVEL_VERSION)
		{
			printf("ERROR: Incorrect level file version for '%s'\n", filename.c_str());
			return false;
		}

		if (!LoadGlobalProperties(game, doc))
		{
			printf("ERROR: Failed to Load Global Properties\n");
			return false;
		}

		const rapidjson::Value& actorsObj = doc["actors"];
		if (actorsObj.IsArray())
			LoadActors(game, actorsObj);

		return true;
	}

	bool LevelLoader::LoadJSON(const std::string& filename, rapidjson::Document& outDoc)
	{
		std::ifstream file(filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate);

		if(!file.is_open())
		{
			printf("ERROR: Failed to Open file '%s'\n", filename.c_str());
			return false;
		}

		std::ifstream::pos_type fileSize = file.tellg();			// get the file size
		file.seekg(0, std::ios::beg);								// seek back to start of the file

		std::vector<char> bytes(static_cast<int>(fileSize) + 1);
		file.read(bytes.data(), static_cast<int>(fileSize));

		outDoc.Parse(bytes.data());
		if (!outDoc.IsObject())
		{
			printf("ERROR: Failed to Parse JSON file '%s'\n", filename.c_str());
			return false;
		}

		return true;
	}

	bool LevelLoader::LoadGlobalProperties(Game* game, const rapidjson::Value& inObject)
	{
		const rapidjson::Value& globalProperties = inObject["globalProperties"];
		if (!globalProperties.IsObject())
		{
			printf("ERROR: Failed to find 'globalProperties'\n");
			return false;
		}

		glm::vec3 ambientColor;
		if (GetVec3(globalProperties, "ambientColor", ambientColor))
			game->GetRenderer()->SetAmbientColor(ambientColor);

		const rapidjson::Value& directionalLight = globalProperties["directionalLight"];
		if (directionalLight.IsObject())
		{
			glm::vec3 directionalLightDirection;
			glm::vec3 directionalLightColor;
			if (GetVec3(directionalLight, "direction", directionalLightDirection) && GetVec3(directionalLight, "color", directionalLightColor))
				game->GetRenderer()->SetDirectionalLight(directionalLightDirection, directionalLightColor);
		}

		return true;
	}

	void LevelLoader::LoadActors(Game* game, const rapidjson::Value& inArray)
	{
		for (rapidjson::SizeType i = 0; i < inArray.Size(); i++)
		{
			const rapidjson::Value& actorObj = inArray[i];
			if (actorObj.IsObject())
			{
				std::string type;
				if (GetString(actorObj, "type", type))
				{
					auto iter = sActorFactoryMap.find(type);
					if (iter != sActorFactoryMap.end())
						Actor* actor = iter->second(game, actorObj["properties"]);
					else
						printf("ERROR: Unknown Actor type '%s'\n", type.c_str());
				}
			}
		}
	}


	// JSON Helper getters ------------------------------------------------------------------

	bool LevelLoader::GetInt(const rapidjson::Value& inObject, const char* inProperty, int& outInt)
	{
		auto itr = inObject.FindMember(inProperty);
		if (itr == inObject.MemberEnd())
		{
			printf("ERROR: Property '%s' doesn't exist in the JSON file\n", inProperty);
			return false;
		}

		auto& property = itr->value;
		if(!property.IsInt())
		{
			printf("ERROR: Property '%s' is not an integer\n", inProperty);
			return false;
		}

		outInt = property.GetInt();

		return true;
	}

	bool LevelLoader::GetFloat(const rapidjson::Value& inObject, const char* inProperty, float& outFloat)
	{
		auto itr = inObject.FindMember(inProperty);
		if (itr == inObject.MemberEnd())
		{
			printf("ERROR: Property '%s' doesn't exist in the JSON file\n", inProperty);
			return false;
		}

		auto& property = itr->value;
		if (!property.IsFloat())
		{
			printf("ERROR: Property '%s' is not a float\n", inProperty);
			return false;
		}

		outFloat = property.GetFloat();

		return true;
	}

	bool LevelLoader::GetBool(const rapidjson::Value& inObject, const char* inProperty, bool& outBool)
	{
		auto itr = inObject.FindMember(inProperty);
		if (itr == inObject.MemberEnd())
		{
			printf("ERROR: Property '%s' doesn't exist in the JSON file\n", inProperty);
			return false;
		}

		auto& property = itr->value;
		if (!property.IsBool())
		{
			printf("ERROR: Property '%s' is not a boolean\n", inProperty);
			return false;
		}

		outBool = property.GetBool();

		return true;
	}

	bool LevelLoader::GetString(const rapidjson::Value& inObject, const char* inProperty, std::string& outString)
	{
		auto itr = inObject.FindMember(inProperty);
		if (itr == inObject.MemberEnd())
		{
			printf("ERROR: Property '%s' doesn't exist in the JSON file\n", inProperty);
			return false;
		}

		auto& property = itr->value;
		if (!property.IsString())
		{
			printf("ERROR: Property '%s' is not a string\n", inProperty);
			return false;
		}

		outString = property.GetString();

		return true;
	}

	bool LevelLoader::GetVec3(const rapidjson::Value& inObject, const char* inProperty, glm::vec3& outVec)
	{
		auto itr = inObject.FindMember(inProperty);
		if (itr == inObject.MemberEnd())
		{
			printf("ERROR: Property '%s' doesn't exist in the JSON file\n", inProperty);
			return false;
		}

		auto& property = itr->value;
		if (!property.IsArray() || property.Size() != 3)
		{
			printf("ERROR: Property '%s' is not a Vector3 \n", inProperty);
			return false;
		}

		for (rapidjson::SizeType i = 0; i < property.Size(); i++)
		{
			if (!property[i].IsDouble())
			{
				printf("ERROR: Property '%s' doesn't have double member in the vector3\n", inProperty);
				return false;
			}
		}

		outVec.x = property[0].GetDouble();
		outVec.y = property[1].GetDouble();
		outVec.z = property[2].GetDouble();

		return true;
	}

	bool LevelLoader::GetQuat(const rapidjson::Value& inObject, const char* inProperty, glm::quat& outQuat)
	{
		auto itr = inObject.FindMember(inProperty);
		if (itr == inObject.MemberEnd())
		{
			printf("ERROR: Property '%s' doesn't exist in the JSON file\n", inProperty);
			return false;
		}

		auto& property = itr->value;
		if (!property.IsArray() || property.Size() != 4)
		{
			printf("ERROR: Property '%s' is not a Quaternion \n", inProperty);
			return false;
		}

		for (rapidjson::SizeType i = 0; i < property.Size(); i++)
		{
			if (!property[i].IsDouble())
			{
				printf("ERROR: Property '%s' doesn't have double member in the vector3\n", inProperty);
				return false;
			}
		}

		outQuat.x = property[0].GetDouble();
		outQuat.y = property[1].GetDouble();
		outQuat.z = property[2].GetDouble();
		outQuat.w = property[3].GetDouble();

		return true;
	}
}