
#ifndef __ELASTOS_DROID_WIDGET_CCURSORFILTER_H__
#define __ELASTOS_DROID_WIDGET_CCURSORFILTER_H__

#include "_Elastos_Droid_Widget_CCursorFilter.h"
#include "elastos/droid/widget/CursorFilter.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CCursorFilter), public CursorFilter
{
public:
    CARAPI SetDelayer(
        /* [in] */ IFilterDelayer * pDelayer);

    CARAPI DoFilter(
        /* [in] */ ICharSequence * pConstraint);

    CARAPI DoFilter(
        /* [in] */ ICharSequence * pConstraint,
        /* [in] */ IFilterListener * pListener);

    CARAPI ConvertResultToString(
        /* [in] */ IInterface * pResultValue,
        /* [out] */ ICharSequence ** ppCs);

    CARAPI constructor(
        /* [in] */ ICursorFilterClient * pClient);

private:
};

}// namespace Elastos
}// namespace Droid
}// namespace Widget

#endif // __ELASTOS_DROID_WIDGET_CCURSORFILTER_H__
