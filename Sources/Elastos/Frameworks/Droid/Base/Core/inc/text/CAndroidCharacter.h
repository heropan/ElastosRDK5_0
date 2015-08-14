
#ifndef __ELASTOS_DROID_TEXT_CANDROIDCHARACTER_H__
#define __ELASTOS_DROID_TEXT_CANDROIDCHARACTER_H__

#include "_CAndroidCharacter.h"

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CAndroidCharacter)
{
public:
    static CARAPI GetDirectionalities(
        /* [in] */ ArrayOf<Char32>* src,
        /* [in] */ ArrayOf<Byte>* dest,
        /* [in] */ Int32 count);

    static CARAPI_(Int32) GetEastAsianWidth(
        /* [in] */ Char32 input);

    static CARAPI GetEastAsianWidths(
        /* [in] */ ArrayOf<Char32>* src,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count,
        /* [in] */ ArrayOf<Byte>* dest);

    static CARAPI_(Boolean) Mirror(
        /* [in] */ ArrayOf<Char32>* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count);

    static CARAPI_(Char32) GetMirror(
        /* [in] */ Char32 ch);

};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_CANDROIDCHARACTER_H__
