#include "Precompiled.h"
#include "MeshBuilder.h"

using namespace NotRed;
using namespace NotRed::Graphics;

namespace
{
    Color GetNextColor(int& index)
    {
        constexpr Color colorTable[] = {
            Colors::Red,
            Colors::Blue,
            Colors::Green,
            Colors::Black,
            Colors::Pink,
            Colors::Purple,
            Colors::Orange,
            Colors::Yellow,
            Colors::LightGreen,
            Colors::DarkGreen,
            Colors::DarkRed,
            Colors::Salmon,
            Colors::Cyan,
            Colors::Wheat,
            Colors::DarkSalmon
        };

        index = (index + 1) % std::size(colorTable);
        return colorTable[index];
    }

    void CreateCubeIndices(std::vector<uint32_t>& indices)
    {
        indices = {
            0, 1, 2,
            0, 2, 3,

            3, 2, 6,
            3, 6, 7,

            7, 6, 5,
            7, 5, 4,

            0, 5, 1,
            0, 4, 5,

            1, 5, 6,
            1, 6, 2,

            0, 3, 7,
            0, 7, 4
        };
    }

    void CreatePlaneIndices(std::vector<uint32_t>& indices, uint32_t numRows, uint32_t numCols)
    {
        for (uint32_t r = 0; r < numRows; ++r)
        {
            for (uint32_t c = 0; c < numCols; ++c)
            {
                uint32_t i = (r * (numCols + 1)) + c;

                indices.push_back(i);
                indices.push_back(i + numCols + 2);
                indices.push_back(i + 1);

                indices.push_back(i);
                indices.push_back(i + numCols + 1);
                indices.push_back(i + numCols + 2);
            }
        }
    }

    void CreateCapIndices(std::vector<uint32_t>& indices, uint32_t slices, uint32_t topIndex, uint32_t bottomIndex)
    {
        for (uint32_t s = 0; s < slices; ++s)
        {
            indices.push_back(bottomIndex);
            indices.push_back(s);
            indices.push_back(s + 1);

            uint32_t topRowIndex = topIndex - slices - 1 + s;
            indices.push_back(topIndex);
            indices.push_back(topRowIndex + 1);
            indices.push_back(topRowIndex);
        }
    }
}

MeshPC MeshBuilder::CreatePyramidPC(float size)
{
    const float hs = size * 0.5f;

    srand(time(nullptr));
    int index = rand() % 100;

    MeshPC mesh;

    mesh.vertices.push_back({ { -hs, -hs, -hs }, GetNextColor(index) });
    mesh.vertices.push_back({ { 0.0f,  hs, 0.0f }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs, -hs, -hs }, GetNextColor(index) });

    mesh.vertices.push_back({ { -hs, -hs, hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs, -hs, hs }, GetNextColor(index) });

    mesh.indices = {
        0, 1, 2,

        2, 1, 4,

        4, 1, 3,

        0, 3, 1,

        0, 2, 4,
        0, 4, 3
    };

    return mesh;
}

MeshPC MeshBuilder::CreateCubePC(float size)
{
    const float hs = size * 0.5f;

    srand(time(nullptr));
    int index = rand() % 100;

    MeshPC mesh;

    mesh.vertices.push_back({ { -hs, -hs, -hs }, GetNextColor(index) });
    mesh.vertices.push_back({ { -hs,  hs, -hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs,  hs, -hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs, -hs, -hs }, GetNextColor(index) });

    mesh.vertices.push_back({ { -hs, -hs, hs }, GetNextColor(index) });
    mesh.vertices.push_back({ { -hs,  hs, hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs,  hs, hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs, -hs, hs }, GetNextColor(index) });

    CreateCubeIndices(mesh.indices);

    return mesh;
}

MeshPC MeshBuilder::CreateRectPC(float width, float height, float depth)
{
    const float hw = width * 0.5f;
    const float hh = height * 0.5f;
    const float hd = depth * 0.5f;

    srand(time(nullptr));
    int index = rand() % 100;

    MeshPC mesh;

    mesh.vertices.push_back({ { -hw, -hh, -hd }, GetNextColor(index) });
    mesh.vertices.push_back({ { -hw,  hh, -hd }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw,  hh, -hd }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw, -hh, -hd }, GetNextColor(index) });

    mesh.vertices.push_back({ { -hw, -hh, hd }, GetNextColor(index) });
    mesh.vertices.push_back({ { -hw,  hh, hd }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw,  hh, hd }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw, -hh, hd }, GetNextColor(index) });

    CreateCubeIndices(mesh.indices);

    return mesh;
}

MeshPC MeshBuilder::CreateVerticalPlanePC(uint32_t numRows, uint32_t numCols, float spacing)
{
    srand(time(nullptr));
    int index = rand() % 100;

    MeshPC mesh;

    const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;

    float x = -hpw;
    float y = -hph;

    for (uint32_t r = 0; r <= numRows; ++r)
    {
        for (uint32_t c = 0; c <= numCols; ++c)
        {
            mesh.vertices.push_back({ {x, y, 0.0f}, GetNextColor(index) });
            x += spacing;
        }

        x = -hpw;
        y += spacing;
    }

    CreatePlaneIndices(mesh.indices, numRows, numCols);

    return mesh;
}

MeshPC MeshBuilder::CreateHorizontalPlanePC(uint32_t numRows, uint32_t numCols, float spacing)
{
    srand(time(nullptr));
    int index = rand() % 100;

    MeshPC mesh;

    const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;

    float x = -hpw;
    float z = -hph;

    for (uint32_t r = 0; r <= numRows; ++r)
    {
        for (uint32_t c = 0; c <= numCols; ++c)
        {
            mesh.vertices.push_back({ {x, 0.0f, z}, GetNextColor(index) });
            x += spacing;
        }

        x = -hpw;
        z += spacing;
    }

    CreatePlaneIndices(mesh.indices, numRows, numCols);

    return mesh;
}

MeshPX NotRed::Graphics::MeshBuilder::CreateVerticalPlanePX(uint32_t numRows, uint32_t numCols, float spacing)
{
    MeshPX mesh;

    const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;
    const float uInc = 1.0f / static_cast<float>(numCols);
    const float vInc = 1.0f / static_cast<float>(numRows);

    float x = -hpw;
    float y = -hph;
    float u = 0.0f;
    float v = 1.0f;

    for (uint32_t r = 0; r <= numRows; ++r)
    {
        for (uint32_t c = 0; c <= numCols; ++c)
        {
            mesh.vertices.push_back({ {x, y, 0.0f}, {u, v} });
            x += spacing;
            u += uInc;
        }

        x = -hpw;
        y += spacing;
        u = 0.0f;
        v += (-vInc);
    }

    CreatePlaneIndices(mesh.indices, numRows, numCols);

    return mesh;
}

MeshPX NotRed::Graphics::MeshBuilder::CreateHorizontalPlanePX(uint32_t numRows, uint32_t numCols, float spacing)
{
    MeshPX mesh;

    const float hpw = static_cast<float>(numCols) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;
    const float uInc = 1.0f / static_cast<float>(numCols);
    const float vInc = 1.0f / static_cast<float>(numRows);

    float x = -hpw;
    float z = -hph;
    float u = 0.0f;
    float v = 1.0f;

    for (uint32_t r = 0; r <= numRows; ++r)
    {
        for (uint32_t c = 0; c <= numCols; ++c)
        {
            mesh.vertices.push_back({ {x, 0.0f, z}, {u, v} });
            x += spacing;
            u += uInc;
        }

        x = -hpw;
        z += spacing;
        u = 0.0f;
        v += (-vInc);
    }

    CreatePlaneIndices(mesh.indices, numRows, numCols);

    return mesh;
}

MeshPC MeshBuilder::CreateCylinderPC(uint32_t slices, uint32_t rings)
{
    srand(time(nullptr));
    int index = rand() % 100;

    MeshPC mesh;

    const float hh = static_cast<float>(rings) * 0.5f;

    for (uint32_t r = 0; r <= rings; ++r)
    {
        float rPos = static_cast<float>(r);
        for (uint32_t s = 0; s <= slices; ++s)
        {
            float sPos = static_cast<float>(s);
            float rotation = (sPos / static_cast<float>(slices)) * Math::Constants::TwoPi;

            mesh.vertices.push_back( { { cos(rotation), rPos - hh,  sin(rotation) }, GetNextColor(index) });
        }
    }

    mesh.vertices.push_back({ { 0.0f, hh, 0.0f}, GetNextColor(index) });
    mesh.vertices.push_back({ { 0.0f, -hh, 0.0f}, GetNextColor(index) });

    CreatePlaneIndices(mesh.indices, rings, slices);
    CreateCapIndices(mesh.indices, slices, mesh.vertices.size() - 2, mesh.vertices.size() - 1);

    return mesh;
}

MeshPC MeshBuilder::CreateSpherePC(uint32_t slices, uint32_t rings, float radius)
{
    srand(time(nullptr));
    int index = rand() % 100;

    MeshPC mesh;

    float vertRotation = (Math::Constants::Pi / static_cast<float>(rings));
    float horzRotation = (Math::Constants::TwoPi / static_cast<float>(slices));

    for (uint32_t r = 0; r <= rings; ++r)
    {
        float rPos = static_cast<float>(r);
        float phi = rPos * vertRotation;
        for (uint32_t s = 0; s <= slices; ++s)
        {
            float sPos = static_cast<float>(s);
            float rotation = sPos * horzRotation;

            mesh.vertices.push_back
            ({ 
                {
                    radius * sin(rotation) * sin(phi),
                    radius * cos(phi),
                    radius * cos(rotation) * sin(phi)
                },
                    GetNextColor(index) 
            });
        }
    }

    CreatePlaneIndices(mesh.indices, rings, slices);

    return mesh;
}

MeshPX NotRed::Graphics::MeshBuilder::CreateSpherePX(uint32_t slices, uint32_t rings, float radius)
{
    MeshPX mesh;

    float vertRotation = (Math::Constants::Pi / static_cast<float>(rings));
    float horzRotation = (Math::Constants::TwoPi / static_cast<float>(slices));
    float uInc = 1.0f / static_cast<float>(slices);
    float vInc = 1.0f / static_cast<float>(rings);

    for (uint32_t r = 0; r <= rings; ++r)
    {
        float ringPos = static_cast<float>(r);
        float phi = ringPos * vertRotation;
        for (uint32_t s = 0; s <= slices; ++s)
        {
            float slicePos = static_cast<float>(s);
            float rotation = slicePos * horzRotation;

            float u = 1.0f - (uInc * slicePos);
            float v = vInc * ringPos;
            mesh.vertices.push_back({ {
                    radius * sin(rotation) * sin(phi),
                    radius * cos(phi),
                    radius * cos(rotation) * sin(phi)},
                    {u, v } 
                });
        }
    }

    CreatePlaneIndices(mesh.indices, rings, slices);

    return mesh;
}

MeshPX NotRed::Graphics::MeshBuilder::CreateSkySpherePX(uint32_t slices, uint32_t rings, float radius)
{
    MeshPX mesh;

    float vertRotation = (Math::Constants::Pi / static_cast<float>(rings));
    float horzRotation = (Math::Constants::TwoPi / static_cast<float>(slices));
    float uInc = 1.0f / static_cast<float>(slices);
    float vInc = 1.0f / static_cast<float>(rings);

    for (uint32_t r = 0; r <= rings; ++r)
    {
        float ringPos = static_cast<float>(r);
        float phi = ringPos * vertRotation;
        for (uint32_t s = 0; s <= slices; ++s)
        {
            float slicePos = static_cast<float>(s);
            float rotation = slicePos * horzRotation;

            float u = 1.0f - (uInc * slicePos);
            float v = vInc * ringPos;
            mesh.vertices.push_back({ {
                    radius * cos(rotation) * sin(phi),
                    radius * cos(phi),
                    radius * sin(rotation) * sin(phi)},
                    {u, v } 
                });
        }
    }

    CreatePlaneIndices(mesh.indices, rings, slices);

    return mesh;
}

MeshPX NotRed::Graphics::MeshBuilder::CreateSkyBoxPX(float size)
{
    MeshPX mesh;
    const float hs = size * 0.5f;
    const float q = 0.25f;
    const float q2 = q * 2.0f;
    const float q3 = q * 3.0f;
    const float t = 0.3334f;
    const float t2 = t * 2.0f;

    mesh.vertices.push_back({ { hs, -hs, -hs}, {0.0f, t2} });
    mesh.vertices.push_back({ { hs,  hs, -hs}, {0.0f, t} });
    mesh.vertices.push_back({ { hs,  hs,  hs}, {q, t} });
    mesh.vertices.push_back({ { hs, -hs,  hs}, {q, t2} });

    mesh.vertices.push_back({ { hs,  hs,  hs}, {q, t} });
    mesh.vertices.push_back({ { hs,  hs, -hs}, {q, 0.0f} });
    mesh.vertices.push_back({ {-hs,  hs, -hs}, {q2, 0.0f} });
    mesh.vertices.push_back({ {-hs,  hs,  hs}, {q2, t} });

    mesh.vertices.push_back({ { -hs, -hs,  hs }, {q2, t2} });
    mesh.vertices.push_back({ { -hs,  hs,  hs }, {q2, t} });
    mesh.vertices.push_back({ {  hs,  hs,  hs }, {q, t} });
    mesh.vertices.push_back({ {  hs, -hs,  hs }, {q, t2} });

    mesh.vertices.push_back({ {  hs, -hs,  hs }, {q, t2} });
    mesh.vertices.push_back({ {  hs, -hs, -hs }, {q, 1.0f} });
    mesh.vertices.push_back({ { -hs, -hs, -hs }, {q2, 1.0f} });
    mesh.vertices.push_back({ { -hs, -hs,  hs }, {q2, t2} });

    mesh.vertices.push_back({ { -hs, -hs, -hs }, {q3, t2} });
    mesh.vertices.push_back({ { -hs,  hs, -hs }, {q3, t} });
    mesh.vertices.push_back({ { -hs,  hs,  hs }, {q2, t} });
    mesh.vertices.push_back({ { -hs, -hs,  hs }, {q2, t2} });

    mesh.vertices.push_back({ { -hs, -hs, -hs }, {q3, t2} });
    mesh.vertices.push_back({ { -hs,  hs, -hs }, {q3, t} });
    mesh.vertices.push_back({ {  hs,  hs, -hs }, {1.0f, t} });
    mesh.vertices.push_back({ {  hs, -hs, -hs }, {1.0f, t2} });

    mesh.indices = {
        2, 1, 0,
        2, 0, 3,

        6, 5, 4,
        6, 4, 7,

        8, 9, 10,
        11, 8, 10,

        12, 13, 14,
        15, 12, 14,

        16, 17, 18,
        19, 16, 18,

        22, 21, 20,
        22, 20, 23
    };

    return mesh;
}
