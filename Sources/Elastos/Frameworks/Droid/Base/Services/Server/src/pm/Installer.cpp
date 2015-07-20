
#include "pm/Installer.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Droid::Net::CLocalSocket;
using Elastos::Droid::Net::ILocalSocketAddress;
using Elastos::Droid::Net::CLocalSocketAddress;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Pm {

const String Installer::TAG("Installer");
const Boolean Installer::LOCAL_DEBUG;

Installer::Installer()
    : mBuflen(0)
{
    mBuf = ArrayOf<Byte>::Alloc(1024);
}

Int32 Installer::Install(
    /* [in] */ const String& name,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    StringBuilder builder;
    builder.AppendCStr("install");
    builder.AppendChar(' ');
    builder.AppendString(name);
    builder.AppendChar(' ');
    builder.AppendInt32(uid);
    builder.AppendChar(' ');
    builder.AppendInt32(gid);
    return Execute(builder.ToString());
}

Boolean Installer::Connect()
{
    if (mSocket != NULL) {
        return TRUE;
    }
    Slogger::I(TAG, "connecting...");
    ECode ec;
    do {
        CLocalSocket::New((ILocalSocket**)&mSocket);

        AutoPtr<ILocalSocketAddress> address;
        CLocalSocketAddress::New(
            String("installd"), Elastos::Droid::Net::LocalSocketAddressNamespace_RESERVED,
            (ILocalSocketAddress**)&address);

        ec = mSocket->Connect(address);
        if (FAILED(ec))
            break;

        mIn = NULL;
        ec = mSocket->GetInputStream((IInputStream**)&mIn);
        if (FAILED(ec))
            break;
        mOut = NULL;
        ec = mSocket->GetOutputStream((IOutputStream**)&mOut);
    } while (0);

    if (FAILED(ec)) {
        Disconnect();
        return FALSE;
    }

    return TRUE;
}

void Installer::Disconnect()
{
    Slogger::I(TAG, "disconnecting...");

    if (mSocket != NULL)
        mSocket->Close();

    if (mIn != NULL)
        mIn->Close();

    if (mOut != NULL)
        mOut->Close();

    mSocket = NULL;
    mIn = NULL;
    mOut = NULL;
}

Boolean Installer::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 len)
{
    Int32 off = 0, count;
    if (len < 0) {
        return FALSE;
    }
    while (off != len) {
        // try {
        if (SUCCEEDED(mIn->ReadBytes(buffer, off, len - off, &count))) {
            if (count <= 0) {
                Slogger::E(TAG, "read error %d", count);
                break;
            }
            off += count;
        }
        else {
            Slogger::E(TAG, "read exception");
            break;
        }
        // } catch (IOException ex) {
        //     Slog.e(TAG, "read exception");
        //     break;
        // }
    }
    if (LOCAL_DEBUG) {
        Slogger::I(TAG, "read %d bytes", len);
    }
    if (off == len) {
        return TRUE;
    }
    Disconnect();
    return FALSE;
}

Boolean Installer::ReadReply()
{
    Int32 len;
    mBuflen = 0;
    if (!ReadBytes(mBuf, 2)) {
        return FALSE;
    }
    len = (((Int32) (*mBuf)[0]) & 0xff) | ((((Int32) (*mBuf)[1]) & 0xff) << 8);
    if ((len < 1) || (len > 1024)) {
        Slogger::E(TAG, "invalid reply length (%d)", len);
        Disconnect();
        return FALSE;
    }
    if (!ReadBytes(mBuf, len)) {
        return FALSE;
    }
    mBuflen = len;
    return TRUE;
}

Boolean Installer::WriteCommand(
    /* [in] */ const String& _cmd)
{
    AutoPtr< ArrayOf<Byte> > cmd = ArrayOf<Byte>::Alloc((Byte*)_cmd.string(), _cmd.GetByteLength());
    Int32 len = cmd->GetLength();
    if ((len < 1) || (len > 1024)) {
        return FALSE;
    }
    (*mBuf)[0] = (Byte) (len & 0xff);
    (*mBuf)[1] = (Byte) ((len >> 8) & 0xff);
    // try {
    mOut->WriteBytes(*mBuf, 0, 2);
    mOut->WriteBytes(*cmd, 0, len);
    // } catch (IOException ex) {
    //     Slog.e(TAG, "write error");
    //     disconnect();
    //     return FALSE;
    // }
    return TRUE;
}

String Installer::Transaction(
    /* [in] */ const String& cmd)
{
    AutoLock lock(mLock);

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
    if (ReadReply()) {
        String s((const char*)mBuf->GetPayload(), mBuflen);
        if (LOCAL_DEBUG) {
            Slogger::I(TAG, "recv: 's'", s.string());
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

Int32 Installer::Execute(
    /* [in] */ const String& cmd)
{
    String res = Transaction(cmd);
    // try {
    return StringUtils::ParseInt32(res, 10, -1);
    // } catch (NumberFormatException ex) {
    //     return -1;
    // }
}

Int32 Installer::Dexopt(
    /* [in] */ const String& apkPath,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean isPublic)
{
    StringBuilder builder;
    builder.AppendCStr("dexopt");
    builder.AppendChar(' ');
    builder.AppendString(apkPath);
    builder.AppendChar(' ');
    builder.AppendInt32(uid);
    builder.AppendCStr(isPublic ? " 1" : " 0");
    return Execute(builder.ToString());
}

Int32 Installer::Movedex(
    /* [in] */ const String& srcPath,
    /* [in] */ const String& dstPath)
{
    StringBuilder builder;
    builder.AppendCStr("movedex");
    builder.AppendChar(' ');
    builder.AppendString(srcPath);
    builder.AppendChar(' ');
    builder.AppendString(dstPath);
    return Execute(builder.ToString());
}

Int32 Installer::Rmdex(
    /* [in] */ const String& codePath)
{
    StringBuilder builder;
    builder.AppendCStr("rmdex");
    builder.AppendChar(' ');
    builder.AppendString(codePath);
    return Execute(builder.ToString());
}

Int32 Installer::Remove(
    /* [in] */ const String& name,
    /* [in] */ Int32 userId)
{
    StringBuilder builder;
    builder.AppendCStr("remove");
    builder.AppendChar(' ');
    builder.AppendString(name);
    builder.AppendChar(' ');
    builder.AppendInt32(userId);
    return Execute(builder.ToString());
}

Int32 Installer::Rename(
    /* [in] */ const String& oldname,
    /* [in] */ const String& newname)
{
    StringBuilder builder;
    builder.AppendCStr("rename");
    builder.AppendChar(' ');
    builder.AppendString(oldname);
    builder.AppendChar(' ');
    builder.AppendString(newname);
    return Execute(builder.ToString());
}

Int32 Installer::FixUid(
    /* [in] */ const String& name,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    StringBuilder builder;
    builder.AppendCStr("fixuid");
    builder.AppendChar(' ');
    builder.AppendString(name);
    builder.AppendChar(' ');
    builder.AppendInt32(uid);
    builder.AppendChar(' ');
    builder.AppendInt32(gid);
    return Execute(builder.ToString());
}

Int32 Installer::DeleteCacheFiles(
    /* [in] */ const String& name,
    /* [in] */ Int32 userId)
{
    StringBuilder builder;
    builder.AppendCStr("rmcache");
    builder.AppendChar(' ');
    builder.AppendString(name);
    builder.AppendChar(' ');
    builder.AppendInt32(userId);
    return Execute(builder.ToString());
}

Int32 Installer::CreateUserData(
    /* [in] */ const String& name,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 userId)
{
    StringBuilder builder;
    builder.AppendCStr("mkuserdata");
    builder.AppendChar(' ');
    builder.AppendString(name);
    builder.AppendChar(' ');
    builder.AppendInt32(uid);
    builder.AppendChar(' ');
    builder.AppendInt32(userId);
    return Execute(builder.ToString());
}

Int32 Installer::RemoveUserDataDirs(
    /* [in] */ Int32 userId)
{
    StringBuilder builder;
    builder.AppendCStr("rmuser");
    builder.AppendChar(' ');
    builder.AppendInt32(userId);
    return Execute(builder.ToString());
}

Int32 Installer::ClearUserData(
    /* [in] */ const String& name,
    /* [in] */ Int32 userId)
{
    StringBuilder builder;
    builder.AppendCStr("rmuserdata");
    builder.AppendChar(' ');
    builder.AppendString(name);
    builder.AppendChar(' ');
    builder.AppendInt32(userId);
    return Execute(builder.ToString());
}

Int32 Installer::CloneUserData(
    /* [in] */ Int32 srcUserId,
    /* [in] */ Int32 targetUserId,
    /* [in] */ Boolean copyData)
{
    StringBuilder builder;
    builder.AppendCStr("cloneuserdata");
    builder.AppendChar(' ');
    builder.AppendInt32(srcUserId);
    builder.AppendChar(' ');
    builder.AppendInt32(targetUserId);
    builder.AppendChar(' ');
    builder.AppendChar(copyData ? '1' : '0');
    return Execute(builder.ToString());
}

Boolean Installer::Ping()
{
    if (Execute(String("ping")) < 0) {
        return FALSE;
    }
    else {
        return TRUE;
    }
}

Int32 Installer::FreeCache(
    /* [in] */ Int64 freeStorageSize)
{
    StringBuilder builder;
    builder.AppendCStr("freecache");
    builder.AppendChar(' ');
    builder.AppendString(StringUtils::Int64ToString(freeStorageSize));
    return Execute(builder.ToString());
}

Int32 Installer::GetSizeInfo(
    /* [in] */ const String& pkgName,
    /* [in] */ Int32 persona,
    /* [in] */ const String& apkPath,
    /* [in] */ const String& fwdLockApkPath,
    /* [in] */ const String& asecPath,
    /* [in] */ IPackageStats* pStats)
{
    StringBuilder builder;
    builder.AppendCStr("getsize");
    builder.AppendChar(' ');
    builder.AppendString(pkgName);
    builder.AppendChar(' ');
    builder.AppendInt32(persona);
    builder.AppendChar(' ');
    builder.AppendString(apkPath);
    builder.AppendChar(' ');
    builder.AppendString(!fwdLockApkPath.IsNull() ? fwdLockApkPath : String("!"));
    builder.AppendChar(' ');
    builder.AppendString(!asecPath.IsNull() ? asecPath : String("!"));

    String s = Transaction(builder.ToString());
    AutoPtr< ArrayOf<String> > res;
    StringUtils::Split(s, String(" "), (ArrayOf<String>**)&res);

    if ((res == NULL) || (res->GetLength() != 5)) {
        return -1;
    }
    // try {
    pStats->SetCodeSize(StringUtils::ParseInt64((*res)[1]));
    pStats->SetDataSize(StringUtils::ParseInt64((*res)[2]));
    pStats->SetCacheSize(StringUtils::ParseInt64((*res)[3]));
    pStats->SetExternalCodeSize(StringUtils::ParseInt64((*res)[4]));
    return StringUtils::ParseInt32((*res)[0], 10, -1);
    // } catch (NumberFormatException e) {
    //     return -1;
    // }
}

Int32 Installer::MoveFiles()
{
    return Execute(String("movefiles"));
}

Int32 Installer::LinkNativeLibraryDirectory(
    /* [in] */ const String& dataPath,
    /* [in] */ const String& nativeLibPath,
    /* [in] */ Int32 userId)
{
    if (dataPath.IsNull()) {
        Slogger::E(TAG, "linkNativeLibraryDirectory dataPath is null");
        return -1;
    }
    else if (nativeLibPath.IsNull()) {
        Slogger::E(TAG, "linkNativeLibraryDirectory nativeLibPath is null");
        return -1;
    }

    StringBuilder builder;
    builder.AppendCStr("linklib ");
    builder.AppendString(dataPath);
    builder.AppendChar(' ');
    builder.AppendString(nativeLibPath);
    builder.AppendChar(' ');
    builder.AppendInt32(userId);

    return Execute(builder.ToString());
}

} // namespace Pm
} // namespace Server
} // namespace Droid
} // namespace Elastos
