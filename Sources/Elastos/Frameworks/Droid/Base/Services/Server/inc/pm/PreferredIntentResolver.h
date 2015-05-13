#ifndef __PREFERREDINTENTRESOLVER_H__
#define __PREFERREDINTENTRESOLVER_H__

#include "IntentResolver.h"
#include "pm/PreferredActivity.h"

using Elastos::Droid::Content::IIntentFilter;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

class PreferredIntentResolver
    : public IntentResolver<PreferredActivity, PreferredActivity>
{
protected:
    // @Override
    CARAPI_(AutoPtr< ArrayOf<PreferredActivity*> >) NewArray(
        /* [in] */ Int32 size)
    {
        AutoPtr<ArrayOf<PreferredActivity*> > ret = ArrayOf<PreferredActivity*>::Alloc(size);
        return ret;
    }

    // @Override
    CARAPI_(String) PackageForFilter(
        /* [in] */ PreferredActivity* filter)
    {
        String pkg;
        filter->mPref->mComponent->GetPackageName(&pkg);
        return pkg;
    }

    // @Override
    CARAPI_(void) DumpFilter(
        /* [in] */ IPrintWriter* out,
        /* [in] */ const String& prefix,
        /* [in] */ PreferredActivity* filter)
    {
        filter->mPref->Dump(out, prefix, (IIntentFilter*)filter);
    }
};

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__PREFERREDINTENTRESOLVER_H__

