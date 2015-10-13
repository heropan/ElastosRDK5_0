#include "elastos/droid/webkit/native/android_webview/AwPicture.h"

using Elastos::Core::EIID_IRunnable;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//                 AwPicture::DestroyRunnable
//===============================================================
CAR_INTERFACE_IMPL(AwPicture::DestroyRunnable, Object, IRunnable);

AwPicture::DestroyRunnable::DestroyRunnable(
    /* [in] */ Int64 nativeAwPicture)
    : mNativeAwPicture(nativeAwPicture)
{
}

//@Override
ECode AwPicture::DestroyRunnable::Run()
{
    NativeDestroy(mNativeAwPicture);
    return NOERROR;
}

//===============================================================
//                         AwPicture
//===============================================================

/**
 * @param nativeAwPicture is an instance of the AwPicture native class. Ownership is
 *                        taken by this java instance.
 */
AwPicture::AwPicture(
    /* [in] */ Int64 nativeAwPicture)
    : mNativeAwPicture(nativeAwPicture)
{
    AutoPtr<IRunnable> runnable = new DestroyRunnable(nativeAwPicture);
    //TODO mCleanupReference = new CleanupReference(this, runnable);
}

//@Override
AutoPtr<ICanvas> AwPicture::BeginRecording(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    UnsupportedOperation();
    return NULL;
}

//@Override
ECode AwPicture::EndRecording()
{
    // Intentional no-op. The native picture ended recording prior to java c'tor call.
    return NOERROR;
}

//@Override
Int32 AwPicture::GetWidth()
{
    return NativeGetWidth(mNativeAwPicture);
}

//@Override
Int32 AwPicture::GetHeight()
{
    return NativeGetHeight(mNativeAwPicture);
}

//@Override
ECode AwPicture::Draw(
    /* [in] */ ICanvas* canvas)
{
    NativeDraw(mNativeAwPicture, canvas);
    return NOERROR;
}

//@Override
//@SuppressWarnings("deprecation")
ECode AwPicture::WriteToStream(
    /* [in] */ IOutputStream* stream)
{
    UnsupportedOperation();
    return E_NOT_IMPLEMENTED;
}

void AwPicture::UnsupportedOperation()
{
    //throw new IllegalStateException("Unsupported in AwPicture");
    assert(0);
}

void AwPicture::NativeDestroy(
    /* [in] */ Int64 nativeAwPicture)
{
}

Int32 AwPicture::NativeGetWidth(
    /* [in] */ Int64 nativeAwPicture)
{
    return 0;
}

Int32 AwPicture::NativeGetHeight(
    /* [in] */ Int64 nativeAwPicture)
{
    return 0;
}

void AwPicture::NativeDraw(
    /* [in] */ Int64 nativeAwPicture,
    /* [in] */ ICanvas* canvas)
{
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
