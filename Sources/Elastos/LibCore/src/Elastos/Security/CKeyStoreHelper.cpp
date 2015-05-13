
#include "CKeyStoreHelper.h"
#include "KeyStore.h"
#include "cmdef.h"

namespace Elastos {
namespace Security {

ECode CKeyStoreHelper::GetInstance(
    /* [in] */ const String& type,
    /* [out] */ IKeyStore **instance)
{
    return KeyStore::GetInstance(type, instance);
}

ECode CKeyStoreHelper::GetInstanceEx(
    /* [in] */ const String& type,
    /* [in] */ const String& provider,
    /* [out] */ IKeyStore **instance)
{
    return KeyStore::GetInstance(type, provider, instance);
}

ECode CKeyStoreHelper::GetInstanceEx2(
    /* [in] */ const String& type,
    /* [in] */ IProvider *provider,
    /* [out] */ IKeyStore **instance)
{
    return KeyStore::GetInstance(type, provider, instance);
}

ECode CKeyStoreHelper::GetDefaultType(
    /* [out] */ String *defaultType)
{
    return KeyStore::GetDefaultType(defaultType);
}

} // namespace Security
} // namespace Elastos