
#ifndef __CCLIENTHANDSHAKEIMPL_H__
#define __CCLIENTHANDSHAKEIMPL_H__

#include "_CClientHandshakeImpl.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CClientHandshakeImpl)
{
public:
    CARAPI GetEngineOwner(
        /* [out] */ Elastosx::Net::Ssl::ISSLEngine ** ppEngineOwner);

    CARAPI SetEngineOwner(
        /* [in] */ Elastosx::Net::Ssl::ISSLEngine * pEngineOwner);

    CARAPI GetSocketOwner(
        /* [out] */ Elastosx::Net::Ssl::ISSLSocket ** ppSocketOwner);

    CARAPI SetSocketOwner(
        /* [in] */ Elastosx::Net::Ssl::ISSLSocket * pSocketOwner);

    CARAPI SetRecordProtocol(
        /* [in] */ Org::Apache::Harmony::Xnet::Provider::Jsse::ISSLRecordProtocol * pRecordProtocol);

    CARAPI Start();

    CARAPI GetStatus(
        /* [out] */ Int32 * pStatus);

    CARAPI GetSession(
        /* [out] */ Elastosx::Net::Ssl::ISSLSession ** ppSession);

    CARAPI Unwrap(
        /* [in] */ ArrayOf<Byte> * pBytes);

    CARAPI UnwrapSSLv2(
        /* [in] */ ArrayOf<Byte> * pBytes);

    CARAPI Wrap(
        /* [out, callee] */ ArrayOf<Byte> ** ppWrap);

    CARAPI Send(
        /* [in] */ Org::Apache::Harmony::Xnet::Provider::Jsse::IMessage * pMessage);

    CARAPI ComputerMasterSecret();

    CARAPI GetTask(
        /* [out] */ Elastos::Core::IRunnable ** ppTask);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CCLIENTHANDSHAKEIMPL_H__
