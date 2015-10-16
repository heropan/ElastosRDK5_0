#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/CDialerFilter.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;


namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CDialerFilter, DialerFilter)
IVIEWGROUP_METHODS_IMPL(CDialerFilter, DialerFilter)
IVIEWPARENT_METHODS_IMPL(CDialerFilter, DialerFilter)
IVIEWMANAGER_METHODS_IMPL(CDialerFilter, DialerFilter)
IDRAWABLECALLBACK_METHODS_IMPL(CDialerFilter, DialerFilter)
IKEYEVENTCALLBACK_METHODS_IMPL(CDialerFilter, DialerFilter)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CDialerFilter, DialerFilter)


PInterface CDialerFilter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CDialerFilter::Probe(riid);
}

ECode CDialerFilter::constructor(
    /* [in] */ IContext* context)
{
    return DialerFilter::Init(context);
}

ECode CDialerFilter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return DialerFilter::Init(context, attrs);
}

ECode CDialerFilter::IsQwertyKeyboard(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = DialerFilter::IsQwertyKeyboard();
    return NOERROR;
}

ECode CDialerFilter::GetMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = DialerFilter::GetMode();
    return NOERROR;
}

ECode CDialerFilter::SetMode(
    /* [in] */ Int32 newMode)
{
    return DialerFilter::SetMode(newMode);
}

ECode CDialerFilter::GetLetters(
    /* [out] */ ICharSequence** letters)
{
    VALIDATE_NOT_NULL(letters);
    AutoPtr<ICharSequence> temp = DialerFilter::GetLetters();
    *letters = temp;
    REFCOUNT_ADD(*letters);
    return NOERROR;
}

ECode CDialerFilter::GetDigits(
    /* [out] */ ICharSequence** digits)
{
    VALIDATE_NOT_NULL(digits);
    AutoPtr<ICharSequence> temp = DialerFilter::GetDigits();
    *digits = temp;
    REFCOUNT_ADD(*digits);
    return NOERROR;
}

ECode CDialerFilter::GetFilterText(
    /* [out] */ ICharSequence** filterText)
{
    VALIDATE_NOT_NULL(filterText);
    AutoPtr<ICharSequence> temp = DialerFilter::GetFilterText();
    *filterText = temp;
    REFCOUNT_ADD(*filterText);
    return NOERROR;
}

ECode CDialerFilter::Append(
    /* [in] */ const String& text)
{
    return DialerFilter::Append(text);
}

ECode CDialerFilter::ClearText()
{
    return DialerFilter::ClearText();
}

ECode CDialerFilter::SetLettersWatcher(
    /* [in] */ ITextWatcher* watcher)
{
    return DialerFilter::SetLettersWatcher(watcher);
}

ECode CDialerFilter::SetDigitsWatcher(
    /* [in] */ ITextWatcher* watcher)
{
    return DialerFilter::SetDigitsWatcher(watcher);
}

ECode CDialerFilter::SetFilterWatcher(
    /* [in] */ ITextWatcher* watcher)
{
    return DialerFilter::SetFilterWatcher(watcher);
}

ECode CDialerFilter::RemoveFilterWatcher(
    /* [in] */  ITextWatcher* watcher)
{
    return DialerFilter::RemoveFilterWatcher(watcher);
}

ECode CDialerFilter::SetIgnoreGravity(
    /* [in] */ Int32 viewId)
{
    return DialerFilter::SetIgnoreGravity(viewId);
}

ECode CDialerFilter::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity);
    *gravity = DialerFilter::GetGravity();
    return NOERROR;
}

ECode CDialerFilter::SetGravity(
    /* [in] */ Int32 gravity)
{
    return DialerFilter::SetGravity(gravity);
}

ECode CDialerFilter::SetHorizontalGravity(
    /* [in] */ Int32 horizontalGravity)
{
    return DialerFilter::SetHorizontalGravity(horizontalGravity);
}

ECode CDialerFilter::SetVerticalGravity(
    /* [in] */ Int32 verticalGravity)
{
    return DialerFilter::SetVerticalGravity(verticalGravity);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
