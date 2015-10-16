#include "elastos/droid/widget/CRadioGroup.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos{
namespace Droid{
namespace Widget{


IVIEW_METHODS_IMPL(CRadioGroup, RadioGroup)
IVIEWGROUP_METHODS_IMPL(CRadioGroup, RadioGroup)
IVIEWPARENT_METHODS_IMPL(CRadioGroup, RadioGroup)
IVIEWMANAGER_METHODS_IMPL(CRadioGroup, RadioGroup)
IDRAWABLECALLBACK_METHODS_IMPL(CRadioGroup, RadioGroup)
IKEYEVENTCALLBACK_METHODS_IMPL(CRadioGroup, RadioGroup)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CRadioGroup, RadioGroup)
ILINEARLAYOUT_METHODS_IMPL(CRadioGroup, RadioGroup)

PInterface CRadioGroup::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }

    return _CRadioGroup::Probe(riid);
}

ECode CRadioGroup::constructor(
    /* [in] */ IContext* context)
{
    return RadioGroup::Init(context);
}

ECode CRadioGroup::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return RadioGroup::Init(context, attrs);
}

ECode CRadioGroup::Check(
    /* [in] */ Int32 id)
{
    return RadioGroup::Check(id);
}

ECode CRadioGroup::GetCheckedRadioButtonId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = RadioGroup::GetCheckedRadioButtonId();
    return NOERROR;
}

ECode CRadioGroup::ClearCheck()
{
    return RadioGroup::ClearCheck();
}

ECode CRadioGroup::SetOnCheckedChangeListener(
    /* [in] */ IRadioGroupOnCheckedChangeListener* listener)
{
    return RadioGroup::SetOnCheckedChangeListener(listener);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
