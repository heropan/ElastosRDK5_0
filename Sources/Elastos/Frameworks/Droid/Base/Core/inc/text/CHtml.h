
#ifndef __CHTML_H__
#define __CHTML_H__

#include "_CHtml.h"

using Elastos::Core::ICharSequence;
//using Elastos::Droid::Text::IHtmlImageGetter;
//using Elastos::Droid::Text::IHtmlTagHandler;

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CHtml)
{
public:
    CARAPI FromHtml(
        /* [in] */ const String& source,
        /* [out] */ ISpanned** ret);

    CARAPI FromHtml(
        /* [in] */ const String& source,
        /* [in] */ IHtmlImageGetter* imageGetter,
        /* [in] */ IHtmlTagHandler* tagHandler,
        /* [out] */ ISpanned** ret);

    CARAPI ToHtml(
        /* [in] */ ISpanned* text,
        /* [out] */ String* ret);

    CARAPI EscapeHtml(
        /* [in] */ ICharSequence* text,
        /* [out] */ String* ret);
};

} // namespace Text
} // namespace Droid
} // namespace Elastos

#endif // __CHTML_H__
