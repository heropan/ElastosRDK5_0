
#include "coredef.h"
#include "CDatabase.h"

using Elastos::Sql::SQLite::EIID_IDatabase;

namespace Elastos {
namespace Sql {
namespace SQLite {

CAR_OBJECT_IMPL(CDatabase)

PInterface CDatabase::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IDatabase) {
        return reinterpret_cast<PInterface>((IDatabase*)this);
    }
    return _CDatabase::Probe(riid);
}

ECode CDatabase::Open(
    /* [in] */ const String& filename,
    /* [in] */ Int32 mode)
{
    return Database::Open(filename,mode);
}

ECode CDatabase::Open(
    /* [in] */ const String& filename,
    /* [in] */ Int32 mode,
    /* [in] */ const String& vfs)
{
    return Database::Open(filename,mode,vfs);
}

ECode CDatabase::Open(
    /* [in] */ const String& filename,
    /* [in] */ Int32 mode,
    /* [in] */ const String& vfs,
    /* [in] */ Boolean ver2)
{
    return Database::Open(filename,mode,vfs,ver2);
}

ECode CDatabase::OpenAuxFile(
    /* [in] */ const String& filename)
{
    return Database::OpenAuxFile(filename);
}

ECode CDatabase::Finalize()
{
    return Database::Finalize();
}

ECode CDatabase::Close()
{
    return Database::Close();
}

ECode CDatabase::Exec(
    /* [in] */ const String& sql,
    /* [in] */ ICallback * cb)
{
    return Database::Exec(sql,cb);
}

ECode CDatabase::Exec(
    /* [in] */ const String& sql,
    /* [in] */ ICallback * cb,
    /* [in] */ const ArrayOf<String> & args)
{
    return Database::Exec(sql,cb,args);
}

ECode CDatabase::LastInsertRowid(
    /* [out] */ Int64 * id)
{
    *id = Database::LastInsertRowid();
    return NOERROR;
}

ECode CDatabase::Interrupt()
{
    return Database::Interrupt();
}

ECode CDatabase::Changes(
    /* [out] */ Int64 * number)
{
    *number = Database::Changes();
    return NOERROR;
}

ECode CDatabase::BusyHandler(
    /* [in] */ IBusyHandler * bh)
{
    return Database::BusyHandler(bh);
}

ECode CDatabase::BusyTimeout(
    /* [in] */ Int32 ms)
{
    return Database::BusyTimeout(ms);
}

ECode CDatabase::GetTable(
    /* [in] */ const String& sql,
    /* [in] */ Int32 maxrows,
    /* [out] */ ITableResult ** tableresult)
{
    VALIDATE_NOT_NULL(tableresult);
    AutoPtr<ITableResult> r = Database::GetTable(sql,maxrows);
    *tableresult = r;
    INTERFACE_ADDREF(*tableresult);
    return NOERROR;
}

ECode CDatabase::GetTable(
    /* [in] */ const String& sql,
    /* [out] */ ITableResult ** tableresult)
{
    VALIDATE_NOT_NULL(tableresult);
    AutoPtr<ITableResult> r = Database::GetTable(sql);
    *tableresult = r;
    INTERFACE_ADDREF(*tableresult);
    return NOERROR;
}

ECode CDatabase::GetTable(
    /* [in] */ const String& sql,
    /* [in] */ Int32 maxrows,
    /* [in] */ const ArrayOf<String> & args,
    /* [out] */ ITableResult ** tableresult)
{
    VALIDATE_NOT_NULL(tableresult);
    AutoPtr<ITableResult> r = Database::GetTable(sql,maxrows,args);
    *tableresult = r;
    INTERFACE_ADDREF(*tableresult);
    return NOERROR;
}

ECode CDatabase::GetTable(
    /* [in] */ const String& sql,
    /* [in] */ const ArrayOf<String> & args,
    /* [out] */ ITableResult ** tableresult)
{
    VALIDATE_NOT_NULL(tableresult);
    AutoPtr<ITableResult> r = Database::GetTable(sql,args);
    *tableresult = r;
    INTERFACE_ADDREF(*tableresult);
    return NOERROR;
}

ECode CDatabase::GetTable(
    /* [in] */ const String& sql,
    /* [in] */ const ArrayOf<String> & args,
    /* [in] */ ITableResult * tbl)
{
    Database::GetTable(sql,args,tbl);
    return NOERROR;
}

ECode CDatabase::Dbversion(
    /* [out] */ String * ver)
{
    *ver = Database::Dbversion();
    return NOERROR;
}

ECode CDatabase::CreateFunction(
    /* [in] */ const String& name,
    /* [in] */ Int32 nargs,
    /* [in] */ IFunction * f)
{
    return Database::CreateFunction(name,nargs,f);
}

ECode CDatabase::CreateAggregate(
    /* [in] */ const String& name,
    /* [in] */ Int32 nargs,
    /* [in] */ IFunction * f)
{
    return Database::CreateAggregate(name,nargs,f);
}

ECode CDatabase::FunctionType(
    /* [in] */ const String& name,
    /* [in] */ Int32 type)
{
    return Database::FunctionType(name,type);
}

ECode CDatabase::LastError(
    /* [out] */ Int32 * err)
{
    *err = Database::LastError();
    return NOERROR;
}

ECode CDatabase::SetLastError(
    /* [in] */ Int32 error_code)
{
    return Database::SetLastError(error_code);
}

ECode CDatabase::ErrorMessage(
    /* [out] */ String * str)
{
    *str = Database::ErrorMessage();
    return NOERROR;
}

ECode CDatabase::SetEncoding(
    /* [in] */ const String& enc)
{
    return Database::SetEncoding(enc);
}

ECode CDatabase::SetAuthorizer(
    /* [in] */ IAuthorizer * auth)
{
    return Database::SetAuthorizer(auth);
}

ECode CDatabase::Trace(
    /* [in] */ ITrace * tr)
{
    return Database::Trace(tr);
}

ECode CDatabase::Backup(
    /* [in] */ IDatabase * dest,
    /* [in] */ const String& destName,
    /* [in] */ const String& srcName,
    /* [out] */ IBackup ** backup)
{
    AutoPtr<IBackup> r = Database::Backup(dest,destName,srcName);
    VALIDATE_NOT_NULL(backup);
    *backup = r;
    INTERFACE_ADDREF(*backup);
    return NOERROR;
}

ECode CDatabase::Profile(
    /* [in] */ IProfile * r)
{
    return Database::Profile(r);
}

ECode CDatabase::DbStatus(
    /* [in] */ Int32 op,
    /* [in] */ const ArrayOf<Int32> & info,
    /* [in] */ Boolean flag,
    /* [out] */ Int32 * code)
{
    *code = Database::DbStatus(op,info,flag);
    return NOERROR;
}

ECode CDatabase::Compile(
    /* [in] */ const String& sql,
    /* [out] */ IVm ** ivm)
{
    VALIDATE_NOT_NULL(ivm);
    AutoPtr<IVm> r = Database::Compile(sql);
    *ivm = r;
    INTERFACE_ADDREF(*ivm);
    return NOERROR;
}

ECode CDatabase::Compile(
    /* [in] */ const String& sql,
    /* [in] */ const ArrayOf<String> & args,
    /* [out] */ IVm ** ivm)
{
    VALIDATE_NOT_NULL(ivm);
    AutoPtr<IVm> r = Database::Compile(sql,args);
    *ivm = r;
    INTERFACE_ADDREF(*ivm);
    return NOERROR;
}

ECode CDatabase::Prepare(
    /* [in] */ const String& sql,
    /* [out] */ IStmt ** stmt)
{
    AutoPtr<IStmt> r = Database::Prepare(sql);
    if (stmt)
    {
        *stmt = r;
        INTERFACE_ADDREF(*stmt);
    }

    return NOERROR;
}

ECode CDatabase::OpenBlob(
    /* [in] */ const String& db,
    /* [in] */ const String& table,
    /* [in] */ const String& column,
    /* [in] */ Int64 row,
    /* [in] */ Boolean rw,
    /* [out] */ IBlob ** blob)
{
    VALIDATE_NOT_NULL(blob);
    AutoPtr<IBlob> r = Database::OpenBlob(db,table,column,row,rw);
    *blob = r;
    INTERFACE_ADDREF(*blob);
    return NOERROR;
}

ECode CDatabase::Is3(
    /* [out] */ Boolean * value)
{
    *value = Database::Is3();
    return NOERROR;
}

ECode CDatabase::ProgressHandler(
    /* [in] */ Int32 n,
    /* [in] */ IProgressHandler * p)
{
    return Database::ProgressHandler(n,p);
}

ECode CDatabase::Key(
    /* [in] */ const ArrayOf<Byte> & ekey)
{
    return Database::Key(ekey);
}

ECode CDatabase::Key(
    /* [in] */ const String& skey)
{
    return Database::Key(skey);
}

UInt32 CDatabase::Release()
{
    return ElRefBase::Release();
}

UInt32 CDatabase::AddRef()
{
    return ElRefBase::AddRef();
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos
