
#ifndef __CFIXEDSIZEDRAWABLE_H__
#define __CFIXEDSIZEDRAWABLE_H__

#include "_CFixedSizeDrawable.h"

#include "systemui/statusbar/policy/FixedSizeDrawable.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Policy {

CarClass(CFixedSizeDrawable), public FixedSizeDrawable
{
public:
    IDRAWABLE_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IDrawable* that);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetFixedBounds(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 r,
        /* [in] */ Int32 b);
};

}// namespace Policy
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__CFIXEDSIZEDRAWABLE_H__
