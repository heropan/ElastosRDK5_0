
#include "elastos/droid/webkit/native/content/browser/input/DateTimeChooserAndroid.h"
#include "elastos/droid/webkit/native/content/api/DateTimeChooserAndroid_dec.h"
#include "elastos/droid/webkit/native/content/browser/input/DateTimeSuggestion.h"
#include "elastos/droid/webkit/native/content/browser/ContentViewCore.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

//==================================================================
//        DateTimeChooserAndroid::InnerInputActionDelegate
//==================================================================

DateTimeChooserAndroid::InnerInputActionDelegate::InnerInputActionDelegate(
    /* [in] */ DateTimeChooserAndroid* owner)
    : mOwner(owner)
{
}

//@Override
void DateTimeChooserAndroid::InnerInputActionDelegate::ReplaceDateTime(
    /* [in] */ Double value)
{
    mOwner->NativeReplaceDateTime(mOwner->mNativeDateTimeChooserAndroid, value);
}

//@Override
void DateTimeChooserAndroid::InnerInputActionDelegate::CancelDateTimeDialog()
{
    mOwner->NativeCancelDialog(mOwner->mNativeDateTimeChooserAndroid);
}

//==================================================================
//                     DateTimeChooserAndroid
//==================================================================

DateTimeChooserAndroid::DateTimeChooserAndroid(
    /* [in] */ IContext* context,
    /* [in] */ Int64 nativeDateTimeChooserAndroid)
    : mNativeDateTimeChooserAndroid(nativeDateTimeChooserAndroid)
{
    AutoPtr<InputDialogContainer::InputActionDelegate> delegate = new InnerInputActionDelegate(this);
    mInputDialogContainer = new InputDialogContainer(context, delegate);
}

void DateTimeChooserAndroid::ShowDialog(
    /* [in] */ Int32 dialogType,
    /* [in] */ Double dialogValue,
    /* [in] */ Double min,
    /* [in] */ Double max,
    /* [in] */ Double step,
    /* [in] */ ArrayOf<IInterface*>* suggestions)
{
    mInputDialogContainer->ShowDialog(dialogType, dialogValue, min, max, step, suggestions);
}

//@CalledByNative
AutoPtr<DateTimeChooserAndroid> DateTimeChooserAndroid::CreateDateTimeChooser(
    /* [in] */ ContentViewCore* contentViewCore,
    /* [in] */ Int64 nativeDateTimeChooserAndroid,
    /* [in] */ Int32 dialogType,
    /* [in] */ Double dialogValue,
    /* [in] */ Double min,
    /* [in] */ Double max,
    /* [in] */ Double step,
    /* [in] */ ArrayOf<IInterface*>* suggestions)
{
    AutoPtr<DateTimeChooserAndroid> chooser =
            new DateTimeChooserAndroid(
                    contentViewCore->GetContext(),
                    nativeDateTimeChooserAndroid);
    chooser->ShowDialog(dialogType, dialogValue, min, max, step, suggestions);

    return chooser;
}

//@CalledByNative
AutoPtr<ArrayOf<IInterface*> > DateTimeChooserAndroid::CreateSuggestionsArray(
    /* [in] */ Int32 size)
{
    AutoPtr<ArrayOf<IInterface*> > array = ArrayOf<IInterface*>::Alloc(size);
    return array;
}

/**
 * @param array DateTimeSuggestion array that should get a new suggestion set.
 * @param index Index in the array where to place a new suggestion.
 * @param value Value of the suggestion.
 * @param localizedValue Localized value of the suggestion.
 * @param label Label of the suggestion.
 */
//@CalledByNative
void DateTimeChooserAndroid::SetDateTimeSuggestionAt(
    /* [in] */ ArrayOf<IInterface*>* array,
    /* [in] */ Int32 index,
    /* [in] */ Double value,
    /* [in] */ const String& localizedValue,
    /* [in] */ const String& label)
{
    //(*array)[index] = new DateTimeSuggestion(value, localizedValue, label);
    array->Set(index, TO_IINTERFACE(new DateTimeSuggestion(value, localizedValue, label)));
}

//@CalledByNative
void DateTimeChooserAndroid::InitializeDateInputTypes(
    /* [in] */ Int32 textInputTypeDate,
    /* [in] */ Int32 textInputTypeDateTime,
    /* [in] */ Int32 textInputTypeDateTimeLocal,
    /* [in] */ Int32 textInputTypeMonth,
    /* [in] */ Int32 textInputTypeTime,
    /* [in] */ Int32 textInputTypeWeek)
{
    InputDialogContainer::InitializeInputTypes(
            textInputTypeDate,
            textInputTypeDateTime, textInputTypeDateTimeLocal,
            textInputTypeMonth, textInputTypeTime, textInputTypeWeek);
}

void DateTimeChooserAndroid::NativeReplaceDateTime(
    /* [in] */ Int64 nativeDateTimeChooserAndroid,
    /* [in] */ Double dialogValue)
{
    Elastos_DateTimeChooserAndroid_nativeReplaceDateTime(THIS_PROBE(IInterface), (Handle32)nativeDateTimeChooserAndroid, dialogValue);
}

void DateTimeChooserAndroid::NativeCancelDialog(
    /* [in] */ Int64 nativeDateTimeChooserAndroid)
{
    Elastos_DateTimeChooserAndroid_nativeCancelDialog(THIS_PROBE(IInterface), (Handle32)nativeDateTimeChooserAndroid);
}

AutoPtr<IInterface> DateTimeChooserAndroid::CreateDateTimeChooser(
    /* [in] */ IInterface* contentViewCore,
    /* [in] */ Int64 nativeDateTimeChooserAndroid,
    /* [in] */ Int32 dialogType,
    /* [in] */ Double dialogValue,
    /* [in] */ Double min,
    /* [in] */ Double max,
    /* [in] */ Double step,
    /* [in] */ ArrayOf<IInterface*>* suggestions)
{
    AutoPtr<ContentViewCore> cvc = (ContentViewCore*)(IObject::Probe(contentViewCore));
    return TO_IINTERFACE(CreateDateTimeChooser(cvc, nativeDateTimeChooserAndroid,
                dialogType, dialogValue, min, max, step, suggestions));
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
