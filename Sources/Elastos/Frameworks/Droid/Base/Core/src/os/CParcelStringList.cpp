
#include "os/CParcelStringList.h"
#include "ext/frameworkdef.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CParcelStringList::constructor(
    /* [in] */ ArrayOf<String>* list)
{
    mStringList = list;
    return NOERROR;
}

ECode CParcelStringList::GetStringList(
    /* [out, callee] */ ArrayOf<String>** list)
{
    VALIDATE_NOT_NULL(list)
    *list = mStringList;
    REFCOUNT_ADD(*list)
    return NOERROR;
}

ECode CParcelStringList::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    mStringList = NULL;
    return source->ReadArrayOfString((ArrayOf<String>**)&mStringList);
}

ECode CParcelStringList::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return dest->WriteArrayOfString(mStringList);
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
