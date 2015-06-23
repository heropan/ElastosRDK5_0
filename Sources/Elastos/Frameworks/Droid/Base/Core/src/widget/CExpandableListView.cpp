#include "ext/frameworkext.h"
#include "widget/CExpandableListView.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CExpandableListView, ExpandableListView)
IVIEWGROUP_METHODS_IMPL(CExpandableListView, ExpandableListView)
IVIEWPARENT_METHODS_IMPL(CExpandableListView, ExpandableListView)
IVIEWMANAGER_METHODS_IMPL(CExpandableListView, ExpandableListView)
IDRAWABLECALLBACK_METHODS_IMPL(CExpandableListView, ExpandableListView)
IKEYEVENTCALLBACK_METHODS_IMPL(CExpandableListView, ExpandableListView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CExpandableListView, ExpandableListView)
IADAPTERVIEW_METHODS_IMPL(CExpandableListView, ExpandableListView)
IABSLISTVIEW_METHODS_IMPL(CExpandableListView, ExpandableListView)
IABSLISTVIEW_INTERFACE_METHODS_IMPL(CExpandableListView, ExpandableListView)
ILISTVIEW_METHODS_IMPL(CExpandableListView, ExpandableListView)

PInterface CExpandableListView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CExpandableListView::Probe(riid);
}

ECode CExpandableListView::constructor(
    /* [in] */ IContext* context)
{
    return ExpandableListView::Init(context);
}

ECode CExpandableListView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ExpandableListView::Init(context, attrs);
}

ECode CExpandableListView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ExpandableListView::Init(context, attrs, defStyle);
}

ECode CExpandableListView::SetChildDivider(
    /* [in] */ IDrawable* childDivider)
{
    return ExpandableListView::SetChildDivider(childDivider);
}

ECode CExpandableListView::SetAdapter(
    /* [in] */ IExpandableListAdapter* adapter)
{
    return ExpandableListView::SetAdapter(adapter);
}

ECode CExpandableListView::GetExpandableListAdapter(
     /* [out] */ IExpandableListAdapter** adapter)
{
    VALIDATE_NOT_NULL(adapter);
    *adapter = ExpandableListView::GetExpandableListAdapter();
    return NOERROR;
}

ECode CExpandableListView::ExpandGroup(
    /* [in] */ Int32 groupPos,
    /* [out] */ Boolean* expanded)
{
    VALIDATE_NOT_NULL(expanded);
    *expanded = ExpandableListView::ExpandGroup(groupPos);
    return NOERROR;
}

ECode CExpandableListView::ExpandGroup(
    /* [in] */ Int32 groupPos,
    /* [in] */ Boolean animate,
    /* [out] */ Boolean* expanded)
{
    VALIDATE_NOT_NULL(expanded);
    *expanded = ExpandableListView::ExpandGroup(groupPos, animate);
    return NOERROR;
}

ECode CExpandableListView::CollapseGroup(
    /* [in] */ Int32 groupPos,
    /* [out] */ Boolean* collapse)
{
    VALIDATE_NOT_NULL(collapse);
    *collapse = ExpandableListView::CollapseGroup(groupPos);
    return NOERROR;
}

ECode CExpandableListView::SetOnGroupCollapseListener(
    /* [in] */ IOnGroupCollapseListener* onGroupCollapseListener)
{
    return ExpandableListView::SetOnGroupCollapseListener(onGroupCollapseListener);
}

ECode CExpandableListView::SetOnGroupExpandListener(
    /* [in] */ IOnGroupExpandListener* onGroupExpandListener)
{
    return ExpandableListView::SetOnGroupExpandListener(onGroupExpandListener);
}

ECode CExpandableListView::SetOnGroupClickListener(
    /* [in] */ IOnGroupClickListener* onGroupClickListener)
{
    return ExpandableListView::SetOnGroupClickListener(onGroupClickListener);
}

ECode CExpandableListView::SetOnChildClickListener(
    /* [in] */ IOnChildClickListener* onChildClickListener)
{
    return ExpandableListView::SetOnChildClickListener(onChildClickListener);
}

ECode CExpandableListView::GetExpandableListPosition(
    /* [in] */ Int32 flatListPosition,
    /* [out] */ Int64* position)
{
    VALIDATE_NOT_NULL(position);
    *position = ExpandableListView::GetExpandableListPosition(flatListPosition);
    return NOERROR;
}

ECode CExpandableListView::GetFlatListPosition(
    /* [in] */ Int64 packedPosition,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = ExpandableListView::GetFlatListPosition(packedPosition);
    return NOERROR;
}

ECode CExpandableListView::GetSelectedPosition(
    /* [out] */ Int64* position)
{
    VALIDATE_NOT_NULL(position);
    *position = ExpandableListView::GetSelectedPosition();
    return NOERROR;
}

ECode CExpandableListView::GetSelectedId(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    *id = ExpandableListView::GetSelectedId();
    return NOERROR;
}

ECode CExpandableListView::SetSelectedGroup(
    /* [in] */ Int32 groupPosition)
{
    return ExpandableListView::SetSelectedGroup(groupPosition);
}

ECode CExpandableListView::SetSelectedChild(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition,
    /* [in] */ Boolean shouldExpandGroup,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = ExpandableListView::SetSelectedChild(groupPosition, childPosition, shouldExpandGroup);
    return NOERROR;
}

ECode CExpandableListView::IsGroupExpanded(
    /* [in] */ Int32 groupPosition,
    /* [out] */ Boolean* expanded)
{
    VALIDATE_NOT_NULL(expanded);
    *expanded = ExpandableListView::IsGroupExpanded(groupPosition);
    return NOERROR;
}

ECode CExpandableListView::SetChildIndicator(
    /* [in] */ IDrawable* childIndicator)
{
    return ExpandableListView::SetChildIndicator(childIndicator);
}


ECode CExpandableListView::SetChildIndicatorBounds(
    /* [in] */ Int32 left,
    /* [in] */ Int32 right)
{
    return ExpandableListView::SetChildIndicatorBounds(left, right);
}


ECode CExpandableListView::SetGroupIndicator(
    /* [in] */ IDrawable* groupIndicator)
{
    return ExpandableListView::SetGroupIndicator(groupIndicator);
}


ECode CExpandableListView::SetIndicatorBounds(
    /* [in] */ Int32 left,
    /* [in] */ Int32 right)
{
    return ExpandableListView::SetIndicatorBounds(left, right);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
