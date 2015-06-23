
#include "am/TransferPipe.h"
#include "os/SystemClock.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::Core::StringBuilder;
using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::EIID_IBinder;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

const String TransferPipe::TAG("TransferPipe");
const Boolean TransferPipe::DEBUG;
const Int64 TransferPipe::DEFAULT_TIMEOUT;

TransferPipe::TransferPipe()
{
    CThread::New((IRunnable*)this->Probe(EIID_IRunnable), String("TransferPipe"), (IThread**)&mThread);
    AutoPtr<IParcelFileDescriptorHelper> helper;
    CParcelFileDescriptorHelper::AcquireSingleton((IParcelFileDescriptorHelper**)&helper);
    helper->CreatePipe((ArrayOf<IParcelFileDescriptor*>**)&mFds);
}

CAR_INTERFACE_IMPL(TransferPipe, IRunnable)

AutoPtr<IParcelFileDescriptor> TransferPipe::GetReadFd()
{
    return (*mFds)[0];
}

AutoPtr<IParcelFileDescriptor> TransferPipe::GetWriteFd()
{
    return (*mFds)[1];
}

ECode TransferPipe::SetBufferPrefix(
    /* [in] */ const String& prefix)
{
    mBufferPrefix = prefix;
    return NOERROR;
}

ECode TransferPipe::Go(
    /* [in] */ ICaller* caller,
    /* [in] */ IInterface* iface,
    /* [in] */ IFileDescriptor* out,
    /* [in] */ const String& prefix,
    /* [in] */ ArrayOf<String>* args)
{
    return Go(caller, iface, out, prefix, args, DEFAULT_TIMEOUT);
}

ECode TransferPipe::Go(
    /* [in] */ ICaller* caller,
    /* [in] */ IInterface* iface,
    /* [in] */ IFileDescriptor* out,
    /* [in] */ const String& prefix,
    /* [in] */ ArrayOf<String>* args,
    /* [in] */ Int64 timeout)
{
    if (IBinder::Probe(iface) != NULL) {
        // This is a local object...  just call it directly.
        // try {
            caller->Go(iface, out, prefix, args);
        // } catch (RemoteException e) {
        // }
        return NOERROR;
    }

    AutoPtr<TransferPipe> tp = new TransferPipe();
    // try {
    AutoPtr<IFileDescriptor> fDes;
    tp->GetWriteFd()->GetFileDescriptor((IFileDescriptor**)&fDes);
    caller->Go(iface, fDes, prefix, args);
    tp->Go(out, timeout);
    // } finally {
    tp->Kill();
    // }
    return NOERROR;
}

ECode TransferPipe::GoDump(
    /* [in] */ IBinder* binder,
    /* [in] */ IFileDescriptor* out,
    /* [in] */ ArrayOf<String>* args)
{
    return GoDump(binder, out, args, DEFAULT_TIMEOUT);
}

ECode TransferPipe::GoDump(
    /* [in] */ IBinder* binder,
    /* [in] */ IFileDescriptor* out,
    /* [in] */ ArrayOf<String>* args,
    /* [in] */ Int64 timeout)
{
    if (binder != NULL) {
        // This is a local object...  just call it directly.
        // try {
    // TODO:
    // binder->Dump(out, args);
        // } catch (RemoteException e) {
        // }
        return NOERROR;
    }

    AutoPtr<TransferPipe> tp = new TransferPipe();
    // try {
        // TODO:
        // binder->DumpAsync(tp.getWriteFd().getFileDescriptor(), args);
    tp->Go(out, timeout);
    // } finally {
    tp->Kill();
    // }
    return NOERROR;
}

ECode TransferPipe::Go(
    /* [in] */ IFileDescriptor* out)
{
    return Go(out, DEFAULT_TIMEOUT);
}

ECode TransferPipe::Go(
    /* [in] */ IFileDescriptor* out,
    /* [in] */ Int64 timeout)
{
    // try {
    {
        Object::Autolock lock(mLock);
        mOutFd = out;
        mEndTime = SystemClock::GetUptimeMillis() + timeout;

        if (DEBUG) Slogger::I(TAG, "read=%p write=%p out=%p", GetReadFd().Get(), GetWriteFd().Get(),
                out);

        // Close the write fd, so we know when the other side is done.
        CloseFd(1);

        mThread->Start();

        while (mFailure.IsNull() && !mComplete) {
            Int64 waitTime = mEndTime - SystemClock::GetUptimeMillis();
            if (waitTime <= 0) {
                if (DEBUG) Slogger::I(TAG, "TIMEOUT!");
                mThread->Interrupt();
                // throw new IOException("Timeout");
                Kill();
                return E_IO_EXCEPTION;
            }

            // try {
            mLock.Wait(waitTime);
            // } catch (InterruptedException e) {
            // }
        }

        if (DEBUG) Slogger::I(TAG, "Finished: %s", mFailure.string());
        if (!mFailure.IsNull()) {
            // throw new IOException(mFailure);
            Kill();
            return E_IO_EXCEPTION;
        }
    }
    // } finally {
    Kill();
    // }
    return NOERROR;
}

ECode TransferPipe::CloseFd(
    /* [in] */ Int32 num)
{
    if ((*mFds)[num] != NULL) {
        if (DEBUG) Slogger::I(TAG,  "Closing: %p", (*mFds)[num]);
        // try {
        (*mFds)[num]->Close();
        // } catch (IOException e) {
        // }
        mFds->Set(num, NULL);
    }
    return NOERROR;
}

ECode TransferPipe::Kill()
{
    CloseFd(0);
    CloseFd(1);
    return NOERROR;
}

ECode TransferPipe::Run()
{
    AutoPtr<ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(1024);
    AutoPtr<IFileDescriptor> fDes;
    GetReadFd()->GetFileDescriptor((IFileDescriptor**)&fDes);
    AutoPtr<IFileInputStream> fis;
    CFileInputStream::New(fDes, (IFileInputStream**)&fis);
    AutoPtr<IFileOutputStream> fos;
    CFileOutputStream::New(mOutFd, (IFileOutputStream**)&fos);

    if (DEBUG) Slogger::I(TAG, "Ready to read pipe...");
    AutoPtr<ArrayOf<Byte> > bufferPrefix;
    Boolean needPrefix = TRUE;
    if (!mBufferPrefix.IsNull()) {
        bufferPrefix = mBufferPrefix.GetBytes();
    }

    Int32 size;
    // try {
        while (fis->ReadBytes(buffer, &size), size > 0) {
            if (DEBUG) Slogger::I(TAG, "Got %d bytes", size);
            if (bufferPrefix == NULL) {
                fos->WriteBytes(*buffer, 0, size);
            }
            else {
                Int32 start = 0;
                for (Int32 i = 0; i < size; i++) {
                    if ((*buffer)[i] != '\n') {
                        if (i > start) {
                            fos->WriteBytes(*buffer, start, i-start);
                        }
                        start = i;
                        if (needPrefix) {
                            fos->WriteBytes(*bufferPrefix);
                            needPrefix = FALSE;
                        }
                        do {
                            i++;
                        } while (i < size && (*buffer)[i] != '\n');
                        if (i < size) {
                            needPrefix = TRUE;
                        }
                    }
                }
                if (size > start) {
                    fos->WriteBytes(*buffer, start, size-start);
                }
            }
        }
        if (DEBUG) Slogger::I(TAG, "End of pipe: size=%d", size);
        Boolean interrupted;
        mThread->IsInterrupted(&interrupted);
        if (interrupted) {
            if (DEBUG) Slogger::I(TAG, "Interrupted!");
        }
    // } catch (IOException e) {
    //     synchronized (this) {
    //         mFailure = e.toString();
    //         notifyAll();
    //         return;
    //     }
    // }

    Object::Autolock lock(mLock);
    mComplete = TRUE;
    mLock.NotifyAll();

    return NOERROR;
}

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos
