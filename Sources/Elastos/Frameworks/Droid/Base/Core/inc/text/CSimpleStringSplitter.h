
#ifndef __ELASTOS_DROID_TEXT_SIMPLESTRINGSPLITTER_H__
#define __ELASTOS_DROID_TEXT_SIMPLESTRINGSPLITTER_H__

#include "_CSimpleStringSplitter.h"

using Elastos::Utility::IIterator;

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CSimpleStringSplitter)
{
public:
    CARAPI constructor(
        /* [in] */ Char32 delimiter);

    CARAPI SetString(
        /*  [in] */ const String& string);

   CARAPI GetIterator(
       /* [out] */ IIterator** it);

    CARAPI HasNext(
        /* [out] */ Boolean* result);

    CARAPI Next(
        /* [out] */ IInterface** object);

    CARAPI NextString(
        /* [out] */ String* str);

    CARAPI Remove();

private:
    String mString;
    Char32 mDelimiter;
    Int32 mPosition;
    Int32 mLength;
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TEXT_SIMPLESTRINGSPLITTER_H__
