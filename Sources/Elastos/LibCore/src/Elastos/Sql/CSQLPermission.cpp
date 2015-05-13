
#include "CSQLPermission.h"

namespace Elastos {
namespace Sql {

ECode CSQLPermission::constructor(
    /* [in] */ const String& name)
{
    // super("");
    return NOERROR;
}

ECode CSQLPermission::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& actions)
{
    // super("", "");
    return NOERROR;
}

ECode CSQLPermission::GetActions(
    /* [out] */ String * str)
{
    *str = String(NULL);
    return NOERROR;
}

// ECode CSQLPermission::Implies(
//     /* [in] */ IPermission permission,
//     /* [out] */ Boolean * value)
// {
//     *value = TRUE;
//     return NOERROR;
// }

} // namespace Sql
} // namespace Elastos
