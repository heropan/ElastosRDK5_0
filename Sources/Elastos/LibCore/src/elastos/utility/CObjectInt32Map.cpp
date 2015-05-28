
#include "CObjectInt32Map.h"
#include <elatypes.h>
#include <elapi.h>
#include "CObjectContainer.h"

using Elastos::Core::CObjectContainer;

namespace Elastos {
namespace Utility {

CObjectInt32Map::CObjectInt32Map() :
    mMap(11)
{}

CObjectInt32Map::~CObjectInt32Map()
{
    mMap.Clear();
}

ECode CObjectInt32Map::ContainsKey(
    /* [in] */ Int32 key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mMap.Find(key) != mMap.End();
    return NOERROR;
}

ECode CObjectInt32Map::Get(
    /* [in] */ Int32 key,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;

    HashMap<Int32, AutoPtr<IInterface> >::Iterator it = mMap.Find(key);
    if (it != mMap.End()) {
        *value = it->mSecond;
        REFCOUNT_ADD(*value);
        return NOERROR;
    }

    return NOERROR;
}

ECode CObjectInt32Map::Put(
    /* [in] */ Int32 key,
    /* [in] */ IInterface* value)
{
    mMap[key] = value;
    return NOERROR;
}

ECode CObjectInt32Map::Remove(
    /* [in] */ Int32 key)
{
    HashMap<Int32, AutoPtr<IInterface> >::Iterator it = mMap.Find(key);
    if (it != mMap.End()) {
        mMap.Erase(it);
    }
    return NOERROR;
}

ECode CObjectInt32Map::GetSize(
    /* [out] */ Int32* size)
{
    if (NULL == size) return E_INVALID_ARGUMENT;
    *size = mMap.GetSize();

    return NOERROR;
}

ECode CObjectInt32Map::GetKeys(
    /* [out, callee] */ ArrayOf<Int32>** keys)
{
    if (NULL == keys) return E_INVALID_ARGUMENT;

    Int32 size = mMap.GetSize();
    if (size == 0) {
        *keys = NULL;
        return NOERROR;
    }

    *keys = ArrayOf<Int32>::Alloc(size);
    ARRAYOF_ADDREF(*keys)

    HashMap<Int32, AutoPtr<IInterface> >::Iterator iter = mMap.Begin();
    for (Int32 i = 0; iter != mMap.End(); ++iter, ++i) {
        (**keys)[i] = iter->mFirst;
    }

    return NOERROR;
}

ECode CObjectInt32Map::GetValues(
    /* [out] */ IObjectContainer** values)
{
    if (NULL == values) return E_INVALID_ARGUMENT;

    Int32 size = mMap.GetSize();
    if (size == 0) {
        *values = NULL;
        return NOERROR;
    }

    ASSERT_SUCCEEDED(CObjectContainer::New(values));

    HashMap<Int32, AutoPtr<IInterface> >::Iterator iter = mMap.Begin();
    for (Int32 i = 0; iter != mMap.End(); ++iter, ++i) {
        (*values)->Add(iter->mSecond);
    }

    return NOERROR;
}

ECode CObjectInt32Map::GetAllItems(
    /* [out, callee] */ ArrayOf<Int32>** keys,
    /* [out] */ IObjectContainer** values)
{
    if (NULL == keys || NULL == values) return E_INVALID_ARGUMENT;

    Int32 size = mMap.GetSize();
    if (size == 0) {
        *keys = NULL;
        *values = NULL;
        return NOERROR;
    }

    *keys = ArrayOf<Int32>::Alloc(size);
    ARRAYOF_ADDREF(*keys)
    ASSERT_SUCCEEDED(CObjectContainer::New(values));

    HashMap<Int32, AutoPtr<IInterface> >::Iterator iter = mMap.Begin();
    for (Int32 i=0; iter!=mMap.End(); ++iter, ++i) {
        (**keys)[i] = iter->mFirst;
        (*values)->Add(iter->mSecond);
    }

    return NOERROR;
}

}
}
