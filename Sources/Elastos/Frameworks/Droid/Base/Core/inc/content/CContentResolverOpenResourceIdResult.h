
#ifndef __CCONTENTRESOLVEROPENRESOURCEIDRESULT_H__
#define __CCONTENTRESOLVEROPENRESOURCEIDRESULT_H__

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

#endif // __CCONTENTRESOLVEROPENRESOURCEIDRESULT_H__
