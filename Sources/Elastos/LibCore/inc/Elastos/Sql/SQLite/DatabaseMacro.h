
#ifndef __DATABASEMACRO_H__
#define __DATABASEMACRO_H__

#define DATABASE_METHODS_DECL()                                   \
                                                                  \
    CARAPI Open(                                                  \
        /* [in] */ const String& filename,                        \
        /* [in] */ Int32 mode);                                   \
                                                                  \
    CARAPI OpenEx(                                                \
        /* [in] */ const String& filename,                        \
        /* [in] */ Int32 mode,                                    \
        /* [in] */ const String& vfs);                            \
                                                                  \
    CARAPI OpenEx2(                                               \
        /* [in] */ const String& filename,                        \
        /* [in] */ Int32 mode,                                    \
        /* [in] */ const String& vfs,                             \
        /* [in] */ Boolean ver2);                                 \
                                                                  \
    CARAPI OpenAuxFile(                                         \
        /* [in] */ const String& filename);                       \
                                                                  \
    CARAPI Finalize();                                            \
                                                                  \
    CARAPI Close();                                               \
                                                                  \
    CARAPI Exec(                                                  \
        /* [in] */ const String& sql,                             \
        /* [in] */ ICallback * cb);                               \
                                                                  \
    CARAPI ExecEx(                                                \
        /* [in] */ const String& sql,                             \
        /* [in] */ ICallback * cb,                                \
        /* [in] */ const ArrayOf<String> & args);                 \
                                                                  \
    CARAPI LastInsertRowid(                                     \
        /* [out] */ Int64 * id);                                  \
                                                                  \
    CARAPI Interrupt();                                           \
                                                                  \
    CARAPI Changes(                                               \
        /* [out] */ Int64 * number);                              \
                                                                  \
    CARAPI BusyHandler(                                          \
        /* [in] */ IBusyHandler * bh);                            \
                                                                  \
    CARAPI BusyTimeout(                                          \
        /* [in] */ Int32 ms);                                     \
                                                                  \
    CARAPI GetTable(                                             \
        /* [in] */ const String& sql,                             \
        /* [in] */ Int32 maxrows,                                 \
        /* [out] */ ITableResult ** tableresult);                 \
                                                                  \
    CARAPI GetTableEx(                                           \
        /* [in] */ const String& sql,                             \
        /* [out] */ ITableResult ** tableresult);                 \
                                                                  \
    CARAPI GetTableEx2(                                          \
        /* [in] */ const String& sql,                             \
        /* [in] */ Int32 maxrows,                                 \
        /* [in] */ const ArrayOf<String> & args,                  \
        /* [out] */ ITableResult ** tableresult);                 \
                                                                  \
    CARAPI GetTableEx3(                                          \
        /* [in] */ const String& sql,                             \
        /* [in] */ const ArrayOf<String> & args,                  \
        /* [out] */ ITableResult ** tableresult);                 \
                                                                  \
    CARAPI GetTableEx4(                                          \
        /* [in] */ const String& sql,                             \
        /* [in] */ const ArrayOf<String> & args,                  \
        /* [in] */ ITableResult * tbl);                           \
                                                                  \
    CARAPI Dbversion(                                             \
        /* [out] */ String * ver);                                \
                                                                  \
    CARAPI CreateFunction(                                       \
        /* [in] */ const String& name,                            \
        /* [in] */ Int32 nargs,                                   \
        /* [in] */ IFunction * f);                                \
                                                                  \
    CARAPI CreateAggregate(                                      \
        /* [in] */ const String& name,                            \
        /* [in] */ Int32 nargs,                                   \
        /* [in] */ IFunction * f);                                \
                                                                  \
    CARAPI FunctionType(                                         \
        /* [in] */ const String& name,                            \
        /* [in] */ Int32 type);                                   \
                                                                  \
    CARAPI LastError(                                            \
        /* [out] */ Int32 * err);                                 \
                                                                  \
    CARAPI SetLastError(                                        \
        /* [in] */ Int32 error_code);                             \
                                                                  \
    CARAPI ErrorMessage(                                         \
        /* [out] */ String * str);                                \
                                                                  \
    CARAPI SetEncoding(                                          \
        /* [in] */ const String& enc);                            \
                                                                  \
    CARAPI SetAuthorizer(                                        \
        /* [in] */ IAuthorizer * auth);                           \
                                                                  \
    CARAPI Trace(                                                 \
        /* [in] */ ITrace * tr);                                  \
                                                                  \
    CARAPI Backup(                                                \
        /* [in] */ IDatabase * dest,                              \
        /* [in] */ const String& destName,                        \
        /* [in] */ const String& srcName,                         \
        /* [out] */ IBackup ** backup);                           \
                                                                  \
    CARAPI Profile(                                               \
        /* [in] */ IProfile * pr);                                \
                                                                  \
    CARAPI DbStatus(                                             \
        /* [in] */ Int32 op,                                      \
        /* [in] */ const ArrayOf<Int32> & info,                   \
        /* [in] */ Boolean flag,                                  \
        /* [out] */ Int32 * code);                                \
                                                                  \
    CARAPI Compile(                                               \
        /* [in] */ const String& sql,                             \
        /* [out] */ IVm ** ivm);                                  \
                                                                  \
    CARAPI CompileEx(                                             \
        /* [in] */ const String& sql,                             \
        /* [in] */ const ArrayOf<String>& args,                   \
        /* [out] */ IVm ** ivm);                                  \
                                                                  \
    CARAPI Prepare(                                               \
        /* [in] */ const String& sql,                             \
        /* [out] */ IStmt ** st);                                 \
                                                                  \
    CARAPI OpenBlob(                                             \
        /* [in] */ const String& db,                              \
        /* [in] */ const String& table,                           \
        /* [in] */ const String& column,                          \
        /* [in] */ Int64 row,                                     \
        /* [in] */ Boolean rw,                                    \
        /* [out] */ IBlob ** blob);                               \
                                                                  \
    CARAPI Is3(                                                   \
        /* [out] */ Boolean * value);                             \
                                                                  \
    CARAPI ProgressHandler(                                      \
        /* [in] */ Int32 n,                                       \
        /* [in] */ IProgressHandler * p);                         \
                                                                  \
    CARAPI Key(                                                   \
        /* [in] */ const ArrayOf<Byte> & ekey);                   \
                                                                  \
    CARAPI KeyEx(                                                 \
        /* [in] */ const String& skey);

#define DATABASE_METHODS_IMPL(className, superClass)              \
                                                                  \
ECode className::Open(                                           \
        /* [in] */ const String& filename,                        \
        /* [in] */ Int32 mode)                                    \
{                                                                 \
    return superClass::Open(filename,mode);                       \
}                                                                 \
                                                                  \
ECode className::OpenEx(                                          \
        /* [in] */ const String& filename,                        \
        /* [in] */ Int32 mode,                                    \
        /* [in] */ const String& vfs)                             \
{                                                                 \
    return superClass::OpenEx(filename,mode,vfs);                 \
}                                                                 \
                                                                  \
ECode className::OpenEx2(                                         \
        /* [in] */ const String& filename,                        \
        /* [in] */ Int32 mode,                                    \
        /* [in] */ const String& vfs,                             \
        /* [in] */ Boolean ver2)                                  \
{                                                                 \
    return superClass::OpenEx2(filename,mode,vfs,ver2);           \
}                                                                 \
                                                                  \
ECode className::OpenAuxFile(                                   \
        /* [in] */ const String& filename)                        \
{                                                                 \
    return superClass::OpenAuxFile(filename);                   \
}                                                                 \
                                                                  \
ECode className::Finalize()                                       \
{                                                                 \
    return superClass::Finalize();                                \
}                                                                 \
                                                                  \
ECode className::Close()                                          \
{                                                                 \
    return superClass::Close();                                   \
}                                                                 \
                                                                  \
ECode className::Exec(                                            \
        /* [in] */ const String& sql,                             \
        /* [in] */ ICallback * cb)                                \
{                                                                 \
    return superClass::Exec(sql,cb);                              \
}                                                                 \
                                                                  \
ECode className::ExecEx(                                          \
        /* [in] */ const String& sql,                             \
        /* [in] */ ICallback * cb,                                \
        /* [in] */ const ArrayOf<String> & args)                  \
{                                                                 \
    return superClass::ExecEx(sql,cb,args);                       \
}                                                                 \
                                                                  \
ECode className::LastInsertRowid(                               \
        /* [out] */ Int64 * id)                                   \
{                                                                 \
    *id = superClass::LastInsertRowid();                        \
    return NOERROR;                                               \
}                                                                 \
                                                                  \
ECode className::Interrupt()                                      \
{                                                                 \
    return superClass::Interrupt();                               \
}                                                                 \
                                                                  \
ECode className::Changes(                                         \
        /* [out] */ Int64 * number)                               \
{                                                                 \
    *number = superClass::Changes();                              \
    return NOERROR;                                               \
}                                                                 \
                                                                  \
ECode className::BusyHandler(                                    \
        /* [in] */ IBusyHandler * bh)                             \
{                                                                 \
    return superClass::BusyHandler(bh);                          \
}                                                                 \
                                                                  \
ECode className::BusyTimeout(                                    \
        /* [in] */ Int32 ms)                                      \
{                                                                 \
    return superClass::BusyTimeout(ms);                          \
}                                                                 \
                                                                  \
ECode className::GetTable(                                       \
        /* [in] */ const String& sql,                             \
        /* [in] */ Int32 maxrows,                                 \
        /* [out] */ ITableResult ** tableresult)                  \
{                                                                 \
    *tableresult = superClass::GetTable(sql,maxrows);            \
    return NOERROR;                                               \
}                                                                 \
                                                                  \
ECode className::GetTableEx(                                     \
        /* [in] */ const String& sql,                             \
        /* [out] */ ITableResult ** tableresult)                  \
{                                                                 \
    *tableresult = superClass::GetTableEx(sql);                  \
    return NOERROR;                                               \
}                                                                 \
                                                                  \
ECode className::GetTableEx2(                                    \
        /* [in] */ const String& sql,                             \
        /* [in] */ Int32 maxrows,                                 \
        /* [in] */ const ArrayOf<String> & args,                  \
        /* [out] */ ITableResult ** tableresult)                  \
{                                                                 \
    *tableresult = superClass::GetTableEx2(sql,maxrows,args);    \
    return NOERROR;                                               \
}                                                                 \
                                                                  \
ECode className::GetTableEx3(                                    \
        /* [in] */ const String& sql,                             \
        /* [in] */ const ArrayOf<String> & args,                  \
        /* [out] */ ITableResult ** tableresult)                  \
{                                                                 \
    *tableresult = superClass::GetTableEx3(sql,args);            \
    return NOERROR;                                               \
}                                                                 \
                                                                  \
ECode className::GetTableEx4(                                    \
        /* [in] */ const String& sql,                             \
        /* [in] */ const ArrayOf<String> & args,                  \
        /* [in] */ ITableResult * tbl)                            \
{                                                                 \
    return superClass::GetTableEx4(sql,args,tbl);                \
}                                                                 \
                                                                  \
ECode className::Dbversion(                                       \
        /* [out] */ String * ver)                                 \
{                                                                 \
    *ver = superClass::Dbversion();                               \
    return NOERROR;                                               \
}                                                                 \
                                                                  \
ECode className::CreateFunction(                                 \
        /* [in] */ const String& name,                            \
        /* [in] */ Int32 nargs,                                   \
        /* [in] */ IFunction * f)                                 \
{                                                                 \
    return superClass::CreateFunction(name,nargs,f);             \
}                                                                 \
                                                                  \
ECode className::CreateAggregate(                                \
        /* [in] */ const String& name,                            \
        /* [in] */ Int32 nargs,                                   \
        /* [in] */ IFunction * f)                                 \
{                                                                 \
    return superClass::CreateAggregate(name,nargs,f);            \
}                                                                 \
                                                                  \
ECode className::FunctionType(                                   \
        /* [in] */ const String& name,                            \
        /* [in] */ Int32 type)                                    \
{                                                                 \
    return superClass::FunctionType(name,type);                  \
}                                                                 \
                                                                  \
ECode className::LastError(                                      \
        /* [out] */ Int32 * err)                                  \
{                                                                 \
    *err = superClass::LastError();                              \
    return NOERROR;                                               \
}                                                                 \
                                                                  \
ECode className::SetLastError(                                  \
        /* [in] */ Int32 error_code)                              \
{                                                                 \
    return superClass::SetLastError(error_code);                \
}                                                                 \
                                                                  \
ECode className::ErrorMessage(                                   \
        /* [out] */ String * str)                                 \
{                                                                 \
    *str = superClass::ErrorMessage();                           \
    return NOERROR;                                               \
}                                                                 \
                                                                  \
ECode className::SetEncoding(                                    \
        /* [in] */ const String& enc)                             \
{                                                                 \
    return superClass::SetEncoding(enc);                         \
}                                                                 \
                                                                  \
ECode className::SetAuthorizer(                                  \
        /* [in] */ IAuthorizer * auth)                            \
{                                                                 \
    return superClass::SetAuthorizer(auth);                      \
}                                                                 \
                                                                  \
ECode className::Trace(                                           \
        /* [in] */ ITrace * tr)                                   \
{                                                                 \
    return superClass::Trace(tr);                                 \
}                                                                 \
                                                                  \
ECode className::Backup(                                          \
        /* [in] */ IDatabase * dest,                              \
        /* [in] */ const String& destName,                        \
        /* [in] */ const String& srcName,                         \
        /* [out] */ IBackup ** backup)                            \
{                                                                 \
    *backup = superClass::Backup(dest,destName,srcName);          \
    return NOERROR;                                               \
}                                                                 \
                                                                  \
ECode className::Profile(                                         \
        /* [in] */ IProfile * pr)                                 \
{                                                                 \
    return superClass::Profile(pr);                               \
}                                                                 \
                                                                  \
ECode className::DbStatus(                                       \
        /* [in] */ Int32 op,                                      \
        /* [in] */ const ArrayOf<Int32> & info,                   \
        /* [in] */ Boolean flag,                                  \
        /* [out] */ Int32 * code)                                 \
{                                                                 \
    *code = superClass::DbStatus(op,info,flag);                  \
    return NOERROR;                                               \
}                                                                 \
                                                                  \
ECode className::Compile(                                         \
        /* [in] */ const String& sql,                             \
        /* [out] */ IVm ** ivm)                                   \
{                                                                 \
    *ivm = superClass::Compile(sql);                              \
    return NOERROR;                                               \
}                                                                 \
                                                                  \
ECode className::CompileEx(                                       \
        /* [in] */ const String& sql,                             \
        /* [in] */ const ArrayOf<String>& args,                   \
        /* [out] */ IVm ** ivm)                                   \
{                                                                 \
    *ivm = superClass::CompileEx(sql,args);                       \
    return NOERROR;                                               \
}                                                                 \
                                                                  \
ECode className::Prepare(                                         \
        /* [in] */ const String& sql,                             \
        /* [out] */ IStmt ** st)                                  \
{                                                                 \
    *st = superClass::Prepare(sql);                               \
    return NOERROR;                                               \
}                                                                 \
                                                                  \
ECode className::OpenBlob(                                       \
        /* [in] */ const String& db,                              \
        /* [in] */ const String& table,                           \
        /* [in] */ const String& column,                          \
        /* [in] */ Int64 row,                                     \
        /* [in] */ Boolean rw,                                    \
        /* [out] */ IBlob ** blob)                                \
{                                                                 \
    *blob = superClass::OpenBlob(db,table,column,row,rw);        \
    return NOERROR;                                               \
}                                                                 \
                                                                  \
ECode className::Is3(                                             \
        /* [out] */ Boolean * value)                              \
{                                                                 \
    *value = superClass::Is3();                                   \
    return NOERROR;                                               \
}                                                                 \
                                                                  \
ECode className::ProgressHandler(                                \
        /* [in] */ Int32 n,                                       \
        /* [in] */ IProgressHandler * p)                          \
{                                                                 \
    return superClass::ProgressHandler(n,p);                     \
}                                                                 \
                                                                  \
ECode className::Key(                                             \
        /* [in] */ const ArrayOf<Byte> & ekey)                    \
{                                                                 \
    return superClass::Key(ekey);                                 \
}                                                                 \
                                                                  \
ECode className::KeyEx(                                           \
        /* [in] */ const String& skey)                            \
{                                                                 \
    return superClass::KeyEx(skey);                               \
}                                                                 \

#endif  //__DATABASEMACRO_H__
