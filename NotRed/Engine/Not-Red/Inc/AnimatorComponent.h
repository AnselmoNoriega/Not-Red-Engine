#pragma once
#include "Component.h"

namespace NotRed
{
	class AnimatorComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Animator);

		void Initialize() override;
		void Terminate() override;

		void Update(float deltaTime) override;
		void DebugUI() override;

		virtual void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;

		bool Play(int index, bool looping = false);

		Graphics::Animator& GetAnimator() { return mAnimator; }
		const Graphics::Animator& GetAnimator() const { return mAnimator; }

	private:
		Graphics::Animator mAnimator;
	};
}