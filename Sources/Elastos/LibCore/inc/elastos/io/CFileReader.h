
#ifndef __ELASTOS_IO_CFILEREADER_H__
#define __ELASTOS_IO_CFILEREADER_H__

#include "_Elastos_IO_CFileReader.h"
#include "FileReader.h"

namespace Elastos {
namespace IO {

CarClass(CFileReader)
    , public FileReader
{
public:
    CAR_OBJECT_DECL()

    /**
     * Constructs a new FileReader on the given {@code file}.
     *
     * @param file
     *            a File to be opened for reading characters from.
     * @throws FileNotFoundException
     *             if {@code file} does not exist.
     */
    CARAPI constructor(
        /* [in] */ IFile* file);

    /**
     * Construct a new FileReader on the given FileDescriptor {@code fd}. Since
     * a previously opened FileDescriptor is passed as an argument, no
     * FileNotFoundException can be thrown.
     *
     * @param fd
     *            the previously opened file descriptor.
     */
    CARAPI constructor(
        /* [in] */ IFileDescriptor* fd);

    /**
     * Construct a new FileReader on the given file named {@code filename}.
     *
     * @param filename
     *            an absolute or relative path specifying the file to open.
     * @throws FileNotFoundException
     *             if there is no file named {@code filename}.
     */
    CARAPI constructor(
        /* [in] */ const String& filename);
};

} // namespace IO
} // namespace Elastos

#endif //__ELASTOS_IO_CFILEREADER_H__
