
#include "elastos/droid/text/CAndroidCharacterHelper.h"
#include "elastos/droid/text/CAndroidCharacter.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {

ECode CAndroidCharacterHelper::GetDirectionalities(
    /* [in] */ ArrayOf<Char32>* src,
    /* [in] */ ArrayOf<Byte>* dest,
    /* [in] */ Int32 count)
{
    return CAndroidCharacter::GetDirectionalities(src, dest, count);
}

ECode CAndroidCharacterHelper::GetEastAsianWidth(
    /* [in] */ Char32 input,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = CAndroidCharacter::GetEastAsianWidth(input);
    return NOERROR;
}

ECode CAndroidCharacterHelper::GetEastAsianWidths(
    /* [in] */ ArrayOf<Char32>* src,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Byte>* dest)
{
    return CAndroidCharacter::GetEastAsianWidths(src, start, count, dest);
}

ECode CAndroidCharacterHelper::Mirror(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = CAndroidCharacter::Mirror(text, start, count);
    return NOERROR;
}

ECode CAndroidCharacterHelper::GetMirror(
    /* [in] */ Char32 ch,
    /* [out] */ Char32* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = CAndroidCharacter::GetMirror(ch);
    return NOERROR;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos

