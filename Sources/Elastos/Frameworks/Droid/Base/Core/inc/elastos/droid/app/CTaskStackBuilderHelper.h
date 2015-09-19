#ifndef __ELASTOS_DROID_APP_CTASKSTACKBUILDERHELPER_H__
#define __ELASTOS_DROID_APP_CTASKSTACKBUILDERHELPER_H__

#include "_Elastos_Droid_App_CTaskStackBuilderHelper.h"

using Elastos::Droid::Content::IContext;

namespace Elastos{
namespace Droid{
namespace App{

CarClass(CTaskStackBuilderHelper)
{
public:
    /**
     * Return a new TaskStackBuilder for launching a fresh task stack consisting
     * of a series of activities.
     *
     * @param context The context that will launch the new task stack or generate a PendingIntent
     * @return A new TaskStackBuilder
     */
    CARAPI Create(
        /* [in] */ IContext * context,
        /* [out] */ ITaskStackBuilder **builder);
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_APP_CTASKSTACKBUILDERHELPER_H__
