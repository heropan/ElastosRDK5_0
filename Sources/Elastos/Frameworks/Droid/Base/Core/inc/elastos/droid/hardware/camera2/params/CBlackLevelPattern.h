
#ifndef __ELASTOS_DROID_HARDWARE_CAMERA2_PARAMS_CCARRIERFREQUENCYRANGE_H__
#define __ELASTOS_DROID_HARDWARE_CAMERA2_PARAMS_CCARRIERFREQUENCYRANGE_H__

#include "_Elastos_Droid_Hardware_Camera2_Params_CBlackLevelPattern.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Camera2 {
namespace Params {

CarClass(CBlackLevelPattern)
    , public Object
    , public IBlackLevelPattern
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CBlackLevelPattern();

    virtual ~CBlackLevelPattern();

    CARAPI constructor();

    /**
     * Create a new {@link BlackLevelPattern} from a given offset array.
     *
     * <p>The given offset array must contain offsets for each color channel in
     * a 2x2 pattern corresponding to the color filter arrangement.  Offsets are
     * given in row-column scan order.</p>
     *
     * @param offsets an array containing a 2x2 pattern of offsets.
     *
     * @throws IllegalArgumentException if the given array has an incorrect length.
     * @throws NullPointerException if the given array is null.
     * @hide
     */
    CARAPI constructor(
        /* [in] */ ArrayOf<Int32>* offsets);

    /**
     * Return the color channel offset for a given index into the array of raw pixel values.
     *
     * @param column the column index in the the raw pixel array.
     * @param row the row index in the raw pixel array.
     * @return a color channel offset.
     *
     * @throws IllegalArgumentException if a column or row given is negative.
     */
    CARAPI GetOffsetForIndex(
        /* [in] */ Int32 column,
        /* [in] */ Int32 row,
        /* [out] */ Int32* value);

    /**
     * Copy the ColorChannel offsets into the destination vector.
     *
     * <p>Offsets are given in row-column scan order for a given 2x2 color pattern.</p>
     *
     * @param destination an array big enough to hold at least {@value #COUNT} elements after the
     *          {@code offset}
     * @param offset a non-negative offset into the array
     *
     * @throws IllegalArgumentException if the offset is invalid.
     * @throws ArrayIndexOutOfBoundsException if the destination vector is too small.
     * @throws NullPointerException if the destination is null.
     */
    CARAPI CopyTo(
        /* [in, out] */ ArrayOf<Int32>* destination,
        /* [in] */ Int32 offset);

    /**
     * Check if this {@link BlackLevelPattern} is equal to another {@link BlackLevelPattern}.
     *
     * <p>Two vectors are only equal if and only if each of the respective elements is equal.</p>
     *
     * @return {@code true} if the objects were equal, {@code false} otherwise
     */
    // @Override
    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* value);

    /**
     * {@inheritDoc}
     */
    // @Override
    CARAPI GetHashCode(
        /* [out] */ Int32* value);

private:
    AutoPtr<ArrayOf<Int32> > mCfaOffsets;
};

} // namespace Params
} // namespace Camera2
} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_HARDWARE_CAMERA2_PARAMS_CCARRIERFREQUENCYRANGE_H__
