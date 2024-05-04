#include <Not-Red/Inc/NotRed.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

using namespace NotRed;
using namespace NotRed::Graphics;
using namespace NotRed::Math;

struct Arguments
{
    std::filesystem::path inputFileName;
    std::filesystem::path outputFileName;
    float scale = 1.0f;
};

Vector3 ToVector3(const aiVector3D& v)
{
    return {
        static_cast<float>(v.x),
        static_cast<float>(v.y),
        static_cast<float>(v.z)
    };
}

Vector2 ToTexCoord(const aiVector3D& v)
{
    return {
        static_cast<float>(v.x),
        static_cast<float>(v.y)
    };
}

std::optional<Arguments> ParseArgs(int argc, char* argv[])
{
    if (argc < 3)
    {
        return std::nullopt;
    }

    Arguments args;
    args.inputFileName = argv[argc - 2];
    args.outputFileName = argv[argc - 1];
    for (int i = 1; i + 2 < argc; ++i)
    {
        if (strcmp(argv[i], "-scale") == 0) 
        {
            args.scale = atof(argv[i + 1]);
            ++i;
        }
    }

    return args;
}

int main(int argc, char* argv[])
{
    const auto argOpt = ParseArgs(argc, argv);
    if (argOpt.has_value() == false)
    {
        printf("No arguments!");
            return -1;
    }

    const Arguments& args = argOpt.value();
    Assimp::Importer importer;
    const uint32_t flags = aiProcessPreset_TargetRealtime_Quality | aiProcess_ConvertToLeftHanded;
    const aiScene* scene = importer.ReadFile(args.inputFileName.string().c_str(), flags);
    if (scene == nullptr)
    {
        printf("Error: %s\n", importer.GetErrorString());
        return -1;
    }

    printf("Importing %s...\n", args.inputFileName.string().c_str());

    Model model;
    if (scene->HasMeshes())
    {
        printf("Reading Mesh Data...\n");
        for (int i = 0; i < scene->mNumMeshes; ++i)
        {
            const aiMesh* assimpMesh = scene->mMeshes[i]; 
            if (assimpMesh->mPrimitiveTypes != aiPrimitiveType_TRIANGLE)
            {
                continue;
            }

            const uint32_t numVertices = assimpMesh->mNumVertices;
            const uint32_t numFaces = assimpMesh->mNumFaces;
            const uint32_t numIndices = numFaces * 3;

            Model::MeshData& meshData = model.meshData.emplace_back();

            printf("Reading Material Index...\n");
            meshData.materialIndex = assimpMesh->mMaterialIndex;

            printf("Reading Vertices...\n");
            Mesh& mesh = meshData.mesh;
            mesh.vertices.reserve(numVertices);

            const aiVector3D* positions = assimpMesh->mVertices;
            const aiVector3D* normals = assimpMesh->mNormals;
            const aiVector3D* tangents = assimpMesh->HasTangentsAndBitangents() ? assimpMesh->mTangents : nullptr;
            const aiVector3D* texCoord = assimpMesh->HasTextureCoords(0) ? assimpMesh->mTextureCoords[0] : nullptr;

            for (int v = 0; v < numVertices; ++v)
            {
                Vertex& vertex = mesh.vertices.emplace_back();
                vertex.position = ToVector3(positions[v]) * args.scale;
                vertex.normal = ToVector3(normals[v]);
                vertex.tangent = tangents ? ToVector3(tangents[v]) : Vector3::Zero;
                vertex.uvCoord = texCoord ? ToTexCoord(texCoord[v]) : Vector2::Zero;
            }

            printf("Reading Indices...\n");
            mesh.indices.reserve(numIndices);
            const aiFace* aiFaces = assimpMesh->mFaces;
            for (uint32_t f = 0; f < numFaces; ++f)
            {
                const aiFace& assimpFace = aiFaces[f];
                for (uint32_t j = 0; j < 3; ++j)
                {
                    mesh.indices.push_back(assimpFace.mIndices[j]);
                }
            }
        }
    }

    printf("Saving Model...\n");
    if (ModelIO::SaveModel(args.outputFileName, model))
    {
        printf("Model saved [%s]...\n", args.outputFileName.string().c_str());
    }
    else
    {
        printf("Failed to save Model data [%s]...\n", args.outputFileName.string().c_str());
    }

    return 0;
}