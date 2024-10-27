#include "Precompiled.h"
#include "SaveUtil.h"

namespace NotRed
{
	void SaveUtil::SaveFloat(const char* key, float value, rapidjson::Document& doc, rapidjson::Value& member)
	{
		rapidjson::GenericStringRef<char> str(key);
		member.AddMember(str, value, doc.GetAllocator());
	}

	void SaveUtil::SaveInt(const char* key, int value, rapidjson::Document& doc, rapidjson::Value& member)
	{
		rapidjson::GenericStringRef<char> str(key);
		member.AddMember(str, value, doc.GetAllocator());
	}

	void SaveUtil::SaveBool(const char* key, bool value, rapidjson::Document& doc, rapidjson::Value& member)
	{
		rapidjson::GenericStringRef<char> str(key);
		member.AddMember(str, value, doc.GetAllocator());
	}

	void SaveUtil::SaveString(const char* key, const char* value, rapidjson::Document& doc, rapidjson::Value& member)
	{
		rapidjson::GenericStringRef<char> str(key);
		rapidjson::GenericStringRef<char> strvalue(value);
		member.AddMember(str, strvalue, doc.GetAllocator());
	}

	void SaveUtil::SaveStringArray(const char* key, const std::vector<std::string>& value, rapidjson::Document& doc, rapidjson::Value& member)
	{
		rapidjson::GenericStringRef<char> str(key);
		rapidjson::Value strArray(rapidjson::kArrayType);
		for (const std::string& v : value)
		{
			rapidjson::GenericStringRef<char> strValue(v.c_str());
			strArray.PushBack(strValue, doc.GetAllocator());
		}
		member.AddMember(str, strArray, doc.GetAllocator());
	}

	void SaveUtil::SaveVector3(const char* key, const Vector3& value, rapidjson::Document& doc, rapidjson::Value& member)
	{
		rapidjson::GenericStringRef<char> str(key);
		rapidjson::Value vecArray(rapidjson::kArrayType);
		vecArray.PushBack(value.x, doc.GetAllocator());
		vecArray.PushBack(value.y, doc.GetAllocator());
		vecArray.PushBack(value.z, doc.GetAllocator());
		member.AddMember(str, vecArray, doc.GetAllocator());
	}

	void SaveUtil::SaveQuaternion(const char* key, const Quaternion& value, rapidjson::Document& doc, rapidjson::Value& member)
	{
		rapidjson::GenericStringRef<char> str(key);
		rapidjson::Value quatArray(rapidjson::kArrayType);
		quatArray.PushBack(value.x, doc.GetAllocator());
		quatArray.PushBack(value.y, doc.GetAllocator());
		quatArray.PushBack(value.z, doc.GetAllocator());
		quatArray.PushBack(value.w, doc.GetAllocator());
		member.AddMember(str, quatArray, doc.GetAllocator());
	}

	void SaveUtil::SaveColor(const char* key, const Color& value, rapidjson::Document& doc, rapidjson::Value& member)
	{
		rapidjson::GenericStringRef<char> str(key);
		rapidjson::Value colorArray(rapidjson::kArrayType);
		colorArray.PushBack(value.x, doc.GetAllocator());
		colorArray.PushBack(value.y, doc.GetAllocator());
		colorArray.PushBack(value.z, doc.GetAllocator());
		colorArray.PushBack(value.w, doc.GetAllocator());
		member.AddMember(str, colorArray, doc.GetAllocator());
	}
}