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

Mesh MeshBuilder::CreateCube(float size)
{
    Mesh mesh;

    const float hs = size * 0.5f;
    const float q = 0.25f;
    const float q2 = 0.5f;
    const float q3 = 0.75f;
    const float t = 0.34f;
    const float t2 = 0.65f;

    // Left
    mesh.vertices.push_back({ { hs, -hs, -hs}, -Math::Vector3::XAxis, Math::Vector3::ZAxis, {0.0f, t2} });
    mesh.vertices.push_back({ { hs,  hs, -hs}, -Math::Vector3::XAxis, Math::Vector3::ZAxis, {0.0f, t} });
    mesh.vertices.push_back({ { hs,  hs,  hs}, -Math::Vector3::XAxis, Math::Vector3::ZAxis, {q, t} });
    mesh.vertices.push_back({ { hs, -hs,  hs}, -Math::Vector3::XAxis, Math::Vector3::ZAxis, {q, t2} });

    // Top
    mesh.vertices.push_back({ { hs,  hs,  hs}, Math::Vector3::YAxis, Math::Vector3::XAxis, {q, t} });
    mesh.vertices.push_back({ { hs,  hs, -hs}, Math::Vector3::YAxis, Math::Vector3::XAxis, {q, 0.0f} });
    mesh.vertices.push_back({ {-hs,  hs, -hs}, Math::Vector3::YAxis, Math::Vector3::XAxis, {q2, 0.0f} });
    mesh.vertices.push_back({ {-hs,  hs,  hs}, Math::Vector3::YAxis, Math::Vector3::XAxis, {q2, t} });

    // Front
    mesh.vertices.push_back({ {-hs, -hs,  hs}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {q2, t2} });
    mesh.vertices.push_back({ {-hs,  hs,  hs}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {q2, t} });
    mesh.vertices.push_back({ { hs,  hs,  hs}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {q, t} });
    mesh.vertices.push_back({ { hs, -hs,  hs}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {q, t2} });

    // Bottom				    
    mesh.vertices.push_back({ { hs, -hs,  hs}, -Math::Vector3::YAxis, -Math::Vector3::XAxis, {q, t2} });
    mesh.vertices.push_back({ { hs, -hs, -hs}, -Math::Vector3::YAxis, -Math::Vector3::XAxis, {q, 1.0f} });
    mesh.vertices.push_back({ {-hs, -hs, -hs}, -Math::Vector3::YAxis, -Math::Vector3::XAxis, {q2, 1.0f} });
    mesh.vertices.push_back({ {-hs, -hs,  hs}, -Math::Vector3::YAxis, -Math::Vector3::XAxis, {q2, t2} });

    // Right
    mesh.vertices.push_back({ {-hs, -hs, -hs}, Math::Vector3::XAxis, -Math::Vector3::ZAxis, {q3, t2} });
    mesh.vertices.push_back({ {-hs,  hs, -hs}, Math::Vector3::XAxis, -Math::Vector3::ZAxis, {q3, t} });
    mesh.vertices.push_back({ {-hs,  hs,  hs}, Math::Vector3::XAxis, -Math::Vector3::ZAxis, {q2, t} });
    mesh.vertices.push_back({ {-hs, -hs,  hs}, Math::Vector3::XAxis, -Math::Vector3::ZAxis, {q2, t2} });

    // Back
    mesh.vertices.push_back({ {-hs,-hs, -hs}, Math::Vector3::ZAxis, -Math::Vector3::XAxis, {q3, t2} });
    mesh.vertices.push_back({ {-hs, hs, -hs}, Math::Vector3::ZAxis, -Math::Vector3::XAxis, {q3, t} });
    mesh.vertices.push_back({ { hs, hs, -hs}, Math::Vector3::ZAxis, -Math::Vector3::XAxis, {1.0f, t} });
    mesh.vertices.push_back({ { hs,-hs, -hs}, Math::Vector3::ZAxis, -Math::Vector3::XAxis, {1.0f, t2} });
    mesh.indices = {
        //Left
        2, 1, 0,
        2, 0, 3,
        //Top
        6, 5, 4,
        6, 4, 7,
        //Front
        8, 9, 10,
        11, 8, 10,
        //Bottom
        12, 13, 14,
        15, 12, 14,
        //Right
        16, 17, 18,
        19, 16, 18,
        //Back
        22, 21, 20,
        22, 20, 23
    };

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

Mesh MeshBuilder::CreateVerticalPlane(uint32_t numRows, uint32_t numCols, float spacing)
{
    Mesh mesh;
    const Math::Vector3& up = Math::Vector3::YAxis;
    const Math::Vector3& right = Math::Vector3::XAxis;

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
            mesh.vertices.push_back({ {x, z, 0.0f}, up, right, {u, v} });
            x += spacing;
            u += uInc;
        }
        x += -hpw;
        z += spacing;
        u = 0.0f;
        v += (-vInc);
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

Mesh NotRed::Graphics::MeshBuilder::CreateHorizontalPlane(uint32_t numRows, uint32_t numCols, float spacing)
{
    Mesh mesh;

    const Math::Vector3 up = Math::Vector3::YAxis;
    const Math::Vector3 right = Math::Vector3::XAxis;
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
            mesh.vertices.push_back({ {x, 0.0f, z}, up, right, {u, v} });
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

MeshPC MeshBuilder::CreateCone(uint32_t numSlices, float height, float radius)
{
    MeshPC mesh;

    mesh.vertices.push_back({ {0.0f, height, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } });
    uint32_t apexIndex = 0;

    mesh.vertices.push_back({ {0.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f, 1.0f } });
    uint32_t centerIndex = 1;

    float angleStep = 2.0f * Math::PI() / static_cast<float>(numSlices);
    uint32_t baseStartIndex = 2;
    for (uint32_t i = 0; i <= numSlices; ++i)
    {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        mesh.vertices.push_back({ {x, 0.0f, z} , { 1.0f, 1.0f, 1.0f, 1.0f } });
    }

    for (uint32_t i = 0; i < numSlices; ++i)
    {
        mesh.indices.push_back(apexIndex);
        mesh.indices.push_back(baseStartIndex + i);
        mesh.indices.push_back(baseStartIndex + i + 1);
    }

    for (uint32_t i = 0; i < numSlices; ++i)
    {
        mesh.indices.push_back(centerIndex);
        mesh.indices.push_back(baseStartIndex + i + 1);
        mesh.indices.push_back(baseStartIndex + i);
    }

    return mesh;
}

MeshPC MeshBuilder::CreateInCone(uint32_t numSlices, float height, float radius)
{
    MeshPC mesh;

    mesh.vertices.push_back({ {0.0f, height, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
    uint32_t apexIndex = 0;

    mesh.vertices.push_back({ {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f} });
    uint32_t centerIndex = 1;

    float angleStep = 2.0f * Math::PI() / static_cast<float>(numSlices);
    uint32_t baseStartIndex = 2;
    for (uint32_t i = 0; i <= numSlices; ++i)
    {
        float angle = i * angleStep;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        mesh.vertices.push_back({ {x, 0.0f, z}, {1.0f, 1.0f, 1.0f, 1.0f} });
    }

    for (uint32_t i = 0; i < numSlices; ++i)
    {
        mesh.indices.push_back(baseStartIndex + i + 1);
        mesh.indices.push_back(baseStartIndex + i);
        mesh.indices.push_back(apexIndex);
    }

    for (uint32_t i = 0; i < numSlices; ++i)
    {
        mesh.indices.push_back(baseStartIndex + i);
        mesh.indices.push_back(baseStartIndex + i + 1);
        mesh.indices.push_back(centerIndex);
    }

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

            mesh.vertices.push_back({ { cos(rotation), rPos - hh,  sin(rotation) }, GetNextColor(index) });
        }
    }

    mesh.vertices.push_back({ { 0.0f, hh, 0.0f}, GetNextColor(index) });
    mesh.vertices.push_back({ { 0.0f, -hh, 0.0f}, GetNextColor(index) });

    CreatePlaneIndices(mesh.indices, rings, slices);
    CreateCapIndices(mesh.indices, slices, mesh.vertices.size() - 2, mesh.vertices.size() - 1);

    return mesh;
}

MeshPC NotRed::Graphics::MeshBuilder::CreateMergedCylinders(
    uint32_t numCylinders, 
    uint32_t slices, 
    uint32_t rings, 
    float minRadius, 
    float maxRadius,
    Math::Vector3 spawnMinBounds, 
    Math::Vector3 spawnMaxBounds
)
{
    MeshPC mergedMesh;

    const float angleStep = Math::Constants::TwoPi / static_cast<float>(slices); // Angle step per slice

    // Define a fixed tilt direction (unit vector)
    Math::Vector3 tiltDirection = Math::Normalize(Math::Vector3(1.0f, 1.0f, 0.0f)); // Example: Tilt equally along X and Y
    float infiniteExtension = 1000.0f; // Arbitrary large value to simulate infinite extension

    for (uint32_t c = 0; c < numCylinders; ++c)
    {
        // Randomize starting position within bounds
        Math::Vector3 startBase = {
            Math::Random(spawnMinBounds.x, spawnMaxBounds.x),
            Math::Random(spawnMinBounds.y, spawnMaxBounds.y),
            Math::Random(spawnMinBounds.z, spawnMaxBounds.z)
        };

        // Calculate the infinite start and end points based on tilt direction
        Math::Vector3 startOffset = startBase - (tiltDirection * infiniteExtension); // Extend infinitely backward
        Math::Vector3 endOffset = startBase + (tiltDirection * infiniteExtension);   // Extend infinitely forward

        // Randomize radii
        float startRadius = Math::Random(minRadius, maxRadius);
        float endRadius = Math::Random(startRadius, maxRadius); // Ensures the end radius is same or thicker

        // Base vertex index for this cylinder
        uint32_t baseVertexIndex = static_cast<uint32_t>(mergedMesh.vertices.size());

        // Generate vertices for the cylinder
        for (uint32_t r = 0; r <= rings; ++r)
        {
            float t = static_cast<float>(r) / static_cast<float>(rings); // Interpolation factor
            Math::Vector3 ringCenter = Math::Lerp(startOffset, endOffset, t); // Interpolated ring center
            float currentRadius = Math::Lerp(startRadius, endRadius, t); // Interpolated radius

            for (uint32_t s = 0; s <= slices; ++s)
            {
                float angle = s * angleStep;
                float x = cos(angle) * currentRadius;
                float z = sin(angle) * currentRadius;

                Math::Vector3 vertexPos = ringCenter + Math::Vector3(x, 0.0f, z);
                mergedMesh.vertices.push_back({ vertexPos, Colors::White });
            }
        }

        // Generate indices for the sides of the cylinder
        for (uint32_t r = 0; r < rings; ++r)
        {
            for (uint32_t s = 0; s < slices; ++s)
            {
                uint32_t i0 = baseVertexIndex + (r * (slices + 1)) + s;
                uint32_t i1 = baseVertexIndex + (r * (slices + 1)) + (s + 1);
                uint32_t i2 = baseVertexIndex + ((r + 1) * (slices + 1)) + s;
                uint32_t i3 = baseVertexIndex + ((r + 1) * (slices + 1)) + (s + 1);

                // First triangle
                mergedMesh.indices.push_back(i0);
                mergedMesh.indices.push_back(i2);
                mergedMesh.indices.push_back(i1);

                // Second triangle
                mergedMesh.indices.push_back(i1);
                mergedMesh.indices.push_back(i2);
                mergedMesh.indices.push_back(i3);
            }
        }
    }

    return mergedMesh;
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

Mesh NotRed::Graphics::MeshBuilder::CreateSphere(uint32_t slices, uint32_t rings, float radius)
{
    Mesh mesh;

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

            float x = radius * sin(rotation) * sin(phi);
            float y = radius * cos(phi);
            float z = radius * cos(rotation) * sin(phi);

            Math::Vector3 position = { x, y, z };
            Math::Vector3 normal = Math::Normalize(position);
            Math::Vector3 tangent = Math::Normalize({ -z, 0.0f, x });
            Math::Vector2 uvCoord = { u, v };

            mesh.vertices.push_back({
                    position,
                    normal,
                    tangent,
                    uvCoord
                });
        }
    }

    CreatePlaneIndices(mesh.indices, rings, slices);

    return mesh;
}

// duplicate skyspherepx >> skysphere
// make "Mesh" instead of "MeshPX"
// keep position as it is
// get normal and tangent from CreateSphere
// set normal to -
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
    const float q = 0.251f;
    const float q2 = 0.487;
    const float q3 = q * 3.0f;
    const float t = 0.339f;
    const float t2 = 0.662;

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

MeshPX NotRed::Graphics::MeshBuilder::CreateScreenQuad()
{
    MeshPX mesh;
    mesh.vertices.push_back({ {-1.0f, 1.0f,0.0f}, {0.0f,0.0f} });
    mesh.vertices.push_back({ { 1.0f, 1.0f,0.0f}, {1.0f,0.0f} });
    mesh.vertices.push_back({ { 1.0f,-1.0f,0.0f}, {1.0f,1.0f} });
    mesh.vertices.push_back({ {-1.0f,-1.0f,0.0f}, {0.0f,1.0f} });
    mesh.indices = { 0, 1, 3, 1, 2, 3 };

    return mesh;
}

Mesh MeshBuilder::CreateSpriteQuad(float width, float height)
{
    Mesh mesh;

    const float hw = width * 0.5f;
    const float hh = height * 0.5f;

    mesh.vertices.push_back({ {-hw, -hh, 0.0f}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {0.0f, 1.0f} });
    mesh.vertices.push_back({ {-hw,  hh, 0.0f}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {0.0f, 0.0f} });
    mesh.vertices.push_back({ { hw,  hh, 0.0f}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {1.0f, 0.0f} });
    mesh.vertices.push_back({ { hw, -hh, 0.0f}, -Math::Vector3::ZAxis, Math::Vector3::XAxis, {1.0f, 1.0f} });

    mesh.indices = {
        0, 1, 2,
        0, 2, 3
    };

    return mesh;
}