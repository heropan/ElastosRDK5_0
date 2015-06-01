
#ifndef __ELASTOS_IO_CFILEWRITER_H__
#define __ELASTOS_IO_CFILEWRITER_H__

#include "_Elastos_IO_CFileWriter.h"
#include "FileWriter.h"

namespace Elastos {
namespace IO {

CarClass(CFileWriter)
    , public FileWriter
{
public:
    CARAPI constructor(
        /* [in] */ IFile* file);

    CARAPI constructor(
        /* [in] */ IFile* file,
        /* [in] */ Boolean append);

    CARAPI constructor(
        /* [in] */ IFileDescriptor* fd);

    CARAPI constructor(
        /* [in] */ const String& filename);

    CARAPI constructor(
        /* [in] */ const String& filename,
        /* [in] */ Boolean append);

    CARAPI Close();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ Int32 oneChar32);

    CARAPI Write(
        /* [in] */ const ArrayOf<Char32>& buffer);

    CARAPI Write(
        /* [in] */ const ArrayOf<Char32>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI WriteString(
        /* [in] */ const String& str);

    CARAPI WriteString(
        /* [in] */ const String& str,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI AppendChar(
        /* [in] */ Char32 c);

    CARAPI AppendCharSequence(
        /* [in] */ ICharSequence* csq);

    CARAPI AppendCharSequence(
        /* [in] */ ICharSequence* csq,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI GetEncoding(
        /* [out] */ String* encoding);

    CARAPI GetLock(
        /* [out] */ IInterface** lockobj);
};

} // namespace IO
} // namespace Elastos

#endif //__ELASTOS_IO_CFILEWRITER_H__
