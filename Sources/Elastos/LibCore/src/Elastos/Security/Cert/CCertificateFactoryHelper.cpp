
#include "CCertificateFactoryHelper.h"
#include "CCertificateFactory.h"

namespace Elastos {
namespace Security {
namespace Cert {

ECode CCertificateFactoryHelper::GetInstance(
    /* [in] */ const String& type,
    /* [out] */ ICertificateFactory** factory)
{
    return CCertificateFactory::GetInstance(type, factory);
}

ECode CCertificateFactoryHelper::GetInstanceEx(
    /* [in] */ const String& type,
    /* [in] */ const String& provider,
    /* [out] */ ICertificateFactory** factory)
{
    return CCertificateFactory::GetInstance(type, provider, factory);
}

ECode CCertificateFactoryHelper::GetInstanceEx2(
    /* [in] */ const String& type,
    /* [in] */ IProvider* provider,
    /* [out] */ ICertificateFactory** factory)
{
    return CCertificateFactory::GetInstance(type, provider, factory);
}

} // end Cert
} // end Security
} // end Elastos
