
#ifndef __SIMPLEONSCALEGESTURELISTENER_H__
#define __SIMPLEONSCALEGESTURELISTENER_H__

#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace View {

class SimpleOnScaleGestureListener
    : public ElRefBase
    , public IOnScaleGestureListener
{
public:
    CAR_INTERFACE_DECL();

    virtual CARAPI OnScale(
            /* [in ]*/ IScaleGestureDetector* detector,
            /* [out] */ Boolean* res);

    virtual CARAPI OnScaleBegin(
        /* [in ]*/ IScaleGestureDetector* detector,
        /* [out] */ Boolean* res);

    virtual CARAPI OnScaleEnd(
        /* [in ]*/ IScaleGestureDetector* detector);
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__SIMPLEONSCALEGESTURELISTENER_H__
