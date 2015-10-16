
#include "elastos/droid/view/CContextThemeWrapper.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/content/CComponentName.h"
#include "elastos/droid/content/res/CConfiguration.h"
#include "elastos/droid/view/CKeyCharacterMap.h"
#include "elastos/droid/view/ActionProvider.h"
#include "elastos/droid/view/menu/MenuBuilderBase.h"
#include "elastos/droid/view/menu/MenuItemImpl.h"
#include "elastos/droid/view/menu/CIconMenuView.h"
#include "elastos/droid/view/menu/CSubMenuBuilder.h"
#include "elastos/droid/view/menu/CMenuItemImpl.h"
#include "elastos/droid/R.h"

extern "C" const InterfaceID EIID_MenuBuilderBase =
        { 0x2dbd4c5b, 0xd8cc, 0x489e, { 0xb3, 0xe3, 0xae, 0xeb, 0x1c, 0x5b, 0x99, 0x42 } };

using Elastos::Droid::R;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::CComponentName;
using Elastos::Utility::CObjectInt32Map;
using Elastos::Droid::View::Menu::CSubMenuBuilder;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

const String MenuBuilderBase::TAG("MenuBuilder");
const String MenuBuilderBase::PRESENTER_KEY = String("android:menu:presenters");
const String MenuBuilderBase::ACTION_VIEW_STATES_KEY = String("android:menu:actionviewstates");
const String MenuBuilderBase::EXPANDED_ACTION_VIEW_ID = String("android:menu:expandedactionview");


const Int32  MenuBuilderBase::sCategoryToOrder[6] = {
    1, /* No category */
    4, /* CONTAINER */
    5, /* SYSTEM */
    3, /* SECONDARY */
    2, /* ALTERNATIVE */
    0, /* SELECTED_ALTERNATIVE */
};

MenuBuilderBase::MenuBuilderBase()
    : mQwertyMode(FALSE)
    , mShortcutsVisible(FALSE)
    , mIsVisibleItemsStale(TRUE)
    , mDefaultShowAsAction(IMenuItem::SHOW_AS_ACTION_NEVER)
    , mPreventDispatchingItemsChanged(FALSE)
    , mItemsChangedWhileDispatchPrevented(FALSE)
    , mOptionalIconsVisible(FALSE)
    , mIsClosing(FALSE)
{
    mItems = new MenuItemImplList();
    mVisibleItems = new MenuItemImplList();
    mActionItems = new MenuItemImplList();
    mNonActionItems = new MenuItemImplList();
    mTempShortcutItemList = new MenuItemImplList();
}

MenuBuilderBase::MenuBuilderBase(
    /* [in] */ IContext* context)
    : mQwertyMode(FALSE)
    , mShortcutsVisible(FALSE)
    , mIsVisibleItemsStale(TRUE)
    , mDefaultShowAsAction(IMenuItem::SHOW_AS_ACTION_NEVER)
    , mPreventDispatchingItemsChanged(FALSE)
    , mItemsChangedWhileDispatchPrevented(FALSE)
    , mOptionalIconsVisible(FALSE)
    , mIsClosing(FALSE)
{
    mItems = new MenuItemImplList();
    mVisibleItems = new MenuItemImplList();
    mActionItems = new MenuItemImplList();
    mNonActionItems = new MenuItemImplList();
    mTempShortcutItemList = new MenuItemImplList();

    Init(context);
}

ECode MenuBuilderBase::Init(
    /* [in] */ IContext* context)
{
    mContext = context;
    context->GetResources((IResources**)&mResources);
    mIsVisibleItemsStale = TRUE;
    mIsActionItemsStale = TRUE;
    SetShortcutsVisibleInner(TRUE);
    return NOERROR;
}

AutoPtr<IMenuBuilder> MenuBuilderBase::SetDefaultShowAsAction(
    /* [in] */ Int32 defaultShowAsAction)
{
    mDefaultShowAsAction = defaultShowAsAction;
    AutoPtr<IMenuBuilder> result = THIS_PROBE(IMenuBuilder);
    return result;
}

ECode MenuBuilderBase::AddMenuPresenter(
    /* [in] */ IMenuPresenter* presenter)
{
    VALIDATE_NOT_NULL(presenter);

    IWeakReferenceSource* wrs = IWeakReferenceSource::Probe(presenter);
    assert(wrs != NULL);
    AutoPtr<IWeakReference> wr;
    wrs->GetWeakReference((IWeakReference**)&wr);
    mPresenters.PushBack(wr);

    AutoPtr<IMenuBuilder> build = THIS_PROBE(IMenuBuilder);
    presenter->InitForMenu(mContext, build);
    mIsActionItemsStale = TRUE;
    return NOERROR;
}

ECode MenuBuilderBase::RemoveMenuPresenter(
    /* [in] */ IMenuPresenter* presenter)
{
    List< AutoPtr<IWeakReference> >::Iterator ator = mPresenters.Begin();
    for (; ator != mPresenters.End();) {
        AutoPtr<IMenuPresenter> item;
        (*ator)->Resolve(EIID_IMenuPresenter, (IInterface**)&item);

        if (item == NULL || item.Get() == presenter) {
            ator = mPresenters.Erase(ator);
        }
        else {
            ator++;
        }
    }

    return NOERROR;
}

void MenuBuilderBase::DispatchPresenterUpdate(
    /* [in] */ Boolean cleared)
{
    if (mPresenters.IsEmpty()) return;

    StopDispatchingItemsChanged();

    List< AutoPtr<IWeakReference> >::Iterator ator = mPresenters.Begin();
    for (; ator != mPresenters.End();) {
        AutoPtr<IMenuPresenter> presenter;
        (*ator)->Resolve(EIID_IMenuPresenter, (IInterface**)&presenter);
        if (presenter == NULL) {
            ator = mPresenters.Erase(ator);
        } else {
            presenter->UpdateMenuView(cleared);
            ator++;
        }
    }

    StartDispatchingItemsChanged();
}

Boolean MenuBuilderBase::DispatchSubMenuSelected(
    /* [in] */ ISubMenuBuilder* subMenu)
{
    if (mPresenters.IsEmpty()) return FALSE;

    Boolean result = FALSE;

    List< AutoPtr<IWeakReference> >::Iterator ator = mPresenters.Begin();
    for (; ator != mPresenters.End();) {
        AutoPtr<IMenuPresenter> presenter;
        (*ator)->Resolve(EIID_IMenuPresenter, (IInterface**)&presenter);
        if (presenter == NULL) {
            ator = mPresenters.Erase(ator);
        } else if (!result) {
            presenter->OnSubMenuSelected(subMenu, &result);
            ator++;
        }
        else {
            ator++;
        }
    }

    return result;
}

void MenuBuilderBase::DispatchSaveInstanceState(
    /* [in] */ IBundle* outState)
{
    assert(outState != NULL);
    if (mPresenters.IsEmpty()) return;

    AutoPtr<IObjectInt32Map> presenterStates;
    CObjectInt32Map::New((IObjectInt32Map**)&presenterStates);

    List< AutoPtr<IWeakReference> >::Iterator ref = mPresenters.Begin();
    for (; ref != mPresenters.End();) {
        AutoPtr<IMenuPresenter> presenter;
        (*ref)->Resolve(EIID_IMenuPresenter, (IInterface**)&presenter);
        if (presenter == NULL) {
            ref = mPresenters.Erase(ref);
        } else {
            ref ++;

            Int32 id = 0;
            presenter->GetId(&id);
            if (id > 0) {
                AutoPtr<IParcelable> state;
                presenter->OnSaveInstanceState((IParcelable**)&state);
                if (state != NULL) {
                    presenterStates->Put(id, state);
                }
            }
        }
    }

    outState->PutParcelableMap(PRESENTER_KEY, presenterStates);
}

void MenuBuilderBase::DispatchRestoreInstanceState(
    /* [in] */ IBundle* state)
{
    assert(state != NULL);
    AutoPtr<IObjectInt32Map> presenterStates;
    state->GetParcelableMap(PRESENTER_KEY, (IInterface**)&presenterStates);

    if (presenterStates == NULL || mPresenters.IsEmpty()) return;

    List< AutoPtr<IWeakReference> >::Iterator ref = mPresenters.Begin();
    for (; ref != mPresenters.End();) {
        AutoPtr<IMenuPresenter> presenter;
        (*ref)->Resolve(EIID_IMenuPresenter, (IInterface**)&presenter);
        if (presenter == NULL) {
            ref = mPresenters.Erase(ref);
        } else {
            ref ++;

            Int32 id = 0;
            presenter->GetId(&id);
            if (id > 0) {
                AutoPtr<IParcelable> parcel;
                presenterStates->Get(id, (IInterface**)&parcel);
                if (parcel != NULL) {
                    presenter->OnRestoreInstanceState(parcel);
                }
            }
        }
    }
}

ECode MenuBuilderBase::SavePresenterStates(
    /* [in] */ IBundle* outState)
{
    DispatchSaveInstanceState(outState);
    return NOERROR;
}

ECode MenuBuilderBase::RestorePresenterStates(
    /* [in] */ IBundle* state)
{
    DispatchRestoreInstanceState(state);
    return NOERROR;
}

ECode MenuBuilderBase::SaveActionViewStates(
    /* [in] */ IBundle* outStates)
{
    assert(outStates != NULL);
    AutoPtr<IObjectInt32Map> viewStates;

    const Int32 itemCount = GetSize();
    for (Int32 i = 0; i < itemCount; i++) {
        AutoPtr<IMenuItem> item = GetItem(i);
        AutoPtr<IView> v;
        item->GetActionView((IView**)&v);

        Int32 id = 0;
        if (v != NULL && (v->GetId(&id), id) != IView::NO_ID) {
            if (viewStates == NULL) {
                CObjectInt32Map::New((IObjectInt32Map**)&viewStates);
            }

            v->SaveHierarchyState(viewStates);

            Boolean expanded = FALSE;
            if (item->IsActionViewExpanded(&expanded), expanded) {
                item->GetItemId(&id);
                outStates->PutInt32(EXPANDED_ACTION_VIEW_ID, id);
            }
        }

        Boolean has = FALSE;
        if (item->HasSubMenu(&has), has) {
            AutoPtr<ISubMenu> submenu;
            item->GetSubMenu((ISubMenu**)&submenu);
            AutoPtr<ISubMenuBuilder> subMenu = ISubMenuBuilder::Probe(submenu);
            subMenu->SaveActionViewStates(outStates);
        }
    }

    if (viewStates != NULL) {
        String key;
        GetActionViewStatesKey(&key);
        outStates->PutParcelableMap(key, viewStates);
    }

    return NOERROR;
}

ECode MenuBuilderBase::RestoreActionViewStates(
    /* [in] */ IBundle* states)
{
    if (states == NULL) {
        return NOERROR;
    }

    String key;
    GetActionViewStatesKey(&key);
    AutoPtr<IObjectInt32Map> viewStates;
    states->GetParcelableMap(key, (IInterface**)&viewStates);

    const Int32 itemCount = GetSize();
    for (Int32 i = 0; i < itemCount; i++) {
        AutoPtr<IMenuItem> item = GetItem(i);
        AutoPtr<IView> v;
        item->GetActionView((IView**)&v);

        Int32 id = 0;
        if (v != NULL && (v->GetId(&id), id) != IView::NO_ID) {
            v->RestoreHierarchyState(viewStates);
        }

        Boolean has = FALSE;
        if (item->HasSubMenu(&has), has) {
            AutoPtr<ISubMenu> submenu;
            item->GetSubMenu((ISubMenu**)&submenu);
            AutoPtr<ISubMenuBuilder> subMenu = ISubMenuBuilder::Probe(submenu);
            subMenu->RestoreActionViewStates(states);
        }
    }

    Int32 expandedId = 0;
    states->GetInt32(EXPANDED_ACTION_VIEW_ID, &expandedId);
    if (expandedId > 0) {
        AutoPtr<IMenuItem> itemToExpand = FindItem(expandedId);
        if (itemToExpand != NULL) {
            Boolean expanded = FALSE;
            itemToExpand->ExpandActionView(&expanded);
        }
    }

    return NOERROR;
}

void MenuBuilderBase::GetActionViewStatesKey(
    /* [out] */ String* key)
{
    assert(key != NULL);
    *key = ACTION_VIEW_STATES_KEY;
}

ECode MenuBuilderBase::SetCallback(
    /* [in] */ IMenuBuilderCallback* callback)
{
    mCallback = callback;
    return NOERROR;
}

ECode MenuBuilderBase::GetCallback(
    /* [out] */ IMenuBuilderCallback** callback)
{
    assert(callback != NULL);
    *callback = mCallback;
    REFCOUNT_ADD(*callback);
    return NOERROR;
}

AutoPtr<IMenuItem> MenuBuilderBase::AddInternal(
    /* [in] */ Int32 group,
    /* [in] */ Int32 id,
    /* [in] */ Int32 categoryOrder,
    /* [in] */ ICharSequence* title)
{
    Int32 ordering = GetOrdering(categoryOrder);

    AutoPtr<IMenuItemImpl> item;
    CMenuItemImpl::New(
            THIS_PROBE(IMenuBuilder), group,
            id, categoryOrder, ordering, title, mDefaultShowAsAction, (IMenuItemImpl**)&item);

    if (mCurrentMenuInfo != NULL) {
        // Pass along the current menu info
        item->SetMenuInfo(mCurrentMenuInfo);
    }

    mItems->Insert(FindInsertIndex(mItems, ordering), item);
    OnItemsChanged(TRUE);
    AutoPtr<IMenuItem> result = (IMenuItem*)item->Probe(EIID_IMenuItem);
    return result;
}

AutoPtr<IMenuItem> MenuBuilderBase::Add(
    /* [in] */ ICharSequence* title)
{
    return AddInternal(0, 0, 0, title);
}

AutoPtr<IMenuItem> MenuBuilderBase::Add(
    /* [in] */ Int32 titleRes)
{
    String str;
    mResources->GetString(titleRes, &str);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(str, (ICharSequence**)&csq);
    return AddInternal(0, 0, 0, csq);
}

AutoPtr<IMenuItem> MenuBuilderBase::Add(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ ICharSequence* title)
{
    return AddInternal(groupId, itemId, order, title);
}

AutoPtr<IMenuItem> MenuBuilderBase::Add(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ Int32 titleRes)
{
    String str;
    mResources->GetString(titleRes, &str);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(str, (ICharSequence**)&csq);

    return AddInternal(groupId, itemId, order, csq);
}

AutoPtr<ISubMenu> MenuBuilderBase::AddSubMenu(
    /* [in] */ ICharSequence* title)
{
    return AddSubMenu(0, 0, 0, title);
}

AutoPtr<ISubMenu> MenuBuilderBase::AddSubMenu(
    /* [in] */ Int32 titleRes)
{
    String str;
    mResources->GetString(titleRes, &str);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(str, (ICharSequence**)&csq);

    return AddSubMenu(0, 0, 0, csq);
}

AutoPtr<ISubMenu> MenuBuilderBase::AddSubMenu(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ ICharSequence*  title)
{
    AutoPtr<IMenuItemImpl> item = IMenuItemImpl::Probe(AddInternal(groupId, itemId, order, title));
    AutoPtr<ISubMenuBuilder> subMenu;
    CSubMenuBuilder::New(mContext, THIS_PROBE(IMenuBuilder), item, (ISubMenuBuilder**)&subMenu);
    item->SetSubMenu(subMenu);

    return ISubMenu::Probe(subMenu);
}

AutoPtr<ISubMenu> MenuBuilderBase::AddSubMenu(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ Int32 titleRes)
{
    String str;
    mResources->GetString(titleRes, &str);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(str, (ICharSequence**)&csq);

    return AddSubMenu(groupId, itemId, order, csq);
}

Int32 MenuBuilderBase::AddIntentOptions(
    /* [in] */ Int32 group,
    /* [in] */ Int32 id,
    /* [in] */ Int32 categoryOrder,
    /* [in] */ IComponentName* caller,
    /* [in] */ const ArrayOf<IIntent*>& specifics,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ const ArrayOf<IMenuItem*>& outSpecificItems)
{
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);

    AutoPtr<IObjectContainer> lri;
    pm->QueryIntentActivityOptions(caller, (ArrayOf<IIntent*>*)&specifics, intent, 0, (IObjectContainer**)&lri);
    Int32 N = 0;
    if (lri != NULL) {
        lri->GetObjectCount(&N);
    }

    if ((flags & IMenu::FLAG_APPEND_TO_GROUP) == 0) {
        RemoveGroup(group);
    }

    if (N > 0) {
        AutoPtr<IObjectEnumerator> ator;
        lri->GetObjectEnumerator((IObjectEnumerator**)&ator);

        Boolean hasNext = FALSE;
        while (ator->MoveNext(&hasNext), hasNext) {
            AutoPtr<IResolveInfo> ri;
            ator->Current((IInterface**)&ri);

            AutoPtr<IIntent> rintent;
            Int32 specificIndex = 0;
            ri->GetSpecificIndex(&specificIndex);
            if (specificIndex < 0) {
                rintent = intent;
            }
            else {
                rintent = specifics[specificIndex];
            }

            AutoPtr<IActivityInfo> info;
            ri->GetActivityInfo((IActivityInfo**)&info);
            assert(info != NULL);

            AutoPtr<IApplicationInfo> appInfo;
            info->GetApplicationInfo((IApplicationInfo**)&appInfo);
            assert(appInfo != NULL);

            String packageName, name;
            appInfo->GetPackageName(&packageName);
            info->GetName(&name);

            AutoPtr<IComponentName> cName;
            CComponentName::New(packageName, name, (IComponentName**)&cName);
            rintent->SetComponent(cName);

            AutoPtr<ICharSequence> label;
            ri->LoadLabel(pm, (ICharSequence**)&label);
            AutoPtr<IMenuItem> item = Add(group, id, categoryOrder, label);
            assert(item != NULL);

            AutoPtr<IDrawable> icon;
            ri->LoadIcon(pm, (IDrawable**)&icon);
            item->SetIcon(icon);
            item->SetIntent(rintent);

            if (outSpecificItems.GetLength() != 0 && specificIndex >= 0) {
                //outSpecificItems.Set(specificIndex, item);
            }
        }
    }

    return N;
}

ECode MenuBuilderBase::RemoveItem(
    /* [in] */ Int32 id)
{
    RemoveItemAtInt(FindItemIndex(id), TRUE);
    return NOERROR;
}

ECode MenuBuilderBase::RemoveGroup(
    /* [in] */ Int32 group)
{
    const Int32 i = FindGroupIndex(group);

    if (i >= 0) {
        const Int32 maxRemovable = mItems->GetSize() - i;
        Int32 numRemoved = 0, groupid;
        while ((numRemoved++ < maxRemovable) && (((*mItems)[i]->GetGroupId(&groupid), groupid) == group)) {
            // Don't force update for each one, this method will do it at the end
            RemoveItemAtInt(i, FALSE);
        }

        // Notify menu views
        OnItemsChanged(TRUE);
    }

    return NOERROR;
}

void MenuBuilderBase::RemoveItemAtInt(
    /* [in] */ Int32 index,
    /* [in] */ Boolean updateChildrenOnMenuViews)
{
    if ((index < 0) || (index >= (Int32)mItems->GetSize())) return;

    mItems->Remove(index);

    if (updateChildrenOnMenuViews) OnItemsChanged(TRUE);
}

ECode MenuBuilderBase::RemoveItemAt(
    /* [in] */ Int32 index)
{
    RemoveItemAtInt(index, TRUE);
    return NOERROR;
}

ECode MenuBuilderBase::ClearAll()
{
    mPreventDispatchingItemsChanged = TRUE;
    Clear();
    ClearHeader();
    mPreventDispatchingItemsChanged = FALSE;
    mItemsChangedWhileDispatchPrevented = FALSE;
    OnItemsChanged(TRUE);

    return NOERROR;
}

ECode MenuBuilderBase::Clear()
{
    if (mExpandedItem != NULL) {
        CollapseItemActionView(mExpandedItem);
    }

    mItems->Clear();

    OnItemsChanged(TRUE);

    return NOERROR;
}

ECode MenuBuilderBase::SetExclusiveItemChecked(
    /* [in] */ IMenuItem* item)
{
    Int32 group;
    item->GetGroupId(&group);

    List<AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = mItems->Begin(); iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItemImpl> curItem = *iter;
        Int32 groupid;
        curItem->GetGroupId(&groupid);
        if (groupid == group) {
            Boolean isCheckable;
            curItem->IsExclusiveCheckable(&isCheckable);
            if (!isCheckable) continue;
            curItem->IsCheckable(&isCheckable);
            if (!isCheckable) continue;

            // Check the item meant to be checked, uncheck the others (that are in the group)
            curItem->SetCheckedInt(curItem.Get() == IMenuItemImpl::Probe(item));
        }
    }
    return NOERROR;
}

ECode MenuBuilderBase::SetGroupCheckable(
    /* [in] */ Int32 group,
    /* [in] */ Boolean checkable,
    /* [in] */ Boolean exclusive)
{
    List<AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = mItems->Begin(); iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItemImpl> item = *iter;
        Int32 groupid;
        item->GetGroupId(&groupid);
        if (groupid == group) {
            item->SetExclusiveCheckable(exclusive);
            item->SetCheckable(checkable);
        }
    }

    return NOERROR;
}

ECode MenuBuilderBase::SetGroupVisible(
    /* [in] */ Int32 group,
    /* [in] */ Boolean visible)
{
    // We handle the notification of items being changed ourselves, so we use setVisibleInt rather
    // than setVisible and at the end notify of items being changed
    Boolean changedAtLeastOneItem = FALSE;

    List<AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = mItems->Begin(); iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItemImpl> item = *iter;
        Int32 groupid;
        item->GetGroupId(&groupid);
        if (groupid == group) {
            Boolean res = FALSE;
            item->SetVisibleInt(visible, &res);
            if (res) changedAtLeastOneItem = TRUE;
        }
    }

    if (changedAtLeastOneItem) OnItemsChanged(TRUE);

    return NOERROR;
}

ECode MenuBuilderBase::SetGroupEnabled(
    /* [in] */ Int32 group,
    /* [in] */ Boolean enabled)
{
    List<AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = mItems->Begin(); iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItemImpl> item = *iter;
        Int32 groupid;
        item->GetGroupId(&groupid);
        if (groupid == group) {
            item->SetEnabled(enabled);
        }
    }

    return NOERROR;
}

Boolean MenuBuilderBase::HasVisibleItems()
{
    List<AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = mItems->Begin(); iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItemImpl> item = *iter;
        Boolean visible;
        item->IsVisible(&visible);
        if (visible) {
            return TRUE;
        }
    }

    return FALSE;
}

AutoPtr<IMenuItem> MenuBuilderBase::FindItem(
    /* [in] */ Int32 id)
{
    List<AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = mItems->Begin(); iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItemImpl> item = *iter;
        Int32 itemId;
        item->GetItemId(&itemId);
        Boolean hasSub;
        if (itemId == id) {
            AutoPtr<IMenuItem> result = IMenuItem::Probe(item);
            return result;
        } else if ((item->HasSubMenu(&hasSub), hasSub)){
            if (hasSub) {
                AutoPtr<ISubMenu> subMenu;
                item->GetSubMenu((ISubMenu**)&subMenu);
                AutoPtr<IMenuItem> possibleItem;
                subMenu->FindItem(id, (IMenuItem**)&possibleItem);

                if (possibleItem != NULL) {
                    return possibleItem;
                }
            }
        }
    }

    return NULL;
}

Int32 MenuBuilderBase::FindItemIndex(
    /* [in] */ Int32 id)
{
    Int32 i;
    List<AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = mItems->Begin(), i = 0; iter != mItems->End(); ++iter, ++i) {
        AutoPtr<IMenuItemImpl> item = *iter;
        Int32 groupid;
        item->GetGroupId(&groupid);
        if (groupid == id) {
            return i;
        }
    }

    return -1;
}

Int32 MenuBuilderBase::FindGroupIndex(
    /* [in] */ Int32 group)
{
    return FindGroupIndex(group, 0);
}

Int32 MenuBuilderBase::FindGroupIndex(
    /* [in] */ Int32 group,
    /* [in] */ Int32 start)
{
    //////////////is right?
    if (start < 0) {
        start = 0;
    }

    List<AutoPtr<IMenuItemImpl> >::Iterator iter;
    Int32 i;
    for(iter = mItems->Begin(), i = 0; iter != mItems->End(); ++iter, ++i) {
        if (i >= start) {
            IMenuItemImpl* item = *iter;

            Int32 groupid;
            item->GetGroupId(&groupid);
            if (groupid == group) {
                return i;
            }
        }
    }

    return -1;
}

Int32 MenuBuilderBase::GetSize()
{
    return mItems->GetSize();
}

AutoPtr<IMenuItem> MenuBuilderBase::GetItem(
    /* [in] */ Int32 index)
{
    return (*mItems)[index];
}

Boolean MenuBuilderBase::IsShortcutKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return FindItemWithShortcutForKey(keyCode, event) != NULL;
}

ECode MenuBuilderBase::SetQwertyMode(
    /* [in] */ Boolean isQwerty)
{
    mQwertyMode = isQwerty;
    OnItemsChanged(FALSE);
    return NOERROR;
}

Int32 MenuBuilderBase::GetOrdering(
    /* [in] */ Int32 categoryOrder)
{
    const Int32 index = (categoryOrder & IMenu::CATEGORY_MASK) >> IMenu::CATEGORY_SHIFT;

    //if (index < 0 || index >= sCategoryToOrder.length) {
    if (index < 0 || index >= 6) {
        //TODO throw new IllegalArgumentException("order does not contain a valid category.");
        assert(0 && "TODO");
    }

    return (sCategoryToOrder[index] << IMenu::CATEGORY_SHIFT) | (categoryOrder & IMenu::USER_MASK);
}

Boolean MenuBuilderBase::IsQwertyMode()
{
    return mQwertyMode;
}

ECode MenuBuilderBase::SetShortcutsVisible(
    /* [in] */ Boolean shortcutsVisible)
{
    if (mShortcutsVisible == shortcutsVisible) return NOERROR;

    SetShortcutsVisibleInner(shortcutsVisible);
    OnItemsChanged(FALSE);
    return NOERROR;
}

void MenuBuilderBase::SetShortcutsVisibleInner(
    /* [in] */ Boolean shortcutsVisible)
{
    AutoPtr<IConfiguration> config;
    mResources->GetConfiguration((IConfiguration**)&config);
    assert(config != NULL);
    Int32 keyboard = 0;
    config->GetKeyboard(&keyboard);

    Boolean tmp = FALSE;
    mResources->GetBoolean(R::bool_::config_showMenuShortcutsWhenKeyboardPresent, &tmp);
    mShortcutsVisible = shortcutsVisible && keyboard != IConfiguration::KEYBOARD_NOKEYS && tmp;
}

Boolean MenuBuilderBase::IsShortcutsVisible()
{
    return mShortcutsVisible;
}

AutoPtr<IResources> MenuBuilderBase::GetResources()
{
    return mResources;
}

AutoPtr<IContext> MenuBuilderBase::GetContext()
{
    return mContext;
}

Boolean MenuBuilderBase::DispatchMenuItemSelected(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItem* item)
{
    Boolean state = FALSE;
    return mCallback != NULL && (mCallback->OnMenuItemSelected(menu, item, &state), state);
}

ECode MenuBuilderBase::ChangeMenuMode()
{
    if (mCallback != NULL) {
        return mCallback->OnMenuModeChange(THIS_PROBE(IMenuBuilder));
    }

    return NOERROR;
}

Int32 MenuBuilderBase::FindInsertIndex(
    /* [in] */ List<AutoPtr<IMenuItemImpl> >* items,
    /* [in] */ Int32 ordering)
{
    Int32 i = items->GetSize() - 1;
    List<AutoPtr<IMenuItemImpl> >::ReverseIterator riter;
    for (riter = items->RBegin(); riter != items->REnd(); ++riter, --i) {
        AutoPtr<IMenuItemImpl> item = *riter;
        Int32 _ordering;
        item->GetOrdering(&_ordering);
        if (_ordering <= ordering) {
            return i + 1;
        }
    }

    return 0;
}

Boolean MenuBuilderBase::PerformShortcut(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 flags)
{
    AutoPtr<IMenuItemImpl> item = FindItemWithShortcutForKey(keyCode, event);
    Boolean handled = FALSE;

    if (item != NULL) {
        handled = PerformItemAction(item, flags);
    }

    if ((flags & IMenu::FLAG_ALWAYS_PERFORM_CLOSE) != 0) {
        Close(TRUE);
    }

    return handled;
}

void MenuBuilderBase::FindItemsWithShortcutForKey(
    /* [in, out] */ List< AutoPtr<IMenuItemImpl> >* items,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    const Boolean qwerty = IsQwertyMode();
    Int32 metaState = 0;
    event->GetMetaState(&metaState);
    AutoPtr<CKeyCharacterMap::KeyData> possibleChars = new CKeyCharacterMap::KeyData();
    // Get the chars associated with the keyCode (i.e using any chording combo)
    Boolean isKeyCodeMapped = FALSE;
    event->GetKeyData(possibleChars, &isKeyCodeMapped);
    // The delete key is not mapped to '\b' so we treat it specially
    if (!isKeyCodeMapped && (keyCode != IKeyEvent::KEYCODE_DEL)) {
        return;
    }

    // Look for an item whose shortcut is this key.
    List< AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = mItems->Begin(); iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItemImpl> item = *iter;
        Boolean hasSub;
        item->HasSubMenu(&hasSub);
        if (hasSub) {
            AutoPtr<ISubMenu> subMenu;
            item->GetSubMenu((ISubMenu**)&subMenu);
            ((MenuBuilderBase*)subMenu->Probe(EIID_MenuBuilderBase))->FindItemsWithShortcutForKey(items, keyCode, event);
        }

        Char32 shortcutChar = 0;
        if (qwerty) {
            item->GetAlphabeticShortcut(&shortcutChar);
        }
        else {
            item->GetNumericShortcut(&shortcutChar);
        }
        Boolean isEnabled = FALSE;
        if (((metaState & (IKeyEvent::META_SHIFT_ON | IKeyEvent::META_SYM_ON)) == 0) &&
              (shortcutChar != 0) &&
              (shortcutChar == (*possibleChars->mMeta)[0]
                  || shortcutChar == (*possibleChars->mMeta)[2]
                  || (qwerty && shortcutChar == '\b' &&
                      keyCode == IKeyEvent::KEYCODE_DEL)) && (item->IsEnabled(&isEnabled), isEnabled)) {
            items->PushBack(item);
        }
    }
}

AutoPtr<IMenuItemImpl> MenuBuilderBase::FindItemWithShortcutForKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    // Get all items that can be associated directly or indirectly with the keyCode
    List<AutoPtr<IMenuItemImpl> > * items = mTempShortcutItemList;
    items->Clear();
    FindItemsWithShortcutForKey(items, keyCode, event);

    if (items->IsEmpty()) {
        return NULL;
    }

    Int32 metaState;
    event->GetMetaState(&metaState);
    AutoPtr<CKeyCharacterMap::KeyData> possibleChars = new CKeyCharacterMap::KeyData();
    // Get the chars associated with the keyCode (i.e using any chording combo)
    Boolean isKeyCodeMapped;
    event->GetKeyData(possibleChars, &isKeyCodeMapped);

    // If we have only one element, we can safely returns it
    if (items->GetSize() == 1) {
        AutoPtr<IMenuItemImpl> item = (*items)[0];
        return item;
    }

    const Boolean qwerty = IsQwertyMode();
    // If we found more than one item associated with the key,
    // we have to return the exact match
    List< AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = items->Begin(); iter != items->End(); ++iter) {
        AutoPtr<IMenuItemImpl> item = *iter;
        Char32 shortcutChar;
        if (qwerty) {
            item->GetAlphabeticShortcut(&shortcutChar);
        }
        else {
            item->GetNumericShortcut(&shortcutChar);
        }

        if ((shortcutChar == (*possibleChars->mMeta)[0] &&
                (metaState & IKeyEvent::META_ALT_ON) == 0)
            || (shortcutChar == (*possibleChars->mMeta)[2] &&
                (metaState & IKeyEvent::META_ALT_ON) != 0)
            || (qwerty && shortcutChar == '\b' &&
                keyCode == IKeyEvent::KEYCODE_DEL)) {
            return item;
        }
    }

    return NULL;
}

Boolean MenuBuilderBase::PerformIdentifierAction(
    /* [in] */ Int32 id,
    /* [in] */ Int32 flags)
{
    // Look for an item whose identifier is the id.
    return PerformItemAction(FindItem(id), flags);
}

Boolean MenuBuilderBase::PerformItemAction(
    /* [in] */ IMenuItem* item,
    /* [in] */ Int32 flags)
{
    AutoPtr<IMenuItemImpl> itemImpl = IMenuItemImpl::Probe(item);

    Boolean enabled = FALSE;
    if (itemImpl == NULL || (itemImpl->IsEnabled(&enabled), !enabled)) {
        return FALSE;
    }

    Boolean invoked = FALSE;
    itemImpl->Invoke(&invoked);

    AutoPtr<IActionProvider> provider;
    item->GetActionProvider((IActionProvider**)&provider);
    Boolean res = FALSE;
    Boolean providerHasSubMenu = (provider != NULL && (provider->HasSubMenu(&res), res));

    Boolean has = FALSE;
    if ((itemImpl->HasCollapsibleActionView(&has), has)) {
        Boolean tmp = FALSE;
        invoked |= (itemImpl->ExpandActionView(&tmp), tmp);
        if (invoked) Close(TRUE);
    } else if ((itemImpl->HasSubMenu(&has), has) || providerHasSubMenu) {
        Close(FALSE);

        if (!(itemImpl->HasSubMenu(&has), has)) {
            AutoPtr<ISubMenuBuilder> smb;
            CSubMenuBuilder::New(GetContext(), THIS_PROBE(IMenuBuilder), itemImpl, (ISubMenuBuilder**)&smb);
            itemImpl->SetSubMenu(smb);
        }

        AutoPtr<ISubMenu> tmpSubMenu;
        itemImpl->GetSubMenu((ISubMenu**)&tmpSubMenu);
        AutoPtr<ISubMenuBuilder> subMenu = ISubMenuBuilder::Probe(tmpSubMenu);
        if (providerHasSubMenu) {
            provider->OnPrepareSubMenu(ISubMenu::Probe(subMenu));
        }

        invoked |= DispatchSubMenuSelected(subMenu);
        if (!invoked) Close(TRUE);
    }
    else {
        if ((flags & IMenu::FLAG_PERFORM_NO_CLOSE) == 0) {
            Close(TRUE);
        }
    }

    return invoked;
}

ECode MenuBuilderBase::Close(
    /* [in] */ Boolean allMenusAreClosing)
{
    if (mIsClosing) return NOERROR;

    mIsClosing = TRUE;

    List< AutoPtr<IWeakReference> >::Iterator ator = mPresenters.Begin();
    for (; ator != mPresenters.End();) {
        AutoPtr<IMenuPresenter> presenter;
        (*ator)->Resolve(EIID_IMenuPresenter, (IInterface**)&presenter);
        if (presenter == NULL) {
            ator = mPresenters.Erase(ator);
        }
        else {
            ator++;
            presenter->OnCloseMenu(THIS_PROBE(IMenuBuilder), allMenusAreClosing);
        }
    }

    mIsClosing = FALSE;
    return NOERROR;
}

ECode MenuBuilderBase::Close()
{
    Close(TRUE);
    return NOERROR;
}

ECode MenuBuilderBase::OnItemsChanged(
    /* [in] */ Boolean structureChanged)
{
    if (!mPreventDispatchingItemsChanged) {
        if (structureChanged) {
            mIsVisibleItemsStale = TRUE;
            mIsActionItemsStale = TRUE;
        }

        DispatchPresenterUpdate(structureChanged);
    } else {
        mItemsChangedWhileDispatchPrevented = TRUE;
    }
    return NOERROR;
}

ECode MenuBuilderBase::StopDispatchingItemsChanged()
{
    if (!mPreventDispatchingItemsChanged) {
        mPreventDispatchingItemsChanged = TRUE;
        mItemsChangedWhileDispatchPrevented = FALSE;
    }

    return NOERROR;
}

ECode MenuBuilderBase::StartDispatchingItemsChanged()
{
    mPreventDispatchingItemsChanged = FALSE;

    if (mItemsChangedWhileDispatchPrevented) {
        mItemsChangedWhileDispatchPrevented = FALSE;
        OnItemsChanged(TRUE);
    }

    return NOERROR;
}

ECode MenuBuilderBase::OnItemVisibleChanged(
    /* [in] */ IMenuItemImpl* item)
{
    // Notify of items being changed
    mIsVisibleItemsStale = TRUE;
    OnItemsChanged(TRUE);
    return NOERROR;
}

ECode MenuBuilderBase::OnItemActionRequestChanged(
    /* [in] */ IMenuItemImpl* item)
{
    // Notify of items being changed
    mIsActionItemsStale = TRUE;
    OnItemsChanged(TRUE);
    return NOERROR;
}

AutoPtr<MenuItemImplList> MenuBuilderBase::GetVisibleItems()
{
    if (!mIsVisibleItemsStale) return mVisibleItems;

    // Refresh the visible items
    mVisibleItems->Clear();

    List<AutoPtr<IMenuItemImpl> >::Iterator iter;
    for (iter = mItems->Begin(); iter != mItems->End(); ++iter) {
        AutoPtr<IMenuItemImpl> item = *iter;
        Boolean visible;
        item->IsVisible(&visible);
        if (visible) {
            mVisibleItems->PushBack(item);
        }
    }

    mIsVisibleItemsStale = FALSE;
    mIsActionItemsStale = TRUE;

    return mVisibleItems;
}

ECode MenuBuilderBase::FlagActionItems()
{
    if (!mIsActionItemsStale) {
        return NOERROR;
    }

    // Presenters flag action items as needed.
    Boolean flagged = FALSE;
    List< AutoPtr<IWeakReference> >::Iterator ator = mPresenters.Begin();
    for (; ator != mPresenters.End();) {
        AutoPtr<IMenuPresenter> presenter;
        (*ator)->Resolve(EIID_IMenuPresenter, (IInterface**)&presenter);
        if (presenter == NULL) {
            ator = mPresenters.Erase(ator);
        }
        else {
            ator ++;
            Boolean flagAction = FALSE;
            flagged |= (presenter->FlagActionItems(&flagAction), flagAction);
        }
    }

    AutoPtr<MenuItemImplList> visibleItems = GetVisibleItems();
    const Int32 itemsSize = visibleItems->GetSize();
    if (flagged) {
        mActionItems->Clear();
        mNonActionItems->Clear();
        for (Int32 i = 0; i < itemsSize; i++) {
            AutoPtr<IMenuItemImpl> item = (*visibleItems)[i];
            Boolean isAction = FALSE;
            if (item->IsActionButton(&isAction), isAction) {
                mActionItems->PushBack(item);
            } else {
                mNonActionItems->PushBack(item);
            }
        }
    } else {
        // Nobody flagged anything, everything is a non-action item.
        // (This happens during a first pass with no action-item presenters.)
        mActionItems->Clear();
        mNonActionItems->Clear();
        for (Int32 i = 0; i < itemsSize; i++) {
            ///////////// is right?
            mNonActionItems->PushBack((*visibleItems)[i]);
        }
    }
    mIsActionItemsStale = FALSE;

    return NOERROR;
}

AutoPtr<MenuItemImplList> MenuBuilderBase::GetActionItems()
{
    FlagActionItems();
    return mActionItems;
}

AutoPtr<MenuItemImplList> MenuBuilderBase::GetNonActionItems()
{
    FlagActionItems();
    return mNonActionItems;
}

ECode MenuBuilderBase::ClearHeader()
{
    mHeaderIcon = NULL;
    mHeaderTitle = NULL;
    mHeaderView = NULL;

    OnItemsChanged(FALSE);
    return NOERROR;
}

void MenuBuilderBase::SetHeaderInternal(
        /* [in] */ Int32 titleRes,
        /* [in] */ ICharSequence* title,
        /* [in] */ Int32 iconRes,
        /* [in] */ IDrawable* icon,
        /* [in] */ IView* view)
{
    AutoPtr<IResources> r = GetResources();

    if (view != NULL) {
        mHeaderView = view;

        // If using a custom view, then the title and icon aren't used
        mHeaderTitle = NULL;
        mHeaderIcon = NULL;
    }
    else {
        if (titleRes > 0) {
            mHeaderTitle = NULL;
            r->GetText(titleRes, (ICharSequence**)&mHeaderTitle);
        }
        else if (title != NULL) {
            mHeaderTitle = title;
        }

        if (iconRes > 0) {
            mHeaderIcon = NULL;
            r->GetDrawable(iconRes, (IDrawable**)&mHeaderIcon);
        }
        else if (icon != NULL) {
            mHeaderIcon = icon;
        }

        // If using the title or icon, then a custom view isn't used
        mHeaderView = NULL;
    }

    // Notify of change
    OnItemsChanged(FALSE);
}

void MenuBuilderBase::SetHeaderTitleInt(
    /* [in] */ ICharSequence* title)
{
    SetHeaderInternal(0, title, 0, NULL, NULL);
}

void MenuBuilderBase::SetHeaderTitleInt(
    /* [in] */ Int32 titleRes)
{
    SetHeaderInternal(titleRes, NULL, 0, NULL, NULL);
}

void MenuBuilderBase::SetHeaderIconInt(
    /* [in] */ IDrawable* icon)
{
    SetHeaderInternal(0, NULL, 0, icon, NULL);
}

void MenuBuilderBase::SetHeaderIconInt(
    /* [in] */ Int32 iconRes)
{
    SetHeaderInternal(0, NULL, iconRes, NULL, NULL);
}

void MenuBuilderBase::SetHeaderViewInt(
    /* [in] */ IView* view)
{
    SetHeaderInternal(0, NULL, 0, NULL, view);
}

AutoPtr<ICharSequence> MenuBuilderBase::GetHeaderTitle()
{
    return mHeaderTitle;
}

AutoPtr<IDrawable> MenuBuilderBase::GetHeaderIcon()
{
    return mHeaderIcon;
}

AutoPtr<IView> MenuBuilderBase::GetHeaderView()
{
    return mHeaderView;
}

AutoPtr<IMenuBuilder> MenuBuilderBase::GetRootMenu()
{
    return THIS_PROBE(IMenuBuilder);
}

ECode MenuBuilderBase::SetCurrentMenuInfo(
    /* [in] */ IContextMenuInfo* menuInfo)
{
    mCurrentMenuInfo = menuInfo;

    return NOERROR;
}

ECode MenuBuilderBase::SetOptionalIconsVisible(Boolean visible)
{
    mOptionalIconsVisible = visible;
    return NOERROR;
}

Boolean MenuBuilderBase::GetOptionalIconsVisible()
{
    return mOptionalIconsVisible;
}

Boolean MenuBuilderBase::ExpandItemActionView(
    /* [in] */ IMenuItemImpl* item)
{
    if (mPresenters.IsEmpty()) return FALSE;

    Boolean expanded = FALSE;

    StopDispatchingItemsChanged();

    List< AutoPtr<IWeakReference> >::Iterator ator = mPresenters.Begin();
    for (; ator != mPresenters.End();) {
        AutoPtr<IMenuPresenter> presenter;
        (*ator)->Resolve(EIID_IMenuPresenter, (IInterface**)&presenter);
        if (presenter == NULL) {
            ator = mPresenters.Erase(ator);
        }
        else if ((presenter->ExpandItemActionView(
                THIS_PROBE(IMenuBuilder), item, &expanded), expanded)) {
            break;
        }
        else {
            ++ator;
        }
    }

    StartDispatchingItemsChanged();

    if (expanded) {
        mExpandedItem = item;
    }

    return expanded;
}

Boolean MenuBuilderBase::CollapseItemActionView(
    /* [in] */ IMenuItemImpl* item)
{
    if (mPresenters.IsEmpty() || mExpandedItem.Get() != item) return FALSE;

    Boolean collapsed = FALSE;

    StopDispatchingItemsChanged();

    List< AutoPtr<IWeakReference> >::Iterator ator = mPresenters.Begin();
    for (; ator != mPresenters.End();) {
        AutoPtr<IMenuPresenter> presenter;
        (*ator)->Resolve(EIID_IMenuPresenter, (IInterface**)&presenter);
        if (presenter == NULL) {
            ator = mPresenters.Erase(ator);
        } else if ((presenter->CollapseItemActionView(
                THIS_PROBE(IMenuBuilder), item, &collapsed), collapsed)) {
            break;
        }
        else {
            ++ator;
        }
    }

    StartDispatchingItemsChanged();

    if (collapsed) {
        mExpandedItem = NULL;
    }
    return collapsed;
}

AutoPtr<IMenuItemImpl> MenuBuilderBase::GetExpandedItem()
{
    return mExpandedItem;
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
