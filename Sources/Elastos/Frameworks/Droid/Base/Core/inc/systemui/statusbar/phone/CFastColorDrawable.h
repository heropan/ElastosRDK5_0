#ifndef __CFASTCOLORDRAWABLE_H_
#define __CFASTCOLORDRAWABLE_H_

#include "_CFastColorDrawable.h"

#include "systemui/statusbar/phone/FastColorDrawable.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Phone {

CarClass(CFastColorDrawable), public FastColorDrawable
{
public:
    IDRAWABLE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ Int32 color);
};

}// namespace Phone
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__CFASTCOLORDRAWABLE_H_
