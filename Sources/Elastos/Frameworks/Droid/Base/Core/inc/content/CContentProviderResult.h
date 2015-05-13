
#ifndef __CCONTENTPROVIDERRESULT_H__
#define __CCONTENTPROVIDERRESULT_H__

#include "_CContentProviderResult.h"
#include "ext/frameworkext.h"

using namespace Elastos::Droid::Net;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CContentProviderResult)
{
public:
    CContentProviderResult();

    ~CContentProviderResult();

    CARAPI GetUri(
        /* [out] */ IUri** uri);

    CARAPI GetCount(
        /* [out] */ Int32* count);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IUri* uri);

    CARAPI constructor(
        /* [in] */ Int32 count);

private:
    AutoPtr<IUri> mUri;
    Int32 mCount;

};

}
}
}

#endif // __CCONTENTPROVIDERRESULT_H__
