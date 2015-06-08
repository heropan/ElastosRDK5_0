
#ifndef __COLLATIONELEMENTITERATORICU_H__
#define __COLLATIONELEMENTITERATORICU_H__

#include "Elastos.CoreLibrary_server.h"

using Elastos::Text::ICharacterIterator;

namespace Libcore {
namespace ICU {

class CollationElementIteratorICU
    : public ElRefBase
    , public ICollationElementIteratorICU
{
public:
    CollationElementIteratorICU();

    ~CollationElementIteratorICU();

    CAR_INTERFACE_DECL();

    CARAPI Reset();

    CARAPI Next(
        /* [out] */ Int32* nextValue);

    CARAPI Previous(
        /* [out] */ Int32* previousValue);

    CARAPI GetMaxExpansion(
        /* [in] */ Int32 order,
        /* [out] */ Int32* maxExpansion);

    CARAPI SetText(
        /* [in] */ const String& source);

    CARAPI SetText(
        /* [in] */ ICharacterIterator* source);

    CARAPI GetOffset(
        /* [out] */ Int32* offset);

    CARAPI SetOffset(
        /* [in] */ Int32 offset);

    static CARAPI_(Int32) PrimaryOrder(
        /* [in] */ Int32 order);

    static CARAPI_(Int32) SecondaryOrder(
        /* [in] */ Int32 order);

    static CARAPI_(Int32) TertiaryOrder(
        /* [in] */ Int32 order);

    static CARAPI_(AutoPtr<ICollationElementIteratorICU>) GetInstance(
        /* [in] */ Int32 collatorAddress,
        /* [in] */ const String& source);

private:
    CollationElementIteratorICU(
        /* [in] */ Int32 collelemiteratoraddress);

private:
    Int32 mAddress;

    const static Int32 PRIMARY_ORDER_MASK_;

    const static Int32 SECONDARY_ORDER_MASK_;

    const static Int32 TERTIARY_ORDER_MASK_;

    const static Int32 PRIMARY_ORDER_SHIFT_;

    const static Int32 SECONDARY_ORDER_SHIFT_;

    const static Int32 UNSIGNED_16_BIT_MASK_;
};

} // namespace ICU
} // namespace Libcore

#endif // __COLLATIONELEMENTITERATORICU_H__
