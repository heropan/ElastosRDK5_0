#ifndef  __ELASTOS_DROID_LAUNCHER2_CUSERINITIALIZERECEIVER_H__
#define  __ELASTOS_DROID_LAUNCHER2_CUSERINITIALIZERECEIVER_H__

#include "_Launcher2.h"
#include "_Elastos_Droid_Launcher2_CUserInitializeReceiver.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/content/BroadcastReceiver.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.CoreLibrary.Utility.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Utility::IArrayList;

namespace Elastos {
namespace Droid {
namespace Launcher2 {

CarClass(CUserInitializeReceiver)
    , public BroadcastReceiver
{
public:
    //@Override
    CARAPI OnReceive(
        /* [in] */ IContext* context,
        /* [in] */ IIntent* intent);

private:
    CARAPI_(void) AddWallpapers(
        /* [in] */ IResources* resources,
        /* [in] */ const String& packageName,
        /* [in] */ Int32 resid,
        /* [in] */ IArrayList* outList);
};

} // namespace Launcher2
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_LAUNCHER2_CUSERINITIALIZERECEIVER_H__