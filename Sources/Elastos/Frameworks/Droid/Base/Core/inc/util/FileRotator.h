
#ifndef __ELASTOS_DROID_INTERNAL_UTILITY_FILEROTATOR_H__
#define __ELASTOS_DROID_INTERNAL_UTILITY_FILEROTATOR_H__

#include "ext/frameworkdef.h"
#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif

using Elastos::IO::IFile;
using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

class FileRotator : public ElRefBase
{
public:
    // TODO: provide method to append to active file

    /**
     * External class that reads data from a given {@link InputStream}. May be
     * called multiple times when reading rotated data.
     */
    interface Reader : public IInterface
    {
    public:
        virtual CARAPI Read(
            /* [in] */ IInputStream* in) = 0;
    };

    /**
     * External class that writes data to a given {@link OutputStream}.
     */
    interface Writer : public IInterface
    {
    public:
        virtual CARAPI Write(
            /* [in] */ IOutputStream* out) = 0;
    };

    /**
     * External class that reads existing data from given {@link InputStream},
     * then writes any modified data to {@link OutputStream}.
     */
    interface Rewriter
        : public Reader
        , public Writer
    {
    public:
        virtual CARAPI Reset() = 0;

        virtual CARAPI ShouldWrite(
            /* [out] */ Boolean* result) = 0;
    };

private:
    class FileInfo : public ElRefBase
    {
    public:
        FileInfo(
            /* [in] */ const String& prefix);

        /**
         * Attempt parsing the given filename.
         *
         * @return Whether parsing was successful.
         */
        CARAPI_(Boolean) Parse(
            /* [in] */ const String& name);

        /**
         * Build current state into filename.
         */
        CARAPI_(String) Build();

        /**
         * Test if current file is active (no end timestamp).
         */
        CARAPI_(Boolean) IsActive();

    public:
        String mPrefix;

        Int64 mStartMillis;
        Int64 mEndMillis;
    };

    class CombineActiveRewriter
        : public ElRefBase
        , public Rewriter
    {
    public:
        CombineActiveRewriter(
            /* [in] */ Reader* reader,
            /* [in] */ Writer* writer);

        CAR_INTERFACE_DECL()

        CARAPI Reset();

        CARAPI Read(
            /* [in] */ IInputStream* in);

        CARAPI ShouldWrite(
            /* [out] */ Boolean* result);

        CARAPI Write(
            /* [in] */ IOutputStream* out);

    private:
        AutoPtr<Reader> mReader;
        AutoPtr<Writer> mWriter;
    };

public:
    /**
     * Create a file rotator.
     *
     * @param basePath Directory under which all files will be placed.
     * @param prefix Filename prefix used to identify this rotator.
     * @param rotateAgeMillis Age in milliseconds beyond which an active file
     *            may be rotated into a historical file.
     * @param deleteAgeMillis Age in milliseconds beyond which a rotated file
     *            may be deleted.
     */
    FileRotator(
        /* [in] */ IFile* basePath,
        /* [in] */ const String& prefix,
        /* [in] */ Int64 rotateAgeMillis,
        /* [in] */ Int64 deleteAgeMillis);

    /**
     * Delete all files managed by this rotator.
     */
    CARAPI_(void) DeleteAll();

    /**
     * Dump all files managed by this rotator for debugging purposes.
     */
    CARAPI DumpAll(
        /* [in] */ IOutputStream* os);

    /**
     * Process currently active file, first reading any existing data, then
     * writing modified data. Maintains a backup during write, which is restored
     * if the write fails.
     */
    CARAPI RewriteActive(
        /* [in] */ Rewriter* rewriter,
        /* [in] */ Int64 currentTimeMillis);

    CARAPI CombineActive(
        /* [in] */ Reader* reader,
        /* [in] */ Writer* writer,
        /* [in] */ Int64 currentTimeMillis);

    /**
     * Process all files managed by this rotator, usually to rewrite historical
     * data. Each file is processed atomically.
     */
    CARAPI RewriteAll(
        /* [in] */ Rewriter* rewriter);

    /**
     * Read any rotated data that overlap the requested time range.
     */
    CARAPI ReadMatching(
        /* [in] */ Reader* reader,
        /* [in] */ Int64 matchStartMillis,
        /* [in] */ Int64 matchEndMillis);

    /**
     * Examine all files managed by this rotator, renaming or deleting if their
     * age matches the configured thresholds.
     */
    CARAPI_(void) MaybeRotate(
        /* [in] */ Int64 currentTimeMillis);

private:
    /**
     * Process a single file atomically, first reading any existing data, then
     * writing modified data. Maintains a backup during write, which is restored
     * if the write fails.
     */
    CARAPI RewriteSingle(
        /* [in] */ Rewriter* rewriter,
        /* [in] */ const String& name);

    /**
     * Return the currently active file, which may not exist yet.
     */
    CARAPI_(String) GetActiveName(
        /* [in] */ Int64 currentTimeMillis);

    static CARAPI ReadFile(
        /* [in] */ IFile* file,
        /* [in] */ Reader* reader);

    static CARAPI WriteFile(
        /* [in] */ IFile* file,
        /* [in] */ Writer* writer);

private:
    static const String TAG;
    static const Boolean LOGD = FALSE;

    AutoPtr<IFile> mBasePath;
    String mPrefix;
    Int64 mRotateAgeMillis;
    Int64 mDeleteAgeMillis;

    static const String SUFFIX_BACKUP;
    static const String SUFFIX_NO_BACKUP;
};

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_UTILITY_FILEROTATOR_H__
