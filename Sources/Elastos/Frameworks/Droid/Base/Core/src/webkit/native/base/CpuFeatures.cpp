
#include "webkit/native/base/CpuFeatures.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

/**
 * Return the number of CPU Cores on the device.
 */
Int32 CpuFeatures::GetCount()
{
    return NativeGetCoreCount();
}

/**
 * Return the CPU feature mask.
 * This is a 64-bit integer that corresponds to the CPU's features.
 * The value comes directly from android_getCpuFeatures().
 */
Int64 CpuFeatures::GetMask()
{
    return NativeGetCpuFeatures();
}

Int32 CpuFeatures::NativeGetCoreCount()
{
}

Int64 CpuFeatures::NativeGetCpuFeatures()
{
}

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos