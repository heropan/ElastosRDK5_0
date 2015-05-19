
#ifndef __ELASTOS_CORE_DEF_H__
#define __ELASTOS_CORE_DEF_H__

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

#include <errno.h>

#define ANDROID_SMP 1

/*
 * TEMP_FAILURE_RETRY is defined by some, but not all, versions of
 * <unistd.h>. (Alas, it is not as standard as we'd hoped!) So, if it's
 * not already defined, then define it here.
 */
#ifndef TEMP_FAILURE_RETRY
/* Used to retry syscalls that can return EINTR. */
#define TEMP_FAILURE_RETRY(exp) ({         \
    typeof (exp) _rc;                      \
    do {                                   \
        _rc = (exp);                       \
    } while (_rc == -1 && errno == EINTR); \
    _rc; })
#endif

#ifndef ASSERT_TRUE
#define ASSERT_TRUE(expr) \
    do { \
        Boolean assert_result_ = (Boolean)(expr); \
        assert(assert_result_); \
    } while(0);
#endif

#ifndef ASSERT_SUCCEEDED
#define ASSERT_SUCCEEDED(expr) \
   do { \
       ECode assert_ec_ = expr; \
       assert(SUCCEEDED(assert_ec_)); \
   } while(0);
#endif

#ifndef VALIDATE_NOT_NULL
#define VALIDATE_NOT_NULL(x) if (!x) { return E_ILLEGAL_ARGUMENT_EXCEPTION; }
#endif

#ifndef FAIL_RETURN
#define FAIL_RETURN(expr) \
    do { \
        ECode fail_ec_ = expr; \
        if (FAILED(fail_ec_)) return fail_ec_; \
    } while(0);
#endif

#ifndef FAIL_RETURN_NULL
#define FAIL_RETURN_NULL(expr) \
    do { \
        ECode fail_ec_ = expr; \
        if (FAILED(fail_ec_)) return NULL; \
    } while(0);
#endif

#ifndef FAIL_GOTO
#define FAIL_GOTO(expr, label) \
    do { \
        ECode fail_ec_ = expr; \
        if (FAILED(fail_ec_)) goto label; \
    } while(0);
#endif

#ifndef THIS_PROBE
#define THIS_PROBE(Interface) ((Interface *)this->Probe(EIID_##Interface))
#endif

// Car interface decls and impls
//
#ifndef CAR_INTERFACE_DECL
#define CAR_INTERFACE_DECL()           \
    CARAPI_(UInt32) AddRef();          \
                                       \
    CARAPI_(UInt32) Release();         \
                                       \
    CARAPI_(PInterface) Probe(         \
        /* [in] */ REIID riid);        \
                                       \
    CARAPI GetInterfaceID(             \
        /* [in] */ IInterface* object, \
        /* [out] */ InterfaceID* iid);
#endif

#ifndef CAR_INTERFACE_IMPL
#define CAR_INTERFACE_IMPL(ClassName, SupperClassName, InterfaceName)       \
                                                           \
    UInt32 ClassName::AddRef()                             \
    {                                                      \
        return ElRefBase::AddRef();                        \
    }                                                      \
                                                           \
    UInt32 ClassName::Release()                            \
    {                                                      \
        return ElRefBase::Release();                       \
    }                                                      \
                                                           \
    PInterface ClassName::Probe(                           \
        /* [in] */ REIID riid)                             \
    {                                                      \
        if (riid == EIID_IInterface) {                     \
            return (IInterface*)(InterfaceName*)this;      \
        }                                                  \
        else if (riid == EIID_##InterfaceName) {           \
            return (InterfaceName*)this;                   \
        }                                                  \
        return SupperClassName::Probe(riid);               \
    }                                                      \
                                                           \
    ECode ClassName::GetInterfaceID(                       \
        /* [in] */ IInterface* object,                     \
        /* [out] */ InterfaceID* iid)                      \
    {                                                      \
        VALIDATE_NOT_NULL(iid);                            \
        if (object == (IInterface*)(InterfaceName*)this) { \
            *iid = EIID_##InterfaceName;                   \
        }                                                  \
        else {                                             \
            return E_ILLEGAL_ARGUMENT_EXCEPTION;           \
        }                                                  \
        return NOERROR;                                    \
    }
#endif

#ifndef CAR_INTERFACE_IMPL_LIGHT
#define CAR_INTERFACE_IMPL_LIGHT(ClassName, SupperClassName, InterfaceName) \
    UInt32 ClassName::AddRef()                             \
    {                                                      \
        return ElLightRefBase::AddRef();                   \
    }                                                      \
                                                           \
    UInt32 ClassName::Release()                            \
    {                                                      \
        return ElLightRefBase::Release();                  \
    }                                                      \
                                                           \
    PInterface ClassName::Probe(                           \
        /* [in] */ REIID riid)                             \
    {                                                      \
        if (riid == EIID_IInterface) {                     \
            return (IInterface*)(InterfaceName*)this;      \
        }                                                  \
        else if (riid == EIID_##InterfaceName) {           \
            return (InterfaceName*)this;                   \
        }                                                  \
        return SupperClassName::Probe(riid);               \
    }                                                      \
                                                           \
    ECode ClassName::GetInterfaceID(                       \
        /* [in] */ IInterface* object,                     \
        /* [out] */ InterfaceID* iid)                      \
    {                                                      \
        VALIDATE_NOT_NULL(iid);                            \
        if (object == (IInterface*)(InterfaceName*)this) { \
            *iid = EIID_##InterfaceName;                   \
        }                                                  \
        else {                                             \
            return E_ILLEGAL_ARGUMENT_EXCEPTION;           \
        }                                                  \
        return NOERROR;                                    \
    }
#endif

#ifndef CAR_INTERFACE_IMPL_2
#define CAR_INTERFACE_IMPL_2(ClassName, SupperClassName, Interface1, Interface2) \
    UInt32 ClassName::AddRef()                                  \
    {                                                           \
        return ElRefBase::AddRef();                             \
    }                                                           \
                                                                \
    UInt32 ClassName::Release()                                 \
    {                                                           \
        return ElRefBase::Release();                            \
    }                                                           \
                                                                \
    PInterface ClassName::Probe(                                \
        /* [in] */ REIID riid)                                  \
    {                                                           \
        if (riid == EIID_IInterface) {                          \
            return (IInterface*)(Interface1*)this;              \
        }                                                       \
        else if (riid == EIID_##Interface1) {                   \
            return (Interface1*)this;                           \
        }                                                       \
        else if (riid == EIID_##Interface2) {                   \
            return (Interface2*)this;                           \
        }                                                       \
        return SupperClassName::Probe(riid);                    \
    }                                                           \
                                                                \
    ECode ClassName::GetInterfaceID(                            \
        /* [in] */ IInterface* object,                          \
        /* [out] */ InterfaceID* iid)                           \
    {                                                           \
        VALIDATE_NOT_NULL(iid);                                 \
        if (object == (IInterface*)(Interface1*)this) {         \
            *iid = EIID_##Interface1;                           \
        }                                                       \
        else if (object == (IInterface*)(Interface2*)this) {    \
            *iid = EIID_##Interface2;                           \
        }                                                       \
        else {                                                  \
            return E_ILLEGAL_ARGUMENT_EXCEPTION;                \
        }                                                       \
        return NOERROR;                                         \
    }
#endif

#ifndef CAR_INTERFACE_IMPL_LIGHT_2
#define CAR_INTERFACE_IMPL_LIGHT_2(ClassName, SupperClassName, Interface1, Interface2) \
    UInt32 ClassName::AddRef()                                        \
    {                                                                 \
        return ElLightRefBase::AddRef();                              \
    }                                                                 \
                                                                      \
    UInt32 ClassName::Release()                                       \
    {                                                                 \
        return ElLightRefBase::Release();                             \
    }                                                                 \
                                                                      \
    PInterface ClassName::Probe(                                      \
        /* [in] */ REIID riid)                                        \
    {                                                                 \
        if (riid == EIID_IInterface) {                                \
            return (IInterface*)(Interface1*)this;                    \
        }                                                             \
        else if (riid == EIID_##Interface1) {                         \
            return (Interface1*)this;                                 \
        }                                                             \
        else if (riid == EIID_##Interface2) {                         \
            return (Interface2*)this;                                 \
        }                                                             \
        return SupperClassName::Probe(riid);                          \
    }                                                                 \
                                                                      \
    ECode ClassName::GetInterfaceID(                                  \
        /* [in] */ IInterface* object,                                \
        /* [out] */ InterfaceID* iid)                                 \
    {                                                                 \
        VALIDATE_NOT_NULL(iid);                                       \
        if (object == (IInterface*)(Interface1*)this) {               \
            *iid = EIID_##Interface1;                                 \
        }                                                             \
        else if (object == (IInterface*)(Interface2*)this) {          \
            *iid = EIID_##Interface2;                                 \
        }                                                             \
        else {                                                        \
            return E_ILLEGAL_ARGUMENT_EXCEPTION;                      \
        }                                                             \
        return NOERROR;                                               \
    }
#endif

#ifndef CAR_INTERFACE_IMPL_3
#define CAR_INTERFACE_IMPL_3(ClassName, SupperClassName, Interface1, Interface2, Interface3) \
    UInt32 ClassName::AddRef()                                              \
    {                                                                       \
        return ElRefBase::AddRef();                                         \
    }                                                                       \
                                                                            \
    UInt32 ClassName::Release()                                             \
    {                                                                       \
        return ElRefBase::Release();                                        \
    }                                                                       \
                                                                            \
    PInterface ClassName::Probe(                                            \
        /* [in] */ REIID riid)                                              \
    {                                                                       \
        if (riid == EIID_IInterface) {                                      \
            return (IInterface*)(Interface1*)this;                          \
        }                                                                   \
        else if (riid == EIID_##Interface1) {                               \
            return (Interface1*)this;                                       \
        }                                                                   \
        else if (riid == EIID_##Interface2) {                               \
            return (Interface2*)this;                                       \
        }                                                                   \
        else if (riid == EIID_##Interface3) {                               \
            return (Interface3*)this;                                       \
        }                                                                   \
        return SupperClassName::Probe(riid);                                \
    }                                                                       \
                                                                            \
    ECode ClassName::GetInterfaceID(                                        \
        /* [in] */ IInterface* object,                                      \
        /* [out] */ InterfaceID* iid)                                       \
    {                                                                       \
        VALIDATE_NOT_NULL(iid);                                             \
        if (object == (IInterface*)(Interface1*)this) {                     \
            *iid = EIID_##Interface1;                                       \
        }                                                                   \
        else if (object == (IInterface*)(Interface2*)this) {                \
            *iid = EIID_##Interface2;                                       \
        }                                                                   \
        else if (object == (IInterface*)(Interface3*)this) {                \
            *iid = EIID_##Interface3;                                       \
        }                                                                   \
        else {                                                              \
            return E_ILLEGAL_ARGUMENT_EXCEPTION;                            \
        }                                                                   \
        return NOERROR;                                                     \
    }
#endif

#ifndef CAR_INTERFACE_IMPL_LIGHT_3
#define CAR_INTERFACE_IMPL_LIGHT_3(ClassName, SupperClassName, Interface1, Interface2, Interface3) \
    UInt32 ClassName::AddRef()                                                    \
    {                                                                             \
        return ElLightRefBase::AddRef();                                          \
    }                                                                             \
                                                                                  \
    UInt32 ClassName::Release()                                                   \
    {                                                                             \
        return ElLightRefBase::Release();                                         \
    }                                                                             \
                                                                                  \
    PInterface ClassName::Probe(                                                  \
        /* [in] */ REIID riid)                                                    \
    {                                                                             \
        if (riid == EIID_IInterface) {                                            \
            return (IInterface*)(Interface1*)this;                                \
        }                                                                         \
        else if (riid == EIID_##Interface1) {                                     \
            return (Interface1*)this;                                             \
        }                                                                         \
        else if (riid == EIID_##Interface2) {                                     \
            return (Interface2*)this;                                             \
        }                                                                         \
        else if (riid == EIID_##Interface3) {                                     \
            return (Interface3*)this;                                             \
        }                                                                         \
        return SupperClassName::Probe(riid);                                      \
    }                                                                             \
                                                                                  \
    ECode ClassName::GetInterfaceID(                                              \
        /* [in] */ IInterface* object,                                            \
        /* [out] */ InterfaceID* iid)                                             \
    {                                                                             \
        VALIDATE_NOT_NULL(iid);                                                   \
        if (object == (IInterface*)(Interface1*)this) {                           \
            *iid = EIID_##Interface1;                                             \
        }                                                                         \
        else if (object == (IInterface*)(Interface2*)this) {                      \
            *iid = EIID_##Interface2;                                             \
        }                                                                         \
        else if (object == (IInterface*)(Interface3*)this) {                      \
            *iid = EIID_##Interface3;                                             \
        }                                                                         \
        else {                                                                    \
            return E_ILLEGAL_ARGUMENT_EXCEPTION;                                  \
        }                                                                         \
        return NOERROR;                                                           \
    }
#endif

#ifndef CAR_INTERFACE_IMPL_4
#define CAR_INTERFACE_IMPL_4(ClassName, SupperClassName, Interface1, Interface2, Interface3, Interface4) \
    UInt32 ClassName::AddRef()                                                          \
    {                                                                                   \
        return ElRefBase::AddRef();                                                     \
    }                                                                                   \
                                                                                        \
    UInt32 ClassName::Release()                                                         \
    {                                                                                   \
        return ElRefBase::Release();                                                    \
    }                                                                                   \
                                                                                        \
    PInterface ClassName::Probe(                                                        \
        /* [in] */ REIID riid)                                                          \
    {                                                                                   \
        if (riid == EIID_IInterface) {                                                  \
            return (IInterface*)(Interface1*)this;                                      \
        }                                                                               \
        else if (riid == EIID_##Interface1) {                                           \
            return (Interface1*)this;                                                   \
        }                                                                               \
        else if (riid == EIID_##Interface2) {                                           \
            return (Interface2*)this;                                                   \
        }                                                                               \
        else if (riid == EIID_##Interface3) {                                           \
            return (Interface3*)this;                                                   \
        }                                                                               \
        else if (riid == EIID_##Interface4) {                                           \
            return (Interface4*)this;                                                   \
        }                                                                               \
        return SupperClassName::Probe(riid);                                            \
    }                                                                                   \
                                                                                        \
    ECode ClassName::GetInterfaceID(                                                    \
        /* [in] */ IInterface* object,                                                  \
        /* [out] */ InterfaceID* iid)                                                   \
    {                                                                                   \
        VALIDATE_NOT_NULL(iid);                                                         \
        if (object == (IInterface*)(Interface1*)this) {                                 \
            *iid = EIID_##Interface1;                                                   \
        }                                                                               \
        else if (object == (IInterface*)(Interface2*)this) {                            \
            *iid = EIID_##Interface2;                                                   \
        }                                                                               \
        else if (object == (IInterface*)(Interface3*)this) {                            \
            *iid = EIID_##Interface3;                                                   \
        }                                                                               \
        else if (object == (IInterface*)(Interface4*)this) {                            \
            *iid = EIID_##Interface4;                                                   \
        }                                                                               \
        else {                                                                          \
            return E_ILLEGAL_ARGUMENT_EXCEPTION;                                        \
        }                                                                               \
        return NOERROR;                                                                 \
    }
#endif

#ifndef CAR_INTERFACE_IMPL_LIGHT_4
#define CAR_INTERFACE_IMPL_LIGHT_4(ClassName, SupperClassName, Interface1, Interface2, Interface3, Interface4) \
    UInt32 ClassName::AddRef()                                                                \
    {                                                                                         \
        return ElLightRefBase::AddRef();                                                      \
    }                                                                                         \
                                                                                              \
    UInt32 ClassName::Release()                                                               \
    {                                                                                         \
        return ElLightRefBase::Release();                                                     \
    }                                                                                         \
                                                                                              \
    PInterface ClassName::Probe(                                                              \
        /* [in] */ REIID riid)                                                                \
    {                                                                                         \
        if (riid == EIID_IInterface) {                                                        \
            return (IInterface*)(Interface1*)this;                                            \
        }                                                                                     \
        else if (riid == EIID_##Interface1) {                                                 \
            return (Interface1*)this;                                                         \
        }                                                                                     \
        else if (riid == EIID_##Interface2) {                                                 \
            return (Interface2*)this;                                                         \
        }                                                                                     \
        else if (riid == EIID_##Interface3) {                                                 \
            return (Interface3*)this;                                                         \
        }                                                                                     \
        else if (riid == EIID_##Interface4) {                                                 \
            return (Interface4*)this;                                                         \
        }                                                                                     \
        return SupperClassName::Probe(riid);                                                  \
    }                                                                                         \
                                                                                              \
    ECode ClassName::GetInterfaceID(                                                          \
        /* [in] */ IInterface* object,                                                        \
        /* [out] */ InterfaceID* iid)                                                         \
    {                                                                                         \
        VALIDATE_NOT_NULL(iid);                                                               \
        if (object == (IInterface*)(Interface1*)this) {                                       \
            *iid = EIID_##Interface1;                                                         \
        }                                                                                     \
        else if (object == (IInterface*)(Interface2*)this) {                                  \
            *iid = EIID_##Interface2;                                                         \
        }                                                                                     \
        else if (object == (IInterface*)(Interface3*)this) {                                  \
            *iid = EIID_##Interface3;                                                         \
        }                                                                                     \
        else if (object == (IInterface*)(Interface4*)this) {                                  \
            *iid = EIID_##Interface4;                                                         \
        }                                                                                     \
        else {                                                                                \
            return E_ILLEGAL_ARGUMENT_EXCEPTION;                                              \
        }                                                                                     \
        return NOERROR;                                                                       \
    }
#endif

#endif //__ELASTOS_CORE_DEF_H__
