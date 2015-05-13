
#include "AbstractSet.h"
#include <elastos/ObjectUtils.h>

using Elastos::Core::ObjectUtils;
using Elastos::Utility::ISet;
using Elastos::Utility::EIID_ISet;

namespace Elastos {
namespace Utility {

ECode AbstractSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    if (this->Probe(EIID_IInterface) == object) {
        *value = TRUE;
        return NOERROR;
    }
    if (ISet::Probe(object)) {
        AutoPtr<ISet> s = (ISet*) object->Probe(EIID_ISet);
        // try {
        Int32 value1 = 0;
        Int32 value2 = 0;
        GetSize(&value1);
        s->GetSize(&value2);
        Boolean isflag = FALSE;
        *value = value1 == value2 && (ContainsAll(s, &isflag), isflag);
        return NOERROR;
        // } catch (NullPointerException ignored) {
        //     return false;
        // } catch (ClassCastException ignored) {
        //     return false;
        // }
    }
    *value = FALSE;
    return NOERROR;
}

ECode AbstractSet::GetHashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    Int32 result = 0;
    AutoPtr<IIterator> it;
    GetIterator((IIterator**)&it);
    Boolean isflag = FALSE;
    while (it->HasNext(&isflag), isflag) {
        AutoPtr<IInterface> next;
        it->Next((IInterface**)&next);
        result += next == NULL ? 0 : ObjectUtils::GetHashCode(next);
    }
    *value = result;
    return NOERROR;
}

ECode AbstractSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    Boolean result = FALSE;
    Int32 value1 = 0;
    Int32 value2 = 0;
    if ((GetSize(&value1), value1) <= (collection->GetSize(&value2), value2)) {
        AutoPtr<IIterator> it;
        GetIterator((IIterator**)&it);
        Boolean isflag = FALSE;
        while (it->HasNext(&isflag), isflag) {
            AutoPtr<IInterface> next;
            it->Next((IInterface**)&next);
            if (collection->Contains(next, &isflag), isflag) {
                it->Remove();
                result = TRUE;
            }
        }
    }
    else {
        AutoPtr<IIterator> it;
        collection->GetIterator((IIterator**)&it);
        Boolean isflag = FALSE;
        while (it->HasNext(&isflag), isflag) {
            AutoPtr<IInterface> next;
            it->Next((IInterface**)&next);
            Boolean rmflag = FALSE;
            result = (Remove(next, &rmflag), rmflag) || result;
        }
    }
    *value = result;
    return NOERROR;
}

AbstractSet::AbstractSet()
{
}

} // namespace Utility
} // namespace Elastos
