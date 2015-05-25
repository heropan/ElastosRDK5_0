
#include "CObjectMap.h"
#include <elatypes.h>
#include <elapi.h>
#include "CObjectContainer.h"

using Elastos::Core::CObjectContainer;

namespace Elastos{
namespace Utility{

CObjectMap::CObjectMap() :
    mMap(11)
{}

CObjectMap::~CObjectMap()
{
    mMap.Clear();
}

ECode CObjectMap::Get(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value)

    HashMap<AutoPtr<IInterface>, AutoPtr<IInterface> >::Iterator it = mMap.Find(key);
    if (it != mMap.End()) {
        *value = it->mSecond;
        INTERFACE_ADDREF(*value);
        return NOERROR;
    }
    *value = NULL;
    return NOERROR;
}

ECode CObjectMap::Put(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value)
{
    if (key == NULL || value == NULL) return NOERROR;

    mMap[key] = value;
    return NOERROR;
}

ECode CObjectMap::Remove(
    /* [in] */ IInterface* key)
{
    if (key == NULL) return NOERROR;
    HashMap<AutoPtr<IInterface>, AutoPtr<IInterface> >::Iterator it = mMap.Find(key);
    if (it != mMap.End()) {
        mMap.Erase(it);
    }
    return NOERROR;
}

ECode CObjectMap::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = mMap.GetSize();

    return NOERROR;
}

ECode CObjectMap::GetKeys(
    /* [out, callee] */ ArrayOf<IInterface*>** keys)
{
    VALIDATE_NOT_NULL(keys)

    Int32 size = mMap.GetSize();

    AutoPtr< ArrayOf<IInterface*> > outinter = ArrayOf<IInterface*>::Alloc(size);

    HashMap<AutoPtr<IInterface>, AutoPtr<IInterface> >::Iterator iter = mMap.Begin();
    for (Int32 i=0; iter!=mMap.End(); ++iter, ++i) {
        outinter->Set(i, iter->mFirst);
    }
    *keys = outinter;
    INTERFACE_ADDREF(*keys)

    return NOERROR;
}

ECode CObjectMap::GetValues(
    /* [out] */ IObjectContainer** values)
{
    VALIDATE_NOT_NULL(values)

    Int32 size = mMap.GetSize();
    if (size == 0) {
        *values = NULL;
        return NOERROR;
    }
    AutoPtr<IObjectContainer> obj;
    ASSERT_SUCCEEDED(CObjectContainer::New((IObjectContainer**)&obj));

    HashMap<AutoPtr<IInterface>, AutoPtr<IInterface> >::Iterator iter = mMap.Begin();
    for (Int32 i=0; iter!=mMap.End(); ++iter, ++i) {
        obj->Add(iter->mSecond);
    }
    *values = obj;
    INTERFACE_ADDREF(*values)

    return NOERROR;
}

ECode CObjectMap::GetAllItems(
    /* [out, callee] */ ArrayOf<IInterface*>** keys,
    /* [out] */ IObjectContainer** values)
{
    VALIDATE_NOT_NULL(keys)
    VALIDATE_NOT_NULL(values)

    Int32 size = mMap.GetSize();
    if (size == 0) {
        *keys = NULL;
        *values = NULL;
        return NOERROR;
    }

    AutoPtr< ArrayOf<IInterface*> > outinter = ArrayOf<IInterface*>::Alloc(size);
    AutoPtr<IObjectContainer> obj;
    ASSERT_SUCCEEDED(CObjectContainer::New((IObjectContainer**)&obj));

    HashMap<AutoPtr<IInterface>, AutoPtr<IInterface> >::Iterator iter = mMap.Begin();
    for (Int32 i=0; iter!=mMap.End(); ++iter, ++i) {
        outinter->Set(i, iter->mFirst);
        obj->Add(iter->mSecond);
    }
    *keys = outinter;
    *values = obj;
    INTERFACE_ADDREF(*keys)
    INTERFACE_ADDREF(*values)

    return NOERROR;
}

ECode CObjectMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* contain)
{
    VALIDATE_NOT_NULL(contain)
    VALIDATE_NOT_NULL(key)
    *contain = FALSE;

    HashMap<AutoPtr<IInterface>, AutoPtr<IInterface> >::Iterator it = mMap.Find(key);
    if (it != mMap.End()) {
        *contain = TRUE;
    }

    return NOERROR;
}

} // namespace Utility
} // namespace Elastos
