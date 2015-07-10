
#ifndef __DATETIMECHOOSERANDROID_H__
#define __DATETIMECHOOSERANDROID_H__

// import android.content.Context;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.content.browser.ContentViewCore;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

/**
 * Plumbing for the different date/time dialog adapters.
 */
//@JNINamespace("content")
class DateTimeChooserAndroid
{
private:
    class InnerInputActionDelegate
        : public Object
        : public InputDialogContainer::InputActionDelegate
    {
    public:
        InnerInputActionDelegate(
            /* [in] */ DateTimeChooserAndroid* owner);

        //@Override
        CARAPI_(void) ReplaceDateTime(
            /* [in] */ Double value);

        //@Override
        CARAPI_(void) CancelDateTimeDialog();

    private:
        DateTimeChooserAndroid* mOwner;
    };

private:
    DateTimeChooserAndroid(
        /* [in] */ IContext* context,
        /* [in] */ Int64 nativeDateTimeChooserAndroid);

    CARAPI_(void) ShowDialog(
        /* [in] */ Int32 dialogType,
        /* [in] */ Double dialogValue,
        /* [in] */ Double min,
        /* [in] */ Double max,
        /* [in] */ Double step,
        /* [in] */ ArrayOf<DateTimeSuggestion>* suggestions);

    //@CalledByNative
    static CARAPI_(AutoPtr<DateTimeChooserAndroid>) CreateDateTimeChooser(
        /* [in] */ ContentViewCore* contentViewCore,
        /* [in] */ Int64 nativeDateTimeChooserAndroid,
        /* [in] */ Int32 dialogType,
        /* [in] */ Double dialogValue,
        /* [in] */ Double min,
        /* [in] */ Double max,
        /* [in] */ Double step,
        /* [in] */ ArrayOf<DateTimeSuggestion>* suggestions);

    //@CalledByNative
    static CARAPI_(AutoPtr< ArrayOf<DateTimeSuggestion> >) CreateSuggestionsArray(
        /* [in] */ Int32 size);

    /**
     * @param array DateTimeSuggestion array that should get a new suggestion set.
     * @param index Index in the array where to place a new suggestion.
     * @param value Value of the suggestion.
     * @param localizedValue Localized value of the suggestion.
     * @param label Label of the suggestion.
     */
    //@CalledByNative
    static CARAPI_(void) SetDateTimeSuggestionAt(
        /* [in] */ ArrayOf<DateTimeSuggestion>* array,
        /* [in] */ Int32 index,
        /* [in] */ Double value,
        /* [in] */ String localizedValue,
        /* [in] */ String label);

    //@CalledByNative
    static CARAPI_(void) InitializeDateInputTypes(
        /* [in] */ Int32 textInputTypeDate,
        /* [in] */ Int32 textInputTypeDateTime,
        /* [in] */ Int32 textInputTypeDateTimeLocal,
        /* [in] */ Int32 textInputTypeMonth,
        /* [in] */ Int32 textInputTypeTime,
        /* [in] */ Int32 textInputTypeWeek);

    CARAPI_(void) NativeReplaceDateTime(
        /* [in] */ Int64 nativeDateTimeChooserAndroid,
        /* [in] */ Double dialogValue);

    CARAPI_(void) NativeCancelDialog(
        /* [in] */ Int64 nativeDateTimeChooserAndroid);

private:
    const Int64 mNativeDateTimeChooserAndroid;
    const AutoPtr<InputDialogContainer> mInputDialogContainer;
};

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__DATETIMECHOOSERANDROID_H__
