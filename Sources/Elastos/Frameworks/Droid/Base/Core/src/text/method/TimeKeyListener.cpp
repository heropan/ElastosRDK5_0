#include "text/method/TimeKeyListener.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

const Char32 TimeKeyListener::CHARACTERS[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'm', 'p', ':' };

Int32 TimeKeyListener::GetInputType()
{
    return IInputType::TYPE_CLASS_DATETIME | IInputType::TYPE_DATETIME_VARIATION_TIME;
}

AutoPtr< ArrayOf<Char32> > TimeKeyListener::GetAcceptedChars()
{
    AutoPtr< ArrayOf<Char32> > charactersR = ArrayOf<Char32>::Alloc(14);
    for(Int32 i=0; i<14; i++){
        (*charactersR)[i]=CHARACTERS[i];
    }
    return charactersR;
}

AutoPtr< ArrayOf<Char32> > TimeKeyListener::GetCHARACTERS()
{
    AutoPtr< ArrayOf<Char32> > charactersR = ArrayOf<Char32>::Alloc(14);
    for(Int32 i=0; i<14; i++){
        (*charactersR)[i]=CHARACTERS[i];
    }
    return charactersR;
}

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos