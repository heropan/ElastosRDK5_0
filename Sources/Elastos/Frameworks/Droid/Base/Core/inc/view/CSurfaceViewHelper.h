#ifndef __CSURFACEVIEWHELPER_H__
#define __CSURFACEVIEWHELPER_H__

#include "_CSurfaceViewHelper.h"

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CSurfaceViewHelper)
{
public:

    CARAPI AdjustSurfaceViewMotion(
        /* [in]*/ IMotionEvent* evt);

};

}// namespace View
}// namespace Droid
}// namespace Elastos

#endif