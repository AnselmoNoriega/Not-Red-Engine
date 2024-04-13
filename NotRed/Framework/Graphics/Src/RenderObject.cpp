#include "Precompiled.h"
#include "RenderObject.h"

namespace NotRed::Graphics
{
    void RenderObject::Terminate()
    {
        mMeshBuffer.Terminate();
        mTexture.Terminate();
    }
}