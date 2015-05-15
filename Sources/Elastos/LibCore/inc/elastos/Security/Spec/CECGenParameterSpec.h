
#ifndef __CECGENPARAMETERSPEC_H__
#define __CECGENPARAMETERSPEC_H__

#include "_CECGenParameterSpec.h"

namespace Elastos {
namespace Security {
namespace Spec {

CarClass(CECGenParameterSpec)
{
public:
    CARAPI GetName(
        /* [out] */ String *name);

    CARAPI constructor(
        /* [in] */ const String& name);

private:
    // Standard (or predefined) name for EC domain
    // parameters to be generated
    const String mName;
};

}
}
}

#endif // __CECGENPARAMETERSPEC_H__
