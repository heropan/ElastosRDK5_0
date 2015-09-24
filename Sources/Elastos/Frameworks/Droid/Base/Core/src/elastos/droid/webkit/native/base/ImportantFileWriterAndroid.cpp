
#include "webkit/native/base/ImportantFileWriterAndroid.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

/**
 * Write a binary file atomically.
 *
 * This either writes all the data or leaves the file unchanged.
 *
 * @param fileName The complete path of the file to be written
 * @param data The data to be written to the file
 * @return true if the data was written to the file, false if not.
 */
Boolean ImportantFileWriterAndroid::WriteFileAtomically(
    /* [in] */ const String& fileName,
    /* [in] */ ArrayOf<Byte>* data)
{
    return NativeWriteFileAtomically(fileName, data);
}

Boolean ImportantFileWriterAndroid::NativeWriteFileAtomically(
    /* [in] */ const String& fileName,
    /* [in] */ ArrayOf<Byte>* data)
{
}

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos