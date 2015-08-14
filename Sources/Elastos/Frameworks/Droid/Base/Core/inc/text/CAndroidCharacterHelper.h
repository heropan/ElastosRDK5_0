
#ifndef __ELASTOS_DROID_TEXT_CANDROIDCHARACTERHELPER_H__
#define __ELASTOS_DROID_TEXT_CANDROIDCHARACTERHELPER_H__

#include "_CAndroidCharacterHelper.h"

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CAndroidCharacterHelper)
{
public:
    CARAPI GetDirectionalities(
        /* [in] */ ArrayOf<Char32>* src,
        /* [in] */ ArrayOf<Byte>* dest,
        /* [in] */ Int32 count);

    CARAPI GetEastAsianWidth(
        /* [in] */ Char32 input,
        /* [out] */ Int32* ret);

    CARAPI GetEastAsianWidths(
        /* [in] */ ArrayOf<Char32>* src,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count,
        /* [in] */ ArrayOf<Byte>* dest);

    CARAPI Mirror(
        /* [in] */ ArrayOf<Char32>* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count,
        /* [out] */ Boolean* ret);

    CARAPI GetMirror(
        /* [in] */ Char32 ch,
        /* [out] */ Char32* ret);

};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_CANDROIDCHARACTERHELPER_H__
