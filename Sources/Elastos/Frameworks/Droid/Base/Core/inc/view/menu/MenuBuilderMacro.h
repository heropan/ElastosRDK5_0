
#ifndef __MENUBUILDERMACRO_H__
#define __MENUBUILDERMACRO_H__


#define IMENUBUILDER_METHODS_DECL()                                              \
    CARAPI SetDefaultShowAsAction(                                               \
        /* [in] */ Int32 defaultShowAsAction,                                    \
        /* [out] */ IMenuBuilder** builder);                                     \
                                                                                 \
    CARAPI AddMenuPresenter(                                                     \
        /* [in] */ IMenuPresenter* presenter);                                   \
                                                                                 \
    CARAPI RemoveMenuPresenter(                                                  \
        /* [in] */ IMenuPresenter* presenter);                                   \
                                                                                 \
    CARAPI SavePresenterStates(                                                  \
        /* [in] */ IBundle* outState);                                           \
                                                                                 \
    CARAPI RestorePresenterStates(                                               \
        /* [in] */ IBundle* state);                                              \
                                                                                 \
    CARAPI SaveActionViewStates(                                                 \
        /* [in] */ IBundle* outStates);                                          \
                                                                                 \
    CARAPI RestoreActionViewStates(                                              \
        /* [in] */ IBundle* states);                                             \
                                                                                 \
    CARAPI StopDispatchingItemsChanged();                                        \
                                                                                 \
    CARAPI StartDispatchingItemsChanged();                                       \
                                                                                 \
    CARAPI FlagActionItems();                                                    \
                                                                                 \
    CARAPI ExpandItemActionView(                                                 \
        /* [in] */ IMenuItemImpl* item,                                          \
        /* [out] */ Boolean* expanded);                                          \
                                                                                 \
    CARAPI CollapseItemActionView(                                               \
        /* [in] */ IMenuItemImpl* item,                                          \
        /* [out] */ Boolean* collapsed);                                         \
                                                                                 \
    CARAPI GetExpandedItem(                                                      \
        /* [out] */ IMenuItemImpl** itemImpl);                                   \
                                                                                 \
    CARAPI SetCallback(                                                          \
        /* [in] */ IMenuBuilderCallback* callback);                              \
                                                                                 \
    CARAPI ClearAll();                                                           \
                                                                                 \
    CARAPI FindItemIndex(                                                        \
        /* [in] */ Int32 id,                                                     \
        /* [out] */ Int32* index);                                               \
                                                                                 \
    CARAPI FindGroupIndex(                                                       \
        /* [in] */ Int32 group,                                                  \
        /* [out] */ Int32* index);                                               \
                                                                                 \
    CARAPI FindGroupIndexEx(                                                     \
        /* [in] */ Int32 group,                                                  \
        /* [in] */ Int32 start,                                                  \
        /* [out] */ Int32* index);                                               \
                                                                                 \
    CARAPI SetShortcutsVisible(                                                  \
        /* [in] */ Boolean shortcutsVisible);                                    \
                                                                                 \
    CARAPI IsShortcutsVisible(                                                   \
        /* [out] */ Boolean* qwertyMode);                                        \
                                                                                 \
    CARAPI GetContext(                                                           \
        /* [out] */ IContext** context);                                         \
                                                                                 \
    CARAPI ChangeMenuMode();                                                     \
                                                                                 \
    CARAPI PerformItemAction(                                                    \
        /* [in] */ IMenuItem* item,                                              \
        /* [in] */ Int32 flags,                                                  \
        /* [out] */ Boolean* state);                                             \
                                                                                 \
    CARAPI CloseEx(                                                              \
        /* [in] */ Boolean allMenusAreClosing);                                  \
                                                                                 \
    CARAPI OnItemsChanged(                                                       \
        /* [in] */ Boolean structureChanged);                                    \
                                                                                 \
    CARAPI OnItemVisibleChanged(                                                 \
        /* [in] */ IMenuItemImpl* item);                                         \
                                                                                 \
    CARAPI OnItemActionRequestChanged(                                           \
        /* [in] */ IMenuItemImpl* item);                                         \
                                                                                 \
    CARAPI GetHeaderTitle(                                                       \
        /* [out] */ ICharSequence** title);                                      \
                                                                                 \
    CARAPI GetHeaderIcon(                                                        \
        /* [out] */ IDrawable** drawable);                                       \
                                                                                 \
    CARAPI GetHeaderView(                                                        \
        /* [out] */ IView** headerView);                                         \
                                                                                 \
    CARAPI GetRootMenu(                                                          \
        /* [out] */ IMenuBuilder** menu);                                        \
                                                                                 \
    CARAPI GetCallback(                                                          \
        /* [out] */ IMenuBuilderCallback** menu);                                \
                                                                                 \
    CARAPI SetCurrentMenuInfo(                                                   \
        /* [in] */ IContextMenuInfo* menuInfo);                                  \
                                                                                 \
    CARAPI SetExclusiveItemChecked(                                              \
        /* [in] */ IMenuItem* item);                                             \
                                                                                 \
    CARAPI IsQwertyMode(                                                         \
        /* [out] */ Boolean* isQwerty);                                          \
                                                                                 \
    CARAPI GetResources(                                                         \
        /* [out] */ IResources** res);                                           \
                                                                                 \
    CARAPI DispatchMenuItemSelected(                                             \
        /* [in] */ IMenuBuilder* menu,                                           \
        /* [in] */ IMenuItem* item,                                              \
        /* [out] */ Boolean* res);                                               \
                                                                                 \
    CARAPI FindItemWithShortcutForKey(                                           \
        /* [in] */ Int32 keyCode,                                                \
        /* [in] */ IKeyEvent* event,                                             \
        /* [out] */ IMenuItemImpl** impl);                                       \
                                                                                 \
    CARAPI SetOptionalIconsVisible(                                              \
        /* [in] */ Boolean visible);                                             \
                                                                                 \
    CARAPI GetOptionalIconsVisible(                                              \
        /* [out] */ Boolean* res);


#define IMENUBUILDER_METHODS_IMPL(className, superClass, overRideClass)          \
ECode className::SetDefaultShowAsAction(                                         \
    /* [in] */ Int32 defaultShowAsAction,                                        \
    /* [out] */ IMenuBuilder** builder)                                          \
{                                                                                \
    VALIDATE_NOT_NULL(builder);                                                  \
    *builder = superClass::SetDefaultShowAsAction(defaultShowAsAction);          \
                                                                                 \
    if (*builder != NULL) {                                                      \
        (*builder)->AddRef();                                                    \
    }                                                                            \
                                                                                 \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::AddMenuPresenter(                                               \
    /* [in] */ IMenuPresenter* presenter)                                        \
{                                                                                \
    return superClass::AddMenuPresenter(presenter);                                          \
}                                                                                \
                                                                                 \
ECode className::RemoveMenuPresenter(                                            \
    /* [in] */ IMenuPresenter* presenter)                                        \
{                                                                                \
    return superClass::RemoveMenuPresenter(presenter);                           \
}                                                                                \
                                                                                 \
ECode className::SavePresenterStates(                                            \
    /* [in] */ IBundle* outState)                                                \
{                                                                                \
    return superClass::SavePresenterStates(outState);                            \
}                                                                                \
                                                                                 \
ECode className::RestorePresenterStates(                                         \
    /* [in] */ IBundle* state)                                                   \
{                                                                                \
    return superClass::RestorePresenterStates(state);                            \
}                                                                                \
                                                                                 \
ECode className::SaveActionViewStates(                                           \
    /* [in] */ IBundle* outStates)                                               \
{                                                                                \
    return superClass::SaveActionViewStates(outStates);                          \
}                                                                                \
                                                                                 \
ECode className::RestoreActionViewStates(                                        \
    /* [in] */ IBundle* states)                                                  \
{                                                                                \
    return superClass::RestoreActionViewStates(states);                          \
}                                                                                \
                                                                                 \
ECode className::StopDispatchingItemsChanged()                                   \
{                                                                                \
    return superClass::StopDispatchingItemsChanged();                            \
}                                                                                \
                                                                                 \
ECode className::StartDispatchingItemsChanged()                                  \
{                                                                                \
    return superClass::StartDispatchingItemsChanged();                           \
}                                                                                \
                                                                                 \
ECode className::FlagActionItems()                                               \
{                                                                                \
    return superClass::FlagActionItems();                                        \
}                                                                                \
                                                                                 \
ECode className::ExpandItemActionView(                                           \
    /* [in] */ IMenuItemImpl* item,                                              \
    /* [out] */ Boolean* expanded)                                               \
{                                                                                \
    VALIDATE_NOT_NULL(expanded);                                                 \
    *expanded = superClass::ExpandItemActionView(item);                          \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::CollapseItemActionView(                                         \
    /* [in] */ IMenuItemImpl* item,                                              \
    /* [out] */ Boolean* collapsed)                                              \
{                                                                                \
    VALIDATE_NOT_NULL(collapsed);                                                \
    *collapsed = superClass::CollapseItemActionView(item);                       \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::GetExpandedItem(                                                \
    /* [out] */ IMenuItemImpl** itemImpl)                                        \
{                                                                                \
    VALIDATE_NOT_NULL(itemImpl);                                                 \
    *itemImpl = superClass::GetExpandedItem();                                   \
    if (*itemImpl != NULL) {                                                     \
        (*itemImpl)->AddRef();                                                   \
    }                                                                            \
                                                                                 \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::SetCallback(                                                    \
    /* [in] */ IMenuBuilderCallback* callback)                                   \
{                                                                                \
    return superClass::SetCallback(callback);                                    \
}                                                                                \
                                                                                 \
ECode className::ClearAll()                                                      \
{                                                                                \
    return superClass::ClearAll();                                               \
}                                                                                \
                                                                                 \
ECode className::FindItemIndex(                                                  \
    /* [in] */ Int32 id,                                                         \
    /* [out] */ Int32* index)                                                    \
{                                                                                \
    VALIDATE_NOT_NULL(index);                                                    \
                                                                                 \
    *index = superClass::FindItemIndex(id);                                      \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::FindGroupIndex(                                                 \
    /* [in] */ Int32 group,                                                      \
    /* [out] */ Int32* index)                                                    \
{                                                                                \
    VALIDATE_NOT_NULL(index);                                                    \
                                                                                 \
    *index = superClass::FindGroupIndex(group);                                  \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::FindGroupIndexEx(                                               \
    /* [in] */ Int32 group,                                                      \
    /* [in] */ Int32 start,                                                      \
    /* [out] */ Int32* index)                                                    \
{                                                                                \
    VALIDATE_NOT_NULL(index);                                                    \
                                                                                 \
    *index = superClass::FindGroupIndex(group, start);                           \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::SetShortcutsVisible(                                            \
    /* [in] */ Boolean shortcutsVisible)                                         \
{                                                                                \
    return superClass::SetShortcutsVisible(shortcutsVisible);                    \
}                                                                                \
                                                                                 \
ECode className::IsShortcutsVisible(                                             \
    /* [out] */ Boolean* qwertyMode)                                             \
{                                                                                \
    VALIDATE_NOT_NULL(qwertyMode);                                               \
                                                                                 \
    *qwertyMode = superClass::IsShortcutsVisible();                              \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::GetContext(                                                     \
    /* [out] */ IContext** context)                                              \
{                                                                                \
    VALIDATE_NOT_NULL(context);                                                  \
                                                                                 \
    AutoPtr<IContext> _context = superClass::GetContext();                       \
    *context = _context.Get();                                                   \
    if (*context != NULL) (*context)->AddRef();                                  \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::ChangeMenuMode()                                                \
{                                                                                \
    return superClass::ChangeMenuMode();                                         \
}                                                                                \
                                                                                 \
ECode className::PerformItemAction(                                              \
    /* [in] */ IMenuItem* item,                                                  \
    /* [in] */ Int32 flags,                                                      \
    /* [out] */ Boolean* state)                                                  \
{                                                                                \
    VALIDATE_NOT_NULL(state);                                                    \
                                                                                 \
    *state = superClass::PerformItemAction(item, flags);                         \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::CloseEx(                                                        \
    /* [in] */ Boolean allMenusAreClosing)                                       \
{                                                                                \
    return superClass::Close(allMenusAreClosing);                                \
}                                                                                \
                                                                                 \
ECode className::OnItemsChanged(                                                 \
    /* [in] */ Boolean structureChanged)                                         \
{                                                                                \
    return superClass::OnItemsChanged(structureChanged);                         \
}                                                                                \
                                                                                 \
ECode className::OnItemVisibleChanged(                                           \
    /* [in] */ IMenuItemImpl* item)                                              \
{                                                                                \
    return superClass::OnItemVisibleChanged(item);                               \
}                                                                                \
                                                                                 \
ECode className::OnItemActionRequestChanged(                                     \
    /* [in] */ IMenuItemImpl* item)                                              \
{                                                                                \
    return superClass::OnItemActionRequestChanged(item);                         \
}                                                                                \
                                                                                 \
ECode className::GetHeaderTitle(                                                 \
    /* [out] */ ICharSequence** title)                                           \
{                                                                                \
    VALIDATE_NOT_NULL(title);                                                    \
                                                                                 \
    AutoPtr<ICharSequence> t = superClass::GetHeaderTitle();                     \
    *title = t.Get();                                                            \
    if (*title != NULL) (*title)->AddRef();                                      \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::GetHeaderIcon(                                                  \
    /* [out] */ IDrawable** drawable)                                            \
{                                                                                \
    VALIDATE_NOT_NULL(drawable);                                                 \
                                                                                 \
    AutoPtr<IDrawable> d = superClass::GetHeaderIcon();                          \
    *drawable = d.Get();                                                         \
    if (*drawable != NULL) (*drawable)->AddRef();                                \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::GetHeaderView(                                                  \
    /* [out] */ IView** view)                                                    \
{                                                                                \
    VALIDATE_NOT_NULL(view);                                                     \
                                                                                 \
    AutoPtr<IView> _view = superClass::GetHeaderView();                          \
    *view = _view.Get();                                                         \
    if (*view != NULL) (*view)->AddRef();                                        \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::GetRootMenu(                                                    \
    /* [out] */ IMenuBuilder** menu)                                             \
{                                                                                \
    VALIDATE_NOT_NULL(menu);                                                     \
                                                                                 \
    AutoPtr<IMenuBuilder> _menu = superClass::GetRootMenu();                     \
    *menu = _menu.Get();                                                         \
    if (*menu != NULL) (*menu)->AddRef();                                        \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::GetCallback(                                                    \
    /* [out] */ IMenuBuilderCallback** callback)                                 \
{                                                                                \
    return superClass::GetCallback(callback);                                    \
}                                                                                \
                                                                                 \
ECode className::SetCurrentMenuInfo(                                             \
    /* [in] */ IContextMenuInfo* menuInfo)                                       \
{                                                                                \
    return superClass::SetCurrentMenuInfo(menuInfo);                             \
}                                                                                \
                                                                                 \
ECode className::SetExclusiveItemChecked(                                        \
    /* [in] */ IMenuItem* item)                                                  \
{                                                                                \
    return superClass::SetExclusiveItemChecked(item);                            \
}                                                                                \
                                                                                 \
ECode className::IsQwertyMode(                                                   \
    /* [out] */ Boolean* isQwerty)                                               \
{                                                                                \
    VALIDATE_NOT_NULL(isQwerty);                                                 \
    *isQwerty = superClass::IsQwertyMode();                                      \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::GetResources(                                                   \
    /* [out] */ IResources** res)                                                \
{                                                                                \
    VALIDATE_NOT_NULL(res);                                                      \
                                                                                 \
    AutoPtr<IResources> _res = superClass::GetResources();                       \
    *res = _res;                                                                 \
    if (*res != NULL) (*res)->AddRef();                                          \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::DispatchMenuItemSelected(                                       \
    /* [in] */ IMenuBuilder* menu,                                               \
    /* [in] */ IMenuItem* item,                                                  \
    /* [out] */ Boolean* res)                                                    \
{                                                                                \
    VALIDATE_NOT_NULL(res);                                                      \
                                                                                 \
    *res = superClass::DispatchMenuItemSelected(menu, item);                     \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::FindItemWithShortcutForKey(                                     \
    /* [in] */ Int32 keyCode,                                                    \
    /* [in] */ IKeyEvent* event,                                                 \
    /* [out] */ IMenuItemImpl** impl)                                            \
{                                                                                \
    VALIDATE_NOT_NULL(impl);                                                     \
                                                                                 \
    AutoPtr<IMenuItemImpl> _impl = superClass::FindItemWithShortcutForKey(keyCode, event);  \
    *impl = _impl;                                                               \
    if (*impl != NULL) (*impl)->AddRef();                                        \
    return NOERROR;                                                              \
}                                                                                \
                                                                                 \
ECode className::SetOptionalIconsVisible(                                        \
    /* [in] */ Boolean visible)                                                  \
{                                                                                \
    return superClass::SetOptionalIconsVisible(visible);                         \
}                                                                                \
                                                                                 \
ECode className::GetOptionalIconsVisible(                                        \
    /* [out] */ Boolean* res)                                                    \
{                                                                                \
    VALIDATE_NOT_NULL(res);                                                      \
                                                                                 \
    *res = superClass::GetOptionalIconsVisible();                                \
    return NOERROR;                                                              \
}                                                                                \

#endif //__MENUBUILDERMACRO_H__
