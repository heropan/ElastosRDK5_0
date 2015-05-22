
#include "cmdef.h"
#include "ObjectUtils.h"
#include "StringUtils.h"

#ifdef ELASTOS_CORE
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif

namespace Elastos {
namespace Core {

Boolean ObjectUtils::Equals(
    /* [in] */ IInterface* first,
    /* [in] */ IInterface* second)
{
    if (first == second) {
        return TRUE;
    }
    AutoPtr<IObject> obj = IObject::Probe(first);
    if (obj == NULL) {
        return FALSE;
    }
    Boolean isflag = FALSE;
    obj->Equals(second, &isflag);
    return isflag;
}

Int32 ObjectUtils::GetHashCode(
    /* [in] */ IInterface* first)
{
    AutoPtr<IObject> obj = IObject::Probe(first);
    if (obj == NULL) {
        return (Int32)first;
    }
    Int32 hashcode = 0;
    obj->GetHashCode(&hashcode);
    return hashcode;
}

String ObjectUtils::ToString(
    /* [in] */ IInterface* first)
{
    AutoPtr<IObject> obj = IObject::Probe(first);
    if (obj == NULL) {
        return StringUtils::Int32ToHexString((Int32)first);
    }
    String str;
    obj->ToString(&str);
    return str;
}

} // namespace Core
} // namespace Elastos
