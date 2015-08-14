
#ifndef __ELASTOS_DROID_ACCOUNTS_CACCOUNTMANAGERAMSRESPONSE_H__
#define __ELASTOS_DROID_ACCOUNTS_CACCOUNTMANAGERAMSRESPONSE_H__

#include "_CAccountManagerAmsResponse.h"
#include "accounts/CAccountManager.h"

using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Accounts {

CarClass(CAccountManagerAmsResponse)
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
    AutoPtr<CAccountManager::AmsTask> mHost;
};

} // namespace Accounts
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_ACCOUNTS_CACCOUNTMANAGERAMSRESPONSE_H__
