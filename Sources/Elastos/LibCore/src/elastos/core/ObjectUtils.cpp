
#include "ObjectUtils.h"
#include "StringUtils.h"
#include <elastos/core/Object.h>
#include <utils/Log.h>

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
        ALOGD("Waring: ObjectUtils::Equals first object %p is not a IObject!", first);
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
        return StringUtils::ToHexString((Int32)first);
    }
    String str;
    obj->ToString(&str);
    return str;
}

} // namespace Core
} // namespace Elastos
