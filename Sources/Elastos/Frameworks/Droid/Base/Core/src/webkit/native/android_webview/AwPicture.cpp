
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//                 AwPicture::DestroyRunnable
//===============================================================

AwPicture::DestroyRunnable::DestroyRunnable(
    /* [in] */ Int64 nativeAwPicture)
    : mNativeAwPicture(nativeAwPicture)
{
}

//@Override
ECode AwPicture::DestroyRunnable::Run()
{
    NativeDestroy(mNativeAwPicture);
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
    mCleanupReference = new CleanupReference(this, runnable);
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
void AwPicture::EndRecording()
{
    // Intentional no-op. The native picture ended recording prior to java c'tor call.
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
void AwPicture::Draw(
    /* [in] */ ICanvas* canvas)
{
    NativeDraw(mNativeAwPicture, canvas);
}

//@Override
//@SuppressWarnings("deprecation")
void AwPicture::WriteToStream(
    /* [in] */ IOutputStream* stream)
{
    UnsupportedOperation();
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
}

Int32 AwPicture::NativeGetHeight(
    /* [in] */ Int64 nativeAwPicture)
{
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
