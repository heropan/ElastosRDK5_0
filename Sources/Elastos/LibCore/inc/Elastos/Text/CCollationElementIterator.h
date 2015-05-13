
#ifndef __CCOLLATIONELEMENTITERATOR_H__
#define __CCOLLATIONELEMENTITERATOR_H__

#include "_CCollationElementIterator.h"

using Libcore::ICU::ICollationElementIteratorICU;
using Elastos::Text::ICharacterIterator;

namespace Elastos {
namespace Text {

CarClass(CCollationElementIterator)
{
public:
    CARAPI constructor(
        /* [in] */ ICollationElementIteratorICU* iterator);

    CARAPI GetMaxExpansion(
        /* [in] */ Int32 order,
        /* [out] */ Int32* maxExpansion);

    CARAPI GetOffset(
        /* [out] */ Int32* offset);

    CARAPI Next(
        /* [out] */ Int32* nextValue);

    CARAPI Previous(
        /* [out] */ Int32* previousValue);

    CARAPI Reset();

    CARAPI SetOffset(
        /* [in] */ Int32 newOffset);

    CARAPI SetText(
        /* [in] */ ICharacterIterator* source);

    CARAPI SetTextEx(
        /* [in] */ const String& source);

private:
    AutoPtr<ICollationElementIteratorICU> mIcuIterator;
};

} // namespace Text
} // namespace Elastos

#endif // __CCOLLATIONELEMENTITERATOR_H__
