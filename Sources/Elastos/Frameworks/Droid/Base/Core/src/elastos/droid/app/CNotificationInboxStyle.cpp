#include "app/CNotificationInboxStyle.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace App {

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

PInterface CNotificationInboxStyle::Probe(
    /* [in] */ REIID riid)
{
    return _CNotificationInboxStyle::Probe(riid);
}

ECode CNotificationInboxStyle::SetBuilder(
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
    InternalSetBigContentTitle(title);
    return NOERROR;
}

ECode CNotificationInboxStyle::SetSummaryText(
    /* [in] */ ICharSequence* cs)
{
    InternalSetSummaryText(cs);
    return NOERROR;
}

ECode CNotificationInboxStyle::AddLine(
    /* [in] */ ICharSequence* cs)
{
    AutoPtr<ICharSequence> obj = cs;
    mTexts.PushBack(obj);
    return NOERROR;
}

AutoPtr<IRemoteViews> CNotificationInboxStyle::MakeBigContentView()
{
    assert(mBuilder != NULL);

    // Remove the content text so line3 disappears unless you have a summary
    mBuilder->SetContentText(NULL);
    AutoPtr<IRemoteViews> contentView = GetStandardView(R::layout::notification_template_inbox);
    if (contentView == NULL) return NULL;

    contentView->SetViewVisibility(R::id::text2, IView::GONE);

    const Int32 idsSize = 7;
    Int32 rowIds[7] = {
        R::id::inbox_text0, R::id::inbox_text1, R::id::inbox_text2,
        R::id::inbox_text3, R::id::inbox_text4, R::id::inbox_text5,
        R::id::inbox_text6};

    // Make sure all rows are gone in case we reuse a view.
    for (Int32 i = 0; i < 7; ++i) {
        contentView->SetViewVisibility(rowIds[i], IView::GONE);
    }

    Int32 textSize = mTexts.GetSize();
    Int32 i = 0;
    AutoPtr<ICharSequence> str;
    String seqStr;
    List<AutoPtr<ICharSequence> >::Iterator it = mTexts.Begin();
    for (; it != mTexts.End() && i < idsSize; ++i, ++it) {
        str = *it;
        if (str != NULL) {
            str->ToString(&seqStr);
            if (!seqStr.IsNullOrEmpty()) {
                contentView->SetViewVisibility(rowIds[i], IView::VISIBLE);
                contentView->SetTextViewText(rowIds[i], str);
            }
        }
    }

    contentView->SetViewVisibility(R::id::inbox_end_pad,
        textSize > 0 ? IView::VISIBLE : IView::GONE);

    contentView->SetViewVisibility(R::id::inbox_more,
        textSize > idsSize ? IView::VISIBLE : IView::GONE);

    return contentView;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
