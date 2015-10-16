
#include "elastos/droid/widget/CCheckBox.h"

using Elastos::Droid::View::EIID_View;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CCheckBox, CheckBox)
IDRAWABLECALLBACK_METHODS_IMPL(CCheckBox, CheckBox)
IKEYEVENTCALLBACK_METHODS_IMPL(CCheckBox, CheckBox)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CCheckBox, CheckBox)
ITEXTVIEW_METHODS_IMPL(CCheckBox, CheckBox)
ICOMPOUNDBUTTON_METHODS_IMPL(CCheckBox, CheckBox)
ICHECKABLE_METHODS_IMPL(CCheckBox, CheckBox)

PInterface CCheckBox::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_TextView) {
        return reinterpret_cast<PInterface>((TextView*)this);
    }
    return _CCheckBox::Probe(riid);
}

ECode CCheckBox::constructor(
    /* [in] */ IContext* ctx)
{
    return CheckBox::Init(ctx);
}

ECode CCheckBox::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    return CheckBox::Init(ctx, attrs);
}

ECode CCheckBox::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* atrs,
    /* [in] */ Int32 defStyle)
{
    return CheckBox::Init(context, atrs, defStyle);
}

ECode CCheckBox::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = CheckBox::OnPreDraw();

    return NOERROR;
}

ECode CCheckBox::GetButtonDrawable(
    /* [out] */ IDrawable** d)
{
    AutoPtr<IDrawable> dTemp = CheckBox::GetButtonDrawable();
    *d = dTemp;
    REFCOUNT_ADD(*d);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
