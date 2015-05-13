
#include "net/CCompareResult.h"
#include <elastos/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::CObjectContainer;

namespace Elastos {
namespace Droid {
namespace Net {

ECode CCompareResult::constructor()
{
    return NOERROR;
}

ECode CCompareResult::ToString(
    /* [out] */ String * result)
{
    StringBuilder sb("removed=[");
    String strR;
    IInterface* itf;
    IObject* obj;
    List< AutoPtr<IInterface> >::Iterator iter1;
    for (iter1 = mRemoved.Begin(); iter1 != mRemoved.End(); ++iter1) {
        if (iter1 != mRemoved.Begin()) sb += String(", ");
        sb += ", ";
        itf = *iter1;
        if (itf) {
            obj = (IObject*)itf->Probe(EIID_IObject);
            if (obj) {
                obj->ToString(&strR);
                sb += strR;
            }
            else {
                String info;
                info.AppendFormat("%p", itf);
                sb += info;
            }
        }
        else {
            sb += "NULL";
        }
    }

    sb += String("] added=[");
    List< AutoPtr<IInterface> >::Iterator iter2;
    for (iter2 = mAdded.Begin(); iter2 != mAdded.End(); ++iter2) {
        if (iter2 != mRemoved.Begin()) sb += String(", ");
        sb += ", ";
        itf = *iter2;
        if (itf) {
            obj = (IObject*)itf->Probe(EIID_IObject);
            if (obj) {
                obj->ToString(&strR);
                sb += strR;
            }
            else {
                String info;
                info.AppendFormat("%p", itf);
                sb += info;
            }
        }
        else {
            sb += "NULL";
        }
    }
    sb += "]";
    *result = sb.ToString();
    return NOERROR;
}

ECode CCompareResult::GetRemoved(
    /* [out] */ IObjectContainer ** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IObjectContainer> removed;
    FAIL_RETURN(CObjectContainer::New((IObjectContainer**)&removed));

    List< AutoPtr<IInterface> >::Iterator iter;
    for (iter = mRemoved.Begin(); iter != mRemoved.End(); ++iter){
        removed->Add(*iter);
    }
    *result = removed.Get();
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CCompareResult::AddRemoved(
    /* [in] */ IInterface * removed)
{
    if (removed != NULL)
        mRemoved.PushBack(removed);
    return NOERROR;
}

ECode CCompareResult::GetAdded(
    /* [out] */ IObjectContainer ** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IObjectContainer> added;
    FAIL_RETURN(CObjectContainer::New((IObjectContainer**)&added));

    List< AutoPtr<IInterface> >::Iterator iter;
    for (iter = mAdded.Begin(); iter != mAdded.End(); ++iter){
        added->Add(*iter);
    }
    *result = added.Get();
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CCompareResult::AddAdded(
    /* [in] */ IInterface * added)
{
    if (added != NULL)
        mAdded.PushBack(added);
    return NOERROR;
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
