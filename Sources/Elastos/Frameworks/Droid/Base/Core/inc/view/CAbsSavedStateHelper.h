#ifndef __ELASTOS_DROID_VIEW_CABSSAVEDSTATEHELPER_H__
#define __ELASTOS_DROID_VIEW_CABSSAVEDSTATEHELPER_H__

#include "_CAbsSavedStateHelper.h"

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CAbsSavedStateHelper)
{
public:
    CARAPI GetEMPTY_STATE(
        /* [out] */ IAbsSavedState** emptyState);
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_VIEW_CABSSAVEDSTATEHELPER_H__
