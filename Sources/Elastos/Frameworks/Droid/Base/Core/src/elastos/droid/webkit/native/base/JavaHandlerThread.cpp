
#include "elastos/droid/webkit/native/base/JavaHandlerThread.h"
#include "elastos/droid/os/CHandler.h"
#include "elastos/droid/os/CHandlerThread.h"

using Elastos::Core::IThread;
using Elastos::Core::EIID_IThread;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::CHandlerThread;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

//===============================================================
//               JavaHandlerThread::InnerRunnable
//===============================================================

JavaHandlerThread::InnerRunnable::InnerRunnable(
    /* [in] */ JavaHandlerThread* owner,
    /* [in] */ const Int64 nativeThread,
    /* [in] */ const Int64 nativeEvent)
    : mOwner(owner)
    , mNativeThread(nativeThread)
    , mNativeEvent(nativeEvent)
{
}

//@Override
ECode JavaHandlerThread::InnerRunnable::Run()
{
    mOwner->NativeInitializeThread(mNativeThread, mNativeEvent);
    return NOERROR;
}

//===============================================================
//                   JavaHandlerThread
//===============================================================

JavaHandlerThread::JavaHandlerThread(
    /* [in] */ const String& name)
{
    CHandlerThread::New(name, (IHandlerThread**)&mThread);
}

//@CalledByNative
AutoPtr<JavaHandlerThread> JavaHandlerThread::Create(
    /* [in] */ const String& name)
{
    AutoPtr<JavaHandlerThread> thread = new JavaHandlerThread(name);
    return thread;
}

//@CalledByNative
void JavaHandlerThread::Start(
    /* [in] */ const Int64 nativeThread,
    /* [in] */ const Int64 nativeEvent)
{
    AutoPtr<IThread> thread = (IThread*)mThread->Probe(EIID_IThread);
    thread->Start();
    AutoPtr<ILooper> looper;
    mThread->GetLooper((ILooper**)&looper);
    AutoPtr<IHandler> handler;
    CHandler::New(looper, (IHandler**)&handler);
    AutoPtr<IRunnable> runnable = new InnerRunnable(this, nativeThread, nativeEvent);
    Boolean result = FALSE;
    handler->Post(runnable, &result);
}

void JavaHandlerThread::NativeInitializeThread(
    /* [in] */ Int64 nativeJavaHandlerThread,
    /* [in] */ Int64 nativeEvent)
{
}

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos