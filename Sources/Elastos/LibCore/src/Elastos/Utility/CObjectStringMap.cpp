
#include "CObjectStringMap.h"
#include <elatypes.h>
#include <elapi.h>
#include "CObjectContainer.h"

using Elastos::Core::CObjectContainer;

namespace Elastos {
namespace Utility {

CObjectStringMap::CObjectStringMap() :
    mMap(11)
{}

CObjectStringMap::~CObjectStringMap()
{
    mMap.Clear();
}

ECode CObjectStringMap::ContainsKey(
    /* [in] */ const String& key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (!key.IsNullOrEmpty()) {
        *result = mMap.Find(key) != mMap.End();
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode CObjectStringMap::Get(
    /* [in] */ const String& key,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);

    if (!key.IsNullOrEmpty()) {
        HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap.Find(key);
        if (it != mMap.End()) {
            *value = it->mSecond;
            INTERFACE_ADDREF(*value);
            return NOERROR;
        }
    }
    *value = NULL;
    return NOERROR;
}

ECode CObjectStringMap::Put(
    /* [in] */ const String& key,
    /* [in] */ IInterface* value)
{
    if (key.IsNull() || key.IsEmpty() || value == NULL) return NOERROR;

    mMap[key] = value;
    return NOERROR;
}

ECode CObjectStringMap::Remove(
    /* [in] */ const String& key)
{
    if (key.IsNullOrEmpty()) return NOERROR;
    HashMap<String, AutoPtr<IInterface> >::Iterator it = mMap.Find(key);
    if (it != mMap.End()) {
        mMap.Erase(it);
    }
    return NOERROR;
}

ECode CObjectStringMap::GetSize(
    /* [out] */ Int32* size)
{
    if (NULL == size) return E_INVALID_ARGUMENT;
    *size = mMap.GetSize();

    return NOERROR;
}

ECode CObjectStringMap::GetKeys(
    /* [out, callee] */ ArrayOf<String>** keys)
{
    if (NULL == keys) return E_INVALID_ARGUMENT;

    Int32 size = mMap.GetSize();
    if (size == 0) {
        *keys = NULL;
        return NOERROR;
    }

    *keys = ArrayOf<String>::Alloc(size);
    ARRAYOF_ADDREF(*keys)

    HashMap<String, AutoPtr<IInterface> >::Iterator iter = mMap.Begin();
    for (Int32 i=0; iter!=mMap.End(); ++iter, ++i) {
        (**keys)[i] = iter->mFirst;
    }

    return NOERROR;
}

ECode CObjectStringMap::GetValues(
    /* [out] */ IObjectContainer** values)
{
    if (NULL == values) return E_INVALID_ARGUMENT;

    Int32 size = mMap.GetSize();
    if (size == 0) {
        *values = NULL;
        return NOERROR;
    }

    ASSERT_SUCCEEDED(CObjectContainer::New(values));

    HashMap<String, AutoPtr<IInterface> >::Iterator iter = mMap.Begin();
    for (Int32 i=0; iter!=mMap.End(); ++iter, ++i) {
        (*values)->Add(iter->mSecond);
    }

    return NOERROR;
}

ECode CObjectStringMap::GetAllItems(
    /* [out, callee] */ ArrayOf<String>** keys,
    /* [out] */ IObjectContainer** values)
{
    if (NULL == keys || NULL == values) return E_INVALID_ARGUMENT;

    Int32 size = mMap.GetSize();
    if (size == 0) {
        *keys = NULL;
        *values = NULL;
        return NOERROR;
    }

    *keys = ArrayOf<String>::Alloc(size);
    ARRAYOF_ADDREF(*keys)
    ASSERT_SUCCEEDED(CObjectContainer::New(values));

    HashMap<String, AutoPtr<IInterface> >::Iterator iter = mMap.Begin();
    for (Int32 i=0; iter!=mMap.End(); ++iter, ++i) {
        (**keys)[i] = iter->mFirst;
        (*values)->Add(iter->mSecond);
    }

    return NOERROR;
}

}
}
