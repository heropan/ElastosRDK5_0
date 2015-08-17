
#include "text/method/QwertyKeyListener.h"
#include "text/method/TextKeyListener.h"
#include "text/method/CTextKeyListener.h"
#include "text/SpannableStringInternal.h"
#include "text/TextUtils.h"
#include "text/Selection.h"
#include "text/CAutoText.h"
#include "view/CKeyCharacterMap.h"
#include "view/CKeyEvent.h"
#include "ext/frameworkext.h"
#include <elastos/core/Math.h>
#include <elastos/core/Character.h>
#include <stdio.h>
#include <elastos/core/StringUtils.h>

using namespace Elastos::Core;
using Elastos::Core::StringUtils;
using Elastos::Droid::Text::Selection;
using Elastos::Droid::View::CKeyCharacterMap;
using Elastos::Droid::View::CKeyEvent;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

const InterfaceID EIID_Replaced =
    {0xa40b81a, 0x803b, 0x4e5f, {0xb6, 0xde, 0x49, 0x87, 0x24, 0x49, 0xab, 0x50}};

QwertyKeyListener::Replaced::Replaced(
    /* [in] */ ArrayOf<Char8>* text)
{
    mText = text;
}

QwertyKeyListener::Replaced::~Replaced()
{
}

PInterface QwertyKeyListener::Replaced::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(INoCopySpan*)this;
    }
    else if (riid == EIID_INoCopySpan) {
        return (INoCopySpan*)this;
    }
    else if (riid == EIID_Replaced) {
        return reinterpret_cast<PInterface>(this);
    }

    return NULL;
}

UInt32 QwertyKeyListener::Replaced::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 QwertyKeyListener::Replaced::Release()
{
    return ElRefBase::Release();
}

ECode QwertyKeyListener::Replaced::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == reinterpret_cast<PInterface>(this)) {
        *pIID = EIID_Replaced;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

static Boolean InitStaticPICKER_SETS()
{
    QwertyKeyListener::PICKER_SETS['A'] = String("\u00C0\u00C1\u00C2\u00C4\u00C6\u00C3\u00C5\u0104\u0100");
    QwertyKeyListener::PICKER_SETS['C'] = String("\u00C7\u0106\u010C");
    QwertyKeyListener::PICKER_SETS['D'] = String("\u010E");
    QwertyKeyListener::PICKER_SETS['E'] = String("\u00C8\u00C9\u00CA\u00CB\u0118\u011A\u0112");
    QwertyKeyListener::PICKER_SETS['G'] = String("\u011E");
    QwertyKeyListener::PICKER_SETS['L'] = String("\u0141");
    QwertyKeyListener::PICKER_SETS['I'] = String("\u00CC\u00CD\u00CE\u00CF\u012A\u0130");
    QwertyKeyListener::PICKER_SETS['N'] = String("\u00D1\u0143\u0147");
    QwertyKeyListener::PICKER_SETS['O'] = String("\u00D8\u0152\u00D5\u00D2\u00D3\u00D4\u00D6\u014C");
    QwertyKeyListener::PICKER_SETS['R'] = String("\u0158");
    QwertyKeyListener::PICKER_SETS['S'] = String("\u015A\u0160\u015E");
    QwertyKeyListener::PICKER_SETS['T'] = String("\u0164");
    QwertyKeyListener::PICKER_SETS['U'] = String("\u00D9\u00DA\u00DB\u00DC\u016E\u016A");
    QwertyKeyListener::PICKER_SETS['Y'] = String("\u00DD\u0178");
    QwertyKeyListener::PICKER_SETS['Z'] = String("\u0179\u017B\u017D");
    QwertyKeyListener::PICKER_SETS['a'] = String("\u00E0\u00E1\u00E2\u00E4\u00E6\u00E3\u00E5\u0105\u0101");
    QwertyKeyListener::PICKER_SETS['c'] = String("\u00E7\u0107\u010D");
    QwertyKeyListener::PICKER_SETS['d'] = String("\u010F");
    QwertyKeyListener::PICKER_SETS['e'] = String("\u00E8\u00E9\u00EA\u00EB\u0119\u011B\u0113");
    QwertyKeyListener::PICKER_SETS['g'] = String("\u011F");
    QwertyKeyListener::PICKER_SETS['i'] = String("\u00EC\u00ED\u00EE\u00EF\u012B\u0131");
    QwertyKeyListener::PICKER_SETS['l'] = String("\u0142");
    QwertyKeyListener::PICKER_SETS['n'] = String("\u00F1\u0144\u0148");
    QwertyKeyListener::PICKER_SETS['o'] = String("\u00F8\u0153\u00F5\u00F2\u00F3\u00F4\u00F6\u014D");
    QwertyKeyListener::PICKER_SETS['r'] = String("\u0159");
    QwertyKeyListener::PICKER_SETS['s'] = String("\u00A7\u00DF\u015B\u0161\u015F");
    QwertyKeyListener::PICKER_SETS['t'] = String("\u0165");
    QwertyKeyListener::PICKER_SETS['u'] = String("\u00F9\u00FA\u00FB\u00FC\u016F\u016B");
    QwertyKeyListener::PICKER_SETS['y'] = String("\u00FD\u00FF");
    QwertyKeyListener::PICKER_SETS['z'] = String("\u017A\u017C\u017E");
    QwertyKeyListener::PICKER_SETS[CKeyCharacterMap::PICKER_DIALOG_INPUT] = String("\u2026\u00A5\u2022\u00AE\u00A9\u00B1[]{}\\|");
    QwertyKeyListener::PICKER_SETS['/'] = String("\\");

    // From packages/inputmethods/LatinIME/res/xml/kbd_symbols.xml

    QwertyKeyListener::PICKER_SETS['1'] = String("\u00b9\u00bd\u2153\u00bc\u215b");
    QwertyKeyListener::PICKER_SETS['2'] = String("\u00b2\u2154");
    QwertyKeyListener::PICKER_SETS['3'] = String("\u00b3\u00be\u215c");
    QwertyKeyListener::PICKER_SETS['4'] = String("\u2074");
    QwertyKeyListener::PICKER_SETS['5'] = String("\u215d");
    QwertyKeyListener::PICKER_SETS['7'] = String("\u215e");
    QwertyKeyListener::PICKER_SETS['0'] = String("\u207f\u2205");
    QwertyKeyListener::PICKER_SETS['$'] = String("\u00a2\u00a3\u20ac\u00a5\u20a3\u20a4\u20b1");
    QwertyKeyListener::PICKER_SETS['%'] = String("\u2030");
    QwertyKeyListener::PICKER_SETS['*'] = String("\u2020\u2021");
    QwertyKeyListener::PICKER_SETS['-'] = String("\u2013\u2014");
    QwertyKeyListener::PICKER_SETS['+'] = String("\u00b1");
    QwertyKeyListener::PICKER_SETS['('] = String("[{<");
    QwertyKeyListener::PICKER_SETS[')'] = String("]}>");
    QwertyKeyListener::PICKER_SETS['!'] = String("\u00a1");
    QwertyKeyListener::PICKER_SETS['"'] = String("\u201c\u201d\u00ab\u00bb\u02dd");
    QwertyKeyListener::PICKER_SETS['?'] = String("\u00bf");
    QwertyKeyListener::PICKER_SETS[','] = String("\u201a\u201e");

    // From packages/inputmethods/LatinIME/res/xml/kbd_symbols_shift.xml

    QwertyKeyListener::PICKER_SETS['='] = String("\u2260\u2248\u221e");
    QwertyKeyListener::PICKER_SETS['<'] = String("\u2264\u00ab\u2039");
    QwertyKeyListener::PICKER_SETS['>'] = String("\u2265\u00bb\u203a");
    return TRUE;
};

HashMap<Char32, String> QwertyKeyListener::PICKER_SETS;
Boolean QwertyKeyListener::sInitPickerSet = InitStaticPICKER_SETS();

QwertyKeyListener::QwertyKeyListener()
{}

QwertyKeyListener::QwertyKeyListener(
    /* [in] */ Capitalize cap,
    /* [in] */ Boolean autotext)
{
    Init(cap, autotext);
}

QwertyKeyListener::QwertyKeyListener(
    /* [in] */ Capitalize cap,
    /* [in] */ Boolean autotext,
    /* [in] */ Boolean fullKeyboard)
{
    Init(cap, autotext, fullKeyboard);
}

void QwertyKeyListener::Init(
    /* [in] */ Capitalize cap,
    /* [in] */ Boolean autotext)
{
    Init(cap, autotext, FALSE);
}

void QwertyKeyListener::Init(
    /* [in] */ Capitalize cap,
    /* [in] */ Boolean autotext,
    /* [in] */ Boolean fullKeyboard)
{
    mAutoCap = cap;
    mAutoText = autotext;
    mFullKeyboard = fullKeyboard;
}

Int32 QwertyKeyListener::GetInputType()
{
    return MakeTextContentType(mAutoCap, mAutoText);
}

Boolean QwertyKeyListener::OnKeyDown(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Int32 selStart, selEnd;
    Int32 pref = 0;

    if (view != NULL) {
        AutoPtr<IContext> context;
        view->GetContext((IContext**)&context);
        AutoPtr<ITextKeyListener> obj = CTextKeyListener::GetInstance();
        CTextKeyListener* listener = (CTextKeyListener*)obj.Get();
        pref = listener->GetPrefs(context);
    }

    {
        Int32 a = Selection::GetSelectionStart(content);
        Int32 b = Selection::GetSelectionEnd(content);

        selStart = Elastos::Core::Math::Min(a, b);
        selEnd = Elastos::Core::Math::Max(a, b);

        if (selStart < 0 || selEnd < 0) {
            selStart = selEnd = 0;
            Selection::SetSelection(content, 0, 0);
        }
    }
    Int32 activeStart;
    ISpannable::Probe(content)->GetSpanStart(
        TextKeyListener::ACTIVE, &activeStart);
    Int32 activeEnd;
    ISpannable::Probe(content)->GetSpanEnd(
        TextKeyListener::ACTIVE, &activeEnd);

    // QWERTY keyboard normal case

    Int32 eMetaState;
    Int32 i;
    event->GetUnicodeChar((event->GetMetaState(&eMetaState), eMetaState)|GetMetaState(content), &i);

    if (!mFullKeyboard) {
        Int32 count;
        event->GetRepeatCount(&count);
        if (count > 0 && selStart == selEnd && selStart > 0) {
            Char32 c;
            content->GetCharAt(selStart - 1, &c);

            if (c == i || c == Character::ToUpperCase(i) && view != NULL) {
                if (ShowCharacterPicker(view, content, c, FALSE, count)) {
                    ResetMetaState(content);
                    return TRUE;
                }
            }
        }
    }

    if (i == CKeyCharacterMap::PICKER_DIALOG_INPUT) {
        if (view != NULL) {
            ShowCharacterPicker(view, content,
                CKeyCharacterMap::PICKER_DIALOG_INPUT, TRUE, 1);
        }
        ResetMetaState(content);
        return TRUE;
    }
    if (i == CKeyCharacterMap::HEX_INPUT) {
        Int32 start;

        if (selStart == selEnd) {
            start = selEnd;

            Char32 ch;
            while (start > 0 && selEnd - start < 4 &&
                Character::ToDigit((content->GetCharAt(start - 1, &ch), ch), 16) >= 0) {
                start--;
            }
        }
        else {
            start = selStart;
        }

        Int32 ch = -1;

        String hex;
        hex = TextUtils::Substring(content, start, selEnd);
        ch = StringUtils::ParseInt32(hex, 16);

        if (ch >= 0) {
            selStart = start;
            Selection::SetSelection(content, selStart, selEnd);
            i = ch;
        }
        else {
            i = 0;
        }
    }
    Boolean bHasNoModifiers, bHasModifiers;
    if (i != 0) {
        Boolean dead = FALSE;

        if ((i & CKeyCharacterMap::COMBINING_ACCENT) != 0) {
            dead = TRUE;
            i = i & CKeyCharacterMap::COMBINING_ACCENT_MASK;
        }

        if (activeStart == selStart && activeEnd == selEnd) {
            Boolean replace = FALSE;

            if (selEnd - selStart - 1 == 0) {
                Char32 accent;
                content->GetCharAt(selStart, &accent);
                Int32 composed = CKeyEvent::GetDeadChar(accent, i);

                if (composed != 0) {
                    i = composed;
                    replace = TRUE;
                }
            }

            if (!replace) {
                Selection::SetSelection(content, selEnd);
                content->RemoveSpan(TextKeyListener::ACTIVE);
                selStart = selEnd;
            }
        }
        if ((pref & TextKeyListener::AUTO_CAP) != 0 &&
            Character::IsLowerCase(i) &&
            TextKeyListener::ShouldCap(mAutoCap, content, selStart)) {
            Int32 where;
            content->GetSpanEnd(TextKeyListener::CAPPED, &where);
            Int32 flags;
            content->GetSpanFlags(TextKeyListener::CAPPED, &flags);

            if (where == selStart && (((flags >> 16) & 0xFFFF) == i)) {
                content->RemoveSpan(TextKeyListener::CAPPED);
            }
            else {
                flags = i << 16;
                i = Character::ToUpperCase(i);

                if (selStart == 0)
                    content->SetSpan(TextKeyListener::CAPPED, 0, 0,
                                    ISpanned::SPAN_MARK_MARK | flags);
                else
                    content->SetSpan(TextKeyListener::CAPPED,
                                    selStart - 1, selStart,
                                    ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE |
                                    flags);
            }
        }
        if (selStart != selEnd) {
            Selection::SetSelection(content, selEnd);
        }
        content->SetSpan(OLD_SEL_START, selStart, selStart,
                        ISpanned::SPAN_MARK_MARK);

        String str("");
        str.Append(i);
        AutoPtr<ICharSequence> cs;
        CString::New(str, (ICharSequence**)&cs);
        content->Replace(selStart, selEnd, cs);

        Int32 oldStart;
        content->GetSpanStart(OLD_SEL_START, &oldStart);
        selEnd = Selection::GetSelectionEnd(content);
        if (oldStart < selEnd) {
            content->SetSpan(TextKeyListener::LAST_TYPED,
                            oldStart, selEnd,
                            ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
            if (dead) {
                Selection::SetSelection(content, oldStart, selEnd);
                content->SetSpan(TextKeyListener::ACTIVE, oldStart, selEnd,
                    ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
            }
        }

        AdjustMetaAfterKeypress(content);

        // potentially do autotext replacement if the character
        // that was typed was an autotext terminator

        Int32 end;
        if ((pref & TextKeyListener::AUTO_TEXT) != 0 && mAutoText &&
            (i == ' ' || i == '\t' || i == '\n' ||
             i == ',' || i == '.' || i == '!' || i == '?' ||
             i == '"' || Character::GetType(i) == Character::END_PUNCTUATION) &&
             (content->GetSpanEnd(TextKeyListener::INHIBIT_REPLACEMENT, &end), end)
             != oldStart) {
            Int32 x;

            for (x = oldStart; x > 0; x--) {
                Char32 c;
                content->GetCharAt(x - 1, &c);
                if (c != '\'' && !Character::IsLetter(c)) {
                    break;
                }
            }

            String rep = GetReplacement(content, x, oldStart, view);

            if (!rep.IsNull()) {
                AutoPtr<ArrayOf<IInterface*> > repl;
                Int32 len;
                content->GetLength(&len);
                content->GetSpans(0, len, EIID_Replaced, (ArrayOf<IInterface*>**)&repl);
                for (Int32 a = 0; a < repl->GetLength(); a++)
                    content->RemoveSpan((*repl)[a]);

                AutoPtr<ArrayOf<Char8> > orig = ArrayOf<Char8>::Alloc((oldStart - x) * 4);
                //TODO
                TextUtils::GetChars(content, x, oldStart, (ArrayOf<Char32>*)orig.Get(), 0);

                AutoPtr<Replaced> r = new Replaced(orig);
                content->SetSpan(r, x, oldStart,
                                ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
                AutoPtr<ICharSequence> cs;
                CString::New(rep, (ICharSequence**)&cs);
                content->Replace(x, oldStart, cs);
            }
        }

        // Replace two spaces by a period and a space.

        if ((pref & TextKeyListener::AUTO_PERIOD) != 0 && mAutoText) {
            selEnd = Selection::GetSelectionEnd(content);
            if (selEnd - 3 >= 0) {
                Char32 ch;
                if ((content->GetCharAt(selEnd - 1, &ch), ch) == ' ' &&
                    (content->GetCharAt(selEnd - 2, &ch), ch) == ' ') {
                    Char32 c;
                    content->GetCharAt(selEnd - 3, &c);

                    for (Int32 j = selEnd - 3; j > 0; j--) {
                        if (c == '"' ||
                            Character::GetType(c) == Character::END_PUNCTUATION) {
                            content->GetCharAt(j - 1, &c);
                        }
                        else {
                            break;
                        }
                    }

                    if (Character::IsLetter(c) || Character::IsDigit(c)) {
                        AutoPtr<ICharSequence> cs;
                        CString::New(String("."), (ICharSequence**)&cs);
                        content->Replace(selEnd - 2, selEnd - 1, cs);
                    }
                }
            }
        }

        return TRUE;
    }
    else if (keyCode == IKeyEvent::KEYCODE_DEL
        && (event->HasNoModifiers(&bHasNoModifiers), bHasNoModifiers) || (event->HasModifiers(IKeyEvent::META_ALT_ON, &bHasModifiers), bHasModifiers)
        && selStart == selEnd) {
        // special backspace case for undoing autotext

        Int32 consider = 1;

        // if backspacing over the last typed character,
        // it undoes the autotext prior to that character
        // (unless the character typed was newline, in which
        // case this behavior would be confusing)

        Int32 end;
        content->GetSpanEnd(TextKeyListener::LAST_TYPED, &end);
        if (end == selStart) {
            Char32 c;
            if ((content->GetCharAt(selStart - 1, &c), c) != '\n')
                consider = 2;
        }

        AutoPtr<ArrayOf<IInterface*> > repl = NULL;
        content->GetSpans(
            selStart - consider, selStart, EIID_Replaced, (ArrayOf<IInterface*>**)&repl);

        if (repl->GetLength() > 0) {
            Int32 st;
            content->GetSpanStart((*repl)[0], &st);
            Int32 en;
            content->GetSpanEnd((*repl)[0], &en);
            String old(((Replaced*)(*repl)[0])->mText->GetPayload());

            content->RemoveSpan((*repl)[0]);

            // only cancel the autocomplete if the cursor is at the end of
            // the replaced span (or after it, because the user is
            // backspacing over the space after the word, not the word
            // itself).
            if (selStart >= en) {
                content->SetSpan(TextKeyListener::INHIBIT_REPLACEMENT,
                                en, en, ISpanned::SPAN_POINT_POINT);
                AutoPtr<ICharSequence> oldCs;
                CString::New(old, (ICharSequence**)&oldCs);
                content->Replace(st, en, oldCs);

                content->GetSpanStart(TextKeyListener::INHIBIT_REPLACEMENT, &en);
                if (en - 1 >= 0) {
                    content->SetSpan(
                        TextKeyListener::INHIBIT_REPLACEMENT,
                        en - 1, en, ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
                }
                else {
                    content->RemoveSpan(TextKeyListener::INHIBIT_REPLACEMENT);
                }
                AdjustMetaAfterKeypress(content);
            }
            else {
                AdjustMetaAfterKeypress(content);
                return BaseKeyListener::OnKeyDown(view, content, keyCode, event);
            }

            return TRUE;
        }
    }
    return BaseKeyListener::OnKeyDown(view, content, keyCode, event);
}

String QwertyKeyListener::GetReplacement(
    /* [in] */ ICharSequence* src,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IView* view)
{
    Int32 len = end - start;
    Boolean changecase = FALSE;

    String replacement = CAutoText::Get(src, start, end, view);

    if (replacement.IsNull()) {
        String key;
        key = TextUtils::Substring(src, start, end);
        key = key.ToLowerCase();
        AutoPtr<ICharSequence> csKey;
        CString::New(key, (ICharSequence**)&csKey);
        replacement = CAutoText::Get(csKey, 0, end - start, view);
        changecase = TRUE;

        if (replacement.IsNull())
            return String(NULL);
    }

    Int32 caps = 0;

    if (changecase) {
        for (Int32 j = start; j < end; j++) {
            Char32 c;
            src->GetCharAt(j, &c);
            if (Character::IsUpperCase(c))
                caps++;
        }
    }

    String out;

    if (caps == 0)
        out = replacement;
    else if (caps == 1)
        out = ToTitleCase(replacement);
    else if (caps == len)
        out = replacement.ToUpperCase();
    else
        out = ToTitleCase(replacement);

    AutoPtr<ICharSequence> cs;
    CString::New(out, (ICharSequence**)&cs);
    if (out.GetLength() == len &&
        TextUtils::RegionMatches(src, start, cs, 0, len))
        return String(NULL);

    return out;
}

/**
 * Marks the specified region of <code>content</code> as having
 * contained <code>original</code> prior to AutoText replacement.
 * Call this method when you have done or are about to do an
 * AutoText-style replacement on a region of text and want to let
 * the same mechanism (the user pressing DEL immediately after the
 * change) undo the replacement.
 *
 * @param content the Editable text where the replacement was made
 * @param start the start of the replaced region
 * @param end the end of the replaced region; the location of the cursor
 * @param original the text to be restored if the user presses DEL
 */
void QwertyKeyListener::MarkAsReplaced(
    /* [in] */ ISpannable* content,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ const String& original)
{
    AutoPtr<ArrayOf<IInterface*> > repl;
    Int32 len;
    content->GetLength(&len);
    content->GetSpans(0, len, EIID_Replaced, (ArrayOf<IInterface*>**)&repl);
    for (Int32 a = 0; a < repl->GetLength(); a++) {
        content->RemoveSpan((*repl)[a]);
    }

    len = original.GetByteLength();
    AutoPtr<ArrayOf<Char8> > orig = ArrayOf<Char8>::Alloc(len);
    memcpy(orig->GetPayload(), original.string(), len);

    AutoPtr<Replaced> r = new Replaced(orig);
    content->SetSpan(r, start, end,
                    ISpanned::SPAN_EXCLUSIVE_EXCLUSIVE);
}

Boolean QwertyKeyListener::ShowCharacterPicker(
    /* [in] */ IView* view,
    /* [in] */ IEditable* content,
    /* [in] */ Char32 c,
    /* [in] */ Boolean insert,
    /* [in] */ Int32 count)
{
    HashMap<Char32, String>::Iterator iter = PICKER_SETS.Find(c);
    HashMap<Char32, String>::ValueType value = *iter;
    String set = value.mSecond;
    if (set == NULL) {
       return FALSE;
    }

    if (count == 1) {
//       new CharacterPickerDialog(view.getContext(),
//                                 view, content, set, insert).show();
    }

    return TRUE;
}

String QwertyKeyListener::ToTitleCase(
    /* [in] */ const String& src)
{
    return src.ToUpperCase(0, 1);
}


} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos
