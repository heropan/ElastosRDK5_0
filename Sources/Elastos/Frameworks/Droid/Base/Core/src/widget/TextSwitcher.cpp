
#include "widget/TextSwitcher.h"

using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Droid {
namespace Widget {

    /**
     * Creates a new empty TextSwitcher.
     *
     * @param context the application's environment
     */
TextSwitcher::TextSwitcher()
{}
TextSwitcher::TextSwitcher(
    /* [in] */ IContext* context) : ViewSwitcher(context)
{}

    /**
     * Creates a new empty TextSwitcher for the given context and with the
     * specified set attributes.
     *
     * @param context the application environment
     * @param attrs a collection of attributes
     */
TextSwitcher::TextSwitcher(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs) : ViewSwitcher(context, attrs)
{}

    /**
     * {@inheritDoc}
     *
     * @throws IllegalArgumentException if child is not an instance of
     *         {@link android.widget.TextView}
     */

ECode TextSwitcher::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (child->Probe(EIID_ITextView) == NULL) {
//        throw new IllegalArgumentException(
//                "TextSwitcher children must be instances of TextView");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return ViewSwitcher::AddView(child, index, params);
}
    /**
     * Sets the text of the next view and switches to the next view. This can
     * be used to animate the old text out and animate the next text in.
     *
     * @param text the new text to display
     */
ECode TextSwitcher::SetText(
    /* [in] */ ICharSequence* text)
{
    AutoPtr<IView> v;
    GetNextView((IView**)&v);
    AutoPtr<ITextView> tv = (ITextView*)v->Probe(EIID_ITextView);
    if(tv == NULL)
    {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    tv->SetText(text);
    ShowNext();
    return NOERROR;
}

    /**
     * Sets the text of the text view that is currently showing.  This does
     * not perform the animations.
     *
     * @param text the new text to display
     */
ECode TextSwitcher::SetCurrentText(
    /* [in] */ ICharSequence* text)
{
    AutoPtr<IView> v;
    GetCurrentView((IView**)&v);
    AutoPtr<ITextView> tv = (ITextView*)v->Probe(EIID_ITextView);
    if(tv == NULL)
    {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    tv->SetText(text);
    return NOERROR;
}

ECode TextSwitcher::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    ViewSwitcher::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CTextSwitcher"), (ICharSequence**)&seq);
    return event->SetClassName(seq);
}

ECode TextSwitcher::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    ViewSwitcher::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CTextSwitcher"), (ICharSequence**)&seq);
    return info->SetClassName(seq);
}

ECode TextSwitcher::Init(
    /* [in] */ IContext* context)
{
    return ViewSwitcher::Init(context);
}

ECode TextSwitcher::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ViewSwitcher::Init(context, attrs);
}

}// namespace Elastos
}// namespace Droid
}// namespace Widget
