
#ifndef __CINSTRUMENTATIONHELPER_H__
#define __CINSTRUMENTATIONHELPER_H__

#include "_CInstrumentationHelper.h"
#include "ext/frameworkext.h"

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

    CARAPI NewApplicationEx(
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

#endif // __CINSTRUMENTATIONHELPER_H__
