
#ifndef __CCHOOSERESPONSE_H__
#define __CCHOOSERESPONSE_H__

#include "_CChooseResponse.h"
#include "accounts/CAccountManager.h"

using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Accounts {

CarClass(CChooseResponse)
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
    AutoPtr<CAccountManager::GetAuthTokenByTypeAndFeaturesTask> mHost;
};

} // namespace Accounts
} // namespace Droid
} // namespace Elastos

#endif //__CCHOOSERESPONSE_H__
