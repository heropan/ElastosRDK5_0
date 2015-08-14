
#ifndef __ELASTOS_DROID_APPWIDGET_CAPPWIDGETHOSTHELPER_H__
#define  __ELASTOS_DROID_APPWIDGET_CAPPWIDGETHOSTHELPER_H__

#include "_CAppWidgetHostHelper.h"

namespace Elastos {
namespace Droid {
namespace AppWidget {

CarClass(CAppWidgetHostHelper)
{
public:
    /**
     * Get a appWidgetId for a host in the calling process.
     *
     * @return a appWidgetId
     * @hide
     */
    CARAPI AllocateAppWidgetIdForSystem(
        /* [in] */ Int32 hostId,
        /* [out] */ Int32* appWidgetId);

    /**
     * Stop listening to changes for this AppWidget.
     * @hide
     */
    CARAPI DeleteAppWidgetIdForSystem(
        /* [in] */ Int32 appWidgetId);

    /**
     * Remove all records about all hosts for your package.
     * <ul>
     *   <li>Call this when initializing your database, as it might be because of a data wipe.</li>
     *   <li>Call this to have the AppWidget manager release all resources associated with your
     *   host.  Any future calls about this host will cause the records to be re-allocated.</li>
     * </ul>
     */
    CARAPI DeleteAllHosts();
};

} // namespace AppWidget
} // namespace Droid
} // namespace Elastos

#endif  // __ELASTOS_DROID_APPWIDGET_CAPPWIDGETHOSTHELPER_H__
