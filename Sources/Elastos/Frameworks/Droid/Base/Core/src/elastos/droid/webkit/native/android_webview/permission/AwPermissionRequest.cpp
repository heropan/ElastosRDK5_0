#include "elastos/droid/webkit/native/android_webview/permission/AwPermissionRequest.h"
#include "elastos/droid/webkit/native/base/ThreadUtils.h"

//TODO #include "elastos/droid/net/CUriHelper.h"
#include "elastos/utility/logging/Logger.h"

using Elastos::Droid::Net::IUri;
using Elastos::Droid::Net::IUriHelper;
//TODO using Elastos::Droid::Net::CUriHelper;
using Elastos::Droid::Webkit::Base::ThreadUtils;

using Elastos::Utility::Logging::Logger;

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
    : mOrigin(origin)
    , mResources(resources)
    , mProcessed(FALSE)
    , mNativeAwPermissionRequest(nativeAwPermissionRequest)
{
}

//@Override
AwPermissionRequest::~AwPermissionRequest()
{
    if (mNativeAwPermissionRequest == 0) return;
    Logger::E(TAG, "Neither grant() nor deny() has been called,\
                    the permission request is denied by default.");
    Deny();
}

//@CalledByNative return AwPermissionRequest
AutoPtr<IInterface> AwPermissionRequest::Create(
    /* [in] */ Int64 nativeAwPermissionRequest,
    /* [in] */ const String& url,
    /* [in] */ Int64 resources)
{
    if (nativeAwPermissionRequest == 0) return NULL;

    AutoPtr<IUriHelper> helper;
    //TODO CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    AutoPtr<IUri> origin;
    helper->Parse(url, (IUri**)&origin);
    AutoPtr<AwPermissionRequest> request = new AwPermissionRequest(nativeAwPermissionRequest, origin, resources);
    AutoPtr<IInterface> result = request->Probe(EIID_IInterface);
    return result;
}



AutoPtr<IUri> AwPermissionRequest::GetOrigin()
{
    return mOrigin;
}

Int64 AwPermissionRequest::GetResources()
{
    return mResources;
}

ECode AwPermissionRequest::Grant()
{
    ECode validate = Validate();
    if (FAILED(validate))
        return validate;
    if (mNativeAwPermissionRequest != 0)
        NativeOnAccept(mNativeAwPermissionRequest, TRUE);
    mProcessed = TRUE;
    return NOERROR;
}

ECode AwPermissionRequest::Deny()
{
    ECode validate = Validate();
    if (FAILED(validate))
        return validate;
    if (mNativeAwPermissionRequest != 0)
        NativeOnAccept(mNativeAwPermissionRequest, FALSE);
    mProcessed = TRUE;
    return NOERROR;
}

//@CalledByNative
void AwPermissionRequest::DetachNativeInstance()
{
    mNativeAwPermissionRequest = 0;
}

ECode AwPermissionRequest::Validate()
{
    if (!ThreadUtils::RunningOnUiThread()) {
        // throw new IllegalStateException(
        //         "Either grant() or deny() should be called on UI thread");
        //assert(0);
        Logger::E(TAG, "Either grant() or deny() should be called on UI thread");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (mProcessed) {
        // throw new IllegalStateException("Either grant() or deny() has been already called.");
        //assert(0);
        Logger::E(TAG, "Either grant() or deny() has been already called.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
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
