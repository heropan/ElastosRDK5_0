#include "ext/frameworkext.h"
#include "net/wifi/CEnterpriseField.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

ECode CEnterpriseField::constructor(
    /* [in] */ const String& varName)
{
    mVarName = varName;
    return NOERROR;
}

ECode CEnterpriseField::SetValue(
    /* [in] */ const String& value)
{
    mValue = value;
    return NOERROR;
}

ECode CEnterpriseField::GetVarName(
    /* [out] */ String* varName)
{
    VALIDATE_NOT_NULL(varName);
    *varName = mVarName;
    return NOERROR;
}

ECode CEnterpriseField::GetValue(
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;
    return NOERROR;
}


}
}
}
}
