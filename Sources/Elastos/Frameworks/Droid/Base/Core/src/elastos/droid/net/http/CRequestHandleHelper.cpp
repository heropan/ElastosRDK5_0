
#include "CRequestHandleHelper.h"
#include "elastos/droid/ext/frameworkext.h"
#include "CRequestHandle.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

ECode CRequestHandleHelper::ComputeBasicAuthResponse(
    /* [in] */ const String& username,
    /* [in] */ const String& password,
    /* [out] */ String* response)
{
    if (username.IsNullOrEmpty() || password.IsNullOrEmpty()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // encode username:password to base64
    // TODO:
    // AutoPtr<Org::Apache::Commons::Codec::Binary> base64;
    // CBase64::AcquireSingleton((IBase64**)&pBase64);
    // AutoPtr<ArrayOf<Byte> > bytesIn;
    // return String(Base64.encodeBase64((username + ':' + password).getBytes()));
    return E_NOT_IMPLEMENTED;
}

ECode CRequestHandleHelper::AuthorizationHeader(
    /* [in] */ Boolean isProxy,
    /* [out] */ String* header)
{
    VALIDATE_NOT_NULL(header);

    if (!isProxy) {
        *header = CRequestHandle::AUTHORIZATION_HEADER;
    } else {
        *header = CRequestHandle::PROXY_AUTHORIZATION_HEADER;
    }
    return NOERROR;
}

}
}
}
}

