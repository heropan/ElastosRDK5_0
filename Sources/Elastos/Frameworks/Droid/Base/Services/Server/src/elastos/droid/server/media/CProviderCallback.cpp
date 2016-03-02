
#include "elastos/droid/server/media/CProviderCallback.h"
#include "elastos/droid/server/media/RemoteDisplayProviderProxy.h"

namespace Elastos {
namespace Droid {
namespace Server {
namespace Media {

ECode CProviderCallback::constructor(
    /* [in] */ Handle64 host)
{
    AutoPtr<IWeakReferenceSource> wrs = IWeakReferenceSource::Probe((RemoteDisplayProviderProxy::Connection*)host);
    return wrs->GetWeakReference((IWeakReference**)&mConnectionRef);
}

void CProviderCallback::Dispose()
{
    mConnectionRef->Clear();
}

ECode CProviderCallback::OnStateChanged(
    /* [in] */ IRemoteDisplayState * state)
{
    AutoPtr<IInterface> obj;
    mConnectionRef->Resolve(EIID_IInterface, (IInterface**)&obj);
    AutoPtr<RemoteDisplayProviderProxy::Connection> = (RemoteDisplayProviderProxy::Connection*)IObject::Probe(obj);
    if (connection != NULL) {
        connection->PostStateChanged(state);
    }
    return NOERROR;
}

} // namespace Media
} // namespace Server
} // namespace Droid
} // namespace Elastos
