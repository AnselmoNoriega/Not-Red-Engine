#pragma once

namespace NotRed::Graphics
{
	class GeometryShader final
	{
	public:
		void Initialize(const std::filesystem::path& filePath);
		void Terminate();
		void Bind();
		void Unbind();

	private:
		ID3D11GeometryShader* mGeometryShader = nullptr;
	};
}