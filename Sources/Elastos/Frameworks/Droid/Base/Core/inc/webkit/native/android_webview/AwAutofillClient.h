
#ifndef __AWAUTOFILLCLIENT_H__
#define __AWAUTOFILLCLIENT_H__

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.content.browser.ContentViewCore;
// import org.chromium.ui.autofill.AutofillPopup;
// import org.chromium.ui.autofill.AutofillSuggestion;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * Java counterpart to the AwAutofillClient. This class is owned by AwContents and has
 * a weak reference from native side.
 */
//@JNINamespace("android_webview")
class AwAutofillClient
{
private:
    class InnerAutofillPopupDelegate : public AutofillPopupDelegate
    {
    public:
        InnerAutofillPopupDelegate(
            /* [in] */ AwAutofillClient* owner);

        //@Override
        CARAPI_(void) RequestHide();

        //@Override
        CARAPI_(void) SuggestionSelected(
            /* [in] */ Int32 listIndex);

    private:
        AwAutofillClient* mOwner;
    };

public:
    //@CalledByNative
    static CARAPI_(AutoPtr<AwAutofillClient>) Create(
        /* [in] */ Int64 nativeClient);

    virtual CARAPI_(void) Init(
        /* [in] */ ContentViewCore* contentViewCore);

    //@CalledByNative
    virtual CARAPI_(void) HideAutofillPopup();

private:
    AwAutofillClient(
        /* [in] */ Int64 nativeAwAutofillClient);

    //@CalledByNative
    CARAPI_(void) ShowAutofillPopup(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float width,
        /* [in] */ Float height,
        /* [in] */ ArrayOf<AutofillSuggestion>* suggestions);

    //@CalledByNative
    static CARAPI_(AutoPtr< ArrayOf<AutofillSuggestion> >) CreateAutofillSuggestionArray(
        /* [in] */ Int32 size);

    /**
     * @param array AutofillSuggestion array that should get a new suggestion added.
     * @param index Index in the array where to place a new suggestion.
     * @param name Name of the suggestion.
     * @param label Label of the suggestion.
     * @param uniqueId Unique suggestion id.
     */
    //@CalledByNative
    static CARAPI_(void) AddToAutofillSuggestionArray(
        /* [in] */ ArrayOf<AutofillSuggestion>* array,
        /* [in] */ Int32 index,
        /* [in] */ String name,
        /* [in] */ String label,
        /* [in] */ Int32 uniqueId);

    CARAPI_(void) NativeSuggestionSelected(
        /* [in] */ Int64 nativeAwAutofillClient,
        /* [in] */ Int32 position);

private:
    const Int64 mNativeAwAutofillClient;
    AutoPtr<AutofillPopup> mAutofillPopup;
    AutoPtr<IViewGroup> mContainerView;
    AutoPtr<ContentViewCore> mContentViewCore;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__AWAUTOFILLCLIENT_H__
