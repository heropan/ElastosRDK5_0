
#ifndef __CACCOUNTAUTHENTICATORRESPONSE_H__
#define __CACCOUNTAUTHENTICATORRESPONSE_H__

#include "_CAccountAuthenticatorResponse.h"

using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Accounts {

CarClass(CAccountAuthenticatorResponse)
{
public:
    CARAPI OnResult(
        /* [in] */ IBundle* result);

    CARAPI OnRequestContinued();

    CARAPI OnError(
        /* [in] */ Int32 errorCode,
        /* [in] */ const String& errorMessage);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI constructor(
        /* [in] */ IIAccountAuthenticatorResponse* response);

    CARAPI constructor();

private:
    static const String TAG;
    AutoPtr<IIAccountAuthenticatorResponse> mAccountAuthenticatorResponse;
};

} // namespace Accounts
} // namespace Droid
} // namespace Elastos

#endif //__CACCOUNTAUTHENTICATORRESPONSE_H__
