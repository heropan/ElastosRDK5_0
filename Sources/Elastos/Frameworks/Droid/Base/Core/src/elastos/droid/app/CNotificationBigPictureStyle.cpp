#include "elastos/droid/app/CNotificationBigPictureStyle.h"
#include "elastos/droid/R.h"

using Elastos::Droid::R;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace App {

CNotificationBigPictureStyle::CNotificationBigPictureStyle()
    : mBigLargeIconSet(FALSE)
{
}

CNotificationBigPictureStyle::~CNotificationBigPictureStyle()
{
}

ECode CNotificationBigPictureStyle::constructor()
{
    return NOERROR;
}

ECode CNotificationBigPictureStyle::constructor(
    /* [in] */ INotificationBuilder* builder)
{
    return NotificationStyle::SetBuilder(builder);
}

ECode CNotificationBigPictureStyle::SetBuilder(
    /* [in] */ INotificationBuilder* builder)
{
    return NotificationStyle::SetBuilder(builder);
}

ECode CNotificationBigPictureStyle::SetBigContentTitle(
    /* [in] */ ICharSequence* title)
{
    InternalSetBigContentTitle(SafeCharSequence(title));
    return NOERROR;
}

ECode CNotificationBigPictureStyle::SetSummaryText(
    /* [in] */ ICharSequence* cs)
{
    InternalSetSummaryText(SafeCharSequence(cs));
    return NOERROR;
}

ECode CNotificationBigPictureStyle::BigPicture(
    /* [in] */ IBitmap* b)
{
    mPicture = b;
    return NOERROR;
}

ECode CNotificationBigPictureStyle::BigLargeIcon(
    /* [in] */ IBitmap* b)
{
    mBigLargeIconSet = true;
    mBigLargeIcon = b;
    return NOERROR;
}

AutoPtr<IRemoteViews> CNotificationBigPictureStyle::MakeBigContentView()
{
    // AutoPtr<IRemoteViews> contentView = GetStandardView(R::layout::notification_template_big_picture);
    // if (contentView) {
    //     contentView->SetImageViewBitmap(R::id::big_picture, mPicture);
    // }
    // return contentView;

    RemoteViews contentView = getStandardView(mBuilder.getBigPictureLayoutResource());

    contentView.setImageViewBitmap(R.id.big_picture, mPicture);

    applyTopPadding(contentView);

    boolean twoTextLines = mBuilder.mSubText != null && mBuilder.mContentText != null;
    mBuilder.addProfileBadge(contentView,
            twoTextLines ? R.id.profile_badge_line2 : R.id.profile_badge_line3);
    return contentView;
}

/**
 * @hide
 */
CARAPI AddExtras(
    /* [in] */ IBundle* extras)
{
    super.addExtras(extras);

    if (mBigLargeIconSet) {
        extras.putParcelable(EXTRA_LARGE_ICON_BIG, mBigLargeIcon);
    }
    extras.putParcelable(EXTRA_PICTURE, mPicture);
}

/**
 * @hide
 */
CARAPI RestoreFromExtras(
    /* [in] */ IBundle* extras)
{
    super.restoreFromExtras(extras);

    if (extras.containsKey(EXTRA_LARGE_ICON_BIG)) {
        mBigLargeIcon = extras.getParcelable(EXTRA_LARGE_ICON_BIG);
    }
    mPicture = extras.getParcelable(EXTRA_PICTURE);
}

/**
 * @hide
 */
CARAPI populateBigContentView(
    /* [in] */ INotification* wip)
{
    mBuilder.setBuilderBigContentView(wip, makeBigContentView());
}

} // namespace App
} // namespace Droid
} // namespace Elastos
