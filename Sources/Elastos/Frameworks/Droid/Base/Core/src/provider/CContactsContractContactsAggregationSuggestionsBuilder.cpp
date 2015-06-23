
#include "provider/CContactsContractContactsAggregationSuggestionsBuilder.h"
#include "provider/ContactsContractContacts.h"
#include "text/TextUtils.h"
#include <elastos/core/StringUtils.h>
#include <elastos/coredef.h>

using Elastos::Core::StringUtils;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Net::IUriBuilder;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractContactsAggregationSuggestionsBuilder::SetContactId(
    /* [in] */ Int64 contactId)
{
    mContactId = contactId;
    return NOERROR;
}

ECode CContactsContractContactsAggregationSuggestionsBuilder::AddParameter(
    /* [in] */ const String& kind,
    /* [in] */ const String& value)
{
    if (!TextUtils::IsEmpty(value)) {
        mKinds->PushBack(kind);
        mValues->PushBack(value);
    }
    return NOERROR;
}

ECode CContactsContractContactsAggregationSuggestionsBuilder::SetLimit(
    /* [in] */ Int32 limit)
{
    mLimit = limit;
    return NOERROR;
}

ECode CContactsContractContactsAggregationSuggestionsBuilder::Build(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUriBuilder> builder;
    AutoPtr<IUri> _uri;
    FAIL_RETURN(ContactsContractContacts::GetCONTENTURI((IUri**)&_uri))
    FAIL_RETURN(_uri->BuildUpon((IUriBuilder**)&builder))
    FAIL_RETURN(builder->AppendEncodedPath(StringUtils::Int64ToString(mContactId)))
    FAIL_RETURN(builder->AppendPath(IContactsContractContactsAggregationSuggestions::CONTENT_DIRECTORY))
    if (mLimit != 0) {
        FAIL_RETURN(builder->AppendQueryParameter(String("limit"), StringUtils::Int32ToString(mLimit)))
    }

    Int32 count = mKinds->GetSize();
    for (Int32 i = 0; i < count; i++) {
        FAIL_RETURN(builder->AppendQueryParameter(String("query"), String((*mKinds)[i]) + String(":") + String((*mValues)[i])))
    }

    return builder->Build((IUri**)&uri);
}

} //Provider
} //Droid
} //Elastos