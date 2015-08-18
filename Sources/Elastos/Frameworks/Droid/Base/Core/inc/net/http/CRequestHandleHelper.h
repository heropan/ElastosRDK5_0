
#ifndef __ELASTOS_DROID_NET_HTTP_CREQUESTHANDLEHELPER_H__
#define __ELASTOS_DROID_NET_HTTP_CREQUESTHANDLEHELPER_H__

#include "_Elastos_Droid_Net_Http_CRequestHandleHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CarClass(CRequestHandleHelper)
{
public:
    /**
     * @return Basic-scheme authentication response: BASE64(username:password).
     */
    CARAPI ComputeBasicAuthResponse(
        /* [in] */ const String& username,
        /* [in] */ const String& password,
        /* [out] */ String* response);

    /**
     * @return The right authorization header (dependeing on whether it is a proxy or not).
     */
    CARAPI AuthorizationHeader(
        /* [in] */ Boolean isProxy,
        /* [out] */ String* header);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}

#endif // __ELASTOS_DROID_NET_HTTP_CREQUESTHANDLEHELPER_H__
