#include "LevelLoader.h"

#include <cstdio>
#include <fstream>
#include <vector>
#include <rapidjson/rapidjson.h>

namespace jLab
{
	const int LEVEL_VERSION = 1;

	bool LevelLoader::LoadLevel(Game* game, const std::string& filename)
	{
		rapidjson::Document doc;
		if (!LoadJSON(filename, doc))
		{
			printf("ERROR: Failed to Load JSON file '%s'\n", filename.c_str());
			return false;
		}

		int version;
		if(!GetInt(doc, "version", version) || version != LEVEL_VERSION)
		{
			printf("ERROR: Incorrect level file version for '%s'\n", filename.c_str());
			return false;
		}

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


	// JSON Helper getters ------------------------------------------------------------------

	bool LevelLoader::GetInt(rapidjson::Value& inObject, const char* inProperty, int& outInt)
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
}