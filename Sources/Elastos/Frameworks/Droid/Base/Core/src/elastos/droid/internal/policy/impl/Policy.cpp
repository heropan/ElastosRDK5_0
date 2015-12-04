
#include "elastos/droid/internal/policy/impl/Policy.h"

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
AutoPtr<ArrayOf<String> > Policy::preload_classes = Policy::MiddleInitPreloadClasses();

Policy::Policy()
{
}

ECode Policy::constructor()
{
}

ECode Policy::MakeNewWindow(
    /* [in] */ IContext* context,
    /* [out] */ IWindow** result)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return new PhoneWindow(context);
    assert(0);
    return NOERROR;
}

ECode Policy::MakeNewLayoutInflater(
    /* [in] */ IContext* context,
    /* [out] */ ILayoutInflater** result)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return new PhoneLayoutInflater(context);
    assert(0);
    return NOERROR;
}

ECode Policy::MakeNewWindowManager(
    /* [out] */ IWindowManagerPolicy** result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return new PhoneWindowManager();
    assert(0);
    return NOERROR;
}

ECode Policy::MakeNewFallbackEventHandler(
    /* [in] */ IContext* context,
    /* [out] */ IFallbackEventHandler** result)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return new PhoneFallbackEventHandler(context);
    assert(0);
    return NOERROR;
}

AutoPtr<ArrayOf<String> > Policy::MiddleInitPreloadClasses()
{
    // ==================before translated======================
    // ->WWZ_SIGN: ARRAY_INIT_START {
    // "com.android.internal.policy.impl.PhoneLayoutInflater",
    //          "com.android.internal.policy.impl.PhoneWindow",
    //          "com.android.internal.policy.impl.PhoneWindow$1",
    //          "com.android.internal.policy.impl.PhoneWindow$DialogMenuCallback",
    //          "com.android.internal.policy.impl.PhoneWindow$DecorView",
    //          "com.android.internal.policy.impl.PhoneWindow$PanelFeatureState",
    //          "com.android.internal.policy.impl.PhoneWindow$PanelFeatureState$SavedState",
    // ->WWZ_SIGN: ARRAY_INIT_END }
    assert(0);
    AutoPtr< ArrayOf<String> > empty;
    return empty;
}

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos

