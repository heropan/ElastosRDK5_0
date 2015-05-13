#ifndef __BLACKFRAME_H__
#define __BLACKFRAME_H__

#include "Elastos.Droid.Server_server.h"

using Elastos::Droid::View::ISurfaceSession;
using Elastos::Droid::View::ISurface;
using Elastos::Droid::Graphics::IMatrix;
using Elastos::Droid::Graphics::IRect;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

/**
 * Four black surfaces put together to make a black frame.
 */
class BlackFrame : public ElRefBase
{
public:
    class BlackSurface : public ElRefBase
    {
    public:
        BlackSurface(
            /* [in] */ ISurfaceSession* session,
            /* [in] */ Int32 layer,
            /* [in] */ Int32 l,
            /* [in] */ Int32 t,
            /* [in] */ Int32 r,
            /* [in] */ Int32 b,
            /* [in] */ Int32 layerStack,
            /* [in] */ BlackFrame* host);

        CARAPI_(void) SetMatrix(
            /* [in] */ IMatrix* matrix);

        CARAPI_(void) ClearMatrix();

    public:
        Int32 mLeft;
        Int32 mTop;
        Int32 mLayer;
        AutoPtr<ISurface> mSurface;

    private:
        AutoPtr<BlackFrame> mHost;
    };

public:
    BlackFrame(
        /* [in] */ ISurfaceSession* session,
        /* [in] */ IRect* outer,
        /* [in] */ IRect* inner,
        /* [in] */ Int32 layer,
        /* [in] */ Int32 layerStack);

    // void printTo(String prefix, PrintWriter pw);

    CARAPI_(void) Kill();

    CARAPI_(void) Hide();

    CARAPI_(void) SetMatrix(
        /* [in] */ IMatrix* matrix);

    CARAPI_(void) ClearMatrix();

public:
    AutoPtr<IRect> mOuterRect;
    AutoPtr<IRect> mInnerRect;
    AutoPtr<IMatrix> mTmpMatrix;
    AutoPtr< ArrayOf<Float> > mTmpFloats;
    AutoPtr< ArrayOf<BlackSurface*> > mBlackSurfaces;
};

} // Wm
} // Server
} // Droid
} // Elastos

#endif //__BLACKFRAME_H__
