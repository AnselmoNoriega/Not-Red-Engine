#pragma once

#include "Common.h"
#include "Colors.h"

namespace NotRed::Graphics
{
	// Vertex element flags
	constexpr uint32_t VE_Position		= 0x1 << 0;
	constexpr uint32_t VE_Normal		= 0x1 << 1;
	constexpr uint32_t VE_Tangent		= 0x1 << 2;
	constexpr uint32_t VE_Color			= 0x1 << 3;
	constexpr uint32_t VE_TexCoord		= 0x1 << 4;
	constexpr uint32_t VE_BlendIndex	= 0x1 << 5;
	constexpr uint32_t VE_BlendWeight	= 0x1 << 6;

	#define VERTEX_FORMAT(fmt)\
		static constexpr uint32_t Format = fmt

	struct VertexP
	{
		VERTEX_FORMAT(VE_Position);
		Math::Vector3 position;
	};

	struct VertexPC
	{
		VERTEX_FORMAT(VE_Position | VE_Color);
		Math::Vector3 position;
		Color color;
	};

	struct VertexPX
	{
		VERTEX_FORMAT(VE_Position | VE_TexCoord);
		Math::Vector3 position;
		Math::Vector2 uvCoord;
	};

	struct Vertex
	{
		VERTEX_FORMAT(VE_Position | VE_Normal | VE_Tangent | VE_TexCoord | VE_BlendIndex | VE_BlendWeight);
		static constexpr int MaxBoneWeights = 4;

		Math::Vector3 position;
		Math::Vector3 normal;
		Math::Vector3 tangent;
		Math::Vector2 uvCoord;
		int boneIndices[MaxBoneWeights] = {};
		float boneWeights[MaxBoneWeights] = {};
	};
}


// Texture vs Image:
// -----------------
// Image = a particular file format
//	e.g. png, jpg, gif, targa, bmp, ...

// In memory:
//
// <filename>.jpg
// |
// V
// 324 653 75 123 72 5 2 2 11 1  ....
//
// |
// V
// Texture = colors ready to be sampled
// 23 234 255 109 244 -- get color at (u,v) --->
// 23 234 255 109 244
// 23 234 255 109 244
// 23 234 255 109 244
// 23 234 255 109 244
// 23 234 255 109 244
// 23 234 255 109 244
// 23 234 255 109 244
