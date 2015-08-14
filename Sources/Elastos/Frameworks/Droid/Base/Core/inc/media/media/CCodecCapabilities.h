
#ifndef __ELASTOS_DROID_MEDIA_CODECCAPABILITIES_H__
#define __ELASTOS_DROID_MEDIA_CODECCAPABILITIES_H__

#include "_CCodecCapabilities.h"

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CCodecCapabilities)
{
public:
    CCodecCapabilities();

    CARAPI GetProfileLevels(
        /* [out, callee] */ ArrayOf<ICodecProfileLevel*>** value);

    CARAPI SetProfileLevels(
        /* [in] */ ArrayOf<ICodecProfileLevel*>* value);

    CARAPI GetColorFormats(
        /* [out, callee] */ ArrayOf<Int32>** value);

    CARAPI SetColorFormats(
        /* [in] */ ArrayOf<Int32>* value);

private:
    AutoPtr<ArrayOf<ICodecProfileLevel*> > mProfileLevels;
    AutoPtr<ArrayOf<Int32> > mColorFormats;
};


} // namespace Media
} // namepsace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_MEDIA_CODECCAPABILITIES_H__
