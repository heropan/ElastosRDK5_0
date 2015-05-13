
#ifndef __COBJECTInt32MAP_H__
#define __COBJECTInt32MAP_H__

#include "cmdef.h"
#include "_CObjectInt32Map.h"
#include <elastos/HashMap.h>

using Elastos::Utility::HashMap;

namespace Elastos {
namespace Utility {

CarClass(CObjectInt32Map)
{
public:
    CObjectInt32Map();

    ~CObjectInt32Map();

    CARAPI ContainsKey(
        /* [in] */ Int32 key,
        /* [out] */ Boolean* result);

    CARAPI Get(
        /* [in] */ Int32 key,
        /* [out] */ IInterface** value);

    CARAPI Put(
        /* [in] */ Int32 key,
        /* [in] */ IInterface* value);

    CARAPI Remove(
        /* [in] */ Int32 key);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI GetKeys(
        /* [out, callee] */ ArrayOf<Int32>** keys);

    CARAPI GetValues(
        /* [out] */ IObjectContainer** values);

    CARAPI GetAllItems(
        /* [out, callee] */ ArrayOf<Int32>** keys,
        /* [out] */ IObjectContainer** values);

private:
    HashMap<Int32, AutoPtr<IInterface> > mMap;
};

} // namespace Utility
} // namespace Elastos

#endif //__COBJECTInt32MAP_H__
