#ifndef __CABSOLUTELAYOUTLAYOUTPARAMS_H_
#define __CABSOLUTELAYOUTLAYOUTPARAMS_H_

#include "_CAbsoluteLayoutLayoutParams.h"
#include "widget/AbsoluteLayoutLayoutParams.h"


namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CAbsoluteLayoutLayoutParams), public AbsoluteLayoutLayoutParams
{
public:
    CAbsoluteLayoutLayoutParams() {}

    ~CAbsoluteLayoutLayoutParams() {}

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);

    CARAPI SetX(
        /* [in] */ Int32 x);

    CARAPI SetY(
        /* [in] */ Int32 y);

    CARAPI GetX(
        /* [out] */ Int32* x);

    CARAPI GetY(
        /* [out] */ Int32* y);

    IVIEWGROUPLP_METHODS_DECL();
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__CABSOLUTELAYOUTLAYOUTPARAMS_H_
