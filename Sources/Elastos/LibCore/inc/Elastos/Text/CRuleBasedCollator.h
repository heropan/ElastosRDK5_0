
#ifndef __CRULEBASEDCOLLATOR_H__
#define __CRULEBASEDCOLLATOR_H__

#include "_CRuleBasedCollator.h"
#include "RuleBasedCollator.h"

using Libcore::ICU::IRuleBasedCollatorICU;
using Elastos::Text::ICharacterIterator;
using Elastos::Text::ICollationKey;

namespace Elastos {
namespace Text {

CarClass(CRuleBasedCollator) , public RuleBasedCollator
{
public:

    CARAPI constructor(
        /* [in] */ IRuleBasedCollatorICU* pWrapper);

    CARAPI constructor(
        /* [in] */ const String& rules);

    CARAPI Compare(
        /* [in] */ IInterface* object1,
        /* [in] */ IInterface* object2,
        /* [out] */ Int32* result);

    CARAPI CompareEx(
        /* [in] */ const String& string1,
        /* [in] */ const String& string2,
        /* [out] */ Int32* value);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI EqualsEx(
        /* [in] */ const String& string1,
        /* [in] */ const String& string2,
        /* [out] */ Boolean* result);

    CARAPI GetCollationKey(
        /* [in] */ const String& string,
        /* [out] */ ICollationKey** collationKey);

    CARAPI GetDecomposition(
        /* [out] */ Int32 * pDecomposition);

    CARAPI GetStrength(
        /* [out] */ Int32 * pStrength);

    CARAPI SetDecomposition(
        /* [in] */ Int32 value);

    CARAPI SetStrength(
        /* [in] */ Int32 value);

    CARAPI GetCollationElementIteratorEx(
        /* [in] */ ICharacterIterator * pSource,
        /* [out] */ ICollationElementIterator ** ppCollationElementIterator);

    CARAPI GetCollationElementIterator(
        /* [in] */ const String& source,
        /* [out] */ ICollationElementIterator ** ppCollationElementIterator);

    CARAPI GetRules(
        /* [out] */ String * pRules);

    CARAPI GetICUCollator(
        /* [out] */ IRuleBasedCollatorICU** icuCollator);
};

} // namespace Text
} // namespace Elastos

#endif // __CRULEBASEDCOLLATOR_H__
