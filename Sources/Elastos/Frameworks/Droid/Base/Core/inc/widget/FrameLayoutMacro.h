#ifndef __FRAMELAYOUTMACRO_H__
#define __FRAMELAYOUTMACRO_H__

#define IFRAMELAYOUT_METHODS_DECL()                                          \
                                                                            \
        CARAPI GetForegroundGravity(                                        \
            /* [out] */ Int32* foregroundGravity);                          \
                                                                            \
        CARAPI SetForegroundGravity(                                        \
            /* [in] */ Int32 foregroundGravity);                            \
                                                                            \
        CARAPI SetForeground(                                               \
            /* [in] */ IDrawable* drawable);                                \
                                                                            \
        CARAPI GetForeground(                                               \
            /* [out] */ IDrawable** foreground);                            \
                                                                            \
        CARAPI GetMeasureAllChildren(                                       \
            /* [out] */ Boolean* measureAll);                               \
                                                                            \
        CARAPI SetMeasureAllChildren(                                       \
            /* [in] */ Boolean measureAll);                                 \
                                                                            \
        CARAPI GetConsiderGoneChildrenWhenMeasuring(                        \
            /* [out] */ Boolean* measureAll);

#define IFRAMELAYOUT_METHODS_IMPL(className, superClass)                     \
ECode className::GetForegroundGravity(                                      \
    /* [out] */ Int32* foregroundGravity)                                   \
{                                                                           \
    VALIDATE_NOT_NULL(foregroundGravity);                                   \
    *foregroundGravity = superClass::GetForegroundGravity();                \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetForegroundGravity(                                      \
    /* [in] */ Int32 foregroundGravity)                                     \
{                                                                           \
    return superClass::SetForegroundGravity(foregroundGravity);             \
}                                                                           \
                                                                            \
ECode className::SetForeground(                                             \
    /* [in] */ IDrawable* drawable)                                         \
{                                                                           \
    return superClass::SetForeground(drawable);                             \
}                                                                           \
                                                                            \
ECode className::GetForeground(                                             \
    /* [out] */ IDrawable** foreground)                                     \
{                                                                           \
    VALIDATE_NOT_NULL(foreground);                                          \
    AutoPtr<IDrawable> temp = superClass::GetForeground();                  \
    *foreground = temp;                                                     \
    if(*foreground)                                                         \
        (*foreground)->AddRef();                                            \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::GetMeasureAllChildren(                                     \
    /* [out] */ Boolean* measureAll)                                        \
{                                                                           \
    VALIDATE_NOT_NULL(measureAll);                                          \
    *measureAll = superClass::GetMeasureAllChildren();                      \
    return NOERROR;                                                         \
}                                                                           \
                                                                            \
ECode className::SetMeasureAllChildren(                                     \
    /* [in] */ Boolean measureAll)                                          \
{                                                                           \
    return superClass::SetMeasureAllChildren(measureAll);                   \
}                                                                           \
                                                                            \
ECode className::GetConsiderGoneChildrenWhenMeasuring(                      \
    /* [out] */ Boolean* measureAll)                                        \
{                                                                           \
    VALIDATE_NOT_NULL(measureAll);                                          \
    *measureAll = superClass::GetConsiderGoneChildrenWhenMeasuring();       \
    return NOERROR;                                                         \
}


#endif
