
#ifndef __CSOCKS4MESSAGE_H__
#define __CSOCKS4MESSAGE_H__

#include "_CSocks4Message.h"
#include "Socks4Message.h"

namespace Elastos {
namespace Net {

CarClass(CSocks4Message) , public Socks4Message
{
public:
    CARAPI constructor();

    /**
     * Get the request's command or result.
     */
    CARAPI GetCommandOrResult(
        /* [out] */ Int32* result);

    /**
     * Set the request's command or result.
     */
    CARAPI SetCommandOrResult(
        /* [in] */ Int32 command);

    /**
     * Returns the request's port number.
     */
    CARAPI GetPort(
        /* [out] */ Int32* port);

    /**
     * Set the request's port number.
     */
    CARAPI SetPort(
        /* [in] */ Int32 port);
    /**
     * Returns the IP address of the request as an integer.
     */
    CARAPI GetIP(
        /* [out] */ Int32* ip);

    /**
     * Set the IP address. This expects an array of four bytes in host order.
     */
    CARAPI SetIP(
        /* [in] */ ArrayOf<Byte>* ip);

    /**
     * Returns the user id for authentication.
     */
    CARAPI GetUserId(
        /* [out] */ String* id);
    /**
     * Set the user id for authentication.
     */
    CARAPI SetUserId(
        /* [in] */ const String& id);

    /**
     * Returns the total number of bytes used for the request. This method
     * searches for the end of the user id, then searches for the end of the
     * password and returns the final index as the requests length.
     */
    CARAPI GetLength(
        /* [out] */ Int32* length);
    /**
     * Returns an error string corresponding to the given error value.
     */
    CARAPI GetErrorString(
        /* [in] */ Int32 error,
        /* [out] */ String* str);
    /**
     * Returns the message's byte buffer.
     */
    CARAPI GetBytes(
        /* [out, callee] */ ArrayOf<Byte>** bytes);
};

} // namespace Net
} // namespace Elastos

#endif //__CSOCKS4MESSAGE_H__
