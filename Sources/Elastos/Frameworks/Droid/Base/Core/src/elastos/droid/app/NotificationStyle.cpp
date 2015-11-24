#include "elastos/droid/app/NotificationStyle.h"
#include "elastos/droid/app/CNotificationBuilder.h"
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/R.h"

using Elastos::Droid::View::IView;
using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace App {

CAR_INTERFACE_IMPL(NotificationStyle, Object, INotificationStyle)

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
            mBuilder->SetStyle((INotificationStyle*)this);
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

    // Nasty.
    AutoPtr<ICharSequence> oldBuilderContentTitle;
    mBuilder->GetContentTitle((ICharSequence**)&oldBuilderContentTitle);

    if (mBigContentTitle != NULL) {
        mBuilder->SetContentTitle(mBigContentTitle);
    }

    AutoPtr<IRemoteViews> contentView;
    mBuilder->ApplyStandardTemplateWithActions(layoutId, (IRemoteViews**)&contentView);

    mBuilder->SetContentTitle(oldBuilderContentTitle);

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
        contentView->SetTextViewText(R::id::text, mBuilder->ProcessLegacyText(overflowText));
        contentView->SetViewVisibility(R::id::overflow_divider, IView::VISIBLE);
        contentView->SetViewVisibility(R::id::line3, IView::VISIBLE);
    }
    else {
        // Clear text in case we use the line to show the profile badge.
        contentView->SetTextViewText(R::id::text, String(""));
        contentView->SetViewVisibility(R::id::overflow_divider, IView::GONE);
        contentView->SetViewVisibility(R::id::line3, IView::GONE);
    }

    return contentView;
}

ECode NotificationStyle::ApplyTopPadding(
    /* [in] */ IRemoteViews* contentView)
{
    AutoPtr<IResources> res;
    mBuilder->mContext->GetResources((IResources**)&res);
    AutoPtr<IConfiguration> cfg;
    res->GetConfiguration((IConfiguration**)&cfg)
    Float fontScale;
    cfg->GetFontScale(&fontScale);
    Int32 topPadding = CNotificationBuilder::CalculateTopPadding(
        mBuilder->mContext,
        mBuilder->mHasThreeLines, fontScale);
    return contentView->SetViewPadding(R::id::line1, 0, topPadding, 0, 0);
}

ECode NotificationStyle::AddExtras(
    /* [in] */ IBundle* extras)
{
    if (mSummaryTextSet) {
        extras->PutCharSequence(EXTRA_SUMMARY_TEXT, mSummaryText);
    }
    if (mBigContentTitle != null) {
        extras->PutCharSequence(EXTRA_TITLE_BIG, mBigContentTitle);
    }
    extras->PutString(EXTRA_TEMPLATE, String("CNotificationStyle"));
    return NOERROR;
}

ECode NotificationStyle::RestoreFromExtras(
    /* [in] */ IBundle* extras)
{
    Boolean bval;
    extras->ContainsKey(EXTRA_SUMMARY_TEXT, &bval);
    if (bval) {
        mSummaryText = NULL;
        extras->GetCharSequence(EXTRA_SUMMARY_TEXT, (ICharSequence**)&mSummaryText);
        mSummaryTextSet = TRUE;
    }
    extras->ContainsKey(EXTRA_TITLE_BIG, &bval)
    if (bval)) {
        mBigContentTitle = NULL;
        extras->GetCharSequence(EXTRA_TITLE_BIG, (ICharSequence**)&mBigContentTitle);
    }
    return NOERROR;
}

AutoPtr<INotification> NotificationStyle::BuildStyled(
    /* [in] */ INotification* wip)
{
    PopulateTickerView(wip);
    PopulateContentView(wip);
    PopulateBigContentView(wip);
    PopulateHeadsUpContentView(wip);
    return wip;
}

ECode NotificationStyle::PopulateTickerView(
    /* [in] */ INotification* wip)
{
    return NOERROR;
}

ECode NotificationStyle::PopulateContentView(
    /* [in] */ INotification* wip)
{
    return NOERROR;
}

ECode NotificationStyle::PopulateBigContentView(
    /* [in] */ INotification* wip)
{
    return NOERROR;
}

ECode NotificationStyle::PopulateHeadsUpContentView(
    /* [in] */ INotification* wip)
{
    return NOERROR;
}

ECode NotificationStyle::Build(
    /* [out] */ INotification** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;
    FAIL_RETURN(CheckBuilder())
    return mBuilder->Build();
}

Boolean NotificationStyle::HasProgress()
{
    return TRUE;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
