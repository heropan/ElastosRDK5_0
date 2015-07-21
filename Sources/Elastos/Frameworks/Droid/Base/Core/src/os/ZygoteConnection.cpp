
#include "ext/frameworkdef.h"
#include "os/ZygoteConnection.h"
#include "os/CZygoteInit.h"
#include "os/SELinux.h"
#include "os/Process.h"
#include "os/RuntimeInit.h"
#include <Elastos.CoreLibrary.h>
#include <elastos/core/StringUtils.h>

using Elastos::IO::IOutputStream;
using Elastos::IO::IInputStream;
using Elastos::IO::IInputStreamReader;
using Elastos::IO::CInputStreamReader;
using Elastos::IO::CDataOutputStream;
using Elastos::IO::CBufferedReader;
using Elastos::IO::IIoUtils;
using Elastos::IO::CIoUtils;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::CDataInputStream;
using Elastos::IO::IDataInput;
using Elastos::IO::IDataOutput;
using Elastos::Core::StringUtils;
using Elastos::Droid::System::IZygote;
using Elastos::Droid::System::CZygote;
using Elastos::Droid::Os::SELinux;
using Elastos::Droid::Os::Process;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Os {

const Int32 ZygoteConnection::CONNECTION_TIMEOUT_MILLIS;
const Int32 ZygoteConnection::MAX_ZYGOTE_ARGC;
AutoPtr<ILocalSocket> ZygoteConnection::sPeerWaitSocket;

ZygoteConnection::Arguments::Arguments(
    /* [in] */ const ArrayOf<String>& args)
    : mUid(0)
    , mUidSpecified(FALSE)
    , mGid(0)
    , mGidSpecified(FALSE)
    , mPeerWait(FALSE)
    , mDebugFlags(0)
    , mMountExternal(IZygote::MOUNT_EXTERNAL_NONE)
    , mTargetSdkVersion(0)
    , mTargetSdkVersionSpecified(FALSE)
    , mRuntimeInit(FALSE)
    , mCapabilitiesSpecified(FALSE)
    , mPermittedCapabilities(0)
    , mEffectiveCapabilities(0)
    , mSeInfoSpecified(FALSE)
{
    ASSERT_SUCCEEDED(ParseArgs(args));
}

ECode ZygoteConnection::Arguments::ParseArgs(
    /* [in] */ const ArrayOf<String>& args)
{
    Int32 curArg = 0;

    for ( /* curArg */ ; curArg < args.GetLength(); curArg++) {
        String arg = args[curArg];

        if (arg.Equals("--")) {
            curArg++;
            break;
        }
        else if (arg.StartWith("--setuid=")) {
            if (mUidSpecified) {
                // throw new IllegalArgumentException(
                //         "Duplicate arg specified");
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            mUidSpecified = TRUE;
            mUid = StringUtils::ParseInt32(arg.Substring(arg.IndexOf('=') + 1));
        }
        else if (arg.StartWith("--setgid=")) {
            if (mGidSpecified) {
                // throw new IllegalArgumentException(
                //         "Duplicate arg specified");
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            mGidSpecified = TRUE;
            mGid = StringUtils::ParseInt32(arg.Substring(arg.IndexOf('=') + 1));
        }
        else if (arg.StartWith("--target-sdk-version=")) {
            if (mTargetSdkVersionSpecified) {
                // throw new IllegalArgumentException(
                //         "Duplicate target-sdk-version specified");
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            mTargetSdkVersionSpecified = TRUE;
            mTargetSdkVersion = StringUtils::ParseInt32(arg.Substring(arg.IndexOf('=') + 1));
        }
        else if (arg.Equals("--enable-debugger")) {
        //     debugFlags |= Zygote.DEBUG_ENABLE_DEBUGGER;
            assert(0);
        }
        else if (arg.Equals("--enable-safemode")) {
        //     debugFlags |= Zygote.DEBUG_ENABLE_SAFEMODE;
            assert(0);
        }
        else if (arg.Equals("--enable-checkjni")) {
        //     debugFlags |= Zygote.DEBUG_ENABLE_CHECKJNI;
            assert(0);
        }
        else if (arg.Equals("--enable-jni-logging")) {
        //     debugFlags |= Zygote.DEBUG_ENABLE_JNI_LOGGING;
            assert(0);
        }
        else if (arg.Equals("--enable-assert")) {
        //     debugFlags |= Zygote.DEBUG_ENABLE_ASSERT;
            assert(0);
        }
        else if (arg.Equals("--peer-wait")) {
            mPeerWait = TRUE;
        }
        else if (arg.Equals("--runtime-init")) {
            mRuntimeInit = TRUE;
        }
        else if (arg.StartWith("--seinfo=")) {
            if (mSeInfoSpecified) {
                // throw new IllegalArgumentException(
                //         "Duplicate arg specified");
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            mSeInfoSpecified = TRUE;
            mSeInfo = arg.Substring(arg.IndexOf('=') + 1);
        }
        else if (arg.StartWith("--capabilities=")) {
            if (mCapabilitiesSpecified) {
                // throw new IllegalArgumentException(
                //         "Duplicate arg specified");
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            mCapabilitiesSpecified = TRUE;
            String capString = arg.Substring(arg.IndexOf('=') + 1);

            AutoPtr< ArrayOf<String> > capStrings;
            StringUtils::Split(capString, String(","), 2, (ArrayOf<String>**)&capStrings);

            if (capStrings->GetLength() == 1) {
                mEffectiveCapabilities = StringUtils::ParseInt64((*capStrings)[0]);
                mPermittedCapabilities = mEffectiveCapabilities;
            }
            else {
                mPermittedCapabilities = StringUtils::ParseInt64((*capStrings)[0]);
                mEffectiveCapabilities = StringUtils::ParseInt64((*capStrings)[1]);
            }
        }
        else if (arg.StartWith("--rlimit=")) {
            // Duplicate --rlimit arguments are specifically allowed.
            // String[] limitStrings
            //         = arg.substring(arg.indexOf('=')+1).split(",");

            // if (limitStrings.length != 3) {
            //     throw new IllegalArgumentException(
            //             "--rlimit= should have 3 comma-delimited ints");
            // }
            // int[] rlimitTuple = new int[limitStrings.length];

            // for(int i=0; i < limitStrings.length; i++) {
            //     rlimitTuple[i] = Integer.parseInt(limitStrings[i]);
            // }

            // if (rlimits == null) {
            //     rlimits = new ArrayList();
            // }

            // rlimits.add(rlimitTuple);
        }
        else if (arg.Equals("-classpath")) {
            // if (classpath != null) {
            //     throw new IllegalArgumentException(
            //             "Duplicate arg specified");
            // }
            // try {
            //     classpath = args[++curArg];
            // } catch (IndexOutOfBoundsException ex) {
            //     throw new IllegalArgumentException(
            //             "-classpath requires argument");
            // }
        }
        else if (arg.StartWith("--setgroups=")) {
            if (mGids != NULL) {
                // throw new IllegalArgumentException(
                //         "Duplicate arg specified");
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }

            AutoPtr< ArrayOf<String> > params;
            String gids = arg.Substring(arg.IndexOf('=') + 1);
            StringUtils::Split(gids, String(","), (ArrayOf<String>**)&params);

            mGids = ArrayOf<Int32>::Alloc(params->GetLength());

            for (Int32 i = params->GetLength() - 1; i >= 0 ; i--) {
                (*mGids)[i] = StringUtils::ParseInt32((*params)[i]);
            }
        }
        else if (arg.Equals("--invoke-with")) {
            if (!mInvokeWith.IsNull()) {
                // throw new IllegalArgumentException(
                //         "Duplicate arg specified");
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            // try {
            mInvokeWith = args[++curArg];
            // } catch (IndexOutOfBoundsException ex) {
            //     throw new IllegalArgumentException(
            //             "--invoke-with requires argument");
            // }
        }
        else if (arg.StartWith("--nice-name=")) {
            if (!mNiceName.IsNull()) {
                // throw new IllegalArgumentException(
                //         "Duplicate arg specified");
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }
            mNiceName = arg.Substring(arg.IndexOf('=') + 1);
        }
        else if (arg.Equals("--mount-external-multiuser")) {
            mMountExternal = IZygote::MOUNT_EXTERNAL_MULTIUSER;
        }
        else if (arg.Equals("--mount-external-multiuser-all")) {
            mMountExternal = IZygote::MOUNT_EXTERNAL_MULTIUSER_ALL;
        }
        else {
            break;
        }
    }

    // if (runtimeInit && classpath != null) {
    //     throw new IllegalArgumentException(
    //             "--runtime-init and -classpath are incompatible");
    // }

    mRemainingArgs = ArrayOf<String>::Alloc(args.GetLength() - curArg);

    for (Int32 i = 0; i < mRemainingArgs->GetLength(); ++i) {
        (*mRemainingArgs)[i] = args[curArg + i];
    }
    return NOERROR;
}

ZygoteConnection::ZygoteConnection(
    /* [in] */ ILocalSocket* socket)
    : mSocket(socket)
{
    AutoPtr<IOutputStream> out;
    socket->GetOutputStream((IOutputStream**)&out);
    CDataOutputStream::New(out, (IDataOutputStream**)&mSocketOutStream);

    AutoPtr<IInputStream> in;
    socket->GetInputStream((IInputStream**)&in);
    AutoPtr<IInputStreamReader> reader;
    CInputStreamReader::New(in, (IInputStreamReader**)&reader);
    CBufferedReader::New(reader, 256, (IBufferedReader**)&mSocketReader);

    mSocket->SetSoTimeout(CONNECTION_TIMEOUT_MILLIS);

    // try {
    mSocket->GetPeerCredentials((ICredentials**)&mPeer);
    // } catch (IOException ex) {
    //     Log.e(TAG, "Cannot read peer credentials", ex);
    //     throw ex;
    // }

    AutoPtr<IFileDescriptor> fd;
    mSocket->GetFileDescriptor((IFileDescriptor**)&fd);
    mPeerSecurityContext = SELinux::GetPeerContext(fd);
}

ECode ZygoteConnection::GetFileDescriptor(
    /* [out] */ IFileDescriptor** fd)
{
    return mSocket->GetFileDescriptor(fd);
}

Boolean ZygoteConnection::RunOnce(
    /* [out] */ IRunnable** task)
{
    AutoPtr< ArrayOf<String> > args;
    AutoPtr<Arguments> parsedArgs;
    AutoPtr< ArrayOf<IFileDescriptor*> > descriptors;

//    try {
    ECode ec = ReadArgumentList((ArrayOf<String>**)&args);
    if (ec == (ECode)E_IO_EXCEPTION) {
        CloseSocket();
        return TRUE;
    }
    mSocket->GetAncillaryFileDescriptors((ArrayOf<IFileDescriptor*>**)&descriptors);
//    } catch (IOException ex) {
//        Log.w(TAG, "IOException on command socket " + ex.getMessage());
//        closeSocket();
//        return true;
//    }

    if (args == NULL) {
        // EOF reached.
        CloseSocket();
        return TRUE;
    }

    /** the stderr of the most recent request, if avail */
    AutoPtr<IPrintStream> newStderr;
//
//    if (descriptors != null && descriptors.length >= 3) {
//        newStderr = new PrintStream(
//                new FileOutputStream(descriptors[2]));
//    }

    Int32 pid = -1;
    AutoPtr<IFileDescriptor> childPipeFd;
    AutoPtr<IFileDescriptor> serverPipeFd;

//    try {
    parsedArgs = new Arguments(*args);

    FAIL_RETURN(ApplyUidSecurityPolicy(parsedArgs, mPeer, mPeerSecurityContext));
    FAIL_RETURN(ApplyRlimitSecurityPolicy(parsedArgs, mPeer, mPeerSecurityContext));
    FAIL_RETURN(ApplyCapabilitiesSecurityPolicy(parsedArgs, mPeer, mPeerSecurityContext));
    FAIL_RETURN(ApplyInvokeWithSecurityPolicy(parsedArgs, mPeer, mPeerSecurityContext));
    FAIL_RETURN(ApplyseInfoSecurityPolicy(parsedArgs, mPeer, mPeerSecurityContext));

    ApplyDebuggerSystemProperty(parsedArgs);
    ApplyInvokeWithSystemProperty(parsedArgs);

    AutoPtr<ArrayOf<Int32*> > rlimits;

//        if (parsedArgs.rlimits != null) {
//            rlimits = parsedArgs.rlimits.toArray(intArray2d);
//        }
//
//        if (parsedArgs.runtimeInit && parsedArgs.invokeWith != null) {
//            FileDescriptor[] pipeFds = Libcore.os.pipe();
//            childPipeFd = pipeFds[1];
//            serverPipeFd = pipeFds[0];
//            ZygoteInit.setCloseOnExec(serverPipeFd, true);
//        }
//
    AutoPtr<IZygote> zygote;
    CZygote::AcquireSingleton((IZygote**)&zygote);
    zygote->ForkAndSpecialize(parsedArgs->mUid, parsedArgs->mGid, parsedArgs->mGids,
            parsedArgs->mDebugFlags, rlimits, parsedArgs->mMountExternal, parsedArgs->mSeInfo,
            parsedArgs->mNiceName, &pid);
//    } catch (IOException ex) {
//        logAndPrintError(newStderr, "Exception creating pipe", ex);
//    } catch (ErrnoException ex) {
//        logAndPrintError(newStderr, "Exception creating pipe", ex);
//    } catch (IllegalArgumentException ex) {
//        logAndPrintError(newStderr, "Invalid zygote arguments", ex);
//    } catch (ZygoteSecurityException ex) {
//        logAndPrintError(newStderr,
//                "Zygote security policy prevents request: ", ex);
//    }

    AutoPtr<IIoUtils> ioUtils;
    CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
//    try {
    if (pid == 0) {
        // in child
        ioUtils->CloseQuietly(serverPipeFd);
        serverPipeFd = NULL;
        HandleChildProc(parsedArgs, descriptors, childPipeFd, newStderr, task);
        return TRUE;
        // should never get here, the child is expected to either
        // throw ZygoteInit.MethodAndArgsCaller or exec().
        ioUtils->CloseQuietly(childPipeFd);
        return TRUE;
    }
    else {
        // in parent...pid of < 0 means failure
        ioUtils->CloseQuietly(childPipeFd);
        childPipeFd = NULL;
        Boolean ret = HandleParentProc(pid, descriptors, serverPipeFd, parsedArgs);
        ioUtils->CloseQuietly(serverPipeFd);
        return ret;
    }
//    } finally {
//        IoUtils.closeQuietly(childPipeFd);
//        IoUtils.closeQuietly(serverPipeFd);
//    }
}

void ZygoteConnection::CloseSocket()
{
    // try {
    mSocket->Close();
    // } catch (IOException ex) {
    //     Log.e(TAG, "Exception while closing command "
    //             + "socket in parent", ex);
    // }
}

ECode ZygoteConnection::ReadArgumentList(
    /* [out] */ ArrayOf<String>** args)
{
    VALIDATE_NOT_NULL(args);
    *args = NULL;

    /**
     * See android.os.Process.zygoteSendArgsAndGetPid()
     * Presently the wire format to the zygote process is:
     * a) a count of arguments (argc, in essence)
     * b) a number of newline-separated argument strings equal to count
     *
     * After the zygote process reads these it will write the pid of
     * the child or -1 on failure.
     */

    Int32 argc;

//    try {
    String s;
    mSocketReader->ReadLine(&s);

    if (s.IsNull()) {
        // EOF reached.
        return NOERROR;
    }
    argc = StringUtils::ParseInt32(s);
//    } catch (NumberFormatException ex) {
//        Log.e(TAG, "invalid Zygote wire format: non-int at argc");
//        throw new IOException("invalid wire format");
//    }

    // See bug 1092107: large argc can be used for a DOS attack
    if (argc > MAX_ZYGOTE_ARGC) {
        // throw new IOException("max arg count exceeded");
        return E_IO_EXCEPTION;
    }

    AutoPtr< ArrayOf<String> > result = ArrayOf<String>::Alloc(argc);
    for (Int32 i = 0; i < argc; i++) {
        String arg;
        mSocketReader->ReadLine(&arg);
        if (arg.IsNull()) {
            // We got an unexpected EOF.
            // throw new IOException("truncated request");
            return E_IO_EXCEPTION;
        }
        (*result)[i] = arg;
    }

    *args = result;
    REFCOUNT_ADD(*args);
    return NOERROR;
}

ECode ZygoteConnection::ApplyUidSecurityPolicy(
    /* [in] */ Arguments* args,
    /* [in] */ ICredentials* peer,
    /* [in] */ const String& peerSecurityContext)
{
//    int peerUid = peer.getUid();
//
//    if (peerUid == 0) {
//        // Root can do what it wants
//    } else if (peerUid == Process.SYSTEM_UID ) {
//        // System UID is restricted, except in factory test mode
//        String factoryTest = SystemProperties.get("ro.factorytest");
//        boolean uidRestricted;
//
//        /* In normal operation, SYSTEM_UID can only specify a restricted
//         * set of UIDs. In factory test mode, SYSTEM_UID may specify any uid.
//         */
//        uidRestricted
//             = !(factoryTest.equals("1") || factoryTest.equals("2"));
//
//        if (uidRestricted
//                && args.uidSpecified && (args.uid < Process.SYSTEM_UID)) {
//            throw new ZygoteSecurityException(
//                    "System UID may not launch process with UID < "
//                            + Process.SYSTEM_UID);
//        }
//    } else {
//        // Everything else
//        if (args.uidSpecified || args.gidSpecified
//            || args.gids != null) {
//            throw new ZygoteSecurityException(
//                    "App UIDs may not specify uid's or gid's");
//        }
//    }
//
//    if (args.uidSpecified || args.gidSpecified || args.gids != null) {
//        boolean allowed = SELinux.checkSELinuxAccess(peerSecurityContext,
//                                                     peerSecurityContext,
//                                                     "zygote",
//                                                     "specifyids");
//        if (!allowed) {
//            throw new ZygoteSecurityException(
//                    "Peer may not specify uid's or gid's");
//        }
//    }
//
//    // If not otherwise specified, uid and gid are inherited from peer
//    if (!args.uidSpecified) {
//        args.uid = peer.getUid();
//        args.uidSpecified = true;
//    }
//    if (!args.gidSpecified) {
//        args.gid = peer.getGid();
//        args.gidSpecified = true;
//    }
    return NOERROR;
}

void ZygoteConnection::ApplyDebuggerSystemProperty(
    /* [in] */ Arguments* args)
{
//    if ("1".equals(SystemProperties.get("ro.debuggable"))) {
//        args.debugFlags |= Zygote.DEBUG_ENABLE_DEBUGGER;
//    }
}

ECode ZygoteConnection::ApplyRlimitSecurityPolicy(
    /* [in] */ Arguments* args,
    /* [in] */ ICredentials* peer,
    /* [in] */ const String& peerSecurityContext)
{
//    int peerUid = peer.getUid();
//
//    if (!(peerUid == 0 || peerUid == Process.SYSTEM_UID)) {
//        // All peers with UID other than root or SYSTEM_UID
//        if (args.rlimits != null) {
//            throw new ZygoteSecurityException(
//                    "This UID may not specify rlimits.");
//        }
//    }
//
//    if (args.rlimits != null) {
//        boolean allowed = SELinux.checkSELinuxAccess(peerSecurityContext,
//                                                     peerSecurityContext,
//                                                     "zygote",
//                                                     "specifyrlimits");
//        if (!allowed) {
//            throw new ZygoteSecurityException(
//                    "Peer may not specify rlimits");
//        }
//     }
    return NOERROR;
}

ECode ZygoteConnection::ApplyCapabilitiesSecurityPolicy(
    /* [in] */ Arguments* args,
    /* [in] */ ICredentials* peer,
    /* [in] */ const String& peerSecurityContext)
{
//    if (args.permittedCapabilities == 0
//            && args.effectiveCapabilities == 0) {
//        // nothing to check
//        return;
//    }
//
//    boolean allowed = SELinux.checkSELinuxAccess(peerSecurityContext,
//                                                 peerSecurityContext,
//                                                 "zygote",
//                                                 "specifycapabilities");
//    if (!allowed) {
//        throw new ZygoteSecurityException(
//                "Peer may not specify capabilities");
//    }
//
//    if (peer.getUid() == 0) {
//        // root may specify anything
//        return;
//    }
//
//    long permittedCaps;
//
//    try {
//        permittedCaps = ZygoteInit.capgetPermitted(peer.getPid());
//    } catch (IOException ex) {
//        throw new ZygoteSecurityException(
//                "Error retrieving peer's capabilities.");
//    }
//
//    /*
//     * Ensure that the client did not specify an effective set larger
//     * than the permitted set. The kernel will enforce this too, but we
//     * do it here to make the following check easier.
//     */
//    if (((~args.permittedCapabilities) & args.effectiveCapabilities) != 0) {
//        throw new ZygoteSecurityException(
//                "Effective capabilities cannot be superset of "
//                        + " permitted capabilities" );
//    }
//
//    /*
//     * Ensure that the new permitted (and thus the new effective) set is
//     * a subset of the peer process's permitted set
//     */
//
//    if (((~permittedCaps) & args.permittedCapabilities) != 0) {
//        throw new ZygoteSecurityException(
//                "Peer specified unpermitted capabilities" );
//    }
    return NOERROR;
}

ECode ZygoteConnection::ApplyInvokeWithSecurityPolicy(
    /* [in] */ Arguments* args,
    /* [in] */ ICredentials* peer,
    /* [in] */ const String& peerSecurityContext)
{
//    int peerUid = peer.getUid();
//
//    if (args.invokeWith != null && peerUid != 0) {
//        throw new ZygoteSecurityException("Peer is not permitted to specify "
//                + "an explicit invoke-with wrapper command");
//    }
//
//    if (args.invokeWith != null) {
//        boolean allowed = SELinux.checkSELinuxAccess(peerSecurityContext,
//                                                     peerSecurityContext,
//                                                     "zygote",
//                                                     "specifyinvokewith");
//        if (!allowed) {
//            throw new ZygoteSecurityException("Peer is not permitted to specify "
//                + "an explicit invoke-with wrapper command");
//        }
//    }
    return NOERROR;
}

ECode ZygoteConnection::ApplyseInfoSecurityPolicy(
    /* [in] */ Arguments* args,
    /* [in] */ ICredentials* peer,
    /* [in] */ const String& peerSecurityContext)
{
//    int peerUid = peer.getUid();
//
//    if (args.seInfo == null) {
//        // nothing to check
//        return;
//    }
//
//    if (!(peerUid == 0 || peerUid == Process.SYSTEM_UID)) {
//        // All peers with UID other than root or SYSTEM_UID
//        throw new ZygoteSecurityException(
//                "This UID may not specify SEAndroid info.");
//    }
//
//    boolean allowed = SELinux.checkSELinuxAccess(peerSecurityContext,
//                                                 peerSecurityContext,
//                                                 "zygote",
//                                                 "specifyseinfo");
//    if (!allowed) {
//        throw new ZygoteSecurityException(
//                "Peer may not specify SEAndroid info");
//    }
//
//    return;
    return NOERROR;
}

void ZygoteConnection::ApplyInvokeWithSystemProperty(
    /* [in] */ Arguments* args)
{
//    if (args.invokeWith == null && args.niceName != null) {
//        if (args.niceName != null) {
//            String property = "wrap." + args.niceName;
//            if (property.length() > 31) {
//                property = property.substring(0, 31);
//            }
//            args.invokeWith = SystemProperties.get(property);
//            if (args.invokeWith != null && args.invokeWith.length() == 0) {
//                args.invokeWith = null;
//            }
//        }
//    }
}

ECode ZygoteConnection::HandleChildProc(
    /* [in] */ Arguments* parsedArgs,
    /* [in] */ ArrayOf<IFileDescriptor*>* descriptors,
    /* [in] */ IFileDescriptor* pipeFd,
    /* [in] */ IPrintStream* newStderr,
    /* [out] */ IRunnable** task)
{
    /*
     * Close the socket, unless we're in "peer wait" mode, in which
     * case it's used to track the liveness of this process.
     */

    if (parsedArgs->mPeerWait) {
        // try {
        AutoPtr<IFileDescriptor> fd;
        mSocket->GetFileDescriptor((IFileDescriptor**)&fd);
        CZygoteInit::SetCloseOnExec(fd, TRUE);
        sPeerWaitSocket = mSocket;
        // } catch (IOException ex) {
        //     Log.e(TAG, "Zygote Child: error setting peer wait "
        //             + "socket to be close-on-exec", ex);
        // }
    }
    else {
        CloseSocket();
        CZygoteInit::CloseServerSocket();
    }

    if (descriptors != NULL) {
        // try {
        CZygoteInit::ReopenStdio((*descriptors)[0],
                (*descriptors)[1], (*descriptors)[2]);

        AutoPtr<IIoUtils> ioUtils;
        CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
        for (Int32 i = 0; i < descriptors->GetLength(); ++i) {
            AutoPtr<IFileDescriptor> fd = (*descriptors)[i];
            ioUtils->CloseQuietly(fd);
        }
//        newStderr = System.err;
        // } catch (IOException ex) {
        //     Log.e(TAG, "Error reopening stdio", ex);
        // }
    }

    if (!parsedArgs->mNiceName.IsNull()) {
        Process::SetArgV0(parsedArgs->mNiceName);
    }

    if (parsedArgs->mRuntimeInit) {
        if (!parsedArgs->mInvokeWith.IsNull()) {
            assert(0);
//            WrapperInit.execApplication(parsedArgs.invokeWith,
//                    parsedArgs.niceName, parsedArgs.targetSdkVersion,
//                    pipeFd, parsedArgs.remainingArgs);
        }
        else {
            RuntimeInit::ZygoteInit(parsedArgs->mTargetSdkVersion,
                    parsedArgs->mRemainingArgs, task);
        }
    }
    else {
        assert(0);
//        String className;
//        // try {
//        className = parsedArgs.remainingArgs[0];
//        // } catch (ArrayIndexOutOfBoundsException ex) {
//        //     logAndPrintError(newStderr,
//        //             "Missing required class name argument", null);
//        //     return;
//        // }
//
//        String[] mainArgs = new String[parsedArgs.remainingArgs.length - 1];
//        System.arraycopy(parsedArgs.remainingArgs, 1,
//                mainArgs, 0, mainArgs.length);
//
//        if (parsedArgs.invokeWith != null) {
//            WrapperInit.execStandalone(parsedArgs.invokeWith,
//                    parsedArgs.classpath, className, mainArgs);
//        }
//        else {
//            ClassLoader cloader;
//            if (parsedArgs.classpath != null) {
//                cloader = new PathClassLoader(parsedArgs.classpath,
//                        ClassLoader.getSystemClassLoader());
//            }
//            else {
//                cloader = ClassLoader.getSystemClassLoader();
//            }
//
//            try {
//                ZygoteInit.invokeStaticMain(cloader, className, mainArgs);
//            } catch (RuntimeException ex) {
//                logAndPrintError(newStderr, "Error starting.", ex);
//            }
//        }
    }
    return NOERROR;
}

Boolean ZygoteConnection::HandleParentProc(
    /* [in] */ Int32 pid,
    /* [in] */ ArrayOf<IFileDescriptor*>* descriptors,
    /* [in] */ IFileDescriptor* pipeFd,
    /* [in] */ Arguments* parsedArgs)
{
    if (pid > 0) {
        SetChildPgid(pid);
    }

    if (descriptors != NULL) {
        AutoPtr<IIoUtils> ioUtils;
        CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
        for (Int32 i = 0; i < descriptors->GetLength(); ++i) {
            AutoPtr<IFileDescriptor> fd = (*descriptors)[i];
            ioUtils->CloseQuietly(fd);
        }
    }

    Boolean usingWrapper = FALSE;
    if (pipeFd != NULL && pid > 0) {
        AutoPtr<IFileInputStream> fis;
        CFileInputStream::New(pipeFd, (IFileInputStream**)&fis);
        AutoPtr<IDataInputStream> is;
        CDataInputStream::New((IInputStream*)fis.Get(), (IDataInputStream**)&is);
        Int32 innerPid = -1;
        // try {
        IDataInput::Probe(is)->ReadInt32(&innerPid);
        // } catch (IOException ex) {
        //     Log.w(TAG, "Error reading pid from wrapped process, child may have died", ex);
        // } finally {
        //     try {
        is->Close();
        //     } catch (IOException ex) {
        //     }
        // }

        // Ensure that the pid reported by the wrapped process is either the
        // child process that we forked, or a descendant of it.
        if (innerPid > 0) {
            Int32 parentPid = innerPid;
            while (parentPid > 0 && parentPid != pid) {
                parentPid = Process::GetParentPid(parentPid);
            }
            if (parentPid > 0) {
                // Log.i(TAG, "Wrapped process has pid " + innerPid);
                pid = innerPid;
                usingWrapper = TRUE;
            }
            else {
                // Log.w(TAG, "Wrapped process reported a pid that is not a child of "
                //         + "the process that we forked: childPid=" + pid
                //         + " innerPid=" + innerPid);
            }
        }
    }

    // try {
    IDataOutput::Probe(mSocketOutStream)->WriteInt32(pid);
    IDataOutput::Probe(mSocketOutStream)->WriteBoolean(usingWrapper);
    // } catch (IOException ex) {
    //     Log.e(TAG, "Error reading from command socket", ex);
    //     return true;
    // }

    /*
     * If the peer wants to use the socket to wait on the
     * newly spawned process, then we're all done.
     */
    if (parsedArgs->mPeerWait) {
        // try {
        mSocket->Close();
        // } catch (IOException ex) {
        //     Log.e(TAG, "Zygote: error closing sockets", ex);
        // }
        return TRUE;
    }
    return FALSE;
}

void ZygoteConnection::SetChildPgid(
    /* [in] */ Int32 pid)
{
    // Try to move the new child into the peer's process group.
//    try {
    Int32 ppid;
    mPeer->GetPid(&ppid);
    Int32 pgid;
    CZygoteInit::Getpgid(ppid, &pgid);
    CZygoteInit::Setpgid(pid, pgid);
//    } catch (IOException ex) {
//        // This exception is expected in the case where
//        // the peer is not in our session
//        // TODO get rid of this log message in the case where
//        // getsid(0) != getsid(peer.getPid())
//        Log.i(TAG, "Zygote: setpgid failed. This is "
//            + "normal if peer is not in our session");
//    }
}

} // namespace Os
} // namespace Internal
} // namespace Droid
} // namespace Elastos
