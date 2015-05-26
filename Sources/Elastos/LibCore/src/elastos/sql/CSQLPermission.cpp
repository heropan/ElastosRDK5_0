
#include "CSQLPermission.h"

namespace Elastos {
namespace Sql {

CAR_OBJECT_IMPL(CSQLPermission);

PInterface CTimestamp::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_ISQLPermission) {
        return (IInterface*)(ISQLPermission*)this;
    }
    return BasicPermission::Probe(riid);
}

ECode CSQLPermission::constructor(
    /* [in] */ const String& name)
{
    // super("");
    BasicPermission::Init(String(""));
    return NOERROR;
}

ECode CSQLPermission::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& actions)
{
    // super("", "");
    BasicPermission::Init(String(""), String(""));
    return NOERROR;
}

ECode CSQLPermission::GetActions(
    /* [out] */ String * str)
{
    *str = String(NULL);
    return NOERROR;
}

ECode CSQLPermission::Implies(
    /* [in] */ IPermission* permission,
    /* [out] */ Boolean * value)
{
    VALIDATE_NOT_NULL(value);
    *value = TRUE;
    return NOERROR;
}

} // namespace Sql
} // namespace Elastos
