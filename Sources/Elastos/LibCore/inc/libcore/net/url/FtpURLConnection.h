
#ifndef __LIBCORE_NET_URI_FTPHANDLER_H__
#define __LIBCORE_NET_URI_FTPHANDLER_H__

#include "URLConnection.h"

using Elastos::Net::IURLConnection;
using Elastos::Net::IURL;
using Elastos::Net::URLConnection;
using Elastos::Net::ISocket;
using Elastos::Net::IServerSocket;
using Elastos::Net::IURI;

namespace Libcore {
namespace Net {
namespace Url {

extern "C"  const InterfaceID EIID_FtpURLConnection;

class FtpURLConnection
    : public URLConnection
    , public IFtpURLConnection
{
public:
    CAR_INTERFACE_DECL()

    CARAPI ToString(
        /* [out] */ String* result);

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

    CARAPI constructor(
        /* [in] */ IURL* url);

    FtpURLConnection()

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

    AutoPtr<Elastos::Net::IProxy> mProxy;

    AutoPtr<Elastos::Net::IProxy> mCurrentProxy;

    AutoPtr<IURI> mUri;
};

} // namespace Url
} // namespace Net
} // namespace Libcore

#endif //__LIBCORE_NET_URI_FTPHANDLER_H__
