#include "elastos/droid/widget/CTwoLineListItem.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {

IVIEW_METHODS_IMPL(CTwoLineListItem, TwoLineListItem)
IVIEWGROUP_METHODS_IMPL(CTwoLineListItem, TwoLineListItem)
IVIEWPARENT_METHODS_IMPL(CTwoLineListItem, TwoLineListItem)
IVIEWMANAGER_METHODS_IMPL(CTwoLineListItem, TwoLineListItem)
IDRAWABLECALLBACK_METHODS_IMPL(CTwoLineListItem, TwoLineListItem)
IKEYEVENTCALLBACK_METHODS_IMPL(CTwoLineListItem, TwoLineListItem)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CTwoLineListItem, TwoLineListItem)
IRELATIVELAYOUT_METHODS_IMPL(CTwoLineListItem, TwoLineListItem)

PInterface CTwoLineListItem::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CTwoLineListItem::Probe(riid);
}

ECode CTwoLineListItem::constructor(
    /* [in] */ IContext* context)
{
    return TwoLineListItem::Init(context);
}

ECode CTwoLineListItem::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return TwoLineListItem::Init(context, attrs);
}

ECode CTwoLineListItem::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return TwoLineListItem::Init(context, attrs, defStyle);
}

ECode CTwoLineListItem::GetText1(
    /* [out] */ ITextView** tv)
{
    VALIDATE_NOT_NULL(tv);
    AutoPtr<ITextView> view = TwoLineListItem::GetText1();
    *tv = view;
    REFCOUNT_ADD(*tv);
    return NOERROR;
}

ECode CTwoLineListItem::GetText2(
    /* [out] */ ITextView** tv)
{
    VALIDATE_NOT_NULL(tv);
    AutoPtr<ITextView> view = TwoLineListItem::GetText2();
    *tv = view;
    REFCOUNT_ADD(*tv);
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos