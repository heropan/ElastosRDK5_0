
#ifndef  __ANIMATORMACRO_H__
#define  __ANIMATORMACRO_H__

#define IANIMATOR_METHOD_DECL()                                     \
    virtual CARAPI Start();                                         \
                                                                    \
    virtual CARAPI Cancel();                                        \
                                                                    \
    virtual CARAPI End();                                           \
                                                                    \
    virtual CARAPI GetStartDelay(                                   \
        /* [out] */ Int64* delay);                                  \
                                                                    \
    virtual CARAPI SetStartDelay(                                   \
        /* [in] */ Int64 startDelay);                               \
                                                                    \
    virtual CARAPI SetDuration(                                     \
        /* [in] */ Int64 duration);                                 \
                                                                    \
    virtual CARAPI GetDuration(                                     \
        /* [out] */ Int64* duration);                               \
                                                                    \
    virtual CARAPI SetInterpolator(                                 \
        /* [in] */ ITimeInterpolator* value);                       \
                                                                    \
    virtual CARAPI IsRunning(                                       \
        /* [out] */ Boolean* running);                              \
                                                                    \
    virtual CARAPI IsStarted(                                       \
        /* [out] */ Boolean* started);                              \
                                                                    \
    virtual CARAPI AddListener(                                     \
        /* [in] */ IAnimatorListener* listener);                    \
                                                                    \
    virtual CARAPI RemoveListener(                                  \
        /* [in] */ IAnimatorListener* listener);                    \
                                                                    \
    virtual CARAPI GetListeners(                                   \
        /* [out, callee] */ ArrayOf<IAnimatorListener*>** listeners); \
                                                                    \
    virtual CARAPI RemoveAllListeners();                           \
                                                                    \
    virtual CARAPI Clone(                                           \
        /* [out] */ IAnimator** object);                           \
                                                                    \
    virtual CARAPI SetupStartValues();                              \
                                                                    \
    virtual CARAPI SetupEndValues();                                \
                                                                    \
    virtual CARAPI SetTarget(                                       \
        /* [in] */ IInterface* target);


#define IANIMATOR_METHOD_IMPL(className, superClass)               \
    ECode className::Start()                                        \
    {                                                               \
        return superClass::Start();                                 \
    }                                                               \
                                                                    \
    ECode className::Cancel()                                       \
    {                                                               \
        return superClass::Cancel();                                \
    }                                                               \
                                                                    \
    ECode className::End()                                          \
    {                                                               \
        return superClass::End();                                   \
    }                                                               \
                                                                    \
    ECode className::GetStartDelay(                                 \
        /* [out] */ Int64* delay)                                   \
    {                                                               \
        VALIDATE_NOT_NULL(delay);                                   \
        *delay = superClass::GetStartDelay();                      \
        return NOERROR;                                             \
    }                                                               \
                                                                    \
    ECode className::SetStartDelay(                                 \
        /* [in] */ Int64 startDelay)                                \
    {                                                               \
        return superClass::SetStartDelay(startDelay);               \
    }                                                               \
                                                                    \
    ECode className::SetDuration(                                   \
        /* [in] */ Int64 duration)                                    \
    {                                                               \
        return superClass::SetDuration(duration);                    \
    }                                                               \
                                                                    \
    ECode className::GetDuration(                                   \
        /* [out] */ Int64* duration)                                \
    {                                                               \
        VALIDATE_NOT_NULL(duration);                                 \
        *duration = superClass::GetDuration();                      \
        return NOERROR;                                             \
    }                                                               \
                                                                    \
    ECode className::SetInterpolator(                               \
        /* [in] */ ITimeInterpolator* value)                        \
    {                                                               \
        return superClass::SetInterpolator(value);                  \
    }                                                               \
                                                                    \
    ECode className::IsRunning(                                     \
        /* [out] */ Boolean* running)                               \
    {                                                               \
        VALIDATE_NOT_NULL(running);                                 \
        *running = superClass::IsRunning();                         \
        return NOERROR;                                             \
    }                                                               \
                                                                    \
    ECode className::IsStarted(                                     \
        /* [out] */ Boolean* started)                               \
    {                                                               \
        VALIDATE_NOT_NULL(started);                                 \
        *started = superClass::IsStarted();                         \
        return NOERROR;                                             \
    }                                                               \
                                                                    \
    ECode className::AddListener(                                   \
        /* [in] */ IAnimatorListener* listener)                     \
    {                                                               \
        return superClass::AddListener(listener);                   \
    }                                                               \
                                                                    \
    ECode className::RemoveListener(                                \
        /* [in] */ IAnimatorListener* listener)                     \
    {                                                               \
        return superClass::RemoveListener(listener);                \
    }                                                               \
                                                                    \
    ECode className::GetListeners(                                  \
        /* [out, callee] */ ArrayOf<IAnimatorListener*>** listeners)\
    {                                                               \
        VALIDATE_NOT_NULL(listeners);                               \
        return superClass::GetListeners(listeners);                 \
    }                                                               \
                                                                    \
    ECode className::RemoveAllListeners()                           \
    {                                                               \
        return superClass::RemoveAllListeners();                    \
    }                                                               \
                                                                    \
    ECode className::Clone(                                         \
        /* [out] */ IAnimator** object)                             \
    {                                                               \
        return superClass::Clone(object);                           \
    }                                                               \
                                                                    \
    ECode className::SetupStartValues()                             \
    {                                                               \
        return superClass::SetupStartValues();                      \
    }                                                               \
                                                                    \
    ECode className::SetupEndValues()                               \
    {                                                               \
        return superClass::SetupEndValues();                        \
    }                                                               \
                                                                    \
    ECode className::SetTarget(                                     \
        /* [in] */ IInterface* target)                              \
    {                                                               \
        return superClass::SetTarget(target);                       \
    }

#endif  //__ANIMATORMACRO_H__
