
#ifndef __LIBCORE_UTILITY_CCOLLECTIONUTILS_H__
#define __LIBCORE_UTILITY_CCOLLECTIONUTILS_H__

#include "Elastos.CoreLibrary.Utility.h"
#include "_Libcore_Utility_CCollectionUtils.h"
#include "Singleton.h"

using Elastos::Core::IComparator;
using Elastos::Utility::IIterable;
using Elastos::Utility::IList;
using Elastos::Utility::IIterator;

namespace Libcore {
namespace Utility {

CarClass(CCollectionUtils)
    , public Singleton
    , public ICollectionUtils
{
private:
    class _Iterator
        : public Object
        , public IIterator
    {
    public:
        CAR_INTERFACE_DECL()

        _Iterator(
            /* [in] */ IIterable* iterable,
            /* [in] */ Boolean trim);

        CARAPI HasNext(
            /* [out] */ Boolean* result);

        CARAPI GetNext(
            /* [out] */ IInterface** object);

        CARAPI Remove();

    private:
        CARAPI ComputeNext();

    private:
        AutoPtr<IIterator> mDelegate;
        Boolean mRemoveIsOkay;
        AutoPtr<IInterface> mNext;
        AutoPtr<IIterable> mIter;
        Boolean mTrim;
    };

    class _Iterable
        : public Object
        , public IIterable
    {
    public:
        CAR_INTERFACE_DECL()

        _Iterable(
            /* [in] */ IIterable* iterable,
            /* [in] */ Boolean trim);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

    private:
        AutoPtr<IIterable> mIter;
        Boolean mTrim;
    };

public:
    /**
     * Returns an iterator over the values referenced by the elements of {@code
     * iterable}.
     *
     * @param trim true to remove reference objects from the iterable after
     *     their referenced values have been cleared.
     */
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI DereferenceIterable(
        /* [in] */ IIterable* iterable,
        /* [in] */ Boolean trim,
        /* [out] */ IIterable** outiter);

    /**
     * Sorts and removes duplicate elements from {@code list}. This method does
     * not use {@link Object#equals}: only the comparator defines equality.
     */
    CARAPI RemoveDuplicates(
        /* [in] */ IList* list,
        /* [in] */ IComparator* comparator);

    static CARAPI _DereferenceIterable(
        /* [in] */ IIterable* iterable,
        /* [in] */ Boolean trim,
        /* [out] */ IIterable** outiter);

    static CARAPI _RemoveDuplicates(
        /* [in] */ IList* list,
        /* [in] */ IComparator* comparator);

};

} // namespace Utility
} // namespace Libcore

#endif // __LIBCORE_UTILITY_CCOLLECTIONUTILS_H__
