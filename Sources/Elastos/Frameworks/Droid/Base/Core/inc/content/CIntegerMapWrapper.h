
#ifndef __CINTEGERMAPWRAPPER_H__
#define __CINTEGERMAPWRAPPER_H__

#include "_CIntegerMapWrapper.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CIntegerMapWrapper)
{
public:
    CIntegerMapWrapper();

    CARAPI GetKey(
        /* [out] */ Int32* key);

    CARAPI GetValue(
        /* [out] */ Int32* value);

    CARAPI constructor(
        /* [in] */ Int32 key,
        /* [in] */ Int32 value);

private:
    Int32 mKey;
    Int32 mValue;

};

}
}
}

#endif // __CINTEGERMAPWRAPPER_H__
