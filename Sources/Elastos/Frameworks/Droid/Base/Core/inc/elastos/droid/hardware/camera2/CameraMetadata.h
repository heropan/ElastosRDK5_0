
#ifndef __ELASTOS_DROID_HARDWARE_CAMERA2_CAMERAMETADATA_H__
#define __ELASTOS_DROID_HARDWARE_CAMERA2_CAMERAMETADATA_H__

#include "Elastos.Droid.Hardware.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;
using Elastos::Utility::IList;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Camera2 {

class CameraMetadata
    : public Object
    , public ICameraMetadata
{
public:
    CAR_INTERFACE_DECL()

    /**
     * Returns a list of the keys contained in this map.
     *
     * <p>The list returned is not modifiable, so any attempts to modify it will throw
     * a {@code UnsupportedOperationException}.</p>
     *
     * <p>All values retrieved by a key from this list with {@code #get} are guaranteed to be
     * non-{@code null}. Each key is only listed once in the list. The order of the keys
     * is undefined.</p>
     *
     * @return List of the keys contained in this map.
     */
    // @SuppressWarnings("unchecked")
    CARAPI GetKeys(
        /* [out] */ IList** outlist);

protected:
    /**
     * Set a camera metadata field to a value. The field definitions can be
     * found in {@link CameraCharacteristics}, {@link CaptureResult}, and
     * {@link CaptureRequest}.
     *
     * @param key The metadata field to write.
     * @param value The value to set the field to, which must be of a matching
     * type to the key.
     *
     * @hide
     */
    CameraMetadata();

private:
    static const String TAG;
    static const Boolean VERBOSE;
};

} // namespace Camera2
} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_HARDWARE_CAMERA2_CAMERAMETADATA_H__
