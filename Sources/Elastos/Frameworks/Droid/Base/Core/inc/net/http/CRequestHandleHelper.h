
#ifndef __CREQUESTHANDLEHELPER_H__
#define __CREQUESTHANDLEHELPER_H__

#include "_CRequestHandleHelper.h"

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

#endif // __CREQUESTHANDLEHELPER_H__
