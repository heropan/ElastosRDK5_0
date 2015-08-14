
#ifndef __ELASTOS_DROID_APP_CONKEYGUARDEXITRESULT_H__
#define __ELASTOS_DROID_APP_CONKEYGUARDEXITRESULT_H__

#include "_COnKeyguardExitResult.h"

namespace Elastos {
namespace Droid {
namespace App {

CarClass(COnKeyguardExitResult)
{
public:
    CARAPI constructor(
        /* [in] */ IKeyguardManagerOnKeyguardExitResult* cb);

    CARAPI OnKeyguardExitResult(
        /* [in] */ Boolean success);

private:
    AutoPtr<IKeyguardManagerOnKeyguardExitResult> mCallback;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_CONKEYGUARDEXITRESULT_H__
