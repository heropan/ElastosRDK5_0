#include "elastos/droid/server/IoThread.h"
#include <elastos/core/AutoLock.h>

using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::CHandler;

namespace Elastos {
namespace Droid {
namespace Server {

Object IoThread::sLock;
AutoPtr<IoThread> IoThread::sInstance;
AutoPtr<IHandler> IoThread::sHandler;

IoThread::IoThread()
    : ServiceThread(String("android.io"), IProcess::THREAD_PRIORITY_DEFAULT, TRUE /*allowIo*/)
{
}

void IoThread::EnsureThreadLocked()
{
    if (sInstance == NULL) {
        sInstance = new IoThread();
        sInstance->Start();
        AutoPtr<ILooper> looper;
        sInstance->GetLooper((ILooper**)&looper);
        CHandler::New((IHandler**)&sHandler);
    }
}

AutoPtr<IoThread> IoThread::Get()
{
    synchronized(sLock) {
        EnsureThreadLocked();
    }
    return sInstance;
}

AutoPtr<IHandler> IoThread::GetHandler()
{
    synchronized(sLock) {
        EnsureThreadLocked();
    }
    return sHandler;
}


} // namespace Server
} // namepsace Droid
} // namespace Elastos
