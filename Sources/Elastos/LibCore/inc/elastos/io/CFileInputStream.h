
#ifndef __CFILEINPUTSTREAM_H__
#define __CFILEINPUTSTREAM_H__

#include "_CFileInputStream.h"
#include "FileInputStream.h"

namespace Elastos {
namespace IO {

CarClass(CFileInputStream)
    , public FileInputStream
{
public:
    CARAPI Available(
        /* [out] */ Int32* number);

    CARAPI Close();

    CARAPI Mark(
        /* [in] */ Int32 readLimit);

    CARAPI IsMarkSupported(
        /* [out] */ Boolean* supported);

    CARAPI Read(
        /* [out] */ Int32* value);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadBytes(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [out] */ Int32* number);

    CARAPI Reset();

    CARAPI Skip(
        /* [in] */ Int64 byteCount,
        /* [out] */ Int64* number);

    CARAPI GetChannel(
        /* [out] */ IFileChannel** channel);

    CARAPI GetFD(
        /* [out] */ IFileDescriptor** fd);

    CARAPI constructor(
        /* [in] */ IFile* file);

    CARAPI constructor(
        /* [in] */ IFileDescriptor* fd);

    CARAPI constructor(
        /* [in] */ const String& fileName);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

};

} // namespace IO
} // namespace Elastos

#endif // __CFILEINPUTSTREAM_H__
