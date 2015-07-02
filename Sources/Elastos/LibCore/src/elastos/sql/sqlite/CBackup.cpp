
#include "CBackup.h"
#include "AutoLock.h"

namespace Elastos {
namespace Sql {
namespace SQLite {

CAR_OBJECT_IMPL(CBackup);

CAR_INTERFACE_IMPL(CBackup, Object, IBackup);

CBackup::CBackup()
    : mHandle(0)
{
}

ECode CBackup::Finish()
{
    ECode ec = NOERROR;
    synchronized(this) {
        ec = _Finalize();
    }
    return ec;
}

ECode CBackup::Finalize()
{
    ECode ec = NOERROR;
    synchronized(this) {
        ec = _Finalize();
    }
    return ec;
}

ECode CBackup::Step(
    /* [in] */ Int32 n,
    /* [out] */ Boolean* isCompleted)
{
    ECode ec = NOERROR;
    synchronized(this) {
        ec = _Step(n, isCompleted);
    }

    return ec;
}

ECode CBackup::Backup()
{
    ECode ec = NOERROR;
    synchronized(this) {
        Boolean res = FALSE;
        ec = _Step(-1, &res);
    }

    return ec;
}

ECode CBackup::Remaining(
    /* [out] */ Int32* number)
{
    synchronized(this) {
        *number = _Remaining();
    }

    return NOERROR;
}

ECode CBackup::Pagecount(
    /* [out] */ Int32* number)
{
    synchronized(this) {
        *number = _Pagecount();
    }

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

ECode CBackup::_Step(
    /* [in] */ Int32 n,
    /* [out] */ Boolean* state)
{
    VALIDATE_NOT_NULL(state);
    *state = TRUE;
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
                *state = FALSE;
                break;
            default:
                *state = FALSE;
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
                break;
        }
    }
    } else {
        return E_NULL_POINTER_EXCEPTION;
    }
#else
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
#endif
    return NOERROR;
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
