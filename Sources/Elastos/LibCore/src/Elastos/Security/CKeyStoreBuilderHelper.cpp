
#include "CKeyStoreBuilderHelper.h"
#include <cmdef.h>
#include "KeyStoreBuilder.h"

namespace Elastos {
namespace Security {

ECode CKeyStoreBuilderHelper::NewInstance(
    /* [in] */ IKeyStore* keyStore,
    /* [in] */ IKeyStoreProtectionParameter* protectionParameter,
    /* [out] */ IKeyStoreBuilder** instance)
{
    return KeyStoreBuilder::NewInstance(keyStore, protectionParameter, instance);
}

ECode CKeyStoreBuilderHelper::NewInstanceEx(
    /* [in] */ const String& type,
    /* [in] */ IProvider* provider,
    /* [in] */ IFile* file,
    /* [in] */ IKeyStoreProtectionParameter* protectionParameter,
    /* [out] */ IKeyStoreBuilder** instance)
{
    return KeyStoreBuilder::NewInstance(type, provider, file ,protectionParameter, instance);
}

ECode CKeyStoreBuilderHelper::NewInstanceEx2(
    /* [in] */ const String& type,
    /* [in] */ IProvider* provider,
    /* [in] */ IKeyStoreProtectionParameter* protectionParameter,
    /* [out] */ IKeyStoreBuilder** instance)
{
    return KeyStoreBuilder::NewInstance(type, provider, protectionParameter, instance);
}

} // namespace Security
} // namespace Elastos