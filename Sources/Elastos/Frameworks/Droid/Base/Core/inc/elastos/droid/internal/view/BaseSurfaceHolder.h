
#ifndef __ELASTOS_DROID_VIEW_BASESURFACEHOLDER_H__
#define __ELASTOS_DROID_VIEW_BASESURFACEHOLDER_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/List.h>

using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::View::ISurface;
using Elastos::Droid::View::ISurfaceHolder;
using Elastos::Utility::Concurrent::Locks::IReentrantLock;
using Elastos::Utility::Etl::List;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {

class BaseSurfaceHolder
    : public Object
    , public ISurfaceHolder
{
public:
    BaseSurfaceHolder();

    CAR_INTERFACE_DECL();

    virtual CARAPI OnUpdateSurface() = 0;

    virtual CARAPI_(void) OnRelayoutContainer() = 0;

    virtual CARAPI_(Boolean) OnAllowLockCanvas() = 0;

    CARAPI_(Int32) GetRequestedWidth();

    CARAPI_(Int32) GetRequestedHeight();

    CARAPI_(Int32) GetRequestedFormat();

    CARAPI_(Int32) GetRequestedType();

    virtual CARAPI AddCallback(
        /* [in] */ ISurfaceHolderCallback* cback);

    virtual CARAPI RemoveCallback(
        /* [in] */ ISurfaceHolderCallback* cback);

    CARAPI_(Vector<AutoPtr<ISurfaceHolderCallback> >&) GetCallbacks();

    CARAPI_(void) UngetCallbacks();

    virtual CARAPI SetFixedSize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    virtual CARAPI SetSizeFromLayout();

    virtual CARAPI SetFormat(
        /* [in] */ Int32 format);

    virtual CARAPI SetType(
        /* [in] */ Int32 type);

    virtual CARAPI LockCanvas(
        /* [out] */ ICanvas** canvas);

    virtual CARAPI LockCanvas(
        /* [in] */ IRect* dirty,
        /* [out] */ ICanvas** canvas);

    virtual CARAPI UnlockCanvasAndPost(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI GetSurface(
        /* [out] */ ISurface** surface);

    virtual CARAPI GetSurfaceFrame(
        /* [out] */ IRect** rect);

    virtual CARAPI SetSurfaceFrameSize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

private:
    CARAPI InternalLockCanvas(
        /* [in] */ IRect* dirty,
        /* [out] */ ICanvas** canvas);

public:
    List<AutoPtr<ISurfaceHolderCallback> > mCallbacks;
    AutoPtr<IReentrantLock> mSurfaceLock;
    AutoPtr<ISurface> mSurface;

protected:
    static const Boolean DEBUG;
    AutoPtr<ArrayOf<ISurfaceHolderCallback*> > mGottenCallbacks;
    Boolean mHaveGottenCallbacks;
    Int32 mRequestedWidth;
    Int32 mRequestedHeight;
    /** @hide */
    Int32 mRequestedFormat;
    Int32 mRequestedType;
    Int64 mLastLockTime;
    Int32 mType;
    AutoPtr<IRect> mSurfaceFrame;
    AutoPtr<IRect> mTmpDirty;

private:
    static const String TAG;
    Object mCallbackLock;
};

} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_BASESURFACEHOLDER_H__
