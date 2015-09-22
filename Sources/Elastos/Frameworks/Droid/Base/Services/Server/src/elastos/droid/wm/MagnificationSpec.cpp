
#include "wm/MagnificationSpec.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

MagnificationSpec::MagnificationSpec()
    : mScale(1.0f)
    , mOffsetX(0)
    , mOffsetY(0)
{ }

void MagnificationSpec::Initialize(
    /* [in] */ Float scale,
    /* [in] */ Float offsetX,
    /* [in] */ Float offsetY)
{
    mScale = scale;
    mOffsetX = offsetX;
    mOffsetY = offsetY;
}

Boolean MagnificationSpec::IsNop()
{
    return mScale == 1.0f && mOffsetX == 0 && mOffsetY == 0;
}


} // Wm
} // Server
} // Droid
} // Elastos
