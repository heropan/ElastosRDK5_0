
#ifndef __ELASTOS_DROID_WEBKIT_CL10NUTILSHELPER_H__
#define __ELASTOS_DROID_WEBKIT_CL10NUTILSHELPER_H__

#include "_Elastos_Droid_Webkit_CL10nUtilsHelper.h"
#include <elastos/core/Singleton.h>

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CL10nUtilsHelper)
    , public Singleton
    , public IL10nUtilsHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI SetApplicationContext(
        /* [in] */ IContext* applicationContext);

    CARAPI GetLocalisedString(
        /* [in] */ Int32 id,
        /* [out] */ String* localisedString);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CL10NUTILSHELPER_H__
