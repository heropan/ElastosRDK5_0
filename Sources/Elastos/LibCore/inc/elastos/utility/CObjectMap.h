
#ifndef __COBJECTMAP_H__
#define __COBJECTMAP_H__

#include "_CObjectMap.h"
#include <elastos/HashMap.h>

using Elastos::Utility::HashMap;

_ETL_NAMESPACE_BEGIN

template<> struct Hash<AutoPtr<IInterface> >
{
    size_t operator()(const AutoPtr<IInterface> x) const { return (size_t)x.Get(); }
};
_ETL_NAMESPACE_END

namespace Elastos{
namespace Utility{

CarClass(CObjectMap)
{
public:
    CObjectMap();

    ~CObjectMap();

    CARAPI Get(
        /* [in] */ IInterface* key,
        /* [out] */ IInterface** value);

    CARAPI Put(
        /* [in] */ IInterface* key,
        /* [in] */ IInterface* value);

    CARAPI Remove(
        /* [in] */ IInterface* key);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI GetKeys(
        /* [out, callee] */ ArrayOf<IInterface*>** keys);

    CARAPI GetValues(
        /* [out] */ IObjectContainer** values);

    CARAPI GetAllItems(
        /* [out, callee] */ ArrayOf<IInterface*>** keys,
        /* [out] */ IObjectContainer** values);

    CARAPI ContainsKey(
        /* [in] */ IInterface* key,
        /* [out] */ Boolean* contain);

private:
    HashMap<AutoPtr<IInterface>, AutoPtr<IInterface> > mMap;
};

} // namespace Utility
} // namespace Elastos

#endif //__COBJECTMAP_H__
