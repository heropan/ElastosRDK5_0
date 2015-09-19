
#ifndef __ELASTOS_DROID_WEBKIT_CWEBVIEWHELPER_H__
#define __ELASTOS_DROID_WEBKIT_CWEBVIEWHELPER_H__

#include "_Elastos_Droid_Webkit_CWebViewHelper.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CWebViewHelper)
{
public:
    CARAPI EnablePlatformNotifications();

    CARAPI DisablePlatformNotifications();

    CARAPI FindAddress(
        /* [in] */ const String& addr,
        /* [out] */ String* outAddr);

    CARAPI GetPluginList(
        /* [out] */ IPluginList** pluginList);
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CWEBVIEWHELPER_H__
