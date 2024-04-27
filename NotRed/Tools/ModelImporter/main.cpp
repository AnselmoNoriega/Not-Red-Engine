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

    return 0;
}