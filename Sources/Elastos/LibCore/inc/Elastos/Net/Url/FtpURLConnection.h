
#ifndef __FTPHANDLER_H__
#define __FTPHANDLER_H__

#include "Elastos.Core_server.h"
#include "URLConnection.h"

using Elastos::Net::IURLConnection;
using Elastos::Net::IURL;

namespace Elastos {
namespace Net {
namespace Url {

class FtpURLConnection
    : public ElRefBase
    , public ICacheRequest
    , public URLConnection
{
public:
    CAR_INTERFACE_DECL()

    CARAPI Connect();

    CARAPI GetContentType(
        /* [out] */ String* type);

    CARAPI GetInputStream(
        /* [out] */ IInputStream** is);

    CARAPI GetOutputStream(
        /* [out] */ IOutputStream** os);

//        public java.security.Permission getPermission();

    CARAPI SetDoInput(
        /* [in] */ Boolean newValue);

    CARAPI SetDoOutput(
        /* [in] */ Boolean newValue);

protected:
    CARAPI Init(
        /* [in] */ IURL* url);

    FtpURLConnection(
        /* [in] */ IURL* url);

    FtpURLConnection(
        /* [in] */ IURL* url,
        /* [in] */ IProxy* proxy);

private:
    CARAPI Cd();

    CARAPI ConnectInternal();

    CARAPI GetFile();

    CARAPI GetReply(
        /* [out] */ Int32* value);

    CARAPI Login();

    CARAPI Port();

    CARAPI_(String) ReadLine();

    CARAPI_(Boolean) ReadMultiLine();

    CARAPI SendFile();

    CARAPI SetType();

    CARAPI_(void) Write(
        /* [in] */ const String& command);

private:

    static const Int32 FTP_PORT; // = 21;

    // FTP Reply Constants
    static const Int32 FTP_DATAOPEN; // = 125;

    static const Int32 FTP_OPENDATA; // = 150;

    static const Int32 FTP_OK; // = 200;

    static const Int32 FTP_USERREADY; // = 220;

    static const Int32 FTP_TRANSFEROK; // = 226;

    // static const Int32 FTP_PASV = 227;

    static const Int32 FTP_LOGGEDIN; // = 230;

    static const Int32 FTP_FILEOK; // = 250;

    static const Int32 FTP_PASWD; // = 331;

    // static const Int32 FTP_DATAERROR = 451;

    // static const Int32 FTP_ERROR = 500;

    static const Int32 FTP_NOTFOUND; // = 550;

    AutoPtr<ISocket> mControlSocket;

    AutoPtr<ISocket> mDataSocket;

    AutoPtr<IServerSocket> mAcceptSocket;

    AutoPtr<IInputStream> mCtrlInput;

    AutoPtr<IInputStream> mInputStream;

    AutoPtr<IOutputStream> mCtrlOutput;

    Int32 mDataPort;

    String mUsername; // = "anonymous";

    String mPassword; // = "";

    String mReplyCode;

    String mHostName;

    AutoPtr<IProxy> mProxy;

    AutoPtr<IProxy> mCurrentProxy;

    AutoPtr<IURI> mUri;
};

} // namespace Url
} // namespace Net
} // namespace Elastos

#endif //__FTPHANDLER_H__
