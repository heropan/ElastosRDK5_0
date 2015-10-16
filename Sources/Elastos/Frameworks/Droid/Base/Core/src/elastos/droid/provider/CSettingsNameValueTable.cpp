
#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/provider/CSettingsNameValueTable.h"
#include "elastos/droid/provider/Settings.h"

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CSettingsNameValueTable::GetUriFor(
    /* [in] */ IUri* uri,
    /* [in] */ const String& name,
    /* [out] */ IUri** value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::NameValueTable::GetUriFor(uri, name, value);
}

} //namespace Provider
} //namespace Droid
} //namespace Elastos