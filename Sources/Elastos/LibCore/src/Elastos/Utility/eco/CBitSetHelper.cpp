
#include "CBitSetHelper.h"
#include "CBitSet.h"

namespace Elastos {
namespace Utility {

ECode CBitSetHelper::ValueOfInt64Array(
    /* [in] */ const ArrayOf<Int64>& int64Arr,
    /* [out] */ IBitSet** bs)
{
    return CBitSet::ValueOfInt64Array(int64Arr, bs);
}

/**
 * Returns a {@code BitSet} corresponding to {@code longBuffer}, interpreted as a little-endian
 * sequence of bits. This method does not alter the {@code LongBuffer}.
 * @hide 1.7
 */
ECode CBitSetHelper::ValueOfInt64Buffer(
    /* [in] */ IInt64Buffer* int64Arr,
    /* [out] */ IBitSet** bs)
{
    return CBitSet::ValueOfInt64Buffer(int64Arr, bs);
}

/**
 * Equivalent to {@code BitSet.valueOf(ByteBuffer.wrap(bytes))}.
 * @hide 1.7
 */
ECode CBitSetHelper::ValueOfByteArray(
    /* [in] */ const ArrayOf<Byte>& byteArr,
    /* [out] */ IBitSet** bs)
{
    return CBitSet::ValueOfByteArray(byteArr, bs);
}

/**
 * Returns a {@code BitSet} corresponding to {@code byteBuffer}, interpreted as a little-endian
 * sequence of bits. This method does not alter the {@code ByteBuffer}.
 * @hide 1.7
 */
ECode CBitSetHelper::ValueOfByteBuffer(
    /* [in] */ IByteBuffer* byteArr,
    /* [out] */ IBitSet** bs)
{
    return CBitSet::ValueOfByteBuffer(byteArr, bs);
}

} // namespace Utility
} // namespace Elastos
