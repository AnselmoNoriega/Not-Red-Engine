#pragma once

namespace NotRed::Graphics
{
	class PixelShader final
	{
	public:
		void Initialize(const std::filesystem::path& filePath, const char* entryPoint = "PS");
		void Terminate();
		void Bind() const;

	private:
		ID3D11PixelShader* mPixelShader = nullptr;
	};
}