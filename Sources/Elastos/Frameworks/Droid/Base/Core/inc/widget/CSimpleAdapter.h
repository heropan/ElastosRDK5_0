#ifndef __CSIMPLEADAPTER_H__
#define __CSIMPLEADAPTER_H__

#include "_CSimpleAdapter.h"
#include "widget/SimpleAdapter.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CSimpleAdapter), public SimpleAdapter
{
public:

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IObjectContainer* data,
        /* [in] */ Int32 resource,
        /* [in] */ ArrayOf<String>* from,
        /* [in] */ ArrayOf<Int32>* to);

    /**
     * <p>Sets the layout resource to create the drop down views.</p>
     *
     * @param resource the layout resource defining the drop down views
     * @see #getDropDownView(Int32, android.view.View, android.view.ViewGroup)
     */
    CARAPI SetDropDownViewResource(
        /* [in] */ Int32 resource);

    /**
     * Returns the {@link ViewBinder} used to bind data to views.
     *
     * @return a ViewBinder or NULL if the binder does not exist
     *
     * @see #setViewBinder(android.widget.SimpleAdapter.ViewBinder)
     */
    CARAPI GetViewBinder(
        /* [out] */ ISimpleAdapterViewBinder** viewBinder);

    /**
     * Sets the binder used to bind data to views.
     *
     * @param viewBinder the binder used to bind data to views, can be NULL to
     *        remove the existing binder
     *
     * @see #getViewBinder()
     */
    CARAPI SetViewBinder(
        /* [in] */ ISimpleAdapterViewBinder* viewBinder);

    /**
     * Called by bindView() to set the image for an ImageView but only if
     * there is no existing ViewBinder or if the existing ViewBinder cannot
     * handle binding to an ImageView.
     *
     * This method is called instead of {@link #setViewImage(ImageView, String)}
     * if the supplied data is an Int32 or Integer.
     *
     * @param v ImageView to receive an image
     * @param value the value retrieved from the data set
     *
     * @see #setViewImage(ImageView, String)
     */
    CARAPI SetViewImage(
        /* [in] */ IImageView* v,
        /* [in] */ Int32 value);

    /**
     * Called by bindView() to set the image for an ImageView but only if
     * there is no existing ViewBinder or if the existing ViewBinder cannot
     * handle binding to an ImageView.
     *
     * By default, the value will be treated as an image resource. If the
     * value cannot be used as an image resource, the value is used as an
     * image Uri.
     *
     * This method is called instead of {@link #setViewImage(ImageView, Int32)}
     * if the supplied data is not an Int32 or Integer.
     *
     * @param v ImageView to receive an image
     * @param value the value retrieved from the data set
     *
     * @see #setViewImage(ImageView, Int32)
     */
    CARAPI SetViewImage(
        /* [in] */ IImageView* v,
        /* [in] */ const String& value);

    /**
     * Called by bindView() to set the text for a TextView but only if
     * there is no existing ViewBinder or if the existing ViewBinder cannot
     * handle binding to an TextView.
     *
     * @param v TextView to receive text
     * @param text the text to be set for the TextView
     */
    CARAPI SetViewText(
        /* [in] */ ITextView* v,
        /* [in] */ const String& text);

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

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__CSIMPLEADAPTER_H__
