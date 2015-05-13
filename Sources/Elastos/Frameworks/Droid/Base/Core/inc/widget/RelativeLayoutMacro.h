#ifndef __RELATIVELAYOUTMACRO_H__
#define __RELATIVELAYOUTMACRO_H__

#define IRELATIVELAYOUT_METHODS_DECL()                                         \
    CARAPI SetIgnoreGravity(                                                    \
        /* [in] */ Int32 viewId);                                               \
                                                                                \
    CARAPI GetGravity(                                                          \
        /* [out] */ Int32* gravity);                                            \
                                                                                \
    CARAPI SetGravity(                                                          \
        /* [in] */ Int32 gravity);                                              \
                                                                                \
    CARAPI SetHorizontalGravity(                                                \
        /* [in] */ Int32 horizontalGravity);                                    \
                                                                                \
    CARAPI SetVerticalGravity(                                                  \
        /* [in] */ Int32 verticalGravity);


#define IRELATIVELAYOUT_METHODS_IMPL(className, superClass)                    \
ECode className::SetIgnoreGravity(                                              \
    /* [in] */ Int32 viewId)                                                    \
{                                                                               \
    return superClass::SetIgnoreGravity(viewId);                               \
}                                                                               \
                                                                                \
ECode className::GetGravity(                                                    \
    /* [out] */ Int32* gravity)                                                 \
        {                                                                       \
    VALIDATE_NOT_NULL(gravity);                                                 \
    *gravity = superClass::GetGravity();                                        \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetGravity(                                                    \
    /* [in] */ Int32 gravity)                                                   \
{                                                                               \
    return superClass::SetGravity(gravity);                                    \
}                                                                               \
                                                                                \
ECode className::SetHorizontalGravity(                                          \
    /* [in] */ Int32 horizontalGravity)                                         \
{                                                                               \
    return superClass::SetHorizontalGravity(horizontalGravity);                \
}                                                                               \
                                                                                \
ECode className::SetVerticalGravity(                                            \
    /* [in] */ Int32 verticalGravity)                                           \
{                                                                               \
    return superClass::SetVerticalGravity(verticalGravity);                    \
}


#define IRELATIVELAYOUTLP_METHODS_DECL()                                       \
    CARAPI AddRule(                                                             \
        /* [in] */ Int32 verb);                                                 \
                                                                                \
    CARAPI AddRuleEx(                                                           \
        /* [in] */ Int32 verb,                                                  \
        /* [in] */ Int32 anchor);                                               \
                                                                                \
    CARAPI RemoveRule(                                                          \
         /* [in] */ Int32 verb);                                                \
                                                                                \
    CARAPI GetRules(                                                            \
        /* [out, callee] */ ArrayOf<Int32>** rules);                            \
                                                                                \
    CARAPI GetRulesEx(                                                          \
        /* [in] */ Int32 layoutDirection,                                       \
        /* [out, callee] */ ArrayOf<Int32>** rules);                            \
                                                                                \
    CARAPI SetAlignWithParent(                                                  \
        /* [in] */ Boolean align);                                              \
                                                                                \
    CARAPI GetAlignWithParent(                                                  \
        /* [out] */ Boolean* align);


#define IRELATIVELAYOUTLP_METHODS_IMPL(className, superClass)                  \
ECode className::AddRule(                                                       \
    /* [in] */ Int32 verb)                                                      \
{                                                                               \
    return superClass::AddRule(verb);                                          \
}                                                                               \
                                                                                \
ECode className::AddRuleEx(                                                     \
    /* [in] */ Int32 verb,                                                      \
    /* [in] */ Int32 anchor)                                                    \
{                                                                               \
    return superClass::AddRule(verb, anchor);                                  \
}                                                                               \
                                                                                \
ECode className::RemoveRule(                                                    \
     /* [in] */ Int32 verb)                                                     \
{                                                                               \
    return superClass::RemoveRule(verb);                                       \
}                                                                               \
                                                                                \
ECode className::GetRules(                                                      \
    /* [out, callee] */ ArrayOf<Int32>** rules)                                 \
{                                                                               \
    VALIDATE_NOT_NULL(rules);                                                   \
    AutoPtr<ArrayOf<Int32> > tmp = superClass::GetRules();                      \
    *rules = tmp;                                                               \
    INTERFACE_ADDREF(*rules);                                                   \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::GetRulesEx(                                                    \
    /* [in] */ Int32 layoutDirection,                                           \
    /* [out, callee] */ ArrayOf<Int32>** rules)                                 \
{                                                                               \
    VALIDATE_NOT_NULL(rules);                                                   \
    AutoPtr<ArrayOf<Int32> > tmp = superClass::GetRules(layoutDirection);       \
    *rules = tmp;                                                               \
    INTERFACE_ADDREF(*rules);                                                   \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetAlignWithParent(                                            \
    /* [in] */ Boolean align)                                                   \
{                                                                               \
    return superClass::SetAlignWithParent(align);                              \
}                                                                               \
                                                                                \
ECode className::GetAlignWithParent(                                            \
    /* [out] */ Boolean* align)                                                 \
{                                                                               \
    VALIDATE_NOT_NULL(align);                                                   \
    *align = superClass::GetAlignWithParent();                                  \
    return NOERROR;                                                            \
}

#endif //__RELATIVELAYOUTMACRO_H__
