#ifndef __ELASTOS_DROID_SYSTEMUI_CVIEWSCALLER_H__
#define __ELASTOS_DROID_SYSTEMUI_CVIEWSCALLER_H__

#include "_Elastos_Droid_SystemUI_CViewScaller.h"

using Elastos::Droid::View::IView;
using Elastos::Droid::Animation::IAnimator;

namespace Elastos {
namespace Droid {
namespace SystemUI {

class CExpandHelper;

CarClass(CViewScaller)
{
public:
    CARAPI constructor();

    CARAPI SetView(
        /* [in] */ IView* v);

    CARAPI SetHeight(
        /* [in] */ Float h);

    CARAPI GetHeight(
        /* [out] */ Float* rst);

    CARAPI GetNaturalHeight(
        /* [in] */ Int32 maximum,
        /* [out] */ Int32* naturalHeight);
private:
    AutoPtr<IView> mView;
    friend class CExpandHelper;
};

}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif
