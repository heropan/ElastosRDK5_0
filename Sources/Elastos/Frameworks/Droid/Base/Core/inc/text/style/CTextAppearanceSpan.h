
#ifndef __CTEXTAPPEARANCESPAN_H__
#define __CTEXTAPPEARANCESPAN_H__

#include "_CTextAppearanceSpan.h"
#include "text/style/TextAppearanceSpan.h"




namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CTextAppearanceSpan)
    , public TextAppearanceSpan
{
public:
    ICHARACTERSTYLE_METHODS_DECL()
    IMETRICAFFECTINGSPAN_METHODS_DECL()
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetFamily(
        /* [out] */ String* ret);

    CARAPI GetTextColor(
        /* [out] */ IColorStateList** ret);

    CARAPI GetLinkTextColor(
        /* [out] */ IColorStateList** ret);

    CARAPI GetTextSize(
        /* [out] */ Int32* ret);

    CARAPI GetTextStyle(
        /* [out] */ Int32* ret);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 appearance);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 appearance,
        /* [in] */ Int32 colorList);

    CARAPI constructor(
        /* [in] */ const String& family,
        /* [in] */ Int32 style,
        /* [in] */ Int32 size,
        /* [in] */ IColorStateList* color,
        /* [in] */ IColorStateList* linkColor);

    CARAPI constructor(
        /* [in] */ IParcel* src);

};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CTEXTAPPEARANCESPAN_H__
