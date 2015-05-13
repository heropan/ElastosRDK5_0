
#ifndef __CPREFERENCEACTIVITYHEADER_H__
#define __CPREFERENCEACTIVITYHEADER_H__

#include "_CPreferenceActivityHeader.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CPreferenceActivityHeader)
{
public:
    CPreferenceActivityHeader();

    CARAPI constructor();

    /**
     * Return the currently set title.  If {@link #titleRes} is set,
     * this resource is loaded from <var>res</var> and returned.  Otherwise
     * {@link #title} is returned.
     */
    CARAPI GetTitleEx(
        /* [in] */ IResources* res,
        /* [out] */ ICharSequence** title);

    /**
     * Return the currently set summary.  If {@link #summaryRes} is set,
     * this resource is loaded from <var>res</var> and returned.  Otherwise
     * {@link #summary} is returned.
     */
    CARAPI GetSummaryEx(
        /* [in] */ IResources* res,
        /* [out] */ ICharSequence** summary);

    /**
     * Return the currently set bread crumb title.  If {@link #breadCrumbTitleRes} is set,
     * this resource is loaded from <var>res</var> and returned.  Otherwise
     * {@link #breadCrumbTitle} is returned.
     */
    CARAPI GetBreadCrumbTitleEx(
        /* [in] */ IResources* res,
        /* [out] */ ICharSequence** title);

    /**
     * Return the currently set bread crumb short title.  If
     * {@link #breadCrumbShortTitleRes} is set,
     * this resource is loaded from <var>res</var> and returned.  Otherwise
     * {@link #breadCrumbShortTitle} is returned.
     */
    CARAPI GetBreadCrumbShortTitleEx(
        /* [in] */ IResources* res,
        /* [out] */ ICharSequence** title);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI GetId(
        /* [out] */ Int64* id);

    CARAPI SetId(
        /* [in] */ Int64 id);

    CARAPI GetTitleRes(
        /* [out] */ Int32* titleRes);

    CARAPI SetTitleRes(
        /* [in] */ Int32 titleRes);

    CARAPI GetTitle(
        /* [out] */ ICharSequence** title);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI GetSummaryRes(
        /* [out] */ Int32* summaryRes);

    CARAPI SetSummaryRes(
        /* [in] */ Int32 summaryRes);

    CARAPI GetSummary(
        /* [out] */ ICharSequence** summary);

    CARAPI SetSummary(
        /* [in] */ ICharSequence* summary);

    CARAPI GetBreadCrumbTitleRes(
        /* [out] */ Int32* breadCrumbTitleRes);

    CARAPI SetBreadCrumbTitleRes(
        /* [in] */ Int32 breadCrumbTitleRes);

    CARAPI GetBreadCrumbTitle(
        /* [out] */ ICharSequence** breadCrumbTitle);

    CARAPI SetBreadCrumbTitle(
        /* [in] */ ICharSequence* breadCrumbTitle);

    CARAPI GetBreadCrumbShortTitleRes(
        /* [out] */ Int32* breadCrumbShortTitleRes);

    CARAPI SetBreadCrumbShortTitleRes(
        /* [in] */ Int32 breadCrumbShortTitleRes);

    CARAPI GetBreadCrumbShortTitle(
        /* [out] */ ICharSequence** breadCrumbShortTitle);

    CARAPI SetBreadCrumbShortTitle(
        /* [in] */ ICharSequence* breadCrumbShortTitle);

    CARAPI GetIconRes(
        /* [out] */ Int32* iconRes);

    CARAPI SetIconRes(
        /* [in] */ Int32 iconRes);

    CARAPI GetFragment(
        /* [out] */ String* fragment);

    CARAPI SetFragment(
        /* [in] */ const String& fragment);

    CARAPI GetFragmentArguments(
        /* [out] */ IBundle** fragmentArguments);

    CARAPI SetFragmentArguments(
        /* [in] */ IBundle* fragmentArguments);

    CARAPI GetIntent(
        /* [out] */ IIntent** intent);

    CARAPI SetIntent(
        /* [in] */ IIntent* intent);

    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    CARAPI SetExtras(
        /* [in] */ IBundle* extras);

public:
    /**
     * Identifier for this header, to correlate with a new list when
     * it is updated.  The default value is
     * {@link PreferenceActivity#HEADER_ID_UNDEFINED}, meaning no id.
     * @attr ref android.R.styleable#PreferenceHeader_id
     */
    Int64 mId;

    /**
     * Resource ID of title of the header that is shown to the user.
     * @attr ref android.R.styleable#PreferenceHeader_title
     */
    Int32 mTitleRes;

    /**
     * Title of the header that is shown to the user.
     * @attr ref android.R.styleable#PreferenceHeader_title
     */
    AutoPtr<ICharSequence> mTitle;

    /**
     * Resource ID of optional summary describing what this header controls.
     * @attr ref android.R.styleable#PreferenceHeader_summary
     */
    Int32 mSummaryRes;

    /**
     * Optional summary describing what this header controls.
     * @attr ref android.R.styleable#PreferenceHeader_summary
     */
    AutoPtr<ICharSequence> mSummary;

    /**
     * Resource ID of optional text to show as the title in the bread crumb.
     * @attr ref android.R.styleable#PreferenceHeader_breadCrumbTitle
     */
    Int32 mBreadCrumbTitleRes;

    /**
     * Optional text to show as the title in the bread crumb.
     * @attr ref android.R.styleable#PreferenceHeader_breadCrumbTitle
     */
    AutoPtr<ICharSequence> mBreadCrumbTitle;

    /**
     * Resource ID of optional text to show as the short title in the bread crumb.
     * @attr ref android.R.styleable#PreferenceHeader_breadCrumbShortTitle
     */
    Int32 mBreadCrumbShortTitleRes;

    /**
     * Optional text to show as the short title in the bread crumb.
     * @attr ref android.R.styleable#PreferenceHeader_breadCrumbShortTitle
     */
    AutoPtr<ICharSequence> mBreadCrumbShortTitle;

    /**
     * Optional icon resource to show for this header.
     * @attr ref android.R.styleable#PreferenceHeader_icon
     */
    Int32 mIconRes;

    /**
     * Full class name of the fragment to display when this header is
     * selected.
     * @attr ref android.R.styleable#PreferenceHeader_fragment
     */
    String mFragment;

    /**
     * Optional arguments to supply to the fragment when it is
     * instantiated.
     */
    AutoPtr<IBundle> mFragmentArguments;

    /**
     * Intent to launch when the preference is selected.
     */
    AutoPtr<IIntent> mIntent;

    /**
     * Optional additional data for use by subclasses of PreferenceActivity.
     */
    AutoPtr<IBundle> mExtras;

};

}
}
}

#endif // __CPREFERENCEACTIVITYHEADER_H__
