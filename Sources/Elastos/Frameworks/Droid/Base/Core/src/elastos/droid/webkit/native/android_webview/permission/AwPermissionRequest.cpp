
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {
namespace Permission {

String AwPermissionRequest::TAG("AwPermissionRequest");

AwPermissionRequest::AwPermissionRequest(
    /* [in] */ Int64 nativeAwPermissionRequest,
    /* [in] */ IUri* origin,
    /* [in] */ Int64 resources)
    : mNativeAwPermissionRequest(nativeAwPermissionRequest)
    , mOrigin(origin)
    , mResources(resources)
    , mProcessed(FALSE)
{
}

//@Override
AwPermissionRequest::~AwPermissionRequest()
{
    if (mNativeAwPermissionRequest == 0) return;
//    Log.e(TAG, "Neither grant() nor deny() has been called, "
//            + "the permission request is denied by default.");
    Deny();
}

//@CalledByNative
AutoPtr<AwPermissionRequest> AwPermissionRequest::Create(
    /* [in] */ Int64 nativeAwPermissionRequest,
    /* [in] */ String url,
    /* [in] */ Int64 resources)
{
    if (nativeAwPermissionRequest == 0) return NULL;

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    AutoPtr<IUri> origin;
    helper->Parse(url, (IUri**)&origin);
    AutoPtr<AwPermissionRequest> request = new AwPermissionRequest(nativeAwPermissionRequest, origin, resources);
    return request;
}



AutoPtr<IUri> AwPermissionRequest::GetOrigin()
{
    return mOrigin;
}

Int64 AwPermissionRequest::GetResources()
{
    return mResources;
}

void AwPermissionRequest::Grant()
{
    Validate();
    if (mNativeAwPermissionRequest != 0)
        NativeOnAccept(mNativeAwPermissionRequest, TRUE);
    mProcessed = TRUE;
}

void AwPermissionRequest::Deny()
{
    Validate();
    if (mNativeAwPermissionRequest != 0)
        NativeOnAccept(mNativeAwPermissionRequest, FALSE);
    mProcessed = TRUE;
}

//@CalledByNative
void AwPermissionRequest::DetachNativeInstance()
{
    mNativeAwPermissionRequest = 0;
}

void AwPermissionRequest::Validate()
{
    if (!ThreadUtils::RunningOnUiThread()) {
        // throw new IllegalStateException(
        //         "Either grant() or deny() should be called on UI thread");
        assert(0);
    }

    if (mProcessed) {
        // throw new IllegalStateException("Either grant() or deny() has been already called.");
        assert(0);
    }
}

void AwPermissionRequest::NativeOnAccept(
    /* [in] */ Int64 nativeAwPermissionRequest,
    /* [in] */ Boolean allowed)
{
}

} // namespace Permission
} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
