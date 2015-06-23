
#ifndef __CBITMAPREGIONDECODERHELPER_H__
#define __CBITMAPREGIONDECODERHELPER_H__

#include "_CBitmapRegionDecoderHelper.h"
#include <Elastos.CoreLibrary.h>

using Elastos::IO::IFileDescriptor;
using Elastos::IO::IInputStream;

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CBitmapRegionDecoderHelper)
{
public:
    CARAPI NewInstance(
        /* [in] */ const ArrayOf<Byte>& data,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    CARAPI NewInstance(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    CARAPI NewInstance(
        /* [in] */ IInputStream* is,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);

    CARAPI NewInstance(
        /* [in] */ const String& pathName,
        /* [in] */ Boolean isShareable,
        /* [out] */ IBitmapRegionDecoder** decoder);
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __CBITMAPREGIONDECODERHELPER_H__
