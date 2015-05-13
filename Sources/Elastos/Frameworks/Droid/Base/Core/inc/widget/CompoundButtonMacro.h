#ifndef __COMPOUNDBUTTONMACRO_H__
#define __COMPOUNDBUTTONMACRO_H__

using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Widget::ICompoundButtonOnCheckedChangeListener;

#define ICOMPOUNDBUTTON_METHODS_DECL()                                         \
    CARAPI SetOnCheckedChangeListener(                                          \
        /* [in] */ ICompoundButtonOnCheckedChangeListener* listener);           \
                                                                                \
    CARAPI SetOnCheckedChangeWidgetListener(                                    \
        /* [in] */ ICompoundButtonOnCheckedChangeListener* listener);           \
                                                                                \
    CARAPI SetButtonDrawable(                                                   \
        /* [in] */ Int32 resid);                                                \
                                                                                \
    CARAPI SetButtonDrawableEx(                                                 \
        /* [in] */ IDrawable* d);


#define ICOMPOUNDBUTTON_METHODS_IMPL(className, superClass)                    \
ECode className::SetOnCheckedChangeListener(                                    \
    /* [in] */ ICompoundButtonOnCheckedChangeListener* listener)                \
{                                                                               \
    return superClass::SetOnCheckedChangeListener(listener);                   \
}                                                                               \
                                                                                \
ECode className::SetOnCheckedChangeWidgetListener(                              \
    /* [in] */ ICompoundButtonOnCheckedChangeListener* listener)                \
{                                                                               \
    return superClass::SetOnCheckedChangeWidgetListener(listener);             \
}                                                                               \
                                                                                \
ECode className::SetButtonDrawable(                                             \
    /* [in] */ Int32 resid)                                                     \
{                                                                               \
    return superClass::SetButtonDrawable(resid);                               \
}                                                                               \
                                                                                \
ECode className::SetButtonDrawableEx(                                           \
    /* [in] */ IDrawable* d)                                                    \
{                                                                               \
    return superClass::SetButtonDrawable(d);                                   \
}

#define ICHECKABLE_METHODS_DECL()                                              \
    CARAPI SetChecked(                                                          \
        /* [in] */ Boolean checked);                                            \
                                                                                \
    CARAPI IsChecked(                                                           \
        /* [out] */ Boolean* isChecked);                                        \
                                                                                \
    CARAPI Toggle();


#define ICHECKABLE_METHODS_IMPL(className, superClass)                         \
ECode className::SetChecked(                                                    \
    /* [in] */ Boolean checked)                                                 \
{                                                                               \
    return superClass::SetChecked(checked);                                    \
}                                                                               \
                                                                                \
ECode className::IsChecked(                                                     \
    /* [out] */ Boolean* isChecked)                                             \
{                                                                               \
    VALIDATE_NOT_NULL(isChecked);                                               \
    *isChecked = superClass::IsChecked();                                       \
                                                                                \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::Toggle()                                                       \
{                                                                               \
    return superClass::Toggle();                                               \
}

#endif //__COMPOUNDBUTTONMACRO_H__

