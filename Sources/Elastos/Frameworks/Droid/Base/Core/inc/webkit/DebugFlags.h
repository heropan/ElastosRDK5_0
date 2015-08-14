
#ifndef __ELASTOS_DROID_WEBKIT_DEBUGFLAGS_H__
#define __ELASTOS_DROID_WEBKIT_DEBUGFLAGS_H__

#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

/**
 * This class is a container for all of the debug flags used in the Java
 * components of webkit.  These flags must be final in order to ensure that
 * the compiler optimizes the code that uses them out of the final executable.
 *
 * The name of each flags maps directly to the name of the class in which that
 * flag is used.
 *
 */
class DebugFlags
{
public:
    static const Boolean BROWSER_FRAME;
    static const Boolean CACHE_MANAGER;
    static const Boolean CALLBACK_PROXY;
    static const Boolean COOKIE_MANAGER;
    static const Boolean COOKIE_SYNC_MANAGER;
    static const Boolean FRAME_LOADER;
    static const Boolean J_WEB_CORE_JAVA_BRIDGE;// HIGHLY VERBOSE
    static const Boolean LOAD_LISTENER;
    static const Boolean NETWORK;
    static const Boolean SSL_ERROR_HANDLER;
    static const Boolean STREAM_LOADER;
    static const Boolean URL_UTIL;
    static const Boolean WEB_BACK_FORWARD_LIST;
    static const Boolean WEB_SETTINGS;
    static const Boolean WEB_SYNC_MANAGER;
    static const Boolean WEB_VIEW;
    static const Boolean WEB_VIEW_CORE;
    static const Boolean MEASURE_PAGE_SWAP_FPS;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_DEBUGFLAGS_H__
