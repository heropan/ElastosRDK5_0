
#ifndef __ELASTOS_DROID_VIEW_CWINDOWMANAGERLAYOUTPARAMSHELPER_H__
#define  __ELASTOS_DROID_VIEW_CWINDOWMANAGERLAYOUTPARAMSHELPER_H__

#include "_CWindowManagerLayoutParamsHelper.h"

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CWindowManagerLayoutParamsHelper)
{
public:
    CARAPI MayUseInputMethodHelper(
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* mayUse);
};

} // namespace View
} // namepsace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_VIEW_CWINDOWMANAGERLAYOUTPARAMSHELPER_H__
