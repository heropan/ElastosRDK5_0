
#include <Elastos.CoreLibrary.IO.h>
#include <Elastos.CoreLibrary.Net.h>
#include <Elastos.CoreLibrary.Utility.h>
#include "elastos/droid/net/dhcp/DhcpInformPacket.h"
#include "elastos/droid/net/ReturnOutValue.h"
#include "elastos/droid/net/dhcp/DhcpInformPacket.h"
#include "elastos/droid/net/dhcp/DhcpPacket.h"
#include "elastos/droid/os/Build.h"

using Elastos::Droid::Os::Build;

using Elastos::IO::CByteBufferHelper;
using Elastos::IO::IBuffer;
using Elastos::IO::IByteBufferHelper;
using Elastos::Net::IInetAddress;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Dhcp {

DhcpInformPacket::DhcpInformPacket()
{}

ECode DhcpInformPacket::constructor(
    /* [in] */ Int32 transId,
    /* [in] */ IInetAddress* clientIp,
    /* [in] */ IInetAddress* yourIp,
    /* [in] */ IInetAddress* nextIp,
    /* [in] */ IInetAddress* relayIp,
    /* [in] */ ArrayOf<Byte>* clientMac)
{
    return DhcpPacket::constructor(transId, clientIp, yourIp, nextIp, relayIp, clientMac, FALSE);
}

ECode DhcpInformPacket::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)

    String s;
    DhcpPacket::ToString(&s);
    *result = s + " INFORM";
    return NOERROR;
}

ECode DhcpInformPacket::BuildPacket(
    /* [in] */ Int32 encap,
    /* [in] */ Int16 destUdp,
    /* [in] */ Int16 srcUdp,
    /* [out] */ IByteBuffer** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IByteBuffer> rev;
    AutoPtr<IByteBufferHelper> byteBufferHelper;
    CByteBufferHelper::AcquireSingleton((IByteBufferHelper**)&byteBufferHelper);
    byteBufferHelper->Allocate(MAX_LENGTH, (IByteBuffer**)&rev);
    FillInPacket(encap, mClientIp, mYourIp, destUdp, srcUdp, rev,
        DHCP_BOOTREQUEST, FALSE);
    IBuffer::Probe(rev)->Flip();
    FUNC_RETURN(rev)
}

ECode DhcpInformPacket::FinishPacket(
    /* [in] */ IByteBuffer* buffer)
{
    AutoPtr<ArrayOf<Byte> > clientId = ArrayOf<Byte>::Alloc(7);
    (*clientId)[0] = CLIENT_ID_ETHER;
    clientId->Copy(1, mClientMac, 0, 6);
    AddTlv(buffer, DHCP_MESSAGE_TYPE, DHCP_MESSAGE_TYPE_REQUEST);
    AddTlv(buffer, DHCP_PARAMETER_LIST, mRequestedParams);
    AddTlvEnd(buffer);
    return NOERROR;
}

ECode DhcpInformPacket::DoNextOp(
    /* [in] */ DhcpStateMachine* machine)
{
    AutoPtr<IInetAddress> clientRequest =
        mRequestedIp == NULL ? mClientIp : mRequestedIp;
    return machine->OnInformReceived(mTransId, mClientMac, clientRequest,
        mRequestedParams);
}

} // namespace Dhcp
} // namespace Net
} // namespace Droid
} // namespace Elastos
