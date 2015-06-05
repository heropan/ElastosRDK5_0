
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

//===============================================================
//               JavaHandlerThread::InnerRunnable
//===============================================================

JavaHandlerThread::InnerRunnable::InnerRunnable(
    /* [in] */ JavaHandlerThread* owner)
    : mOwner(owner)
{
}

//@Override
ECode JavaHandlerThread::InnerRunnable::Run()
{
    mOwner->NativeInitializeThread(nativeThread, nativeEvent);
    return NOERROR;
}

//===============================================================
//                   JavaHandlerThread
//===============================================================

JavaHandlerThread::JavaHandlerThread(
    /* [in] */ String name)
{
    CHandlerThread::New(name, (IHandlerThread**)&mThread);
}

//@CalledByNative
AutoPtr<JavaHandlerThread> JavaHandlerThread::Create(
    /* [in] */ String name)
{
    AutoPtr<JavaHandlerThread> thread = new JavaHandlerThread(name);
    return thread;
}

//@CalledByNative
void JavaHandlerThread::Start(
    /* [in] */ const Int64 nativeThread,
    /* [in] */ const Int64 nativeEvent)
{
    mThread->Start();
    AutoPtr<ILooper> looper;
    mThread->GetLooper((ILooper**)&looper);
    AutoPtr<IHandler> handler;
    CHandler::New(looper, (IHandler**)&handler);
    AutoPtr<IRunnable> runnable = new InnerRunnable(this);
    handler->Post(runnable);
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