#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/CGridLayout.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;
using Elastos::Droid::Widget::EIID_GridLayout;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CGridLayout, GridLayout)
IVIEWGROUP_METHODS_IMPL(CGridLayout, GridLayout)
IVIEWPARENT_METHODS_IMPL(CGridLayout, GridLayout)
IVIEWMANAGER_METHODS_IMPL(CGridLayout, GridLayout)
IDRAWABLECALLBACK_METHODS_IMPL(CGridLayout, GridLayout)
IKEYEVENTCALLBACK_METHODS_IMPL(CGridLayout, GridLayout)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CGridLayout, GridLayout)

PInterface CGridLayout::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    else if (riid == EIID_GridLayout) {
        return reinterpret_cast<PInterface>((GridLayout*)this);
    }
    return _CGridLayout::Probe(riid);
}

ECode CGridLayout::constructor(
    /* [in] */ IContext* context)
{
    return GridLayout::Init(context);
}

ECode CGridLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return GridLayout::Init(context, attrs);
}

ECode CGridLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return GridLayout::Init(context, attrs, defStyle);
}

ECode CGridLayout::GetOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);
    *orientation = GridLayout::GetOrientation();
    return NOERROR;
}

ECode CGridLayout::SetOrientation(
    /* [in] */ Int32 orientation)
{
    return GridLayout::SetOrientation(orientation);
}

ECode CGridLayout::GetRowCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = GridLayout::GetRowCount();
    return NOERROR;
}

ECode CGridLayout::SetRowCount(
    /* [in] */ Int32 rowCount)
{
    return GridLayout::SetRowCount(rowCount);
}

ECode CGridLayout::GetColumnCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = GridLayout::GetColumnCount();
    return NOERROR;
}

ECode CGridLayout::SetColumnCount(
    /* [in] */ Int32 columnCount)
{
    return GridLayout::SetColumnCount(columnCount);
}

ECode CGridLayout::GetUseDefaultMargins(
    /* [out] */ Boolean* margins)
{
    VALIDATE_NOT_NULL(margins);
    *margins = GridLayout::GetUseDefaultMargins();
    return NOERROR;
}

ECode CGridLayout::SetUseDefaultMargins(
    /* [in] */ Boolean useDefaultMargins)
{
    return GridLayout::SetUseDefaultMargins(useDefaultMargins);
}

ECode CGridLayout::GetAlignmentMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = GridLayout::GetAlignmentMode();
    return NOERROR;
}

ECode CGridLayout::SetAlignmentMode(
    /* [in] */ Int32 alignmentMode)
{
    return GridLayout::SetAlignmentMode(alignmentMode);
}

ECode CGridLayout::IsRowOrderPreserved(
    /* [out] */ Boolean* preserved)
{
    VALIDATE_NOT_NULL(preserved);
    *preserved = GridLayout::IsRowOrderPreserved();
    return NOERROR;
}

ECode CGridLayout::SetRowOrderPreserved(
    /* [in] */ Boolean rowOrderPreserved)
{
    return GridLayout::SetRowOrderPreserved(rowOrderPreserved);
}

ECode CGridLayout::IsColumnOrderPreserved(
    /* [out] */ Boolean* preserved)
{
    VALIDATE_NOT_NULL(preserved);
    *preserved = GridLayout::IsColumnOrderPreserved();
    return NOERROR;
}

ECode CGridLayout::SetColumnOrderPreserved(
    /* [in] */ Boolean columnOrderPreserved)
{
    return GridLayout::SetColumnOrderPreserved(columnOrderPreserved);
}

ECode CGridLayout::GetMeasurementIncludingMargin(
    /* [in] */ IView* c,
    /* [in] */ Boolean horizontal,
    /* [out] */ Int32* margin)
{
    VALIDATE_NOT_NULL(margin);
    *margin = GridLayout::GetMeasurementIncludingMargin(c, horizontal);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos