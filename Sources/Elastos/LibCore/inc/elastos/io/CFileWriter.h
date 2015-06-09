
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
    CAR_OBJECT_DECL()
    
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
};

} // namespace IO
} // namespace Elastos

#endif //__ELASTOS_IO_CFILEWRITER_H__
