
#include "CSocks4Message.h"

namespace Elastos {
namespace Net {

ECode CSocks4Message::constructor()
{
    return Socks4Message::Init();
}

ECode CSocks4Message::GetCommandOrResult(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    *result = Socks4Message::GetCommandOrResult();
    return NOERROR;
}
ECode CSocks4Message::SetCommandOrResult(
    /* [in] */ Int32 command)
{
    Socks4Message::SetCommandOrResult(command);
    return NOERROR;
}
ECode CSocks4Message::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port)

    *port = Socks4Message::GetPort();
    return NOERROR;
}
ECode CSocks4Message::SetPort(
    /* [in] */ Int32 port)
{
    Socks4Message::SetPort(port);
    return NOERROR;
}
ECode CSocks4Message::GetIP(
    /* [out] */ Int32* ip)
{
    VALIDATE_NOT_NULL(ip)

    *ip = Socks4Message::GetIP();
    return NOERROR;
}
ECode CSocks4Message::SetIP(
    /* [in] */ ArrayOf<Byte>* ip)
{
    Socks4Message::SetIP(*ip);
    return NOERROR;
}
ECode CSocks4Message::GetUserId(
    /* [out] */ String* id)
{
    VALIDATE_NOT_NULL(id)

    *id = Socks4Message::GetUserId();
    return NOERROR;
}
ECode CSocks4Message::SetUserId(
    /* [in] */ const String& id)
{
    Socks4Message::SetUserId(id);
    return NOERROR;
}
ECode CSocks4Message::GetLength(
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length)

    *length = Socks4Message::GetLength();
    return NOERROR;
}
ECode CSocks4Message::GetErrorString(
    /* [in] */ Int32 error,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = Socks4Message::GetErrorString(error);
    return NOERROR;
}
ECode CSocks4Message::GetBytes(
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    VALIDATE_NOT_NULL(bytes)

    AutoPtr<ArrayOf<Byte> > outbyte = Socks4Message::GetBytes();
    *bytes = outbyte;
    REFCOUNT_ADD(*bytes)
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
