
#ifndef __CCONTACTSCONTRACTCONTACTSAGGREGATIONSUGGESTIONS_H__
#define __CCONTACTSCONTRACTCONTACTSAGGREGATIONSUGGESTIONS_H__

#include "_CContactsContractContactsAggregationSuggestions.h"

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsContractContactsAggregationSuggestions)
{
public:
    /**
     * No public constructor since this is a utility class
     */
    CARAPI constructor();

    /**
     * @hide
     */
    CARAPI Builder(
        /* [out] */ IContactsContractContactsAggregationSuggestionsBuilder** build);
};

} //Provider
} //Droid
} //Elastos

#endif //__CCONTACTSCONTRACTCONTACTSAGGREGATIONSUGGESTIONS_H__
