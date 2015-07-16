#include "ViewSwitcher.h"
#ifdef DROID_CORE
#include "CFrameLayoutLayoutParams.h"
#endif

using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Droid {
namespace Widget {

ViewSwitcher::ViewSwitcher()
{}

ViewSwitcher::ViewSwitcher(
    /* [in] */ IContext* context) : ViewAnimator(context)
{}

/**
 * Creates a new empty ViewSwitcher for the given context and with the
 * specified set attributes.
 *
 * @param context the application environment
 * @param attrs a collection of attributes
 */
ViewSwitcher::ViewSwitcher(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs) : ViewAnimator(context, attrs)
{}

/**
 * {@inheritDoc}
 *
 * @throws IllegalStateException if this switcher already contains two children
 */
ECode ViewSwitcher::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (GetChildCount() >= 2) {
//        throw new IllegalStateException("Can't add more than 2 views to a ViewSwitcher");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return ViewAnimator::AddView(child, index, params);
}

ECode ViewSwitcher::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    ViewAnimator::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(String("CViewSwitcher"), (ICharSequence**)&cs);
    event->SetClassName(cs);
    return NOERROR;
}

ECode ViewSwitcher::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    ViewAnimator::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(String("CViewSwitcher"), (ICharSequence**)&cs);
    info->SetClassName(cs);
    return NOERROR;
}

/**
 * Returns the next view to be displayed.
 *
 * @return the view that will be displayed after the next views flip.
 */
ECode ViewSwitcher::GetNextView(
    /* [out] */ IView** v)
{
    Int32 which = mWhichChild == 0 ? 1 : 0;
    AutoPtr<IView> temp = GetChildAt(which);
    *v = temp;
    REFCOUNT_ADD(*v);
    return NOERROR;
}



/**
 * Sets the factory used to create the two views between which the
 * ViewSwitcher will flip. Instead of using a factory, you can call
 * {@link #addView(android.view.View, int, android.view.ViewGroup.LayoutParams)}
 * twice.
 *
 * @param factory the view factory used to generate the switcher's content
 */
ECode ViewSwitcher::SetFactory(
    /* [in] */ IViewFactory* factory)
{
    mFactory = factory;
    AutoPtr<IView> temp;
    ObtainView((IView**)&temp);
    temp = NULL;
    ObtainView((IView**)&temp);
    return NOERROR;
}

/**
 * Reset the ViewSwitcher to hide all of the existing views and to make it
 * think that the first time animation has not yet played.
 */
ECode ViewSwitcher::Reset()
{
    mFirstTime = TRUE;
    AutoPtr<IView> v = GetChildAt(0);
    if (v != NULL) {
        v->SetVisibility(IView::GONE);
    }
    v = GetChildAt(1);
    if (v != NULL) {
        v->SetVisibility(IView::GONE);
    }
    return NOERROR;
}

ECode ViewSwitcher::ObtainView(
    /* [out] */ IView** v)
{
    AutoPtr<IView> child;
    mFactory->MakeView((IView**)&child);
    AutoPtr<IFrameLayoutLayoutParams> lp;
    AutoPtr<IViewGroupLayoutParams> temp;
    child->GetLayoutParams((IViewGroupLayoutParams**)&temp);
    if (temp) {
        lp = (IFrameLayoutLayoutParams*)temp->Probe(EIID_IFrameLayoutLayoutParams);
    }
    if (lp == NULL) {
        CFrameLayoutLayoutParams::New(IViewGroupLayoutParams::MATCH_PARENT,
                IViewGroupLayoutParams::WRAP_CONTENT, (IFrameLayoutLayoutParams**)&lp);
    }
    AddView(child, lp);
    *v = child;
    REFCOUNT_ADD(*v);
    return NOERROR;
}

ECode ViewSwitcher::Init(
    /* [in] */ IContext* context)
{
    return ViewAnimator::Init(context);
}

ECode ViewSwitcher::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ViewAnimator::Init(context, attrs);
}

}// namespace Elastos
}// namespace Droid
}// namespace Widget
