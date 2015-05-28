
#include "CPack200Helper.h"
#include "CSystem.h"

using Elastos::Core::ISystem;
using Elastos::Core::IClassLoader;

namespace Elastos {
namespace Utility {
namespace Jar {

static const String TAG("CPack200Helper");

static const String SYSTEM_PROPERTY_PACKER("java.util.jar.Pack200.Packer");

static const String SYSTEM_PROPERTY_UNPACKER("java.util.jar.Pack200.Unpacker");

#define LOAD_CLASS(var, var_type) \
do {\
    AutoPtr<IClassLoader> classLoader; \
    AutoPtr<IClassInfo> classInfo; \
    AutoPtr<IInterface> object; \
    ECode ec; \
    /*FAIL_GOTO(ec = CActivityThread::sSystemContext->GetClassLoader((IClassLoader**)&classLoader), label)*/ \
    FAIL_GOTO(ec = classLoader->LoadClass(className, (IClassInfo**)&classInfo), label) \
    FAIL_GOTO(ec = classInfo->CreateObject((PInterface*)&object), label) \
label: \
    if (FAILED(ec)) { \
        /*Slogger::E(TAG, "Can't load class %s", className.string()); */\
        *var = NULL; \
        return ec; \
    } \
    *var = (var_type*)var_type::Probe(object.Get()); \
    INTERFACE_ADDREF(*var) \
} while(0);

ECode CPack200Helper::NewPacker(
    /* [out] */ IPacker** packer)
{
    VALIDATE_NOT_NULL(packer)
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    String className;
    system->GetProperty(SYSTEM_PROPERTY_PACKER,
        String("org.apache.harmony.pack200.Pack200PackerAdapter"), &className);
    LOAD_CLASS(packer, IPacker)
    return NOERROR;
}

ECode CPack200Helper::NewUnpacker(
    /* [out] */ IUnpacker** unpacker)
{
    VALIDATE_NOT_NULL(unpacker)
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    String className;
    system->GetProperty(SYSTEM_PROPERTY_UNPACKER,
        String("org.apache.harmony.unpack200.Pack200UnpackerAdapter"), &className);
    LOAD_CLASS(unpacker, IUnpacker)
    return NOERROR;
}

} // namespace Jar
} // namespace Utility
} // namespace Elastos

