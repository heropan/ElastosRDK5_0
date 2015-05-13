
#ifndef __CMEDIACODECLIST_H__
#define __CMEDIACODECLIST_H__

#include "_CMediaCodecList.h"

using Elastos::Droid::Media::ICodecCapabilities;

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CMediaCodecList)
{
public:
    /**
     * Count the number of available codecs.
     */
    /*static*/
    CARAPI GetCodecCount(
        /* [out] */ Int32* result);

    CARAPI GetCodecInfoAt(
        /* [in] */ Int32 index,
        /* [out] */ IMediaCodecInfo** result);

    static CARAPI GetCodecName(
        /* [in] */ Int32 index,
        /* [out] */ String* result);

    static CARAPI IsEncoder(
        /* [in] */ Int32 index,
        /* [out] */ Boolean* result);

    static CARAPI GetSupportedTypes(
        /* [in] */ Int32 index,
        /* [out, callee] */ ArrayOf<String>** result);

    static CARAPI GetCodecCapabilities(
        /* [in] */ Int32 index,
        /* [in] */ const String& type,
        /* [out] */ ICodecCapabilities** result);


private:
    CARAPI_(void) NativeInit();

    static const String TAG;
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos
#endif // __CMEDIACODECLIST_H__
