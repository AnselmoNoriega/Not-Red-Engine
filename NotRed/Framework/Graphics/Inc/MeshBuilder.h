#pragma once
#include "MeshTypes.h"

namespace NotRed::Graphics
{
	class MeshBuilder
	{
	public:
		static MeshPC CreatePyramidPC(float size);

		static MeshPC CreateCubePC(float size);
		static Mesh CreateCube(float size);

		static MeshPC CreateRectPC(float width, float height, float depth);

		static MeshPC CreateVerticalPlanePC(uint32_t numRows, uint32_t numCols, float spacing);
		static MeshPC CreateHorizontalPlanePC(uint32_t numRows, uint32_t numCols, float spacing);
		static MeshPX CreateVerticalPlanePX(uint32_t numRows, uint32_t numCols, float spacing);
		static Mesh CreateVerticalPlane(uint32_t numRows, uint32_t numCols, float spacing);
		
		static MeshPX CreateHorizontalPlanePX(uint32_t numRows, uint32_t numCols, float spacing);
		static Mesh CreateHorizontalPlane(uint32_t numRows, uint32_t numCols, float spacing);

		static MeshPC CreateCylinderPC(uint32_t slices, uint32_t rings);

		static MeshPC CreateSpherePC(uint32_t slices, uint32_t rings, float radius);
		static MeshPX CreateSpherePX(uint32_t slices, uint32_t rings, float radius);
		static Mesh CreateSphere(uint32_t slices, uint32_t rings, float radius);

		static MeshPX CreateSkySpherePX(uint32_t slices, uint32_t rings, float radius);
		static MeshPX CreateSkyBoxPX(float size);

		static MeshPX CreateScreenQuad();

		static Mesh CreateSpriteQuad(float width, float height);
	};
}