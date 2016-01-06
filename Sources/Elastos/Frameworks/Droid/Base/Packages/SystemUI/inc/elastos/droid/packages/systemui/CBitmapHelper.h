
#ifndef __ELASTOS_DROID_SYSTEMUI_CBITMAPHELPER_H__
#define __ELASTOS_DROID_SYSTEMUI_CBITMAPHELPER_H__

#include "_Elastos_Droid_SystemUI_CBitmapHelper.h"
#include <elastos/core/Singleton.h>

using Elastos::Droid::Graphics::IBitmap;

using Elastos::Core::Singleton;

namespace Elastos {
namespace Droid {
namespace SystemUI {

CarClass(CBitmapHelper)
    , public Singleton
    , public IBitmapHelper
{
public:
    CAR_SINGLETON_DECL()

    CAR_INTERFACE_DECL()

    CARAPI CreateCircularClip(
        /* [in] */ IBitmap* input,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [out] */ IBitmap** result);
};

} // namespace SystemUI
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SYSTEMUI_CBITMAPHELPER_H__
