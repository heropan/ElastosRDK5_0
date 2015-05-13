
#ifndef __CZIPFILE_H__
#define __CZIPFILE_H__

#include "_CZipFile.h"
#include "ZipFile.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CarClass(CZipFile) , public ZipFile
{
public:
    CARAPI Close();

    CARAPI GetEntries(
        /* [out] */ IObjectContainer** entries);

    CARAPI GetEntry(
        /* [in] */ const String& entryName,
        /* [out] */ IZipEntry** entry);

    CARAPI GetInputStream(
        /* [in] */ IZipEntry* entry,
        /* [out] */ IInputStream** is);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    /**
     * Constructs a new {@code ZipFile} with the specified file.
     *
     * @param file
     *            the file to read from.
     * @throws ZipException
     *             if a ZIP error occurs.
     * @throws IOException
     *             if an {@code IOException} occurs.
     */
    CARAPI constructor(
        /* [in] */ IFile* file);

    /**
     * Opens a file as <i>ZIP-archive</i>. "mode" must be {@code OPEN_READ} or
     * {@code OPEN_DELETE} . The latter sets the "delete on exit" flag through a
     * file.
     *
     * @param file
     *            the ZIP file to read.
     * @param mode
     *            the mode of the file open operation.
     * @throws IOException
     *             if an {@code IOException} occurs.
     */
    CARAPI constructor(
        /* [in] */ IFile* file,
        /* [in] */ Int32 mode);

    /**
     * Opens a ZIP archived file.
     *
     * @param name
     *            the name of the ZIP file.
     * @throws IOException
     *             if an IOException occurs.
     */
    CARAPI constructor(
        /* [in] */ const String& name);
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__CZIPFILE_H__
