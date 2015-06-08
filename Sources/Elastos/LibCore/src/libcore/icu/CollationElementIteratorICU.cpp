
#include "CollationElementIteratorICU.h"
#include "NativeCollation.h"

namespace Libcore {
namespace ICU {


const Int32 CollationElementIteratorICU::PRIMARY_ORDER_MASK_ = 0xffff0000;
const Int32 CollationElementIteratorICU::SECONDARY_ORDER_MASK_ = 0x0000ff00;
const Int32 CollationElementIteratorICU::TERTIARY_ORDER_MASK_ = 0x000000ff;
const Int32 CollationElementIteratorICU::PRIMARY_ORDER_SHIFT_ = 16;
const Int32 CollationElementIteratorICU::SECONDARY_ORDER_SHIFT_ = 8;
const Int32 CollationElementIteratorICU::UNSIGNED_16_BIT_MASK_ = 0x0000FFFF;

CollationElementIteratorICU::CollationElementIteratorICU()
{
}

CollationElementIteratorICU::CollationElementIteratorICU(
    /* [in] */ Int32 address)
    : mAddress(address)
{ }

CollationElementIteratorICU::~CollationElementIteratorICU()
{
    //try {
    NativeCollation::CloseElements(mAddress);
    //} finally {
    //    super.finalize();
    //}
}

CAR_INTERFACE_IMPL(CollationElementIteratorICU, ICollationElementIteratorICU);

ECode CollationElementIteratorICU::Reset()
{
    NativeCollation::Reset(mAddress);
    return NOERROR;
}

ECode CollationElementIteratorICU::Next(
    /* [out] */ Int32* nextValue)
{
    VALIDATE_NOT_NULL(nextValue);
    return NativeCollation::Next(mAddress, nextValue);
}

ECode CollationElementIteratorICU::Previous(
    /* [out] */ Int32* previousValue)
{
    VALIDATE_NOT_NULL(previousValue);
    return NativeCollation::Previous(mAddress, previousValue);
}

ECode CollationElementIteratorICU::GetMaxExpansion(
    /* [in] */ Int32 order,
    /* [out] */ Int32* maxExpansion)
{
    VALIDATE_NOT_NULL(maxExpansion);
    *maxExpansion = NativeCollation::GetMaxExpansion(mAddress, order);
    return NOERROR;
}

ECode CollationElementIteratorICU::SetText(
    /* [in] */ const String& source)
{
    return NativeCollation::SetText(mAddress, source);
}

ECode CollationElementIteratorICU::SetText(
    /* [in] */ ICharacterIterator* source)
{
    return NativeCollation::SetText(mAddress,
            String("CharacterIterator")/*source.toString()*/);
}

ECode CollationElementIteratorICU::GetOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = NativeCollation::GetOffset(mAddress);
    return NOERROR;
}

ECode CollationElementIteratorICU::SetOffset(
    /* [in] */ Int32 offset)
{
    return NativeCollation::SetOffset(mAddress, offset);
}

Int32 CollationElementIteratorICU::PrimaryOrder(
    /* [in] */ Int32 order)
{
    return ((order & PRIMARY_ORDER_MASK_) >> PRIMARY_ORDER_SHIFT_) &
            UNSIGNED_16_BIT_MASK_;
}

Int32 CollationElementIteratorICU::SecondaryOrder(
    /* [in] */ Int32 order)
{
    return (order & SECONDARY_ORDER_MASK_) >> SECONDARY_ORDER_SHIFT_;
}

Int32 CollationElementIteratorICU::TertiaryOrder(
    /* [in] */ Int32 order)
{
    return order & TERTIARY_ORDER_MASK_;
}

AutoPtr<ICollationElementIteratorICU> CollationElementIteratorICU::GetInstance(
    /* [in] */ Int32 collatorAddress,
    /* [in] */ const String& source)
{
    Int32 iteratorAddress = 0;
    NativeCollation::GetCollationElementIterator(collatorAddress, source , &iteratorAddress);
    return (ICollationElementIteratorICU*)new CollationElementIteratorICU(iteratorAddress);
}

} // namespace ICU
} // namespace Libcore
