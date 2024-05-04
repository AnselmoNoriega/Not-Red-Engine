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
		filePath.replace_extension("model");

		FILE* file = nullptr;
		fopen_s(&file, filePath.string().c_str(), "r");
		if (file == nullptr)
		{
			return false;
		}

		uint32_t meshCount = 0;
		fscanf_s(file, "MeshCount: %d\n", &meshCount);
		model.meshData.resize(meshCount);
		for (uint32_t i = 0; i < meshCount; ++i)
		{
			Model::MeshData& meshData = model.meshData[i];
			fscanf_s(file, "MaterialIndex: %d\n", &meshData.materialIndex);

			Mesh& mesh = meshData.mesh;
			uint32_t vertexCount = static_cast<uint32_t>(mesh.vertices.size());
			fscanf_s(file, "VertexCount: %d\n", &vertexCount);
			mesh.vertices.resize(vertexCount);
			for (Vertex& v : mesh.vertices)
			{
				fscanf_s(file, "%f %f %f %f %f %f %f %f %f %f %f\n",
					&v.position.x, &v.position.y, &v.position.z,
					&v.normal.x, &v.normal.y, &v.normal.z,
					&v.tangent.x, &v.tangent.y, &v.tangent.z,
					&v.uvCoord.x, &v.uvCoord.y);
			}

			uint32_t indexCount = static_cast<uint32_t>(mesh.indices.size());
			fscanf_s(file, "IndexCount: %d\n", &indexCount);
			mesh.indices.resize(indexCount);
			for (uint32_t j = 2; j < indexCount; j += 3)
			{
				fscanf_s(file, "%d %d %d\n", &mesh.indices[j - 2], &mesh.indices[j - 1], &mesh.indices[j]);
			}
		}

		fclose(file);
		return true;
	}
}
