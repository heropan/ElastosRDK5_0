
#ifndef __TRANSFERPIPE_H__
#define __TRANSFERPIPE_H__

#include "ext/frameworkext.h"
#include "Elastos.Droid.Server_server.h"
#include <elastos/Core/Object.h>

using Elastos::IO::IFileDescriptor;
using Elastos::Core::IRunnable;
using Elastos::Core::Object;
using Elastos::Core::IThread;
using Elastos::Core::CThread;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IParcelFileDescriptor;
using Elastos::Droid::Os::IParcelFileDescriptorHelper;
using Elastos::Droid::Os::CParcelFileDescriptorHelper;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Am {

/**
 * Helper for transferring data through a pipe from a client app.
 */
class TransferPipe
    : public ElRefBase
    , public IRunnable
{
public:
    interface ICaller : public IInterface
    {
        virtual CARAPI Go(
            /* [in] */ IInterface* iface,
            /* [in] */ IFileDescriptor* fd,
            /* [in] */ const String& prefix,
            /* [in] */ ArrayOf<String>* args);
    };

public:
    TransferPipe();

    CAR_INTERFACE_DECL();

    CARAPI Run();

    CARAPI_(AutoPtr<IParcelFileDescriptor>) GetReadFd();

    CARAPI_(AutoPtr<IParcelFileDescriptor>) GetWriteFd();

    CARAPI SetBufferPrefix(
        /* [in] */ const String& prefix);

    static CARAPI Go(
        /* [in] */ ICaller* caller,
        /* [in] */ IInterface* iface,
        /* [in] */ IFileDescriptor* out,
        /* [in] */ const String& prefix,
        /* [in] */ ArrayOf<String>* args);

    static CARAPI Go(
        /* [in] */ ICaller* caller,
        /* [in] */ IInterface* iface,
        /* [in] */ IFileDescriptor* out,
        /* [in] */ const String& prefix,
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ Int64 timeout);

    static CARAPI GoDump(
        /* [in] */ IBinder* binder,
        /* [in] */ IFileDescriptor* out,
        /* [in] */ ArrayOf<String>* args);

    static CARAPI GoDump(
        /* [in] */ IBinder* binder,
        /* [in] */ IFileDescriptor* out,
        /* [in] */ ArrayOf<String>* args,
        /* [in] */ Int64 timeout);

    CARAPI Go(
        /* [in] */ IFileDescriptor* out);

    CARAPI Go(
        /* [in] */ IFileDescriptor* out,
        /* [in] */ Int64 timeout);

    CARAPI CloseFd(
        /* [in] */ Int32 num);

    CARAPI Kill();

public:
    static const String TAG;
    static const Boolean DEBUG = FALSE;

    static const Int64 DEFAULT_TIMEOUT = 5000;  // 5 seconds

    AutoPtr<IThread> mThread;
    AutoPtr<ArrayOf<IParcelFileDescriptor*> > mFds;

    AutoPtr<IFileDescriptor> mOutFd;
    Int64 mEndTime;
    String mFailure;
    Boolean mComplete;

    String mBufferPrefix;

    Object mLock;
};

} // namespace Am
} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__TRANSFERPIPE_H__
