#ifndef __CLEGACYFLASHLIGHTHACK_H__
#define __CLEGACYFLASHLIGHTHACK_H__

#include "ext/frameworkext.h"
#include "_CLegacyFlashlightHack.h"

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Server {

CarClass(CLegacyFlashlightHack)
{
public:
    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI GetFlashlightEnabled(
        /* [out] */ Boolean* isEnabled);

    CARAPI SetFlashlightEnabled(
        /* [in] */ Boolean on);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    static const String FLASHLIGHT_FILE;
    AutoPtr<IContext> mContext;
};

}
}
}

#endif //__CLEGACYFLASHLIGHTHACK_H__
