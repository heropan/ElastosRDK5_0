#include "elastos/droid/app/CNotificationBigTextStyle.h"
#include "elastos/droid/R.h"

using Elastos::Droid::R;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace App {

const Int32 CNotificationBigTextStyle::MAX_LINES = 13;
const Int32 CNotificationBigTextStyle::LINES_CONSUMED_BY_ACTIONS = 3;
const Int32 CNotificationBigTextStyle::LINES_CONSUMED_BY_SUMMARY = 2;

CNotificationBigTextStyle::CNotificationBigTextStyle()
{
}

CNotificationBigTextStyle::~CNotificationBigTextStyle()
{
}

ECode CNotificationBigTextStyle::constructor()
{
    return NOERROR;
}

ECode CNotificationBigTextStyle::constructor(
    /* [in] */ INotificationBuilder* builder)
{
    return NotificationStyle::SetBuilder(builder);
}

ECode CNotificationBigTextStyle::SetBuilder(
    /* [in] */ INotificationBuilder* builder)
{
    return NotificationStyle::SetBuilder(builder);
}

/**
 * Overrides ContentTitle in the big form of the template.
 * This defaults to the value passed to setContentTitle().
 */
ECode CNotificationBigTextStyle::SetBigContentTitle(
    /* [in] */ ICharSequence* title)
{
    InternalSetBigContentTitle(SafeCharSequence(title));
    return NOERROR;
}

/**
 * Set the first line of text after the detail section in the big form of the template.
 */
ECode CNotificationBigTextStyle::SetSummaryText(
    /* [in] */ ICharSequence* cs)
{
    InternalSetSummaryText(SafeCharSequence(cs));
    return NOERROR;
}

/**
 * Provide the longer text to be displayed in the big form of the
 * template in place of the content text.
 */
ECode CNotificationBigTextStyle::BigText(
    /* [in] */ ICharSequence* cs)
{
    mBigText = SafeCharSequence(cs);
    return NOERROR;
}

/**
 * @hide
 */
ECode CNotificationBigTextStyle::AddExtras(
    /* [in] */ IBundle* extras)
{
    super.addExtras(extras);

    extras.putCharSequence(EXTRA_BIG_TEXT, mBigText);
    return NOERROR;
}

/**
 * @hide
 */
ECode CNotificationBigTextStyle::RestoreFromExtras(
    /* [in] */ IBundle* extras)
{
    super.restoreFromExtras(extras);

    mBigText = extras.getCharSequence(EXTRA_BIG_TEXT);
    return NOERROR;
}

AutoPtr<IRemoteViews> CNotificationBigTextStyle::MakeBigContentView()
{
    assert(mBuilder != NULL);

    // Nasty
    CharSequence oldBuilderContentText = mBuilder.mContentText;
    mBuilder.mContentText = null;

    RemoteViews contentView = getStandardView(mBuilder.getBigTextLayoutResource());

    mBuilder.mContentText = oldBuilderContentText;

    contentView.setTextViewText(R.id.big_text, mBuilder.processLegacyText(mBigText));
    contentView.setViewVisibility(R.id.big_text, View.VISIBLE);
    contentView.setInt(R.id.big_text, "setMaxLines", calculateMaxLines());
    contentView.setViewVisibility(R.id.text2, View.GONE);

    applyTopPadding(contentView);

    mBuilder.shrinkLine3Text(contentView);

    mBuilder.addProfileBadge(contentView, R.id.profile_badge_large_template);

    return contentView;

    // // Remove the content text so line3 only shows if you have a summary
    // Boolean hadThreeLines = TRUE;

    // AutoPtr<ICharSequence> tmpSeq;
    // mBuilder->GetContentText((ICharSequence**)&tmpSeq);
    // if (tmpSeq == NULL) hadThreeLines = FALSE;

    // if (hadThreeLines) {
    //     tmpSeq = NULL;
    //     mBuilder->GetSubText((ICharSequence**)&tmpSeq);
    //     if (tmpSeq == NULL) hadThreeLines = FALSE;
    // }

    // mBuilder->SetContentText(NULL);

    // AutoPtr<IRemoteViews> contentView = GetStandardView(R::layout::notification_template_big_text);
    // if (contentView) {
    //     if (hadThreeLines) {
    //         // vertical centering
    //         contentView->SetViewPadding(R::id::line1, 0, 0, 0, 0);
    //     }

    //     contentView->SetTextViewText(R::id::big_text, mBigText);
    //     contentView->SetViewVisibility(R::id::big_text, IView::VISIBLE);
    //     contentView->SetViewVisibility(R::id::text2, IView::GONE);
    // }

    // return contentView;
}

Int32 CNotificationBigTextStyle::CalculateMaxLines()
{
    int lineCount = MAX_LINES;
    boolean hasActions = mBuilder.mActions.size() > 0;
    boolean hasSummary = (mSummaryTextSet ? mSummaryText : mBuilder.mSubText) != null;
    if (hasActions) {
        lineCount -= LINES_CONSUMED_BY_ACTIONS;
    }
    if (hasSummary) {
        lineCount -= LINES_CONSUMED_BY_SUMMARY;
    }

    // If we have less top padding at the top, we can fit less lines.
    if (!mBuilder.mHasThreeLines) {
        lineCount--;
    }
    return lineCount;
}

CARAPI CNotificationBigTextStyle::PopulateBigContentView(
    /* [in] */ INotification* wip)
{
    mBuilder.setBuilderBigContentView(wip, makeBigContentView());
}

} // namespace App
} // namespace Droid
} // namespace Elastos
