
#include "text/method/QwertyKeyListener.h"
#include "text/TextUtils.h"
#include "widget/MultiAutoCompleteTextView.h"
#include "text/CSpannableString.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Text::CSpannableString;
using Elastos::Droid::Text::ISpannableString;
using Elastos::Droid::Text::Method::QwertyKeyListener;

namespace Elastos{
namespace Droid{
namespace Widget{

const String MultiAutoCompleteTextView::MULTIAUTOCOMPLETETEXTVIEW_NAME = String("MultiAutoCompleteTextView");

MultiAutoCompleteTextView::MultiAutoCompleteTextView()
{

}

MultiAutoCompleteTextView::MultiAutoCompleteTextView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{

}

ECode MultiAutoCompleteTextView::Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle)
{
    AutoCompleteTextView::Init(context, attrs, defStyle);
    return NOERROR;
}

ECode MultiAutoCompleteTextView::SetTokenizer(
    /* [in] */ ITokenizer* t)
{
    mTokenizer = t;
    return NOERROR;
}

Boolean MultiAutoCompleteTextView::EnoughToFilter()
{
    AutoPtr<ICharSequence> seq = GetText();
    AutoPtr<IEditable> text = IEditable::Probe(seq);

    Int32 end = GetSelectionEnd();
    if(end < 0 || mTokenizer == NULL) {
        return FALSE;
    }

    Int32 start = 0;
    mTokenizer->FindTokenStart(text, end, &start);

    if(end - start >= GetThreshold()) {
        return TRUE;
    } else {
        return FALSE;
    }
}

ECode MultiAutoCompleteTextView::PerformValidation()
{
    AutoPtr<IValidator> v = GetValidator();
    if(!v || !mTokenizer) {
        return NOERROR;
    }
    AutoPtr<ICharSequence> seq = GetText();
    AutoPtr<IEditable> text = IEditable::Probe(seq);
    Int32 i = 0;
    GetText()->GetLength(&i);

    while(i > 0) {
        Int32 start = 0;
        mTokenizer->FindTokenStart (text, i, &start);
        Int32 end = 0;
        mTokenizer->FindTokenEnd(text, start, &end);

        AutoPtr<ICharSequence> sub;
        text->SubSequence(start, end, (ICharSequence**)&sub);

        Boolean isValid = false;
        v->IsValid(sub, &isValid);
        if(TextUtils::IsEmpty(sub)) {
            AutoPtr<ICharSequence> csqnull;
            CStringWrapper::New(String(""), (ICharSequence**)&csqnull);
            text->Replace(start, i, csqnull);
        } else if (!isValid) {
            AutoPtr<ICharSequence> csq;
            v->FixText(sub, (ICharSequence**)&csq);
            AutoPtr<ICharSequence> terCsq;
            mTokenizer->TerminateToken(csq, (ICharSequence**)&terCsq);
            text->Replace(start, i, terCsq);
        }
        i = start;
    }
    return NOERROR;
}

ECode MultiAutoCompleteTextView::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    AutoCompleteTextView::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(MULTIAUTOCOMPLETETEXTVIEW_NAME, (ICharSequence**)&seq));
    event->SetClassName(seq);
    return NOERROR;
}

ECode MultiAutoCompleteTextView::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    AutoCompleteTextView::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(MULTIAUTOCOMPLETETEXTVIEW_NAME, (ICharSequence**)&seq));
    info->SetClassName(seq);
    return NOERROR;
}

void MultiAutoCompleteTextView::PerformFiltering(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 keyCode)
{
    if(EnoughToFilter()) {
        Int32 end = GetSelectionEnd();
        Int32 start = 0;
        mTokenizer->FindTokenStart(text, end, &start);

        PerformFiltering(text, start, end, keyCode);
    } else {
        DismissDropDown();

        AutoPtr<IFilter> f = GetFilter();
        if(f) {
            f->DoFilter(NULL);
        }
    }
}

void MultiAutoCompleteTextView::PerformFiltering(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 keyCode)
{
    AutoPtr<ICharSequence> sub;
    text->SubSequence(start, end, (ICharSequence**)&sub);
    GetFilter()->DoFilter(sub, (IFilterListener*)this->Probe(EIID_IFilterListener));
}

void MultiAutoCompleteTextView::ReplaceText(
    /* [in] */ ICharSequence* text)
{
    ClearComposingText();
    Int32 end = GetSelectionEnd();
    Int32 start = 0;
    mTokenizer->FindTokenStart(GetText(), end, &start);

    AutoPtr<ICharSequence> chars = GetText();
    AutoPtr<IEditable> editable = IEditable::Probe(chars);
    String original = TextUtils::Substring(editable, start, end);
    QwertyKeyListener::MarkAsReplaced(editable, start, end, original);
    AutoPtr<ICharSequence> seq;
    mTokenizer->TerminateToken(text, (ICharSequence**)&seq);
    editable->Replace(start, end, seq);
}

void MultiAutoCompleteTextView::FinishInit()
{
    //do nothing;
}

//=========================================================================
//             MultiAutoCompleteTextView::CommaTokenizer
//=========================================================================
MultiAutoCompleteTextView::CommaTokenizer::CommaTokenizer()
{

}

ECode MultiAutoCompleteTextView::CommaTokenizer::FindTokenStart(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 cursor,
    /* [out] */ Int32* start)
{
    Int32 i = cursor;
    Char32 c = 0;
    text->GetCharAt(i - 1, &c);
    while (i > 0 && c != ',') {
        i--;
        text->GetCharAt(i - 1, &c);
    }
    Char32 ci = 0;
    text->GetCharAt(i, &ci);
    while (i < cursor && ci == ' ') {
        i++;
        text->GetCharAt(i, &ci);
    }
    *start = i;
    return NOERROR;
}

ECode MultiAutoCompleteTextView::CommaTokenizer::FindTokenEnd(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 cursor,
    /* [out] */ Int32* end)
{
    Int32 i = cursor;
    Int32 len = 0;
    text->GetLength(&len);
    while (i < len) {
        Char32 c = 0;
        text->GetCharAt(i, &c);
        if (c == ',') {
            *end = i;
        } else {
            i++;
        }
    }
    *end = len;
    return NOERROR;
}

ECode MultiAutoCompleteTextView::CommaTokenizer::TerminateToken(
    /* [in] */ ICharSequence* text,
    /* [out] */ ICharSequence** res)
{
    Int32 i = 0;
    text->GetLength(&i);
    Char32 c = 0;
    text->GetCharAt(i - 1, &c);
    while (i > 0 && c == ' ') {
        i--;
        text->GetCharAt(i - 1, &c);
    }
    if (i > 0 && c == ',') {
        *res = text;
        // is right?
        INTERFACE_ADDREF(*res);
    } else {
        if (ISpanned::Probe(text)) {
            String str;
            text->ToString(&str);
            AutoPtr<ICharSequence> csq;
            CStringWrapper::New(str + String(","), (ICharSequence**)&csq);
            AutoPtr<ISpannable> sp;
            CSpannableString::New(csq, (ISpannableString**)&sp);
            Int32 len = 0;
            text->GetLength(&len);
            TextUtils::CopySpansFrom(ISpanned::Probe(text), 0, len, EIID_IInterface, sp, 0);
            *res = ICharSequence::Probe(sp.Get());
            INTERFACE_ADDREF(*res);
        } else {
            String str;
            text->ToString(&str);
            AutoPtr<ICharSequence> csq;
            CStringWrapper::New(str + String(","), (ICharSequence**)&csq);
            *res = csq;
            INTERFACE_ADDREF(*res);
        }
    }
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos

