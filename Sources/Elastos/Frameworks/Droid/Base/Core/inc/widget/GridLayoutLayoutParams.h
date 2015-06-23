#ifndef __GRIDLAYOUTLAYOUTPARAMS_H__
#define __GRIDLAYOUTLAYOUTPARAMS_H__

#include "ext/frameworkext.h"
#include "widget/GridLayout.h"
#include "view/ViewGroupMarginLayoutParams.h"
#include <R.h>

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::ViewGroupMarginLayoutParams;
using Elastos::Droid::View::IViewGroupMarginLayoutParams;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::Widget::ISpec;

namespace Elastos {
namespace Droid {
namespace Widget {

class GridLayoutLayoutParams : public ViewGroupMarginLayoutParams
{
private:
    GridLayoutLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [in] */ GridLayout::Spec* rowSpec,
        /* [in] */ GridLayout::Spec* columnSpec);

    CARAPI_(void) ReInitSuper(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

public:
    GridLayoutLayoutParams(
        /* [in] */ ISpec* rowSpec,
        /* [in] */ ISpec* columnSpec);

    GridLayoutLayoutParams();

    GridLayoutLayoutParams(
        /* [in] */ ViewGroupLayoutParams* params);

    GridLayoutLayoutParams(
        /* [in] */ ViewGroupMarginLayoutParams* params);

    GridLayoutLayoutParams(
        /* [in] */ GridLayoutLayoutParams* that);

    GridLayoutLayoutParams(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

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

    CARAPI Init(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [in] */ GridLayout::Spec* rowSpec,
        /* [in] */ GridLayout::Spec* columnSpec);

    CARAPI Init();

    CARAPI Init(
        /* [in] */ ISpec* rowSpec,
        /* [in] */ ISpec* columnSpec);

    CARAPI Init(
        /* [in] */ IViewGroupLayoutParams* params);

    CARAPI Init(
        /* [in] */ IViewGroupMarginLayoutParams* params);

    CARAPI Init(
        /* [in] */ IGridLayoutLayoutParams* that);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

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
    static const Int32 ROW = R::styleable::GridLayout_Layout_layout_row;
    static const Int32 ROW_SPAN = R::styleable::GridLayout_Layout_layout_rowSpan;
    static const Int32 GRAVITY = R::styleable::GridLayout_Layout_layout_gravity;

public:
    AutoPtr<ISpec> mRowSpec;
    AutoPtr<ISpec> mColumnSpec;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__GRIDLAYOUTLAYOUTPARAMS_H__




