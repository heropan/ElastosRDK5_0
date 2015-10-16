
#ifndef __ELASTOS_DROID_WIDGET_ABSLISTVIEWLAYOUTPARAMS_H__
#define __ELASTOS_DROID_WIDGET_ABSLISTVIEWLAYOUTPARAMS_H__

#include "elastos/droid/view/ViewGroupLayoutParams.h"
#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::View::ViewGroupLayoutParams;
using Elastos::Droid::View::IViewGroupLayoutParams;

namespace Elastos {
namespace Droid {
namespace Widget {

class AbsListViewLayoutParams : public ViewGroupLayoutParams
{
public:
    AbsListViewLayoutParams();

    /**
     * {@inheritDoc}
     */
    AbsListViewLayoutParams(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    /**
     * {@inheritDoc}
     */
    AbsListViewLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    /**
     * Creates a new set of layout parameters with the specified width, height
     * and weight.
     *
     * @param width the width, either {@link #MATCH_PARENT},
     *        {@link #WRAP_CONTENT} or a fixed size in pixels
     * @param height the height, either {@link #MATCH_PARENT},
     *        {@link #WRAP_CONTENT} or a fixed size in pixels
     * @param gravity the gravity
     *
     * @see android.view.Gravity
     */
    AbsListViewLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 viewType);

    /**
     * {@inheritDoc}
     */
    AbsListViewLayoutParams(
        /* [in] */ ViewGroupLayoutParams* source);

    CARAPI Init(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI Init(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 viewType);

    CARAPI Init(
        /* [in] */ IViewGroupLayoutParams* source);

    CARAPI SetItemId(
        /* [in] */ Int32 itemId);

    CARAPI SetScrappedFromPosition(
        /* [in] */ Int32 scrappedFromPosition);

    CARAPI SetForceAdd(
        /* [in] */ Boolean force);

    CARAPI SetRecycledHeaderFooter(
        /* [in] */ Boolean footer);

    CARAPI SetViewType(
        /* [in] */ Int32 type);

    CARAPI GetItemId(
        /* [out] */ Int32* itemId);

    CARAPI GetScrappedFromPosition(
        /* [out] */ Int32* scrappedFromPosition);

    CARAPI GetForceAdd(
        /* [in] */ Boolean* force);

    CARAPI GetRecycledHeaderFooter(
        /* [in] */ Boolean* footer);

    CARAPI GetViewType(
        /* [in] */ Int32* type);
public:
    /**
     * View type for this view, as returned by
     * {@link android.widget.Adapter#getItemViewType(int) }
     */
    Int32 mViewType;

    /**
     * When this boolean is set, the view has been added to the AbsListView
     * at least once. It is used to know whether headers/footers have already
     * been added to the list view and whether they should be treated as
     * recycled views or not.
     */
    Boolean mRecycledHeaderFooter;

    Boolean mForceAdd;

    /**
     * The position the view was removed from when pulled out of the
     * scrap heap.
     * @hide
     */
    Int32 mScrappedFromPosition;

    /**
     * The ID the view represents
     */
    Int64 mItemId;
};

}// namespace Elastos
}// namespace Droid
}// namespace Widget
#endif // __ELASTOS_DROID_WIDGET_ABSLISTVIEWLAYOUTPARAMS_H__
