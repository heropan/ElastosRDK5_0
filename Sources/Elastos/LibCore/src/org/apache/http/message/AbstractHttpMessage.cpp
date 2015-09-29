
#include "AbstractHttpMessage.h"
#include "CHeaderGroup.h"
#include "CBasicHeader.h"
#include "CBasicHttpParams.h"
#include "Logger.h"

using Elastos::Utility::IIterator;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::Params::CBasicHttpParams;

namespace Org {
namespace Apache {
namespace Http {
namespace Message {

AbstractHttpMessage::AbstractHttpMessage()
{
    Init(NULL);
}

CAR_INTERFACE_IMPL(AbstractHttpMessage, Object, IHttpMessage)

void AbstractHttpMessage::Init(
    /* [in] */ IHttpParams* params)
{
    mHeadergroup = NULL;
    CHeaderGroup::New((IHeaderGroup**)&mHeadergroup);
    mParams = params;
}

void AbstractHttpMessage::Init()
{
    Init(NULL);
}

ECode AbstractHttpMessage::ContainsHeader(
    /* [in] */ const String& name,
    /* [out] */ Boolean* isContain)
{
    VALIDATE_NOT_NULL(isContain)
    return mHeadergroup->ContainsHeader(name, isContain);
}

ECode AbstractHttpMessage::GetHeaders(
    /* [in] */ const String& name,
    /* [out, callee] */ ArrayOf<IHeader*>** headers)
{
    VALIDATE_NOT_NULL(headers)
    return mHeadergroup->GetHeaders(name, headers);
}

ECode AbstractHttpMessage::GetFirstHeader(
    /* [in] */ const String& name,
    /* [out] */ IHeader** firstHeader)
{
    VALIDATE_NOT_NULL(firstHeader)
    return mHeadergroup->GetFirstHeader(name, firstHeader);
}

ECode AbstractHttpMessage::GetLastHeader(
    /* [in] */ const String& name,
    /* [out] */ IHeader** lastHeader)
{
    VALIDATE_NOT_NULL(lastHeader)
    return mHeadergroup->GetLastHeader(name, lastHeader);
}

ECode AbstractHttpMessage::GetAllHeaders(
    /* [out, callee] */ ArrayOf<IHeader*>** allHeaders)
{
    VALIDATE_NOT_NULL(allHeaders)
    return mHeadergroup->GetAllHeaders(allHeaders);
}

ECode AbstractHttpMessage::AddHeader(
    /* [in] */ IHeader* header)
{
    return mHeadergroup->AddHeader(header);
}

ECode AbstractHttpMessage::AddHeader(
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{
    if (name.IsNull()) {
        Logger::E("AbstractHttpMessage", "Header name may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IHeader> header;
    CBasicHeader::New(name, value, (IHeader**)&header);
    return mHeadergroup->AddHeader(header);
}

ECode AbstractHttpMessage::SetHeader(
    /* [in] */ IHeader* header)
{
    return mHeadergroup->UpdateHeader(header);
}

ECode AbstractHttpMessage::SetHeader(
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{
    if (name.IsNull()) {
        Logger::E("AbstractHttpMessage", "Header name may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IHeader> header;
    CBasicHeader::New(name, value, (IHeader**)&header);
    return mHeadergroup->UpdateHeader(header);
}

ECode AbstractHttpMessage::SetHeaders(
    /* [in] */ ArrayOf<IHeader*>* headers)
{
    return mHeadergroup->SetHeaders(headers);
}

ECode AbstractHttpMessage::RemoveHeader(
    /* [in] */ IHeader* header)
{
    return mHeadergroup->RemoveHeader(header);
}

ECode AbstractHttpMessage::RemoveHeaders(
    /* [in] */ const String& name)
{
    if (name.IsNull()) {
        Logger::E("AbstractHttpMessage", "Header name may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IHeaderIterator> hit;
    mHeadergroup->GetIterator((IHeaderIterator**)&hit);
    AutoPtr<IIterator> it = IIterator::Probe(hit);
    Boolean hasNext;
    while (it->HasNext(&hasNext), hasNext) {
        AutoPtr<IInterface> value;
        it->GetNext((IInterface**)&value);
        AutoPtr<IHeader> header = IHeader::Probe(value);
        String n;
        header->GetName(&n);
        if (name.EqualsIgnoreCase(n)) {
            it->Remove();
        }
    }
    return NOERROR;
}

ECode AbstractHttpMessage::GetHeaderIterator(
    /* [out] */ IHeaderIterator** headerIterator)
{
    VALIDATE_NOT_NULL(headerIterator)
    return mHeadergroup->GetIterator(headerIterator);
}

ECode AbstractHttpMessage::GetHeaderIterator(
    /* [in] */ const String& name,
    /* [out] */ IHeaderIterator** headerIterator)
{
    VALIDATE_NOT_NULL(headerIterator)
    return mHeadergroup->GetIterator(name, headerIterator);
}

ECode AbstractHttpMessage::GetParams(
    /* [out] */ IHttpParams** httpParams)
{
    VALIDATE_NOT_NULL(httpParams)
    if (mParams == NULL) {
        CBasicHttpParams::New((IHttpParams**)&mParams);
    }
    *httpParams = mParams;
    REFCOUNT_ADD(*httpParams)
    return NOERROR;
}

ECode AbstractHttpMessage::SetParams(
    /* [in] */ IHttpParams* params)
{
    if (params == NULL) {
        Logger::E("AbstractHttpMessage", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mParams = params;
    return NOERROR;
}

} // namespace Message
} // namespace Http
} // namespace Apache
} // namespace Org
