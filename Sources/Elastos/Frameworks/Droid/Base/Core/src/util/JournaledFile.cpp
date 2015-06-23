
#include "util/JournaledFile.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

JournaledFile::JournaledFile(
    /* [in] */ IFile* real,
    /* [in] */ IFile* temp)
    : mReal(real)
    , mTemp(temp)
    , mWriting(FALSE)
{
}

AutoPtr<IFile> JournaledFile::ChooseForRead()
{
    AutoPtr<IFile> result;
    Boolean rExists, tExists;
    mReal->Exists(&rExists);
    mTemp->Exists(&tExists);
    if (rExists) {
        result = mReal;
        if (tExists) {
            Boolean res;
            mTemp->Delete(&res);
        }
    }
    else if (tExists) {
        result = mTemp;
        Boolean res;
        mTemp->RenameTo(mReal, &res);
    }
    else {
        return mReal;
    }
    return result;
}

AutoPtr<IFile> JournaledFile::ChooseForWrite()
{
    if (mWriting) {
        Logger::E("JournaledFile", "uncommitted write already in progress");
        assert(0);
        //throw new IllegalStateException("uncommitted write already in progress");
    }

    Boolean rExists, tExists;
    mReal->Exists(&rExists);
    mTemp->Exists(&tExists);
    if (!rExists) {
        // If the real one doesn't exist, it's either because this is the first time
        // or because something went wrong while copying them.  In this case, we can't
        // trust anything that's in temp.  In order to have the chooseForRead code not
        // use the temporary one until it's fully written, create an empty file
        // for real, which will we'll shortly delete.
        Boolean res;
        mReal->CreateNewFile(&res);
    }

    if (tExists) {
        Boolean res;
        mTemp->Delete(&res);
    }
    mWriting = TRUE;
    return mTemp;
}

ECode JournaledFile::Commit()
{
    if (!mWriting) {
        Logger::E("JournaledFile", "no file to commit");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mWriting = FALSE;
    Boolean res;
    return mTemp->RenameTo(mReal, &res);
}

ECode JournaledFile::Rollback()
{
    if (!mWriting) {
        Logger::E("JournaledFile", "no file to roll back");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mWriting = FALSE;
    Boolean res;
    return mTemp->Delete(&res);
}

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos
