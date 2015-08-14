
#ifndef __ELASTOS_DROID_VIEW_CCOMPATIBILITYINFOHOLDER_H__
#define __ELASTOS_DROID_VIEW_CCOMPATIBILITYINFOHOLDER_H__

#include "_CCompatibilityInfoHolder.h"

using Elastos::Droid::Content::Res::ICompatibilityInfo;

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CCompatibilityInfoHolder)
{
public:
    CCompatibilityInfoHolder();

    CARAPI Set(
        /* [in] */ ICompatibilityInfo* compatInfo);

    CARAPI Get(
        /* [out] */ ICompatibilityInfo** compatInfo);

    CARAPI GetIfNeeded(
        /* [out] */ ICompatibilityInfo** compatInfo);

private:
    /*volatile*/ AutoPtr<ICompatibilityInfo> mCompatInfo;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_CCOMPATIBILITYINFOHOLDER_H__
