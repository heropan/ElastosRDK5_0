
#ifndef __ICU_CRULEBASEDCOLLATORICU_H__
#define __ICU_CRULEBASEDCOLLATORICU_H__

#include "_CRuleBasedCollatorICU.h"

using Elastos::Text::ICollationKey;
using Elastos::Text::ICharacterIterator;

namespace Libcore {
namespace ICU {

CarClass(CRuleBasedCollatorICU)
{
public:

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CRuleBasedCollatorICU();

    ~CRuleBasedCollatorICU();

    CARAPI constructor(
        /* [in] */ const String& rules);

    CARAPI constructor(
        /* [in] */ ILocale * locale);

    CARAPI Clone(
        /* [out] */ IRuleBasedCollatorICU ** outruleicu);

    CARAPI Compare(
        /* [in] */ const String& source,
        /* [in] */ const String& target,
        /* [out] */ Int32 * value);

    CARAPI GetDecomposition(
        /* [out] */ Int32 * value);

    CARAPI SetDecomposition(
        /* [in] */ Int32 mode);

    CARAPI GetStrength(
        /* [out] */ Int32 * value);

    CARAPI SetStrength(
        /* [in] */ Int32 strength);

    CARAPI SetAttribute(
        /* [in] */ Int32 type,
        /* [in] */ Int32 value);

    CARAPI GetAttribute(
        /* [in] */ Int32 type,
        /* [out] */ Int32 * value);

    CARAPI GetCollationKey(
        /* [in] */ const String& source,
        /* [out] */ ICollationKey ** outkey);

    CARAPI GetRules(
        /* [out] */ String * str);

    CARAPI GetCollationElementIterator(
        /* [in] */ const String& source,
        /* [out] */ ICollationElementIteratorICU ** outicu);

    CARAPI GetCollationElementIteratorEx(
        /* [in] */ ICharacterIterator * it,
        /* [out] */ ICollationElementIteratorICU ** outicu);

    // @Override
    CARAPI GetHashCode(
        /* [out] */ Int32 * value);

    CARAPI EqualsEx(
        /* [in] */ const String& source,
        /* [in] */ const String& target,
        /* [out] */ Boolean * value);

    // @Override
    CARAPI Equals(
        /* [in] */ IInterface * object,
        /* [out] */ Boolean * value);

private:
    CRuleBasedCollatorICU(
        /* [in] */ Int32 address);

    CARAPI_(String) CharacterIteratorToString(
        /* [in] */ ICharacterIterator * it);

private:
    Int32 mAddress;
};

} // namespace ICU
} // namespace Libcore

#endif //__ICU_CRULEBASEDCOLLATORICU_H__
