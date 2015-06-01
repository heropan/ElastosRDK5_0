#ifndef __RULEBASEDCOLLATOR_H__
#define __RULEBASEDCOLLATOR_H__

#include "cmdef.h"
#include "Elastos.CoreLibrary_server.h"
#include "Collator.h"

using Libcore::ICU::ICollationElementIteratorICU;
using Elastos::Text::ICharacterIterator;
using Elastos::Text::ICollationKey;

namespace Elastos {
namespace Text {

class RuleBasedCollator : public Collator
{
protected:
    CARAPI Init(
        /* [in] */ IRuleBasedCollatorICU* wrapper);

    CARAPI Init(
        /* [in] */ const String& rules);

public:
    virtual CARAPI GetCollationElementIterator(
        /* [in] */ ICharacterIterator* source,
        /* [out] */ ICollationElementIterator** collationElementIterator);

    virtual CARAPI GetCollationElementIterator(
        /* [in] */ const String& source,
        /* [out] */ ICollationElementIterator** collationElementIterator);

    virtual CARAPI GetRules(
        /* [out] */ String* rules);

    //@Override
    CARAPI Compare(
        /* [in] */ const String& source,
        /* [in] */ const String& target,
        /* [out] */ Int32* value);

    //@Override
    CARAPI GetCollationKey(
        /* [in] */ const String& source,
        /* [out] */ ICollationKey ** collationKey);

    //@Override
    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI Equals(
        /* [in] */ const String& string1,
        /* [in] */ const String& string2,
        /* [out] */ Boolean* result);
};

} // namespace Text
} // namespace Elastos

#endif //__RULEBASEDCOLLATOR_H__