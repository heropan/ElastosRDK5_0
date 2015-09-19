
#ifndef __ELASTOS_DROID_VIEW_INPUTMETHOD_CINPUTMETHODSUBTYPEHELPER_H__
#define  __ELASTOS_DROID_VIEW_INPUTMETHOD_CINPUTMETHODSUBTYPEHELPER_H__

#include "ext/frameworkext.h"
#include "_Elastos_Droid_View_InputMethod_CInputMethodSubtypeHelper.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
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

#endif  //__ELASTOS_DROID_VIEW_INPUTMETHOD_CINPUTMETHODSUBTYPEHELPER_H__
