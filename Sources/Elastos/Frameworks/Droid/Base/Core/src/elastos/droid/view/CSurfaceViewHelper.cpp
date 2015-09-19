#include "view/CSurfaceViewHelper.h"
#include "view/CSurfaceView.h"

namespace Elastos {
namespace Droid {
namespace View {

ECode CSurfaceViewHelper::AdjustSurfaceViewMotion(
        /* [in]*/ IMotionEvent* evt)
{
    return CSurfaceView::AdjustSurfaceViewMotion(evt);
}

}// namespace View
}// namespace Droid
}// namespace Elastos
