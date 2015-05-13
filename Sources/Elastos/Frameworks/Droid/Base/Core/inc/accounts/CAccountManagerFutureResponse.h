
#ifndef __CACCOUNTMANAGERFUTURERESPONSE_H__
#define __CACCOUNTMANAGERFUTURERESPONSE_H__

#include "_CAccountManagerFutureResponse.h"
#include "accounts/CAccountManager.h"

using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Accounts {

CarClass(CAccountManagerFutureResponse)
{
public:
    CARAPI OnResult(
        /* [in] */ IBundle* value);

    CARAPI OnError(
        /* [in] */ Int32 errorCode,
        /* [in] */ const String& errorMessage);

    CARAPI constructor (
        /* [in] */ Handle32 host);

private:
    AutoPtr<CAccountManager::BaseFutureTask> mHost;
};

} // namespace Accounts
} // namespace Droid
} // namespace Elastos

#endif //__CACCOUNTMANAGERFUTURERESPONSE_H__
