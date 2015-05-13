#ifndef __MAGNIFICATIONSPEC_H__
#define __MAGNIFICATIONSPEC_H__

#include <elrefbase.h>

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

class MagnificationSpec : public ElRefBase
{
public:
    MagnificationSpec();

    // Note: caller responsible for being inside
    // Surface.openTransaction() / closeTransaction()
    CARAPI_(void) Initialize(
        /* [in] */ Float scale,
        /* [in] */ Float offsetX,
        /* [in] */ Float offsetY);

    CARAPI_(Boolean) IsNop();

public:
    Float mScale;
    Float mOffsetX;
    Float mOffsetY;
};

} // Wm
} // Server
} // Droid
} // Elastos

#endif //__MAGNIFICATIONSPEC_H__
