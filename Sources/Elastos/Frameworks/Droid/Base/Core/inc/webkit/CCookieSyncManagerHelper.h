
#ifndef __ELASTOS_DROID_WEBKIT_CCOOKIESYNCMANAGERHELPER_H__
#define __ELASTOS_DROID_WEBKIT_CCOOKIESYNCMANAGERHELPER_H__

#include "_Elastos_Droid_Webkit_CCookieSyncManagerHelper.h"

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Webkit {

CarClass(CCookieSyncManagerHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ ICookieSyncManager** instance);

    CARAPI CreateInstance(
        /* [in] */ IContext* context,
        /* [out] */ ICookieSyncManager** instance);

private:
    CARAPI CheckInstanceIsCreated();

private:
    AutoPtr<ICookieSyncManager> sRef;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_WEBKIT_CCOOKIESYNCMANAGERHELPER_H__
