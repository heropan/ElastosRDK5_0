#ifndef __ELASTOS_DROID_WIDGET_CGALLERYLAYOUTPARAMS_H__
#define __ELASTOS_DROID_WIDGET_CGALLERYLAYOUTPARAMS_H__

#include "_Elastos_Droid_Widget_CGalleryLayoutParams.h"
#include "elastos/droid/widget/GalleryLayoutParams.h"


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

#endif //__ELASTOS_DROID_WIDGET_CGALLERYLAYOUTPARAMS_H__
