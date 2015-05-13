
#ifndef __MENUMACRO_H__
#define __MENUMACRO_H__


#define IMENU_METHODS_DECL()                                                               \
    CARAPI Add(                                                                            \
        /* [in] */ ICharSequence* title,                                                   \
        /* [out] */ IMenuItem** item);                                                     \
                                                                                           \
    CARAPI AddEx(                                                                          \
        /* [in] */ Int32 titleRes,                                                         \
        /* [out] */ IMenuItem** item);                                                     \
                                                                                           \
    CARAPI AddEx2(                                                                         \
        /* [in] */ Int32 groupId,                                                          \
        /* [in] */ Int32 itemId,                                                           \
        /* [in] */ Int32 order,                                                            \
        /* [in] */ ICharSequence* title,                                                   \
        /* [out] */ IMenuItem** item);                                                     \
                                                                                           \
    CARAPI AddEx3(                                                                         \
        /* [in] */ Int32 groupId,                                                          \
        /* [in] */ Int32 itemId,                                                           \
        /* [in] */ Int32 order,                                                            \
        /* [in] */ Int32 titleRes,                                                         \
        /* [out] */ IMenuItem** item);                                                     \
                                                                                           \
    CARAPI AddSubMenu(                                                                     \
        /* [in] */ ICharSequence* title,                                                   \
        /* [out] */ ISubMenu** subMenu);                                                   \
                                                                                           \
    CARAPI AddSubMenuEx(                                                                   \
        /* [in] */ Int32 titleRes,                                                         \
        /* [out] */ ISubMenu** subMenu);                                                   \
                                                                                           \
    CARAPI AddSubMenuEx2(                                                                  \
        /* [in] */ Int32 groupId,                                                          \
        /* [in] */ Int32 itemId,                                                           \
        /* [in] */ Int32 order,                                                            \
        /* [in] */ ICharSequence* title,                                                   \
        /* [out] */ ISubMenu** subMenu);                                                   \
                                                                                           \
    CARAPI AddSubMenuEx3(                                                                  \
        /* [in] */ Int32 groupId,                                                          \
        /* [in] */ Int32 itemId,                                                           \
        /* [in] */ Int32 order,                                                            \
        /* [in] */ Int32 titleRes,                                                         \
        /* [out] */ ISubMenu** subMenu);                                                   \
                                                                                           \
    CARAPI AddIntentOptions(                                                               \
        /* [in] */ Int32 groupId,                                                          \
        /* [in] */ Int32 itemId,                                                           \
        /* [in] */ Int32 order,                                                            \
        /* [in] */ IComponentName* caller,                                                 \
        /* [in] */ const ArrayOf<IIntent*> & specifics,                                    \
        /* [in] */ IIntent* intent,                                                        \
        /* [in] */ Int32 flags,                                                            \
        /* [in] */ const ArrayOf<IMenuItem*> & outSpecificItems,                           \
        /* [out] */ Int32* count);                                                         \
                                                                                           \
    CARAPI RemoveItem(                                                                     \
        /* [in] */ Int32 id);                                                              \
                                                                                           \
    CARAPI RemoveGroup(                                                                    \
        /* [in] */ Int32 groupId);                                                         \
                                                                                           \
    CARAPI Clear();                                                                        \
                                                                                           \
    CARAPI SetGroupCheckable(                                                              \
        /* [in] */ Int32 group,                                                            \
        /* [in] */ Boolean checkable,                                                      \
        /* [in] */ Boolean exclusive);                                                     \
                                                                                           \
    CARAPI SetGroupVisible(                                                                \
        /* [in] */ Int32 group,                                                            \
        /* [in] */ Boolean visible);                                                       \
                                                                                           \
    CARAPI SetGroupEnabled(                                                                \
        /* [in] */ Int32 group,                                                            \
        /* [in] */ Boolean enabled);                                                       \
                                                                                           \
    CARAPI HasVisibleItems(                                                                \
        /* [out] */ Boolean* result);                                                      \
                                                                                           \
    CARAPI FindItem(                                                                       \
        /* [in] */ Int32 id,                                                               \
        /* [out] */ IMenuItem** item);                                                     \
                                                                                           \
    CARAPI GetSize(                                                                        \
        /* [out] */ Int32* size);                                                          \
                                                                                           \
    CARAPI GetItem(                                                                        \
        /* [in] */ Int32 index,                                                            \
        /* [out] */ IMenuItem** item);                                                     \
                                                                                           \
    CARAPI Close();                                                                        \
                                                                                           \
    CARAPI PerformShortcut(                                                                \
        /* [in] */ Int32 keyCode,                                                          \
        /* [in] */ IKeyEvent* event,                                                       \
        /* [in] */ Int32 flags,                                                            \
        /* [out] */ Boolean* isShown);                                                     \
                                                                                           \
    CARAPI IsShortcutKey(                                                                  \
        /* [in] */ Int32 keyCode,                                                          \
        /* [in] */ IKeyEvent* event,                                                       \
        /* [out] */ Boolean* isShortcutKey);                                               \
                                                                                           \
    CARAPI PerformIdentifierAction(                                                        \
        /* [in] */ Int32 id,                                                               \
        /* [in] */ Int32 flags,                                                            \
        /* [out] */ Boolean* isShown);                                                     \
                                                                                           \
    CARAPI SetQwertyMode(                                                                  \
        /* [in] */ Boolean isQwerty);


#define IMENU_METHODS_IMPL(className, superClass, overRideClass)                           \
ECode className::Add(                                                                      \
    /* [in] */ ICharSequence* title,                                                       \
    /* [out] */ IMenuItem** item)                                                          \
{                                                                                          \
    VALIDATE_NOT_NULL(item);                                                               \
                                                                                           \
    AutoPtr<IMenuItem> _item = superClass::Add(title);                                     \
    *item = _item;                                                                         \
    if (*item != NULL) (*item)->AddRef();                                                  \
    return NOERROR;                                                                        \
}                                                                                          \
                                                                                           \
ECode className::AddEx(                                                                    \
    /* [in] */ Int32 titleRes,                                                             \
    /* [out] */ IMenuItem** item)                                                          \
{                                                                                          \
    VALIDATE_NOT_NULL(item);                                                               \
                                                                                           \
    AutoPtr<IMenuItem> _item = superClass::Add(titleRes);                                  \
    *item = _item;                                                                         \
    if (*item != NULL) (*item)->AddRef();                                                  \
    return NOERROR;                                                                        \
}                                                                                          \
                                                                                           \
ECode className::AddEx2(                                                                   \
    /* [in] */ Int32 groupId,                                                              \
    /* [in] */ Int32 itemId,                                                               \
    /* [in] */ Int32 order,                                                                \
    /* [in] */ ICharSequence* title,                                                       \
    /* [out] */ IMenuItem** item)                                                          \
{                                                                                          \
    VALIDATE_NOT_NULL(item);                                                               \
                                                                                           \
    AutoPtr<IMenuItem> _item = superClass::Add(groupId, itemId, order, title);             \
    *item = _item;                                                                         \
    if (*item != NULL) (*item)->AddRef();                                                  \
    return NOERROR;                                                                        \
}                                                                                          \
                                                                                           \
ECode className::AddEx3(                                                                   \
    /* [in] */ Int32 groupId,                                                              \
    /* [in] */ Int32 itemId,                                                               \
    /* [in] */ Int32 order,                                                                \
    /* [in] */ Int32 titleRes,                                                             \
    /* [out] */ IMenuItem** item)                                                          \
{                                                                                          \
    VALIDATE_NOT_NULL(item);                                                               \
                                                                                           \
    AutoPtr<IMenuItem> _item = superClass::Add(groupId, itemId, order, titleRes);          \
    *item = _item;                                                                         \
    if (*item != NULL) (*item)->AddRef();                                                  \
    return NOERROR;                                                                        \
}                                                                                          \
                                                                                           \
ECode className::AddSubMenu(                                                               \
    /* [in] */ ICharSequence* title,                                                       \
    /* [out] */ ISubMenu** subMenu)                                                        \
{                                                                                          \
    VALIDATE_NOT_NULL(subMenu);                                                            \
                                                                                           \
    AutoPtr<ISubMenu> _subMenu = superClass::AddSubMenu(title);                            \
    *subMenu = _subMenu;                                                                   \
    if (*subMenu != NULL) (*subMenu)->AddRef();                                            \
    return NOERROR;                                                                        \
}                                                                                          \
                                                                                           \
ECode className::AddSubMenuEx(                                                             \
    /* [in] */ Int32 titleRes,                                                             \
    /* [out] */ ISubMenu** subMenu)                                                        \
{                                                                                          \
    VALIDATE_NOT_NULL(subMenu);                                                            \
                                                                                           \
    AutoPtr<ISubMenu> _subMenu = superClass::AddSubMenu(titleRes);                         \
    *subMenu = _subMenu;                                                                   \
    if (*subMenu != NULL) (*subMenu)->AddRef();                                            \
    return NOERROR;                                                                        \
}                                                                                          \
                                                                                           \
ECode className::AddSubMenuEx2(                                                            \
    /* [in] */ Int32 groupId,                                                              \
    /* [in] */ Int32 itemId,                                                               \
    /* [in] */ Int32 order,                                                                \
    /* [in] */ ICharSequence* title,                                                       \
    /* [out] */ ISubMenu** subMenu)                                                        \
{                                                                                          \
    VALIDATE_NOT_NULL(subMenu);                                                            \
                                                                                           \
    AutoPtr<ISubMenu> _subMenu = superClass::AddSubMenu(groupId, itemId, order, title);    \
    *subMenu = _subMenu;                                                                   \
    if (*subMenu != NULL) (*subMenu)->AddRef();                                            \
    return NOERROR;                                                                        \
}                                                                                          \
                                                                                           \
ECode className::AddSubMenuEx3(                                                            \
    /* [in] */ Int32 groupId,                                                              \
    /* [in] */ Int32 itemId,                                                               \
    /* [in] */ Int32 order,                                                                \
    /* [in] */ Int32 titleRes,                                                             \
    /* [out] */ ISubMenu** subMenu)                                                        \
{                                                                                          \
    VALIDATE_NOT_NULL(subMenu);                                                            \
                                                                                           \
    AutoPtr<ISubMenu> _subMenu = superClass::AddSubMenu(groupId, itemId, order, titleRes); \
    *subMenu = _subMenu;                                                                   \
    if (*subMenu != NULL) (*subMenu)->AddRef();                                            \
    return NOERROR;                                                                        \
}                                                                                          \
                                                                                           \
ECode className::AddIntentOptions(                                                         \
    /* [in] */ Int32 groupId,                                                              \
    /* [in] */ Int32 itemId,                                                               \
    /* [in] */ Int32 order,                                                                \
    /* [in] */ IComponentName* caller,                                                     \
    /* [in] */ const ArrayOf<IIntent*> & specifics,                                        \
    /* [in] */ IIntent* intent,                                                            \
    /* [in] */ Int32 flags,                                                                \
    /* [in] */ const ArrayOf<IMenuItem*> & outSpecificItems,                               \
    /* [out] */ Int32* count)                                                              \
{                                                                                          \
    VALIDATE_NOT_NULL(count);                                                              \
                                                                                           \
    *count = superClass::AddIntentOptions(groupId, itemId, order, caller,                  \
            specifics, intent, flags, outSpecificItems);                                   \
    return NOERROR;                                                                        \
}                                                                                          \
                                                                                           \
ECode className::RemoveItem(                                                               \
    /* [in] */ Int32 id)                                                                   \
{                                                                                          \
    return superClass::RemoveItem(id);                                                     \
}                                                                                          \
                                                                                           \
ECode className::RemoveGroup(                                                              \
    /* [in] */ Int32 groupId)                                                              \
{                                                                                          \
    return superClass::RemoveGroup(groupId);                                               \
}                                                                                          \
                                                                                           \
ECode className::Clear()                                                                   \
{                                                                                          \
    return superClass::Clear();                                                            \
}                                                                                          \
                                                                                           \
ECode className::SetGroupCheckable(                                                        \
    /* [in] */ Int32 group,                                                                \
    /* [in] */ Boolean checkable,                                                          \
    /* [in] */ Boolean exclusive)                                                          \
{                                                                                          \
    return superClass::SetGroupCheckable(group, checkable, exclusive);                     \
}                                                                                          \
                                                                                           \
ECode className::SetGroupVisible(                                                          \
    /* [in] */ Int32 group,                                                                \
    /* [in] */ Boolean visible)                                                            \
{                                                                                          \
    return superClass::SetGroupVisible(group, visible);                                    \
}                                                                                          \
                                                                                           \
ECode className::SetGroupEnabled(                                                          \
    /* [in] */ Int32 group,                                                                \
    /* [in] */ Boolean enabled)                                                            \
{                                                                                          \
    return superClass::SetGroupEnabled(group, enabled);                                    \
}                                                                                          \
                                                                                           \
ECode className::HasVisibleItems(                                                          \
    /* [out] */ Boolean* result)                                                           \
{                                                                                          \
    VALIDATE_NOT_NULL(result);                                                             \
                                                                                           \
    *result = superClass::HasVisibleItems();                                               \
    return NOERROR;                                                                        \
}                                                                                          \
                                                                                           \
ECode className::FindItem(                                                                 \
    /* [in] */ Int32 id,                                                                   \
    /* [out] */ IMenuItem** item)                                                          \
{                                                                                          \
    VALIDATE_NOT_NULL(item);                                                               \
                                                                                           \
    AutoPtr<IMenuItem> _item = superClass::FindItem(id);                                   \
    *item = _item;                                                                         \
    if (*item != NULL) (*item)->AddRef();                                                  \
    return NOERROR;                                                                        \
}                                                                                          \
                                                                                           \
ECode className::GetSize(                                                                  \
    /* [out] */ Int32* size)                                                               \
{                                                                                          \
    VALIDATE_NOT_NULL(size);                                                               \
                                                                                           \
    *size = superClass::GetSize();                                                         \
    return NOERROR;                                                                        \
}                                                                                          \
                                                                                           \
ECode className::GetItem(                                                                  \
    /* [in] */ Int32 index,                                                                \
    /* [out] */ IMenuItem** item)                                                          \
{                                                                                          \
    VALIDATE_NOT_NULL(item);                                                               \
                                                                                           \
    AutoPtr<IMenuItem> _item = superClass::GetItem(index);                                 \
    *item = _item;                                                                         \
    if (*item != NULL) (*item)->AddRef();                                                  \
    return NOERROR;                                                                        \
}                                                                                          \
                                                                                           \
ECode className::Close()                                                                   \
{                                                                                          \
    return superClass::Close();                                                            \
}                                                                                          \
                                                                                           \
ECode className::PerformShortcut(                                                          \
    /* [in] */ Int32 keyCode,                                                              \
    /* [in] */ IKeyEvent* event,                                                           \
    /* [in] */ Int32 flags,                                                                \
    /* [out] */ Boolean* isShown)                                                          \
{                                                                                          \
    VALIDATE_NOT_NULL(isShown);                                                            \
                                                                                           \
    *isShown = superClass::PerformShortcut(keyCode, event, flags);                         \
    return NOERROR;                                                                        \
}                                                                                          \
                                                                                           \
ECode className::IsShortcutKey(                                                            \
    /* [in] */ Int32 keyCode,                                                              \
    /* [in] */ IKeyEvent* event,                                                           \
    /* [out] */ Boolean* isShortcutKey)                                                    \
{                                                                                          \
    VALIDATE_NOT_NULL(isShortcutKey);                                                      \
                                                                                           \
    *isShortcutKey = superClass::IsShortcutKey(keyCode, event);                            \
    return NOERROR;                                                                        \
}                                                                                          \
                                                                                           \
ECode className::PerformIdentifierAction(                                                  \
    /* [in] */ Int32 id,                                                                   \
    /* [in] */ Int32 flags,                                                                \
    /* [out] */ Boolean* isShown)                                                          \
{                                                                                          \
    VALIDATE_NOT_NULL(isShown);                                                            \
                                                                                           \
    *isShown = superClass::PerformIdentifierAction(id, flags);                             \
    return NOERROR;                                                                        \
}                                                                                          \
                                                                                           \
ECode className::SetQwertyMode(                                                            \
    /* [in] */ Boolean isQwerty)                                                           \
{                                                                                          \
    return superClass::SetQwertyMode(isQwerty);                                            \
}

#endif //__MENUMACRO_H__