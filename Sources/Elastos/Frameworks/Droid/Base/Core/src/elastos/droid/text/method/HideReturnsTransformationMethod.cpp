#include "text/method/HideReturnsTransformationMethod.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

Char32 HideReturnsTransformationMethod::ORIGINAL[] = { '\r' };
Char32 HideReturnsTransformationMethod::REPLACEMENT[] = { 0xFEFF/*'\uFEFF'*/ };

AutoPtr< ArrayOf<Char32> > HideReturnsTransformationMethod::GetOriginal()
{
    Int32 aryLen = 1;
    AutoPtr< ArrayOf<Char32> > charactersR = ArrayOf<Char32>::Alloc(aryLen);
    for(Int32 i=0; i<aryLen; i++){
        (*charactersR)[i]=ORIGINAL[i];
    }
    return charactersR;
}

AutoPtr< ArrayOf<Char32> > HideReturnsTransformationMethod::GetReplacement()
{
    Int32 aryLen = 1;
    AutoPtr< ArrayOf<Char32> > charactersR = ArrayOf<Char32>::Alloc(aryLen);
    for(Int32 i=0; i<aryLen; i++){
        (*charactersR)[i]=REPLACEMENT[i];
    }
    return charactersR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos
