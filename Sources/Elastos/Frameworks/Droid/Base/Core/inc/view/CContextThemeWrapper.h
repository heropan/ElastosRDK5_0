
#ifndef __ELASTOS_DROID_VIEW_CCONTEXTTHEMEWRAPPER_H__
#define __ELASTOS_DROID_VIEW_CCONTEXTTHEMEWRAPPER_H__

#include "_CContextThemeWrapper.h"
#include "view/ContextThemeWrapper.h"


using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IComponentCallbacks;
using Elastos::Droid::Content::ISharedPreferences;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentSender;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IServiceConnection;

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CContextThemeWrapper), public ContextThemeWrapper
{
public:
    ICONTEXT_METHODS_DECL();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* base,
        /* [in] */ Int32 themeres);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetBaseContext(
        /* [out] */ IContext** context);

    CARAPI ApplyOverrideConfiguration(
        /* [in] */ IConfiguration* overrideConfiguration);
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_VIEW_CCONTEXTTHEMEWRAPPER_H__
