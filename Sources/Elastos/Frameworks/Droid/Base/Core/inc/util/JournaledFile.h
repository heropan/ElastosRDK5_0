
#ifndef __ELASTOS_DROID_INTERNAL_UTILITY_JOURNALEDFILE_H__
#define __ELASTOS_DROID_INTERNAL_UTILITY_JOURNALEDFILE_H__

#include "ext/frameworkext.h"

using Elastos::IO::IFile;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

class JournaledFile : public ElRefBase
{
public:
    JournaledFile(
        /* [in] */ IFile* real,
        /* [in] */ IFile* temp);

    /** Returns the file for you to read.
     * @more
     * Prefers the real file.  If it doesn't exist, uses the temp one, and then copies
     * it to the real one.  If there is both a real file and a temp one, assumes that the
     * temp one isn't fully written and deletes it.
     */
    CARAPI_(AutoPtr<IFile>) ChooseForRead();

    /**
     * Returns a file for you to write.
     * @more
     * If a write is already happening, throws.  In other words, you must provide your
     * own locking.
     * <p>
     * Call {@link #commit} to commit the changes, or {@link #rollback} to forget the changes.
     */
    CARAPI_(AutoPtr<IFile>) ChooseForWrite();

    /**
     * Commit changes.
     */
    CARAPI Commit();

    /**
     * Roll back changes.
     */
    CARAPI Rollback();

private:
    AutoPtr<IFile> mReal;
    AutoPtr<IFile> mTemp;
    Boolean mWriting;
};

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_UTILITY_JOURNALEDFILE_H__
