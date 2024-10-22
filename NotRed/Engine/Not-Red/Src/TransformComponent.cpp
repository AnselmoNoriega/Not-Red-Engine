
#include "Precompiled.h"
#include "TransformComponent.h"

using namespace NotRed;
using namespace NotRed::Graphics;

void NotRed::TransformComponent::DebugUI()
{
	ImGui::DragFloat3("Position", &position.x, 0.1f);
	ImGui::DragFloat3("Rotation", &rotation.x, 0.001f);
	ImGui::DragFloat3("Scale", &scale.x, 0.1f);
	SimpleDraw::AddTransform(GetMatrix());
}