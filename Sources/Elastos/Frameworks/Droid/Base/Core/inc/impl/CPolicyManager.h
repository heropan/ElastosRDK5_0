
#ifndef __CPOLICYMANAGER_H__
#define __CPOLICYMANAGER_H__

#include "_CPolicyManager.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IWindowManagerPolicy;
using Elastos::Droid::View::IFallbackEventHandler;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {

CarClass(CPolicyManager)
{
public:
    CPolicyManager();

    CARAPI MakeNewWindow(
        /* [in] */ IContext* ctx,
        /* [out] */ IWindow** window);

    CARAPI MakeNewLayoutInflater(
        /* [in] */ IContext* ctx,
        /* [out] */ ILayoutInflater** inflater);

    CARAPI MakeNewWindowManager(
        /* [out] */ IWindowManagerPolicy** wm);

    CARAPI MakeNewFallbackEventHandler(
        /* [in] */ IContext* context,
        /* [out] */ IFallbackEventHandler** handler);

private:
    AutoPtr<IPolicy> mPolicy;
};

} // namespace Policy
} // namespace Internal
} // namepsace Droid
} // namespace Elastos

#endif // __CPOLICYMANAGER_H__
