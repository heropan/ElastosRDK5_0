
#ifndef __ELASTOS_CORE_CLASSLOADER_H__
#define __ELASTOS_CORE_CLASSLOADER_H__

#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif

namespace Elastos {
namespace Core {

class ClassLoader
{
private:
    /**
     * The 'System' ClassLoader - the one that is responsible for loading
     * classes from the classpath. It is not equal to the bootstrap class loader -
     * that one handles the built-in classes.
     *
     * Because of a potential class initialization race between ClassLoader and
     * java.lang.System, reproducible when using JDWP with "suspend=y", we defer
     * creation of the system class loader until first use. We use a static
     * inner class to get synchronization at init time without having to sync on
     * every access.
     *
     * @see #getSystemClassLoader()
     */
    class SystemClassLoader
    {
    public:
        static AutoPtr<IClassLoader> sLoader;
    };

public:
    /**
     * Returns the system class loader. This is the parent for new
     * {@code ClassLoader} instances and is typically the class loader used to
     * start the application.
     */
    static CARAPI_(AutoPtr<IClassLoader>) GetSystemClassLoader();

    static CARAPI_(AutoPtr<IClassLoader>) GetClassLoader(
        /* [in] */ IClassInfo* clsInfo);

private:
    /**
     * Create the system class loader. Note this is NOT the bootstrap class
     * loader (which is managed by the VM). We use a null value for the parent
     * to indicate that the bootstrap loader is our parent.
     */
    static CARAPI_(AutoPtr<IClassLoader>) CreateSystemClassLoader();
};

} // namespace Core
} // namespace Elastos

#endif //__ELASTOS_CORE_CLASSLOADER_H__
