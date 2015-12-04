
#include "elastos/droid/internal/policy/impl/PhoneLayoutInflater.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

//=====================================================================
//                         PhoneLayoutInflater
//=====================================================================
CAR_INTERFACE_IMPL(PhoneLayoutInflater, LayoutInflater, IPhoneLayoutInflater)
AutoPtr< ArrayOf<String> > PhoneLayoutInflater::sClassPrefixList = PhoneLayoutInflater::MiddleInitSclassprefixlist();

PhoneLayoutInflater::PhoneLayoutInflater()
{
}

CARAPI PhoneLayoutInflater::constructor(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // super(context);
    return NOERROR;
}

ECode PhoneLayoutInflater::OnCreateView(
    /* [in] */  const String& name,
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IView** view)

{
    VALIDATE_NOT_NULL(view);
    assert(0);
    return NOERROR;
}

ECode PhoneLayoutInflater::CloneInContext(
    /* [in] */ IContext* newContext,
    /* [out] */ ILayoutInflater** result)
{
    VALIDATE_NOT_NULL(result);
    assert(0);
    return NOERROR;
}

PhoneLayoutInflater::PhoneLayoutInflater(
    /* [in] */ ILayoutInflater* original,
    /* [in] */ IContext* newContext)
{
    // ==================before translated======================
    // super(original, newContext);
}

AutoPtr< ArrayOf<String> > PhoneLayoutInflater::MiddleInitSclassprefixlist()
{
    // ==================before translated======================
    // ->WWZ_SIGN: ARRAY_INIT_START {
    // "android.widget.",
    //          "android.webkit.",
    //          "android.app."
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


