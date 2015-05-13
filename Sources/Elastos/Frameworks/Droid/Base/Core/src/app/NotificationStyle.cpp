#include "app/NotificationStyle.h"
#include "ext/frameworkext.h"
#include <R.h>

using Elastos::Droid::View::IView;
using Elastos::Droid::R;

namespace Elastos{
namespace Droid{
namespace App{

NotificationStyle::NotificationStyle()
    : mSummaryTextSet(FALSE)
{
}

NotificationStyle::~NotificationStyle()
{
}

ECode NotificationStyle::SetBuilder(
    /* [in] */ INotificationBuilder *builder)
{
    if (mBuilder.Get() != builder) {
        mBuilder = builder;
        if (mBuilder != NULL) {
            mBuilder->SetStyle((INotificationStyle*)this->Probe(EIID_INotificationStyle));
        }
    }
    return NOERROR;
}
void NotificationStyle::InternalSetBigContentTitle(
    /* [in] */ ICharSequence *title)
{
    mBigContentTitle = title;
}

void NotificationStyle::InternalSetSummaryText(
    /* [in] */ ICharSequence *cs)
{
    mSummaryText = cs;
    mSummaryTextSet = TRUE;
}

ECode NotificationStyle::CheckBuilder()
{
    if (mBuilder == NULL) {
        //throw new IllegalArgumentException("Style requires a valid Builder object");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

AutoPtr<IRemoteViews> NotificationStyle::GetStandardView(
    /* [in] */ Int32 layoutId)
{
    if (FAILED(CheckBuilder()))
        return NULL;

    if (mBigContentTitle != NULL) {
        mBuilder->SetContentTitle(mBigContentTitle);
    }

    AutoPtr<IRemoteViews> contentView;
    mBuilder->ApplyStandardTemplateWithActions(layoutId, (IRemoteViews**)&contentView);

    Boolean visiable = TRUE;
    if (mBigContentTitle != NULL) {
        String str;
        mBigContentTitle->ToString(&str);
        visiable = str.Equals("");
    }

    contentView->SetViewVisibility(R::id::line1,
        visiable ? IView::VISIBLE : IView::GONE);

    // The last line defaults to the subtext, but can be replaced by mSummaryText
    AutoPtr<ICharSequence> overflowText;
    if (mSummaryTextSet){
        overflowText = mSummaryText;
    }
    else {
        mBuilder->GetSubText((ICharSequence**)&overflowText);
    }

    if (overflowText != NULL) {
        contentView->SetTextViewText(R::id::text, overflowText);
        contentView->SetViewVisibility(R::id::overflow_divider, IView::VISIBLE);
        contentView->SetViewVisibility(R::id::line3, IView::VISIBLE);
    }
    else {
        contentView->SetViewVisibility(R::id::overflow_divider, IView::GONE);
        contentView->SetViewVisibility(R::id::line3, IView::GONE);
    }

    return contentView;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
