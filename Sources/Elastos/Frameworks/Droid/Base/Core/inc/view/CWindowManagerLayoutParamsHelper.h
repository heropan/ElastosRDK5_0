
#ifndef  _CWINDOWMANAGERLAYOUTPARAMSHELPER_H__
#define  _CWINDOWMANAGERLAYOUTPARAMSHELPER_H__

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

#endif  //_CWINDOWMANAGERLAYOUTPARAMSHELPER_H__
