
#include "elastos/droid/app/CNotificationMediaStyle.h"

const Int32 CNotificationMediaStyle::MAX_MEDIA_BUTTONS_IN_COMPACT = 3;
const Int32 CNotificationMediaStyle::MAX_MEDIA_BUTTONS = 5;

CAR_INTERFACE_IMPL(CNotificationMediaStyle, NotificationStyle, INotificationMediaStyle)

CAR_OBJECT_IMPL(CNotificationMediaStyle)

CNotificationMediaStyle::CNotificationMediaStyle()
{}

CNotificationMediaStyle::~CNotificationMediaStyle()
{}

ECode CNotificationMediaStyle::constructor()
{
    return NOERROR;
}

ECode CNotificationMediaStyle::constructor(
    /* [in] */ INotificationBuilder* builder)
{
    NotificationStyle::SetBuilder(builder);
    return NOERROR;
}

ECode CNotificationMediaStyle::SetShowActionsInCompactView(
    /* [in] */ ArrayOf<Int32>* actions)
{
    mActionsToShowInCompact = actions;
    return NOERROR;
}

ECode CNotificationMediaStyle::SetMediaSession(
    /* [in] */ IMediaSessionToken* token)
{
    mToken = token;
    return NOERROR;
}

ECode CNotificationMediaStyle::BuildStyled(
    /* [in] */ INotification* wip,
    /* [out] */ INotification** result)
{
    VALIDATION(result)
    NotificationStyle::BuildStyled(wip);
    String category;
    wip->GetCategory(&category);
    if (category.IsNull()) {
        wip->SetCategory(INotification::CATEGORY_TRANSPORT);
    }
    return wip;
}

ECode CNotificationMediaStyle::PopulateContentView(
    /* [in] */ INotification* wip)
{
    return mBuilder->SetBuilderContentView(wip, MakeMediaContentView());
}

ECode CNotificationMediaStyle::PopulateBigContentView(
    /* [in] */ INotification* wip)
{
    return mBuilder->SetBuilderBigContentView(wip, MakeMediaBigContentView());
}

ECode CNotificationMediaStyle::AddExtras(
    /* [in] */ IBundle* extras)
{
    NotificationStyle::AddExtras(extras);

    if (mToken != NULL) {
        extras->PutParcelable(EXTRA_MEDIA_SESSION, mToken);
    }
    if (mActionsToShowInCompact != NULL) {
        extras->PutInt32Array(EXTRA_COMPACT_ACTIONS, mActionsToShowInCompact);
    }
    return NOERROR;
}

ECode CNotificationMediaStyle::RestoreFromExtras(
    /* [in] */ IBundle* extras)
{
    NotificationStyle::RestoreFromExtras(extras);

    Boolean bval;
    extras->ContainsKey(EXTRA_MEDIA_SESSION, &bval);
    if (bval) {
        mToken = NULL;
        extras->GetParcelable(EXTRA_MEDIA_SESSION, &mToken);
    }
    extras->ContainsKey(EXTRA_COMPACT_ACTIONS, &bval);
    if (bval) {
        mActionsToShowInCompact = NULL;
        extras->GetInt32Array(EXTRA_COMPACT_ACTIONS, (ArrayOf<Int32>**)&mActionsToShowInCompact);
    }
    return NOERROR;
}

Boolean CNotificationMediaStyle::HasProgress()
{
    return FALSE;
}

AutoPtr<IRemoteViews> CNotificationMediaStyle::GenerateMediaActionButton(
    /* [in] */ INotificationAction* action)
{
    AutoPtr<IPendingIntent> pi;
    action->GetActionIntent((IPendingIntent**)&pi);
    Int32 icon;
    action->GetIcon(&icon);
    AutoPtr<ICharSequence> title;
    action->GetTitle((ICharSequence**)&title);

    Boolean tombstone = (pi == NULL);

    CNotificationBuilder* cb = (CNotificationBuilder*)mBuilder->Get();
    String pkgName;
    cb->mContext->GetPackageName(&pkgName);

    AutoPtr<IRemoteViews> button;
    assert(0 && "TODO");
    // CRemoteViews::New(pkgName, R::layout::notification_material_media_action, (IRemoteViews**)&button);
    button->SetImageViewResource(R::id::action0, icon);
    button->SetDrawableParameters(R::id::action0, FALSE, -1,
        0xFFFFFFFF, PorterDuffMode_SRC_ATOP, -1);
    if (!tombstone) {
        button->SetOnClickPendingIntent(R::id::action0, pi);
    }
    button->SetContentDescription(R::id::action0, title);
    return button;
}

AutoPtr<IRemoteViews> CNotificationMediaStyle::MakeMediaContentView()
{
    CNotificationBuilder* cb = (CNotificationBuilder*)mBuilder->Get();
    AutoPtr<IRemoteViews> view;
    mBuilder->ApplyStandardTemplate(
        R::layout::notification_template_material_media, FALSE /* hasProgress */,
        (IRemoteViews**)&view);

    using Elastos::Core::Math;

    Int32 numActions = cb->mActions.GetSize();
    Int32 N = mActionsToShowInCompact == NULL
            ? 0
            : Math::Min(mActionsToShowInCompact->GetLength(), MAX_MEDIA_BUTTONS_IN_COMPACT);
    if (N > 0) {
        view->RemoveAllViews(R::id::media_actions);
        for (Int32 i = 0; i < N; i++) {
            if (i >= numActions) {
                Logger::E("CNotificationMediaStyle",
                    "setShowActionsInCompactView: action %d out of bounds (max %d)",
                    i, numActions - 1);
                return E_ILLEGAL_ARGUMENT_EXCEPTION;
            }

            AutoPtr<INotificationAction> action = cb->mActions[(*mActionsToShowInCompact)[i]];
            AutoPtr<IRemoteViews> button = GenerateMediaActionButton(action);
            view->AddView(R::id::media_actions, button);
        }
    }
    StyleText(view);
    HideRightIcon(view);
    return view;
}

AutoPtr<IRemoteViews> CNotificationMediaStyle::MakeMediaBigContentView()
{
    CNotificationBuilder* cb = (CNotificationBuilder*)mBuilder->Get();
    Int32 actionCount = Math::Min(cb->mActions.GetSize(), MAX_MEDIA_BUTTONS);
    AutoPtr<IRemoteViews> big;
    mBuilder->ApplyStandardTemplate(GetBigLayoutResource(actionCount),
            FALSE /* hasProgress */, (IRemoteViews**)&big);

    if (actionCount > 0) {
        big->RemoveAllViews(R::id::media_actions);
        for (Int32 i = 0; i < actionCount; i++) {
            AutoPtr<IRemoteViews> button = GenerateMediaActionButton(cb->mActions[i]);
            big->AddView(R::id::media_actions, button);
        }
    }
    StyleText(big);
    HideRightIcon(big);
    ApplyTopPadding(big);
    big->SetViewVisibility(android.R::id::progress, IView::GONE);
    return big;
}

Int32 CNotificationMediaStyle::GetBigLayoutResource(
    /* [in] */ Int32 actionCount)
{
    if (actionCount <= 3) {
        return R::layout::notification_template_material_big_media_narrow;
    } else {
        return R::layout::notification_template_material_big_media;
    }
}

void CNotificationMediaStyle::HideRightIcon(
    /* [in] */ IRemoteViews* contentView)
{
    contentView->SetViewVisibility(R::id::right_icon, IView::GONE);
}

void CNotificationMediaStyle::StyleText(
    /* [in] */ IRemoteViews* contentView)
{
    CNotificationBuilder* cb = (CNotificationBuilder*)mBuilder->Get();
    AutoPtr<IResources> res;
    cb->mContext->GetResources((IResources**)&res);
    Int32 primaryColor, secondaryColor;
    res->GetColor(R::color::notification_media_primary_color, &primaryColor);
    res->GetColor(R::color::notification_media_secondary_color, &secondaryColor);
    contentView->SetTextColor(R::id::title, primaryColor);
    if (mBuilder->ShowsTimeOrChronometer()) {
        if (mBuilder->mUseChronometer) {
            contentView->SetTextColor(R::id::chronometer, secondaryColor);
        } else {
            contentView->SetTextColor(R::id::time, secondaryColor);
        }
    }
    contentView->SetTextColor(R::id::text2, secondaryColor);
    contentView->SetTextColor(R::id::text, secondaryColor);
    contentView->SetTextColor(R::id::info, secondaryColor);
}

} // namespace App
} // namespace Droid
} // namespace Elastos
