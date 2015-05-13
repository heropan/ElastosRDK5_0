#ifndef __CABSSAVEDSTATEHELPER_H__
#define __CABSSAVEDSTATEHELPER_H__

#include "_CAbsSavedStateHelper.h"

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CAbsSavedStateHelper)
{
public:
    CARAPI GetEmptyState(
        /* [out] */ IAbsSavedState** emptyState);
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif // __CABSSAVEDSTATEHELPER_H__