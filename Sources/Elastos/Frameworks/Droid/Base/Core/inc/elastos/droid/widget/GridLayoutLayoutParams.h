#ifndef __ELASTOS_DROID_WIDGET_GRIDLAYOUTLAYOUTPARAMS_H__
#define __ELASTOS_DROID_WIDGET_GRIDLAYOUTLAYOUTPARAMS_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/GridLayout.h"
#include "elastos/droid/view/ViewGroupMarginLayoutParams.h"
#include <elastos/droid/R.h>

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::ViewGroupMarginLayoutParams;
using Elastos::Droid::View::IViewGroupMarginLayoutParams;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::Widget::IGridLayoutSpec;

namespace Elastos {
namespace Droid {
namespace Widget {

class GridLayoutLayoutParams
    : public ViewGroupMarginLayoutParams
    , public IGridLayoutLayoutParams
{
private:
    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [in] */ IGridLayoutSpec* rowSpec,
        /* [in] */ IGridLayoutSpec* columnSpec);

    CARAPI_(void) ReInitSuper(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

public:
    CAR_INTERFACE_DECL()

    GridLayoutLayoutParams();

    CARAPI constructor(
        /* [in] */ IGridLayoutSpec* rowSpec,
        /* [in] */ IGridLayoutSpec* columnSpec);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* params);

    CARAPI constructor(
        /* [in] */ IViewGroupMarginLayoutParams* params);

    CARAPI constructor(
        /* [in] */ IGridLayoutLayoutParams* that);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI SetRowSpec(
        /* [in] */ IGridLayoutSpec* r);

    CARAPI SetColumnSpec(
        /* [in] */ IGridLayoutSpec* c);

    CARAPI GetRowSpec(
        /* [out] */ IGridLayoutSpec** r);

    CARAPI GetColumnSpec(
        /* [out] */ IGridLayoutSpec** c);

    CARAPI_(void) SetRowSpecSpan(
        /* [in] */ GridLayout::Interval* span);

    CARAPI_(void) SetColumnSpecSpan(
        /* [in] */ GridLayout::Interval* span);

    virtual CARAPI Equals(
        /* [in] */ IInterface* o,
        /* [out] */ Boolean* result);

    virtual CARAPI GetHashCode(
        /* [in] */ Int32* hash);

protected:
    virtual CARAPI SetBaseAttributes(
        /* [in] */ ITypedArray* attributes,
        /* [in] */ Int32 widthAttr,
        /* [in] */ Int32 heightAttr);

private:
    static const Int32 DEFAULT_WIDTH = IViewGroupLayoutParams::WRAP_CONTENT;
    static const Int32 DEFAULT_HEIGHT = IViewGroupLayoutParams::WRAP_CONTENT;
    static const Int32 DEFAULT_MARGIN;// = Elastos::Core::Math::INT32_MIN_VALUE;
    static const Int32 DEFAULT_ROW ;// Elastos::Core::Math::INT32_MIN_VALUE;
    static const Int32 DEFAULT_COLUMN;// = Elastos::Core::Math::INT32_MIN_VALUE;
    static const AutoPtr<GridLayout::Interval> DEFAULT_SPAN;
    static const Int32 DEFAULT_SPAN_SIZE;

    static const Int32 MARGIN = R::styleable::ViewGroup_MarginLayout_layout_margin;
    static const Int32 LEFT_MARGIN = R::styleable::ViewGroup_MarginLayout_layout_marginLeft;
    static const Int32 TOP_MARGIN = R::styleable::ViewGroup_MarginLayout_layout_marginTop;
    static const Int32 RIGHT_MARGIN = R::styleable::ViewGroup_MarginLayout_layout_marginRight;
    static const Int32 BOTTOM_MARGIN = R::styleable::ViewGroup_MarginLayout_layout_marginBottom;
    static const Int32 COLUMN = R::styleable::GridLayout_Layout_layout_column;
    static const Int32 COLUMN_SPAN = R::styleable::GridLayout_Layout_layout_columnSpan;
    static const Int32 COLUMN_WEIGHT = R::styleable::GridLayout_Layout_layout_columnWeight;
    static const Int32 ROW = R::styleable::GridLayout_Layout_layout_row;
    static const Int32 ROW_SPAN = R::styleable::GridLayout_Layout_layout_rowSpan;
    static const Int32 ROW_WEIGHT = R::styleable::GridLayout_Layout_layout_rowWeight;
    static const Int32 GRAVITY = R::styleable::GridLayout_Layout_layout_gravity;

public:
    AutoPtr<IGridLayoutSpec> mRowSpec;
    AutoPtr<IGridLayoutSpec> mColumnSpec;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_GRIDLAYOUTLAYOUTPARAMS_H__




