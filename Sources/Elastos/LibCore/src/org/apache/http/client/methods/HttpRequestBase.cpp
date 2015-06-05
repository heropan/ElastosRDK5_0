
#include "HttpRequestBase.h"
#include "HttpProtocolParams.h"
#include "CBasicRequestLine.h"
#include "utils/CloneUtils.h"
#include <elastos/Logger.h>

using Elastos::Utility::Concurrent::Locks::CReentrantLock;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IProtocolVersion;
using Org::Apache::Http::IRequestLine;
using Org::Apache::Http::Message::CBasicRequestLine;
using Org::Apache::Http::Message::IHeaderGroup;
using Org::Apache::Http::Client::Utils::CloneUtils;
using Org::Apache::Http::Params::IHttpParams;
using Org::Apache::Http::Params::HttpProtocolParams;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Methods {

HttpRequestBase::HttpRequestBase()
    : AbstractHttpMessage()
    , mAborted(FALSE)
{
    AutoPtr<IReentrantLock> l;
    CReentrantLock::New((IReentrantLock**)&l);
    mAbortLock = ILock::Probe(l);
}

CAR_INTERFACE_IMPL_3(HttpRequestBase, AbstractHttpMessage, IHttpUriRequest, IAbortableHttpRequest, ICloneable)

ECode HttpRequestBase::GetProtocolVersion(
    /* [out] */ IProtocolVersion** ver)
{
    VALIDATE_NOT_NULL(ver)
    AutoPtr<IHttpParams> params;
    GetParams((IHttpParams**)&params);
    return HttpProtocolParams::GetVersion(params, ver);
}

ECode HttpRequestBase::GetURI(
    /* [out] */ IURI** uri)
{
    VALIDATE_NOT_NULL(uri)
    *uri = mUri;
    REFCOUNT_ADD(*uri)
    return NOERROR;
}

ECode HttpRequestBase::GetRequestLine(
    /* [out] */ IRequestLine** line)
{
    VALIDATE_NOT_NULL(line)
    String method;
    GetMethod(&method);
    AutoPtr<IProtocolVersion> ver;
    GetProtocolVersion((IProtocolVersion**)&ver);
    AutoPtr<IURI> uri;
    GetURI((IURI**)&uri);
    String uritext = String(NULL);
    if (uri != NULL) {
        uri->ToASCIIString(&uritext);
    }
    if (uritext.IsNull() || uritext.GetLength() == 0) {
        uritext = String("/");
    }
    return CBasicRequestLine::New(method, uritext, ver, line);
}

ECode HttpRequestBase::SetURI(
    /* [in] */ IURI* uri)
{
    mUri = uri;
    return NOERROR;
}

ECode HttpRequestBase::SetConnectionRequest(
    /* [in] */ IClientConnectionRequest* connRequest)
{
    mAbortLock->Lock();
    // try {
    if (mAborted) {
        Logger::E("HttpRequestBase", "Request already aborted");
        mAbortLock->Unlock();
        return E_IO_EXCEPTION;
    }

    mReleaseTrigger = NULL;
    mConnRequest = connRequest;
    // } finally {
    //     this.abortLock.unlock();
    // }
    mAbortLock->Unlock();
    return NOERROR;
}

ECode HttpRequestBase::SetReleaseTrigger(
    /* [in] */ IConnectionReleaseTrigger* releaseTrigger)
{
    mAbortLock->Lock();
    // try {
    if (mAborted) {
        Logger::E("HttpRequestBase", "Request already aborted");
        mAbortLock->Unlock();
        return E_IO_EXCEPTION;
    }

    mConnRequest = NULL;
    mReleaseTrigger = releaseTrigger;
    // } finally {
    //     this.abortLock.unlock();
    // }
    mAbortLock->Unlock();
    return NOERROR;
}

ECode HttpRequestBase::Abort()
{
    AutoPtr<IClientConnectionRequest> localRequest;
    AutoPtr<IConnectionReleaseTrigger> localTrigger;

    mAbortLock->Lock();
    // try {
    if (mAborted) {
        mAbortLock->Unlock();
        return NOERROR;
    }
    mAborted = TRUE;

    localRequest = mConnRequest;
    localTrigger = mReleaseTrigger;
    mAbortLock->Unlock();
    // } finally {
    //     this.abortLock.unlock();
    // }

    // Trigger the callbacks outside of the lock, to prevent
    // deadlocks in the scenario where the callbacks have
    // their own locks that may be used while calling
    // setReleaseTrigger or setConnectionRequest.
    if (localRequest != NULL) {
        localRequest->AbortRequest();
    }
    if (localTrigger != NULL) {
        // try {
        localTrigger->AbortConnection();
        // } catch (IOException ex) {
        //     // ignore
        // }
    }
    return NOERROR;
}

ECode HttpRequestBase::IsAborted(
    /* [out] */ Boolean* isAborted)
{
    VALIDATE_NOT_NULL(isAborted)
    *isAborted = mAborted;
    return NOERROR;
}

ECode HttpRequestBase::Clone(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IInterface> c;
    AbstractHttpMessage::Clone((IInterface**)&c);
    AutoPtr<HttpRequestBase> clone = (HttpRequestBase*)c.Get();
    AutoPtr<IReentrantLock> l;
    CReentrantLock::New((IReentrantLock**)&l);
    clone->mAbortLock = ILock::Probe(l);
    clone->mAborted = FALSE;
    clone->mReleaseTrigger = NULL;
    clone->mConnRequest = NULL;
    AutoPtr<IObject> o;
    CloneUtils::Clone(IObject::Probe(mHeadergroup), (IObject**)&o);
    clone->mHeadergroup = IHeaderGroup::Probe(o);
    o = NULL;
    CloneUtils::Clone(IObject::Probe(mparams), (IObject**)&o);
    clone->mparams = IHttpParams::Probe(o);
    *result = c;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

} // namespace Methods
} // namespace Client
} // namespace Http
} // namespace Apache
} // namespace Org