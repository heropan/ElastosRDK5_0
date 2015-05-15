
#ifndef __CSTRINGCHARACTERIERATOR_H__
#define __CSTRINGCHARACTERIERATOR_H__

#include "_CStringCharacterIterator.h"
#include "StringCharacterIterator.h"
#include <cmdef.h>

namespace Elastos {
namespace Text {

CarClass(CStringCharacterIterator) , public StringCharacterIterator
{
public:
    CARAPI SetText(
        /* [in] */ const String& value);

    CARAPI GetBeginIndex(
        /* [out] */ Int32* index);

    CARAPI Clone(
        /* [out] */ ICharacterIterator** copy);

    CARAPI Current(
        /* [out] */ Char32* character);

    CARAPI GetEndIndex(
        /* [out] */ Int32* index);

    CARAPI First(
        /* [out] */ Char32* character);

    CARAPI GetIndex(
        /* [out] */ Int32* index);

    CARAPI Last(
        /* [out] */ Char32* character);

    CARAPI Next(
        /* [out] */ Char32* character);

    CARAPI Previous(
        /* [out] */ Char32* character);

    CARAPI SetIndex(
        /* [in] */ Int32 location,
        /* [out] */ Char32* character);

    CARAPI constructor(
        /* [in] */ const String& value);

    CARAPI constructor(
        /* [in] */ const String& value,
        /* [in] */ Int32 location);

    CARAPI constructor(
        /* [in] */ const String& value,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 location);
};

} // namespace Text
} // namespace Elastos

#endif // __CSTRINGCHARACTERIERATOR_H__
