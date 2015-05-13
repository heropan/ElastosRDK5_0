
#ifndef __CLINKIFY_H__
#define __CLINKIFY_H__

#include "_CLinkify.h"
#include "text/util/Linkify.h"

using Elastos::Core::ICharSequence;
using Elastos::Utility::Regex::IPattern;
using Elastos::Utility::Regex::IMatcher;
using Elastos::Droid::Widget::ITextView;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Utility {

CarClass(CLinkify), public Linkify
{
public:
    CARAPI GetStaticUrlMatchFilter(
        /* [out] */ ILinkifyMatchFilter** ret);

    CARAPI GetStaticPhoneNumberMatchFilter(
        /* [out] */ ILinkifyMatchFilter** ret);

    CARAPI GetStaticPhoneNumberTransformFilter(
        /* [out] */ ILinkifyTransformFilter** ret);

    CARAPI AddLinks(
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 mask,
        /* [out] */ Boolean* ret);

    CARAPI AddLinksEx(
        /* [in] */ ITextView* text,
        /* [in] */ Int32 mask,
        /* [out] */ Boolean* ret);

    CARAPI AddLinksEx2(
        /* [in] */ ITextView* text,
        /* [in] */ IPattern* pattern,
        /* [in] */ const String& scheme);

    CARAPI AddLinksEx3(
        /* [in] */ ITextView* text,
        /* [in] */ IPattern* p,
        /* [in] */ const String& scheme,
        /* [in] */ ILinkifyMatchFilter* matchFilter,
        /* [in] */ ILinkifyTransformFilter* transformFilter);

    CARAPI AddLinksEx4(
        /* [in] */ ISpannable* text,
        /* [in] */ IPattern* pattern,
        /* [in] */ const String& scheme,
        /* [out] */ Boolean* ret);

    CARAPI AddLinksEx5(
        /* [in] */ ISpannable* s,
        /* [in] */ IPattern* p,
        /* [in] */ const String& scheme,
        /* [in] */ ILinkifyMatchFilter* matchFilter,
        /* [in] */ ILinkifyTransformFilter* transformFilter,
        /* [out] */ Boolean* ret);

};

} // namespace Utility
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CLINKIFY_H__
