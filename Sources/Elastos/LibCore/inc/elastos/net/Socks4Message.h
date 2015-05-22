#ifndef __SOCKS4MESSAGE_H__
#define __SOCKE4MESSAGE_H__

#include "Elastos.CoreLibrary_server.h"

namespace Elastos {
namespace Net {

class Socks4Message
{
public:
    static const Int32 COMMAND_CONNECT = 1;
    static const Int32 COMMAND_BIND = 2;
    static const Int32 RETURN_SUCCESS = 90;
    static const Int32 RETURN_FAILURE = 91;
    static const Int32 RETURN_CANNOT_CONNECT_TO_IDENTD = 92;
    static const Int32 RETURN_DIFFERENT_USER_IDS = 93;
    static const Int32 REPLY_LENGTH = 8;
    static const Int32 INDEX_VERSION = 0;

private:
    static const Int32 SOCKS_VERSION = 4;
    static const Int32 INDEX_COMMAND = 1;
    static const Int32 INDEX_PORT = 2;
    static const Int32 INDEX_IP = 4;
    static const Int32 INDEX_USER_ID = 8;
    static const Int32 BUFFER_LENGTH = 256;
    static const Int32 MAX_USER_ID_LENGTH = BUFFER_LENGTH - INDEX_USER_ID;

protected:
    AutoPtr<ArrayOf<Byte> > mBuffer;

public:
    Socks4Message();

    CARAPI Init();

    CARAPI_(Int32) GetCommandOrResult();

    CARAPI_(void) SetCommandOrResult(
        /* [in] */ Int32 command);

    CARAPI_(Int32) GetPort();

    CARAPI_(void) SetPort(
        /* [in] */ Int32 port);

    CARAPI_(Int32) GetIP();

    CARAPI_(void) SetIP(
        /* [in] */ const ArrayOf<Byte>& ip);

    CARAPI_(String) GetUserId();

    CARAPI_(void) SetUserId(
        /* [in] */ const String& id);

    CARAPI_(String) ToString();

    CARAPI_(Int32) GetLength();

    CARAPI_(String) GetErrorString(
        /* [in] */ Int32 error);

    CARAPI_(AutoPtr<ArrayOf<Byte> >) GetBytes();

private:
    CARAPI_(Int32) GetInt16(
        /* [in] */ Int32 offset);

    CARAPI_(Int32) GetInt32(
        /* [in] */ Int32 offset);

    CARAPI_(String) GetString(
         /* [in] */ Int32 offset,
         /* [in] */ Int32 maxLength);

    CARAPI_(Int32) GetVersionNumber();

    CARAPI_(void) SetInt16(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 value);

    CARAPI_(void) SetString(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 maxLength,
        /* [in] */ const String& theString);

    CARAPI_(void) SetVersionNumber(int number);
};

} // namespace Net
} // namespace Elastos

#endif //__SOCKS4MESSAGE_H__
