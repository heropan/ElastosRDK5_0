
#ifndef __ELASTOS_DROID_APP_CINSTRUMENTATIONHELPER_H__
#define __ELASTOS_DROID_APP_CINSTRUMENTATIONHELPER_H__

#include "_Elastos_Droid_App_CInstrumentationHelper.h"
#include "elastos/droid/ext/frameworkext.h"

using Elastos::Core::IClassLoader;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;


namespace Elastos {
namespace Droid {
namespace App {

CarClass(CInstrumentationHelper)
{
public:
    CARAPI NewApplication(
        /* [in] */ IClassInfo* cls,
        /* [in] */ IContext* context,
        /* [out] */ IApplication** app);

    CARAPI NewApplication(
        /* [in] */ const ClassID& clsid,
        /* [in] */ IContext* context,
        /* [out] */ IApplication** app);

    CARAPI CheckStartActivityResult(
        /* [in] */ Int32 res,
        /* [in] */ IIntent* intent);
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_APP_CINSTRUMENTATIONHELPER_H__
