#ifndef __STRINGCHARACTERITERATOR_H__
#define __STRINGCHARACTERITERATOR_H__

#include "Object.h"

using Elastos::Core::Object;

namespace Elastos {
namespace Text {

class StringCharacterIterator
    : public Object
    , public ICharacterIterator
{
public:
    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ const String& value);

    CARAPI constructor(
        /* [in] */ const String& value,
        /* [in] */ Int32 location);

    CARAPI constructor(
        /* [in] */ const String& value,
        /* [in] */ int start,
        /* [in] */ int end,
        /* [in] */ int location);

    CARAPI Clone(
        /* [out] */ ICharacterIterator** copy);

    CARAPI Current(
        /* [out] */ Char32* currentCharacter);

    //@Override
    //public boolean equals(Object object);

    CARAPI First(
        /* [out] */ Char32* firstCharacter);

    CARAPI GetBeginIndex(
        /* [out] */ Int32* index);

    CARAPI GetEndIndex(
        /* [out] */ Int32* index);

    CARAPI GetIndex(
        /* [out] */ Int32* index);

    //@Override
    //public int hashCode();

    CARAPI Last(
        /* [out] */ Char32* endCharacter);

    CARAPI Next(
        /* [out] */ Char32* nextCharacter);

    CARAPI Previous(
        /* [out] */ Char32* previousCharacter);

    CARAPI SetIndex(
        /* [in] */ Int32 location,
        /* [out] */ Char32* index);

    CARAPI SetText(
        /* [in] */ const String& value);

protected:
    String string;

    Int32 start;

    Int32 end;

    Int32 offset;
};

} // namespace Text
} // namespace Elastos

#endif //__STRINGCHARACTERITERATOR_H__
