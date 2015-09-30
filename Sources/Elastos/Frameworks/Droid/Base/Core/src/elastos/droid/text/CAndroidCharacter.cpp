
#include "elastos/droid/text/CAndroidCharacter.h"

namespace Elastos {
namespace Droid {
namespace Text {

ECode CAndroidCharacter::GetDirectionalities(
    /* [in] */ ArrayOf<Char32>* src,
    /* [in] */ ArrayOf<Byte>* dest,
    /* [in] */ Int32 count)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

Int32 CAndroidCharacter::GetEastAsianWidth(
    /* [in] */ Char32 input)
{
    // TODO: Add your code here
    return 0;
}

ECode CAndroidCharacter::GetEastAsianWidths(
    /* [in] */ ArrayOf<Char32>* src,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Byte>* dest)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

Boolean CAndroidCharacter::Mirror(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count)
{
    // TODO: Add your code here
    return FALSE;
}

Char32 CAndroidCharacter::GetMirror(
    /* [in] */ Char32 ch)
{
    // TODO: Add your code here
    return ' ';
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos