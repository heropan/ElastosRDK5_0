
#ifndef  __ELASTOS_DROID_HARDWARE_CARRAYUTILS_H__
#define  __ELASTOS_DROID_HARDWARE_CARRAYUTILS_H__

#include "_Elastos_Droid_Hardware_Camera2_Utils_CArrayUtils.h"
#include "elastos/droid/hardware/camera2/utils/ArrayUtils.h"
#include <elastos/core/Singleton.h>

using Elastos::Core::Singleton;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Camera2 {
namespace Utils {

CarClass(CArrayUtils)
    , public Singleton
    , public IArrayUtils
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    /** Return the index of {@code needle} in the {@code array}, or else {@code -1} */
    CARAPI GetArrayIndex(
        /* [in] */ ArrayOf<IInterface*>* array,
        /* [in] */ IInterface* needle,
        /* [out] */ Int32* value);

    /** Return the index of {@code needle} in the {@code array}, or else {@code -1} */
    CARAPI GetArrayIndex(
        /* [in] */ ArrayOf<Int32>* array,
        /* [in] */ Int32 needle,
        /* [out] */ Int32* value);

    /**
     * Create an {@code int[]} from the {@code List<>} by using {@code convertFrom} and
     * {@code convertTo} as a one-to-one map (via the index).
     *
     * <p>Strings not appearing in {@code convertFrom} are ignored (with a logged warning);
     * strings appearing in {@code convertFrom} but not {@code convertTo} are silently
     * dropped.</p>
     *
     * @param list Source list of strings
     * @param convertFrom Conversion list of strings
     * @param convertTo Conversion list of ints
     * @return An array of ints where the values correspond to the ones in {@code convertTo}
     *         or {@code null} if {@code list} was {@code null}
     */
    CARAPI ConvertStringListToIntArray(
        /* [in] */ IList* list,
        /* [in] */ ArrayOf<String>* convertFrom,
        /* [in] */ ArrayOf<Int32>* convertTo,
        /* [out, callee] */ ArrayOf<Int32>** outarr);

    /**
     * Create an {@code List<Integer>} from the {@code List<>} by using {@code convertFrom} and
     * {@code convertTo} as a one-to-one map (via the index).
     *
     * <p>Strings not appearing in {@code convertFrom} are ignored (with a logged warning);
     * strings appearing in {@code convertFrom} but not {@code convertTo} are silently
     * dropped.</p>
     *
     * @param list Source list of strings
     * @param convertFrom Conversion list of strings
     * @param convertTo Conversion list of ints
     * @return A list of ints where the values correspond to the ones in {@code convertTo}
     *         or {@code null} if {@code list} was {@code null}
     */
    CARAPI ConvertStringListToIntList(
        /* [in] */ IList* list,
        /* [in] */ ArrayOf<String>* convertFrom,
        /* [in] */ ArrayOf<Int32>* convertTo,
        /* [out] */ IList** outlist);

    /**
     * Convert the list of integers in {@code list} to an {@code int} array.
     *
     * <p>Every element in {@code list} must be non-{@code null}.</p>
     *
     * @param list a list of non-{@code null} integers
     *
     * @return a new int array containing all the elements from {@code list}
     *
     * @throws NullPointerException if any of the elements in {@code list} were {@code null}
     */
    CARAPI ToIntArray(
        /* [in] */ IList* list,
        /* [out, callee] */ ArrayOf<Int32>** outarr);

    /**
     * Returns true if the given {@code array} contains the given element.
     *
     * @param array {@code array} to check for {@code elem}
     * @param elem {@code elem} to test for
     * @return {@code true} if the given element is contained
     */
    CARAPI Contains(
        /* [in] */ ArrayOf<Int32>* array,
        /* [in] */ Int32 elem,
        /* [out] */ Boolean* value);

    /**
     * Returns true if the given {@code array} contains the given element.
     *
     * @param array {@code array} to check for {@code elem}
     * @param elem {@code elem} to test for
     * @return {@code true} if the given element is contained
     */
    CARAPI Contains(
        /* [in] */ ArrayOf<IInterface*>* array,
        /* [in] */ IInterface* elem,
        /* [out] */ Boolean* value);
};

} // namespace Utils
} // namespace Camera2
} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif  // __ELASTOS_DROID_HARDWARE_CARRAYUTILS_H__
