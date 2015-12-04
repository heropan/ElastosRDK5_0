
#include "elastos/droid/internal/policy/impl/GlobalKeyManager.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

//=====================================================================
//                           GlobalKeyManager
//=====================================================================
const String GlobalKeyManager::TAG("GlobalKeyManager");
const String GlobalKeyManager::TAG_GLOBAL_KEYS("global_keys");
const String GlobalKeyManager::ATTR_VERSION("version");
const String GlobalKeyManager::TAG_KEY("key");
const String GlobalKeyManager::ATTR_KEY_CODE("keyCode");
const String GlobalKeyManager::ATTR_COMPONENT("component");
const Int32 GlobalKeyManager::GLOBAL_KEY_FILE_VERSION;

GlobalKeyManager::GlobalKeyManager(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // mKeyMapping = new SparseArray<ComponentName>();
    // loadGlobalKeys(context);
}

ECode GlobalKeyManager::HandleGlobalKey(
    /* [in] */ IContext* context,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(event);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (mKeyMapping.size() > 0) {
    //     ComponentName component = mKeyMapping.get(keyCode);
    //     if (component != null) {
    //         Intent intent = new Intent(Intent.ACTION_GLOBAL_BUTTON)
    //                 .setComponent(component)
    //                 .putExtra(Intent.EXTRA_KEY_EVENT, event);
    //         context.sendBroadcastAsUser(intent, UserHandle.CURRENT, null);
    //         return true;
    //     }
    // }
    // return false;
    assert(0);
    return NOERROR;
}

ECode GlobalKeyManager::ShouldHandleGlobalKey(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(event);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mKeyMapping.get(keyCode) != null;
    assert(0);
    return NOERROR;
}

ECode GlobalKeyManager::Dump(
    /* [in] */ String prefix,
    /* [in] */ IPrintWriter* pw)
{
    VALIDATE_NOT_NULL(pw);
    // ==================before translated======================
    // final int numKeys = mKeyMapping.size();
    // if (numKeys == 0) {
    //     pw.print(prefix); pw.println("mKeyMapping.size=0");
    //     return;
    // }
    // pw.print(prefix); pw.println("mKeyMapping={");
    // for (int i = 0; i < numKeys; ++i) {
    //     pw.print("  ");
    //     pw.print(prefix);
    //     pw.print(KeyEvent.keyCodeToString(mKeyMapping.keyAt(i)));
    //     pw.print("=");
    //     pw.println(mKeyMapping.valueAt(i).flattenToString());
    // }
    // pw.print(prefix); pw.println("}");
    assert(0);
    return NOERROR;
}

void GlobalKeyManager::LoadGlobalKeys(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // XmlResourceParser parser = null;
    // try {
    //     parser = context.getResources().getXml(com.android.internal.R.xml.global_keys);
    //     XmlUtils.beginDocument(parser, TAG_GLOBAL_KEYS);
    //     int version = parser.getAttributeIntValue(null, ATTR_VERSION, 0);
    //     if (GLOBAL_KEY_FILE_VERSION == version) {
    //         while (true) {
    //             XmlUtils.nextElement(parser);
    //             String element = parser.getName();
    //             if (element == null) {
    //                 break;
    //             }
    //             if (TAG_KEY.equals(element)) {
    //                 String keyCodeName = parser.getAttributeValue(null, ATTR_KEY_CODE);
    //                 String componentName = parser.getAttributeValue(null, ATTR_COMPONENT);
    //                 int keyCode = KeyEvent.keyCodeFromString(keyCodeName);
    //                 if (keyCode != KeyEvent.KEYCODE_UNKNOWN) {
    //                     mKeyMapping.put(keyCode, ComponentName.unflattenFromString(
    //                             componentName));
    //                 }
    //             }
    //         }
    //     }
    // } catch (Resources.NotFoundException e) {
    //     Log.w(TAG, "global keys file not found", e);
    // } catch (XmlPullParserException e) {
    //     Log.w(TAG, "XML parser exception reading global keys file", e);
    // } catch (IOException e) {
    //     Log.w(TAG, "I/O exception reading global keys file", e);
    // } finally {
    //     if (parser != null) {
    //         parser.close();
    //     }
    // }
    assert(0);
}

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos


