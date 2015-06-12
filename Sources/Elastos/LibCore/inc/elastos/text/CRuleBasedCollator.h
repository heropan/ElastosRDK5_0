
#ifndef __ELASTOS_TEXT_CRULEBASEDCOLLATOR_H__
#define __ELASTOS_TEXT_CRULEBASEDCOLLATOR_H__

#include "_Elastos_Text_CRuleBasedCollator.h"
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

    CARAPI Compare(
        /* [in] */ const String& string1,
        /* [in] */ const String& string2,
        /* [out] */ Int32* value);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI Equals(
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

    CARAPI GetCollationElementIterator(
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

#endif // __ELASTOS_TEXT_CRULEBASEDCOLLATOR_H__
