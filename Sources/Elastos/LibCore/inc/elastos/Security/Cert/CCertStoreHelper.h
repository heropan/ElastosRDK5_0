
#ifndef __CCERTSTOREHELPER_H__
#define __CCERTSTOREHELPER_H__

#include "_CCertStoreHelper.h"

using Elastos::Security::IProvider;

namespace Elastos {
namespace Security {
namespace Cert {

CarClass(CCertStoreHelper)
{
public:
    CARAPI GetInstance(
        /* [in] */ const String& type,
        /* [in] */ ICertStoreParameters *params,
        /* [out] */ ICertStore **certStore);

    CARAPI GetInstanceEx(
        /* [in] */ const String& type,
        /* [in] */ ICertStoreParameters *params,
        /* [in] */ const String& provider,
        /* [out] */ ICertStore **certStore);

    CARAPI GetInstanceEx2(
        /* [in] */ const String& type,
        /* [in] */ ICertStoreParameters *params,
        /* [in] */ IProvider *provider,
        /* [out] */ ICertStore **certStore);

    CARAPI GetDefaultType(
        /* [out] */ String *type);

private:
    // TODO: Add your private member variables here.
};

}
}
}

#endif // __CCERTSTOREHELPER_H__
