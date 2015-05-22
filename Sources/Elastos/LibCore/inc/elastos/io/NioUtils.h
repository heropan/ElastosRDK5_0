#ifndef __ELASTOS_IO_NIOUTILS_H__
#define __ELASTOS_IO_NIOUTILS_H__

#include "Buffer.h"
#include "ByteBuffer.h"
#include "HeapByteBuffer.h"
#include "Elastos.CoreLibrary_server.h"

using Elastos::IO::Channels::IFileChannel;

namespace Elastos {
namespace IO {
/*
 * @ hilde internal use only
 */
class NioUtils
{
public:
    /*
     * Gets the start address of a direct buffer.
     * @ param buffer
     *      The direct buffer whose address shall be return must not be NULL;
     * @ return the address of the bugger given, or zero if the buffer is not a
     *   direct buffer;
     * */
    static Int32 GetDirectBufferAddress(Buffer* buffer);

    static CARAPI FreeDirectBuffer(ByteBuffer* buffer);

    // static IFileDescriptor GetFD(IFileChannel fc);

    static AutoPtr<IFileChannel> NewFileChannel(IObject *stream, IFileDescriptor *fd, Int32 mode);

    static ArrayOf<Byte>* GetUnsafeArray(HeapByteBuffer* b);

    static Int32 GetUnsafeArrayOffset(HeapByteBuffer* b);
private:
    NioUtils() {}
};

} // namespace IO
} // namespace Elastos

#endif
