
#include "elastos/droid/internal/policy/impl/Policy.h"
#include "elastos/droid/internal/policy/impl/CPhoneWindow.h"
#include "elastos/droid/internal/policy/impl/CPhoneLayoutInflater.h"
#include "elastos/droid/internal/policy/impl/CPhoneWindowManager.h"
#include "elastos/droid/internal/policy/impl/CPhoneFallbackEventHandler.h"

using Elastos::Droid::Internal::Policy::EIID_IIPolicy;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

//=====================================================================
//                                Policy
//=====================================================================
CAR_INTERFACE_IMPL(Policy, Object, IIPolicy);
const String Policy::TAG("PhonePolicy");

Policy::Policy()
{
}

Policy::constructor()
{
    return NOERROR;
}

ECode Policy::MakeNewWindow(
    /* [in] */ IContext* context,
    /* [out] */ IWindow** window)
{
    return CPhoneWindow::New(context, window);
}

ECode Policy::MakeNewLayoutInflater(
    /* [in] */ IContext* context,
    /* [out] */ ILayoutInflater** inflater)
{
    return CPhoneLayoutInflater::New(context, inflater);
}

ECode Policy::MakeNewWindowManager(
    /* [out] */ IWindowManagerPolicy** wm)
{
    return CPhoneWindowManager::New(wm);
}

ECode Policy::MakeNewFallbackEventHandler(
    /* [in] */ IContext* context,
    /* [out] */ IFallbackEventHandler** handler)
{
    return CPhoneFallbackEventHandler::New(context, handler);
}

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos
