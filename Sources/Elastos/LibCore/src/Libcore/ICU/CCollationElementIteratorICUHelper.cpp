
#include "CCollationElementIteratorICUHelper.h"
#include "CollationElementIteratorICU.h"

namespace Libcore {
namespace ICU {

ECode CCollationElementIteratorICUHelper::PrimaryOrder(
    /* [in] */ Int32 order,
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value)
    *value = CollationElementIteratorICU::PrimaryOrder(order);
    return NOERROR;
}

ECode CCollationElementIteratorICUHelper::SecondaryOrder(
    /* [in] */ Int32 order,
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value)
    *value = CollationElementIteratorICU::SecondaryOrder(order);
    return NOERROR;
}

ECode CCollationElementIteratorICUHelper::TertiaryOrder(
    /* [in] */ Int32 order,
    /* [out] */ Int32 * value)
{
    VALIDATE_NOT_NULL(value)
    *value = CollationElementIteratorICU::TertiaryOrder(order);
    return NOERROR;
}

ECode CCollationElementIteratorICUHelper::GetInstance(
    /* [in] */ Int32 collatorAddress,
    /* [in] */ const String& source,
    /* [out] */ ICollationElementIteratorICU ** outiterICU)
{
    VALIDATE_NOT_NULL(outiterICU)
    AutoPtr<ICollationElementIteratorICU> icu = CollationElementIteratorICU::GetInstance(collatorAddress,source);
    *outiterICU = icu;
    INTERFACE_ADDREF(*outiterICU);
    return NOERROR;
}

} // namespace ICU
} // namespace Libcore
