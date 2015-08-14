
#ifndef __ELASTOS_DROID_WIDGET_TABLEROWLAYOUTPARAMS_H__
#define __ELASTOS_DROID_WIDGET_TABLEROWLAYOUTPARAMS_H__

#include "widget/LinearLayoutLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace Widget {

extern "C" const InterfaceID EIID_TableRowLayoutParams;

/**
 * <p>Set of layout parameters used in table rows.</p>
 *
 * @see android.widget.TableLayout.LayoutParams
 *
 * @attr ref android.R.styleable#TableRow_Cell_layout_column
 * @attr ref android.R.styleable#TableRow_Cell_layout_span
 */
class TableRowLayoutParams : public LinearLayoutLayoutParams
{
    friend class TableRow;

public:
    /**
     * {@inheritDoc}
     */
    TableRowLayoutParams(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    /**
     * <p>Sets the child width and the child height.</p>
     *
     * @param w the desired width
     * @param h the desired height
     */
    TableRowLayoutParams(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    /**
     * <p>Sets the child width, height and weight.</p>
     *
     * @param w the desired width
     * @param h the desired height
     * @param initWeight the desired weight
     */
    TableRowLayoutParams(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Float initWeight);

    /**
     * <p>Sets the child width to {@link android.view.ViewGroup.LayoutParams}
     * and the child height to
     * {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}.</p>
     */
    TableRowLayoutParams();

    /**
     * <p>Puts the view in the specified column.</p>
     *
     * <p>Sets the child width to {@link android.view.ViewGroup.LayoutParams#MATCH_PARENT}
     * and the child height to
     * {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}.</p>
     *
     * @param column the column index for the view
     */
    TableRowLayoutParams(
        /* [in] */ Int32 column);

    /**
     * {@inheritDoc}
     */
    TableRowLayoutParams(
        /* [in] */ ViewGroupLayoutParams* p);

    /**
     * {@inheritDoc}
     */
    TableRowLayoutParams(
        /* [in] */ ViewGroupMarginLayoutParams* source);

    virtual CARAPI SetColumn(
        /* [in] */ Int32 column);

    CARAPI_(Int32) GetColumn();

    virtual CARAPI SetSpan(
        /* [in] */ Int32 span);

    CARAPI_(Int32) GetSpan();

protected:
    CARAPI Init(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    CARAPI Init(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Float initWeight);

    CARAPI Init();

    CARAPI Init(
        /* [in] */ Int32 column);

    CARAPI Init(
        /* [in] */ IViewGroupLayoutParams* p);

    CARAPI Init(
        /* [in] */ IViewGroupMarginLayoutParams* source);

    //@Override
    virtual CARAPI SetBaseAttributes(
        /* [in] */ ITypedArray* a,
        /* [in] */ Int32 widthAttr,
        /* [in] */ Int32 heightAttr);

private:
    CARAPI InitFromAttributes(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

public:
    /**
     * <p>The column index of the cell represented by the widget.</p>
     */
    Int32 mColumn;

    /**
     * <p>The number of columns the widgets spans over.</p>
     */
    Int32 mSpan;

private:
    static const Int32 LOCATION = 0;
    static const Int32 LOCATION_NEXT = 1;

    Int32 mOffset[2];
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif
