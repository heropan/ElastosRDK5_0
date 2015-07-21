
#include "ext/frameworkext.h"
#include "graphics/CSurfaceTexture.h"
#include "os/Looper.h"

using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::Looper;


namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CSurfaceTexture::EventHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    if (mHost->mOnFrameAvailableListener != NULL) {
        return mHost->mOnFrameAvailableListener->OnFrameAvailable(mHost);
    }
    return NOERROR;
}

CSurfaceTexture::CSurfaceTexture()
    : mSurfaceTexture(0)
{}

CSurfaceTexture::~CSurfaceTexture()
{
    NativeFinalize();
}

ECode CSurfaceTexture::constructor(
    /* [in] */ Int32 texName)
{
    return constructor(texName, FALSE);
}

ECode CSurfaceTexture::constructor(
    /* [in] */ Int32 texName,
    /* [in] */ Boolean allowSynchronousMode)
{
    AutoPtr<ILooper> looper;
    if ((looper = Looper::GetMyLooper()) != NULL) {
        mEventHandler = new EventHandler(looper, this);
    }
    else if ((looper = Looper::GetMainLooper()) != NULL) {
        mEventHandler = new EventHandler(looper, this);
    }
    else {
        mEventHandler = NULL;
    }
    return NativeInit(texName, allowSynchronousMode);
}

ECode CSurfaceTexture::SetOnFrameAvailableListener(
    /* [in] */ IOnFrameAvailableListener* listener)
{
    mOnFrameAvailableListener = listener;
    return NOERROR;
}

ECode CSurfaceTexture::SetDefaultBufferSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    NativeSetDefaultBufferSize(width, height);
    return NOERROR;
}

ECode CSurfaceTexture::UpdateTexImage()
{
    return NativeUpdateTexImage();
}

ECode CSurfaceTexture::DetachFromGLContext()
{
    Int32 err = NativeDetachFromGLContext();
    if (err != 0) {
        //throw new RuntimeException("Error during detachFromGLContext (see logcat for details)");
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode CSurfaceTexture::AttachToGLContext(
    /* [in] */ Int32 texName)
{
    Int32 err = NativeAttachToGLContext(texName);
    if (err != 0) {
        //throw new RuntimeException("Error during detachFromGLContext (see logcat for details)");
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode CSurfaceTexture::GetTransformMatrix(
    /* [in] */ const ArrayOf<Float>& mtx)
{
    // Note we intentionally don't check mtx for null, so this will result in a
    // NullPointerException. But it's safe because it happens before the call to native.
    if (mtx.GetLength() != 16) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    NativeGetTransformMatrix(mtx);
    return NOERROR;
}

ECode CSurfaceTexture::GetTimestamp(
    /* [out] */ Int64* timestamp)
{
    VALIDATE_NOT_NULL(timestamp);
    *timestamp = NativeGetTimestamp();
    return NOERROR;
}

ECode CSurfaceTexture::ReleaseBuffers()
{
    NativeRelease();
    return NOERROR;
}

void CSurfaceTexture::PostEventFromNative(
    /* [in] */ CSurfaceTexture* selfRef)
{
    // WeakReference weakSelf = (WeakReference)selfRef;
    if (selfRef == NULL) {
        return;
    }

    if (selfRef->mEventHandler != NULL) {
        Boolean result;
        selfRef->mEventHandler->SendEmptyMessage(0, &result);
    }
}

ECode CSurfaceTexture::GetSurfaceTexture(
    /* [out] */ Int32* texture)
{
    *texture = mSurfaceTexture;
    return NOERROR;
}

// ----------------------------------------------------------------------------

static void SurfaceTexture_setSurfaceTexture(
    /* [in] */ CSurfaceTexture* thiz,
    /* [in] */ const android::sp<android::SurfaceTexture>& surfaceTexture)
{
    android::SurfaceTexture* const p = (android::SurfaceTexture*)thiz->mSurfaceTexture;
    if (surfaceTexture.get()) {
        surfaceTexture->incStrong(thiz);
    }
    if (p) {
        p->decStrong(thiz);
    }
    thiz->mSurfaceTexture = (Int32)surfaceTexture.get();
}

android::sp<android::SurfaceTexture> SurfaceTexture_getSurfaceTexture(
    /* [in] */ CSurfaceTexture* thiz)
{
    android::sp<android::SurfaceTexture> surfaceTexture((android::SurfaceTexture*)thiz->mSurfaceTexture);
    return surfaceTexture;
}

// ----------------------------------------------------------------------------

class DroidSurfaceTextureContext : public android::SurfaceTexture::FrameAvailableListener
{
public:
    DroidSurfaceTextureContext(CSurfaceTexture* weakThiz);
    virtual ~DroidSurfaceTextureContext();
    virtual void onFrameAvailable();

private:
    //TODO: should be weak reference
    CSurfaceTexture* mWeakThiz;
};

DroidSurfaceTextureContext::DroidSurfaceTextureContext(
        CSurfaceTexture* weakThiz) : mWeakThiz(weakThiz)
{}

DroidSurfaceTextureContext::~DroidSurfaceTextureContext()
{
    mWeakThiz = NULL;
}

void DroidSurfaceTextureContext::onFrameAvailable()
{
    CSurfaceTexture::PostEventFromNative(mWeakThiz);
}

ECode CSurfaceTexture::NativeInit(
    /* [in] */ Int32 texName,
    /* [in] */ Boolean allowSynchronousMode)
{
    android::sp<android::SurfaceTexture> surfaceTexture(new android::SurfaceTexture(texName, allowSynchronousMode));
    if (surfaceTexture == 0) {
        // jniThrowException(env, OutOfResourcesException,
        //         "Unable to create native SurfaceTexture");
        return E_OUT_OF_RESOURCES_EXCEPTION;
    }
    SurfaceTexture_setSurfaceTexture(this, surfaceTexture);

    android::sp<DroidSurfaceTextureContext> ctx(new DroidSurfaceTextureContext(this));
    surfaceTexture->setFrameAvailableListener(ctx);

    return NOERROR;
}

void CSurfaceTexture::NativeFinalize()
{
    android::sp<android::SurfaceTexture> surfaceTexture(SurfaceTexture_getSurfaceTexture(this));
    surfaceTexture->setFrameAvailableListener(0);
    SurfaceTexture_setSurfaceTexture(this, 0);
}

void CSurfaceTexture::NativeGetTransformMatrix(
    /* [in] */ const ArrayOf<Float>& mtx)
{
    android::sp<android::SurfaceTexture> surfaceTexture(SurfaceTexture_getSurfaceTexture(this));
    surfaceTexture->getTransformMatrix(mtx.GetPayload());
}

Int64 CSurfaceTexture::NativeGetTimestamp()
{
    android::sp<android::SurfaceTexture> surfaceTexture(SurfaceTexture_getSurfaceTexture(this));
    return surfaceTexture->getTimestamp();
}

void CSurfaceTexture::NativeSetDefaultBufferSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    android::sp<android::SurfaceTexture> surfaceTexture(SurfaceTexture_getSurfaceTexture(this));
    surfaceTexture->setDefaultBufferSize(width, height);
}

ECode CSurfaceTexture::NativeUpdateTexImage()
{
    android::sp<android::SurfaceTexture> surfaceTexture(SurfaceTexture_getSurfaceTexture(this));
    android::status_t err = surfaceTexture->updateTexImage();

    if (err == android::INVALID_OPERATION) {
        //jniThrowException(env, IllegalStateException, "Unable to update texture contents (see logcat for details)");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    else if (err < 0) {
        //jniThrowRuntimeException(env, "Error during updateTexImage (see logcat for details)");
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

Int32 CSurfaceTexture::NativeDetachFromGLContext()
{
    android::sp<android::SurfaceTexture> surfaceTexture(SurfaceTexture_getSurfaceTexture(this));
    return surfaceTexture->detachFromContext();
}

Int32 CSurfaceTexture::NativeAttachToGLContext(
    /* [in] */ Int32 texName)
{
    android::sp<android::SurfaceTexture> surfaceTexture(SurfaceTexture_getSurfaceTexture(this));
    return surfaceTexture->attachToContext((GLuint)texName);
}

void CSurfaceTexture::NativeRelease()
{
    android::sp<android::SurfaceTexture> surfaceTexture(SurfaceTexture_getSurfaceTexture(this));
    surfaceTexture->abandon();
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
