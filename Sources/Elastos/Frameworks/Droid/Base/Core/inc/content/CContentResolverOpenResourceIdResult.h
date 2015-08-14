
#ifndef __ELASTOS_DROID_CONTENT_CCONTENTRESOLVEROPENRESOURCEIDRESULT_H__
#define __ELASTOS_DROID_CONTENT_CCONTENTRESOLVEROPENRESOURCEIDRESULT_H__

#include "_CContentResolverOpenResourceIdResult.h"

using Elastos::Droid::Content::Res::IResources;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CContentResolverOpenResourceIdResult)
{
public:
    CContentResolverOpenResourceIdResult();

    ~CContentResolverOpenResourceIdResult();

    CARAPI GetResources(
        /* [out] */ IResources** resources);

    CARAPI SetResources(
        /* [in] */ IResources* resources);

    CARAPI GetResourceId(
        /* [out] */ Int32* id);

    CARAPI SetResourceId(
        /* [in] */ Int32 id);

private:
    AutoPtr<IResources> mRes;
    Int32 mId;
};

}
}
}

#endif // __ELASTOS_DROID_CONTENT_CCONTENTRESOLVEROPENRESOURCEIDRESULT_H__
