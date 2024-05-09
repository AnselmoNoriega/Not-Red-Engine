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

Color ToColor(const aiColor3D& color)
{
    return{
        static_cast<float>(color.r),
        static_cast<float>(color.g),
        static_cast<float>(color.b),
        1.0f
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

void ExportEmbeddedTexture(const aiTexture* texture, const Arguments& args, const std::filesystem::path& fileName)
{
    printf("Extracting embedded texture %s\n", fileName.string().c_str());

    std::string fullFileName = args.outputFileName.string();
    fullFileName = fullFileName.substr(0, fullFileName.rfind('/') + 1);
    fullFileName += fileName.filename().string();

    FILE* file = nullptr;
    auto err = fopen_s(&file, fullFileName.c_str(), "wb");
    if (err != 0 || file == nullptr)
    {
        printf("Error: failed to open file %s!\n", fullFileName.c_str());
        return;
    }

    size_t written = fwrite(texture->pcData, 1, texture->mWidth, file);
    ASSERT(written == texture->mWidth, "Error: failed to extract embedded texture");
    fclose(file);
}

std::string FindTexture(const aiScene* scene, const aiMaterial* aiMaterial,
    aiTextureType textureType, const Arguments& args, const std::string& suffix,
    uint32_t materialIndex
)
{
    const uint32_t textureCount = aiMaterial->GetTextureCount(textureType);
    if (textureCount == 0)
    {
        return "";
    }

    std::filesystem::path textureName;
    aiString texturePath;
    if (aiMaterial->GetTexture(textureType, 0, &texturePath) == aiReturn_SUCCESS)
    {
        if (texturePath.C_Str()[0] == '*')
        {
            std::string fileName = args.inputFileName.string();
            fileName.erase(fileName.length() - 4);
            fileName += suffix;
            fileName += texturePath.C_Str()[1];

            ASSERT(scene->HasTextures(), "Error: no embedded texture found!");

            int textureIndex = atoi(texturePath.C_Str() + 1);
            ASSERT(textureIndex < scene->mNumTextures, "Error: invalid texture index!");

            const aiTexture* embeddedTexture = scene->mTextures[textureIndex];
            ASSERT(embeddedTexture->mHeight == 0, "Error: uncompressed texture found!");

            if (embeddedTexture->CheckFormat("jpg"))
            {
                fileName += ".jpg";
            }
            else if (embeddedTexture->CheckFormat("png"))
            {
                fileName += ".png";
            }
            else
            {
                ASSERT(false, "Error: unrecognized texture format!");
            }

            ExportEmbeddedTexture(embeddedTexture, args, fileName);
            printf("Adding textuer s%...\n", fileName.c_str());
            textureName = fileName;
        }
        else if (auto embeddedTeture = scene->GetEmbeddedTexture(texturePath.C_Str()); embeddedTeture)
        {
            std::filesystem::path embeddedFilePath = texturePath.C_Str();
            std::string fileName = args.inputFileName.string();
            fileName.erase(fileName.length() - 4);
            fileName += suffix;
            fileName += "_" + std::to_string(materialIndex);
            fileName += embeddedFilePath.extension().string();

            ExportEmbeddedTexture(embeddedTeture, args, fileName);
            printf("Adding textuer s%...\n", textureName.c_str());
            textureName = fileName;
        }
        else
        {
            std::filesystem::path embeddedFilePath = texturePath.C_Str();
            std::string fileName = args.inputFileName.string();

            printf("Adding textuer s%...\n", textureName.c_str());
            textureName = fileName;
        }
    }
    return textureName.filename().string();
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

    if (scene->HasMaterials())
    {
        printf("Reading Material Data...");

        const uint32_t numMaterials = scene->mNumMaterials;
        model.meshData.reserve(numMaterials);
        for (uint32_t materialIndex = 0; materialIndex < numMaterials; ++materialIndex)
        {
            const aiMaterial* assimpMaterial = scene->mMaterials[materialIndex];
            aiColor3D ambient, diffuse, emissive, specular;
            ai_real specularPower = 1.0f;

            assimpMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambient);
            assimpMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);
            assimpMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, emissive);
            assimpMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specular);
            assimpMaterial->Get(AI_MATKEY_SHININESS, specularPower);

            Model::MeterialData& materialData = model.meterialData.emplace_back();
            materialData.material.ambient = ToColor(ambient);
            materialData.material.diffuse = ToColor(diffuse);
            materialData.material.emissive = ToColor(emissive);
            materialData.material.specular = ToColor(specular);
            materialData.material.power = (float)specularPower;

            materialData.diffuseMapName = FindTexture(scene, assimpMaterial, aiTextureType_DIFFUSE, args, "_diff", materialIndex);
            materialData.normalMapName = FindTexture(scene, assimpMaterial, aiTextureType_NORMALS, args, "_norm", materialIndex);
            materialData.bumpMapName = FindTexture(scene, assimpMaterial, aiTextureType_DISPLACEMENT, args, "_bump", materialIndex);
            materialData.specularMapName = FindTexture(scene, assimpMaterial, aiTextureType_SPECULAR, args, "_spec", materialIndex);
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

    printf("Saving Material...\n");
    if (ModelIO::SaveMaterial(args.outputFileName, model))
    {
        printf("Material saved [%s]...\n", args.outputFileName.string().c_str());
    }
    else
    {
        printf("Failed to save Material data [%s]...\n", args.outputFileName.string().c_str());
    }

    return 0;
}