#include "elastos/droid/internal/utility/CCharSequences.h"
#include "elastos/droid/ext/frameworkdef.h"
#include <elastos/core/Character.h>

using Elastos::Core::Character;
using Elastos::Core::EIID_ICharSequence;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

CAR_INTERFACE_IMPL(CCharSequences::AsciiBytesCharSequence1, Object, ICharSequence)

CCharSequences::AsciiBytesCharSequence1::AsciiBytesCharSequence1(
    /* [in] */ ArrayOf<Byte>* bytes)
    : mBytes(bytes)
{}

ECode CCharSequences::AsciiBytesCharSequence1::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    VALIDATE_NOT_NULL(c)
    *c = (Char32)(*mBytes)[index];
    return NOERROR;
}

ECode CCharSequences::AsciiBytesCharSequence1::GetLength(
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length)
    *length = mBytes->GetLength();
    return NOERROR;
}

ECode CCharSequences::AsciiBytesCharSequence1::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** cs)
{
    VALIDATE_NOT_NULL(cs)
    return _ForAsciiBytes(mBytes, start, end, cs);
}

ECode CCharSequences::AsciiBytesCharSequence1::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    *str = String(*mBytes);
    return NOERROR;
}

CAR_INTERFACE_IMPL(CCharSequences::AsciiBytesCharSequence2, Object, ICharSequence)

CCharSequences::AsciiBytesCharSequence2::AsciiBytesCharSequence2(
    /* [in] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
    : mBytes(bytes)
    , mStart(start)
    , mEnd(end)
{}

ECode CCharSequences::AsciiBytesCharSequence2::GetCharAt(
    /* [in] */ Int32 index,
    /* [out] */ Char32* c)
{
    VALIDATE_NOT_NULL(c)
    *c = (Char32)(*mBytes)[index + mStart];
    return NOERROR;
}

ECode CCharSequences::AsciiBytesCharSequence2::GetLength(
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length)
    *length = mEnd - mStart;
    return NOERROR;
}

ECode CCharSequences::AsciiBytesCharSequence2::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** cs)
{
    VALIDATE_NOT_NULL(cs)
    start -= mStart;
    end -= mStart;
    FAIL_RETURN(Validate(start, end, mEnd - mStart));
    return _ForAsciiBytes(mBytes, start, end, cs);
}

ECode CCharSequences::AsciiBytesCharSequence2::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    *str = String(*mBytes, mStart, mEnd - mStart);
    return NOERROR;
}

CAR_INTERFACE_IMPL(CCharSequences, Singleton, ICharSequences)

CAR_SINGLETON_IMPL(CCharSequences)

AutoPtr<ICharSequence> CCharSequences::_ForAsciiBytes(
     /* [in] */ ArrayOf<Byte>* bytes)
{
    return new AsciiBytesCharSequence1(bytes);
}

ECode CCharSequences::_ForAsciiBytes(
    /* [in] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** cs)
{
    VALIDATE_NOT_NULL(cs)
    FAIL_RETURN(Validate(start, end, bytes->GetLength()));
    *cs = new AsciiBytesCharSequence2(bytes, start, end);
    REFCOUNT_ADD(*cs)
    return NOERROR;
}

ECode CCharSequences::Validate(
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

Boolean CCharSequences::_Equals(
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

Int32 CCharSequences::_CompareToIgnoreCase(
     /* [in] */ ICharSequence* a,
     /* [in] */ ICharSequence* b)
{
    // Code adapted from String#compareTo
    Int32 al, bl;
    a->GetLength(&al);
    b->GetLength(&bl);
    Int32 aPos = 0, bPos = 0, result;
    Int32 end = (al < bl) ? al : bl;

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

ECode CCharSequences::ForAsciiBytes(
    /* [in] */ ArrayOf<Byte>* bytes,
    /* [out] */ ICharSequence** cs)
{
    VALIDATE_NOT_NULL(cs)
    AutoPtr<ICharSequence> temp = _ForAsciiBytes(bytes);
    *cs = temp;
    REFCOUNT_ADD(*cs)
    return NOERROR;
}

ECode CCharSequences::ForAsciiBytes(
    /* [in] */ ArrayOf<Byte>* bytes,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** cs)
{
    return _ForAsciiBytes(bytes, start, end, cs);
}

ECode CCharSequences::Equals(
    /* [in] */ ICharSequence* a,
    /* [in] */ ICharSequence* b,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = _Equals(a, b);
    return NOERROR;
}

ECode CCharSequences::CompareToIgnoreCase(
    /* [in] */ ICharSequence* me,
    /* [in] */ ICharSequence* another,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = _CompareToIgnoreCase(me, another);
    return NOERROR;
}

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos
