
#include "elastos/droid/webkit/native/content/app/ChromiumLinkerParams.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace App {


const String ChromiumLinkerParams::EXTRA_LINKER_PARAMS_BASE_LOAD_ADDRESS =
    "org.chromium.content.common.linker_params.base_load_address";

const String ChromiumLinkerParams::EXTRA_LINKER_PARAMS_WAIT_FOR_SHARED_RELRO =
    "org.chromium.content.common.linker_params.wait_for_shared_relro";

const String ChromiumLinkerParams::EXTRA_LINKER_PARAMS_TEST_RUNNER_CLASS_NAME =
    "org.chromium.content.common.linker_params.test_runner_class_name";

ChromiumLinkerParams::ChromiumLinkerParams(
    /* [in] */ Int64 baseLoadAddress,
    /* [in] */ Boolean waitForSharedRelro,
    /* [in] */ const String& testRunnerClassName)
    : mBaseLoadAddress(baseLoadAddress)
    , mWaitForSharedRelro(waitForSharedRelro)
    , mTestRunnerClassName(testRunnerClassName)
{
}

/**
 * Use this constructor to recreate a LinkerParams instance from an Intent.
 * @param intent An Intent, its content must have been populated by a previous
 * call to addIntentExtras().
 */
ChromiumLinkerParams::ChromiumLinkerParams(
    /* [in] */ IIntent* intent)
{
    assert(0);
#if 0
    mBaseLoadAddress = intent.getLongExtra(EXTRA_LINKER_PARAMS_BASE_LOAD_ADDRESS, 0);
    mWaitForSharedRelro = intent.getBooleanExtra(
            EXTRA_LINKER_PARAMS_WAIT_FOR_SHARED_RELRO, false);
    mTestRunnerClassName = intent.getStringExtra(
            EXTRA_LINKER_PARAMS_TEST_RUNNER_CLASS_NAME);
#endif
}

/**
 * Ensure this LinkerParams instance is sent to a service process by adding
 * it to an intent's extras.
 * @param intent An Intent use to start or connect to the child service process.
 */
void ChromiumLinkerParams::AddIntentExtras(
    /* [in] */ IIntent* intent)
{
    assert(0);
#if 0
    intent.putExtra(EXTRA_LINKER_PARAMS_BASE_LOAD_ADDRESS, mBaseLoadAddress);
    intent.putExtra(EXTRA_LINKER_PARAMS_WAIT_FOR_SHARED_RELRO, mWaitForSharedRelro);
    intent.putExtra(EXTRA_LINKER_PARAMS_TEST_RUNNER_CLASS_NAME, mTestRunnerClassName);
#endif
}

// For debugging traces only.
String ChromiumLinkerParams::ToString()
{
    assert(0);
#if 0
    return String.format(
            "LinkerParams(baseLoadAddress:0x%x, waitForSharedRelro:%s, " +
                    "testRunnerClassName:%s",
            mBaseLoadAddress,
            mWaitForSharedRelro ? "true" : "false",
            mTestRunnerClassName);
#endif
    return String(NULL);
}

} // namespace App
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
