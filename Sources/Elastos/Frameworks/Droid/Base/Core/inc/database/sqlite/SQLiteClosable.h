#ifndef __SQLITECLOSABLE_H__
#define __SQLITECLOSABLE_H__

#include "ext/frameworkdef.h"

namespace Elastos {
namespace Droid {
namespace Database {
namespace Sqlite {

/**
 * An object created from a SQLiteDatabase that can be closed.
 *
 * This class implements a primitive reference counting scheme for database objects.
 */
class SQLiteClosable
{
public:
    SQLiteClosable();

    virtual CARAPI AcquireReference();

    virtual CARAPI ReleaseReference();

    virtual CARAPI ReleaseReferenceFromContainer();

    virtual CARAPI Close();

protected:
    virtual CARAPI_(void) OnAllReferencesReleased() = 0;

    CARAPI_(void) OnAllReferencesReleasedFromContainer();

private:
    Int32 mReferenceCount;
    Mutex mLock;
};

} //Sqlite
} //Database
} //Droid
} //Elastos

#endif //__SQLITECLOSABLE_H__
