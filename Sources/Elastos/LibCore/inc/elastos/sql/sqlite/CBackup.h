
#ifndef __CBACKUP_H__
#define __CBACKUP_H__

#include "_Elastos_Sql_SQLite_CBackup.h"
#include <elastos/core/Object.h>



namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CBackup)
    , public Object
    , public IBackup
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

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
    CARAPI _Step(
        /* [in] */ Int32 n,
        /* [out] */ Boolean* state);

    CARAPI_(Int32) _Remaining();

    CARAPI_(Int32) _Pagecount();

    static CARAPI_(Boolean) Internal_init();

private:
    static Boolean isinit;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CBACKUP_H__
