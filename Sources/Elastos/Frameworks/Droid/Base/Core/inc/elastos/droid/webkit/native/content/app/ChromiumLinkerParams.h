
#ifndef __ELASTOS_DROID_WEBKIT_CONTENT_APP_CHROMIUMLINKERPARAMS_H__
#define __ELASTOS_DROID_WEBKIT_CONTENT_APP_CHROMIUMLINKERPARAMS_H__

#include "ext/frameworkext.h"

using Elastos::Droid::Content::IIntent;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace App {

/**
 * A class to hold information passed from the browser process to each
 * service one when using the chromium linker. For more information, read the
 * technical notes in Linker.java.
 */
class ChromiumLinkerParams
{
public:
    ChromiumLinkerParams(
        /* [in] */ Int64 baseLoadAddress,
        /* [in] */ Boolean waitForSharedRelro,
        /* [in] */ const String& testRunnerClassName);

    /**
     * Use this constructor to recreate a LinkerParams instance from an Intent.
     * @param intent An Intent, its content must have been populated by a previous
     * call to addIntentExtras().
     */
    ChromiumLinkerParams(
        /* [in] */ IIntent* intent);

    CAR_INTERFACE_DECL();

    /**
     * Ensure this LinkerParams instance is sent to a service process by adding
     * it to an intent's extras.
     * @param intent An Intent use to start or connect to the child service process.
     */
    CARAPI_(void) AddIntentExtras(
        /* [in] */ IIntent* intent);

    // For debugging traces only.
    CARAPI_(String) ToString();

public:
    // Use this base address to load native shared libraries. If 0, ignore other members.
    const Int64 mBaseLoadAddress;

    // If true, wait for a shared RELRO Bundle just after loading the libraries.
    const Boolean mWaitForSharedRelro;

    // If not empty, name of Linker.TestRunner implementation that needs to be
    // registered in the service process.
    const String mTestRunnerClassName;

private:
    static const String EXTRA_LINKER_PARAMS_BASE_LOAD_ADDRESS;

    static const String EXTRA_LINKER_PARAMS_WAIT_FOR_SHARED_RELRO;

    static const String EXTRA_LINKER_PARAMS_TEST_RUNNER_CLASS_NAME;
};

} // namespace App
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_CONTENT_APP_CHROMIUMLINKERPARAMS_H__
