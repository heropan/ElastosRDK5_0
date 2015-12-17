#ifndef __ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACTCONTACTSAGGREGATIONSUGGESTIONSBUILDER_H__
#define __ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACTCONTACTSAGGREGATIONSUGGESTIONSBUILDER_H__

#include "_Elastos_Droid_Provider_CContactsContractContactsAggregationSuggestionsBuilder.h"
#include "elastos/droid/ext/frameworkext.h"
#include <Elastos.CoreLibrary.Utility.h>
#include <elastos/core/Object.h>

using Elastos::Droid::Net::IUri;
using Elastos::Utility::IList;

namespace Elastos {
namespace Droid {
namespace Provider {

/**
 * A convenience builder for aggregation suggestion content URIs.
 *
 * TODO: change documentation for this class to use the builder.
 * @hide
 */
CarClass(CContactsContractContactsAggregationSuggestionsBuilder)
    , public Object
    , public IContactsContractContactsAggregationSuggestionsBuilder
{
public:
    CAR_OBJECT_DECL()

    CAR_INTERFACE_DECL()

    /**
     * Optional existing contact ID.  If it is not provided, the search
     * will be based exclusively on the values supplied with {@link #addParameter}.
     */
    CARAPI SetContactId(
        /* [in] */ Int64 contactId);

    /**
     * A value that can be used when searching for an aggregation
     * suggestion.
     *
     * @param kind can be one of
     *            {@link AggregationSuggestions#PARAMETER_MATCH_NAME},
     *            {@link AggregationSuggestions#PARAMETER_MATCH_EMAIL},
     *            {@link AggregationSuggestions#PARAMETER_MATCH_NICKNAME},
     *            {@link AggregationSuggestions#PARAMETER_MATCH_PHONE}
     */
    CARAPI AddParameter(
        /* [in] */ const String& kind,
        /* [in] */ const String& value);

    CARAPI SetLimit(
        /* [in] */ Int32 limit);

    CARAPI Build(
        /* [out] */ IUri** uri);

private:
    Int64 mContactId;
    AutoPtr<IList> mKinds;
    AutoPtr<IList> mValues;
    Int32 mLimit;
};

} //Provider
} //Droid
} //Elastos

#endif //__ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACTCONTACTSAGGREGATIONSUGGESTIONSBUILDER_H__
