
#include "impl/CPolicy.h"
#include "impl/CPhoneWindow.h"
#include "impl/CPhoneLayoutInflater.h"
#include "impl/CPhoneWindowManager.h"
#include "impl/CPhoneFallbackEventHandler.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

ECode CPolicy::MakeNewWindow(
    /* [in] */ IContext* context,
    /* [out] */ IWindow** window)
{
    VALIDATE_NOT_NULL(window);
    return CPhoneWindow::New(context, window);
}

ECode CPolicy::MakeNewLayoutInflater(
    /* [in] */ IContext* context,
    /* [out] */ ILayoutInflater** inflater)
{
    VALIDATE_NOT_NULL(inflater);
    return CPhoneLayoutInflater::New(context, inflater);
}

ECode CPolicy::MakeNewWindowManager(
    /* [out] */ IWindowManagerPolicy** wm)
{
    VALIDATE_NOT_NULL(wm);
    return CPhoneWindowManager::New(wm);
}

ECode CPolicy::MakeNewFallbackEventHandler(
    /* [in] */ IContext* context,
    /* [out] */ IFallbackEventHandler** handler)
{
    VALIDATE_NOT_NULL(handler);
    AutoPtr<IPhoneFallbackEventHandler> phoneHandler;
    FAIL_RETURN(CPhoneFallbackEventHandler::New(context, (IPhoneFallbackEventHandler**)&phoneHandler));
    *handler = (IFallbackEventHandler*)phoneHandler.Get();
    REFCOUNT_ADD(*handler);
    return NOERROR;
}

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namepsace Droid
} // namespace Elastos
