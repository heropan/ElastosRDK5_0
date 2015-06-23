
#include "provider/CContactsContractContactsAggregationSuggestions.h"
#include "provider/CContactsContractContactsAggregationSuggestionsBuilder.h"
#include <elastos/coredef.h>

using Elastos::Droid::Provider::IContactsContractContactsAggregationSuggestionsBuilder;
using Elastos::Droid::Provider::CContactsContractContactsAggregationSuggestionsBuilder;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractContactsAggregationSuggestions::constructor()
{
    return NOERROR;
}

ECode CContactsContractContactsAggregationSuggestions::Builder(
    /* [out] */ IContactsContractContactsAggregationSuggestionsBuilder** build)
{
    AutoPtr<IContactsContractContactsAggregationSuggestionsBuilder> builder;
    FAIL_RETURN(CContactsContractContactsAggregationSuggestionsBuilder::New((IContactsContractContactsAggregationSuggestionsBuilder**)&builder))
    *build = builder;
    REFCOUNT_ADD(*build);

    return NOERROR;
}

} //Provider
} //Droid
} //Elastos
