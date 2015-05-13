
#ifndef __CCODECPROFILELEVEL_H__
#define __CCODECPROFILELEVEL_H__

#include "_CCodecProfileLevel.h"

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CCodecProfileLevel)
{
public:
    CCodecProfileLevel();

    CARAPI GetPrfile(
        /* [out] */ Int32* value);

    CARAPI SetProfile(
        /* [in] */ Int32 value);

    CARAPI GetLevel(
        /* [out] */ Int32* value);

    CARAPI SetLevel(
       /* [in] */ Int32 value);

private:
    Int32 mProfile;
    Int32 mLevel;
};


} // namespace Media
} // namepsace Droid
} // namespace Elastos
#endif // __CCODECPROFILELEVEL_H__
