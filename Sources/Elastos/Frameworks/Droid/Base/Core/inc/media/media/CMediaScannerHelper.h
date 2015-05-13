
#ifndef __CMEDIASCANNERHELPER_H__
#define __CMEDIASCANNERHELPER_H__

#include "_CMediaScannerHelper.h"

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CMediaScannerHelper)
{
public:

    CARAPI IsNoMediaPath(
        /* [in] */ const String& path,
        /* [out] */ Boolean* result);
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __CMEDIASCANNERHELPER_H__
