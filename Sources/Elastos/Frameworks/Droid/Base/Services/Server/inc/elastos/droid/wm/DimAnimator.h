#ifndef __ELASTOS_DROID_SERVER_WM_DIMANIMATOR_H__
#define __ELASTOS_DROID_SERVER_WM_DIMANIMATOR_H__

#include "elastos/droid/ext/frameworkext.h"
#include "Elastos.Droid.Server_server.h"
#include <elastos/utility/etl/List.h>

using Elastos::Droid::View::ISurfaceSession;
using Elastos::Droid::View::ISurface;
using Elastos::Droid::Content::Res::IResources;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

class WindowStateAnimator;

/**
 * DimAnimator class that controls the dim animation. This holds the surface and
 * all state used for dim animation.
 */
class DimAnimator : public ElRefBase
{
public:
    class Parameters : public ElRefBase
    {
    public:
        Parameters(
            /* [in] */ WindowStateAnimator* dimWinAnimator,
            /* [in] */ Int32 dimWidth,
            /* [in] */ Int32 dimHeight,
            /* [in] */ Float dimTarget);

        Parameters(
            /* [in] */ Parameters* o);

        // void printTo(String prefix, PrintWriter pw);

    public:
        AutoPtr<WindowStateAnimator> mDimWinAnimator;
        Int32 mDimWidth;
        Int32 mDimHeight;
        Float mDimTarget;
    };

public:
    DimAnimator(
        /* [in] */ ISurfaceSession* session,
        /* [in] */ Int32 layerStack);

    /**
     * Set's the dim surface's layer and update dim parameters that will be used in
     * {@link #updateSurface} after all windows are examined.
     */
    CARAPI_(void) UpdateParameters(
        /* [in] */ IResources* res,
        /* [in] */ Parameters* params,
        /* [in] */ Int64 currentTime);

    /**
     * Updating the surface's alpha. Returns true if the animation continues, or returns
     * false when the animation is finished and the dim surface is hidden.
     */
    CARAPI_(Boolean) UpdateSurface(
        /* [in] */ Boolean dimming,
        /* [in] */ Int64 currentTime,
        /* [in] */ Boolean displayFrozen);

    CARAPI_(void) Kill();

    // void printTo(String prefix, PrintWriter pw);

public:
    static const String TAG;

    AutoPtr<ISurface> mDimSurface;
    Boolean mDimShown;
    Float mDimCurrentAlpha;
    Float mDimTargetAlpha;
    Float mDimDeltaPerMs;
    Int64 mLastDimAnimTime;

    Int32 mLastDimWidth;
    Int32 mLastDimHeight;
};

} // Wm
} // Server
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_SERVER_WM_DIMANIMATOR_H__
