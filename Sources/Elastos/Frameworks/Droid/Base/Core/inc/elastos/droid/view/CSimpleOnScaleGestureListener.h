#ifndef __ELASTOS_DROID_VIEW_CSIMPLEONSCALEGESTURELISTENER_H__
#define __ELASTOS_DROID_VIEW_CSIMPLEONSCALEGESTURELISTENER_H__

#include "_Elastos_Droid_View_CSimpleOnScaleGestureListener.h"

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CSimpleOnScaleGestureListener)
{
public:

    CARAPI OnScale(
            /* [in ]*/ IScaleGestureDetector* detector,
            /* [out] */ Boolean* res);

    CARAPI OnScaleBegin(
        /* [in ]*/ IScaleGestureDetector* detector,
        /* [out] */ Boolean* res);

    CARAPI OnScaleEnd(
        /* [in ]*/ IScaleGestureDetector* detector);

private:

};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_CSIMPLEONSCALEGESTURELISTENER_H__
