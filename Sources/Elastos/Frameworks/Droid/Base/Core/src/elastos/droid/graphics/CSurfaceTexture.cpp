
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/graphics/CSurfaceTexture.h"
#include "elastos/droid/os/Looper.h"

using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::Looper;


namespace Elastos {
namespace Droid {
namespace Graphics {

CAR_OBJECT_IMPL(CSurfaceTexture);
CAR_INTERFACE_IMPL(CSurfaceTexture, Object, ISurfaceTexture);
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
    /* [in] */ Boolean singleBufferMode)
{
    assert(0 && "TODO");
    // mCreatorLooper = Looper.myLooper();
    // nativeInit(false, texName, singleBufferMode, new WeakReference<SurfaceTexture>(this));
    return NOERROR;
}

ECode CSurfaceTexture::constructor(
    /* [in] */ Boolean singleBufferMode)
{
    assert(0 && "TODO");
    // mCreatorLooper = Looper.myLooper();
    // nativeInit(true, 0, singleBufferMode, new WeakReference<SurfaceTexture>(this));
    return NOERROR;
}

ECode CSurfaceTexture::SetOnFrameAvailableListener(
    /* [in] */ IOnFrameAvailableListener* listener)
{
    return SetOnFrameAvailableListener(listener, NULL);
}

ECode CSurfaceTexture::SetOnFrameAvailableListener(
    /* [in] */ /*@Nullable*/ IOnFrameAvailableListener* listener,
    /* [in] */ /*@Nullable*/ IHandler* handler)
{
    assert(0 && "TODO");
    // if (listener != null) {
    //     // Although we claim the thread is arbitrary, earlier implementation would
    //     // prefer to send the callback on the creating looper or the main looper
    //     // so we preserve this behavior here.
    //     Looper looper = handler != null ? handler.getLooper() :
    //             mCreatorLooper != null ? mCreatorLooper : Looper.getMainLooper();
    //     mOnFrameAvailableHandler = new Handler(looper, null, true /*async*/) {
    //         @Override
    //         public void handleMessage(Message msg) {
    //             listener.onFrameAvailable(SurfaceTexture.this);
    //         }
    //     };
    // } else {
    //     mOnFrameAvailableHandler = null;
    // }
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

ECode CSurfaceTexture::ReleaseTexImage()
{
    return NativeReleaseTexImage();
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
        //throw new RuntimeException("Error during attachToGLContext (see logcat for details)");
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode CSurfaceTexture::GetTransformMatrix(
    /* [in] */ ArrayOf<Float>* mtx)
{
    // Note we intentionally don't check mtx for null, so this will result in a
    // NullPointerException. But it's safe because it happens before the call to native.
    if (mtx->GetLength() != 16) {
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
    /* [in] */ IWeakReference/*<SurfaceTexture>*/* weakSelf)
{
    assert(0 && "TODO");
    // SurfaceTexture st = weakSelf.get();
    // if (st != null) {
    //     Handler handler = st.mOnFrameAvailableHandler;
    //     if (handler != null) {
    //         handler.sendEmptyMessage(0);
    //     }
    // }
}

ECode CSurfaceTexture::GetSurfaceTexture(
    /* [out] */ Int32* texture)
{
    *texture = mSurfaceTexture;
    return NOERROR;
}

// ----------------------------------------------------------------------------

// static void SurfaceTexture_setSurfaceTexture(
//     /* [in] */ CSurfaceTexture* thiz,
//     /* [in] */ const android::sp<GLConsumer>& surfaceTexture)
// {
//     GLConsumer* const p = (GLConsumer*)thiz->mSurfaceTexture;
//     if (surfaceTexture.get()) {
//         surfaceTexture->incStrong(thiz);
//     }
//     if (p) {
//         p->decStrong(thiz);
//     }
//     thiz->mSurfaceTexture = (Int32)surfaceTexture.get();
// }

// static void SurfaceTexture_setProducer(
//     /* [in] */ CSurfaceTexture* thiz,
//     /* [in] */ const android::sp<IGraphicBufferProducer>& producer)
// {
//     assert(0 && "TODO");
//     // IGraphicBufferProducer* const p =
//     //     (IGraphicBufferProducer*)env->GetLongField(thiz, fields.producer);
//     // if (producer.get()) {
//     //     producer->incStrong((void*)SurfaceTexture_setProducer);
//     // }
//     // if (p) {
//     //     p->decStrong((void*)SurfaceTexture_setProducer);
//     // }
//     // env->SetLongField(thiz, fields.producer, (jlong)producer.get());
// }

// static void SurfaceTexture_setFrameAvailableListener(
//     /* [in] */ CSurfaceTexture* thiz,
//     /* [in] */ android::sp<GLConsumer::FrameAvailableListener> listener)
// {
//     assert(0 && "TODO");
//     // GLConsumer::FrameAvailableListener* const p =
//     //     (GLConsumer::FrameAvailableListener*)
//     //         env->GetLongField(thiz, fields.frameAvailableListener);
//     // if (listener.get()) {
//     //     listener->incStrong((void*)SurfaceTexture_setSurfaceTexture);
//     // }
//     // if (p) {
//     //     p->decStrong((void*)SurfaceTexture_setSurfaceTexture);
//     // }
//     // env->SetLongField(thiz, fields.frameAvailableListener, (jlong)listener.get());
// }

// android::sp<GLConsumer> SurfaceTexture_getSurfaceTexture(
//     /* [in] */ CSurfaceTexture* thiz)
// {
//     android::sp<android::SurfaceTexture> surfaceTexture((android::SurfaceTexture*)thiz->mSurfaceTexture);
//     return surfaceTexture;
// }

// android::sp<GLConsumer> SurfaceTexture_getSurfaceTexture(
//     /* [in] */ CSurfaceTexture* thiz)
// {
//     return (GLConsumer*)thiz->mSurfaceTexture;
// }

// android::sp<IGraphicBufferProducer> SurfaceTexture_getProducer(
//     /* [in] */ CSurfaceTexture* thiz)
// {
//     assert(0 && "TODO");
//     // return (IGraphicBufferProducer*)env->GetLongField(thiz, fields.producer);
//     return NULL;
// }

// android::sp<ANativeWindow> android_SurfaceTexture_getNativeWindow(
//     /* [in] */ CSurfaceTexture* thiz)
// {
//     assert(0 && "TODO");
//     // android::sp<GLConsumer> surfaceTexture(SurfaceTexture_getSurfaceTexture(env, thiz));
//     // android::sp<IGraphicBufferProducer> producer(SurfaceTexture_getProducer(env, thiz));
//     // android::sp<Surface> surfaceTextureClient(surfaceTexture != NULL ? new Surface(producer) : NULL);
//     // return surfaceTextureClient;
//     return NULL;
// }

// ----------------------------------------------------------------------------

// class DroidSurfaceTextureContext : public android::SurfaceTexture::FrameAvailableListener
// {
// public:
//     DroidSurfaceTextureContext(CSurfaceTexture* weakThiz);
//     virtual ~DroidSurfaceTextureContext();
//     virtual void onFrameAvailable();

// private:
//     //TODO: should be weak reference
//     CSurfaceTexture* mWeakThiz;
// };

// DroidSurfaceTextureContext::DroidSurfaceTextureContext(
//         CSurfaceTexture* weakThiz) : mWeakThiz(weakThiz)
// {}

// DroidSurfaceTextureContext::~DroidSurfaceTextureContext()
// {
//     mWeakThiz = NULL;
// }

// void DroidSurfaceTextureContext::onFrameAvailable()
// {
//     CSurfaceTexture::PostEventFromNative(mWeakThiz);
// }

ECode CSurfaceTexture::NativeInit(
    /* [in] */ Boolean isDetached,
    /* [in] */ Int32 texName,
    /* [in] */ Boolean singleBufferMode,
    /* [in] */ IWeakReference/*<SurfaceTexture>*/* weakSelf) /*throws Surface.OutOfResourcesException*/
{
    assert(0 && "TODO");
    // android::sp<IGraphicBufferProducer> producer;
    // android::sp<IGraphicBufferConsumer> consumer;
    // BufferQueue::createBufferQueue(&producer, &consumer);

    // if (singleBufferMode) {
    //     consumer->disableAsyncBuffer();
    //     consumer->setDefaultMaxBufferCount(1);
    // }

    // android::sp<GLConsumer> surfaceTexture;
    // if (isDetached) {
    //     surfaceTexture = new GLConsumer(consumer, GL_TEXTURE_EXTERNAL_OES,
    //             true, true);
    // } else {
    //     surfaceTexture = new GLConsumer(consumer, texName,
    //             GL_TEXTURE_EXTERNAL_OES, true, true);
    // }

    // if (surfaceTexture == 0) {
    //     jniThrowException(env, OutOfResourcesException,
    //             "Unable to create native SurfaceTexture");
    //     return;
    // }
    // SurfaceTexture_setSurfaceTexture(env, thiz, surfaceTexture);
    // SurfaceTexture_setProducer(env, thiz, producer);

    // jclass clazz = env->GetObjectClass(thiz);
    // if (clazz == NULL) {
    //     jniThrowRuntimeException(env,
    //             "Can't find android/graphics/SurfaceTexture");
    //     return;
    // }

    // android::sp<JNISurfaceTextureContext> ctx(new JNISurfaceTextureContext(env, weakThiz,
    //         clazz));
    // surfaceTexture->setFrameAvailableListener(ctx);
    // SurfaceTexture_setFrameAvailableListener(env, thiz, ctx);
    return NOERROR;
}

ECode CSurfaceTexture::NativeReleaseTexImage()
{
    assert(0 && "TODO");
    // android::sp<GLConsumer> surfaceTexture(SurfaceTexture_getSurfaceTexture(env, thiz));
    // status_t err = surfaceTexture->releaseTexImage();
    // if (err == INVALID_OPERATION) {
    //     jniThrowException(env, IllegalStateException, "Unable to release texture contents (see "
    //             "logcat for details)");
    // } else if (err < 0) {
    //     jniThrowRuntimeException(env, "Error during updateTexImage (see logcat for details)");
    // }
    return NOERROR;
}

void CSurfaceTexture::NativeFinalize()
{
    assert(0 && "TODO");
    // android::sp<GLConsumer> surfaceTexture(SurfaceTexture_getSurfaceTexture(this));
    // surfaceTexture->setFrameAvailableListener(0);
    // SurfaceTexture_setFrameAvailableListener(env, thiz, 0);
    // SurfaceTexture_setSurfaceTexture(env, thiz, 0);
    // SurfaceTexture_setProducer(env, thiz, 0);
}

void CSurfaceTexture::NativeGetTransformMatrix(
    /* [in] */ ArrayOf<Float>* mtx)
{
    assert(0 && "TODO");
    // android::sp<GLConsumer> surfaceTexture(SurfaceTexture_getSurfaceTexture(this));
    // surfaceTexture->getTransformMatrix(mtx.GetPayload());
}

Int64 CSurfaceTexture::NativeGetTimestamp()
{
    assert(0 && "TODO");
    // android::sp<GLConsumer> surfaceTexture(SurfaceTexture_getSurfaceTexture(this));
    // return surfaceTexture->getTimestamp();
}

void CSurfaceTexture::NativeSetDefaultBufferSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    assert(0 && "TODO");
    // android::sp<GLConsumer> surfaceTexture(SurfaceTexture_getSurfaceTexture(this));
    // surfaceTexture->setDefaultBufferSize(width, height);
}

ECode CSurfaceTexture::NativeUpdateTexImage()
{
    assert(0 && "TODO");
    // android::sp<GLConsumer> surfaceTexture(SurfaceTexture_getSurfaceTexture(this));
    // status_t err = surfaceTexture->updateTexImage();
    // if (err == android::INVALID_OPERATION) {
    //     // jniThrowException(env, IllegalStateException, "Unable to update texture contents (see "
    //     //         "logcat for details)");
    //     return E_ILLEGAL_STATE_EXCEPTION;
    // } else if (err < 0) {
    //     // jniThrowRuntimeException(env, "Error during updateTexImage (see logcat for details)");
    //     return E_RUNTIME_EXCEPTION;
    // }
    // return NOERROR;
}

Int32 CSurfaceTexture::NativeDetachFromGLContext()
{
    assert(0 && "TODO");
    // android::sp<GLConsumer> surfaceTexture(SurfaceTexture_getSurfaceTexture(this));
    // return surfaceTexture->detachFromContext();
}

Int32 CSurfaceTexture::NativeAttachToGLContext(
    /* [in] */ Int32 texName)
{
    assert(0 && "TODO");
    // android::sp<GLConsumer> surfaceTexture(SurfaceTexture_getSurfaceTexture(this));
    // return surfaceTexture->attachToContext((GLuint)tex);
}

void CSurfaceTexture::NativeRelease()
{
    assert(0 && "TODO");
    // android::sp<GLConsumer> surfaceTexture(SurfaceTexture_getSurfaceTexture(this));
    // surfaceTexture->abandon();
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
