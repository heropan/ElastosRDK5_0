
#ifndef __CCERTIFICATEFACTORYHELPER_H__
#define __CCERTIFICATEFACTORYHELPER_H__

#include "_CCertificateFactoryHelper.h"
#ifdef ELASTOS_CORE
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif

using Elastos::Security::IProvider;

namespace Elastos {
namespace Security {
namespace Cert {

CarClass(CCertificateFactoryHelper)
{
public:
    CARAPI GetInstance(
        /* [in] */ const String& type,
        /* [out] */ ICertificateFactory** factory);

    CARAPI GetInstanceEx(
        /* [in] */ const String& type,
        /* [in] */ const String& provider,
        /* [out] */ ICertificateFactory** factory);

    CARAPI GetInstanceEx2(
        /* [in] */ const String& type,
        /* [in] */ IProvider * provider,
        /* [out] */ ICertificateFactory** factory);

};

} // end Cert
} // end Security
} // end Elastos
#endif // __CCERTIFICATEFACTORYHELPER_H__
