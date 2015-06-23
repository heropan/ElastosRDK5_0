
#include "database/sqlite/SQLiteClosable.h"
#include "Elastos.Droid.Core_server.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Database {
namespace Sqlite {

SQLiteClosable::SQLiteClosable()
    : mReferenceCount(1)
{}

void SQLiteClosable::OnAllReferencesReleasedFromContainer()
{
    return OnAllReferencesReleased();
}

ECode SQLiteClosable::AcquireReference()
{
    Mutex::Autolock lock(mLock);

    if (mReferenceCount <= 0) {
        // throw new IllegalStateException(
        //                 "attempt to re-open an already-closed object: " + this);
        Slogger::E(String("SQLiteClosable"), "attempt to re-open an already-closed object: %p", this);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mReferenceCount++;
    return NOERROR;
}

ECode SQLiteClosable::ReleaseReference()
{
    Boolean refCountIsZero = FALSE;
    {
        Mutex::Autolock lock(mLock);
        refCountIsZero = --mReferenceCount == 0;
    }
    if (refCountIsZero) {
        OnAllReferencesReleased();
    }

    return NOERROR;
}

ECode SQLiteClosable::ReleaseReferenceFromContainer()
{
    Boolean refCountIsZero = FALSE;
    {
        Mutex::Autolock lock(mLock);
        refCountIsZero = --mReferenceCount == 0;
    }
    if (refCountIsZero) {
        OnAllReferencesReleasedFromContainer();
    }
    return NOERROR;
}

ECode SQLiteClosable::Close()
{
    return ReleaseReference();
}

} //Sqlite
} //Database
} //Droid
} //Elastos
