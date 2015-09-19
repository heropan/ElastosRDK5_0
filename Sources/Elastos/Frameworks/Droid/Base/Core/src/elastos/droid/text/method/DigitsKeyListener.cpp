#include "text/method/DigitsKeyListener.h"
#include "text/CSpannableStringBuilder.h"
#include "text/method/CDigitsKeyListener.h"

using Elastos::Core::CString;
using Elastos::Droid::Text::Method::CDigitsKeyListener;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

// {f51e1ea9-7721-4c12-9c44-0a6bcd10b9c1}
extern "C" const InterfaceID EIID_DigitsKeyListener =
        { 0xf51e1ea9, 0x7721, 0x4c12, { 0x9c, 0x44, 0x0a, 0x6b, 0xcd, 0x10, 0xb9, 0xc1 } };

const Char32 DigitsKeyListener::CHARACTERS0[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
const Char32 DigitsKeyListener::CHARACTERS1[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '+' };
const Char32 DigitsKeyListener::CHARACTERS2[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.' };
const Char32 DigitsKeyListener::CHARACTERS3[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '+', '.' };
const Char32* DigitsKeyListener::CHARACTERS[] ={(Char32*)CHARACTERS0, (Char32*)CHARACTERS1, (Char32*)CHARACTERS2, (Char32*)CHARACTERS3};
const Int32 DigitsKeyListener::SIGN = 1;
const Int32 DigitsKeyListener::DECIMAL = 2;

static AutoPtr< ArrayOf< IDigitsKeyListener* > > InitStatic() {
    AutoPtr< ArrayOf< IDigitsKeyListener* > > instance = ArrayOf<IDigitsKeyListener*>::Alloc(4);
    return instance;
}

AutoPtr< ArrayOf<IDigitsKeyListener*> > DigitsKeyListener::sInstance = InitStatic();

AutoPtr< ArrayOf<Char32> > DigitsKeyListener::GetAcceptedChars()
{
    return mAccepted;
}

DigitsKeyListener::DigitsKeyListener()
    : mSign(FALSE)
    , mDecimal(FALSE)
{}

DigitsKeyListener::~DigitsKeyListener()
{}

void DigitsKeyListener::constructor()
{
    constructor(FALSE, FALSE);
}

void DigitsKeyListener::constructor(
    /* [in] */ Boolean sign,
    /* [in] */ Boolean decimal)
{
    mSign = sign;
    mDecimal = decimal;

    Int32 kind = (sign ? SIGN : 0) | (decimal ? DECIMAL : 0);
    if( 0>kind && kind>3 ) {
        return;
    }
    Int32 acceptedLen = 10;
    if(kind == 3) {
        acceptedLen = 12;
    }
    if( kind==1 && kind==2 ) {
        acceptedLen = 11;
    }

    AutoPtr< ArrayOf<Char32> > charactersR = ArrayOf<Char32>::Alloc(acceptedLen);
    for(Int32 i=0; i<acceptedLen; i++){
        (*charactersR)[i]=CHARACTERS[kind][i];
    }
    mAccepted = charactersR;
}

AutoPtr<IDigitsKeyListener> DigitsKeyListener::GetInstance()
{
    return GetInstance(FALSE, FALSE);
}

AutoPtr<IDigitsKeyListener> DigitsKeyListener::GetInstance(
    /* [in] */ Boolean sign,
    /* [in] */ Boolean decimal)
{
    Int32 kind = (sign ? SIGN : 0) | (decimal ? DECIMAL : 0);
    if (sInstance[kind] != NULL) return (*sInstance)[kind];

    AutoPtr<IDigitsKeyListener> instance;
    CDigitsKeyListener::New(sign, decimal, (IDigitsKeyListener**)&instance);
    sInstance->Set(kind, instance);
    return (*sInstance)[kind];
}

AutoPtr<IDigitsKeyListener> DigitsKeyListener::GetInstance(
    /* [in] */ const String& accepted)
{
    AutoPtr<IDigitsKeyListener> dim;
    CDigitsKeyListener::New((IDigitsKeyListener**)&dim);
    DigitsKeyListener* digits = (DigitsKeyListener*)dim->Probe(EIID_DigitsKeyListener);
    digits->mAccepted = ArrayOf<Char32>::Alloc(accepted.GetLength());
    digits->mAccepted = accepted.GetChars(0, accepted.GetLength());

    return dim;
}

Int32 DigitsKeyListener::GetInputType()
{
    Int32 contentType = IInputType::TYPE_CLASS_NUMBER;
    if (mSign) {
        contentType |= IInputType::TYPE_NUMBER_FLAG_SIGNED;
    }
    if (mDecimal) {
            contentType |= IInputType::TYPE_NUMBER_FLAG_DECIMAL;
    }
    return contentType;
}

AutoPtr<ICharSequence> DigitsKeyListener::Filter(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ISpanned* dest,
    /* [in] */ Int32 dstart,
    /* [in] */ Int32 dend)
{
    AutoPtr<ICharSequence> out = NumberKeyListener::Filter(source, start, end, dest, dstart, dend);

    if (mSign == FALSE && mDecimal == FALSE) {
        return out;
    }

    if (out != NULL) {
        source = out;
        start = 0;
        out->GetLength(&end);
    }

    Int32 sign = -1;
    Int32 decimal = -1;
    Int32 dlen;
    dest->GetLength(&dlen);
    /*
     * Find out if the existing text has a sign or decimal point characters.
     */

    for (Int32 i = 0; i < dstart; i++) {
        Char32 c;
        dest->GetCharAt(i, &c);

        if (IsSignChar(c)) {
            sign = i;
        }
        else if (IsDecimalPointChar(c)) {
            decimal = i;
        }
    }
    for (Int32 i = dend; i < dlen; i++) {
        Char32 c;
        dest->GetCharAt(i, &c);

        if (IsSignChar(c)) {
            return NULL;    // Nothing can be inserted in front of a '-'.
        }
        else if (IsDecimalPointChar(c)) {
            decimal = i;
        }
    }

    /*
     * If it does, we must strip them out from the source.
     * In addition, a sign character must be the very first character,
     * and nothing can be inserted before an existing '-'.
     * Go in reverse order so the offsets are stable.
     */

    AutoPtr<ISpannableStringBuilder> stripped;

    for (Int32 i = end - 1; i >= start; i--) {
        Char32 c;
        source->GetCharAt(i, &c);
        Boolean strip = FALSE;
        if (IsSignChar(c)) {
            if (i != start || dstart != 0) {
                strip = TRUE;
            }
            else if (sign >= 0) {
                strip = TRUE;
            }
            else {
                sign = i;
            }
        }
        else if (IsDecimalPointChar(c)) {
            if (decimal >= 0) {
                strip = TRUE;
            }
            else {
                decimal = i;
            }
        }

        if (strip) {
            if (end == start + 1) {
                return NULL;  // Only one character, and it was stripped.
            }

            if (stripped == NULL) {
                CSpannableStringBuilder::New(source, start, end, (ISpannableStringBuilder**)&stripped);
            }

            stripped->Delete(i - start, i + 1 - start);
        }
    }

    if (stripped != NULL) {
        return stripped;
    }
    else if (out != NULL) {
        return out;
    }
    else {
        return NULL;
    }
}

Boolean DigitsKeyListener::IsSignChar(
    /* [in] */ const Char32 c)
{
    return c == '-' || c == '+';
}

 // TODO: Needs internationalization
Boolean DigitsKeyListener::IsDecimalPointChar(
    /* [in] */ const Char32 c)
{
    return c == '.';
}


} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos
