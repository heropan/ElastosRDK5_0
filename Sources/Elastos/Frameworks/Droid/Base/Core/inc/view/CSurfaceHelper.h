
#ifndef __ELASTOS_DROID_VIEW_CSURFACEHELPER_H__
#define __ELASTOS_DROID_VIEW_CSURFACEHELPER_H__

#include "_CSurfaceHelper.h"

using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Os::IBinder;

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CSurfaceHelper)
{
public:
    CARAPI Screenshot(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [out] */ IBitmap** bitmap);

    CARAPI Screenshot(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 minLayer,
        /* [in] */ Int32 maxLayer,
        /* [out] */ IBitmap** bitmap);

    /*
     * set surface parameters.
     * needs to be inside open/closeTransaction block
     */

    /** start a transaction @hide */
    CARAPI OpenTransaction();

    /** end a transaction @hide */
    CARAPI CloseTransaction();

    /** flag the transaction as an animation @hide */
    CARAPI SetAnimationTransaction();

    /** @hide */
    CARAPI GetBuiltInDisplay(
        /* [in] */ Int32 builtInDisplayId,
        /* [out] */ IBinder** binder);

    /** @hide */
    CARAPI CreateDisplay(
        /* [in] */ const String& name,
        /* [in] */ Boolean secure,
        /* [out] */ IBinder** binder);

    /** @hide */
    CARAPI SetDisplaySurface(
        /* [in] */ IBinder* displayToken,
        /* [in] */ ISurface* surface);

    /** @hide */
    CARAPI SetDisplayLayerStack(
        /* [in] */ IBinder* displayToken,
        /* [in] */ Int32 layerStack);

    /** @hide */
    CARAPI SetDisplayProjection(
        /* [in] */ IBinder* displayToken,
        /* [in] */ Int32 orientation,
        /* [in] */ IRect* layerStackRect,
        /* [in] */ IRect* displayRect);

    /** @hide */
    CARAPI GetDisplayInfo(
        /* [in] */ IBinder* displayToken,
        /* [in] */ IPhysicalDisplayInfo* outInfo,
        /* [out] */ Boolean* result);

    /** @hide */
    CARAPI BlankDisplay(
        /* [in] */ IBinder* displayToken);

    /** @hide */
    CARAPI UnblankDisplay(
        /* [in] */ IBinder* displayToken);
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_CSURFACEHELPER_H__
