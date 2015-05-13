
#ifndef  __CINPUTMETHODSUBTYPEHELPER_H__
#define  __CINPUTMETHODSUBTYPEHELPER_H__

#include "ext/frameworkext.h"
#include "_CInputMethodSubtypeHelper.h"
#include <elastos/List.h>

using Elastos::Utility::List;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {


CarClass(CInputMethodSubtypeHelper)
{
public:
    CARAPI Sort(
        /* [in] */ IContext* context,
        /* [in] */ Int32 flags,
        /* [in] */ IInputMethodInfo* imi,
        /* [in] */ IObjectContainer* subtypeList,
        /* [out, callee] */ IObjectContainer** sortedSubtypeList);
};


} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  //__CINPUTMETHODSUBTYPEHELPER_H__
