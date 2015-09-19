
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

AwHttpAuthHandler::AwHttpAuthHandler(
    /* [in] */ Int64 nativeAwHttpAuthHandler,
    /* [in] */ Boolean firstAttempt)
    : mNativeAwHttpAuthHandler(nativeAwHttpAuthHandler)
    , mFirstAttempt(firstAttempt)
{
}

void AwHttpAuthHandler::Proceed(
    /* [in] */ String username,
    /* [in] */ String password)
{
    if (mNativeAwHttpAuthHandler != 0) {
        NativeProceed(mNativeAwHttpAuthHandler, username, password);
        mNativeAwHttpAuthHandler = 0;
    }
}

void AwHttpAuthHandler::Cancel()
{
    if (mNativeAwHttpAuthHandler != 0) {
        NativeCancel(mNativeAwHttpAuthHandler);
        mNativeAwHttpAuthHandler = 0;
    }
}

Boolean AwHttpAuthHandler::IsFirstAttempt()
{
     return mFirstAttempt;
}

//@CalledByNative
AutoPtr<AwHttpAuthHandler> AwHttpAuthHandler::Create(
    /* [in] */ Int64 nativeAwAuthHandler,
    /* [in] */ Boolean firstAttempt)
{
    AutoPtr<AwHttpAuthHandler> handler = new AwHttpAuthHandler(nativeAwAuthHandler, firstAttempt);
    return handler;
}

//@CalledByNative
void AwHttpAuthHandler::HandlerDestroyed()
{
    mNativeAwHttpAuthHandler = 0;
}

void AwHttpAuthHandler::NativeProceed(
    /* [in] */ Int64 nativeAwHttpAuthHandler,
    /* [in] */ String username,
    /* [in] */ String password)
{
}

void AwHttpAuthHandler::NativeCancel(
    /* [in] */ Int64 nativeAwHttpAuthHandler)
{
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos