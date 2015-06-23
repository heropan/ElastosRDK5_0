
#ifndef __CPREFERENCEACTIVITYHEADERADAPTER_H__
#define __CPREFERENCEACTIVITYHEADERADAPTER_H__

#include "_CPreferenceActivityHeaderAdapter.h"
#include "widget/ArrayAdapter.h"

using Elastos::Droid::Widget::ArrayAdapter;
using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::Widget::IImageView;

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CPreferenceActivityHeaderAdapter) , public ArrayAdapter
{
public:
    class HeaderViewHolder
        : public ElRefBase
        , public IInterface
    {
    public:
        CAR_INTERFACE_DECL()

    public:
        AutoPtr<IImageView> mIcon;
        AutoPtr<ITextView> mTitle;
        AutoPtr<ITextView> mSummary;
    };

public:
    CARAPI constructor(
        /* [in] */ IContext* context,
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

    CARAPI AddAll(
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

private:
    AutoPtr<ILayoutInflater> mInflater;
};
}// namespace Preference
}// namespace Droid
}// namespace Elastos

#endif  // __CPREFERENCEACTIVITYHEADERADAPTER_H__
