
#ifndef __ELASTOS_DROID_MEDIA_CMEDIAFILETYPE_H__
#define __ELASTOS_DROID_MEDIA_CMEDIAFILETYPE_H__

#include "_Elastos_Droid_Media_CMediaFileType.h"

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

#endif // __ELASTOS_DROID_MEDIA_CMEDIAFILETYPE_H__
