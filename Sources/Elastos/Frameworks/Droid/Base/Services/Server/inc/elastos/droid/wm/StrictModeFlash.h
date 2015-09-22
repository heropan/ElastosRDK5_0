#ifndef __ELASTOS_DROID_SERVER_WM_STRICTMODEFLASH_H__
#define __ELASTOS_DROID_SERVER_WM_STRICTMODEFLASH_H__

#include "Elastos.Droid.Server_server.h"

using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::ISurfaceSession;
using Elastos::Droid::View::ISurface;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

class StrictModeFlash : public ElRefBase
{
public:
    StrictModeFlash(
        /* [in] */ IDisplay* display,
        /* [in] */ ISurfaceSession* session);

    // Note: caller responsible for being inside
    // Surface.openTransaction() / closeTransaction()
    CARAPI_(void) SetVisibility(
        /* [in] */ Boolean on);

    CARAPI_(void) PositionSurface(
        /* [in] */ Int32 dw,
        /* [in] */ Int32 dh);

private:
    CARAPI_(void) DrawIfNeeded();

public:
    AutoPtr<ISurface> mSurface;
    Int32 mLastDW;
    Int32 mLastDH;
    Boolean mDrawNeeded;
    Int32 mThickness;

private:
    static const String TAG;
};

} // Wm
} // Server
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_SERVER_WM_STRICTMODEFLASH_H__
