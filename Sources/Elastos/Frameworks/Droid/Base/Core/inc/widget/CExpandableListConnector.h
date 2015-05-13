
#ifndef __CEXPANDABLELISTCONNECTOR_H__
#define __CEXPANDABLELISTCONNECTOR_H__

#include "_CExpandableListConnector.h"
#include "widget/ExpandableListConnector.h"

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CExpandableListConnector), public ExpandableListConnector
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetExpandableListAdapter(
        /* [in] */ IExpandableListAdapter* expandableListAdapter);

    CARAPI GetUnflattenedPos(
        /* [in] */ Int32 flPos,
        /* [out] */ IPositionMetadata** positionMetadata);

    CARAPI IsGroupExpanded(
        /* [in] */ Int32 groupPosition,
        /* [out] */ Boolean* expanded);

    CARAPI SetMaxExpGroupCount(
        /* [in] */ Int32 maxExpGroupCount);

    CARAPI GetFlattenedPos(
        /* [in] */ IExpandableListPosition* pos,
        /* [out] */ IPositionMetadata** data);

    CARAPI ExpandGroup(
        /* [in] */ Int32 groupPos,
        /* [out] */ Boolean* res);

    CARAPI ExpandGroupEx(
        /* [in] */ IPositionMetadata* posMetadata,
        /* [out] */ Boolean* res);

    CARAPI CollapseGroup(
        /* [in] */ Int32 groupPos,
        /* [out] */ Boolean* res);

    CARAPI CollapseGroupEx(
        /* [in] */ IPositionMetadata* posMetadata,
        /* [out] */ Boolean* res);

    CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer);

    CARAPI GetCount(
        /* [out] */ Int32* count);

    CARAPI GetItem(
        /* [in] */ Int32 position,
        /* [out] */ IInterface** item);

    CARAPI GetItemId(
        /* [in] */ Int32 position,
        /* [out] */ Int64* itemId);

    CARAPI HasStableIds(
        /* [out] */ Boolean* hasStableIds);

    CARAPI GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** view);

    CARAPI GetItemViewType(
        /* [in] */ Int32 position,
        /* [out] */ Int32* viewType);

    CARAPI GetViewTypeCount(
        /* [out] */ Int32* count);

    CARAPI IsEmpty(
        /* [out] */ Boolean* isEmpty);

    CARAPI IsEnabled(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* enabled);

    CARAPI AreAllItemsEnabled(
        /* [out] */ Boolean* enabled);

    CARAPI NotifyDataSetChanged();

    CARAPI NotifyDataSetInvalidated();

    CARAPI GetDropDownView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** view);

    CARAPI GetFilter(
        /* [out] */ IFilter** filter);

};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__CEXPANDABLELISTCONNECTOR_H__