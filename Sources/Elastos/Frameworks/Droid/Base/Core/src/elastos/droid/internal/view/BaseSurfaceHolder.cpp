
#include "elastos/droid/internal/view/BaseSurfaceHolder.h"
#ifdef DROID_CORE
#include "elastos/droid/internal/view/CSurface.h"
#include "elastos/droid/graphics/CRect.h"
#endif
#include "elastos/droid/os/SystemClock.h"
#include <elastos/utility/etl/Algorithm.h>
#include <elastos/utility/logging/Logger.h>
#include <unistd.h>

using Elastos::Droid::Graphics::IPixelFormat;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Os::SystemClock;
using Elastos::Utility::Etl::Vector;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {

const Boolean BaseSurfaceHolder::DEBUG = FALSE;
const char* BaseSurfaceHolder::TAG = "BaseSurfaceHolder";

CAR_INTERFACE_IMPL(BaseSurfaceHolder, ISurfaceHolder);

BaseSurfaceHolder::BaseSurfaceHolder() :
    mHaveGottenCallbacks(FALSE),
    mRequestedWidth(-1),
    mRequestedHeight(-1),
    mRequestedFormat(IPixelFormat::OPAQUE),
    mRequestedType(-1),
    mLastLockTime(0),
    mType(-1)
{
    ASSERT_SUCCEEDED(CSurface::New((ISurface**)&mSurface));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mSurfaceFrame));
}

Int32 BaseSurfaceHolder::GetRequestedWidth()
{
    return mRequestedWidth;
}

Int32 BaseSurfaceHolder::GetRequestedHeight()
{
    return mRequestedHeight;
}

Int32 BaseSurfaceHolder::GetRequestedFormat()
{
    return mRequestedFormat;
}

Int32 BaseSurfaceHolder::GetRequestedType()
{
    return mRequestedType;
}

ECode BaseSurfaceHolder::AddCallback(
    /* [in] */ ISurfaceHolderCallback* cback)
{
    AutoLock lock(mCallbackLock);

    // This is a linear search, but in practice we'll
    // have only a couple callbacks, so it doesn't matter.
    //
    AutoPtr<ISurfaceHolderCallback> temp = cback;
    List<AutoPtr<ISurfaceHolderCallback> >::Iterator iter =
        Find(mCallbacks.Begin(), mCallbacks.End(), temp);
    if (iter == mCallbacks.End()) {
        mCallbacks.PushBack(cback);
    }

    return NOERROR;
}

ECode BaseSurfaceHolder::RemoveCallback(
    /* [in] */ ISurfaceHolderCallback* cback)
{
    AutoLock lock(mCallbackLock);

    AutoPtr<ISurfaceHolderCallback> temp = cback;
    List<AutoPtr<ISurfaceHolderCallback> >::Iterator iter =
        Find(mCallbacks.Begin(), mCallbacks.End(), temp);
    if (iter != mCallbacks.End()) {
        mCallbacks.Erase(iter);
    }

    return NOERROR;
}

Vector<AutoPtr<ISurfaceHolderCallback> >& BaseSurfaceHolder::GetCallbacks()
{
    if (mHaveGottenCallbacks) {
        return mGottenCallbacks;
    }

    AutoLock lock(mCallbackLock);

    mGottenCallbacks.Clear();
    if (mCallbacks.IsEmpty() == FALSE) {
        //List<AutoPtr<ISurfaceHolderCallback> >::Iterator iter = mCallbacks.Begin();
        //for (; iter!=mCallbacks.End(); ++iter) {
        //    mGottenCallbacks.PushBack(*iter);
        //}
        mGottenCallbacks.Assign<List<AutoPtr<ISurfaceHolderCallback> >::Iterator>(
            mCallbacks.Begin(), mCallbacks.End());
    }
    mHaveGottenCallbacks = TRUE;

    return mGottenCallbacks;
}

void BaseSurfaceHolder::UngetCallbacks()
{
    mHaveGottenCallbacks = FALSE;
}

ECode BaseSurfaceHolder::SetFixedSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (mRequestedWidth != width || mRequestedHeight != height) {
        mRequestedWidth = width;
        mRequestedHeight = height;
        OnRelayoutContainer();
    }

    return NOERROR;
}

ECode BaseSurfaceHolder::SetSizeFromLayout()
{
    if (mRequestedWidth != -1 || mRequestedHeight != -1) {
        mRequestedWidth = mRequestedHeight = -1;
        OnRelayoutContainer();
    }

    return NOERROR;
}

ECode BaseSurfaceHolder::SetFormat(
    /* [in] */ Int32 format)
{
    if (mRequestedFormat != format) {
        mRequestedFormat = format;
        OnUpdateSurface();
    }

    return NOERROR;
}

ECode BaseSurfaceHolder::SetType(
    /* [in] */ Int32 type)
{
    switch (type) {
        case SURFACE_TYPE_HARDWARE:
        case SURFACE_TYPE_GPU:
            // these are deprecated, treat as "NORMAL"
            type = SURFACE_TYPE_NORMAL;
            break;
        default:
            break;
    }

    switch (type) {
        case SURFACE_TYPE_NORMAL:
        case SURFACE_TYPE_PUSH_BUFFERS:
            if (mRequestedType != type) {
                mRequestedType = type;
                OnUpdateSurface();
            }
            break;
        default:
            break;
    }

    return NOERROR;
}

ECode BaseSurfaceHolder::LockCanvas(
    /* [out] */ ICanvas** canvas)
{
    if (canvas == NULL) {
        return E_INVALID_ARGUMENT;
    }

    InternalLockCanvas(NULL, canvas);

    return NOERROR;
}

ECode BaseSurfaceHolder::LockCanvas(
    /* [in] */ IRect* dirty,
    /* [out] */ ICanvas** canvas)
{
    if (canvas == NULL) {
        return E_INVALID_ARGUMENT;
    }

    InternalLockCanvas(dirty, canvas);

    return NOERROR;
}

ECode BaseSurfaceHolder::UnlockCanvasAndPost(
    /* [in] */ ICanvas* canvas)
{
    mSurface->UnlockCanvasAndPost(canvas);
    mSurfaceLock.Unlock();

    return NOERROR;
}

ECode BaseSurfaceHolder::GetSurface(
    /* [out] */ ISurface** surface)
{
    if (surface == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *surface = mSurface;
    REFCOUNT_ADD(*surface);

    return NOERROR;
}

ECode BaseSurfaceHolder::GetSurfaceFrame(
    /* [out] */ IRect** rect)
{
    if (rect == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *rect = mSurfaceFrame;
    REFCOUNT_ADD(*rect);

    return NOERROR;
}

ECode BaseSurfaceHolder::InternalLockCanvas(
    /* [in] */ IRect* dirty,
    /* [out] */ ICanvas** canvas)
{
    VALIDATE_NOT_NULL(canvas);

    if (mType == SURFACE_TYPE_PUSH_BUFFERS) {
        //throw new BadSurfaceTypeException(
        //    "Surface type is SURFACE_TYPE_PUSH_BUFFERS");
        Logger::E(TAG, "Surface type is SURFACE_TYPE_PUSH_BUFFERS");
        return E_BAD_SURFACE_TYPE_EXCEPTION;
    }

    mSurfaceLock.Lock();

    if (DEBUG) {
        Logger::D(TAG, "Locking canvas..,");
    }

    if (OnAllowLockCanvas()) {
        if (dirty == NULL) {
            if (mTmpDirty == NULL) {
                CRect::New((IRect**)&mTmpDirty);
            }

            mTmpDirty->Set(mSurfaceFrame);
            dirty = mTmpDirty;
        }

        ECode ec = mSurface->LockCanvas(dirty, canvas);
        if (FAILED(ec)) {
            Logger::E(TAG, "Exception locking surface");
        }
    }

    if (DEBUG) {
        Logger::D(TAG, "Returned canvas: 0x%08x", Int32(*canvas));
    }

    if (*canvas != NULL) {
        mLastLockTime = SystemClock::GetUptimeMillis();
        return NOERROR;
    }

    // If the Surface is not ready to be drawn, then return NULL,
    // but throttle calls to this function so it isn't called more
    // than every 100ms.
    Int64 now = SystemClock::GetUptimeMillis();
    Int64 nextTime = mLastLockTime + 100;
    if (nextTime > now) {
        //try {
        //    Thread.sleep(nextTime-now);
        //} catch (InterruptedException e) {
        //}
        usleep(nextTime - now);
        now = SystemClock::GetUptimeMillis();
    }
    mLastLockTime = now;
    mSurfaceLock.Unlock();

    return NOERROR;
}

ECode BaseSurfaceHolder::SetSurfaceFrameSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return mSurfaceFrame->Set(0, 0, width, height);
}

} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos
