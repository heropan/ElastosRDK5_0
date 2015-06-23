
#ifndef __CMTPSERVER_H__
#define __CMTPSERVER_H__

#include "_CMtpServer.h"

namespace Elastos {
namespace Droid {
namespace Mtp {

CarClass(CMtpServer)
{
public:
    CMtpServer();

    CARAPI constructor(
        /* [in] */ IMtpDatabase* database,
        /* [in] */ Boolean usePtp);

    CARAPI Start();

    CARAPI Run();

    CARAPI SendObjectAdded(
        /* [in] */ Int32 handle);

    CARAPI SendObjectRemoved(
        /* [in] */ Int32 handle);

    CARAPI AddStorage(
        /* [in] */ IMtpStorage* storage);

    CARAPI RemoveStorage(
        /* [in] */ IMtpStorage* storage);

private:
    CARAPI_(void) NativeSetup(
        /* [in] */ IMtpDatabase* database,
        /* [in] */ Boolean usePtp);

    CARAPI_(void) NativeRun();

    CARAPI_(void) NativeCleanup();

    CARAPI_(void) NativeSendObjectAdded(
        /* [in] */ Int32 handle);

    CARAPI_(void) NativeSendObjectRemoved(
        /* [in] */ Int32 handle);

    CARAPI_(void) NativeAddStorage(
        /* [in] */ IMtpStorage* storage);

    CARAPI_(void) NativeRemoveStorage(
        /* [in] */ Int32 storageId);

private:
    Int32 mNativeContext;

    Mutex mutex;
};

} // namespace Mtp
} // namepsace Droid
} // namespace Elastos

#endif // __CMTPSERVER_H__
