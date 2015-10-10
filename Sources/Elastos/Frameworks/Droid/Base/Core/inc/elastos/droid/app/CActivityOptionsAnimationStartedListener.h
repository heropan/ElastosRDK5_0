
#ifndef __ELASTOS_DROID_APP_CACTIVITYOPTIONSANIMATIONSTARTEDLISTENER_H__
#define __ELASTOS_DROID_APP_CACTIVITYOPTIONSANIMATIONSTARTEDLISTENER_H__

#include "_Elastos_Droid_App_CActivityOptionsAnimationStartedListener.h"
#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CActivityOptionsAnimationStartedListener)
{
public:
    CARAPI constructor(
        /* [in] */ IHandler* handler,
        /* [in] */ IActivityOptionsOnAnimationStartedListener* listener);

    CARAPI SendResult(
        /* [in] */ IBundle* data);

    CARAPI ToString(
        /* [out]*/ String* str);

private:
    AutoPtr<IHandler> mHandler;
    AutoPtr<IActivityOptionsOnAnimationStartedListener> mListener;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_CACTIVITYOPTIONSANIMATIONSTARTEDLISTENER_H__
