
#include "webkit/native/content/browser/ContentSettings.h"
#include "webkit/native/content/browser/ContentViewCore.h"
#include "webkit/native/base/ThreadUtils.h"

using Elastos::Droid::Webkit::Base::ThreadUtils;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

const String ContentSettings::TAG("ContentSettings");

/**
 * Package constructor to prevent clients from creating a new settings
 * instance. Must be called on the UI thread.
 */
ContentSettings::ContentSettings(
    /* [in] */ ContentViewCore* contentViewCore,
    /* [in] */ Int64 nativeContentView)
{
    ThreadUtils::AssertOnUiThread();
    mContentViewCore = contentViewCore;
    mNativeContentSettings = NativeInit(nativeContentView);
    assert(mNativeContentSettings != 0);
}

/**
 * Notification from the native side that it is being destroyed.
 * @param nativeContentSettings the native instance that is going away.
 */
//@CalledByNative
void ContentSettings::OnNativeContentSettingsDestroyed(
    /* [in] */ Int64 nativeContentSettings)
{
    assert(mNativeContentSettings == nativeContentSettings);
    mNativeContentSettings = 0;
}

/**
 * Return true if JavaScript is enabled. Must be called on the UI thread.
 *
 * @return True if JavaScript is enabled.
 */
Boolean ContentSettings::GetJavaScriptEnabled()
{
    ThreadUtils::AssertOnUiThread();
    return mNativeContentSettings != 0 ?
            NativeGetJavaScriptEnabled(mNativeContentSettings) : FALSE;
}

// Initialize the ContentSettings native side.
Int64 ContentSettings::NativeInit(
    /* [in] */ Int64 contentViewPtr)
{
}

Boolean ContentSettings::NativeGetJavaScriptEnabled(
    /* [in] */ Int64 nativeContentSettings)
{
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
