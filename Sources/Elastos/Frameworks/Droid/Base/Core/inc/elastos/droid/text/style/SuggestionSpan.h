#ifndef __ELASTOS_DROID_TEXT_STYLE_SuggestionSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_SuggestionSpan_H__

#include "text/style/CharacterStyle.h"
#include "Elastos.Droid.Core_server.h"

using Libcore::ICU::ILocale;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

/**
 * Holds suggestion candidates for the text enclosed in this span.
 *
 * When such a span is edited in an EditText, double tapping on the text enclosed in this span will
 * display a popup dialog listing suggestion replacement for that text. The user can then replace
 * the original text by one of the suggestions.
 *
 * These spans should typically be created by the input method to provide correction and alternates
 * for the text.
 *
 * @see TextView#isSuggestionsEnabled()
 */
class SuggestionSpan
    : public CharacterStyle
{
public:
    /**
     * @param context Context for the application
     * @param suggestions Suggestions for the string under the span
     * @param flags Additional flags indicating how this span is handled in TextView
     */
    SuggestionSpan(
        /* [in] */ IContext* context,
        /* [in] */ ArrayOf<String>* suggestions,
        /* [in] */ Int32 flags);

    /**
     * @param locale Locale of the suggestions
     * @param suggestions Suggestions for the string under the span
     * @param flags Additional flags indicating how this span is handled in TextView
     */
    SuggestionSpan(
        /* [in] */ ILocale* locale,
        /* [in] */ ArrayOf<String>* suggestions,
        /* [in] */ Int32 flags);

    /**
     * @param context Context for the application
     * @param locale locale Locale of the suggestions
     * @param suggestions Suggestions for the string under the span. Only the first up to
     * {@link SuggestionSpan#SUGGESTIONS_MAX_SIZE} will be considered. Null values not permitted.
     * @param flags Additional flags indicating how this span is handled in TextView
     * @param notificationTargetClass if not null, this class will get notified when the user
     * selects one of the suggestions.
     */
    SuggestionSpan(
        /* [in] */ IContext* context,
        /* [in] */ ILocale* locale,
        /* [in] */ ArrayOf<String>* suggestions,
        /* [in] */ Int32 flags,
        /* [in] */ Handle32 notificationTargetClass);

    SuggestionSpan(
        /* [in] */ IParcel* src);

    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ ArrayOf<String>* suggestions,
        /* [in] */ Int32 flags);

    CARAPI_(void) Init(
        /* [in] */ ILocale* locale,
        /* [in] */ ArrayOf<String>* suggestions,
        /* [in] */ Int32 flags);

    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ ILocale* locale,
        /* [in] */ ArrayOf<String>* suggestions,
        /* [in] */ Int32 flags,
        /* [in] */ Handle32 notificationTargetClass);

    CARAPI_(void) Init(
        /* [in] */ IParcel* src);

    /**
     * @return an array of suggestion texts for this span
     */
    CARAPI_(AutoPtr< ArrayOf<String> >) GetSuggestions();

    /**
     * @return the locale of the suggestions
     */
    CARAPI_(String) GetLocale();

    /**
     * @return The name of the class to notify. The class of the original IME package will receive
     * a notification when the user selects one of the suggestions. The notification will include
     * the original string, the suggested replacement string as well as the hashCode of this span.
     * The class will get notified by an intent that has those information.
     * This is an internal API because only the framework should know the class name.
     *
     * @hide
     */
    CARAPI_(String) GetNotificationTargetClassName();

    CARAPI_(Int32) GetFlags();

    CARAPI_(void) SetFlags(
        /* [in] */ Int32 flags);

    //@Override
    CARAPI_(Int32) DescribeContents();

    //@Override
    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    //@Override
    CARAPI_(Int32) GetSpanTypeId();

    //@Override
    CARAPI_(Boolean) Equals(
        /* [in] */ IInterface* o);

    //@Override
    CARAPI_(Int32) GetHashCode();

    //@Override
    CARAPI UpdateDrawState(
        /* [in] */ ITextPaint* tp);

    /**
     * @return The color of the underline for that span, or 0 if there is no underline
     *
     * @hide
     */
    CARAPI_(Int32) GetUnderlineColor();

protected:
    SuggestionSpan();

private:
    CARAPI_(void) InitStyle(
        /* [in] */ IContext* context);

    static CARAPI_(Int32) HashCodeInternal(
        /* [in] */ ArrayOf<String>* suggestions,
        /* [in] */ const String& locale,
        /* [in] */ const String& notificationTargetClassName);

private:
    /*
     * TODO: Needs to check the validity and add a feature that TextView will change
     * the current IME to the other IME which is specified in SuggestionSpan.
     * An IME needs to set the span by specifying the target IME and Subtype of SuggestionSpan.
     * And the current IME might want to specify any IME as the target IME including other IMEs.
     */

    Int32 mFlags;
    AutoPtr< ArrayOf<String> > mSuggestions;
    String mLocaleString;
    String mNotificationTargetClassName;
    Int32 mHashCode;

    Float mEasyCorrectUnderlineThickness;
    Int32 mEasyCorrectUnderlineColor;

    Float mMisspelledUnderlineThickness;
    Int32 mMisspelledUnderlineColor;

    Float mAutoCorrectionUnderlineThickness;
    Int32 mAutoCorrectionUnderlineColor;

};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_SuggestionSpan_H__
