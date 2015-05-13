
#include "CBackup.h"

namespace Elastos {
namespace Sql {
namespace SQLite {

CBackup::CBackup()
{
    mHandle = 0;
    isinit = FALSE;
}

Boolean CBackup::Internal_init()
{
    if (!isinit)
    {
        isinit = TRUE;
    }
    return isinit;
}

Boolean CBackup::isinit = Internal_init();

ECode CBackup::Finish()
{
    Mutex::Autolock lock(mSyncLock);
    return _Finalize();
}

ECode CBackup::Finalize()
{
    Mutex::Autolock lock(mSyncLock);
    return _Finalize();
}

ECode CBackup::Step(
    /* [in] */ Int32 n,
    /* [out] */ Boolean* isCompleted)
{
    Mutex::Autolock lock(mSyncLock);
    *isCompleted = _Step(n);

    return NOERROR;
}

ECode CBackup::Backup()
{
    Mutex::Autolock lock(mSyncLock);
    _Step(-1);

    return NOERROR;
}

ECode CBackup::Remaining(
    /* [out] */ Int32* number)
{
    Mutex::Autolock lock(mSyncLock);
    *number = _Remaining();

    return NOERROR;
}

ECode CBackup::Pagecount(
    /* [out] */ Int32* number)
{
    Mutex::Autolock lock(mSyncLock);
    *number = _Pagecount();

    return NOERROR;
}

ECode CBackup::_Finalize()
{
#if HAVE_SQLITE3 && HAVE_SQLITE3_BACKUPAPI
    hbk *bk = mHandle;
    Int32 ret = SQLITE_OK;
    char32 *err = 0;

    if (bk) {
    if (bk->h) {
        handle *h = bk->h;
        hbk *bkc, **bkp;

        bkp = &h->backups;
        bkc = *bkp;
        while (bkc) {
        if (bkc == bk) {
            *bkp = bkc->next;
            break;
        }
        bkp = &bkc->next;
        bkc = *bkp;
        }
    }
    if (bk->bkup) {
        ret = sqlite3_backup_finish(bk->bkup);
        if (ret != SQLITE_OK && bk->h) {
        err = (char32 *) sqlite3_errmsg((sqlite3 *) bk->h->sqlite);
        }
    }
    bk->bkup = 0;
    free(bk);
    mHandle = 0;
    if (ret != SQLITE_OK) {
        return E_SQL_SQLITE_THROWEX_EXCEPTION;
    }
    }
#endif
    return NOERROR;
}

Boolean CBackup::_Step(
    /* [in] */ Int32 n)
{
    Boolean result = TRUE;
#if HAVE_SQLITE3 && HAVE_SQLITE3_BACKUPAPI
    hbk *bk = mHandle;
    Int32 ret = 0;

    if (bk) {
    if (bk->bkup) {
        ret = sqlite3_backup_step(bk->bkup, (int) n);
        switch (ret) {
            case SQLITE_DONE:
                break;
            case SQLITE_LOCKED:
            case SQLITE_BUSY:
            case SQLITE_OK:
                result = FALSE;
                break;
            default:
                result = FALSE;
                return E_SQL_ILLEGAL_ARGUMENT_EXCEPTION;
                break;
        }
    }
    } else {
        return E_SQL_NULL_POINTER_EXCEPTION;
    }
#else
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
    return result;
}

Int32 CBackup::_Remaining()
{
    Int32 result = 0;
#if HAVE_SQLITE3 && HAVE_SQLITE3_BACKUPAPI
    hbk *bk = mHandle;

    if (bk) {
    if (bk->bkup) {
        result = sqlite3_backup_remaining(bk->bkup);
    }
    }
#else
    return E_SQL_SQLITE_THROWEX_EXCEPTION;
#endif
    return result;
}

Int32 CBackup::_Pagecount()
{
    Int32 result = 0;
#if HAVE_SQLITE3 && HAVE_SQLITE3_BACKUPAPI
    hbk *bk = mHandle;

    if (bk) {
    if (bk->bkup) {
        result = sqlite3_backup_pagecount(bk->bkup);
    }
    }
#else
    return E_SQL_SQLITE_THROWEX_EXCEPTION;
#endif
    return result;
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos
