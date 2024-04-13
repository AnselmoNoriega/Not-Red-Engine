#pragma once

namespace NotRed::Graphics
{
	class PixelShader final
	{
	public:
		void Initialize(const std::filesystem::path& filePath);
		void Terminate();
		void Bind();

	private:
		ID3D11PixelShader* mPixelShader = nullptr;
	};
}