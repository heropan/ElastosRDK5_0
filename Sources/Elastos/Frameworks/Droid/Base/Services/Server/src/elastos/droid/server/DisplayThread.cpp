#include "elastos/droid/server/DisplayThread.h"
#include <elastos/core/AutoLock.h>

using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::CHandler;

namespace Elastos {
namespace Droid {
namespace Server {

Object DisplayThread::sLock;
AutoPtr<DisplayThread> DisplayThread::sInstance;
AutoPtr<IHandler> DisplayThread::sHandler;

DisplayThread::DisplayThread()
    : ServiceThread(String("android.display"), IProcess::THREAD_PRIORITY_DISPLAY, FALSE /*allowIo*/)
{
}

void DisplayThread::EnsureThreadLocked()
{
    if (sInstance == NULL) {
        sInstance = new DisplayThread();
        sInstance->Start();
        AutoPtr<ILooper> looper;
        sInstance->GetLooper((ILooper**)&looper);
        CHandler::New((IHandler**)&sHandler);
    }
}

AutoPtr<DisplayThread> DisplayThread::Get()
{
    synchronized(sLock) {
        EnsureThreadLocked();
    }
    return sInstance;
}

AutoPtr<IHandler> DisplayThread::GetHandler()
{
    synchronized(sLock) {
        EnsureThreadLocked();
    }
    return sHandler;
}


} // namespace Server
} // namepsace Droid
} // namespace Elastos
