
#ifndef __ELASTOS_IO_PIPEDWRITER_H__
#define __ELASTOS_IO_PIPEDWRITER_H__

#include "Writer.h"

namespace Elastos {
namespace IO {

class PipedWriter
    : public Writer
    , public IPipedWriter
{
public:
    CAR_INTERFACE_DECL()

    PipedWriter(){}

    CARAPI Connect(
        /* [in] */ IPipedReader* reader)
    {
        return NOERROR;
    }

    CARAPI Write(
        /* [in] */ ArrayOf<Char32>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count)
    {
        return NOERROR;
    }

    CARAPI Close()
    {
        return NOERROR;
    }

    CARAPI Flush()
    {
        return NOERROR;
    }
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_PIPEDWRITER_H__