#include "text/method/MultiTapKeyListener.h"
#include "text/method/CTextKeyListener.h"
#include "text/Selection.h"
#include "os/SystemClock.h"
#include <elastos/core/Math.h>
#include <elastos/core/Character.h>

using Elastos::Core::Character;
using Elastos::Core::CStringWrapper;
using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::Text::Selection;
using Elastos::Droid::Text::Method::CTextKeyListener;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::EIID_IHandler;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {
/*****************************MultiTapKeyListener::Timeout*****************************/

MultiTapKeyListener::Timeout::Timeout(
    /* [in] */ IEditable* buffer)
{
    mBuffer = buffer;
    Int32 bufLen;
    mBuffer->GetLength(&bufLen);
    mBuffer->SetSpan((IInterface*)(IRunnable*)this, 0, bufLen, ISpanned::SPAN_INCLUSIVE_INCLUSIVE);
    Boolean result;
    PostAtTime(this, SystemClock::GetUptimeMillis() + 2000, &result);
}

ECode MultiTapKeyListener::Timeout::Run()
{
    AutoPtr<ISpannable> buf = ISpannable::Probe(mBuffer);

    if (buf != NULL) {

        Int32 st;
        st = Selection::GetSelectionStart(buf.Get());
        Int32 en;
        en = Selection::GetSelectionEnd(buf.Get());

        Int32 start;
        buf->GetSpanStart((CTextKeyListener::ACTIVE).Get(), &start);
        Int32 end;
        buf->GetSpanEnd((CTextKeyListener::ACTIVE).Get(), &end);

        if (st == start && en == end) {
            en = Selection::GetSelectionEnd(buf);
            Selection::SetSelection(buf, en);
        }

        buf->RemoveSpan((IInterface*)(IRunnable*)this);
    }
    return NOERROR;
}

/*****************************MultiTapKeyListener*****************************/
HashMap<Int32, String> MultiTapKeyListener::sRecs = MultiTapKeyListener::InitStaticRecs();

HashMap<Int32, String> MultiTapKeyListener::InitStaticRecs()
{
    HashMap<Int32, String> ret;
    ret[IKeyEvent::KEYCODE_1] = String(".,1!@#$%^&*:/?'=()");
    ret[IKeyEvent::KEYCODE_2] = String("abc2ABC");
    ret[IKeyEvent::KEYCODE_3] = String("def3DEF");
    ret[IKeyEvent::KEYCODE_4] = String("ghi4GHI");
    ret[IKeyEvent::KEYCODE_5] = String("jkl5JKL");
    ret[IKeyEvent::KEYCODE_6] = String("mno6MNO");
    ret[IKeyEvent::KEYCODE_7] = String("pqrs7PQRS");
    ret[IKeyEvent::KEYCODE_8] = String("tuv8TUV");
    ret[IKeyEvent::KEYCODE_9] = String("wxyz9WXYZ");
    ret[IKeyEvent::KEYCODE_0] = String("0+");
    ret[IKeyEvent::KEYCODE_POUND] = String(" ");
    return ret;
}

MultiTapKeyListener::MultiTapKeyListener(
    /* [in] */ Capitalize cap,
    /* [in] */ Boolean autotext)
{
    Init(cap, autotext);
}

MultiTapKeyListener::MultiTapKeyListener()
{}

void MultiTapKeyListener::Init(
    /* [in] */ Capitalize cap,
    /* [in] */ Boolean autotext)
{
    mCapitalize = cap;
    mAutoText = autotext;
}

Int32 MultiTapKeyListener::GetInputType()
{
    return MakeTextContentType(mCapitalize, mAutoText);
}

Boolean MultiTapKeyListener::OnKeyDown(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Int32 selStart, selEnd;
    Int32 pref = 0;

    if (view != NULL) {
        AutoPtr<IContext> contextT;
        view->GetContext((IContext**)&contextT);
        pref = ((CTextKeyListener*)((CTextKeyListener::GetInstance()).Get()))->GetPrefs(contextT.Get());
    }

    {
        Int32 a;
        a = Selection::GetSelectionStart(content);
        Int32 b;
        b = Selection::GetSelectionEnd(content);

        selStart = Elastos::Core::Math::Min(a, b);
        selEnd = Elastos::Core::Math::Max(a, b);
    }

    Int32 activeStart;
    content->GetSpanStart(CTextKeyListener::ACTIVE, &activeStart);
    Int32 activeEnd;
    content->GetSpanEnd(CTextKeyListener::ACTIVE, &activeEnd);

    // now for the multitap cases...

    // Try to increment the character we were working on before
    // if we have one and it's still the same key.

    Int32 flagsT;
    content->GetSpanFlags(CTextKeyListener::ACTIVE, &flagsT);

    //Java:    int rec = (content.getSpanFlags(TextKeyListener.ACTIVE) & Spannable.SPAN_USER) >>> Spannable.SPAN_USER_SHIFT;
    Int32 rec = (flagsT & ISpanned::SPAN_USER)/ ((Int32)(Elastos::Core::Math::Pow(2, ISpanned::SPAN_USER_SHIFT)));

    if (activeStart == selStart && activeEnd == selEnd &&
            selEnd - selStart == 1 &&
            rec >= 0 && rec < sRecs.GetSize()) {
        if (keyCode == IKeyEvent::KEYCODE_STAR) {
            Char32 current;
            content->GetCharAt(selStart, &current);

            if (Character::IsLowerCase(current)) {
                String strT = String((char*)&current,1);
                AutoPtr<ICharSequence> cs;
                CStringWrapper::New(strT.ToUpperCase(), (ICharSequence**)&cs);
                content->Replace(selStart, selEnd , cs.Get());
                RemoveTimeouts(content);
                AutoPtr<Timeout> t = new Timeout(content); // for its side effects
                return TRUE;
            }
            if (Character::IsUpperCase(current)) {
                String strT = String((char*)&current,1);
                AutoPtr<ICharSequence> cs;
                CStringWrapper::New(strT.ToLowerCase(), (ICharSequence**)&cs);
                content->Replace(selStart, selEnd, cs.Get());
                RemoveTimeouts(content);
                AutoPtr<Timeout> t = new Timeout(content); // for its side effects
                return TRUE;
            }
        }
        //Java:    if (sRecs.indexOfKey(keyCode) == rec)
        if (((keyCode < 8 ? keyCode+10 : keyCode) -8) == rec)
        {
            //Java:    String val = sRecs.valueAt(rec);
            HashMap<Int32, String>::Iterator iterRecs = sRecs.Find(keyCode);
            String val = (*iterRecs).mSecond;
            Char32 ch;
            content->GetCharAt(selStart, &ch);
            Int32 ix = val.IndexOf(ch);

            if (ix >= 0) {
                ix = (ix + 1) % (val.GetLength());
                AutoPtr<ICharSequence> cs;
                CStringWrapper::New(val, (ICharSequence**)&cs);
                content->Replace(selStart, selEnd, cs, ix, ix + 1);
                RemoveTimeouts(content);
                AutoPtr<Timeout> t = new Timeout(content); // for its side effects
                return TRUE;
            }
        }

        // Is this key one we know about at all?  If so, acknowledge
        // that the selection is our fault but the key has changed
        // or the text no longer matches, so move the selection over
        // so that it inserts instead of replaces.

        //Java:    rec = sRecs.indexOfKey(keyCode);
        rec = ((keyCode < 8 ? keyCode+10 : keyCode) -8);

        if (rec >= 0) {
            Selection::SetSelection(content, selEnd, selEnd);
            selStart = selEnd;
        }
    }
    else {
        //Java:    rec = sRecs.indexOfKey(keyCode);
        rec = ((keyCode < 8 ? keyCode+10 : keyCode) -8);
    }

    if (rec >= 0) {
        // We have a valid key.  Replace the selection or insertion point
        // with the first character for that key, and remember what
        // record it came from for next time.
        //Java:    String val = sRecs.valueAt(rec);
        HashMap<Int32, String>::Iterator iterRecs = sRecs.Find(keyCode);
        String val = (*iterRecs).mSecond;

        Int32 off = 0;
        if ((pref & CTextKeyListener::AUTO_CAP) != 0 &&
                CTextKeyListener::ShouldCap(mCapitalize, content, selStart)) {
            AutoPtr<ArrayOf<Char32> > chars = val.GetChars();
            for (Int32 i = 0; i < chars->GetLength(); i++) {
                if (Character::IsUpperCase((*chars)[i])) {
                    off = i;
                    break;
                }
            }
        }

        if (selStart != selEnd) {
            Selection::SetSelection(content, selEnd);
        }

        content->SetSpan(OLD_SEL_START, selStart, selStart, ISpanned::SPAN_MARK_MARK);

        AutoPtr<ICharSequence> cs;
        CStringWrapper::New(val, (ICharSequence**)&cs);
        content->Replace(selStart, selEnd, cs.Get(), off, off + 1);

        Int32 oldStart;
        content->GetSpanStart(OLD_SEL_START, &oldStart);
        selEnd = Selection::GetSelectionEnd(content);

        if (selEnd != oldStart) {
            Selection::SetSelection(content, oldStart, selEnd);

            content->SetSpan(CTextKeyListener::LAST_TYPED,
                                oldStart, selEnd,
                                ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);

            content->SetSpan(CTextKeyListener::ACTIVE,
                            oldStart, selEnd,
                            ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE |
                            (rec << ISpanned::SPAN_USER_SHIFT));

        }

        RemoveTimeouts(content);
        AutoPtr<Timeout> t = new Timeout(content); // for its side effects
        return TRUE;

        // Set up the callback so we can remove the timeout if the
        // cursor moves.

        Int32 spanStart;
        if ((content->GetSpanStart((IInterface*)(IMultiTapKeyListener*)this, &spanStart), spanStart) < 0) {
            AutoPtr< ArrayOf<IKeyListener*> > methods;
            Int32 contentLen;
            content->GetSpans(0, (content->GetLength(&contentLen), contentLen), EIID_IKeyListener, (ArrayOf<IInterface*>**)&methods );

            Int32 aryLen = methods->GetLength();
            for(Int32 i =0; i < aryLen; i++) {
                AutoPtr<IKeyListener> method = (*methods)[i];
                content->RemoveSpan((IInterface*)(method.Get()));
            }
            content->SetSpan((IInterface*)(IMultiTapKeyListener*)this, 0, (content->GetLength(&contentLen), contentLen), ISpanned::SPAN_INCLUSIVE_INCLUSIVE);
        }

        return TRUE;
    }

    return BaseKeyListener::OnKeyDown(view, content, keyCode, event);
}

ECode MultiTapKeyListener::OnSpanChanged(
    /* [in] */ ISpannable* buf,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 s,
    /* [in] */ Int32 e,
    /* [in] */ Int32 start,
    /* [in] */ Int32 stop)
{
    AutoPtr<IInterface> end;
    end = Selection::SELECTION_END;

    if (what == end) {
        buf->RemoveSpan(CTextKeyListener::ACTIVE);
        RemoveTimeouts(buf);
    }
    return NOERROR;
}

void MultiTapKeyListener::RemoveTimeouts(
    /* [in] */ ISpannable* buf)
{
    Int32 bufLen;
    buf->GetLength(&bufLen);
    AutoPtr< ArrayOf<IRunnable*> > timeout;
    buf->GetSpans(0, bufLen, EIID_IRunnable, (ArrayOf<IInterface*>**)&timeout);

    for (Int32 i = 0; i < timeout->GetLength(); i++) {
        AutoPtr<Timeout> t = (Timeout*)((*timeout)[i]);
        t->RemoveCallbacks(t);

        t->mBuffer = NULL;
        buf->RemoveSpan((IInterface*)(IRunnable*)(t.Get()));
    }
}

ECode MultiTapKeyListener::OnSpanAdded(
    /* [in] */ ISpannable* s,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{return NOERROR;}

ECode MultiTapKeyListener::OnSpanRemoved(
    /* [in] */ ISpannable* s,
    /* [in] */ IInterface* what,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{return NOERROR;}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos
