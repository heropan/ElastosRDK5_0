
#ifndef __CCERTPATHBUILDERHELPER_H__
#define __CCERTPATHBUILDERHELPER_H__

#include "_CCertPathBuilderHelper.h"

using Elastos::Security::IProvider;

namespace Elastos {
namespace Security {
namespace Cert {

CarClass(CCertPathBuilderHelper)
{
public:
    CARAPI GetInstance(
        /* [in] */ const String& algorithm,
        /* [out] */ ICertPathBuilder **builder);

    CARAPI GetInstanceEx(
        /* [in] */ const String& algorithm,
        /* [in] */ const String& provider,
        /* [out] */ ICertPathBuilder **builder);

    CARAPI GetInstanceEx2(
        /* [in] */ const String& algorithm,
        /* [in] */ IProvider *provider,
        /* [out] */ ICertPathBuilder **builder);

    CARAPI GetDefaultType(
        /* [out] */ String *type);
};

}
}
}

#endif // __CCERTPATHBUILDERHELPER_H__
