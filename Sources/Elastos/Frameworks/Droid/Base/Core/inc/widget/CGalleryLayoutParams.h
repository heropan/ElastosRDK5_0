#ifndef __CGALLERYLAYOUTPARAMS_H_
#define __CGALLERYLAYOUTPARAMS_H_

#include "_CGalleryLayoutParams.h"
#include "widget/GalleryLayoutParams.h"


using Elastos::Droid::View::IView;
using Elastos::Droid::Graphics::ICanvas;

namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CGalleryLayoutParams), public GalleryLayoutParams
{
public:
    IVIEWGROUPLP_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);

};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__CGALLERYLAYOUTPARAMS_H_