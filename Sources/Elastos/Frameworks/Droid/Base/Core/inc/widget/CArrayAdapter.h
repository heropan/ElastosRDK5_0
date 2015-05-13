#ifndef __CARRAYADAPTER_H__
#define __CARRAYADAPTER_H__

#include "_CArrayAdapter.h"
#include "widget/ArrayAdapter.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CArrayAdapter) , public ArrayAdapter
{
public:

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 textViewResourceId);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resource,
        /* [in] */ Int32 textViewResourceId);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 textViewResourceId,
        /* [in] */ IObjectContainer* objects);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resource,
        /* [in] */ Int32 textViewResourceId,
        /* [in] */ IObjectContainer* objects);

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

    CARAPI NotifyDataSetChanged();

    CARAPI NotifyDataSetInvalidated();

    CARAPI Add(
        /* [in] */ IInterface* object);

    CARAPI AddAll(
        /* [in] */ IObjectContainer* collection);

    CARAPI AddAllEx(
        /* [in] */ ArrayOf<IInterface* >* items) ;

    CARAPI Insert(
        /* [in] */ IInterface* object,
        /* [in] */ Int32 index);

    CARAPI Remove(
        /* [in] */ IInterface* object);

    CARAPI Clear();

    CARAPI Sort(
        /* [in] */ IComparator* comparator);

    CARAPI SetNotifyOnChange(
        /* [in] */ Boolean notifyOnChange);

    CARAPI GetContext(
        /* [out] */ IContext** context);

    CARAPI GetPosition(
        /* [in] */ IInterface* item,
        /* [out] */ Int32* position);

    CARAPI SetDropDownViewResource(
        /* [in] */ Int32 resource);

    CARAPI GetDropDownView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** view);

    CARAPI AreAllItemsEnabled(
        /* [out] */ Boolean* enable);

    CARAPI IsEnabled(
        /* [in] */ Int32 position,
        /* [in] */ Boolean* enable);

    CARAPI GetFilter(
        /* [out] */ IFilter** filter);
};
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif // __CARRAYADAPTER_H__
