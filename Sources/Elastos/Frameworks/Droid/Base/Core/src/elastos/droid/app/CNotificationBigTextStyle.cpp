#include "elastos/droid/app/CNotificationBigTextStyle.h"
#include "elastos/droid/R.h"

using Elastos::Droid::R;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace App {

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

PInterface CNotificationBigTextStyle::Probe(
    /* [in] */ REIID riid)
{
    return _CNotificationBigTextStyle::Probe(riid);
}

ECode CNotificationBigTextStyle::SetBuilder(
    /* [in] */ INotificationBuilder* builder)
{
    return NotificationStyle::SetBuilder(builder);
}

ECode CNotificationBigTextStyle::Build(
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

/**
 * Overrides ContentTitle in the big form of the template.
 * This defaults to the value passed to setContentTitle().
 */
ECode CNotificationBigTextStyle::SetBigContentTitle(
    /* [in] */ ICharSequence* title)
{
    InternalSetBigContentTitle(title);
    return NOERROR;
}

/**
 * Set the first line of text after the detail section in the big form of the template.
 */
ECode CNotificationBigTextStyle::SetSummaryText(
    /* [in] */ ICharSequence* cs)
{
    InternalSetSummaryText(cs);
    return NOERROR;
}

/**
 * Provide the longer text to be displayed in the big form of the
 * template in place of the content text.
 */
ECode CNotificationBigTextStyle::BigText(
    /* [in] */ ICharSequence* cs)
{
    mBigText = cs;
    return NOERROR;
}

AutoPtr<IRemoteViews> CNotificationBigTextStyle::MakeBigContentView()
{
    assert(mBuilder != NULL);

    // Remove the content text so line3 only shows if you have a summary
    Boolean hadThreeLines = TRUE;

    AutoPtr<ICharSequence> tmpSeq;
    mBuilder->GetContentText((ICharSequence**)&tmpSeq);
    if (tmpSeq == NULL) hadThreeLines = FALSE;

    if (hadThreeLines) {
        tmpSeq = NULL;
        mBuilder->GetSubText((ICharSequence**)&tmpSeq);
        if (tmpSeq == NULL) hadThreeLines = FALSE;
    }

    mBuilder->SetContentText(NULL);

    AutoPtr<IRemoteViews> contentView = GetStandardView(R::layout::notification_template_big_text);
    if (contentView) {
        if (hadThreeLines) {
            // vertical centering
            contentView->SetViewPadding(R::id::line1, 0, 0, 0, 0);
        }

        contentView->SetTextViewText(R::id::big_text, mBigText);
        contentView->SetViewVisibility(R::id::big_text, IView::VISIBLE);
        contentView->SetViewVisibility(R::id::text2, IView::GONE);
    }

    return contentView;
}


} // namespace App
} // namespace Droid
} // namespace Elastos
