#include "elastos/droid/app/CNotificationInboxStyle.h"
#include "elastos/droid/R.h"

using Elastos::Droid::R;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace App {

CAR_INTERFACE_IMPL(CNotificationInboxStyle, NotificationStyle, INotificationInboxStyle)

CAR_OBJECT_IMPL(CNotificationInboxStyle)

CNotificationInboxStyle::CNotificationInboxStyle()
{
}

CNotificationInboxStyle::~CNotificationInboxStyle()
{
}

ECode CNotificationInboxStyle::constructor()
{
    return NOERROR;
}

ECode CNotificationInboxStyle::constructor(
    /* [in] */ INotificationBuilder* builder)
{
    return NotificationStyle::SetBuilder(builder);
}

ECode CNotificationInboxStyle::Build(
    /* [out] */ INotification** notification)
{
    VALIDATE_NOT_NULL(notification);
    *notification = NULL;

    FAIL_RETURN(CheckBuilder())

    AutoPtr<INotification> wip;
    FAIL_RETURN(mBuilder->BuildUnstyled((INotification**)&wip));
    AutoPtr<IRemoteViews> rv = MakeBigContentView();
    wip->SetBigContentView(rv);

    *notification = wip;
    REFCOUNT_ADD(*notification);
    return NOERROR;
}

ECode CNotificationInboxStyle::SetBigContentTitle(
    /* [in] */ ICharSequence* title)
{
    InternalSetBigContentTitle(SafeCharSequence(title));
    return NOERROR;
}

ECode CNotificationInboxStyle::SetSummaryText(
    /* [in] */ ICharSequence* cs)
{
    InternalSetSummaryText(SafeCharSequence(cs));
    return NOERROR;
}

ECode CNotificationInboxStyle::AddLine(
    /* [in] */ ICharSequence* cs)
{
    AutoPtr<ICharSequence> obj = SafeCharSequence(cs));
    mTexts.PushBack(obj);
    return NOERROR;
}

/**
 * @hide
 */
ECode CNotificationInboxStyle::AddExtras(
    /* [in] */ IBundle* extras)
{
    super.addExtras(extras);

    CharSequence[] a = new CharSequence[mTexts.size()];
    extras.putCharSequenceArray(EXTRA_TEXT_LINES, mTexts.toArray(a));
}

/**
 * @hide
 */
ECode CNotificationInboxStyle::RestoreFromExtras(
    /* [in] */ IBundle* extras)
{
    super.restoreFromExtras(extras);

    mTexts.clear();
    if (extras.containsKey(EXTRA_TEXT_LINES)) {
        Collections.addAll(mTexts, extras.getCharSequenceArray(EXTRA_TEXT_LINES));
    }
}

AutoPtr<IRemoteViews> CNotificationInboxStyle::MakeBigContentView()
{
    // Remove the content text so line3 disappears unless you have a summary

    // Nasty
    CharSequence oldBuilderContentText = mBuilder.mContentText;
    mBuilder.mContentText = null;

    RemoteViews contentView = getStandardView(mBuilder.getInboxLayoutResource());

    mBuilder.mContentText = oldBuilderContentText;

    contentView.setViewVisibility(R.id.text2, View.GONE);

    int[] rowIds = {R.id.inbox_text0, R.id.inbox_text1, R.id.inbox_text2, R.id.inbox_text3,
            R.id.inbox_text4, R.id.inbox_text5, R.id.inbox_text6};

    // Make sure all rows are gone in case we reuse a view.
    for (int rowId : rowIds) {
        contentView.setViewVisibility(rowId, View.GONE);
    }

    final boolean largeText =
            mBuilder.mContext.getResources().getConfiguration().fontScale > 1f;
    final float subTextSize = mBuilder.mContext.getResources().getDimensionPixelSize(
            R.dimen.notification_subtext_size);
    int i=0;
    while (i < mTexts.size() && i < rowIds.length) {
        CharSequence str = mTexts.get(i);
        if (str != null && !str.equals("")) {
            contentView.setViewVisibility(rowIds[i], View.VISIBLE);
            contentView.setTextViewText(rowIds[i], mBuilder.processLegacyText(str));
            if (largeText) {
                contentView.setTextViewTextSize(rowIds[i], TypedValue.COMPLEX_UNIT_PX,
                        subTextSize);
            }
        }
        i++;
    }

    contentView.setViewVisibility(R.id.inbox_end_pad,
            mTexts.size() > 0 ? View.VISIBLE : View.GONE);

    contentView.setViewVisibility(R.id.inbox_more,
            mTexts.size() > rowIds.length ? View.VISIBLE : View.GONE);

    applyTopPadding(contentView);

    mBuilder.shrinkLine3Text(contentView);

    mBuilder.addProfileBadge(contentView, R.id.profile_badge_large_template);

    return contentView;

    // assert(mBuilder != NULL);

    // // Remove the content text so line3 disappears unless you have a summary
    // mBuilder->SetContentText(NULL);
    // AutoPtr<IRemoteViews> contentView = GetStandardView(R::layout::notification_template_inbox);
    // if (contentView == NULL) return NULL;

    // contentView->SetViewVisibility(R::id::text2, IView::GONE);

    // const Int32 idsSize = 7;
    // Int32 rowIds[7] = {
    //     R::id::inbox_text0, R::id::inbox_text1, R::id::inbox_text2,
    //     R::id::inbox_text3, R::id::inbox_text4, R::id::inbox_text5,
    //     R::id::inbox_text6};

    // // Make sure all rows are gone in case we reuse a view.
    // for (Int32 i = 0; i < 7; ++i) {
    //     contentView->SetViewVisibility(rowIds[i], IView::GONE);
    // }

    // Int32 textSize = mTexts.GetSize();
    // Int32 i = 0;
    // AutoPtr<ICharSequence> str;
    // String seqStr;
    // List<AutoPtr<ICharSequence> >::Iterator it = mTexts.Begin();
    // for (; it != mTexts.End() && i < idsSize; ++i, ++it) {
    //     str = *it;
    //     if (str != NULL) {
    //         str->ToString(&seqStr);
    //         if (!seqStr.IsNullOrEmpty()) {
    //             contentView->SetViewVisibility(rowIds[i], IView::VISIBLE);
    //             contentView->SetTextViewText(rowIds[i], str);
    //         }
    //     }
    // }

    // contentView->SetViewVisibility(R::id::inbox_end_pad,
    //     textSize > 0 ? IView::VISIBLE : IView::GONE);

    // contentView->SetViewVisibility(R::id::inbox_more,
    //     textSize > idsSize ? IView::VISIBLE : IView::GONE);

    // return contentView;
}


ECode CNotificationInboxStyle::PopulateBigContentView(
    /* [in] */ INotification* wip)
{
    mBuilder.setBuilderBigContentView(wip, makeBigContentView());
}

} // namespace App
} // namespace Droid
} // namespace Elastos
