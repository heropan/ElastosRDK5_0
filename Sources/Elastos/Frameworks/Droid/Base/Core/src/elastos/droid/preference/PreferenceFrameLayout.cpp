
#include "PreferenceFrameLayout.h"
#include "R.h"

using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::EIID_IViewParent;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::EIID_IKeyEventCallback;
using Elastos::Droid::View::EIID_IViewGroupMarginLayoutParams;
using Elastos::Droid::View::EIID_IViewGroupLayoutParams;
using Elastos::Droid::View::EIID_IViewManager;
using Elastos::Droid::View::Accessibility::EIID_IAccessibilityEventSource;
using Elastos::Droid::Widget::EIID_IFrameLayout;
using Elastos::Droid::Widget::EIID_IFrameLayoutLayoutParams;

namespace Elastos {
namespace Droid {
namespace Preference {

//====================================================
// PreferenceFrameLayout::LayoutParams
//====================================================

extern "C" const InterfaceID EIID_PreferenceFrameLayoutParams =
        { 0x1a09ea20, 0xcb66, 0x4d5e, { 0xac, 0x1, 0xf3, 0x85, 0xad, 0xe3, 0xdc, 0xa2 } };

PreferenceFrameLayout::LayoutParams::LayoutParams(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
    : FrameLayoutLayoutParams(c, attrs)
    , mRemoveBorders(FALSE)
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
    : FrameLayoutLayoutParams(width, height)
{}

PInterface PreferenceFrameLayout::LayoutParams::Probe(
    /* [in] */REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IFrameLayoutLayoutParams*)this;
    }
    else if (riid == EIID_IFrameLayoutLayoutParams) {
        return (IFrameLayoutLayoutParams*)this;
    }
    else if (riid == EIID_IViewGroupMarginLayoutParams) {
        return (IViewGroupMarginLayoutParams*)this;
    }
    else if (riid == EIID_IViewGroupLayoutParams) {
        return (IViewGroupLayoutParams*)this;
    }
    else if (riid == EIID_PreferenceFrameLayoutParams) {
        return reinterpret_cast<PInterface>(this);;
    }

    return NULL;
}

UInt32 PreferenceFrameLayout::LayoutParams::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 PreferenceFrameLayout::LayoutParams::Release()
{
    return ElRefBase::Release();
}

ECode PreferenceFrameLayout::LayoutParams::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IFrameLayoutLayoutParams*)this) {
        *pIID = EIID_IFrameLayoutLayoutParams;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IViewGroupMarginLayoutParams*)this) {
        *pIID = EIID_IViewGroupMarginLayoutParams;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IViewGroupLayoutParams*)this) {
        *pIID = EIID_IViewGroupLayoutParams;
        return NOERROR;
    }
    return E_INVALID_ARGUMENT;
}

IVIEWGROUPLP_METHODS_IMPL(PreferenceFrameLayout::LayoutParams, FrameLayoutLayoutParams)
IVIEWGROUPMARGINLP_METHODS_IMPL(PreferenceFrameLayout::LayoutParams, FrameLayoutLayoutParams)

PreferenceFrameLayout::LayoutParams::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity)
    *gravity = FrameLayoutLayoutParams::GetGravity();
    return NOERROR;
}

PreferenceFrameLayout::LayoutParams::SetGravity(
    /* [in] */ Int32 gravity)
{
    return FrameLayoutLayoutParams::SetGravity(gravity);
}


//====================================================
// PreferenceFrameLayout
//====================================================

const Int32 PreferenceFrameLayout::DEFAULT_BORDER_TOP;
const Int32 PreferenceFrameLayout::DEFAULT_BORDER_BOTTOM;
const Int32 PreferenceFrameLayout::DEFAULT_BORDER_LEFT;
const Int32 PreferenceFrameLayout::DEFAULT_BORDER_RIGHT;

PreferenceFrameLayout::PreferenceFrameLayout(
    /* [in] */ IContext* context)
    : mBorderTop(0)
    , mBorderBottom(0)
    , mBorderLeft(0)
    , mBorderRight(0)
    , mPaddingApplied(FALSE)
{
    Init(context, NULL, R::attr::preferenceFrameLayoutStyle);
}

PreferenceFrameLayout::PreferenceFrameLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mBorderTop(0)
    , mBorderBottom(0)
    , mBorderLeft(0)
    , mBorderRight(0)
    , mPaddingApplied(FALSE)
{
    Init(context, attrs, R::attr::preferenceFrameLayoutStyle);
}

PreferenceFrameLayout::PreferenceFrameLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : mBorderTop(0)
    , mBorderBottom(0)
    , mBorderLeft(0)
    , mBorderRight(0)
    , mPaddingApplied(FALSE)
{
    Init(context, attrs, defStyle);
}

void PreferenceFrameLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FrameLayout::Init(context, attrs, defStyle);

    AutoPtr<ITypedArray> a;
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::PreferenceFrameLayout),
            ARRAY_SIZE(R::styleable::PreferenceFrameLayout));
    context->ObtainStyledAttributes(attrs, attrIds, defStyle, 0, (ITypedArray**)&a);

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
}

PInterface PreferenceFrameLayout::Probe(
    /* [in] */REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IFrameLayout*)this;
    }
    else if (riid == EIID_IFrameLayout) {
        return (IFrameLayout*)this;
    }
    else if (riid == EIID_IViewGroup) {
        return (IViewGroup*)this;
    }
    else if (riid == EIID_IView) {
        return (IView*)this;
    }
    else if (riid == EIID_IViewParent) {
        return (IViewParent*)this;
    }
    else if (riid == EIID_IViewManager) {
        return (IViewManager*)this;
    }
    else if (riid == EIID_IDrawableCallback) {
        return (IDrawableCallback*)this;
    }
    else if (riid == EIID_IKeyEventCallback) {
        return (IKeyEventCallback*)this;
    }
    else if (riid == EIID_IAccessibilityEventSource) {
        return (IAccessibilityEventSource*)this;
    }

    return NULL;
}

UInt32 PreferenceFrameLayout::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 PreferenceFrameLayout::Release()
{
    return ElRefBase::Release();
}

ECode PreferenceFrameLayout::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IFrameLayout*)this) {
        *pIID = EIID_IFrameLayout;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IViewGroup*)this) {
        *pIID = EIID_IViewGroup;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IView*)this) {
        *pIID = EIID_IView;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IViewParent*)this) {
        *pIID = EIID_IViewParent;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IViewManager*)this) {
        *pIID = EIID_IViewManager;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IDrawableCallback*)this) {
        *pIID = EIID_IDrawableCallback;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IKeyEventCallback*)this) {
        *pIID = EIID_IKeyEventCallback;
        return NOERROR;
    }
    else if (pObject == (IInterface*)(IAccessibilityEventSource*)this) {
        *pIID = EIID_IAccessibilityEventSource;
        return NOERROR;
    }
    return E_INVALID_ARGUMENT;
}

IVIEW_METHODS_IMPL(PreferenceFrameLayout, FrameLayout)
IVIEWPARENT_METHODS_IMPL(PreferenceFrameLayout, FrameLayout)
IVIEWMANAGER_METHODS_IMPL(PreferenceFrameLayout, FrameLayout)
IDRAWABLECALLBACK_METHODS_IMPL(PreferenceFrameLayout, FrameLayout)
IKEYEVENTCALLBACK_METHODS_IMPL(PreferenceFrameLayout, FrameLayout)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(PreferenceFrameLayout, FrameLayout)

ECode PreferenceFrameLayout::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params)
    AutoPtr<IContext> ctx;
    GetContext((IContext**)&ctx);
    AutoPtr<IViewGroupLayoutParams> lp = new LayoutParams(ctx, attrs);
    *params = lp;
    REFCOUNT_ADD(*params)
    return NOERROR;
}

ECode PreferenceFrameLayout::AddView(
    /* [in] */ IView* child)
{
    Int32 borderTop, borderBottom, borderLeft, borderRight;
    GetPaddingTop(&borderTop);
    GetPaddingBottom(&borderBottom);
    GetPaddingLeft(&borderLeft);
    GetPaddingRight(&borderRight);

    AutoPtr<IViewGroupLayoutParams> params;
    child->GetLayoutParams((IViewGroupLayoutParams**)&params);
    AutoPtr<LayoutParams> layoutParams;
    if (params && params->Probe(EIID_PreferenceFrameLayoutParams) != NULL) {
        layoutParams = reinterpret_cast<LayoutParams*>(params->Probe(EIID_PreferenceFrameLayoutParams));
    }

    // Check on the id of the child before adding it.
    if (layoutParams != NULL && layoutParams->mRemoveBorders) {
        if (mPaddingApplied) {
            borderTop -= mBorderTop;
            borderBottom -= mBorderBottom;
            borderLeft -= mBorderLeft;
            borderRight -= mBorderRight;
            mPaddingApplied = FALSE;
        }
    }
    else {
        // Add the padding to the view group after determining if the
        // padding already exists.
        if (!mPaddingApplied) {
            borderTop += mBorderTop;
            borderBottom += mBorderBottom;
            borderLeft += mBorderLeft;
            borderRight += mBorderRight;
            mPaddingApplied = TRUE;
        }
    }

    Int32 previousTop, previousBottom, previousLeft, previousRight;
    GetPaddingTop(&previousTop);
    GetPaddingBottom(&previousBottom);
    GetPaddingLeft(&previousLeft);
    GetPaddingRight(&previousRight);
    if (previousTop != borderTop || previousBottom != borderBottom
            || previousLeft != borderLeft || previousRight != borderRight) {
        SetPadding(borderLeft, borderTop, borderRight, borderBottom);
    }

    return FrameLayout::AddView(child);
}

ECode PreferenceFrameLayout::GetForegroundGravity(
    /* [out] */ Int32* foregroundGravity)
{
    VALIDATE_NOT_NULL(foregroundGravity)
    *foregroundGravity  = FrameLayout::GetForegroundGravity();
    return NOERROR;
}

ECode PreferenceFrameLayout::SetForegroundGravity(
    /* [in] */ Int32 foregroundGravity)
{
    return FrameLayout::SetForegroundGravity(foregroundGravity);
}

ECode PreferenceFrameLayout::SetForeground(
    /* [in] */ IDrawable* drawable)
{
    return FrameLayout::SetForeground(drawable);
}

ECode PreferenceFrameLayout::GetForeground(
    /* [out] */ IDrawable** foreground)
{
    VALIDATE_NOT_NULL(foreground)
    AutoPtr<IDrawable> g = FrameLayout::GetForeground();
    *foreground = g;
    REFCOUNT_ADD(*foreground)
    return NOERROR;
}

ECode PreferenceFrameLayout::GetMeasureAllChildren(
    /* [out] */ Boolean* measureAll)
{
    VALIDATE_NOT_NULL(measureAll)
    *measureAll  = FrameLayout::GetMeasureAllChildren();
    return NOERROR;
}

ECode PreferenceFrameLayout::SetMeasureAllChildren(
    /* [in] */ Boolean measureAll)
{
    return FrameLayout::SetMeasureAllChildren(measureAll);
}

ECode PreferenceFrameLayout::GetConsiderGoneChildrenWhenMeasuring(
    /* [out] */ Boolean* consider)
{
    VALIDATE_NOT_NULL(consider)
    *consider  = FrameLayout::GetConsiderGoneChildrenWhenMeasuring();
    return NOERROR;
}

ECode PreferenceFrameLayout::GetDescendantFocusability(
    /* [out] */ Int32* focusability)
{
    VALIDATE_NOT_NULL(focusability)
    *focusability = FrameLayout::GetDescendantFocusability();
    return NOERROR;
}

ECode PreferenceFrameLayout::SetDescendantFocusability(
    /* [in] */ Int32 focusability)
{
    return FrameLayout::SetDescendantFocusability(focusability);
}

ECode PreferenceFrameLayout::StartActionModeForChild(
    /* [in] */ IView* originalView,
    /* [in] */ IActionModeCallback* cb,
    /* [out] */ IActionMode** mode)
{
    VALIDATE_NOT_NULL(mode)
    AutoPtr<IActionMode> m = FrameLayout::StartActionModeForChild(originalView, cb);
    *mode = m;
    REFCOUNT_ADD(*mode)
    return NOERROR;
}

ECode PreferenceFrameLayout::OnRequestSendAccessibilityEvent(
    /* [in] */ IView* child,
    /* [in] */ IAccessibilityEvent* event,
    /* [out] */ Boolean* sent)
{
    VALIDATE_NOT_NULL(sent)
    *sent = FrameLayout::OnRequestSendAccessibilityEvent(child, event);
    return NOERROR;
}

ECode PreferenceFrameLayout::ChildHasTransientStateChanged(
    /* [in] */ IView* child,
    /* [in] */ Boolean childHasTransientState)
{
    return FrameLayout::ChildHasTransientStateChanged(child, childHasTransientState);
}

ECode PreferenceFrameLayout::GetFocusedChild(
    /* [out] */ IView** focusedChild)
{
    VALIDATE_NOT_NULL(focusedChild)
    AutoPtr<IView> view = FrameLayout::GetFocusedChild();
    *focusedChild = view;
    REFCOUNT_ADD(*focusedChild)
    return NOERROR;
}

ECode PreferenceFrameLayout::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    *value = FrameLayout::OnInterceptTouchEvent(ev);
    return NOERROR;
}

ECode PreferenceFrameLayout::SetClipChildren(
    /* [in] */ Boolean clipChildren)
{
    FrameLayout::SetClipChildren(clipChildren);
    return NOERROR;
}

ECode PreferenceFrameLayout::SetClipToPadding(
    /* [in] */ Boolean clipToPadding)
{
    FrameLayout::SetClipToPadding(clipToPadding);
    return NOERROR;
}

ECode PreferenceFrameLayout::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index)
{
    return FrameLayout::AddView(child, index);
}

ECode PreferenceFrameLayout::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return FrameLayout::AddView(child, width, height);
}

ECode PreferenceFrameLayout::AddView(
    /* [in] */ IView* child,
    /* [in] */ IViewGroupLayoutParams* params)
{
    return FrameLayout::AddView(child, params);
}

ECode PreferenceFrameLayout::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    return FrameLayout::AddView(child, index, params);
}

ECode PreferenceFrameLayout::SetOnHierarchyChangeListener(
    /* [in] */ IViewGroupOnHierarchyChangeListener* listener)
{
    return FrameLayout::SetOnHierarchyChangeListener(listener);
}

ECode PreferenceFrameLayout::RemoveViewInLayout(
    /* [in] */ IView* view)
{
    return FrameLayout::RemoveViewInLayout(view);
}

ECode PreferenceFrameLayout::RemoveViewsInLayout(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    return FrameLayout::RemoveViewsInLayout(start, count);
}

ECode PreferenceFrameLayout::RemoveViewAt(
    /* [in] */ Int32 index)
{
    return FrameLayout::RemoveViewAt(index);
}

ECode PreferenceFrameLayout::RemoveViews(
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    return FrameLayout::RemoveViews(start, count);
}

ECode PreferenceFrameLayout::RemoveAllViews()
{
    return FrameLayout::RemoveAllViews();
}

ECode PreferenceFrameLayout::RemoveAllViewsInLayout()
{
    return FrameLayout::RemoveAllViewsInLayout();
}

ECode PreferenceFrameLayout::OffsetDescendantRectToMyCoords(
    /* [in] */ IView* descendant,
    /* [in] */ IRect* rect)
{
    return FrameLayout::OffsetDescendantRectToMyCoords(descendant, rect);
}

ECode PreferenceFrameLayout::OffsetRectIntoDescendantCoords(
    /* [in] */ IView* descendant,
    /* [in] */ IRect* rect)
{
    return FrameLayout::OffsetRectIntoDescendantCoords(descendant, rect);
}

ECode PreferenceFrameLayout::OffsetChildrenTopAndBottom(
    /* [in] */ Int32 offset)
{
    return FrameLayout::OffsetChildrenTopAndBottom(offset);
}

ECode PreferenceFrameLayout::StartLayoutAnimation()
{
    return FrameLayout::StartLayoutAnimation();
}

ECode PreferenceFrameLayout::ScheduleLayoutAnimation()
{
    return FrameLayout::ScheduleLayoutAnimation();
}

ECode PreferenceFrameLayout::SetLayoutAnimation(
    /* [in] */ ILayoutAnimationController* controller)
{
    return FrameLayout::SetLayoutAnimation(controller);
}

ECode PreferenceFrameLayout::GetLayoutAnimation(
    /* [out] */ ILayoutAnimationController** controller)
{
    VALIDATE_NOT_NULL(controller)
    AutoPtr<ILayoutAnimationController> ctrl= FrameLayout::GetLayoutAnimation();
    *controller = ctrl;
    REFCOUNT_ADD(*controller)
    return NOERROR;
}

ECode PreferenceFrameLayout::IsAnimationCacheEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled)
    *enabled = FrameLayout::IsAnimationCacheEnabled();
    return NOERROR;
}

ECode PreferenceFrameLayout::SetAnimationCacheEnabled(
    /* [in] */ Boolean enabled)
{
    return FrameLayout::SetAnimationCacheEnabled(enabled);
}

ECode PreferenceFrameLayout::IsAlwaysDrawnWithCacheEnabled(
    /* [out] */ Boolean* always)
{
    VALIDATE_NOT_NULL(always)
    *always = FrameLayout::IsAlwaysDrawnWithCacheEnabled();
    return NOERROR;
}

ECode PreferenceFrameLayout::SetAlwaysDrawnWithCacheEnabled(
    /* [in] */ Boolean always)
{
    return FrameLayout::SetAlwaysDrawnWithCacheEnabled(always);
}

ECode PreferenceFrameLayout::GetPersistentDrawingCache(
    /* [out] */ Int32* drawingCacheToKeep)
{
    VALIDATE_NOT_NULL(drawingCacheToKeep)
    *drawingCacheToKeep = FrameLayout::GetPersistentDrawingCache();
    return NOERROR;
}

ECode PreferenceFrameLayout::SetPersistentDrawingCache(
    /* [in] */ Int32 drawingCacheToKeep)
{
    return FrameLayout::SetPersistentDrawingCache(drawingCacheToKeep);
}

ECode PreferenceFrameLayout::IndexOfChild(
    /* [in] */ IView* child,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    *index = FrameLayout::IndexOfChild(child);
    return NOERROR;
}

ECode PreferenceFrameLayout::GetChildCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    *count = FrameLayout::GetChildCount();
    return NOERROR;
}

ECode PreferenceFrameLayout::GetChildAt(
    /* [in] */ Int32 index,
    /* [out] */ IView** child)
{
    VALIDATE_NOT_NULL(child)
    AutoPtr<IView> _child = ViewGroup::GetChildAt(index);
    *child = _child;
    REFCOUNT_ADD(*child)
    return NOERROR;
}

ECode PreferenceFrameLayout::ClearDisappearingChildren()
{
    return FrameLayout::ClearDisappearingChildren();
}

ECode PreferenceFrameLayout::GetLayoutAnimationListener(
    /* [out] */ IAnimationListener** listener)
{
    VALIDATE_NOT_NULL(listener)
    AutoPtr<IAnimationListener> l = FrameLayout::GetLayoutAnimationListener();
    *listener = l;
    REFCOUNT_ADD(*listener)
    return NOERROR;
}

ECode PreferenceFrameLayout::SetAddStatesFromChildren(
    /* [in] */ Boolean addsStates)
{
    return FrameLayout::SetAddStatesFromChildren(addsStates);
}

ECode PreferenceFrameLayout::AddStatesFromChildren(
    /* [out] */ Boolean* drawable)
{
    VALIDATE_NOT_NULL(drawable)
    *drawable = FrameLayout::AddStatesFromChildren();
    return NOERROR;
}

ECode PreferenceFrameLayout::SetLayoutAnimationListener(
    /* [in] */ IAnimationListener* animationListener)
{
    return FrameLayout::SetLayoutAnimationListener(animationListener);
}

ECode PreferenceFrameLayout::OnInterceptHoverEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* like)
{
    VALIDATE_NOT_NULL(like)
    *like = FrameLayout::OnInterceptHoverEvent(event);
    return NOERROR;
}

ECode PreferenceFrameLayout::SetMotionEventSplittingEnabled(
    /* [in] */ Boolean split)
{
    return FrameLayout::SetMotionEventSplittingEnabled(split);
}

ECode PreferenceFrameLayout::IsMotionEventSplittingEnabled(
    /* [out] */ Boolean* dispatched)
{
    VALIDATE_NOT_NULL(dispatched)
    *dispatched = FrameLayout::IsMotionEventSplittingEnabled();
    return NOERROR;
}

ECode PreferenceFrameLayout::SetLayoutTransition(
    /* [in] */ ILayoutTransition* transition)
{
    return FrameLayout::SetLayoutTransition(transition);
}

ECode PreferenceFrameLayout::GetLayoutTransition(
    /* [out] */ ILayoutTransition** transition)
{
    VALIDATE_NOT_NULL(transition)
    AutoPtr<ILayoutTransition> ransition = FrameLayout::GetLayoutTransition();
    REFCOUNT_ADD(*transition)
    return NOERROR;
}

ECode PreferenceFrameLayout::InvalidateChildFast(
    /* [in] */ IView* child,
    /* [in] */ IRect* dirty)
{
    return FrameLayout::InvalidateChildFast(child, dirty);
}

ECode PreferenceFrameLayout::GetLayoutMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode)
    *mode = FrameLayout::GetLayoutMode();
    return NOERROR;
}

ECode PreferenceFrameLayout::SetLayoutMode(
    /* [in] */ Int32 layoutMode)
{
    return FrameLayout::SetLayoutMode(layoutMode);
}

ECode PreferenceFrameLayout::StartViewTransition(
    /* [in] */ IView* view)
{
    return FrameLayout::StartViewTransition(view);
}

ECode PreferenceFrameLayout::EndViewTransition(
    /* [in] */ IView* view)
{
    return FrameLayout::EndViewTransition(view);
}

ECode PreferenceFrameLayout::RequestTransitionStart(
    /* [in] */ ILayoutTransition* transition)
{
    return FrameLayout::RequestTransitionStart(transition);
}

ECode PreferenceFrameLayout::ShouldDelayChildPressedState(
    /* [out] */ Boolean* compatibility)
{
    VALIDATE_NOT_NULL(compatibility)
    *compatibility = FrameLayout::ShouldDelayChildPressedState();
    return NOERROR;
}

} // Preference
} // Droid
} // Elastos
