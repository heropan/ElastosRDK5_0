
#include "PipedOutputStream.h"

namespace Elastos {
namespace IO {

CAR_INTERFACE_IMPL(PipedOutputStream, OutputStream, IPipedOutputStream)

ECode PipedOutputStream::Connect(
    /* [in] */ IPipedInputStream* ipis)
{
    return NOERROR;
}

ECode PipedOutputStream::Write(
    /* [in] */ Int32 oneByte)
{

}

} // namespace IO
} // namespace Elastos
