
#ifndef __CVIEWGROUPLAYOUTPARAMS_H__
#define __CVIEWGROUPLAYOUTPARAMS_H__

#include "_CViewGroupLayoutParams.h"
#include "view/ViewGroupLayoutParams.h"
#include "view/ViewGroupLayoutParamsMacro.h"

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

#endif //__CVIEWGROUPLAYOUTPARAMS_H__
