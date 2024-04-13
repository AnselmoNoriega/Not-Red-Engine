#include "Precompiled.h"
#include "RenderObject.h"

namespace NotRed::Graphics
{
    void RenderObject::Terminate()
    {
        meshBuffer.Terminate();
    }
}