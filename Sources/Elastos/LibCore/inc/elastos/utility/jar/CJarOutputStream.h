
#ifndef __CJAROUTPUTSTREAM_H__
#define __CJAROUTPUTSTREAM_H__

#include "_CJarOutputStream.h"
#include "ZipOutputStream.h"

using Elastos::Utility::Zip::IZipEntry;
using Elastos::Utility::Zip::ZipOutputStream;
using Elastos::Utility::Jar::IManifest;
using Elastos::IO::IOutputStream;

namespace Elastos {
namespace Utility {
namespace Jar {

CarClass(CJarOutputStream), ZipOutputStream
{
public:
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

    CARAPI CheckError(
        /* [out] */ Boolean* hasError);

    CARAPI Finish();

    CARAPI CloseEntry();

    CARAPI PutNextEntry(
        /* [in] */ IZipEntry* ze);

    CARAPI SetComment(
        /* [in] */ const String& comment);

    CARAPI SetLevel(
        /* [in] */ Int32 level);

    CARAPI SetMethod(
        /* [in] */ Int32 method);

    CARAPI constructor(
        /* [in] */ IOutputStream* os,
        /* [in] */ IManifest* manifest);

    CARAPI constructor(
        /* [in] */ IOutputStream* os);

private:
    AutoPtr<IManifest> mManifest;
};

} // namespace Jar
} // namespace Utility
} // namespace Elastos

#endif // __CJAROUTPUTSTREAM_H__
