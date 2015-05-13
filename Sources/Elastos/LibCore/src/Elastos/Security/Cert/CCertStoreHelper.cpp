
#include "CCertStoreHelper.h"
#include "CertStore.h"

namespace Elastos {
namespace Security {
namespace Cert {


ECode CCertStoreHelper::GetInstance(
    /* [in] */ const String& type,
    /* [in] */ ICertStoreParameters *params,
    /* [out] */ ICertStore **certStore)
{
    return CertStore::GetInstance(type, params, certStore);
}

ECode CCertStoreHelper::GetInstanceEx(
    /* [in] */ const String& type,
    /* [in] */ ICertStoreParameters *params,
    /* [in] */ const String& provider,
    /* [out] */ ICertStore **certStore)
{
    return CertStore::GetInstance(type, params, provider, certStore);
}

ECode CCertStoreHelper::GetInstanceEx2(
    /* [in] */ const String& type,
    /* [in] */ ICertStoreParameters * params,
    /* [in] */ IProvider *provider,
    /* [out] */ ICertStore **certStore)
{
    return CertStore::GetInstance(type, params, provider, certStore);
}

ECode CCertStoreHelper::GetDefaultType(
    /* [out] */ String *type)
{
    return CertStore::GetDefaultType(type);
}

}
}
}

