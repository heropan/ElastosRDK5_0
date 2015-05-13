#ifndef __UTILITY_CVECTOR_H__
#define __UTILITY_CVECTOR_H__

#include "_CVector.h"
#include "Vector.h"

using Elastos::Utility::_Vector;

namespace Elastos {
namespace Utility {

CarClass(CVector), public _Vector
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 capacity);

    CARAPI constructor(
        /* [in] */ Int32 capacity,
        /* [in] */ Int32 capacityIncrement);

    CARAPI constructor(
        /* [in] */ ICollection* collection);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI AddEx(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object);

    CARAPI Add(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* modified);

    CARAPI AddAllEx(
        /* [in] */ Int32 location,
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI AddAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI AddElement(
        /* [in] */ IInterface* object);

    CARAPI Capacity(
        /* [out] */ Int32* value);

    CARAPI Clear();

    CARAPI Clone(
        /* [out] */ IInterface** object);

    CARAPI Contains(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI ContainsAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    CARAPI CopyInto(
        /* [in] */ ArrayOf<IInterface*>* elements);

    CARAPI ElementAt(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** outface);

    CARAPI Elements(
        /* [out] */ IEnumeration** enu);

    CARAPI EnsureCapacity(
        /* [in] */ Int32 minimumCapacity);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI FirstElement(
        /* [out] */ IInterface** outface);

    CARAPI Get(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI IndexOf(
        /* [in] */ IInterface* object,
        /* [out] */ Int32* index);

    CARAPI IndexOfEx(
        /* [in] */ IInterface* object,
        /* [in] */ Int32 location,
        /* [out] */ Int32* value);

    CARAPI InsertElementAt(
        /* [in] */ IInterface* object,
        /* [in] */ Int32 location);

    CARAPI IsEmpty(
        /* [out] */ Boolean* result);

    CARAPI LastElement(
        /* [out] */ IInterface** outface);

    CARAPI LastIndexOf(
        /* [in] */ IInterface* object,
        /* [out] */ Int32* index);

    CARAPI LastIndexOfEx(
        /* [in] */ IInterface* object,
        /* [in] */ Int32 location,
        /* [out] */ Int32* value);

    CARAPI RemoveEx(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object);

    CARAPI Remove(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* modified);

    CARAPI RemoveAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI RemoveAllElements();

    CARAPI RemoveElement(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* value);

    CARAPI RemoveElementAt(
        /* [in] */ Int32 location);

    CARAPI RetainAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI Set(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object,
        /* [out] */ IInterface** prevObject);

    CARAPI SetElementAt(
        /* [in] */ IInterface* object,
        /* [in] */ Int32 location);

    CARAPI SetSize(
        /* [in] */ Int32 length);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI SubList(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ IList** subList);

    CARAPI ToArray(
        /* [out, callee] */ ArrayOf<IInterface*>** array);

    CARAPI ToArrayEx(
        /* [in] */ ArrayOf<IInterface*>* inArray,
        /* [out, callee] */ ArrayOf<IInterface*>** outArray);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI TrimToSize();

    CARAPI GetIterator(
        /* [out] */ IIterator** it);

    CARAPI GetListIterator(
        /* [out] */ IListIterator** it);

    CARAPI GetListIteratorEx(
        /* [in] */ Int32 location,
        /* [out] */ IListIterator** it);

protected:
    CARAPI_(Mutex*) GetSelfLock();
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_CVECTOR_H__