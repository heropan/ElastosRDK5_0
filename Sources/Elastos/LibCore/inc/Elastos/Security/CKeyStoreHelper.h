
#ifndef __CKEYSTOREHELPER_H__
#define __CKEYSTOREHELPER_H__

#include "_CKeyStoreHelper.h"

using Elastos::Security::IProvider;

namespace Elastos {
namespace Security {

CarClass(CKeyStoreHelper)
{
public:
    CARAPI GetInstance(
        /* [in] */ const String& type,
        /* [out] */ IKeyStore **instance);

    CARAPI GetInstanceEx(
        /* [in] */ const String& type,
        /* [in] */ const String& provider,
        /* [out] */ IKeyStore **instance);

    CARAPI GetInstanceEx2(
        /* [in] */ const String& type,
        /* [in] */ IProvider *provider,
        /* [out] */ IKeyStore **instance);

    CARAPI GetDefaultType(
        /* [out] */ String *defaultType);
};

} // namespace Security
} // namespace Elastos

#endif // __CKEYSTOREHELPER_H__
