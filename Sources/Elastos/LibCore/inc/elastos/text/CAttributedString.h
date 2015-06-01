
#ifndef __CATTRIBUTEDSTRING_H__
#define __CATTRIBUTEDSTRING_H__

#include "_CAttributedString.h"
#include "AttributedString.h"

// using Elastos::Utility::IObjectMap;

namespace Elastos {
namespace Text {

CarClass(CAttributedString) , public AttributedString
{
public:
    CARAPI constructor(
        /* [in] */ IAttributedCharacterIterator* iterator);

    CARAPI constructor(
        /* [in] */ IAttributedCharacterIterator* iterator,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI constructor(
        /* [in] */ IAttributedCharacterIterator* iterator,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<IAttributedCharacterIteratorAttribute*>* attributes);

    CARAPI constructor(
        /* [in] */ const String& value);

    // CARAPI constructor(
    //     /* [in] */ const String& value,
    //     /* [in] */ IObjectMap * attributes);

    CARAPI AddAttribute(
        /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
        /* [in] */ IInterface* value);

    CARAPI AddAttribute(
        /* [in] */ IAttributedCharacterIteratorAttribute* attribute,
        /* [in] */ IInterface* value,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI GetIterator(
        /* [out] */ IAttributedCharacterIterator** iterator);

    CARAPI GetIterator(
        /* [in] */ ArrayOf<IAttributedCharacterIteratorAttribute*>* attributes,
        /* [out] */ IAttributedCharacterIterator** iterator);

    CARAPI GetIterator(
        /* [in] */ ArrayOf<IAttributedCharacterIteratorAttribute*>* attributes,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ IAttributedCharacterIterator** iterator);

    // CARAPI AddAttributes(
    //     /* [in] */ IObjectMap * attributes,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end);

};

} // namespace Text
} // namespace Elastos

#endif // __CATTRIBUTEDSTRING_H__
