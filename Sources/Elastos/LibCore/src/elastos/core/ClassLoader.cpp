
#include "ClassLoader.h"
#include "CSystem.h"

namespace Elastos {
namespace Core {

//--------------------------------------------
//  ClassLoader::SystemClassLoader
//--------------------------------------------
AutoPtr<IClassLoader> ClassLoader::SystemClassLoader::sLoader;


//--------------------------------------------
//  ClassLoader
//--------------------------------------------
AutoPtr<IClassLoader> ClassLoader::CreateSystemClassLoader()
{
    AutoPtr<CSystem> system;
    CSystem::AcquireSingletonByFriend((CSystem**)&system);
    String classPath;
    system->GetProperty(String("elastos.class.path"), String("."), &classPath);

    // String[] paths = classPath.split(":");
    // URL[] urls = new URL[paths.length];
    // for (int i = 0; i < paths.length; i++) {
    // try {
    // urls[i] = new URL("file://" + paths[i]);
    // }
    // catch (Exception ex) {
    // ex.printStackTrace();
    // }
    // }
    //
    // return new java.net.URLClassLoader(urls, null);

    // TODO Make this a java.net.URLClassLoader once we have those?
    // return new PathClassLoader(classPath, BootClassLoader.getInstance());
    return NULL;
}

AutoPtr<IClassLoader> ClassLoader::GetSystemClassLoader()
{
    if (SystemClassLoader::sLoader == NULL) {
        SystemClassLoader::sLoader = CreateSystemClassLoader();
    }
    return SystemClassLoader::sLoader;
}

}
}
