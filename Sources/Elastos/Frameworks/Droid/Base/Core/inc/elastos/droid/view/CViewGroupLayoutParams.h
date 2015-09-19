
#ifndef __ELASTOS_DROID_VIEW_CVIEWGROUPLAYOUTPARAMS_H__
#define __ELASTOS_DROID_VIEW_CVIEWGROUPLAYOUTPARAMS_H__

#include "_Elastos_Droid_View_CViewGroupLayoutParams.h"
#include "view/ViewGroupLayoutParams.h"


namespace Elastos {
namespace Droid {
namespace View {

CarClass(CViewGroupLayoutParams), public ViewGroupLayoutParams
{
public:
    IVIEWGROUPLP_METHODS_DECL();

    CARAPI ToString(
        /* [out] */ String* description);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_CVIEWGROUPLAYOUTPARAMS_H__
