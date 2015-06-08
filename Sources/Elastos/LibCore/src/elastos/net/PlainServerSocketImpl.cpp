#include "PlainServerSocketImpl.h"

namespace Elastos {
namespace Net {

CAR_INTERFACE_IMPL(PlainServerSocketImpl, PlainSocketImpl, IPlainServerSocketImpl)

ECode PlainServerSocketImpl::constructor()
{
    return PlainSocketImpl::constructor();
}

ECode PlainServerSocketImpl::constructor(
    /* [in] */ IFileDescriptor* fd)
{
    return PlainSocketImpl::constructor(fd);
}

ECode PlainServerSocketImpl::Create(
    /* [in] */ Boolean isStreaming)
{
    ECode ec = NOERROR;
    ec = PlainSocketImpl::Create(isStreaming);
    if (FAILED(ec)) {
        return ec;
    }

    if (isStreaming) {
        SetOption(0, TRUE);
    }
    return ec;
}

} // namespace Net
} // namespace Elastos
