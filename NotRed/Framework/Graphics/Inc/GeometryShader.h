#pragma once

namespace NotRed::Graphics
{
	class GeometryShader final
	{
	public:
		void Initialize(const std::filesystem::path& filePath, const char* entryPoint = "PS");
		void Terminate();
		void Bind();

	private:
		ID3D11GeometryShader* mGeometryShader = nullptr;
	};
}