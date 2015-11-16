#ifndef __ELASTOS_DROID_VIEW_CWINDOWMANAGERGLOBAL_H__
#define __ELASTOS_DROID_VIEW_CWINDOWMANAGERGLOBAL_H__

#include "elastos/droid/ext/frameworkext.h"
#include "_Elastos_Droid_View_CWindowManagerGlobalSessionCallback.h"

using Elastos::Droid::Os::IBinder;

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CWindowManagerGlobalSessionCallback)
    , public Object
    , public IWindowSessionCallback
    , public IBinder
{
public:

    CAR_OBJECT_DECL()

    CAR_INTERFACE_DECL()

    CWindowManagerGlobalSessionCallback();

    CARAPI constructor();

    CARAPI OnAnimatorScaleChanged(
        /* [in] */ Float scale);

    CARAPI ToString(
        /* [out] */ String* info);
};

} // namespace View
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_CWINDOWMANAGERGLOBAL_H__
