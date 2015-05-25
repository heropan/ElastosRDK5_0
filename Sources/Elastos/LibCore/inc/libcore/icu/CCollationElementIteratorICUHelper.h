
#ifndef __ICU_CCOLLATIONELEMENTITERATORICUHELPER_H__
#define __ICU_CCOLLATIONELEMENTITERATORICUHELPER_H__

#include "_CCollationElementIteratorICUHelper.h"

namespace Libcore {
namespace ICU {

CarClass(CCollationElementIteratorICUHelper)
{
public:

    CARAPI PrimaryOrder(
        /* [in] */ Int32 order,
        /* [out] */ Int32 * value);

    CARAPI SecondaryOrder(
        /* [in] */ Int32 order,
        /* [out] */ Int32 * value);

    CARAPI TertiaryOrder(
        /* [in] */ Int32 order,
        /* [out] */ Int32 * value);

    CARAPI GetInstance(
        /* [in] */ Int32 collatorAddress,
        /* [in] */ const String& source,
        /* [out] */ ICollationElementIteratorICU ** outiterICU);
};

} // namespace ICU
} // namespace Libcore

#endif //__ICU_CCOLLATIONELEMENTITERATORICUHELPER_H__
