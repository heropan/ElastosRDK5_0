#ifdef DROID_CORE
#include "text/method/NumberKeyListener.h"
#include "text/CSpannableStringBuilder.h"
#include "text/Selection.h"
#endif

#include <elastos/core/Math.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::CString;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

Int32 NumberKeyListener::Lookup(
    /* [in] */ IKeyEvent* event,
    /* [in] */ ISpannable* content)
{
    Int32 metaState;
    Char32 c;
    event->GetMatch(*((GetAcceptedChars()).Get()), THIS_PROBE(IMetaKeyKeyListener)->GetMetaState(ICharSequence::Probe(content), event, &metaState));
    return (Int32)c;
}

AutoPtr<ICharSequence> NumberKeyListener::Filter(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ISpanned* dest,
    /* [in] */ Int32 dstart,
    /* [in] */ Int32 dend)
{
    PRINT_ENTER_LEAVE("NumberKeyListener::Filter jiazhenjiang");
    AutoPtr< ArrayOf<Char32> > accept = GetAcceptedChars();
    Boolean filter = FALSE;

    Int32 i;
    for (i = start; i < end; i++) {
        Char32 c;
        if (!Ok(accept, (source->GetCharAt(i, &c), c))) {
            break;
        }
    }

    if (i == end) {
        // It was all OK.
        return NULL;
    }

    if (end - start == 1) {
        // It was not OK, and there is only one char, so nothing remains.
        AutoPtr<ICharSequence> cs;
        CString::New(String(""), (ICharSequence**)&cs);
        return cs;
    }

    AutoPtr<ISpannableStringBuilder> filtered;
    CSpannableStringBuilder::New(source, start, end, (ISpannableStringBuilder**)&filtered);
    i -= start;
    end -= start;

    Int32 len = end - start;
    // Only count down to i because the chars before that were all OK.
    for (Int32 j = end - 1; j >= i; j--) {
        Char32 c;
        if (!Ok(accept, (source->GetCharAt(i, &c), c))) {
            filtered->Delete(j, j + 1);
        }
    }

    return filtered;
}

Boolean NumberKeyListener::Ok(
    /* [in] */ ArrayOf<Char32>* accept,
    /* [in] */ Char32 c)
{
    for (Int32 i = accept->GetLength() - 1; i >= 0; i--) {
        if ((*accept)[i] == c) {
            return TRUE;
        }
    }
    return FALSE;
}

Boolean NumberKeyListener::OnKeyDown(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Int32 selStart, selEnd;

    {
        Int32 a = Selection::GetSelectionStart(content);
        Int32 b = Selection::GetSelectionEnd(content);

        selStart = Elastos::Core::Math::Min(a, b);
        selEnd = Elastos::Core::Math::Max(a, b);
    }

    if (selStart < 0 || selEnd < 0) {
        selStart = selEnd = 0;
        Selection::SetSelection(content, 0);
    }

    Int32 i = event != NULL ? Lookup(event, content) : 0;
    Int32 repeatCount = event != NULL ? event->GetRepeatCount(&repeatCount) : 0;
    if (repeatCount == 0) {
        if (i != 0) {
            if (selStart != selEnd) {
                Selection::SetSelection(content, selEnd);
            }

            AutoPtr<ICharSequence> cs;
            CString::New(StringUtils::Int32ToString(i), (ICharSequence**)&cs);
            content->Replace(selStart, selEnd, cs);

            AdjustMetaAfterKeypress(content);
            return TRUE;
        }
    }
    else if (i == '0' && repeatCount == 1) {
        // Pretty hackish, it replaces the 0 with the +
        Char32 c;
        if (selStart == selEnd && selEnd > 0 && (content->GetCharAt(selStart - 1, &c), c) == '0') {

            AutoPtr<ICharSequence> cs;
            CString::New(String("+"), (ICharSequence**)&cs);
            content->Replace(selStart - 1, selEnd, cs);
            AdjustMetaAfterKeypress(content);
            return TRUE;
        }
    }

    AdjustMetaAfterKeypress(content);
    return BaseKeyListener::OnKeyDown(view, content, keyCode, event);
}


} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos
