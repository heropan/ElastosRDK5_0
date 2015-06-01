
#ifndef __ELASTOS_IO_CFILEOUTPUTSTREAM_H__
#define __ELASTOS_IO_CFILEOUTPUTSTREAM_H__

#include "_Elastos_IO_CFileOutputStream.h"
#include "FileOutputStream.h"

namespace Elastos {
namespace IO {

CarClass(CFileOutputStream)
    , public FileOutputStream
{
public:
    ~CFileOutputStream();

    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneByte);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer);

    CARAPI WriteBytes(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI GetChannel(
        /* [out] */ IFileChannel** channel);

    CARAPI GetFD(
        /* [out] */ IFileDescriptor** fd);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IFile* file);

    CARAPI constructor(
        /* [in] */ IFile* file,
        /* [in] */ Boolean append);

    CARAPI constructor(
        /* [in] */ IFileDescriptor* fd);

    CARAPI constructor(
        /* [in] */ const String& fileName);

    CARAPI constructor(
        /* [in] */ const String& fileName,
        /* [in] */ Boolean append);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_CFILEOUTPUTSTREAM_H__
