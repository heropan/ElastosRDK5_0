
#ifndef __CARRAYOF_H__
#define __CARRAYOF_H__

#include "_Elastos_Core_CArrayOf.h"
#include "Object.h"

namespace Elastos {
namespace Core {

CarClass(CArrayOf)
    , public Object
{
public:
    CArrayOf();

    ~CArrayOf();

    CARAPI constructor(
        /* [in] */ InterfaceID riid,
        /* [in] */ Int32 size);

    CARAPI GetLength(
        /* [out] */ Int32* size);

    CARAPI Get(
        /* [in] */ Int32 index,
        /* [out] */ IInterface** element);

    CARAPI Put(
        /* [in] */ Int32 index,
        /* [in] */ IInterface* element);

    CARAPI GetTypeId(
        /* [out] */ InterfaceID* id);

private:
    AutoPtr<ArrayOf<IInterface*> > mElements;
    InterfaceID mTypeId;
};

} // namespace Core
} // namespace Elastos

#endif //__CARRAYOF_H__
