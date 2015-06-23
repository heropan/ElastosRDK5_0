#ifndef __CEXPANDABLELISTVIEW_H__
#define __CEXPANDABLELISTVIEW_H__

#include "_CExpandableListView.h"
#include "widget/ExpandableListView.h"





namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CExpandableListView), public ExpandableListView
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    IADAPTERVIEW_METHODS_DECL()
    IABSLISTVIEW_METHODS_DECL()
    IABSLISTVIEW_INTERFACE_METHODS_DECL()
    ILISTVIEW_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

public:
    CARAPI SetChildDivider(
        /* [in] */ IDrawable* childDivider);

    CARAPI SetAdapter(
        /* [in] */ IExpandableListAdapter* adapter);

    CARAPI GetExpandableListAdapter(
        /* [out] */ IExpandableListAdapter** adapter);

    CARAPI ExpandGroup(
        /* [in] */ Int32 groupPos,
        /* [out] */ Boolean* expanded);

    CARAPI ExpandGroup(
        /* [in] */ Int32 groupPos,
        /* [in] */ Boolean animate,
        /* [out] */ Boolean* expanded);

    CARAPI CollapseGroup(
        /* [in] */ Int32 groupPos,
        /* [out] */ Boolean* collapse);

    CARAPI SetOnGroupCollapseListener(
        /* [in] */ IOnGroupCollapseListener* onGroupCollapseListener);

    CARAPI SetOnGroupExpandListener(
        /* [in] */ IOnGroupExpandListener* onGroupExpandListener);

    CARAPI SetOnGroupClickListener(
        /* [in] */ IOnGroupClickListener* onGroupClickListener);

    CARAPI SetOnChildClickListener(
        /* [in] */ IOnChildClickListener* onChildClickListener);

    CARAPI GetExpandableListPosition(
        /* [in] */ Int32 flatListPosition,
        /* [out] */ Int64* position);

    CARAPI GetFlatListPosition(
        /* [in] */ Int64 packedPosition,
        /* [out] */ Int32* position);

    CARAPI GetSelectedPosition(
        /* [out] */ Int64* position);

    CARAPI GetSelectedId(
        /* [out] */ Int64* id);

    CARAPI SetSelectedGroup(
        /* [in] */ Int32 groupPosition);

    CARAPI SetSelectedChild(
        /* [in] */ Int32 groupPosition,
        /* [in] */ Int32 childPosition,
        /* [in] */ Boolean shouldExpandGroup,
        /* [out] */ Boolean* res);

    CARAPI IsGroupExpanded(
        /* [in] */ Int32 groupPosition,
        /* [out] */ Boolean* expanded);

    CARAPI SetChildIndicator(
        /* [in] */ IDrawable* childIndicator);

    CARAPI SetChildIndicatorBounds(
        /* [in] */ Int32 left,
        /* [in] */ Int32 right);

    CARAPI SetGroupIndicator(
        /* [in] */ IDrawable* groupIndicator);

    CARAPI SetIndicatorBounds(
        /* [in] */ Int32 left,
        /* [in] */ Int32 right);

};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__CEXPANDABLELISTVIEW_H__
