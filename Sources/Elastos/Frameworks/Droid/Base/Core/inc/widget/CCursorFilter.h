
#ifndef __CCURSORFILTER_H__
#define __CCURSORFILTER_H__

#include "_CCursorFilter.h"
#include "widget/CursorFilter.h"

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
    // TODO: Add your private member variables here.
};

}// namespace Elastos
}// namespace Droid
}// namespace Widget

#endif // __CCURSORFILTER_H__
