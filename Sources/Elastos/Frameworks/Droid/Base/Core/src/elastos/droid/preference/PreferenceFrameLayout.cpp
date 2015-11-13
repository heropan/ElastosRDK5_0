
#include "elastos/droid/preference/PreferenceFrameLayout.h"
#include "elastos/droid/R.h"

using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Widget::EIID_IFrameLayoutLayoutParams;

namespace Elastos {
namespace Droid {
namespace Preference {

//====================================================
// PreferenceFrameLayout::LayoutParams
//====================================================

PreferenceFrameLayout::LayoutParams::LayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    //: FrameLayoutLayoutParams(c, attrs)
    //, mRemoveBorders(FALSE)
    : mRemoveBorders(FALSE)
{
    AutoPtr<ITypedArray> a;
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::PreferenceFrameLayout_Layout),
            ARRAY_SIZE(R::styleable::PreferenceFrameLayout_Layout));
    c->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a);
    a->GetBoolean(R::styleable::PreferenceFrameLayout_Layout_layout_removeBorders,
            FALSE, &mRemoveBorders);
    a->Recycle();
}

PreferenceFrameLayout::LayoutParams::LayoutParams(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
    //: FrameLayoutLayoutParams(width, height)
{}

//CAR_INTERFACE_IMPL(PreferenceFrameLayout::LayoutParams, FrameLayoutLayoutParams, IFrameLayoutLayoutParams)
CAR_INTERFACE_IMPL(PreferenceFrameLayout::LayoutParams, Object, IFrameLayoutLayoutParams)

// IVIEWGROUPLP_METHODS_IMPL(PreferenceFrameLayout::LayoutParams, FrameLayoutLayoutParams)
// IVIEWGROUPMARGINLP_METHODS_IMPL(PreferenceFrameLayout::LayoutParams, FrameLayoutLayoutParams)

ECode PreferenceFrameLayout::LayoutParams::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity)
    // *gravity = FrameLayoutLayoutParams::GetGravity();
    return NOERROR;
}

ECode PreferenceFrameLayout::LayoutParams::SetGravity(
    /* [in] */ Int32 gravity)
{
    // return FrameLayoutLayoutParams::SetGravity(gravity);
    return NOERROR;
}


//====================================================
// PreferenceFrameLayout
//====================================================

const Int32 PreferenceFrameLayout::DEFAULT_BORDER_TOP;
const Int32 PreferenceFrameLayout::DEFAULT_BORDER_BOTTOM;
const Int32 PreferenceFrameLayout::DEFAULT_BORDER_LEFT;
const Int32 PreferenceFrameLayout::DEFAULT_BORDER_RIGHT;

// CAR_INTERFACE_IMPL_7(PreferenceFrameLayout,FrameLayout,IView,IViewGroup,IViewParent,IViewManager,IDrawableCallback,IKeyEventCallback,IAccessibilityEventSource)
// CAR_INTERFACE_IMPL_8(PreferenceFrameLayout,Object,IFrameLayout,IView,IViewGroup,IViewParent,IViewManager,IDrawableCallback,IKeyEventCallback,IAccessibilityEventSource)
CAR_INTERFACE_IMPL(PreferenceFrameLayout, Object, IPreferenceFrameLayout)
// IVIEW_METHODS_IMPL(PreferenceFrameLayout, FrameLayout)
// IVIEWPARENT_METHODS_IMPL(PreferenceFrameLayout, FrameLayout)
// IVIEWMANAGER_METHODS_IMPL(PreferenceFrameLayout, FrameLayout)
// IDRAWABLECALLBACK_METHODS_IMPL(PreferenceFrameLayout, FrameLayout)
// IKEYEVENTCALLBACK_METHODS_IMPL(PreferenceFrameLayout, FrameLayout)
// IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(PreferenceFrameLayout, FrameLayout)

PreferenceFrameLayout::PreferenceFrameLayout()
    : mBorderTop(0)
    , mBorderBottom(0)
    , mBorderLeft(0)
    , mBorderRight(0)
    , mPaddingApplied(FALSE)
{}

ECode PreferenceFrameLayout::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL, R::attr::preferenceFrameLayoutStyle, 0);
}

ECode PreferenceFrameLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, R::attr::preferenceFrameLayoutStyle, 0);
}

ECode PreferenceFrameLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return constructor(context, attrs, defStyleAttr, 0);
}

ECode PreferenceFrameLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    // FAIL_RETURN(FrameLayout::constructor(context, attrs, defStyleAttr, defStyleRes));

    AutoPtr<ITypedArray> a;
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::PreferenceFrameLayout),
            ARRAY_SIZE(R::styleable::PreferenceFrameLayout));
    context->ObtainStyledAttributes(attrs, attrIds, defStyleAttr, defStyleRes, (ITypedArray**)&a);

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IDisplayMetrics> dis;
    res->GetDisplayMetrics((IDisplayMetrics**)&dis);
    Float density;
    dis->GetDensity(&density);

    Int32 defaultBorderTop = (Int32)(density * DEFAULT_BORDER_TOP + 0.5f);
    Int32 defaultBottomPadding = (Int32)(density * DEFAULT_BORDER_BOTTOM + 0.5f);
    Int32 defaultLeftPadding = (Int32)(density * DEFAULT_BORDER_LEFT + 0.5f);
    Int32 defaultRightPadding = (Int32)(density * DEFAULT_BORDER_RIGHT + 0.5f);

    a->GetDimensionPixelSize(
            R::styleable::PreferenceFrameLayout_borderTop,
            defaultBorderTop, &mBorderTop);
    a->GetDimensionPixelSize(
            R::styleable::PreferenceFrameLayout_borderBottom,
            defaultBottomPadding, &mBorderBottom);
    a->GetDimensionPixelSize(
            R::styleable::PreferenceFrameLayout_borderLeft,
            defaultLeftPadding, &mBorderLeft);
    a->GetDimensionPixelSize(
            R::styleable::PreferenceFrameLayout_borderRight,
            defaultRightPadding, &mBorderRight);

    a->Recycle();
    return NOERROR;
}

ECode PreferenceFrameLayout::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params)
    // AutoPtr<IContext> ctx;
    // GetContext((IContext**)&ctx);
    // AutoPtr<IViewGroupLayoutParams> lp = new LayoutParams(ctx, attrs);
    // *params = lp;
    // REFCOUNT_ADD(*params)
    return NOERROR;
}

ECode PreferenceFrameLayout::AddView(
    /* [in] */ IView* child)
{
    // Int32 borderTop, borderBottom, borderLeft, borderRight;
    // GetPaddingTop(&borderTop);
    // GetPaddingBottom(&borderBottom);
    // GetPaddingLeft(&borderLeft);
    // GetPaddingRight(&borderRight);

    // AutoPtr<IViewGroupLayoutParams> params;
    // child->GetLayoutParams((IViewGroupLayoutParams**)&params);
    // AutoPtr<LayoutParams> layoutParams;
    // if (params && params->Probe(EIID_PreferenceFrameLayoutParams) != NULL) {
    //     layoutParams = reinterpret_cast<LayoutParams*>(params->Probe(EIID_PreferenceFrameLayoutParams));
    // }

    // // Check on the id of the child before adding it.
    // if (layoutParams != NULL && layoutParams->mRemoveBorders) {
    //     if (mPaddingApplied) {
    //         borderTop -= mBorderTop;
    //         borderBottom -= mBorderBottom;
    //         borderLeft -= mBorderLeft;
    //         borderRight -= mBorderRight;
    //         mPaddingApplied = FALSE;
    //     }
    // }
    // else {
    //     // Add the padding to the view group after determining if the
    //     // padding already exists.
    //     if (!mPaddingApplied) {
    //         borderTop += mBorderTop;
    //         borderBottom += mBorderBottom;
    //         borderLeft += mBorderLeft;
    //         borderRight += mBorderRight;
    //         mPaddingApplied = TRUE;
    //     }
    // }

    // Int32 previousTop, previousBottom, previousLeft, previousRight;
    // GetPaddingTop(&previousTop);
    // GetPaddingBottom(&previousBottom);
    // GetPaddingLeft(&previousLeft);
    // GetPaddingRight(&previousRight);
    // if (previousTop != borderTop || previousBottom != borderBottom
    //         || previousLeft != borderLeft || previousRight != borderRight) {
    //     SetPadding(borderLeft, borderTop, borderRight, borderBottom);
    // }

    // return FrameLayout::AddView(child);
    return NOERROR;
}

} // Preference
} // Droid
} // Elastos
