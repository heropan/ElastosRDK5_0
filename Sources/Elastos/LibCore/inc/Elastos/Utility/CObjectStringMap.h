
#ifndef __COBJECTSTRINGMAP_H__
#define __COBJECTSTRINGMAP_H__

#include "cmdef.h"
#include "_CObjectStringMap.h"
#include <elastos/HashMap.h>

using Elastos::Utility::HashMap;

namespace Elastos {
namespace Utility {

CarClass(CObjectStringMap)
{
public:
    CObjectStringMap();

    ~CObjectStringMap();

    CARAPI ContainsKey(
        /* [in] */ const String& key,
        /* [out] */ Boolean* result);

    CARAPI Get(
        /* [in] */ const String& key,
        /* [out] */ IInterface** value);

    CARAPI Put(
        /* [in] */ const String& key,
        /* [in] */ IInterface* value);

    CARAPI Remove(
        /* [in] */ const String& key);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI GetKeys(
        /* [out, callee] */ ArrayOf<String>** keys);

    CARAPI GetValues(
        /* [out] */ IObjectContainer** values);

    CARAPI GetAllItems(
        /* [out, callee] */ ArrayOf<String>** keys,
        /* [out] */ IObjectContainer** values);

private:
    HashMap<String, AutoPtr<IInterface> > mMap;
};

} // namespace Utility
} // namespace Elastos

#endif //__COBJECTSTRINGMAP_H__
