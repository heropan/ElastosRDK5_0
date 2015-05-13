#include "util/CharSequences.h"
#include "elastos/Character.h"

using Elastos::Core::Character;

namespace Elastos {
namespace Droid {
namespace Utility {

AutoPtr<ICharSequence> CharSequences::ForAsciiBytes(
     /* [in] */ ArrayOf<Byte>* bytes)
{
    // return new CharSequence() {
    //     public char charAt(int index) {
    //         return (char) bytes[index];
    //     }

    //     public int length() {
    //         return bytes.length;
    //     }

    //     public CharSequence subSequence(int start, int end) {
    //         return forAsciiBytes(bytes, start, end);
    //     }

    //     public String toString() {
    //         return new String(bytes);
    //     }
    // };
    assert(0 && "TODO");
    return NULL;
}

AutoPtr<ICharSequence> CharSequences::ForAsciiBytes(
    /* [in] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    // validate(start, end, bytes.length);
    // return new CharSequence() {
    //     public char charAt(int index) {
    //         return (char) bytes[index + start];
    //     }

    //     public int length() {
    //         return end - start;
    //     }

    //     public CharSequence subSequence(int newStart, int newEnd) {
    //         newStart -= start;
    //         newEnd -= start;
    //         validate(newStart, newEnd, length());
    //         return forAsciiBytes(bytes, newStart, newEnd);
    //     }

    //     public String toString() {
    //         return new String(bytes, start, length());
    //     }
    // };
    assert(0 && "TODO");
    return NULL;
}

ECode CharSequences::Validate(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 length)
{
    if (start < 0) return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    if (end < 0) return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    if (end > length) return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    if (start > end) return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    return NOERROR;
}

Boolean CharSequences::Equals(
     /* [in] */ ICharSequence* a,
     /* [in] */ ICharSequence* b)
{
    if (a == b) {
        return TRUE;
    }
    if (a == NULL || b == NULL) {
        return FALSE;
    }
    Int32 al, bl;
    a->GetLength(&al);
    b->GetLength(&bl);
    if (al != bl) {
        return FALSE;
    }

    Char32 ac, bc;
    for (Int32 i = 0; i < al; i++) {
        a->GetCharAt(i, &ac);
        b->GetCharAt(i, &bc);
        if (ac != bc) {
            return FALSE;
        }
    }
    return TRUE;
}

Int32 CharSequences::CompareToIgnoreCase(
     /* [in] */ ICharSequence* a,
     /* [in] */ ICharSequence* b)
{
    // Code adapted from String#compareTo
    Int32 al, bl;
    a->GetLength(&al);
    b->GetLength(&bl);
    Int32 aPos = 0, bPos = 0, result;
    int end = (al < bl) ? al : bl;

    Char32 ac, bc;
    while (aPos < end) {
        a->GetCharAt(aPos++, &ac);
        b->GetCharAt(bPos++, &bc);
        if ((result = Character::ToLowerCase(ac) - Character::ToLowerCase(bc)) != 0) {
            return result;
        }
    }
    return al - bl;
}


} // namespace Utility
} // namespace Droid
} // namespace Elastos
