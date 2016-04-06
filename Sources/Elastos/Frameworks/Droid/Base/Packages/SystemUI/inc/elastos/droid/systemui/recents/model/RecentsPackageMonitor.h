
#ifndef  __ELASTOS_DROID_SYSTEMUI_RECENTS_MODEL_RECENTSPACKAGEMONITOR_H__
#define  __ELASTOS_DROID_SYSTEMUI_RECENTS_MODEL_RECENTSPACKAGEMONITOR_H__

#include "_SystemUI.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.CoreLibrary.Utility.h"
#include "elastos/droid/internal/content/PackageMonitor.h"
#include "elastos/droid/systemui/recents/misc/SystemServicesProxy.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Internal::Content::PackageMonitor;
using Elastos::Droid::SystemUI::Recents::Misc::SystemServicesProxy;
using Elastos::Droid::SystemUI::Recents::Model::IPackageCallbacks;
using Elastos::Utility::IList;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace Recents {
namespace Model {

/**
 * The package monitor listens for changes from PackageManager to update the contents of the Recents
 * list.
 */
class RecentsPackageMonitor
    : public PackageMonitor
    , public IRecentsPackageMonitor
{
public:
    CAR_INTERFACE_DECL()

    RecentsPackageMonitor();

    /** Registers the broadcast receivers with the specified callbacks. */
    CARAPI Register(
        /* [in] */ IContext* context,
        /* [in] */ IPackageCallbacks* cb);

    /** Unregisters the broadcast receivers. */
    // @Override
    CARAPI Unregister();

    /** Sets the list of tasks to match against package broadcast changes. */
    CARAPI SetTasks(
        /* [in] */ IList* tasks);

    // @Override
    CARAPI OnPackageRemoved(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid);

    // @Override
    CARAPI OnPackageChanged(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 uid,
        /* [in] */ ArrayOf<String>* components,
        /* [out] */ Boolean* result);

    // @Override
    CARAPI OnPackageModified(
        /* [in] */ const String& packageName);

public:
    AutoPtr<IPackageCallbacks> mCb;
    AutoPtr<IList> mTasks;
    AutoPtr<SystemServicesProxy> mSystemServicesProxy;
};

} // namespace Model
} // namespace Recents
} // namespace SystemUI
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SYSTEMUI_RECENTS_MODEL_RECENTSPACKAGEMONITOR_H__
