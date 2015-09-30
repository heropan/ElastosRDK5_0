#include "elastos/droid/text/method/DateKeyListener.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

const Char32 DateKeyListener::CHARACTERS[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '/', '-', '.' };

Int32 DateKeyListener::GetInputType()
{
    return IInputType::TYPE_CLASS_DATETIME | IInputType::TYPE_DATETIME_VARIATION_DATE;
}

AutoPtr< ArrayOf<Char32> > DateKeyListener::GetAcceptedChars()
{
    AutoPtr< ArrayOf<Char32> > charactersR = ArrayOf<Char32>::Alloc(13);
    for(Int32 i=0; i<13; i++){
        (*charactersR)[i]=CHARACTERS[i];
    }
    return charactersR;
}

AutoPtr< ArrayOf<Char32> > DateKeyListener::GetCHARACTERS()
{
    AutoPtr< ArrayOf<Char32> > charactersR = ArrayOf<Char32>::Alloc(13);
    for(Int32 i=0; i<13; i++){
        (*charactersR)[i]=CHARACTERS[i];
    }
    return charactersR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos