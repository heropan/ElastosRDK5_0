#ifndef __CSIMPLEONSCALEGESTURELISTENER_H__
#define __CSIMPLEONSCALEGESTURELISTENER_H__

#include "_CSimpleOnScaleGestureListener.h"

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

#endif //__CSIMPLEONSCALEGESTURELISTENER_H__
