
#ifndef __CCOMPATIBILITYINFOHOLDER_H__
#define __CCOMPATIBILITYINFOHOLDER_H__

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

#endif //__CCOMPATIBILITYINFOHOLDER_H__
