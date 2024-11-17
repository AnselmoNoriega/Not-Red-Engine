#pragma once

#include "Colors.h"
#include "TextureManager.h"

namespace NotRed::Graphics
{
	enum class Pivot
	{
		TopLeft,
		Top,
		TopRight,
		Left,
		Center,
		Right,
		BottomLeft,
		Bottom,
		BottomRight
	};

	enum class Flip
	{
		None,
		Horizontal,
		Vertical,
		Both
	};

	class UISprite
	{
	public:
		UISprite() = default;
		~UISprite();

		void Initialize(const std::filesystem::path& filePath);
		void Terminate();

		void SetPosition(const Math::Vector2& position);
		void SetScale(const Math::Vector2& scale);
		void SetRect(uint32_t top, uint32_t left, uint32_t right, uint32_t bottom);
		void SetPivot(Pivot pivot);
		void SetFlip(Flip flip);
		void SetColor(const Color& color);
		void SetRotation(float rotation);

		bool IsInSprite(float x, float y);

	private:
		friend class UISpriteRenderer;

	private:
		void UpdateOrigin();

	private:
		TextureID mTextureId;
		RECT mRect = { 0, 0, 100, 100 };

		DirectX::XMFLOAT2 mPosition = { 0.0f, 0.0f };
		DirectX::XMFLOAT2 mOrigin = { 0.0f, 0.0f };
		DirectX::XMFLOAT2 mScale = { 1.0f, 1.0f };
		DirectX::XMVECTOR mColor = DirectX::Colors::White;

		float mRotation = 0.0f;
		Pivot mPivot = Pivot::Center;

		DirectX::SpriteEffects mFlip = DirectX::SpriteEffects_None;
	};
}