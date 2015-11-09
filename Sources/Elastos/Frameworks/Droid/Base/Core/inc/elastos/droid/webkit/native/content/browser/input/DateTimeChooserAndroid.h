
#ifndef __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_INPUT_DATETIMECHOOSERANDROID_H__
#define __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_INPUT_DATETIMECHOOSERANDROID_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/webkit/native/content/browser/input/InputDialogContainer.h"

using Elastos::Droid::Content::IContext;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.content.browser.ContentViewCore;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

class ContentViewCore;

namespace Input {

/**
 * Plumbing for the different date/time dialog adapters.
 */
//@JNINamespace("content")
class DateTimeChooserAndroid
    : public Object
{
private:
    class InnerInputActionDelegate
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

public:
    static CARAPI_(void*) ElaDateTimeChooserAndroidCallback_Init();

private:
    static CARAPI_(AutoPtr<IInterface>) CreateDateTimeChooser(
        /* [in] */ IInterface* contentViewCore,
        /* [in] */ Int64 nativeDateTimeChooserAndroid,
        /* [in] */ Int32 dialogType,
        /* [in] */ Double dialogValue,
        /* [in] */ Double min,
        /* [in] */ Double max,
        /* [in] */ Double step,
        /* [in] */ ArrayOf<IInterface*>* suggestions);

    DateTimeChooserAndroid(
        /* [in] */ IContext* context,
        /* [in] */ Int64 nativeDateTimeChooserAndroid);

    CARAPI_(void) ShowDialog(
        /* [in] */ Int32 dialogType,
        /* [in] */ Double dialogValue,
        /* [in] */ Double min,
        /* [in] */ Double max,
        /* [in] */ Double step,
        /* [in] */ ArrayOf<IInterface*>* suggestions);

    //@CalledByNative
    static CARAPI_(AutoPtr<DateTimeChooserAndroid>) CreateDateTimeChooser(
        /* [in] */ ContentViewCore* contentViewCore,
        /* [in] */ Int64 nativeDateTimeChooserAndroid,
        /* [in] */ Int32 dialogType,
        /* [in] */ Double dialogValue,
        /* [in] */ Double min,
        /* [in] */ Double max,
        /* [in] */ Double step,
        /* [in] */ ArrayOf<IInterface*>* suggestions);

    //@CalledByNative return DateTimeSuggestion
    static CARAPI_(AutoPtr<ArrayOf<IInterface*> >) CreateSuggestionsArray(
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
        /* [in] */ ArrayOf<IInterface*>* array,
        /* [in] */ Int32 index,
        /* [in] */ Double value,
        /* [in] */ const String& localizedValue,
        /* [in] */ const String& label);

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
    AutoPtr<InputDialogContainer> mInputDialogContainer;
};

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_INPUT_DATETIMECHOOSERANDROID_H__
