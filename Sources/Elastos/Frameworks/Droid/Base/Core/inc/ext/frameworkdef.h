#ifndef __DROIDFRAMEWORKDEF_H__
#define __DROIDFRAMEWORKDEF_H__

#ifndef __USE_MALLOC
#define __USE_MALLOC
#endif

// #define MAX_PATH    256

// #define XML_NAMESPACE   "http://schemas.elastos.com/capsule/res/elastos"

#ifndef VALIDATE_NOT_NULL
#define VALIDATE_NOT_NULL(x) if (!(x)) { return E_ILLEGAL_ARGUMENT_EXCEPTION; }
#endif

#ifndef VALIDATE_STRING_NOT_NULL
#define VALIDATE_STRING_NOT_NULL(x) if (x.IsNull()) { return E_ILLEGAL_ARGUMENT_EXCEPTION; }
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

#ifndef CAR_INTERFACE_DECL
#define CAR_INTERFACE_DECL() \
    CARAPI_(PInterface) Probe( \
        /* [in] */ REIID riid); \
    CARAPI_(UInt32) AddRef(); \
    CARAPI_(UInt32) Release(); \
    CARAPI GetInterfaceID( \
        /* [in] */ IInterface* object, \
        /* [out] */ InterfaceID* iid);
#endif

#ifndef CAR_INTERFACE_IMPL
#define CAR_INTERFACE_IMPL(ClassName, InterfaceName) \
    PInterface ClassName::Probe( \
        /* [in] */ REIID riid) \
    { \
        if ( riid == EIID_IInterface) { \
            return (IInterface*)(InterfaceName *)this; \
        } \
        else if ( riid == EIID_##InterfaceName ) { \
            return (InterfaceName *)this; \
        } \
        return NULL; \
    } \
    UInt32 ClassName::AddRef() \
    { \
        return ElRefBase::AddRef(); \
    } \
    UInt32 ClassName::Release() \
    { \
        return ElRefBase::Release(); \
    } \
    ECode ClassName::GetInterfaceID( \
        /* [in] */ IInterface* object, \
        /* [out] */ InterfaceID* iid) \
    { \
        VALIDATE_NOT_NULL(iid); \
        if (object == (IInterface*)(InterfaceName *)this) { \
            *iid = EIID_##InterfaceName ; \
        } \
        else { \
            return E_ILLEGAL_ARGUMENT_EXCEPTION; \
        } \
        return NOERROR; \
    }
#endif

#ifndef CAR_INTERFACE_IMPL_LIGHT
#define CAR_INTERFACE_IMPL_LIGHT(ClassName, InterfaceName) \
    PInterface ClassName::Probe( \
        /* [in] */ REIID riid) \
    { \
        if ( riid == EIID_IInterface) { \
            return (IInterface*)(InterfaceName *)this; \
        } \
        else if ( riid == EIID_##InterfaceName ) { \
            return (InterfaceName *)this; \
        } \
        return NULL; \
    } \
    UInt32 ClassName::AddRef() \
    { \
        return ElLightRefBase::AddRef(); \
    } \
    UInt32 ClassName::Release() \
    { \
        return ElLightRefBase::Release(); \
    } \
    ECode ClassName::GetInterfaceID( \
        /* [in] */ IInterface* object, \
        /* [out] */ InterfaceID* iid) \
    { \
        VALIDATE_NOT_NULL(iid); \
        if (object == (IInterface*)(InterfaceName *)this) { \
            *iid = EIID_##InterfaceName ; \
        } \
        else { \
            return E_ILLEGAL_ARGUMENT_EXCEPTION; \
        } \
        return NOERROR; \
    }
#endif

#ifndef CAR_INTERFACE_IMPL_2
#define CAR_INTERFACE_IMPL_2(ClassName, Interface1, Interface2) \
    PInterface ClassName::Probe( \
        /* [in] */ REIID riid) \
    { \
        if ( riid == EIID_IInterface) { \
            return (IInterface*)(Interface1 *)this; \
        } \
        else if ( riid == EIID_##Interface1 ) { \
            return (Interface1 *)this; \
        } \
        else if ( riid == EIID_##Interface2 ) { \
            return (Interface2 *)this; \
        } \
        return NULL; \
    } \
    UInt32 ClassName::AddRef() \
    { \
        return ElRefBase::AddRef(); \
    } \
    UInt32 ClassName::Release() \
    { \
        return ElRefBase::Release(); \
    } \
    ECode ClassName::GetInterfaceID( \
        /* [in] */ IInterface* object, \
        /* [out] */ InterfaceID* iid) \
    { \
        VALIDATE_NOT_NULL(iid); \
        if (object == (IInterface*)(Interface1 *)this) { \
            *iid = EIID_##Interface1; \
        } \
        if (object == (IInterface*)(Interface2 *)this) { \
            *iid = EIID_##Interface2; \
        } \
        else { \
            return E_ILLEGAL_ARGUMENT_EXCEPTION; \
        } \
        return NOERROR; \
    }
#endif

#ifndef CAR_INTERFACE_IMPL_LIGHT_2
#define CAR_INTERFACE_IMPL_LIGHT_2(ClassName, Interface1, Interface2) \
    PInterface ClassName::Probe( \
        /* [in] */ REIID riid) \
    { \
        if ( riid == EIID_IInterface) { \
            return (IInterface*)(Interface1 *)this; \
        } \
        else if ( riid == EIID_##Interface1 ) { \
            return (Interface1 *)this; \
        } \
        else if ( riid == EIID_##Interface2 ) { \
            return (Interface2 *)this; \
        } \
        return NULL; \
    } \
    UInt32 ClassName::AddRef() \
    { \
        return ElLightRefBase::AddRef(); \
    } \
    UInt32 ClassName::Release() \
    { \
        return ElLightRefBase::Release(); \
    } \
    ECode ClassName::GetInterfaceID( \
        /* [in] */ IInterface* object, \
        /* [out] */ InterfaceID* iid) \
    { \
        VALIDATE_NOT_NULL(iid); \
        if (object == (IInterface*)(Interface1 *)this) { \
            *iid = EIID_##Interface1; \
        } \
        else if (object == (IInterface*)(Interface2 *)this) { \
            *iid = EIID_##Interface2; \
        } \
        else { \
            return E_ILLEGAL_ARGUMENT_EXCEPTION; \
        } \
        return NOERROR; \
    }
#endif

#ifndef CAR_INTERFACE_IMPL_3
#define CAR_INTERFACE_IMPL_3(ClassName, Interface1, Interface2, Interface3) \
    PInterface ClassName::Probe( \
        /* [in] */ REIID riid) \
    { \
        if (riid == EIID_IInterface) { \
            return (IInterface*)(Interface1 *)this; \
        } \
        else if (riid == EIID_##Interface1) { \
            return (Interface1 *)this; \
        } \
        else if (riid == EIID_##Interface2) { \
            return (Interface2 *)this; \
        } \
        else if (riid == EIID_##Interface3) { \
            return (Interface3 *)this; \
        } \
        return NULL; \
    } \
    UInt32 ClassName::AddRef() \
    { \
        return ElRefBase::AddRef(); \
    } \
    UInt32 ClassName::Release() \
    { \
        return ElRefBase::Release(); \
    } \
    ECode ClassName::GetInterfaceID( \
        /* [in] */ IInterface* object, \
        /* [out] */ InterfaceID* iid) \
    { \
        VALIDATE_NOT_NULL(iid); \
        if (object == (IInterface*)(Interface1 *)this) { \
            *iid = EIID_##Interface1; \
        } \
        else if (object == (IInterface*)(Interface2 *)this) { \
            *iid = EIID_##Interface2; \
        } \
        else if (object == (IInterface*)(Interface3 *)this) { \
            *iid = EIID_##Interface3; \
        } \
        else { \
            return E_ILLEGAL_ARGUMENT_EXCEPTION; \
        } \
        return NOERROR; \
    }
#endif

#ifndef CAR_INTERFACE_IMPL_LIGHT_3
#define CAR_INTERFACE_IMPL_LIGHT_3(ClassName, Interface1, Interface2, Interface3) \
    PInterface ClassName::Probe( \
        /* [in] */ REIID riid) \
    { \
        if (riid == EIID_IInterface) { \
            return (IInterface*)(Interface1 *)this; \
        } \
        else if (riid == EIID_##Interface1) { \
            return (Interface1 *)this; \
        } \
        else if (riid == EIID_##Interface2) { \
            return (Interface2 *)this; \
        } \
        else if (riid == EIID_##Interface3) { \
            return (Interface3 *)this; \
        } \
        return NULL; \
    } \
    UInt32 ClassName::AddRef() \
    { \
        return ElLightRefBase::AddRef(); \
    } \
    UInt32 ClassName::Release() \
    { \
        return ElLightRefBase::Release(); \
    } \
    ECode ClassName::GetInterfaceID( \
        /* [in] */ IInterface* object, \
        /* [out] */ InterfaceID* iid) \
    { \
        VALIDATE_NOT_NULL(iid); \
        if (object == (IInterface*)(Interface1 *)this) { \
            *iid = EIID_##Interface1; \
        } \
        else if (object == (IInterface*)(Interface2 *)this) { \
            *iid = EIID_##Interface2; \
        } \
        else if (object == (IInterface*)(Interface3 *)this) { \
            *iid = EIID_##Interface3; \
        } \
        else { \
            return E_ILLEGAL_ARGUMENT_EXCEPTION; \
        } \
        return NOERROR; \
    }
#endif

#ifndef CAR_INTERFACE_IMPL_4
#define CAR_INTERFACE_IMPL_4(ClassName, Interface1, Interface2, Interface3, Interface4) \
    PInterface ClassName::Probe( \
        /* [in] */ REIID riid) \
    { \
        if (riid == EIID_IInterface) { \
            return (IInterface*)(Interface1 *)this; \
        } \
        else if (riid == EIID_##Interface1) { \
            return (Interface1 *)this; \
        } \
        else if (riid == EIID_##Interface2) { \
            return (Interface2 *)this; \
        } \
        else if (riid == EIID_##Interface3) { \
            return (Interface3 *)this; \
        } \
        else if (riid == EIID_##Interface4) { \
            return (Interface4 *)this; \
        } \
        return NULL; \
    } \
    UInt32 ClassName::AddRef() \
    { \
        return ElRefBase::AddRef(); \
    } \
    UInt32 ClassName::Release() \
    { \
        return ElRefBase::Release(); \
    } \
    ECode ClassName::GetInterfaceID( \
        /* [in] */ IInterface* object, \
        /* [out] */ InterfaceID* iid) \
    { \
        VALIDATE_NOT_NULL(iid); \
        if (object == (IInterface*)(Interface1 *)this) { \
            *iid = EIID_##Interface1; \
        } \
        else if (object == (IInterface*)(Interface2 *)this) { \
            *iid = EIID_##Interface2; \
        } \
        else if (object == (IInterface*)(Interface3 *)this) { \
            *iid = EIID_##Interface3; \
        } \
        else if (object == (IInterface*)(Interface4 *)this) { \
            *iid = EIID_##Interface4; \
        } \
        else { \
            return E_ILLEGAL_ARGUMENT_EXCEPTION; \
        } \
        return NOERROR; \
    }
#endif

#ifndef CAR_INTERFACE_IMPL_LIGHT_4
#define CAR_INTERFACE_IMPL_LIGHT_4(ClassName, Interface1, Interface2, Interface3, Interface4) \
    PInterface ClassName::Probe( \
        /* [in] */ REIID riid) \
    { \
        if (riid == EIID_IInterface) { \
            return (IInterface*)(Interface1 *)this; \
        } \
        else if (riid == EIID_##Interface1) { \
            return (Interface1 *)this; \
        } \
        else if (riid == EIID_##Interface2) { \
            return (Interface2 *)this; \
        } \
        else if (riid == EIID_##Interface3) { \
            return (Interface3 *)this; \
        } \
        else if (riid == EIID_##Interface4) { \
            return (Interface4 *)this; \
        } \
        return NULL; \
    } \
    UInt32 ClassName::AddRef() \
    { \
        return ElLightRefBase::AddRef(); \
    } \
    UInt32 ClassName::Release() \
    { \
        return ElLightRefBase::Release(); \
    } \
    ECode ClassName::GetInterfaceID( \
        /* [in] */ IInterface* object, \
        /* [out] */ InterfaceID* iid) \
    { \
        VALIDATE_NOT_NULL(iid); \
        if (object == (IInterface*)(Interface1 *)this) { \
            *iid = EIID_##Interface1; \
        } \
        else if (object == (IInterface*)(Interface2 *)this) { \
            *iid = EIID_##Interface2; \
        } \
        else if (object == (IInterface*)(Interface3 *)this) { \
            *iid = EIID_##Interface3; \
        } \
        else if (object == (IInterface*)(Interface4 *)this) { \
            *iid = EIID_##Interface4; \
        } \
        else { \
            return E_ILLEGAL_ARGUMENT_EXCEPTION; \
        } \
        return NOERROR; \
    }
#endif

#ifndef CAR_INTERFACE_IMPL_WITH_CPP_CAST
#define CAR_INTERFACE_IMPL_WITH_CPP_CAST(FullClassName, Interface, CppClassName) \
    PInterface FullClassName::Probe( \
        /* [in] */ REIID riid) \
    { \
        if (riid == EIID_IInterface) { \
            return (IInterface*)(Interface *)this; \
        } \
        if (riid == EIID_##Interface) { \
            return (IInterface*)(Interface *)this; \
        } \
        if (riid == EIID_##CppClassName ) { \
            return reinterpret_cast<PInterface>(this); \
        } \
        return NULL; \
    } \
    UInt32 FullClassName::AddRef() \
    { \
        return ElRefBase::AddRef(); \
    } \
    UInt32 FullClassName::Release() \
    { \
        return ElRefBase::Release(); \
    } \
    ECode FullClassName::GetInterfaceID( \
        /* [in] */ IInterface* object, \
        /* [out] */ InterfaceID* iid) \
    { \
        VALIDATE_NOT_NULL(iid); \
        if (object == (IInterface*)(Interface *)this) { \
            *iid = EIID_##Interface; \
        } \
        else { \
            return E_ILLEGAL_ARGUMENT_EXCEPTION; \
        } \
        return NOERROR; \
    }
#endif

#ifndef CAR_INTERFACE_IMPL_WITH_CPP_CAST_2
#define CAR_INTERFACE_IMPL_WITH_CPP_CAST_2(FullClassName, Interface1, Interface2, CppClassName) \
    PInterface FullClassName::Probe( \
        /* [in] */ REIID riid) \
    { \
        if (riid == EIID_IInterface) { \
            return (IInterface*)(Interface1 *)this; \
        } \
        if (riid == EIID_##Interface1) { \
            return (IInterface*)(Interface1 *)this; \
        } \
        if (riid == EIID_##Interface2) { \
            return (IInterface*)(Interface2 *)this; \
        } \
        if (riid == EIID_##CppClassName ) { \
            return reinterpret_cast<PInterface>(this); \
        } \
        return NULL; \
    } \
    UInt32 FullClassName::AddRef() \
    { \
        return ElRefBase::AddRef(); \
    } \
    UInt32 FullClassName::Release() \
    { \
        return ElRefBase::Release(); \
    } \
    ECode FullClassName::GetInterfaceID( \
        /* [in] */ IInterface* object, \
        /* [out] */ InterfaceID* iid) \
    { \
        VALIDATE_NOT_NULL(iid); \
        if (object == (IInterface*)(Interface1 *)this) { \
            *iid = EIID_##Interface1; \
        } \
        else if (object == (IInterface*)(Interface2 *)this) { \
            *iid = EIID_##Interface2; \
        } \
        else { \
            return E_ILLEGAL_ARGUMENT_EXCEPTION; \
        } \
        return NOERROR; \
    }
#endif

#ifndef DUMP_CLSID
#define DUMP_CLSID(intf) \
    do { \
        ClassID clsid; \
        IObject* obj = IObject::Probe(intf); \
        assert(obj != NULL); \
        obj->GetClassID(&clsid); \
        printf("======== DUMP_CLSID ========\n"); \
        printf("{%p, %p, %p, {%p, %p, %p, %p, %p, %p, %p, %p} }\n", \
                clsid.clsid.Data1, clsid.clsid.Data2, clsid.clsid.Data3, \
                clsid.clsid.Data4[0], clsid.clsid.Data4[1], \
                clsid.clsid.Data4[2], clsid.clsid.Data4[3], \
                clsid.clsid.Data4[4], clsid.clsid.Data4[5], \
                clsid.clsid.Data4[6], clsid.clsid.Data4[7]); \
        printf("============================\n"); \
    } while(0);
#endif

#ifndef DUMP_ITFID
#define DUMP_ITFID(intf) \
    do { \
        InterfaceID iid; \
        intf->GetInterfaceID(intf, &iid); \
        printf("======== DUMP_ITFID ========\n"); \
        printf("{%p, %p, %p, {%p, %p, %p, %p, %p, %p, %p, %p} }\n", \
                iid.Data1, iid.Data2, iid.Data3, \
                iid.Data4[0], iid.Data4[1], \
                iid.Data4[2], iid.Data4[3], \
                iid.Data4[4], iid.Data4[5], \
                iid.Data4[6], iid.Data4[7]); \
        printf("============================\n"); \
    } while(0);
#endif


#endif //__DROIDFRAMEWORKDEF_H__
