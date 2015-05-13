#include <R.h>
#include "widget/DialerFilter.h"
#include "ext/frameworkext.h"
#include "view/CKeyEventHelper.h"
#include "text/CAllCaps.h"
#include "text/Selection.h"
#include "text/method/CTextKeyListenerHelper.h"
#include "text/method/CDialerKeyListenerHelper.h"

using Elastos::Droid::View::IView;
using Elastos::Droid::View::CKeyEventHelper;
using Elastos::Droid::Text::IEditable;
using Elastos::Droid::Text::ISpannable;
using Elastos::Droid::Text::EIID_ISpannable;
using Elastos::Droid::Text::CAllCaps;
using Elastos::Droid::Text::Selection;
using Elastos::Droid::Text::Method::IDialerKeyListenerHelper;
using Elastos::Droid::Text::Method::ITextKeyListenerHelper;
using Elastos::Droid::Text::Method::ITextKeyListener;
using Elastos::Droid::Text::Method::IDialerKeyListener;
using Elastos::Droid::Text::Method::IDialerKeyListenerHelper;
using Elastos::Droid::Text::Method::IKeyListener;
using Elastos::Droid::Text::Method::EIID_IKeyListener;
using Elastos::Droid::Text::Method::CTextKeyListenerHelper;
using Elastos::Droid::Text::Method::CDialerKeyListenerHelper;
using Elastos::Droid::View::IKeyEventCallback;
using Elastos::Droid::View::EIID_IKeyEventCallback;
using Elastos::Droid::View::IKeyEventHelper;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::Widget::RelativeLayout;

namespace Elastos {
namespace Droid {
namespace Widget {

DialerFilter::DialerFilter()
{

}

DialerFilter::DialerFilter(
    /* [in] */ IContext* context)
{
    Init(context);
}

DialerFilter::DialerFilter(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    Init(context, attrs);
}

ECode DialerFilter::Init(
    /* [in] */ IContext* context)
{
    ASSERT_SUCCEEDED(RelativeLayout::Init(context));
    return NOERROR;
}

ECode DialerFilter::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    ASSERT_SUCCEEDED(RelativeLayout::Init(context, attrs));
    return NOERROR;
}

ECode DialerFilter::OnFinishInflate()
{
    View::OnFinishInflate();
    AutoPtr<IInputFilter> allCaps;
    FAIL_RETURN(CAllCaps::New((IInputFilter**)&allCaps));
    mInputFilters = ArrayOf<IInputFilter*>::Alloc(1);
    mInputFilters->Set(0, allCaps);
    AutoPtr<IView> view = FindViewById(R::id::hint);
    mHint = IEditText::Probe(view);

    if(mHint == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    mHint->SetFilters(mInputFilters);
    mLetters = mHint;
    AutoPtr<ITextKeyListenerHelper> textKeyListenerHelper;
    FAIL_RETURN(CTextKeyListenerHelper::AcquireSingleton((ITextKeyListenerHelper**)&textKeyListenerHelper));
    AutoPtr<ITextKeyListener> textKeyListener;
    textKeyListenerHelper->GetInstance((ITextKeyListener**)&textKeyListener);
    AutoPtr<IKeyListener> keyListener;
    keyListener = (IKeyListener*)textKeyListener->Probe(EIID_IKeyListener);
    if(keyListener == NULL){
        assert(0 && "keyListener is not null");
    }
    mLetters->SetKeyListener(keyListener);
    mLetters->SetMovementMethod(NULL);
    mLetters->SetFocusable(FALSE);

    view = FindViewById(R::id::primary);
    mPrimary = IEditText::Probe(view);
    if(mPrimary == NULL){
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mPrimary->SetFilters(mInputFilters);
    mDigits = mPrimary;
    AutoPtr<IDialerKeyListenerHelper> dialerKeyListenerHelper;
    FAIL_RETURN(CDialerKeyListenerHelper::AcquireSingleton((IDialerKeyListenerHelper**)&dialerKeyListenerHelper));
    AutoPtr<IDialerKeyListener> dialerKeyListener;
    dialerKeyListenerHelper->GetInstance((IDialerKeyListener**)&dialerKeyListener);
    AutoPtr<IKeyListener> keyListener1;
    keyListener1 = (IKeyListener*)dialerKeyListener->Probe(EIID_IKeyListener);
    if(keyListener1 == NULL) {
        assert(0 && "keyListener1 is not null");
    }
    mDigits->SetKeyListener(keyListener1);
    mDigits->SetMovementMethod(NULL);
    mDigits->SetFocusable(FALSE);

    view = FindViewById(R::id::icon);
    mIcon = (IImageView*)view->Probe(EIID_IImageView);
    if(mIcon == NULL) {

    }
    SetFocusable(TRUE);
    mIsQwerty = TRUE;

    SetMode(DIGITS_AND_LETTERS);
    return NOERROR;
}

void DialerFilter::OnFocusChanged(
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    RelativeLayout::OnFinishInflate();
    if(mIcon != NULL) {
        mIcon->SetVisibility(focused ? IView::VISIBLE : IView::GONE);
    }
}

Boolean DialerFilter::IsQwertyKeyboard()
{
    return mIsQwerty;
}

Boolean DialerFilter::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Boolean handled = FALSE;
    AutoPtr<ICharSequence> sequence;
    Boolean keyEventResult = FALSE;
    AutoPtr<IKeyEventHelper> keyEventHelper;

    IKeyEventCallback* mLettersCallback = (IKeyEventCallback*)mLetters->Probe(EIID_IKeyEventCallback);
    IKeyEventCallback* mDigitsCallback = (IKeyEventCallback*)mDigits->Probe(EIID_IKeyEventCallback);
    if (mLettersCallback == NULL || mDigitsCallback == NULL) {
        return FALSE;
    }

    switch(keyCode) {
        case IKeyEvent::KEYCODE_DPAD_UP:
        case IKeyEvent::KEYCODE_DPAD_DOWN:
        case IKeyEvent::KEYCODE_DPAD_LEFT:
        case IKeyEvent::KEYCODE_DPAD_RIGHT:
        case IKeyEvent::KEYCODE_ENTER:
        case IKeyEvent::KEYCODE_DPAD_CENTER:
            break;
        case IKeyEvent::KEYCODE_DEL:
            switch(mMode) {
            case DIGITS_AND_LETTERS:
                mDigitsCallback->OnKeyDown(keyCode, event, &handled);
                mLettersCallback->OnKeyDown(keyCode, event, &keyEventResult);
                handled &= keyEventResult;
                break;

            case DIGITS_AND_LETTERS_NO_DIGITS:
                mLettersCallback->OnKeyDown(keyCode, event, &handled);
                Int32 mLetterslen, mDigitslen;
                mLetters->GetText((ICharSequence**)&sequence);
                sequence->GetLength(&mLetterslen);
                sequence = NULL;
                mDigits->GetText((ICharSequence**)&sequence);
                sequence->GetLength(&mDigitslen);
                if(mLetterslen == mDigitslen) {
                    SetMode(DIGITS_AND_LETTERS);
                }
                break;

            case DIGITS_AND_LETTERS_NO_LETTERS:
                Int32 mLetterslen1, mDigitslen1;
                sequence = NULL;
                mLetters->GetText((ICharSequence**)&sequence);
                sequence->GetLength(&mLetterslen1);
                sequence = NULL;
                mDigits->GetText((ICharSequence**)&sequence);
                sequence->GetLength(&mDigitslen1);
                if(mLetterslen1 == mDigitslen1) {
                    Boolean res = FALSE;
                    mLettersCallback->OnKeyDown(keyCode, event, &res);
                    SetMode(DIGITS_AND_LETTERS);
                }
                mDigitsCallback->OnKeyDown(keyCode, event, &handled);
                break;

            case DIGITS_ONLY:
                mDigitsCallback->OnKeyDown(keyCode, event, &handled);
                break;
            case LETTERS_ONLY:
                mLettersCallback->OnKeyDown(keyCode, event, &handled);
                break;
            }
            break;

            default:
                switch (mMode) {
                    case DIGITS_AND_LETTERS:
                        mLettersCallback->OnKeyDown(keyCode, event, &handled);
                        Boolean result;
                        FAIL_RETURN(CKeyEventHelper::AcquireSingleton((IKeyEventHelper**)&keyEventHelper));
                        keyEventHelper->IsModifierKey(keyCode, &result);
                        if(result) {
                            Boolean discard;
                            mDigitsCallback->OnKeyDown(keyCode, event, &discard);
                            handled = TRUE;
                            break;
                        }
                        Boolean isPrint;
                        event->IsPrintingKey(&isPrint);
                        if(isPrint || keyCode == IKeyEvent::KEYCODE_SPACE
                                ||keyCode == IKeyEvent::KEYCODE_TAB) {
                            AutoPtr<IDialerKeyListenerHelper> helper;
                            FAIL_RETURN(CDialerKeyListenerHelper::AcquireSingleton((IDialerKeyListenerHelper**)&helper));
                            AutoPtr<ArrayOf<Char32> > CHARACTERS;

                            helper->GetCHARACTERS((ArrayOf<Char32>**)&CHARACTERS);
                            Char32 c;
                            event->GetMatch(*CHARACTERS ,&c);
                            if(c != 0) {
                                mDigitsCallback->OnKeyDown(keyCode, event, &keyEventResult);
                                handled &= keyEventResult;
                            } else {
                                SetMode(DIGITS_AND_LETTERS_NO_DIGITS);
                            }
                        }
                        break;

                    case DIGITS_AND_LETTERS_NO_LETTERS:
                    case DIGITS_ONLY:
                        mDigitsCallback->OnKeyDown(keyCode, event, &handled);
                        break;

                    case DIGITS_AND_LETTERS_NO_DIGITS:
                    case LETTERS_ONLY:
                        mLettersCallback->OnKeyDown(keyCode, event, &handled);
                        break;
                }
    }

    if (!handled) {
        return RelativeLayout::OnKeyDown(keyCode, event);
    } else {
        return TRUE;
    }
}

Boolean DialerFilter::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    IKeyEventCallback* mLettersCallback = (IKeyEventCallback*)mLetters->Probe(EIID_IKeyEventCallback);
    IKeyEventCallback* mDigitsCallback = (IKeyEventCallback*)mDigits->Probe(EIID_IKeyEventCallback);
    if (mLettersCallback == NULL || mDigitsCallback == NULL) {
        return FALSE;
    }

    Boolean a, b;
    mLettersCallback->OnKeyDown(keyCode, event, &a);
    mDigitsCallback->OnKeyDown(keyCode, event, &b);
    return a || b;
}

Int32 DialerFilter::GetMode()
{
    return mMode;
}

ECode DialerFilter::SetMode(
    /* [in] */ Int32 newMode)
{
    switch (newMode) {
        case DIGITS_AND_LETTERS:
            MakeDigitsPrimary();
            mLetters->SetVisibility(IView::VISIBLE);
            mDigits->SetVisibility(IView::VISIBLE);
            break;
        case DIGITS_ONLY:
            MakeDigitsPrimary();
            mLetters->SetVisibility(IView::GONE);
            mDigits->SetVisibility(IView::VISIBLE);
            break;
        case LETTERS_ONLY:
            MakeLettersPrimary();
            mLetters->SetVisibility(IView::VISIBLE);
            mDigits->SetVisibility(IView::GONE);
            break;
        case DIGITS_AND_LETTERS_NO_LETTERS:
            MakeDigitsPrimary();
            mLetters->SetVisibility(IView::INVISIBLE);
            mDigits->SetVisibility(IView::VISIBLE);
            break;
        case DIGITS_AND_LETTERS_NO_DIGITS:
            MakeLettersPrimary();
            mLetters->SetVisibility(IView::VISIBLE);
            mDigits->SetVisibility(IView::INVISIBLE);
            break;
    }
    Int32 oldMode = mMode;
    mMode = newMode;
    OnModeChange(oldMode, newMode);
    return NOERROR;
}

ECode DialerFilter::MakeLettersPrimary()
{
    if(mPrimary == mLetters) {
        SwapPrimaryAndHint(TRUE);
    }
    return NOERROR;
}

ECode DialerFilter::MakeDigitsPrimary()
{
    if(mPrimary == mLetters) {
        SwapPrimaryAndHint(FALSE);
    }
    return NOERROR;
}

ECode DialerFilter::SwapPrimaryAndHint(
    /* [in] */ Boolean makeLettersPrimary)
{
    AutoPtr<ICharSequence> csq;
    mLetters->GetText((ICharSequence**)&csq);
    AutoPtr<IEditable> lettersText = IEditable::Probe(csq);
    csq = NULL;
    mDigits->GetText((ICharSequence**)&csq);
    AutoPtr<IEditable> digitsText = IEditable::Probe(csq);
    AutoPtr<IKeyListener> lettersInput;
    mLetters->GetKeyListener((IKeyListener**)&lettersInput);
    AutoPtr<IKeyListener> digitsInput;
    mDigits->GetKeyListener((IKeyListener**)&digitsInput);
    if(makeLettersPrimary) {
        mLetters = mPrimary;
        mDigits = mHint;
    } else {
        mLetters = mHint;
        mDigits = mPrimary;
    }

    mLetters->SetKeyListener(lettersInput);
    mLetters->SetText(lettersText);
    csq = NULL;
    mLetters->GetText((ICharSequence**)&csq);
    lettersText = IEditable::Probe(csq);

    Int32 len = 0;
    Selection::SetSelection(lettersText, (lettersText->GetLength(&len), len));

    mDigits->SetKeyListener(digitsInput);
    mDigits->SetText(digitsText);
    csq = NULL;
    mDigits->GetText((ICharSequence**)&csq);
    digitsText = IEditable::Probe(csq);
    Selection::SetSelection(digitsText, (digitsText->GetLength(&len), len));

    mPrimary->SetFilters(mInputFilters);
    mHint->SetFilters(mInputFilters);

    return NOERROR;
}

AutoPtr<ICharSequence> DialerFilter::GetLetters()
{
    Int32 isVisibility;
    mLetters->GetVisibility(&isVisibility);
    if(isVisibility == IView::VISIBLE) {
        AutoPtr<ICharSequence> csq;
        mLetters->GetText((ICharSequence**)&csq);
        return csq;
    } else {
        AutoPtr<ICharSequence> csq;
        CStringWrapper::New(String(""), (ICharSequence**)&csq);
        return csq;
    }
}

AutoPtr<ICharSequence> DialerFilter::GetDigits()
{
    Int32 isVisibility;
    mDigits->GetVisibility(&isVisibility);
    if(isVisibility == IView::VISIBLE) {
        AutoPtr<ICharSequence> csq;
        mDigits->GetText((ICharSequence**)&csq);
        return csq;
    } else {
        AutoPtr<ICharSequence> csq;
        CStringWrapper::New(String(""), (ICharSequence**)&csq);
        return csq;
    }
}

AutoPtr<ICharSequence> DialerFilter::GetFilterText()
{
    if(mMode != DIGITS_ONLY) {
        return GetLetters();
    } else {
        return GetDigits();
    }
}

ECode DialerFilter::Append(
    /* [in] */ const String& text)
{
    AutoPtr<ICharSequence> csq;
    FAIL_RETURN(CStringWrapper::New(text, (ICharSequence**)&csq));

    switch (mMode) {
        case DIGITS_AND_LETTERS:
            mDigits->Append((ICharSequence*)csq);
            mLetters->Append((ICharSequence*)csq);
            break;
        case DIGITS_AND_LETTERS_NO_LETTERS:
        case DIGITS_ONLY:
            mDigits->Append((ICharSequence*)csq);
            break;
        case DIGITS_AND_LETTERS_NO_DIGITS:
        case LETTERS_ONLY:
            mLetters->Append((ICharSequence*)csq);
            break;
    }
    return NOERROR;
}

ECode DialerFilter::ClearText()
{
    AutoPtr<ICharSequence> csq;
    IEditable* text;
    mLetters->GetText((ICharSequence**)&csq);
    text = IEditable::Probe(csq);
    if (text) text->Clear();

    csq = NULL;
    mDigits->GetText((ICharSequence**)&csq);
    text = IEditable::Probe(csq);
    if (text) text->Clear();

    if(mIsQwerty) {
        SetMode(DIGITS_AND_LETTERS);
    } else {
        SetMode(DIGITS_ONLY);
    }
    return NOERROR;
}

ECode DialerFilter::SetLettersWatcher(
    /* [in] */ ITextWatcher* watcher)
{
    AutoPtr<ICharSequence> text;
    mLetters->GetText((ICharSequence**)&text);
    AutoPtr<ISpannable> span = ISpannable::Probe(text);
    if (span == NULL) {
        assert(0 && "span is not null");
    }
    Int32 length;
    text->GetLength(&length);
    span->SetSpan(watcher, 0, length, ISpannable::SPAN_INCLUSIVE_INCLUSIVE);
    return NOERROR;
}

ECode DialerFilter::SetDigitsWatcher(
    /* [in] */ ITextWatcher* watcher)
{
    AutoPtr<ICharSequence> text;
    mDigits->GetText((ICharSequence**)&text);
    AutoPtr<ISpannable> span = ISpannable::Probe(text);
    if (span == NULL) {
        assert(0 && "span is not null");
    }
    Int32 length;
    text->GetLength(&length);
    span->SetSpan(watcher, 0, length, ISpannable::SPAN_INCLUSIVE_INCLUSIVE);
    return NOERROR;
}

ECode DialerFilter::SetFilterWatcher(
    /* [in] */ ITextWatcher* watcher)
{
    if(mMode != DIGITS_ONLY) {
        SetLettersWatcher(watcher);
    } else {
        SetDigitsWatcher(watcher);
    }
    return NOERROR;
}

ECode DialerFilter::RemoveFilterWatcher(
    /* [in] */  ITextWatcher* watcher)
{
    AutoPtr<ISpannable> text;
    if(mMode != DIGITS_ONLY) {
        mLetters->GetText((ICharSequence**)&text);
    } else {
        mDigits->GetText((ICharSequence**)&text);
    }
    text->RemoveSpan(watcher);
    return NOERROR;
}

ECode DialerFilter::OnModeChange(
    /* [in] */ Int32 oldMode,
    /* [in] */ Int32 newMode)
{
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos

