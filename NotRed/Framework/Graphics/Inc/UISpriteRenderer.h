#pragma once

namespace NotRed::Graphics
{
    class UISprite;

    class UISpriteRenderer final
    {
    public:
        static void StaticInitialize();
        static void StaticTerminate();

        UISpriteRenderer() = default;
        ~UISpriteRenderer();

        void Initialize();
        void Terminate();

        void BeginRender();
        void EndRender();

        void Render();

    private:
        DirectX::CommonStates* mCommonStates = nullptr;
        DirectX::SpriteBatch* mSpriteBatch = nullptr;
        DirectX::XMMATRIX mTransform = DirectX::XMMatrixIdentity();
    };
}