
#include "ext/frameworkdef.h"
#include "net/wifi/CWifiConfigurationProtocol.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {

static AutoPtr< ArrayOf<String> > InitStrings()
{
    AutoPtr< ArrayOf<String> > strings = ArrayOf<String>::Alloc(2);
    (*strings)[0] = "WPA";
    (*strings)[1] = "RSN";
    return strings;
}

const AutoPtr< ArrayOf<String> > CWifiConfigurationProtocol::sStrings = InitStrings();

ECode CWifiConfigurationProtocol::GetStrings(
    /* [out, callee] */ ArrayOf<String>** strings)
{
    VALIDATE_NOT_NULL(strings);
    *strings = sStrings;
    ARRAYOF_ADDREF(*strings);
    return NOERROR;
}

ECode CWifiConfigurationProtocol::ToString(
    /* [in] */ Int32 index,
    /* [out] */ String* strValue)
{
    VALIDATE_NOT_NULL(strValue);
    if (index < 0 || index > 1) return E_ILLEGAL_ARGUMENT_EXCEPTION;
    *strValue = (*sStrings)[index];
    return NOERROR;
}

} // namespace Wifi
} // namespace Net
} // namespace Droid
} // namespace Elastos
