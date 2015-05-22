#ifndef __DATABASE_H__
#define __DATABASE_H__

#include "sqlitejni.h"
#include "Object.h"

using Elastos::Core::Object;

namespace Elastos {
namespace Sql {
namespace SQLite {

class Database
    : public Object
    , public IDatabase
{
public:
    CAR_INTERFACE_DECL();

    Database();

    CARAPI Open(
        /* [in] */ const String& filename,
        /* [in] */ Int32 mode);

    CARAPI Open(
        /* [in] */ const String& filename,
        /* [in] */ Int32 mode,
        /* [in] */ const String& vfs);

    CARAPI Open(
        /* [in] */ const String& filename,
        /* [in] */ Int32 mode,
        /* [in] */ const String& vfs,
        /* [in] */ Boolean ver2);

    CARAPI OpenAuxFile(
        /* [in] */ const String& filename);

    CARAPI Finalize();

    CARAPI Close();

    CARAPI Exec(
        /* [in] */ const String& sql,
        /* [in] */ AutoPtr<ICallback> cb);

    CARAPI Exec(
        /* [in] */ const String& sql,
        /* [in] */ AutoPtr<ICallback> cb,
        /* [in] */ const ArrayOf<String> & argsstr);

    CARAPI_(Int64) LastInsertRowid();

    CARAPI Interrupt();

    CARAPI_(Int64) Changes();

    CARAPI BusyHandler(
        /* [in] */ AutoPtr<IBusyHandler> bh);

    CARAPI BusyTimeout(
        /* [in] */ Int32 ms);

    CARAPI_(AutoPtr<ITableResult>) GetTable(
        /* [in] */ const String& sql,
        /* [in] */ Int32 maxrows);

    CARAPI_(AutoPtr<ITableResult>) GetTable(
        /* [in] */ const String& sql);

    CARAPI_(AutoPtr<ITableResult>) GetTable(
        /* [in] */ const String& sql,
        /* [in] */ Int32 maxrows,
        /* [in] */ const ArrayOf<String> & args);

    CARAPI_(AutoPtr<ITableResult>) GetTable(
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<String> & args);

    CARAPI GetTable(
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<String> & args,
        /* [in] */ AutoPtr<ITableResult> tbl);

    CARAPI_(String) Dbversion();

    CARAPI CreateFunction(
        /* [in] */ const String& name,
        /* [in] */ Int32 nargs,
        /* [in] */ AutoPtr<IFunction> f);

    CARAPI CreateAggregate(
        /* [in] */ const String& name,
        /* [in] */ Int32 nargs,
        /* [in] */ AutoPtr<IFunction> f);

    CARAPI FunctionType(
        /* [in] */ const String& name,
        /* [in] */ Int32 type);

    CARAPI_(Int32) LastError();

    CARAPI SetLastError(
        /* [in] */ Int32 error_code);

    CARAPI_(String) ErrorMessage();

    CARAPI SetEncoding(
        /* [in] */ const String& enc);

    CARAPI SetAuthorizer(
        /* [in] */ AutoPtr<IAuthorizer> auth);

    CARAPI Trace(
        /* [in] */ AutoPtr<ITrace> tr);

    CARAPI_(AutoPtr<IBackup>) Backup(
        /* [in] */ AutoPtr<IDatabase> dest,
        /* [in] */ const String& destName,
        /* [in] */ const String& srcName);

    CARAPI Profile(
        /* [in] */ AutoPtr<IProfile> pr);

    CARAPI_(Int32) DbStatus(
        /* [in] */ Int32 op,
        /* [in] */ const ArrayOf<Int32> & info,
        /* [in] */ Boolean flag);

    CARAPI_(AutoPtr<IVm>) Compile(
        /* [in] */ const String& sql);

    CARAPI_(AutoPtr<IVm>) Compile(
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<String>& args);

    CARAPI_(AutoPtr<IStmt>) Prepare(
        /* [in] */ const String& sql);

    CARAPI_(AutoPtr<IBlob>) OpenBlob(
        /* [in] */ const String& db,
        /* [in] */ const String& table,
        /* [in] */ const String& column,
        /* [in] */ Int64 row,
        /* [in] */ Boolean rw);

    CARAPI_(Boolean) Is3();

    CARAPI ProgressHandler(
        /* [in] */ Int32 n,
        /* [in] */ AutoPtr<IProgressHandler> p);

    CARAPI Key(
        /* [in] */ const ArrayOf<Byte> & ekey);

    CARAPI Key(
        /* [in] */ const String& skey);

    static CARAPI_(Boolean) Complete(
        /* [in] */ const String& sql);

    static CARAPI_(String) Version();

    static CARAPI_(String) ErrorString(
        /* [in] */ Int32 error_code);

    static CARAPI_(Int32) Status(
        /* [in] */ Int32 op,
        /* [in] */ ArrayOf<Int32>* info,
        /* [in] */ Boolean flag);

    static CARAPI_(Int64) LongFromJulian(
        /* [in] */ Double d);

    static CARAPI_(Int64) LongFromJulian(
        /* [in] */ const String& s);

    static CARAPI_(Double) JulianFromLong(
        /* [in] */ Int64 ms);

private:
    CARAPI _Open(
        /* [in] */ const String& filename,
        /* [in] */ Int32 mode);

    CARAPI _Open4(
        /* [in] */ const String& filename,
        /* [in] */ Int32 mode,
        /* [in] */ const String& vfs,
        /* [in] */ Boolean ver2);

    CARAPI _OpenAuxFile(
        /* [in] */ const String& filename);

    CARAPI _Finalize();

    CARAPI _Close();

    CARAPI _Exec(
        /* [in] */ const String& sql,
        /* [in] */ AutoPtr<ICallback> cb);

    CARAPI _Exec(
        /* [in] */ const String& sql,
        /* [in] */ AutoPtr<ICallback> cb,
        /* [in] */ const ArrayOf<String>& args);

    CARAPI_(Int64) _LastInsertRowid();

    CARAPI _Interrupt();

    CARAPI_(Int64) _Changes();

    CARAPI _BusyHandler(
        /* [in] */ AutoPtr<IBusyHandler> bh);

    CARAPI _BusyTimeout(
        /* [in] */ Int32 ms);

    CARAPI _CreateFunction(
        /* [in] */ const String& name,
        /* [in] */ Int32 nargs,
        /* [in] */ AutoPtr<IFunction> f);

    CARAPI _CreateAggregate(
        /* [in] */ const String& name,
        /* [in] */ Int32 nargs,
        /* [in] */ AutoPtr<IFunction> f);

    CARAPI _FunctionType(
        /* [in] */ const String& name,
        /* [in] */ Int32 type);

    CARAPI_(String) _Errmsg();

    CARAPI _SetEncoding(
        /* [in] */ const String& enc);

    CARAPI _SetAuthorizer(
        /* [in] */ AutoPtr<IAuthorizer> auth);

    CARAPI _Trace(
        /* [in] */ AutoPtr<ITrace> tr);

    CARAPI _Profile(
        /* [in] */ AutoPtr<IProfile> pr);

    CARAPI_(Int32) _DbStatus(
        /* [in] */ Int32 op,
        /* [in] */ AutoPtr<ArrayOf<Int32> > info,
        /* [in] */ Boolean flag);

    CARAPI Vm_compile(
        /* [in] */ const String& sql,
        /* [in] */ AutoPtr<IVm> vm);

    CARAPI Vm_compile_args(
        /* [in] */ const String& sql,
        /* [in] */ AutoPtr<IVm> vm,
        /* [in] */ const ArrayOf<String>& argsstr);

    CARAPI Stmt_prepare(
        /* [in] */ const String& sql,
        /* [in] */ AutoPtr<IStmt> stmt);

    CARAPI _OpenBlob(
        /* [in] */ const String& db,
        /* [in] */ const String& table,
        /* [in] */ const String& column,
        /* [in] */ Int64 row,
        /* [in] */ Boolean rw,
        /* [in] */ AutoPtr<IBlob> blob);

    CARAPI _ProgressHandler(
        /* [in] */ Int32 n,
        /* [in] */ AutoPtr<IProgressHandler> ph);

    CARAPI _Key(
        /* [in] */ const ArrayOf<Byte>& ekey);

    CARAPI _Rekey(
        /* [in] */ ArrayOf<Byte> * ekey);

    CARAPI doclose(
        /* [in] */ Int32 final);

    static CARAPI_(Boolean) _Complete(
        /* [in] */ const String& sql);

    static CARAPI _Backup(
        /* [in] */ AutoPtr<IBackup> b,
        /* [in] */ AutoPtr<IDatabase> dest,
        /* [in] */ const String& destName,
        /* [in] */ AutoPtr<IDatabase> src,
        /* [in] */ const String& srcName);

    static CARAPI_(Int32) _Status(
        /* [in] */ Int32 op,
        /* [in] */ ArrayOf<Int32>* info,
        /* [in] */ Boolean flag);

    static CARAPI_(Boolean) _Enable_shared_cache(
        /* [in] */ Boolean onoff);

    static CARAPI_(void) Internal_init();

private:
    Int64 mHandle;
    Int32 mError_code;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif //__DATABASE_H__
