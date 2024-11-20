#pragma once

namespace NotRed::Graphics
{
    class UISprite;

    class UISpriteRenderer final
    {
    public:
        static void StaticInitialize();
        static void StaticTerminate();
        static UISpriteRenderer* Get();

        UISpriteRenderer() = default;
        ~UISpriteRenderer();

        void Initialize();
        void Terminate();

        void BeginRender();
        void EndRender();

        void Render(const UISprite* uiSprite);

    private:
        DirectX::CommonStates* mCommonStates = nullptr;
        DirectX::SpriteBatch* mSpriteBatch = nullptr;
        DirectX::XMMATRIX mTransform = DirectX::XMMatrixIdentity();
    };
}