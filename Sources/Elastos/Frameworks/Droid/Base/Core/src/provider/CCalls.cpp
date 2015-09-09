
#include "provider/CCalls.h"
#include "provider/CContactsContractDataUsageFeedback.h"
#include "provider/CContactsContractCommonDataKindsCallable.h"
#include "provider/CContactsContractCommonDataKindsPhone.h"
#include "net/CUriHelper.h"
#include "net/Uri.h"
#include "content/CContentValues.h"
#include <elastos/coredef.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::CString;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::IInteger64;
using Elastos::Core::CInteger64;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;
using Elastos::Droid::Net::IUriBuilder;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IContentValues;
using Elastos::Droid::Content::CContentValues;
using Elastos::Droid::Internal::Telephony::IPhoneConstants;
using Elastos::Droid::Provider::IContactsContractDataUsageFeedback;
using Elastos::Droid::Provider::CContactsContractDataUsageFeedback;
using Elastos::Droid::Provider::IContactsContractCommonDataKindsCallable;
using Elastos::Droid::Provider::CContactsContractCommonDataKindsCallable;
using Elastos::Droid::Provider::IContactsContractCommonDataKindsPhone;
using Elastos::Droid::Provider::CContactsContractCommonDataKindsPhone;

namespace Elastos {
namespace Droid {
namespace Provider {

static AutoPtr<IUri> initCONTENTURI()
{
    AutoPtr<IUri> uri;
    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    helper->Parse(String("content://call_log/calls"), (IUri**)&uri);
    return uri;
}

static AutoPtr<IUri> initCONTENTFILTERURI()
{
    AutoPtr<IUri> uri;
    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    helper->Parse(String("content://call_log/calls/filter"), (IUri**)&uri);
    return uri;
}

static AutoPtr<IUri> initCONTENTURIWITHVOICEMAIL()
{
    AutoPtr<IUri> uri = initCONTENTURI();
    AutoPtr<IUriBuilder> result;
    uri->BuildUpon((IUriBuilder**)&result);
    result->AppendQueryParameter(ICalls::ALLOW_VOICEMAILS_PARAM_KEY, String("true"));
    AutoPtr<IUri> _uri;
    result->Build((IUri**)&_uri);
    return _uri;
}

AutoPtr<IUri> CCalls::CONTENT_URI = initCONTENTURI();
AutoPtr<IUri> CCalls::CONTENT_FILTER_URI = initCONTENTFILTERURI();
AutoPtr<IUri> CCalls::CONTENT_URI_WITH_VOICEMAIL = initCONTENTURIWITHVOICEMAIL();

ECode CCalls::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    *uri = CONTENT_URI;
    REFCOUNT_ADD(*uri);
    return NOERROR;
}

ECode CCalls::GetCONTENT_FILTER_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    *uri = CONTENT_FILTER_URI;
    REFCOUNT_ADD(*uri);
    return NOERROR;
}

ECode CCalls::GetCONTENT_URIWITHVOICEMAIL(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    *uri = CONTENT_URI_WITH_VOICEMAIL;
    REFCOUNT_ADD(*uri);
    return NOERROR;
}

ECode CCalls::AddCall(
    /* [in] */ ICallerInfo* ci,
    /* [in] */ IContext* context,
    /* [in] */ const String& number,
    /* [in] */ Int32 presentation,
    /* [in] */ Int32 callType,
    /* [in] */ Int64 start,
    /* [in] */ Int32 duration,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IContentResolver> resolver;
    FAIL_RETURN(context->GetContentResolver((IContentResolver**)&resolver))

    // If this is a private number then set the number to Private, otherwise check
    // if the number field is empty and set the number to Unavailable
    String _number = number;
    if (presentation == IPhoneConstants::PRESENTATION_RESTRICTED) {
        _number = ICallerInfo::PRIVATE_NUMBER;
        if (ci != NULL) FAIL_RETURN(ci->SetName(String("")))
    }
    else if (presentation == IPhoneConstants::PRESENTATION_PAYPHONE) {
        _number = ICallerInfo::PAYPHONE_NUMBER;
        if (ci != NULL) FAIL_RETURN(ci->SetName(String("")))
    }
    else if (_number.IsEmpty()
            || presentation == IPhoneConstants::PRESENTATION_UNKNOWN) {
        _number = ICallerInfo::UNKNOWN_NUMBER;
        if (ci != NULL) FAIL_RETURN(ci->SetName(String("")))
    }

    AutoPtr<IContentValues> values;
    FAIL_RETURN(CContentValues::New(5, (IContentValues**)&values))

    AutoPtr<ICharSequence> csq;
    FAIL_RETURN(CString::New(_number, (ICharSequence**)&csq))
    FAIL_RETURN(values->PutString(ICalls::NUMBER, csq))

    AutoPtr<IInteger32> integer32_1;
    FAIL_RETURN(CInteger32::New(callType, (IInteger32**)&integer32_1))
    FAIL_RETURN(values->PutInt32(ICalls::TYPE, integer32_1))

    AutoPtr<IInteger64> integer64_1;
    FAIL_RETURN(CInteger64::New(start, (IInteger64**)&integer64_1))
    FAIL_RETURN(values->PutInt64(ICalls::DATE, integer64_1))

    AutoPtr<IInteger64> integer64_2;
    FAIL_RETURN(CInteger64::New(duration, (IInteger64**)&integer64_2))
    FAIL_RETURN(values->PutInt64(ICalls::DURATION, integer64_2))

    AutoPtr<IInteger32> integer32_2;
    FAIL_RETURN(CInteger32::New(1, (IInteger32**)&integer32_2))
    FAIL_RETURN(values->PutInt32(ICalls::NEW, integer32_2))
    if (callType == ICalls::MISSED_TYPE) {
        AutoPtr<IInteger32> integer32;
        FAIL_RETURN(CInteger32::New(0, (IInteger32**)&integer32))
        FAIL_RETURN(values->PutInt32(ICalls::IS_READ, integer32))
    }
    if (ci != NULL) {
        String name;
        FAIL_RETURN(ci->GetName(&name))
        AutoPtr<ICharSequence> csqName;
        FAIL_RETURN(CString::New(name, (ICharSequence**)&csqName))
        FAIL_RETURN(values->PutString(ICalls::CACHED_NAME, csqName))

        Int32 type;
        FAIL_RETURN(ci->GetNumberType(&type))
        AutoPtr<IInteger32> integer32;
        FAIL_RETURN(CInteger32::New(type, (IInteger32**)&integer32))
        FAIL_RETURN(values->PutInt32(ICalls::CACHED_NUMBER_TYPE, integer32))

        String lable;
        FAIL_RETURN(ci->GetNumberLabel(&lable))
        AutoPtr<ICharSequence> csqLable;
        FAIL_RETURN(CString::New(lable, (ICharSequence**)&csqLable))
        FAIL_RETURN(values->PutString(ICalls::CACHED_NUMBER_LABEL, csqLable))
    }

    Int64 pId;
    FAIL_RETURN(ci->GetPerson_id(&pId))
    if ((ci != NULL) && (pId > 0)) {
        // Update usage information for the number associated with the contact ID.
        // We need to use both the number and the ID for obtaining a data ID since other
        // contacts may have the same number.

        AutoPtr<ICursor> cursor;

        // We should prefer normalized one (probably coming from
        // Phone.NORMALIZED_NUMBER column) first. If it isn't available try others.
        String normalNum;
        FAIL_RETURN(ci->GetNormalizedNumber(&normalNum))
        if (!normalNum.IsNull()) {
            const String normalizedPhoneNumber = normalNum;
            AutoPtr<IUri> uri;
            AutoPtr<IContactsContractCommonDataKindsPhone> helper;
            FAIL_RETURN(CContactsContractCommonDataKindsPhone::AcquireSingleton((IContactsContractCommonDataKindsPhone**)&helper))
            FAIL_RETURN(helper->GetCONTENT_URI((IUri**)&uri))
            AutoPtr<ArrayOf<String> > projection = ArrayOf<String>::Alloc(1);
            (*projection)[0] = IBaseColumns::ID;
            AutoPtr<ArrayOf<String> > selectionArgs = ArrayOf<String>::Alloc(2);
            (*selectionArgs)[0] = StringUtils::ToString(pId);
            (*selectionArgs)[1] = normalizedPhoneNumber;
            FAIL_RETURN(resolver->Query(uri, projection,
                IContactsContractRawContactsColumns::CONTACT_ID + String(" =? AND ") + IContactsContractCommonDataKindsPhone::NORMALIZED_NUMBER + String(" =?"),
                selectionArgs, String(NULL), (ICursor**)&cursor))
        }
        else {
            String pNumber;
            FAIL_RETURN(ci->GetPhoneNumber(&pNumber))
            const String phoneNumber = !pNumber.IsNull() ? pNumber : _number;
            AutoPtr<IUri> _uri, uri;
            AutoPtr<IContactsContractCommonDataKindsCallable> helper;
            FAIL_RETURN(CContactsContractCommonDataKindsCallable::AcquireSingleton((IContactsContractCommonDataKindsCallable**)&helper))
            FAIL_RETURN(helper->GetCONTENT_URI((IUri**)&_uri))
            String encoded;
            FAIL_RETURN(Uri::Encode(phoneNumber, &encoded))
            FAIL_RETURN(Uri::WithAppendedPath(_uri, encoded, (IUri**)&uri))
            AutoPtr<ArrayOf<String> > projection = ArrayOf<String>::Alloc(1);
            (*projection)[0] = IBaseColumns::ID;
            AutoPtr<ArrayOf<String> > selectionArgs = ArrayOf<String>::Alloc(1);
            (*selectionArgs)[0] = StringUtils::ToString(pId);
            FAIL_RETURN(resolver->Query(uri, projection, IContactsContractRawContactsColumns::CONTACT_ID + String(" =?"), selectionArgs, String(NULL), (ICursor**)&cursor))
        }

        if (cursor != NULL) {
            //try {
            Int32 count;
            FAIL_GOTO(cursor->GetCount(&count), EXIT)
            Boolean isFirst;
            FAIL_GOTO(cursor->MoveToFirst(&isFirst), EXIT)
            if (count > 0 && isFirst) {
                AutoPtr<IUri> uri;
                AutoPtr<IContactsContractDataUsageFeedback> helper;
                FAIL_GOTO(CContactsContractDataUsageFeedback::AcquireSingleton((IContactsContractDataUsageFeedback**)&helper), EXIT)
                FAIL_GOTO(helper->GetFEEDBACKURI((IUri**)&uri), EXIT)
                AutoPtr<IUriBuilder> builder;
                FAIL_GOTO(uri->BuildUpon((IUriBuilder**)&builder), EXIT)
                String path;
                FAIL_GOTO(cursor->GetString(0, &path), EXIT)
                FAIL_GOTO(builder->AppendPath(path), EXIT)
                FAIL_GOTO(builder->AppendQueryParameter(IContactsContractDataUsageFeedback::USAGE_TYPE, IContactsContractDataUsageFeedback::USAGE_TYPE_CALL), EXIT)
                AutoPtr<IUri> feedbackUri;
                FAIL_GOTO(builder->Build((IUri**)&feedbackUri), EXIT)

                AutoPtr<IContentValues> _values;
                FAIL_GOTO(CContentValues::New((IContentValues**)&_values), EXIT)
                Int32 res;
                FAIL_GOTO(resolver->Update(feedbackUri, _values, String(NULL), NULL, &res), EXIT)
            }
            //} finally {
EXIT:
            FAIL_RETURN(cursor->Close())
            //}
        }
    }

    AutoPtr<IUri> result;
    FAIL_RETURN(resolver->Insert(CONTENT_URI, values, (IUri**)&result))
    FAIL_RETURN(RemoveExpiredEntries(context))
    *uri = result;
    REFCOUNT_ADD(*uri);
    return NOERROR;
}

ECode CCalls::GetLastOutgoingCall(
    /* [in] */ IContext* context,
    /* [out] */ String* call)
{
    AutoPtr<IContentResolver> resolver;
    FAIL_RETURN(context->GetContentResolver((IContentResolver**)&resolver))
    AutoPtr<ICursor> c = NULL;

    //try {
    AutoPtr<ArrayOf<String> > projection = ArrayOf<String>::Alloc(1);
    (*projection)[0] = ICalls::NUMBER;
    FAIL_GOTO(resolver->Query(
        CONTENT_URI,
        projection,
        String(TYPE) + String(" = ") + ICalls::OUTGOING_TYPE,
        NULL,
        String(ICalls::DEFAULT_SORT_ORDER) + String(" LIMIT 1"),
        (ICursor**)&c), EXIT)
    Boolean isFIrst;
    if (c == NULL || (c->MoveToFirst(&isFIrst), !isFIrst)) {
        *call = String("");
    }
    else {
        FAIL_GOTO(c->GetString(0, call), EXIT)
    }
    //} finally {
EXIT:
    if (c != NULL) {
        FAIL_RETURN(c->Close())
    }
    //}
    return NOERROR;
}

ECode CCalls::RemoveExpiredEntries(
    /* [in] */ IContext* context)
{
    AutoPtr<IContentResolver> resolver;
    FAIL_RETURN(context->GetContentResolver((IContentResolver**)&resolver))
    Int32 num;
    return resolver->Delete(CONTENT_URI, String("_id IN ") +
            String("(SELECT _id FROM calls ORDER BY ") + ICalls::DEFAULT_SORT_ORDER
            + String(" LIMIT -1 OFFSET 500)"), NULL, &num);
}

} //Provider
} //Droid
} //Elastos