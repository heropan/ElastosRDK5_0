
#ifndef __CFILEREADER_H__
#define __CFILEREADER_H__

#include "_CFileReader.h"
#include "FileReader.h"

namespace Elastos {
namespace IO {

CarClass(CFileReader)
    , public FileReader
{
public:
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

    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadChars(
        /* [out] */ ArrayOf<Char32>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadCharsEx(
        /* [out] */ ArrayOf<Char32>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [out] */ Int32* number);

    CARAPI IsReady(
        /* [out] */ Boolean* ready);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    CARAPI ReadCharBuffer(
        /* [in] */ ICharBuffer* target,
        /* [out] */ Int32* number);

    CARAPI GetEncoding(
        /* [out] */ String* encoding);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

} // namespace IO
} // namespace Elastos

#endif //__CFILEREADER_H__
