
#ifndef __ELASTOS_UTILITY_UNSAFEARRAYLIST_H__
#define __ELASTOS_UTILITY_UNSAFEARRAYLIST_H__

#include "AbstractList.h"

namespace Elastos {
namespace Utility {

class UnsafeArrayList
    : public AbstractList
    , public IUnsafeArrayList
{
public:
    CAR_INTERFACE_DECL()

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const InterfaceID& elementType,
        /* [in] */ Int32 initialCapacity);

    CARAPI Add(
        /* [in] */ IInterface* element,
        /* [out] */ Boolean* res);

    CARAPI GetArrays(
        /* [out, callee] */ ArrayOf<IInterface*> ** array);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI Get(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object);

    CARAPI Set(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object,
        /* [out] */ IInterface** prevObject);

protected:
    UnsafeArrayList();

    UnsafeArrayList(
        /* [in] */ const InterfaceID& elementType,
        /* [in] */ Int32 initialCapacity);

private:
    const InterfaceID mElementType;

    AutoPtr< ArrayOf <IInterface*> >  mArray;

    Int32 mSize;
};

} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_UNSAFEARRAYLIST_H__