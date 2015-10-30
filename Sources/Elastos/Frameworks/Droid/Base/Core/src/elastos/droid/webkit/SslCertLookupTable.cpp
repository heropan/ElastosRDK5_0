
#include "elastos/droid/os/CBundle.h"
#include "elastos/droid/webkit/SslCertLookupTable.h"

using Elastos::Droid::Os::CBundle;
using Elastos::Net::CURL;
using Elastos::Net::IURL;

namespace Elastos {
namespace Droid {
namespace Webkit {

AutoPtr<SslCertLookupTable> SslCertLookupTable::sTable;

SslCertLookupTable::SslCertLookupTable()
{
    CBundle::New((IBundle**)&mTable);
}

AutoPtr<SslCertLookupTable> SslCertLookupTable::GetInstance()
{
    if (sTable == NULL) {
        sTable = new SslCertLookupTable();
    }
    return sTable;
}

void SslCertLookupTable::SetIsAllowed(
    /* [in] */ ISslError* sslError)
{
    if (sslError == NULL) {
        return;
    }

    String host;
    //try {
    String url;
    sslError->GetUrl(&url);
    AutoPtr<IURL> URL;
    if (FAILED(CURL::New(url, (IURL**)&url))) {
        return;
    }
    URL->GetHost(&host);
    //} catch(MalformedURLException e) {
    //    return;
    //}
    Int32 error;
    sslError->GetPrimaryError(&error);
    mTable->PutInt32(host, error);
}

// We allow the decision to be re-used if it's for the same host and is for
// an error of equal or greater severity than this error.
Boolean SslCertLookupTable::IsAllowed(
    /* [in] */ ISslError* sslError)
{
    if (sslError == NULL) {
        return FALSE;
    }

    String host;
    //try {
    String url;
    sslError->GetUrl(&url);
    AutoPtr<IURL> URL;
    if (FAILED(CURL::New(url, (IURL**)&URL))) {
        return FALSE;
    }
    URL->GetHost(&host);
    //} catch(MalformedURLException e) {
    //    return false;
    //}
    Boolean bContainsKey = FALSE;
    Int32 error, value;
    return (mTable->ContainsKey(host, &bContainsKey), bContainsKey) &&
           (sslError->GetPrimaryError(&error), mTable->GetInt32(host, &value), error <= value);
}

void SslCertLookupTable::Clear()
{
    mTable->Clear();
}

ECode SslCertLookupTable::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "SslCertLookupTable";
    return NOERROR;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos