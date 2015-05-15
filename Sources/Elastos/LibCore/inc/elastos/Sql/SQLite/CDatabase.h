
#ifndef __CDATABASE_H__
#define __CDATABASE_H__

#include "_CDatabase.h"
#include "Database.h"

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CDatabase) , public Database
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Open(
        /* [in] */ const String& filename,
        /* [in] */ Int32 mode);

    CARAPI OpenEx(
        /* [in] */ const String& filename,
        /* [in] */ Int32 mode,
        /* [in] */ const String& vfs);

    CARAPI OpenEx2(
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
        /* [in] */ ICallback * cb);

    CARAPI ExecEx(
        /* [in] */ const String& sql,
        /* [in] */ ICallback * cb,
        /* [in] */ const ArrayOf<String> & args);

    CARAPI LastInsertRowid(
        /* [out] */ Int64 * id);

    CARAPI Interrupt();

    CARAPI Changes(
        /* [out] */ Int64 * number);

    CARAPI BusyHandler(
        /* [in] */ IBusyHandler * bh);

    CARAPI BusyTimeout(
        /* [in] */ Int32 ms);

    CARAPI GetTable(
        /* [in] */ const String& sql,
        /* [in] */ Int32 maxrows,
        /* [out] */ ITableResult ** tableresult);

    CARAPI GetTableEx(
        /* [in] */ const String& sql,
        /* [out] */ ITableResult ** tableresult);

    CARAPI GetTableEx2(
        /* [in] */ const String& sql,
        /* [in] */ Int32 maxrows,
        /* [in] */ const ArrayOf<String> & args,
        /* [out] */ ITableResult ** tableresult);

    CARAPI GetTableEx3(
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<String> & args,
        /* [out] */ ITableResult ** tableresult);

    CARAPI GetTableEx4(
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<String> & args,
        /* [in] */ ITableResult * tbl);

    CARAPI Dbversion(
        /* [out] */ String * ver);

    CARAPI CreateFunction(
        /* [in] */ const String& name,
        /* [in] */ Int32 nargs,
        /* [in] */ IFunction * f);

    CARAPI CreateAggregate(
        /* [in] */ const String& name,
        /* [in] */ Int32 nargs,
        /* [in] */ IFunction * f);

    CARAPI FunctionType(
        /* [in] */ const String& name,
        /* [in] */ Int32 type);

    CARAPI LastError(
        /* [out] */ Int32 * err);

    CARAPI SetLastError(
        /* [in] */ Int32 error_code);

    CARAPI ErrorMessage(
        /* [out] */ String * str);

    CARAPI SetEncoding(
        /* [in] */ const String& enc);

    CARAPI SetAuthorizer(
        /* [in] */ IAuthorizer * auth);

    CARAPI Trace(
        /* [in] */ ITrace * tr);

    CARAPI Backup(
        /* [in] */ IDatabase * dest,
        /* [in] */ const String& destName,
        /* [in] */ const String& srcName,
        /* [out] */ IBackup ** backup);

    CARAPI Profile(
        /* [in] */ IProfile * pr);

    CARAPI DbStatus(
        /* [in] */ Int32 op,
        /* [in] */ const ArrayOf<Int32> & info,
        /* [in] */ Boolean flag,
        /* [out] */ Int32 * code);

    CARAPI Compile(
        /* [in] */ const String& sql,
        /* [out] */ IVm ** ivm);

    CARAPI CompileEx(
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<String>& args,
        /* [out] */ IVm ** ivm);

    CARAPI Prepare(
        /* [in] */ const String& sql,
        /* [out] */ IStmt ** st);

    CARAPI OpenBlob(
        /* [in] */ const String& db,
        /* [in] */ const String& table,
        /* [in] */ const String& column,
        /* [in] */ Int64 row,
        /* [in] */ Boolean rw,
        /* [out] */ IBlob ** blob);

    CARAPI Is3(
        /* [out] */ Boolean * value);

    CARAPI ProgressHandler(
        /* [in] */ Int32 n,
        /* [in] */ IProgressHandler * p);

    CARAPI Key(
        /* [in] */ const ArrayOf<Byte> & ekey);

    CARAPI KeyEx(
        /* [in] */ const String& skey);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CDATABASE_H__
