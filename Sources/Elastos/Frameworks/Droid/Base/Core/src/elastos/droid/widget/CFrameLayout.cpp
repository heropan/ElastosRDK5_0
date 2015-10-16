
#include "elastos/droid/widget/CFrameLayout.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CFrameLayout, FrameLayout)
IVIEWGROUP_METHODS_IMPL(CFrameLayout, FrameLayout)
IVIEWPARENT_METHODS_IMPL(CFrameLayout, FrameLayout)
IVIEWMANAGER_METHODS_IMPL(CFrameLayout, FrameLayout)
IDRAWABLECALLBACK_METHODS_IMPL(CFrameLayout, FrameLayout)
IKEYEVENTCALLBACK_METHODS_IMPL(CFrameLayout, FrameLayout)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CFrameLayout, FrameLayout)

PInterface CFrameLayout::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CFrameLayout::Probe(riid);
}

ECode CFrameLayout::constructor(
    /* [in] */ IContext* context)
{
    return FrameLayout::Init(context);
}

ECode CFrameLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return FrameLayout::Init(context, attrs);
}

ECode CFrameLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return FrameLayout::Init(context, attrs, defStyle);
}

ECode CFrameLayout::GetForegroundGravity(
    /* [out] */ Int32* foregroundGravity)
{
    VALIDATE_NOT_NULL(foregroundGravity);
    *foregroundGravity  = FrameLayout::GetForegroundGravity();
    return NOERROR;
}

ECode CFrameLayout::SetForegroundGravity(
    /* [in] */ Int32 foregroundGravity)
{
    return FrameLayout::SetForegroundGravity(foregroundGravity);
}

ECode CFrameLayout::SetForeground(
    /* [in] */ IDrawable* drawable)
{
    return FrameLayout::SetForeground(drawable);
}

ECode CFrameLayout::GetForeground(
    /* [out] */ IDrawable** foreground)
{
    VALIDATE_NOT_NULL(foreground);
    AutoPtr<IDrawable> g = FrameLayout::GetForeground();
    *foreground = g;
    REFCOUNT_ADD(*foreground);
    return NOERROR;
}

ECode CFrameLayout::GetMeasureAllChildren(
    /* [out] */ Boolean* measureAll)
{
    VALIDATE_NOT_NULL(measureAll);
    *measureAll  = FrameLayout::GetMeasureAllChildren();
    return NOERROR;
}

ECode CFrameLayout::SetMeasureAllChildren(
    /* [in] */ Boolean measureAll)
{
    return FrameLayout::SetMeasureAllChildren(measureAll);
}

ECode CFrameLayout::GetConsiderGoneChildrenWhenMeasuring(
    /* [out] */ Boolean* consider)
{
    VALIDATE_NOT_NULL(consider);
    *consider  = FrameLayout::GetConsiderGoneChildrenWhenMeasuring();
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
