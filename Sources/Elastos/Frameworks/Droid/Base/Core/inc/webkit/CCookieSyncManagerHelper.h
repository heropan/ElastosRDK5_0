
#ifndef __CCOOKIESYNCMANAGERHELPER_H__
#define __CCOOKIESYNCMANAGERHELPER_H__

#include "_CCookieSyncManagerHelper.h"

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

#endif // __CCOOKIESYNCMANAGERHELPER_H__
