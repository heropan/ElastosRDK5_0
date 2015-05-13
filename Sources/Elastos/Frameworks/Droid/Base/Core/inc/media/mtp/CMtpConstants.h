
#ifndef __CMTPCONSTANTS_H__
#define __CMTPCONSTANTS_H__

#include "_CMtpConstants.h"

namespace Elastos {
namespace Droid {
namespace Mtp {

CarClass(CMtpConstants)
{
public:
    CARAPI IsAbstractObject(
        /* [in] */ Int32 format,
        /* [out] */ Boolean* result);
};
} // namespace Mtp
} // namepsace Droid
} // namespace Elastos
#endif // __CMTPCONSTANTS_H__
