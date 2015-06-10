
#include "CCollationElementIterator.h"

namespace Elastos {
namespace Text {

ECode CCollationElementIterator::constructor(
    /* [in] */ ICollationElementIteratorICU* iterator)
{
    mIcuIterator = iterator;
    return NOERROR;
}

ECode CCollationElementIterator::GetMaxExpansion(
    /* [in] */ Int32 order,
    /* [out] */ Int32* maxExpansion)
{
    return mIcuIterator->GetMaxExpansion(order, maxExpansion);
}

ECode CCollationElementIterator::GetOffset(
    /* [out] */ Int32* offset)
{
    return mIcuIterator->GetOffset(offset);
}

ECode CCollationElementIterator::GetNext(
    /* [out] */ Int32* nextValue)
{
    return mIcuIterator->Next(nextValue);
}

ECode CCollationElementIterator::Previous(
    /* [out] */ Int32* previousValue)
{
    return mIcuIterator->Previous(previousValue);
}

ECode CCollationElementIterator::Reset()
{
    return mIcuIterator->Reset();
}

ECode CCollationElementIterator::SetOffset(
    /* [in] */ Int32 newOffset)
{
    return mIcuIterator->SetOffset(newOffset);
}

ECode CCollationElementIterator::SetText(
    /* [in] */ ICharacterIterator* source)
{
    return mIcuIterator->SetText(source);
}

ECode CCollationElementIterator::SetText(
    /* [in] */ const String& source)
{
    return mIcuIterator->SetText(source);
}

} // namespace Text
} // namespace Elastos