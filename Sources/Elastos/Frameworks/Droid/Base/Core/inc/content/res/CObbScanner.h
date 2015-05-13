
#ifndef __COBBSCANNER_H__
#define __COBBSCANNER_H__

#include "_CObbScanner.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

CarClass(CObbScanner)
{
public:
    CARAPI GetObbInfo(
        /* [in] */ const String& filePath,
        /* [out] */ IObbInfo** obbInfo);

private:
    CARAPI NativeGetObbInfo(
        /* [in] */ const String& filePath,
        /* [in, out] */ IObbInfo* obbInfo);
};

} // namespace Res
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __COBBSCANNER_H__
