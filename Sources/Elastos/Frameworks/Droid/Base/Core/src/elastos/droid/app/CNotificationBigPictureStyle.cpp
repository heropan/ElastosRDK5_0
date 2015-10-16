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

PInterface CNotificationBigPictureStyle::Probe(
    /* [in] */ REIID riid)
{
    return _CNotificationBigPictureStyle::Probe(riid);
}

ECode CNotificationBigPictureStyle::SetBuilder(
    /* [in] */ INotificationBuilder* builder)
{
    return NotificationStyle::SetBuilder(builder);
}

ECode CNotificationBigPictureStyle::Build(
    /* [out] */ INotification** notification)
{
    VALIDATE_NOT_NULL(notification);
    *notification = NULL;

    FAIL_RETURN(CheckBuilder())

    AutoPtr<INotification> wip;
    FAIL_RETURN(mBuilder->BuildUnstyled((INotification**)&wip));

    if (mBigLargeIconSet ) {
        mBuilder->SetLargeIcon(mBigLargeIcon);
    }

    AutoPtr<IRemoteViews> rv = MakeBigContentView();
    wip->SetBigContentView(rv);

    *notification = wip;
    REFCOUNT_ADD(*notification);
    return NOERROR;
}

ECode CNotificationBigPictureStyle::SetBigContentTitle(
    /* [in] */ ICharSequence* title)
{
    InternalSetBigContentTitle(title);
    return NOERROR;
}

ECode CNotificationBigPictureStyle::SetSummaryText(
    /* [in] */ ICharSequence* cs)
{
    InternalSetSummaryText(cs);
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
    AutoPtr<IRemoteViews> contentView = GetStandardView(R::layout::notification_template_big_picture);
    if (contentView) {
        contentView->SetImageViewBitmap(R::id::big_picture, mPicture);
    }
    return contentView;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
