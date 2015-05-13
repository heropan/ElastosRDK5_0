#ifndef __HH_AUTODECODERCANCEL_H
#define __HH_AUTODECODERCANCEL_H

#include "Elastos.Droid.Core_server.h"
#include <skia/images/SkImageDecoder.h>


namespace Elastos {
namespace Droid {
namespace Graphics {

class AutoDecoderCancel
{
public:
    AutoDecoderCancel(
        /* [in] */ IBitmapFactoryOptions* options,
        /* [in] */ SkImageDecoder* decoder);

    ~AutoDecoderCancel();

    static CARAPI_(Boolean) RequestCancel(
        /* [in] */ IBitmapFactoryOptions* options);

private:
    AutoDecoderCancel* mNext;
    AutoDecoderCancel* mPrev;
    AutoPtr<IBitmapFactoryOptions> mOptions;
    SkImageDecoder* mDecoder;

#ifdef SK_DEBUG
    static void Validate();
#else
    static void Validate() {}
#endif
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __HH_AUTODECODERCANCEL_H
