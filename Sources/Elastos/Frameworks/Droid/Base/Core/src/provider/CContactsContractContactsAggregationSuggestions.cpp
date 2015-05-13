
#include "provider/CContactsContractContactsAggregationSuggestions.h"
#include "provider/CContactsContractContactsAggregationSuggestionsBuilder.h"
#include "cmdef.h"

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
    INTERFACE_ADDREF(*build);

    return NOERROR;
}

} //Provider
} //Droid
} //Elastos
