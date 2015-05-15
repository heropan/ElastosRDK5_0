#ifndef __UTILITY_VECTOR_H__
#define __UTILITY_VECTOR_H__

#include "cmdef.h"
#include "AbstractList.h"

using Elastos::Utility::AbstractList;
using Elastos::IO::IObjectOutputStream;

namespace Elastos {
namespace Utility {

class _Vector : public AbstractList
{
private:
    class Enumeration
        : public IEnumeration
        , public ElRefBase
    {
    public:
        Enumeration(
            /* [in] */ _Vector* owner)
            : mPos(0)
            , mOwner(owner)
        {}

        CAR_INTERFACE_DECL()

        CARAPI HasMoreElements(
            /* [out] */ Boolean* result);

        CARAPI NextElement(
            /* [out] */ IInterface** out);

    private:
        Int32 mPos;
        AutoPtr<_Vector> mOwner;
    };

public:
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

protected:
    _Vector();

    CARAPI Init(
        /* [in] */ Int32 capacity = DEFAULT_SIZE,
        /* [in] */ Int32 capacityIncrement = 0);

    CARAPI Init(
        /* [in] */ ICollection* collection);

    CARAPI RemoveRange(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI Clone(
        /* [in] */ _Vector* object);

    virtual Mutex* GetSelfLock() = 0;

private:
    AutoPtr<ArrayOf<IInterface*> > NewElementArray(
        /* [in] */ Int32 size);

    CARAPI Grow(
        /* [in] */ Int32 newCapacity);

    CARAPI GrowByOne();

    CARAPI GrowBy(
        /* [in] */ Int32 required);

    CARAPI WriteObject(
        /* [in] */ IObjectOutputStream* stream);

protected:
    /**
     * The number of elements or the size of the vector.
     */
    Int32 mElementCount;

    /**
     * The elements of the vector.
     */
    AutoPtr< ArrayOf<IInterface*> > mElementData;

    /**
     * How many elements should be added to the vector when it is detected that
     * it needs to grow to accommodate extra entries. If this value is zero or
     * negative the size will be doubled if an increase is needed.
     */
    Int32 mCapacityIncrement;

private:
    static const Int32 DEFAULT_SIZE;
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_VECTOR_H__