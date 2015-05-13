
#ifndef __CMEDIAFILETYPE_H__
#define __CMEDIAFILETYPE_H__

#include "_CMediaFileType.h"

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CMediaFileType)
{
public:
    CMediaFileType();

    ~CMediaFileType();

    CARAPI constructor(
        /* [in] */ Int32 fileType,
        /* [in] */ const String& mimeType);

    CARAPI SetFileType(
        /* [in] */ Int32 result);

    CARAPI SetMimeType(
        /* [in] */ const String& result);

    CARAPI GetFileType(
        /* [out] */ Int32* result);

    CARAPI GetMimeType(
        /* [out] */ String* result);

public:
    Int32 mFileType;
    String mMimeType;
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __CMEDIAFILETYPE_H__
