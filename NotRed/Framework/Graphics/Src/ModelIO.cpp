#include "Precompiled.h"
#include "ModelIO.h"

#include "Model.h"

using namespace NotRed;
namespace NotRed::Graphics
{
	bool ModelIO::SaveModel(std::filesystem::path filePath, const Model& model)
	{
		if (model.meshData.empty())
		{
			return false;
		}

		filePath.replace_extension("model");

		FILE* file = nullptr;
		fopen_s(&file, filePath.string().c_str(), "w");
		if (file == nullptr)
		{
			return false;
		}

		const uint32_t meshCount = static_cast<uint32_t>(model.meshData.size());
		fprintf_s(file, "MeshCount: %d\n", meshCount);
		for (uint32_t i = 0; i < meshCount; ++i)
		{
			const Model::MeshData& meshData = model.meshData[i];
			fprintf_s(file, "MaterialIndex: %d\n", meshData.materialIndex);

			const Mesh& mesh = meshData.mesh;
			const uint32_t vertexCount = static_cast<uint32_t>(mesh.vertices.size());
			fprintf_s(file, "VertexCount: %d\n", vertexCount);
			for (const Vertex& v : mesh.vertices)
			{
				fprintf_s(file, "%f %f %f %f %f %f %f %f %f %f %f\n",
					v.position.x, v.position.y, v.position.z,
					v.normal.x, v.normal.y, v.normal.z,
					v.tangent.x, v.tangent.y, v.tangent.z,
					v.uvCoord.x, v.uvCoord.y);
			}

			const uint32_t indexCount = static_cast<uint32_t>(mesh.indices.size());
			fprintf_s(file, "IndexCount: %d\n", indexCount);
			for (uint32_t j = 2; j < indexCount; j += 3)
			{
				fprintf_s(file, "%d %d %d\n", mesh.indices[j - 2], mesh.indices[j - 1], mesh.indices[j]);
			}
		}

		fclose(file);
		return true;
	}

	bool ModelIO::LoadModel(std::filesystem::path filePath, Model& model)
	{
		return false;
	}
}
