
#ifndef __CCERTPATHVALIDATORHELPER_H__
#define __CCERTPATHVALIDATORHELPER_H__

#include "_CCertPathValidatorHelper.h"

using Elastos::Security::IProvider;

namespace Elastos {
namespace Security {
namespace Cert {

CarClass(CCertPathValidatorHelper)
{
public:
    CARAPI GetInstance(
        /* [in] */ const String& algorithm,
        /* [out] */ ICertPathValidator **validator);

    CARAPI GetInstanceEx(
        /* [in] */ const String& algorithm,
        /* [in] */ const String& provider,
        /* [out] */ ICertPathValidator **validator);

    CARAPI GetInstanceEx2(
        /* [in] */ const String& algorithm,
        /* [in] */ IProvider *provider,
        /* [out] */ ICertPathValidator **validator);

    CARAPI GetDefaultType(
        /* [out] */ String *type);
};

}
}
}

#endif // __CCERTPATHVALIDATORHELPER_H__
