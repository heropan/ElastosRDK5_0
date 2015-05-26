
#include "CDriverPropertyInfo.h"

namespace Elastos {
namespace Sql {

CAR_OBJECT_IMPL(CDriverPropertyInfo);

CAR_INTERFACE_IMPL(CDriverPropertyInfo, Object, IDriverPropertyInfo);

ECode CDriverPropertyInfo::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{
    this->name = name;
    this->value = value;
    choices = NULL;
    description = String(NULL);
    required = FALSE;
    return NOERROR;
}

} // namespace Sql
} // namespace Elastos

