#ifndef  __VALUEANIMATORMACRO_H__
#define  __VALUEANIMATORMACRO_H__

#define IVALUEANIMATOR_METHOD_DECL()                                           \
    CARAPI SetInt32Values(                                                        \
        /* [in] */ ArrayOf<Int32>* values);                                     \
                                                                                \
    CARAPI SetFloatValues(                                                      \
        /* [in] */ ArrayOf<Float>* values);                                     \
                                                                                \
    CARAPI SetObjectValues(                                                     \
        /* [in] */ ArrayOf<IInterface*>* values);                               \
                                                                                \
    CARAPI SetValues(                                                           \
        /* [in] */ ArrayOf<IPropertyValuesHolder*>* values);                    \
                                                                                \
    CARAPI GetValues(                                                           \
        /* [out, callee] */ ArrayOf<IPropertyValuesHolder*>** values);           \
                                                                                \
    CARAPI SetCurrentPlayTime(                                                  \
        /* [in] */ Int64 playTime);                                             \
                                                                                \
    CARAPI GetCurrentPlayTime(                                                  \
        /* [out] */ Int64* playeTime);                                          \
                                                                                \
    CARAPI GetAnimatedValue(                                                    \
        /* [out] */ IInterface** value);                                        \
                                                                                \
    CARAPI GetAnimatedValueEx(                                                  \
        /* [in] */ const String& propertyName,                                         \
        /* [out] */ IInterface** value);                                        \
                                                                                \
    CARAPI SetRepeatCount(                                                      \
        /* [in] */ Int32 value);                                                \
                                                                                \
    CARAPI GetRepeatCount(                                                      \
        /* [in] */ Int32* count);                                               \
                                                                                \
    CARAPI SetRepeatMode(                                                       \
        /* [in] */ Int32 value);                                                \
                                                                                \
    CARAPI GetRepeatMode(                                                       \
        /* [out] */ Int32* mode);                                               \
                                                                                \
    CARAPI AddUpdateListener(                                                   \
        /* [in] */ IAnimatorUpdateListener* listener);                          \
                                                                                \
    CARAPI RemoveAllUpdateListeners();                                          \
                                                                                \
    CARAPI RemoveUpdateListener(                                                \
        /* [in] */ IAnimatorUpdateListener* listener);                          \
                                                                                \
    CARAPI GetInterpolator(                                                     \
        /* [out] */ ITimeInterpolator** polator);                               \
                                                                                \
    CARAPI SetEvaluator(                                                        \
        /* [in] */ ITypeEvaluator* value);                                      \
                                                                                \
    CARAPI Reverse();                                                           \
                                                                                \
    CARAPI GetAnimatedFraction(                                                 \
        /* [out] */ Float* farction);

#define IVALUEANIMATOR_METHOD_IMPL(className, superClass)                     \
ECode className::SetInt32Values(                                                  \
    /* [in] */ ArrayOf<Int32>* values)                                          \
{                                                                               \
    return superClass::SetInt32Values(values);                                   \
}                                                                               \
                                                                                \
ECode className::SetFloatValues(                                                \
    /* [in] */ ArrayOf<Float>* values)                                          \
{                                                                               \
    return superClass::SetFloatValues(values);                                 \
}                                                                               \
                                                                                \
ECode className::SetObjectValues(                                               \
    /* [in] */ ArrayOf<IInterface*>* values)                                    \
{                                                                               \
    return superClass::SetObjectValues(values);                                \
}                                                                               \
                                                                                \
ECode className::SetValues(                                                     \
    /* [in] */ ArrayOf<IPropertyValuesHolder*>* values)                         \
{                                                                               \
    return superClass::SetValues(values);                                      \
}                                                                               \
                                                                                \
ECode className::GetValues(                                                     \
    /* [out, callee] */ ArrayOf<IPropertyValuesHolder*>** values)                \
{                                                                               \
    VALIDATE_NOT_NULL(values);                                                  \
    AutoPtr<ArrayOf<IPropertyValuesHolder*> > tmp = superClass::GetValues();    \
    *values = tmp;                                                              \
    INTERFACE_ADDREF(*values);                                                  \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetCurrentPlayTime(                                            \
    /* [in] */ Int64 playTime)                                                  \
{                                                                               \
    return superClass::SetCurrentPlayTime(playTime);                           \
}                                                                               \
                                                                                \
ECode className::GetCurrentPlayTime(                                            \
    /* [out] */ Int64* playeTime)                                               \
{                                                                               \
    VALIDATE_NOT_NULL(playeTime);                                               \
    *playeTime = superClass::GetCurrentPlayTime();                              \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::GetAnimatedValue(                                              \
    /* [out] */ IInterface** value)                                             \
{                                                                               \
    VALIDATE_NOT_NULL(value);                                                   \
    AutoPtr<IInterface> tmp = superClass::GetAnimatedValue();                   \
    *value = tmp;                                                               \
    INTERFACE_ADDREF(*value);                                                   \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::GetAnimatedValueEx(                                            \
    /* [in] */ const String& propertyName,                                             \
    /* [out] */ IInterface** value)                                             \
{                                                                               \
    VALIDATE_NOT_NULL(value);                                                   \
    AutoPtr<IInterface> tmp = superClass::GetAnimatedValue(propertyName);       \
    *value = tmp;                                                               \
    INTERFACE_ADDREF(*value);                                                   \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetRepeatCount(                                                \
    /* [in] */ Int32 value)                                                     \
{                                                                               \
    return superClass::SetRepeatCount(value);                                  \
}                                                                               \
                                                                                \
ECode className::GetRepeatCount(                                                \
    /* [in] */ Int32* count)                                                    \
{                                                                               \
    VALIDATE_NOT_NULL(count);                                                   \
    *count = superClass::GetRepeatCount();                                      \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetRepeatMode(                                                 \
    /* [in] */ Int32 value)                                                     \
{                                                                               \
    return superClass::SetRepeatMode(value);                                   \
}                                                                               \
                                                                                \
ECode className::GetRepeatMode(                                                 \
    /* [out] */ Int32* mode)                                                    \
{                                                                               \
    VALIDATE_NOT_NULL(mode);                                                    \
    *mode = superClass::GetRepeatMode();                                        \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::AddUpdateListener(                                             \
    /* [in] */ IAnimatorUpdateListener* listener)                               \
{                                                                               \
    return superClass::AddUpdateListener(listener);                            \
}                                                                               \
                                                                                \
ECode className::RemoveAllUpdateListeners()                                     \
{                                                                               \
    return superClass::RemoveAllUpdateListeners();                             \
}                                                                               \
                                                                                \
ECode className::RemoveUpdateListener(                                          \
    /* [in] */ IAnimatorUpdateListener* listener)                               \
{                                                                               \
    return superClass::RemoveUpdateListener(listener);                         \
}                                                                               \
                                                                                \
ECode className::GetInterpolator(                                               \
    /* [out] */ ITimeInterpolator** polator)                                    \
{                                                                               \
    VALIDATE_NOT_NULL(polator);                                                 \
    AutoPtr<ITimeInterpolator> tmp = superClass::GetInterpolator();             \
    *polator = tmp;                                                             \
    INTERFACE_ADDREF(*polator);                                                 \
    return NOERROR;                                                            \
}                                                                               \
                                                                                \
ECode className::SetEvaluator(                                                  \
    /* [in] */ ITypeEvaluator* value)                                           \
{                                                                               \
    return superClass::SetEvaluator(value);                                    \
}                                                                               \
                                                                                \
ECode className::Reverse()                                                      \
{                                                                               \
    return superClass::Reverse();                                              \
}                                                                               \
                                                                                \
ECode className::GetAnimatedFraction(                                           \
    /* [out] */ Float* farction)                                                \
{                                                                               \
    VALIDATE_NOT_NULL(farction);                                                \
    *farction = superClass::GetAnimatedFraction();                              \
    return NOERROR;                                                            \
}


#endif //
