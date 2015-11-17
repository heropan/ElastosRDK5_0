
#include "elastos/droid/internal/view/menu/MenuItemImpl.h"
#include "elastos/droid/internal/view/menu/MenuBuilder.h"
#include "elastos/droid/internal/view/menu/CIconMenuItemView.h"
#include "elastos/droid/internal/view/ActionProvider.h"
#include "elastos/droid/internal/view/LayoutInflater.h"
#include <elastos/core/Character.h>
#include "elastos/droid/widget/CLinearLayout.h"
#include "elastos/droid/R.h"

using namespace Elastos::Core;
using Elastos::Droid::R;
using Elastos::Droid::View::EIID_IVisibilityListener;
using Elastos::Droid::Widget::CLinearLayout;
using Elastos::Droid::Widget::ILinearLayout;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {
namespace Menu {

String MenuItemImpl::TAG("MenuItemImpl");
const Int32 MenuItemImpl::SHOW_AS_ACTION_MASK = IMenuItem::SHOW_AS_ACTION_NEVER |
            IMenuItem::SHOW_AS_ACTION_IF_ROOM | IMenuItem::SHOW_AS_ACTION_ALWAYS;

const Int32 MenuItemImpl::CHECKABLE = 0x00000001;
const Int32 MenuItemImpl::CHECKED = 0x00000002;
const Int32 MenuItemImpl::EXCLUSIVE = 0x00000004;
const Int32 MenuItemImpl::HIDDEN = 0x00000008;
const Int32 MenuItemImpl::ENABLED = 0x00000010;
const Int32 MenuItemImpl::IS_ACTION = 0x00000020;

/** Used for the icon resource ID if this item does not have an icon */
const Int32 MenuItemImpl::NO_ICON = 0;

String MenuItemImpl::sPrependShortcutLabel;
String MenuItemImpl::sEnterShortcutLabel;
String MenuItemImpl::sDeleteShortcutLabel;
String MenuItemImpl::sSpaceShortcutLabel;

CAR_INTERFACE_IMPL(MenuItemImpl::VisibilityListener, IVisibilityListener)

MenuItemImpl::VisibilityListener::VisibilityListener(
    /* [in] */ MenuItemImpl* host)
    : mHost(host)
{}

ECode MenuItemImpl::VisibilityListener::OnActionProviderVisibilityChanged(
    /* [in] */ Boolean isVisible)
{
    mHost->mMenu->OnItemVisibleChanged((IMenuItemImpl*)(mHost->Probe(EIID_IMenuItemImpl)));
    return NOERROR;
}

MenuItemImpl::MenuItemImpl()
    : mShowAsAction(0)
    , mIsActionViewExpanded(FALSE)
    , mId(0)
    , mGroup(0)
    , mCategoryOrder(0)
    , mOrdering(0)
    , mShortcutNumericChar(0)
    , mShortcutAlphabeticChar(0)
    , mIconResId(NO_ICON)
    , mFlags(ENABLED)
{}

MenuItemImpl::MenuItemImpl (
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Int32 group,
    /* [in] */ Int32 id,
    /* [in] */ Int32 categoryOrder,
    /* [in] */ Int32 ordering,
    /* [in] */ ICharSequence* title,
    /* [in] */ Int32 showAsAction)
    : mShowAsAction(showAsAction)
    , mIsActionViewExpanded(FALSE)
    , mId(id)
    , mGroup(group)
    , mCategoryOrder(categoryOrder)
    , mOrdering(ordering)
    , mTitle(title)
    , mShortcutNumericChar(0)
    , mShortcutAlphabeticChar(0)
    , mIconResId(NO_ICON)
    , mMenu(menu)
    , mFlags(ENABLED)
{
    if (sPrependShortcutLabel.IsNull()) {
        // This is instantiated from the UI thread, so no chance of sync issues
        AutoPtr<IContext> context;
        mMenu->GetContext((IContext**)&context);
        AutoPtr<IResources> res;
        context->GetResources((IResources**)&res);
        res->GetString(R::string::prepend_shortcut_label, &sPrependShortcutLabel);
        res->GetString(R::string::menu_enter_shortcut_label, &sEnterShortcutLabel);
        res->GetString(R::string::menu_delete_shortcut_label, &sDeleteShortcutLabel);
        res->GetString(R::string::menu_space_shortcut_label, &sSpaceShortcutLabel);
    }
}

ECode MenuItemImpl::Init(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Int32 group,
    /* [in] */ Int32 id,
    /* [in] */ Int32 categoryOrder,
    /* [in] */ Int32 ordering,
    /* [in] */ ICharSequence* title,
    /* [in] */ Int32 showAsAction)
{
    if (sPrependShortcutLabel.IsNull()) {
        // This is instantiated from the UI thread, so no chance of sync issues
        AutoPtr<IContext> context;
        menu->GetContext((IContext**)&context);
        AutoPtr<IResources> res;
        context->GetResources((IResources**)&res);
        res->GetString(R::string::prepend_shortcut_label, &sPrependShortcutLabel);
        res->GetString(R::string::menu_enter_shortcut_label, &sEnterShortcutLabel);
        res->GetString(R::string::menu_delete_shortcut_label, &sDeleteShortcutLabel);
        res->GetString(R::string::menu_space_shortcut_label, &sSpaceShortcutLabel);
    }
    mMenu = menu;
    mId = id;
    mGroup = group;
    mCategoryOrder = categoryOrder;
    mOrdering = ordering;
    mTitle = title;
    mShowAsAction = showAsAction;
    return NOERROR;
}

ECode MenuItemImpl::Invoke(
    /* [out] */ Boolean* state)
{
    VALIDATE_NOT_NULL(state);

    Boolean isConsumed;
    if (mClickListener != NULL &&
        (mClickListener->OnMenuItemClick((IMenuItem*)this->Probe(EIID_IMenuItem), &isConsumed), isConsumed)) {
        *state = TRUE;
        return NOERROR;
    }

    AutoPtr<IMenuBuilder> menu;
    mMenu->GetRootMenu((IMenuBuilder**)&menu);

    if (mMenu->DispatchMenuItemSelected(menu, (IMenuItem*)this->Probe(EIID_IMenuItem), &isConsumed), isConsumed) {
        *state = TRUE;
        return NOERROR;
    }

    if (mItemCallback != NULL) {
        mItemCallback->Run();
        *state = TRUE;
        return NOERROR;
    }

    if (mIntent != NULL) {
//        try {
            AutoPtr<IContext> context;
            mMenu->GetContext((IContext**)&context);
            FAIL_RETURN(context->StartActivity(mIntent));
            *state = TRUE;
            return NOERROR;
//        } catch (ActivityNotFoundException e) {
//            Log.e(TAG, "Can't find activity to handle intent; ignoring", e);
//        }

    }

    Boolean pDefault;
    if (mActionProvider != NULL && (mActionProvider->OnPerformDefaultAction(&pDefault), pDefault)) {
        *state = TRUE;
        return NOERROR;
    }

    *state = FALSE;
    return NOERROR;
}

ECode MenuItemImpl::IsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);

    *enabled = (mFlags & ENABLED) != 0;
    return NOERROR;
}

ECode MenuItemImpl::SetEnabled(
    /* [in] */Boolean enabled)
{
    if (enabled) {
        mFlags |= ENABLED;
    } else {
        mFlags &= ~ENABLED;
    }

    mMenu->OnItemsChanged(FALSE);

    return NOERROR;
}

ECode MenuItemImpl::GetGroupId(
    /* [out] */ Int32* groupId)
{
    VALIDATE_NOT_NULL(groupId);
    *groupId = mGroup;
    return NOERROR;
}

ECode MenuItemImpl::GetItemId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;
    return NOERROR;
}

ECode MenuItemImpl::GetOrder(
    /* [out] */ Int32* order)
{
    VALIDATE_NOT_NULL(order);
    *order = mCategoryOrder;
    return NOERROR;
}

ECode MenuItemImpl::GetOrdering(
    /* [out] */ Int32* ordering)
{
    VALIDATE_NOT_NULL(ordering);
    *ordering = mOrdering;
    return NOERROR;
}

ECode MenuItemImpl::GetIntent(
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    *intent = mIntent;
    REFCOUNT_ADD(*intent);
    return NOERROR;
}

ECode MenuItemImpl::SetIntent(
    /* [in] */ IIntent* intent)
{
    mIntent = intent;
    return NOERROR;
}

AutoPtr<IRunnable> MenuItemImpl::GetCallback()
{
    return mItemCallback;
}

ECode MenuItemImpl::SetCallback(
    /* [in] */ IRunnable* callback)
{
    mItemCallback = callback;
    return NOERROR;
}

ECode MenuItemImpl::GetAlphabeticShortcut(
    /* [out] */ Char32* shortcut)
{
    VALIDATE_NOT_NULL(shortcut);
    *shortcut = mShortcutAlphabeticChar;
    return NOERROR;
}

ECode MenuItemImpl::SetAlphabeticShortcut(
    /* [in] */ Char32 alphaChar)
{
    if (mShortcutAlphabeticChar == alphaChar) {
        return NOERROR;
    }

    mShortcutAlphabeticChar = Character::ToLowerCase(alphaChar);

    mMenu->OnItemsChanged(FALSE);

    return NOERROR;
}

ECode MenuItemImpl::GetNumericShortcut(
    /* [out] */ Char32* shortcut)
{
    VALIDATE_NOT_NULL(shortcut);
    *shortcut = mShortcutNumericChar;
    return NOERROR;
}

ECode MenuItemImpl::SetNumericShortcut(
    /* [in] */ Char32 numericChar)
{
    if (mShortcutNumericChar == numericChar) {
        return NOERROR;
    }

    mShortcutNumericChar = numericChar;

    mMenu->OnItemsChanged(FALSE);

    return NOERROR;
}

ECode MenuItemImpl::SetShortcut(
    /* [in] */ Char32 numericChar,
    /* [in] */ Char32 alphaChar)
{
    mShortcutNumericChar = numericChar;
    mShortcutAlphabeticChar = Character::ToLowerCase(alphaChar);

    mMenu->OnItemsChanged(FALSE);

    return NOERROR;
}

/**
 * @return The active shortcut (based on QWERTY-mode of the menu).
 */
ECode MenuItemImpl::GetShortcut(
    /* [out] */ Char32* c)
{
    VALIDATE_NOT_NULL(c);
    Boolean isQwertyMode;
    mMenu->IsQwertyMode(&isQwertyMode);
    *c = isQwertyMode ? mShortcutAlphabeticChar : mShortcutNumericChar;

    return NOERROR;
}

ECode MenuItemImpl::GetShortcutLabel(
    /* [out] */ String* label)
{
    VALIDATE_NOT_NULL(label);
    Char32 shortcut;
    GetShortcut(&shortcut);
    if (shortcut == 0) {
        *label = String("");
        return NOERROR;
    }

    StringBuffer sb(sPrependShortcutLabel);
    switch (shortcut) {
        case '\n':
            sb += sEnterShortcutLabel;
            break;

        case '\b':
            sb += sDeleteShortcutLabel;
            break;

        case ' ':
            sb += sSpaceShortcutLabel;
            break;

        default:
            sb += shortcut;
            break;
    }

    sb.ToString(label);
    return NOERROR;
}

/**
 * @return Whether this menu item should be showing shortcuts (depends on
 *         whether the menu should show shortcuts and whether this item has
 *         a shortcut defined)
 */
ECode MenuItemImpl::ShouldShowShortcut(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has);
    // Show shortcuts if the menu is supposed to show shortcuts AND this item has a shortcut
    Boolean visiable;
    mMenu->IsShortcutsVisible(&visiable);
    Char32 c;
    GetShortcut(&c);
    *has = visiable && (c != 0);
    return NOERROR;
}

ECode MenuItemImpl::GetSubMenu(
    /* [out] */ ISubMenu** subMenu)
{
    VALIDATE_NOT_NULL(subMenu);

    AutoPtr<ISubMenu> sub = ISubMenu::Probe(mSubMenu);
    *subMenu = sub;
    REFCOUNT_ADD(*subMenu);

    return NOERROR;
}

ECode MenuItemImpl::HasSubMenu(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has);

    *has = mSubMenu != NULL;
    return NOERROR;
}

ECode MenuItemImpl::SetSubMenu(
    /* [in] */ ISubMenuBuilder* subMenu)
{
    mSubMenu = subMenu;

    AutoPtr<ICharSequence> csq;
    GetTitle((ICharSequence**)&csq);
    ISubMenu::Probe(subMenu)->SetHeaderTitle(csq);
    return NOERROR;
}

ECode MenuItemImpl::GetTitle(
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title);
    *title = mTitle;
    REFCOUNT_ADD(*title);

    return NOERROR;
}

/**
 * Gets the title for a particular {@link ItemView}
 *
 * @param itemView The ItemView that is receiving the title
 * @return Either the title or condensed title based on what the ItemView
 *         prefers
 */
ECode MenuItemImpl::GetTitleForItemView(
    /* [in] */ IMenuItemView* itemView,
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title);
    Boolean supported;
    if ((itemView != NULL) && (itemView->PrefersCondensedTitle(&supported), supported)) {
        GetTitleCondensed(title);
        return NOERROR;
    }
    else {
        GetTitle(title);
        return NOERROR;
    }
}

ECode MenuItemImpl::SetTitle(
    /* [in] */ ICharSequence* title)
{
    mTitle = title;
    mMenu->OnItemsChanged(FALSE);

    if (mSubMenu != NULL) {
        ISubMenu::Probe(mSubMenu)->SetHeaderTitle(title);
    }

    return NOERROR;
}

ECode MenuItemImpl::SetTitle(
    /* [in] */ Int32 title)
{
    String str;

    AutoPtr<IContext> context;
    mMenu->GetContext((IContext**)&context);
    context->GetString(title, &str);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(str, (ICharSequence**)&csq);
    return SetTitle(csq);
}

ECode MenuItemImpl::GetTitleCondensed(
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title);

    *title = mTitleCondensed != NULL ? mTitleCondensed : mTitle;
    REFCOUNT_ADD(*title);

    return NOERROR;
}

ECode MenuItemImpl::SetTitleCondensed(
    /* [in] */ ICharSequence* title)
{
    mTitleCondensed = title;

    // Could use getTitle() in the loop below, but just cache what it would do here
    if (title == NULL) {
        title = mTitle;
    }

    mMenu->OnItemsChanged(FALSE);

    return NOERROR;
}

ECode MenuItemImpl::GetIcon(
    /* [out] */ IDrawable** retDrawable)
{
    VALIDATE_NOT_NULL(retDrawable);

    if (mIconDrawable != NULL) {
        *retDrawable = mIconDrawable;
        REFCOUNT_ADD(*retDrawable);
        return NOERROR;
    }

    if (mIconResId != NO_ICON) {
        AutoPtr<IResources> res;
        mMenu->GetResources((IResources**)&res);
        AutoPtr<IDrawable> icon;
        res->GetDrawable(mIconResId, (IDrawable**)&icon);

        mIconResId = NO_ICON;
        mIconDrawable = icon;

        *retDrawable = icon;
        REFCOUNT_ADD(*retDrawable);
        return NOERROR;
    }

    *retDrawable = NULL;
    return NOERROR;
}

ECode MenuItemImpl::SetIcon(
    /* [in] */ IDrawable* icon)
{
    mIconResId = NO_ICON;
    mIconDrawable = icon;

    mMenu->OnItemsChanged(FALSE);

    return NOERROR;
}

ECode MenuItemImpl::SetIcon(
    /* [in] */Int32 iconResId)
{
    mIconDrawable = NULL;
    mIconResId = iconResId;

    // If we have a view, we need to push the Drawable to them
    mMenu->OnItemsChanged(FALSE);

    return NOERROR;
}

ECode MenuItemImpl::IsCheckable(
    /* [out] */ Boolean* checkable)
{
    VALIDATE_NOT_NULL(checkable);

    *checkable = (mFlags & CHECKABLE) == CHECKABLE;

    return NOERROR;
}

ECode MenuItemImpl::SetCheckable(
    /* [in] */ Boolean checkable)
{
    const Int32 oldFlags = mFlags;
    mFlags = (mFlags & ~CHECKABLE) | (checkable ? CHECKABLE : 0);
    if (oldFlags != mFlags) {
        mMenu->OnItemsChanged(FALSE);
    }

    return NOERROR;
}

ECode MenuItemImpl::SetExclusiveCheckable(
    /* [in] */ Boolean exclusive)
{
    mFlags = (mFlags & ~EXCLUSIVE) | (exclusive ? EXCLUSIVE : 0);
    return NOERROR;
}

ECode MenuItemImpl::IsExclusiveCheckable(
    /* [out] */ Boolean* checkable)
{
    VALIDATE_NOT_NULL(checkable);

    *checkable = (mFlags & EXCLUSIVE) != 0;
    return NOERROR;
}

ECode MenuItemImpl::IsChecked(
    /* [out] */ Boolean* checked)
{
    VALIDATE_NOT_NULL(checked);

    *checked = (mFlags & CHECKED) == CHECKED;
    return NOERROR;
}

ECode MenuItemImpl::SetChecked(
        /* [in] */ Boolean checked)
{
    if ((mFlags & EXCLUSIVE) != 0) {
        // Call the method on the Menu since it knows about the others in this
        // exclusive checkable group
        mMenu->SetExclusiveItemChecked((IMenuItem*)this->Probe(EIID_IMenuItem));
    }
    else {
        SetCheckedInt(checked);
    }

    return NOERROR;
}

ECode MenuItemImpl::SetCheckedInt(
    /* [in] */ Boolean checked)
{
    const Int32 oldFlags = mFlags;
    mFlags = (mFlags & ~CHECKED) | (checked ? CHECKED : 0);
    if (oldFlags != mFlags) {
        mMenu->OnItemsChanged(FALSE);
    }
    return NOERROR;
}

ECode MenuItemImpl::IsVisible(
    /* [out] */ Boolean* visiable)
{
    VALIDATE_NOT_NULL(visiable);

    Boolean bIf;
    if (mActionProvider != NULL && (mActionProvider->OverridesItemVisibility(&bIf), bIf)) {
        Boolean visible;
        mActionProvider->IsVisible(&visible);
        *visiable = (mFlags & HIDDEN) == 0 && visible;
        return NOERROR;
    }

    *visiable = (mFlags & HIDDEN) == 0;
    return NOERROR;
}

ECode MenuItemImpl::SetVisibleInt(
    /* [in] */ Boolean shown,
    /* [out] */ Boolean* isVisible)
{
    VALIDATE_NOT_NULL(isVisible);
    const Int32 oldFlags = mFlags;
    mFlags = (mFlags & ~HIDDEN) | (shown ? 0 : HIDDEN);
    *isVisible = oldFlags != mFlags;
    return NOERROR;
}

ECode MenuItemImpl::SetVisible(
    /* [in] */ Boolean shown)
{

    // Try to set the shown state to the given state. If the shown state was changed
    // (i.e. the previous state isn't the same as given state), notify the parent menu that
    // the shown state has changed for this item
    Boolean res = FALSE;
    SetVisibleInt(shown, &res);
    if (res) {
        mMenu->OnItemVisibleChanged((IMenuItemImpl*)this->Probe(EIID_IMenuItemImpl));
    }

    return NOERROR;
}

ECode MenuItemImpl::SetOnMenuItemClickListener(
        /* [in] */ IOnMenuItemClickListener* clickListener)
{
    mClickListener = clickListener;
    return NOERROR;
}

ECode MenuItemImpl::ToString(
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string);

    return mTitle->ToString(string);
}

ECode MenuItemImpl::SetMenuInfo(
    /* [in] */ IContextMenuInfo* menuInfo)
{
    mMenuInfo = menuInfo;
    return NOERROR;
}

ECode MenuItemImpl::GetMenuInfo(
    /* [out] */ IContextMenuInfo** menuInfo)
{
    VALIDATE_NOT_NULL(menuInfo);

    *menuInfo = mMenuInfo;
    REFCOUNT_ADD(*menuInfo);

    return NOERROR;
}

ECode MenuItemImpl::ActionFormatChanged()
{
    mMenu->OnItemActionRequestChanged((IMenuItemImpl*)this->Probe(EIID_IMenuItemImpl));
    return NOERROR;
}

ECode MenuItemImpl::ShouldShowIcon(
    /* [out] */ Boolean* show)
{
    VALIDATE_NOT_NULL(show);
    mMenu->GetOptionalIconsVisible(show);
    return NOERROR;
}

ECode MenuItemImpl::IsActionButton(
    /* [out] */ Boolean* action)
{
    assert(action != NULL);
    *action = (mFlags & IS_ACTION) == IS_ACTION;
    return NOERROR;
}

ECode MenuItemImpl::RequestsActionButton(
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = (mShowAsAction & IMenuItem::SHOW_AS_ACTION_IF_ROOM) == IMenuItem::SHOW_AS_ACTION_IF_ROOM;
    return NOERROR;
}

ECode MenuItemImpl::RequiresActionButton(
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = (mShowAsAction & IMenuItem::SHOW_AS_ACTION_ALWAYS) == IMenuItem::SHOW_AS_ACTION_ALWAYS;
    return NOERROR;
}

ECode MenuItemImpl::SetIsActionButton(
    /* [in] */ Boolean isActionButton)
{
    if (isActionButton) {
        mFlags |= IS_ACTION;
    } else {
        mFlags &= ~IS_ACTION;
    }

    return NOERROR;
}

ECode MenuItemImpl::ShowsTextAsAction(
    /* [out] */ Boolean* shows)
{
    assert(shows != NULL);
    *shows = (mShowAsAction & IMenuItem::SHOW_AS_ACTION_WITH_TEXT) == IMenuItem::SHOW_AS_ACTION_WITH_TEXT;
    return NOERROR;
}

ECode MenuItemImpl::SetShowAsAction(
    /* [in] */ Int32 actionEnum)
{
    switch (actionEnum & SHOW_AS_ACTION_MASK) {
        case IMenuItem::SHOW_AS_ACTION_ALWAYS:
        case IMenuItem::SHOW_AS_ACTION_IF_ROOM:
        case IMenuItem::SHOW_AS_ACTION_NEVER:
            // Looks good!
            break;

        default:
            // Mutually exclusive options selected!
            // throw new IllegalArgumentException("SHOW_AS_ACTION_ALWAYS, SHOW_AS_ACTION_IF_ROOM,"
            //         + " and SHOW_AS_ACTION_NEVER are mutually exclusive.");
            assert(0);
    }

    mShowAsAction = actionEnum;
    mMenu->OnItemActionRequestChanged((IMenuItemImpl*)this->Probe(EIID_IMenuItemImpl));
    return NOERROR;
}

ECode MenuItemImpl::SetActionView(
    /* [in] */ IView* view)
{
    mActionView = view;
    mActionProvider = NULL;
    Int32 id = 0;
    if (view != NULL && (view->GetId(&id), id) == IView::NO_ID && mId > 0) {
        view->SetId(mId);
    }

    mMenu->OnItemActionRequestChanged((IMenuItemImpl*)this->Probe(EIID_IMenuItemImpl));

    return NOERROR;
}

ECode MenuItemImpl::SetActionView(
    /* [in] */ Int32 resId)
{
    AutoPtr<IContext> context;
    mMenu->GetContext((IContext**)&context);
    AutoPtr<ILayoutInflater> inflater;
    LayoutInflater::From(context, (ILayoutInflater**)&inflater);

    AutoPtr<ILinearLayout> linearLayout;
    CLinearLayout::New(context, (ILinearLayout**)&linearLayout);
    AutoPtr<IView> view;
    inflater->Inflate(resId, linearLayout, FALSE, (IView**)&view);

    SetActionView(view);

    return NOERROR;
}

ECode MenuItemImpl::GetActionView(
    /* [out] */ IView** view)
{
    assert(view != NULL);

    *view = NULL;
    if (mActionView != NULL) {
        *view = mActionView;
        REFCOUNT_ADD(*view);
        return NOERROR;
    }
    else if (mActionProvider != NULL) {
        mActionProvider->OnCreateActionView((IMenuItem*)this->Probe(EIID_IMenuItem), (IView**)&mActionView);
        *view = mActionView;
        REFCOUNT_ADD(*view);
        return NOERROR;
    }
    else {
        *view = NULL;
        return NOERROR;
    }
}

ECode MenuItemImpl::GetActionProvider(
    /* [out] */ IActionProvider** provider)
{
    assert(provider != NULL);
    *provider = mActionProvider;
    REFCOUNT_ADD(*provider);

    return NOERROR;
}

ECode MenuItemImpl::SetActionProvider(
    /* [in] */ IActionProvider* actionProvider)
{
    if (mActionProvider != NULL) {
        mActionProvider->SetVisibilityListener(NULL);
    }

    mActionView = NULL;
    mActionProvider = actionProvider;
    mMenu->OnItemsChanged(TRUE); // Measurement can be changed
    AutoPtr<VisibilityListener> listener = new VisibilityListener(this);
    mActionProvider->SetVisibilityListener(listener);

    return NOERROR;
}

ECode MenuItemImpl::SetShowAsActionFlags(
    /* [in] */ Int32 actionEnum)
{
    SetShowAsAction(actionEnum);

    return NOERROR;
}

ECode MenuItemImpl::ExpandActionView(
    /* [out] */ Boolean* expand)
{
    assert(expand != NULL);
    if ((mShowAsAction & IMenuItem::SHOW_AS_ACTION_COLLAPSE_ACTION_VIEW) == 0 || mActionView == NULL) {
        *expand = FALSE;
        return NOERROR;
    }

    Boolean tmp = FALSE;
    if (mOnActionExpandListener == NULL ||
            (mOnActionExpandListener->OnMenuItemActionExpand((IMenuItem*)this->Probe(EIID_IMenuItem), &tmp), tmp)) {
        return mMenu->ExpandItemActionView((IMenuItemImpl*)this->Probe(EIID_IMenuItemImpl), expand);
    }

    *expand = FALSE;
    return NOERROR;
}

ECode MenuItemImpl::CollapseActionView(
    /* [out] */ Boolean* collapse)
{
    assert(collapse != NULL);
    if ((mShowAsAction & IMenuItem::SHOW_AS_ACTION_COLLAPSE_ACTION_VIEW) == 0) {
        *collapse = FALSE;
        return NOERROR;
    }

    if (mActionView == NULL) {
        // We're already collapsed if we have no action view.
        *collapse = TRUE;
        return NOERROR;
    }

    Boolean tmp = FALSE;
    if (mOnActionExpandListener == NULL ||
            (mOnActionExpandListener->OnMenuItemActionCollapse((IMenuItem*)this->Probe(EIID_IMenuItem), &tmp), tmp)) {
        return mMenu->CollapseItemActionView((IMenuItemImpl*)this->Probe(EIID_IMenuItemImpl), collapse);
    }

    *collapse = FALSE;
    return NOERROR;
}

ECode MenuItemImpl::SetOnActionExpandListener(
    /* [in] */ IOnActionExpandListener* listener)
{
    mOnActionExpandListener = listener;

    return NOERROR;
}

ECode MenuItemImpl::HasCollapsibleActionView(
    /* [out] */ Boolean* has)
{
    assert(has != NULL);
    *has = (mShowAsAction & IMenuItem::SHOW_AS_ACTION_COLLAPSE_ACTION_VIEW) != 0 && mActionView != NULL;
    return NOERROR;
}

ECode MenuItemImpl::SetActionViewExpanded(
    /* [in] */ Boolean isExpanded)
{
    mIsActionViewExpanded = isExpanded;
    mMenu->OnItemsChanged(FALSE);
    return NOERROR;
}

ECode MenuItemImpl::IsActionViewExpanded(
    /* [out] */ Boolean* expanded)
{
    assert(expanded != NULL);
    *expanded = mIsActionViewExpanded;
    return NOERROR;
}

} // namespace Menu
} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos
