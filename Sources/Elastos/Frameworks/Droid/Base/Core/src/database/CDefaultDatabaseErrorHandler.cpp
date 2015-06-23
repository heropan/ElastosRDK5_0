
#include "database/CDefaultDatabaseErrorHandler.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::ICharSequence;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::Utility::Logging::Slogger;
using Elastos::Utility::IObjectStringMap;

namespace Elastos {
namespace Droid {
namespace Database {

const String CDefaultDatabaseErrorHandler::TAG("DefaultDatabaseErrorHandler");

ECode CDefaultDatabaseErrorHandler::OnCorruption(
    /* [in] */ ISQLiteDatabase* dbObj)
{
    String path;
    dbObj->GetPath(&path);
    Slogger::E(TAG, "Corruption reported by sqlite on database: %s", path.string());

    // is the corruption detected even before database could be 'opened'?
    Boolean isOpened;
    if (dbObj->IsOpen(&isOpened), !isOpened) {
        // database files are not even openable. delete this database file.
        // NOTE if the database has attached databases, then any of them could be corrupt.
        // and not deleting all of them could cause corrupted database file to remain and
        // make the application crash on database open operation. To avoid this problem,
        // the application should provide its own {@link DatabaseErrorHandler} impl class
        // to delete ALL files of the database (including the attached databases).
        dbObj->GetPath(&path);
        DeleteDatabaseFile(path);
        return NOERROR;
    }

    AutoPtr<IObjectStringMap> attachedDbs;
    //try {
    // Close the database, which will cause subsequent operations to fail.
    // before that, get the attached database list first.
    //try {
    dbObj->GetAttachedDbs((IObjectStringMap**)&attachedDbs);
    //} catch (SQLiteException e) {
        /* ignore */
    //}
    //try {
    dbObj->Close();
    //} catch (SQLiteException e) {
        /* ignore */
    //}
    //} finally {
    // Delete all files of this corrupt database and/or attached databases
    if (attachedDbs != NULL) {
        AutoPtr< ArrayOf<String> > keys;
        attachedDbs->GetKeys((ArrayOf<String>**)&keys);
        for (Int32 i = 0; i < keys->GetLength(); i++) {
            AutoPtr<ICharSequence> value;
            attachedDbs->Get((*keys)[i], (IInterface**)&value);
            String second;
            value->ToString(&second);
            DeleteDatabaseFile(second);
        }
    }
    else {
        // attachedDbs = null is possible when the database is so corrupt that even
        // "PRAGMA database_list;" also fails. delete the main database file
        dbObj->GetPath(&path);
        DeleteDatabaseFile(path);
    }
    //}
    return NOERROR;
}

void CDefaultDatabaseErrorHandler::DeleteDatabaseFile(
    /* [in] */ const String& fileName)
{
    if (fileName.EqualsIgnoreCase(":memory:") || fileName.Trim().GetLength() == 0) {
        return;
    }
    Slogger::E(TAG, "deleting the database file: %s", fileName.string());
    //try {
    AutoPtr<IFile> mfile;
    CFile::New(fileName, (IFile**)&mfile);
    Boolean result;
    if (FAILED(mfile->Delete(&result))) {
        /* print warning and ignore exception */
        Slogger::W(TAG, "delete failed: ");
    }
    //} catch (Exception e) {
        /* print warning and ignore exception */
        // Log.w(TAG, "delete failed: " + e.getMessage());
    //}
}

} //Database
} //Droid
} //Elastos
