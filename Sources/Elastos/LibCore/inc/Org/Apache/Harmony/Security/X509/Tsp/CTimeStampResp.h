
#ifndef __CTIMESTAMPRESP_H__
#define __CTIMESTAMPRESP_H__

#include "_CTimeStampResp.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {
namespace Tsp {

CarClass(CTimeStampResp)
{
public:
    CARAPI ToString(
        /* [out] */ String * pStr);

    CARAPI GetStatus(
        /* [out] */ Org::Apache::Harmony::Security::X509::Tsp::IPKIStatusInfo ** ppStatus);

    CARAPI GetTimeStampToken(
        /* [out] */ Org::Apache::Harmony::Security::Pkcs7::IContentInfo ** ppTimeStampToken);

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Security::X509::Tsp::IPKIStatusInfo * pStatus,
        /* [in] */ Org::Apache::Harmony::Security::Pkcs7::IContentInfo * pTimeStampToken);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CTIMESTAMPRESP_H__
