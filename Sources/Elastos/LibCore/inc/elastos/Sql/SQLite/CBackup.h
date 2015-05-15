
#ifndef __CBACKUP_H__
#define __CBACKUP_H__

#include "_CBackup.h"
#include <elastos/Mutex.h>

using Elastos::Core::Threading::Mutex;

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CBackup)
{
public:
    CARAPI Finish();

    CARAPI Finalize();

    CARAPI _Finalize();

    CARAPI Step(
        /* [in] */ Int32 n,
        /* [out] */ Boolean* isCompleted);

    CARAPI Backup();

    CARAPI Remaining(
        /* [out] */ Int32* number);

    CARAPI Pagecount(
        /* [out] */ Int32* number);

    CBackup();

protected:
    Int64 mHandle;

private:
    CARAPI_(Boolean) _Step(
        /* [in] */ Int32 n);

    CARAPI_(Int32) _Remaining();

    CARAPI_(Int32) _Pagecount();

    static CARAPI_(Boolean) Internal_init();

private:
    static Boolean isinit;
    Mutex mSyncLock;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CBACKUP_H__
