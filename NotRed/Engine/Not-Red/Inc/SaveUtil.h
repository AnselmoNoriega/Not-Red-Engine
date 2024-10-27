#pragma once

namespace NotRed::SaveUtil
{
	void SaveFloat(const char* key, float value, rapidjson::Document& doc, rapidjson::Value& member);
	void SaveInt(const char* key, int value, rapidjson::Document& doc, rapidjson::Value& member);
	void SaveBool(const char* key, bool value, rapidjson::Document& doc, rapidjson::Value& member);
	void SaveString(const char* key, const char* value, rapidjson::Document& doc, rapidjson::Value& member);
	void SaveStringArray(const char* key, const std::vector<std::string>& value, rapidjson::Document& doc, rapidjson::Value& member);
	void SaveVector3(const char* key, const Vector3& value, rapidjson::Document& doc, rapidjson::Value& member);
	void SaveQuaternion(const char* key, const Quaternion& value, rapidjson::Document& doc, rapidjson::Value& member);
	void SaveColor(const char* key, const Color& value, rapidjson::Document& doc, rapidjson::Value& member);
}