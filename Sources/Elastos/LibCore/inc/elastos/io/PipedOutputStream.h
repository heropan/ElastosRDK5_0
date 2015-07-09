
#ifndef __ELASTOS_IO_PIPEDOUTPUTSTREAM_H__
#define __ELASTOS_IO_PIPEDOUTPUTSTREAM_H__

#include "OutputStream.h"

namespace Elastos {
namespace IO {

class PipedOutputStream
    : public OutputStream
    , public IPipedOutputStream
{
public:
    CAR_INTERFACE_DECL()

    CARAPI Connect(
        /* [in] */ IPipedInputStream* ipis);

    CARAPI Write(
        /* [in] */ Int32 oneByte);

};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_PIPEDOUTPUTSTREAM_H__