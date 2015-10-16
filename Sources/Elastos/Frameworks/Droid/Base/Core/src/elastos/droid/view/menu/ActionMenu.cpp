
#include "elastos/droid/view/menu/ActionMenu.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/content/CComponentName.h"
#include "elastos/droid/view/menu/CActionMenuItem.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::View::Menu::CActionMenuItem;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

ActionMenu::ActionMenu(
    /* [in] */ IContext* context)
    : mContext(context)
    , mIsQwerty(FALSE)
{
    //mItems = new ArrayList<ActionMenuItem>();
}

AutoPtr<IContext> ActionMenu::GetContext()
{
    return mContext;
}

ECode ActionMenu::Add(
    /* [in] */ ICharSequence* title,
    /* [out] */ IMenuItem** item)
{
    return Add(0, 0, 0, title, item);
}

ECode ActionMenu::Add(
    /* [in] */ Int32 titleRes,
    /* [out] */ IMenuItem** item)
{
    return Add(0, 0, 0, titleRes, item);
}

ECode ActionMenu::Add(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ Int32 titleRes,
    /* [out] */ IMenuItem** item)
{
    AutoPtr<IResources> res;
    mContext->GetResources((IResources**)&res);
    assert(res != NULL);

    String str;
    res->GetString(titleRes, &str);

    AutoPtr<ICharSequence> title;
    CStringWrapper::New(str, (ICharSequence**)&title);
    return Add(groupId, itemId, order, title, item);
}

ECode ActionMenu::Add(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ ICharSequence* title,
    /* [out] */ IMenuItem** item)
{
    assert(item != NULL);
    AutoPtr<IActionMenuItem> res;
    CActionMenuItem::New(GetContext(), groupId, itemId, 0, order, title, (IActionMenuItem**)&res);
    *item = IMenuItem::Probe(res);
    REFCOUNT_ADD(*item);
    mItems.Insert(order, (IActionMenuItem*)*item);

    return NOERROR;
}

ECode ActionMenu::AddIntentOptions(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ IComponentName* caller,
    /* [in] */ const ArrayOf<IIntent*>& specifics,
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ const ArrayOf<IMenuItem*>& outSpecificItems,
    /* [out] */ Int32* count)
{
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IObjectContainer> lri;
    pm->QueryIntentActivityOptions(caller, (ArrayOf<IIntent*>*)&specifics, intent, 0, (IObjectContainer**)&lri);
    Int32 N = 0;
    if (lri != NULL) {
        lri->GetObjectCount(&N);
    }

    if ((flags & FLAG_APPEND_TO_GROUP) == 0) {
        RemoveGroup(groupId);
    }

    AutoPtr<IObjectEnumerator> objEnum;
    lri->GetObjectEnumerator((IObjectEnumerator**)&objEnum);

    Boolean hasNext = FALSE;
    while (objEnum->MoveNext(&hasNext), hasNext) {
        AutoPtr<IResolveInfo> ri;
        objEnum->Current((IInterface**)&ri);
        assert(ri != NULL);

        AutoPtr<IIntent> rintent;
        Int32 index = 0;
        ri->GetSpecificIndex(&index);
        CIntent::New(index < 0 ? intent : specifics[index], (IIntent**)&rintent);
        assert(rintent != NULL);

        AutoPtr<IActivityInfo> info;
        ri->GetActivityInfo((IActivityInfo**)&info);
        assert(info != NULL);

        String name;
        info->GetName(&name);

        AutoPtr<IApplicationInfo> appInfo;
        info->GetApplicationInfo((IApplicationInfo**)&appInfo);
        assert(appInfo != NULL);

        String packageName;
        appInfo->GetPackageName(&packageName);

        AutoPtr<IComponentName> cName;
        CComponentName::New(packageName, name, (IComponentName**)&cName);

        rintent->SetComponent(cName);
        AutoPtr<IMenuItem> item;
        AutoPtr<ICharSequence> label;
        ri->LoadLabel(pm, (ICharSequence**)&label);
        Add(groupId, itemId, order, label, (IMenuItem**)&item);

        AutoPtr<IDrawable> icon;
        ri->LoadIcon(pm, (IDrawable**)&icon);
        item->SetIcon(icon);
        item->SetIntent(rintent);

        if (outSpecificItems.GetLength() > 0 && index >= 0) {
            (*((ArrayOf<IMenuItem*>*)&outSpecificItems))[index] = item;
        }
    }

    return N;
}

ECode ActionMenu::AddSubMenu(
    /* [in] */ ICharSequence* title,
    /* [out] */ ISubMenu** menu)
{
    assert(menu != NULL);
    *menu = NULL;
    // TODO Implement submenus
    return NOERROR;
}

ECode ActionMenu::AddSubMenu(
    /* [in] */ Int32 titleRes,
    /* [out] */ ISubMenu** menu)
{
    assert(menu != NULL);
    *menu = NULL;
    // TODO Implement submenus
    return NOERROR;
}

ECode ActionMenu::AddSubMenu(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ ICharSequence* title,
    /* [out] */ ISubMenu** menu)
{
    assert(menu != NULL);
    *menu = NULL;
    // TODO Implement submenus
    return NOERROR;
}

ECode ActionMenu::AddSubMenu(
    /* [in] */ Int32 groupId,
    /* [in] */ Int32 itemId,
    /* [in] */ Int32 order,
    /* [in] */ Int32 titleRes,
    /* [out] */ ISubMenu** menu)
{
    assert(menu != NULL);
    *menu = NULL;
    // TODO Implement submenus
    return NOERROR;
}

ECode ActionMenu::Clear()
{
    mItems.Clear();
    return NOERROR;
}

ECode ActionMenu::Close()
{
    return NOERROR;
}

Int32 ActionMenu::FindItemIndex(
    /* [in] */ Int32 id)
{
    List<AutoPtr<IActionMenuItem> >::Iterator ator = mItems.Begin();
    Int32 index = 0;
    for (; ator != mItems.End(); ++ator, index++) {
        AutoPtr<IActionMenuItem> item = *ator;
        assert(item != NULL);
        Int32 _id = 0;
        item->GetItemId(&_id);
        if (_id == id) {
            return index;
        }
    }

    return -1;
}

ECode ActionMenu::FindItem(
    /* [in] */ Int32 id,
    /* [out] */ IMenuItem** item)
{
    assert(item != NULL);
    *item = mItems[FindItemIndex(id)];
    REFCOUNT_ADD(*item);

    return NOERROR;
}

ECode ActionMenu::GetItem(
    /* [in] */ Int32 index,
    /* [out] */ IMenuItem** item)
{
    assert(item != NULL);
    *item = mItems[index];
    REFCOUNT_ADD(*item);

    return NOERROR;
}

ECode ActionMenu::HasVisibleItems(
    /* [out] */ Boolean* has)
{
    assert(has != NULL);
    const Int32 itemCount = mItems.GetSize();

    Boolean visible = FALSE;
    for (Int32 i = 0; i < itemCount; i++) {
        if (mItems[i]->IsVisible(&visible), visible) {
            *has = TRUE;
            return NOERROR;
        }
    }

    *has = FALSE;
    return NOERROR;
}

AutoPtr<IActionMenuItem> ActionMenu::FindItemWithShortcut(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    // TODO Make this smarter.
    Boolean qwerty = mIsQwerty;
    const Int32 itemCount = mItems.GetSize();

    for (Int32 i = 0; i < itemCount; i++) {
        AutoPtr<IActionMenuItem> item = mItems[i];
        Char32 shortcut = 0;
        if (qwerty) {
            item->GetAlphabeticShortcut(&shortcut);
        }
        else {
            item->GetNumericShortcut(&shortcut);
        }

        if (keyCode == (Int32)shortcut) {
            return item;
        }
    }

    return NULL;
}

ECode ActionMenu::IsShortcutKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* isShort)
{
    assert(isShort != NULL);
    *isShort = FindItemWithShortcut(keyCode, event) != NULL;
    return NOERROR;
}

ECode ActionMenu::PerformIdentifierAction(
    /* [in] */ Int32 id,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* perform)
{
    assert(perform != NULL);
    const Int32 index = FindItemIndex(id);
    if (index < 0) {
        *perform = FALSE;
        return NOERROR;
    }

    AutoPtr<IActionMenuItem> item = mItems[index];
    assert(item != NULL);
    Boolean res = FALSE;
    *perform = item->Invoke(&res);

    return NOERROR;
}

ECode ActionMenu::PerformShortcut(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* perform)
{
    assert(perform != NULL);
    AutoPtr<IActionMenuItem> item = FindItemWithShortcut(keyCode, event);
    if (item == NULL) {
        *perform = FALSE;
        return NOERROR;
    }
    Boolean res = FALSE;
    *perform = item->Invoke(&res);

    return NOERROR;
}

ECode ActionMenu::RemoveGroup(
    /* [in] */ Int32 groupId)
{
    List<AutoPtr<IActionMenuItem> >::Iterator ator = mItems.Begin();
    for (; ator != mItems.End();) {
        AutoPtr<IActionMenuItem> item = *ator;

        assert(item != NULL);
        Int32 id = 0;
        item->GetGroupId(&id);

        if (id == groupId) {
            ator = mItems.Erase(ator);
        }
        else {
            ator++;
        }
    }

    return NOERROR;
}

ECode ActionMenu::RemoveItem(
    /* [in] */ Int32 id)
{
    mItems.Remove(FindItemIndex(id));
    return NOERROR;
}

ECode ActionMenu::SetGroupCheckable(
    /* [in] */ Int32 group,
    /* [in] */ Boolean checkable,
    /* [in] */ Boolean exclusive)
{
    List<AutoPtr<IActionMenuItem> >::Iterator ator = mItems.Begin();
    for (; ator != mItems.End(); ++ator) {
        AutoPtr<IActionMenuItem> item = *ator;
        assert(item != NULL);
        Int32 id = 0;
        item->GetGroupId(&id);

        if (id == group) {
            item->SetCheckable(checkable);
            item->SetExclusiveCheckable(exclusive);
        }
    }

    return NOERROR;
}

ECode ActionMenu::SetGroupEnabled(
    /* [in] */ Int32 group,
    /* [in] */ Boolean enabled)
{
    List<AutoPtr<IActionMenuItem> >::Iterator ator = mItems.Begin();
    for (; ator != mItems.End(); ++ator) {
        AutoPtr<IActionMenuItem> item = *ator;
        assert(item != NULL);

        Int32 id = 0;
        item->GetGroupId(&id);

        if (id == group) {
            item->SetEnabled(enabled);
        }
    }

    return NOERROR;
}

ECode ActionMenu::SetGroupVisible(
    /* [in] */ Int32 group,
    /* [in] */ Boolean visible)
{
    const Int32 itemCount = mItems.GetSize();

    for (Int32 i = 0; i < itemCount; i++) {
        AutoPtr<IActionMenuItem> item = mItems[i];
        assert(item != NULL);

        Int32 id = 0;
        item->GetGroupId(&id);

        if (id == group) {
            item->SetVisible(visible);
        }
    }

    return NOERROR;
}

ECode ActionMenu::SetQwertyMode(
    /* [in] */ Boolean isQwerty)
{
    mIsQwerty = isQwerty;
    return NOERROR;
}

Int32 ActionMenu::Size()
{
    return mItems.GetSize();
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
