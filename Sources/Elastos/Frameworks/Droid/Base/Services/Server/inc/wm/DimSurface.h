#ifndef __DIMSURFACE_H__
#define __DIMSURFACE_H__

#include "Elastos.Droid.Server_server.h"

using Elastos::Droid::View::ISurfaceSession;
using Elastos::Droid::View::ISurface;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

class DimSurface : public ElRefBase
{
public:
    DimSurface(
        /* [in] */ ISurfaceSession* session,
        /* [in] */ Int32 layerStack);

    /**
     * Show the dim surface.
     */
    CARAPI_(void) Show(
        /* [in] */ Int32 dw,
        /* [in] */ Int32 dh,
        /* [in] */ Int32 layer,
        /* [in] */ Int32 color);

    CARAPI_(void) Hide();

    CARAPI_(void) Kill();

    // void printTo(String prefix, PrintWriter pw);

public:
    static const String TAG;

    AutoPtr<ISurface> mDimSurface;
    Boolean mDimShown;
    Int32 mDimColor;
    Int32 mLayer;
    Int32 mLastDimWidth;
    Int32 mLastDimHeight;
};

} // Wm
} // Server
} // Droid
} // Elastos

#endif //__DIMSURFACE_H__
