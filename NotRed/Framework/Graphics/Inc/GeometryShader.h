#pragma once

namespace NotRed::Graphics
{
	class GeometryShader final
	{
	public:
		void Initialize(const std::filesystem::path& filePath);
		void Terminate();
		void Bind();

	private:
		ID3D11GeometryShader* mGeometryShader = nullptr;
	};
}