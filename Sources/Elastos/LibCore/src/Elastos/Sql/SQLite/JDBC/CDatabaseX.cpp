
#include "CDatabaseX.h"

using Elastos::Sql::SQLite::JDBC::EIID_IDatabaseX;

namespace Elastos {
namespace Sql {
namespace SQLite {
namespace JDBC{

DATABASE_METHODS_IMPL(CDatabaseX, Database)

PInterface CDatabaseX::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IDatabaseX) {
        return reinterpret_cast<PInterface>((IDatabaseX*)this);
    }
    return _CDatabaseX::Probe(riid);
}

ECode CDatabaseX::constructor()
{
    return NOERROR;
}

ECode CDatabaseX::ExecEx2(
    /* [in] */ const String& sql,
    /* [in] */ ICallback * cb)
{
    ECode ec = NOERROR;
    ec = Exec(sql, cb);
    lock.Lock();
    lock.NotifyAll();
    lock.Unlock();
    return ec;
}

ECode CDatabaseX::ExecEx3(
    /* [in] */ const String& sql,
    /* [in] */ ICallback * cb,
    /* [in] */ const ArrayOf<String>& args)
{
    ECode ec = NOERROR;
    ec = ExecEx(sql, cb, args);
    lock.Lock();
    lock.NotifyAll();
    lock.Unlock();
    return ec;
}

ECode CDatabaseX::GetTableEx5(
    /* [in] */ const String& sql,
    /* [in] */ const ArrayOf<String>& args,
    /* [out] */ ITableResult ** tbl)
{
    ECode ec = NOERROR;
    ec = GetTableEx3(sql, args, tbl);
    lock.Lock();
    lock.NotifyAll();
    lock.Unlock();
    return ec;
}

ECode CDatabaseX::GetTableEx6(
    /* [in] */ const String& sql,
    /* [in] */ const ArrayOf<String>& args,
    /* [in] */ ITableResult * tbl)
{
    ECode ec = NOERROR;
    ec = GetTableEx4(sql, args, tbl);
    lock.Lock();
    lock.NotifyAll();
    lock.Unlock();
    return ec;
}

void CDatabaseX::Wait(Int32 ms)
{
    lock.Lock();
    lock.Wait(ms);
    lock.Unlock();
}

} // namespace JDBC
} // namespace SQLite
} // namespace Sql
} // namespace Elastos
