
#include "elastos/droid/app/COnKeyguardExitResult.h"

namespace Elastos {
namespace Droid {
namespace App {

ECode COnKeyguardExitResult::constructor(
    /* [in] */ IKeyguardManagerOnKeyguardExitResult* cb)
{
    mCallback = cb;
    return NOERROR;
}

ECode COnKeyguardExitResult::OnKeyguardExitResult(
    /* [in] */ Boolean success)
{
    return mCallback->OnKeyguardExitResult(success);
}

} // namespace App
} // namespace Droid
} // namespace Elastos
