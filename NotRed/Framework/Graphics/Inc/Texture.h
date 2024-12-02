#pragma once

namespace NotRed::Graphics
{
	class Texture
	{
	public:
		enum class Format
		{
			RGBA_U8,
			RGBA_U32,
			R32_FLOAT
		};

		static void UnbindPS(uint32_t slot = 0);

		Texture() = default;
		virtual ~Texture();

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		Texture(Texture&&) noexcept;
		Texture& operator=(Texture&&) noexcept;

		virtual void Initialize(const std::filesystem::path& fileName);
		virtual void Initialize(uint32_t width, uint32_t height, Format format);
		virtual void Terminate();

		void BindVS(uint32_t slot) const;
		void BindPS(uint32_t slot) const;

		void* GetRawData() const { return mShaderResourceView; }

		uint32_t GetWidth() const;
		uint32_t GetHeight() const;

	protected:
		DXGI_FORMAT GetDXGIFormat(Format format);
		ID3D11ShaderResourceView* mShaderResourceView = nullptr;

		uint32_t mWidth = 0;
		uint32_t mHeight = 0;
	};
}