#ifndef __ELASTOS_DROID_INTERNAL_POLICY_IMPL_POLICY_H__
#define __ELASTOS_DROID_INTERNAL_POLICY_IMPL_POLICY_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Internal::Policy::IIPolicy;
using Elastos::Droid::View::IFallbackEventHandler;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::IWindowManagerPolicy;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

/**
  * {@hide}
  */
// Simple implementation of the policy interface that spawns the right
// set of objects
class Policy
    : public Object
    , public IIPolicy
{
public:
    CAR_INTERFACE_DECL()

    Policy();

    CARAPI constructor();

    CARAPI MakeNewWindow(
        /* [in] */ IContext* context,
        /* [out] */ IWindow** result);

    CARAPI MakeNewLayoutInflater(
        /* [in] */ IContext* context,
        /* [out] */ ILayoutInflater** result);

    CARAPI MakeNewWindowManager(
        /* [out] */ IWindowManagerPolicy** result);

    CARAPI MakeNewFallbackEventHandler(
        /* [in] */ IContext* context,
        /* [out] */ IFallbackEventHandler** result);

private:
    static CARAPI_(AutoPtr<ArrayOf<String> >) MiddleInitPreloadClasses();

private:
    static const String TAG;
    static AutoPtr<ArrayOf<String> > preload_classes;
};

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_POLICY_IMPL_POLICY_H__

