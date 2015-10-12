
#include "internal/os/InstallerConnection.h"
#include "net/CLocalSocket.h"
#include "net/CLocalSocketAddress.h"
#include "net/CLocalSocketAddressNamespace.h"
#include <elastos/core/AutoLock.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Net::CLocalSocket;
using Elastos::Droid::Net::CLocalSocketAddress;
using Elastos::Droid::Net::ILocalSocketAddress;
using Elastos::Droid::Net::CLocalSocketAddressNamespace;
using Elastos::Droid::Net::ILocalSocketAddressNamespace;
using Elastos::Core::AutoLock;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::IO::ICloseable;
using Elastos::Utility::Logging::Slogger;
using Libcore::IO::CIoUtils;
using Libcore::IO::IIoUtils;
using Libcore::IO::CStreams;
using Libcore::IO::IStreams;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Os {

const String InstallerConnection::TAG("InstallerConnection");
const Boolean InstallerConnection::LOCAL_DEBUG = FALSE;

InstallerConnection::InstallerConnection()
{
    mBuf = ArrayOf<Byte>::Alloc(1024);
}

String InstallerConnection::Transact(
    /* [in] */ const String& cmd)
{
    AutoLock lock(this);
    if (!Connect()) {
        Slogger::E(TAG, "connection failed");
        return String("-1");
    }

    if (!WriteCommand(cmd)) {
        /*
         * If installd died and restarted in the background (unlikely but
         * possible) we'll fail on the next write (this one). Try to
         * reconnect and write the command one more time before giving up.
         */
        Slogger::E(TAG, "write command failed? reconnect!");
        if (!Connect() || !WriteCommand(cmd)) {
            return String("-1");
        }
    }
    if (LOCAL_DEBUG) {
        Slogger::I(TAG, "send: '%s'", cmd.string());
    }

    Int32 replyLength = ReadReply();
    if (replyLength > 0) {
        String s(*mBuf, 0, replyLength);
        if (LOCAL_DEBUG) {
            Slogger::I(TAG, "recv: '%s'", s.string());
        }
        return s;
    }
    else {
        if (LOCAL_DEBUG) {
            Slogger::I(TAG, "fail");
        }
        return String("-1");
    }
}

Int32 InstallerConnection::Execute(
    /* [in] */ const String& cmd)
{
    String res = Transact(cmd);
    return StringUtils::ParseInt32(res, 10 , -1);
}

Int32 InstallerConnection::Dexopt(
    /* [in] */ const String& apkPath,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean isPublic,
    /* [in] */ const String& instructionSet)
{
    return Dexopt(apkPath, uid, isPublic, String("*"), instructionSet, FALSE);
}

Int32 InstallerConnection::Dexopt(
    /* [in] */ const String& apkPath,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean isPublic,
    /* [in] */ const String& pkgName,
    /* [in] */ const String& instructionSet,
    /* [in] */ Boolean vmSafeMode)
{
    StringBuilder builder("dexopt");
    builder.AppendChar(' ');
    builder.Append(apkPath);
    builder.AppendChar(' ');
    builder.Append(uid);
    builder.Append(isPublic ? " 1" : " 0");
    builder.AppendChar(' ');
    builder.Append(pkgName);
    builder.AppendChar(' ');
    builder.Append(instructionSet);
    builder.AppendChar(' ');
    builder.Append(vmSafeMode ? " 1" : " 0");
    return Execute(builder.ToString());
}

Int32 InstallerConnection::Patchoat(
    /* [in] */ const String& apkPath,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean isPublic,
    /* [in] */ const String& instructionSet)
{
    return Patchoat(apkPath, uid, isPublic, String("*"), instructionSet);
}

Int32 InstallerConnection::Patchoat(
    /* [in] */ const String& apkPath,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean isPublic,
    /* [in] */ const String& pkgName,
    /* [in] */ const String& instructionSet)
{
    StringBuilder builder("patchoat");
    builder.AppendChar(' ');
    builder.Append(apkPath);
    builder.AppendChar(' ');
    builder.Append(uid);
    builder.Append(isPublic ? " 1" : " 0");
    builder.AppendChar(' ');
    builder.Append(pkgName);
    builder.AppendChar(' ');
    builder.Append(instructionSet);
    return Execute(builder.ToString());
}

Boolean InstallerConnection::Connect()
{
    if (mSocket != NULL) {
        return TRUE;
    }
    Slogger::I(TAG, "connecting...");
    ECode ec = NOERROR;
    do {
        CLocalSocket::New((ILocalSocket**)&mSocket);

        AutoPtr<ILocalSocketAddressNamespace> reserved;
        CLocalSocketAddressNamespace::New(ILocalSocketAddressNamespace::LocalSocketAddressNamespace_RESERVED,
            (ILocalSocketAddressNamespace**)&reserved);
        AutoPtr<ILocalSocketAddress> address;
        CLocalSocketAddress::New(String("installd"), reserved, (ILocalSocketAddress**)&address);

        ec = mSocket->Connect(address);
        if (FAILED(ec))
            break;

        ec = mSocket->GetInputStream((IInputStream**)&mIn);
        if (FAILED(ec))
            break;
        ec = mSocket->GetOutputStream((IOutputStream**)&mOut);
    } while (0);

    if (FAILED(ec)) { //E_IO_EXCEPTION
        Disconnect();
        return FALSE;
    }
    return TRUE;
}

void InstallerConnection::Disconnect()
{
    Slogger::I(TAG, "disconnecting...");
    AutoPtr<IIoUtils> ioUtils;
    CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
    ioUtils->CloseQuietly(ICloseable::Probe(mSocket));
    ioUtils->CloseQuietly(ICloseable::Probe(mIn));
    ioUtils->CloseQuietly(ICloseable::Probe(mOut));

    mSocket = NULL;
    mIn = NULL;
    mOut = NULL;
}

Boolean InstallerConnection::ReadFully(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 len)
{
    AutoPtr<IStreams> streams;
    CStreams::AcquireSingleton((IStreams**)&streams);
    if (FAILED(streams->ReadFully(mIn, buffer, 0, len))) { //E_IO_EXCEPTION
        Slogger::E(TAG, "read exception");
        Disconnect();
        return FALSE;
    }

    if (LOCAL_DEBUG) {
        Slogger::I(TAG, "read %d bytes", len);
    }

    return TRUE;
}

Int32 InstallerConnection::ReadReply()
{
    if (!ReadFully(mBuf, 2)) {
        return -1;
    }

    Int32 len = (((Int32)(*mBuf)[0]) & 0xff) | ((((Int32)(*mBuf)[1]) & 0xff) << 8);
    if ((len < 1) || (len > mBuf->GetLength())) {
        Slogger::E(TAG, "invalid reply length (%d)", len);
        Disconnect();
        return -1;
    }

    if (!ReadFully(mBuf, len)) {
        return -1;
    }

    return len;
}

Boolean InstallerConnection::WriteCommand(
    /* [in] */ const String& cmdString)
{
    AutoPtr<ArrayOf<Byte> > cmd = cmdString.GetBytes();
    Int32 len = cmd->GetLength();
    if ((len < 1) || (len > mBuf->GetLength())) {
        return FALSE;
    }

    (*mBuf)[0] = (Byte)(len & 0xff);
    (*mBuf)[1] = (Byte)((len >> 8) & 0xff);
    ECode ec = NOERROR;
    do {
        ec = mOut->Write(mBuf, 0, 2);
        if (FAILED(ec))
            break;
        ec = mOut->Write(cmd, 0, len);
    } while (0);

    if (FAILED(ec)) { //E_IO_EXCEPTION
        Slogger::E(TAG, "write error");
        Disconnect();
        return FALSE;
    }
    return TRUE;
}

} // namespace Os
} // namespace Internal
} // namespace Droid
} // namespace Elastos
