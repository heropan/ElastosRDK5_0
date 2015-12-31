
#include "elastos/droid/server/net/DelayedDiskWrite.h"
#include <elastos/core/AutoLock.h>
#include <elastos/droid/net/ReturnOutValue.h>
#include <elastos/droid/text/TextUtils.h>
#include <elastos/utility/logging/Logger.h>
#include <Elastos.CoreLibrary.IO.h>

using Elastos::Core::IRunnable;
using Elastos::Core::IThread;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::CHandlerThread;
using Elastos::Droid::Text::TextUtils;
using Elastos::IO::CBufferedOutputStream;
using Elastos::IO::CDataOutputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::IBufferedOutputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::IOutputStream;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Net {

//=====================================================================
// DelayedDiskWrite::InnerSub_Runnable
//=====================================================================
DelayedDiskWrite::InnerSub_Runnable::InnerSub_Runnable(
    /* [in] */ DelayedDiskWrite* host,
    /* [in] */ const String& filePath,
    /* [in] */ Writer* w)
    : mHost(host)
    , mFilePath(filePath)
    , mWriter(w)
{}

ECode DelayedDiskWrite::InnerSub_Runnable::Run()
{
    return mHost->DoWrite(mFilePath, mWriter);
}

//=====================================================================
// DelayedDiskWrite
//=====================================================================
DelayedDiskWrite::DelayedDiskWrite()
    : mWriteSequence(0)
    , TAG("DelayedDiskWrite")
{}

ECode DelayedDiskWrite::Write(
    /* [in] */ const String& filePath,
    /* [in] */ Writer* w)
{
    if (TextUtils::IsEmpty(filePath)) {
        Logger::E(TAG, "empty file path");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    /* Do a delayed write to disk on a separate handler thread */
    synchronized(this) {
        if (++mWriteSequence == 1) {
            CHandlerThread::New(String("DelayedDiskWriteThread"), (IHandlerThread**)&mDiskWriteHandlerThread);
            IThread::Probe(mDiskWriteHandlerThread)->Start();
            CHandler::New(Ptr(mDiskWriteHandlerThread)->Func(IHandlerThread::GetLooper), (IHandler**)&mDiskWriteHandler);
        }
    }

    AutoPtr<IRunnable> runnable = new InnerSub_Runnable(this, filePath, w);
    Boolean b;
    mDiskWriteHandler->Post(runnable, &b);
    return NOERROR;
}

ECode DelayedDiskWrite::DoWrite(
    /* [in] */ const String& filePath,
    /* [in] */ Writer* w)
{
    AutoPtr<IDataOutputStream> out;
    ECode ec;
    do {
        AutoPtr<IFileOutputStream> fos;
        if (FAILED(ec = CFileOutputStream::New(filePath, (IFileOutputStream**)&fos))) break;
        AutoPtr<IBufferedOutputStream> bos;
        if (FAILED(ec = CBufferedOutputStream::New(IOutputStream::Probe(fos), (IBufferedOutputStream**)&bos))) break;
        if (FAILED(ec = CDataOutputStream::New(IOutputStream::Probe(bos), (IDataOutputStream**)&out))) break;
        if (FAILED(ec = w->OnWriteCalled(out))) break;
    } while(FALSE);
    if (FAILED(ec)) {
        if ((ECode)E_IO_EXCEPTION == ec)
            Logger::E(TAG, "Error writing data file %s", filePath.string());
        else
            return ec;
    }
    // finally {
    if (out != NULL) {
        // try {
        IOutputStream::Probe(out)->Close();
        // } catch (Exception e) {}
    }
    // }
    // Quit if no more writes sent
    synchronized(this) {
        if (--mWriteSequence == 0) {
            Ptr(mDiskWriteHandler)->Func(IHandler::GetLooper)->Quit();
            mDiskWriteHandler = NULL;
            mDiskWriteHandlerThread = NULL;
        }
    }
    return NOERROR;
}

ECode DelayedDiskWrite::Loge(
    /* [in] */ const String& s)
{
    Logger::E(TAG, s);
    return NOERROR;
}

} // namespace Net
} // namespace Server
} // namespace Droid
} // namespace Elastos
