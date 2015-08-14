#ifndef __ELASTOS_DROID_INTERNAL_UTILITY_ARRAYUTILS_H__
#define __ELASTOS_DROID_INTERNAL_UTILITY_ARRAYUTILS_H__

#include "ext/frameworkext.h"

using Elastos::Core::CObjectContainer;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

class ArrayUtils
{
public:
    static CARAPI_(Int32) IdealByteArraySize(
        /* [in] */ Int32 need);

    static CARAPI_(Int32) IdealBooleanArraySize(
        /* [in] */ Int32 need);

    static CARAPI_(Int32) IdealInt16ArraySize(
        /* [in] */ Int32 need);

    static CARAPI_(Int32) IdealCharArraySize(
        /* [in] */ Int32 need);

    static CARAPI_(Int32) IdealInt32ArraySize(
        /* [in] */ Int32 need);

    static CARAPI_(Int32) IdealFloatArraySize(
        /* [in] */ Int32 need);

    static CARAPI_(Int32) IdealObjectArraySize(
        /* [in] */ Int32 need);

    static CARAPI_(Int32) IdealInt64ArraySize(
        /* [in] */ Int32 need);

    static CARAPI_(void) Sort(
        /* [in] */ ArrayOf<Int32> *array);

    static CARAPI_(void) Sort(
        /* [in] */ ArrayOf<Int32> *array,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length);

    static CARAPI_(void) QuickSort(
        /* [in] */ ArrayOf<Int32> *array,
        /* [in] */ Int32 low,
        /* [in] */ Int32 high);

    static CARAPI_(Boolean) Contains(
        /* [in] */ ArrayOf<Int32>* array,
        /* [in] */ Int32 value);

    static CARAPI_(Int64) Total(
        /* [in] */ ArrayOf<Int64> *array);

    static CARAPI_(AutoPtr< ArrayOf<Int32> >) AppendInt32(
        /* [in] */ ArrayOf<Int32>* cur,
        /* [in] */ Int32 val);

    static CARAPI_(AutoPtr< ArrayOf<Int32> >) RemoveInt32(
        /* [in] */ ArrayOf<Int32>* cur,
        /* [in] */ Int32 val);

    template <typename T>
    static AutoPtr<IObjectContainer> ToObjectContainer(
        /* [in] */ const ArrayOf<T>* array)
    {
        assert(SUPERSUBCLASS_EX(IInterface*, T) && "Error: element type must extends IInterface.");
        AutoPtr<IObjectContainer> container;
        CObjectContainer::New((IObjectContainer**)&container);
        for (Int32 i = 0; i < array->GetLength(); ++i) {
            assert((*array)[i] != NULL);
            container->Add((*array)[i]->Probe(EIID_IInterface));
        }
        return container;
    }

    template <typename T>
    static AutoPtr<IObjectContainer> ToObjectContainer(
        /* [in] */ const AutoPtr<ArrayOf<T> >& array)
    {
        return ToObjectContainer(array.Get());
    }
};

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_UTILITY_ARRAYUTILS_H__
