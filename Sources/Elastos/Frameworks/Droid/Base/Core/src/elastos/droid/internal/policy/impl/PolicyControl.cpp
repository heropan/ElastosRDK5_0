
#include "elastos/droid/internal/policy/impl/PolicyControl.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

//=====================================================================
//                        PolicyControl::Filter
//=====================================================================
const String PolicyControl::Filter::ALL("*");
const String PolicyControl::Filter::APPS("apps");

ECode PolicyControl::Filter::Matches(
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(attrs);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (attrs == null) return false;
    // boolean isApp = attrs.type >= WindowManager.LayoutParams.FIRST_APPLICATION_WINDOW
    //         && attrs.type <= WindowManager.LayoutParams.LAST_APPLICATION_WINDOW;
    // if (isApp && mBlacklist.contains(APPS)) return false;
    // if (onBlacklist(attrs.packageName)) return false;
    // if (isApp && mWhitelist.contains(APPS)) return true;
    // return onWhitelist(attrs.packageName);
    assert(0);
    return NOERROR;
}

ECode PolicyControl::Filter::Matches(
    /* [in] */ const String& packageName,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return !onBlacklist(packageName) && onWhitelist(packageName);
    assert(0);
    return NOERROR;
}

ECode PolicyControl::Filter::Dump(
    /* [in] */ IPrintWriter* pw)
{
    VALIDATE_NOT_NULL(pw);
    // ==================before translated======================
    // pw.print("Filter[");
    // dump("whitelist", mWhitelist, pw); pw.print(',');
    // dump("blacklist", mBlacklist, pw); pw.print(']');
    assert(0);
    return NOERROR;
}

String PolicyControl::Filter::ToString()
{
    // ==================before translated======================
    // StringWriter sw = new StringWriter();
    // dump(new PrintWriter(sw, true));
    // return sw.toString();
    assert(0);
    return String("");
}

AutoPtr<Filter> PolicyControl::Filter::Parse(
    /* [in] */ const String& value)
{
    // ==================before translated======================
    // if (value == null) return null;
    // ArraySet<String> whitelist = new ArraySet<String>();
    // ArraySet<String> blacklist = new ArraySet<String>();
    // for (String token : value.split(",")) {
    //     token = token.trim();
    //     if (token.startsWith("-") && token.length() > 1) {
    //         token = token.substring(1);
    //         blacklist.add(token);
    //     } else {
    //         whitelist.add(token);
    //     }
    // }
    // return new Filter(whitelist, blacklist);
    assert(0);
    AutoPtr<Filter> empty;
    return empty;
}

PolicyControl::Filter::Filter(
    /* [in] */ IArraySet* whitelist,
    /* [in] */ IArraySet* blacklist)
{
    // ==================before translated======================
    // mWhitelist = whitelist;
    // mBlacklist = blacklist;
}

Boolean PolicyControl::Filter::OnBlacklist(
    /* [in] */ const String& packageName)
{
    // ==================before translated======================
    // return mBlacklist.contains(packageName) || mBlacklist.contains(ALL);
    assert(0);
    return FALSE;
}

Boolean PolicyControl::Filter::OnWhitelist(
    /* [in] */ const String& packageName)
{
    // ==================before translated======================
    // return mWhitelist.contains(ALL) || mWhitelist.contains(packageName);
    assert(0);
    return FALSE;
}

void PolicyControl::Filter::Dump(
    /* [in] */ const String& name,
    /* [in] */ IArraySet* set,
    /* [in] */ IPrintWriter* pw)
{
    // ==================before translated======================
    // pw.print(name); pw.print("=(");
    // final int n = set.size();
    // for (int i = 0; i < n; i++) {
    //     if (i > 0) pw.print(',');
    //     pw.print(set.valueAt(i));
    // }
    // pw.print(')');
    assert(0);
}

//=====================================================================
//                            PolicyControl
//=====================================================================
CAR_SINGLETON_IMPL(PolicyControl)
CAR_INTERFACE_IMPL(PolicyControl, Singleton, IPolicyControl)

String PolicyControl::TAG("PolicyControl");
Boolean PolicyControl::DEBUG = false;
const String PolicyControl::NAME_IMMERSIVE_FULL("immersive.full");
const String PolicyControl::NAME_IMMERSIVE_STATUS("immersive.status");
const String PolicyControl::NAME_IMMERSIVE_NAVIGATION("immersive.navigation");
const String PolicyControl::NAME_IMMERSIVE_PRECONFIRMATIONS("immersive.preconfirms");
String PolicyControl::sSettingValue;
AutoPtr<Filter> PolicyControl::sImmersivePreconfirmationsFilter;
AutoPtr<Filter> PolicyControl::sImmersiveStatusFilter;
AutoPtr<Filter> PolicyControl::sImmersiveNavigationFilter;

ECode PolicyControl::GetSystemUiVisibility(
    /* [in] */ IWindowState* win,
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [out] */ Int32* res)
{
    VALIDATE_NOT_NULL(res);
    // ==================before translated======================
    // attrs = attrs != null ? attrs : win.getAttrs();
    // int vis = win != null ? win.getSystemUiVisibility() : attrs.systemUiVisibility;
    // if (sImmersiveStatusFilter != null && sImmersiveStatusFilter.matches(attrs)) {
    //     vis |= View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
    //             | View.SYSTEM_UI_FLAG_FULLSCREEN
    //             | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN;
    //     vis &= ~(View.SYSTEM_UI_FLAG_LAYOUT_STABLE
    //             | View.STATUS_BAR_TRANSLUCENT);
    // }
    // if (sImmersiveNavigationFilter != null && sImmersiveNavigationFilter.matches(attrs)) {
    //     vis |= View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
    //             | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
    //             | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION;
    //     vis &= ~(View.SYSTEM_UI_FLAG_LAYOUT_STABLE
    //             | View.NAVIGATION_BAR_TRANSLUCENT);
    // }
    // return vis;
    assert(0);
    return NOERROR;
}

ECode PolicyControl::GetWindowFlags(
    /* [in] */ IWindowState* win,
    /* [in] */ IWindowManagerLayoutParams* attrs,
    /* [out] */ Int32* res)
{
    VALIDATE_NOT_NULL(res);
    // ==================before translated======================
    // attrs = attrs != null ? attrs : win.getAttrs();
    // int flags = attrs.flags;
    // if (sImmersiveStatusFilter != null && sImmersiveStatusFilter.matches(attrs)) {
    //     flags |= WindowManager.LayoutParams.FLAG_FULLSCREEN;
    //     flags &= ~(WindowManager.LayoutParams.FLAG_TRANSLUCENT_STATUS
    //             | WindowManager.LayoutParams.FLAG_FORCE_NOT_FULLSCREEN);
    // }
    // if (sImmersiveNavigationFilter != null && sImmersiveNavigationFilter.matches(attrs)) {
    //     flags &= ~WindowManager.LayoutParams.FLAG_TRANSLUCENT_NAVIGATION;
    // }
    // return flags;
    assert(0);
    return NOERROR;
}

ECode PolicyControl::AdjustClearableFlags(
    /* [in] */ IWindowState* win,
    /* [in] */ Int32 clearableFlags,
    /* [out] */ Int32* res)
{
    VALIDATE_NOT_NULL(res);
    // ==================before translated======================
    // final LayoutParams attrs = win != null ? win.getAttrs() : null;
    // if (sImmersiveStatusFilter != null && sImmersiveStatusFilter.matches(attrs)) {
    //     clearableFlags &= ~View.SYSTEM_UI_FLAG_FULLSCREEN;
    // }
    // return clearableFlags;
    assert(0);
    return NOERROR;
}

ECode PolicyControl::DisableImmersiveConfirmation(
    /* [in] */ const String& pkg,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    // ==================before translated======================
    // return (sImmersivePreconfirmationsFilter != null
    //         && sImmersivePreconfirmationsFilter.matches(pkg))
    //         || ActivityManager.isRunningInTestHarness();
    assert(0);
    return NOERROR;
}

ECode PolicyControl::ReloadFromSetting(
    /* [in] */ IContext* context)
{
    VALIDATE_NOT_NULL(context);
    // ==================before translated======================
    // if (DEBUG) Slog.d(TAG, "reloadFromSetting()");
    // String value = null;
    // try {
    //     value = Settings.Global.getStringForUser(context.getContentResolver(),
    //             Settings.Global.POLICY_CONTROL,
    //             UserHandle.USER_CURRENT);
    //     if (sSettingValue != null && sSettingValue.equals(value)) return;
    //     setFilters(value);
    //     sSettingValue = value;
    // } catch (Throwable t) {
    //     Slog.w(TAG, "Error loading policy control, value=" + value, t);
    // }
    assert(0);
    return NOERROR;
}

ECode PolicyControl::Dump(
    /* [in] */ const String& prefix,
    /* [in] */ IPrintWriter* pw)
{
    VALIDATE_NOT_NULL(pw);
    // ==================before translated======================
    // dump("sImmersiveStatusFilter", sImmersiveStatusFilter, prefix, pw);
    // dump("sImmersiveNavigationFilter", sImmersiveNavigationFilter, prefix, pw);
    // dump("sImmersivePreconfirmationsFilter", sImmersivePreconfirmationsFilter, prefix, pw);
    assert(0);
    return NOERROR;
}

void PolicyControl::Dump(
    /* [in] */ const String& name,
    /* [in] */ Filter* filter,
    /* [in] */ const String& prefix,
    /* [in] */ IPrintWriter* pw)
{
    // ==================before translated======================
    // pw.print(prefix); pw.print("PolicyControl."); pw.print(name); pw.print('=');
    // if (filter == null) {
    //     pw.println("null");
    // } else {
    //     filter.dump(pw); pw.println();
    // }
    assert(0);
}

void PolicyControl::SetFilters(
    /* [in] */ const String& value)
{
    // ==================before translated======================
    // if (DEBUG) Slog.d(TAG, "setFilters: " + value);
    // sImmersiveStatusFilter = null;
    // sImmersiveNavigationFilter = null;
    // sImmersivePreconfirmationsFilter = null;
    // if (value != null) {
    //     String[] nvps = value.split(":");
    //     for (String nvp : nvps) {
    //         int i = nvp.indexOf('=');
    //         if (i == -1) continue;
    //         String n = nvp.substring(0, i);
    //         String v = nvp.substring(i + 1);
    //         if (n.equals(NAME_IMMERSIVE_FULL)) {
    //             Filter f = Filter.parse(v);
    //             sImmersiveStatusFilter = sImmersiveNavigationFilter = f;
    //             if (sImmersivePreconfirmationsFilter == null) {
    //                 sImmersivePreconfirmationsFilter = f;
    //             }
    //         } else if (n.equals(NAME_IMMERSIVE_STATUS)) {
    //             Filter f = Filter.parse(v);
    //             sImmersiveStatusFilter = f;
    //         } else if (n.equals(NAME_IMMERSIVE_NAVIGATION)) {
    //             Filter f = Filter.parse(v);
    //             sImmersiveNavigationFilter = f;
    //             if (sImmersivePreconfirmationsFilter == null) {
    //                 sImmersivePreconfirmationsFilter = f;
    //             }
    //         } else if (n.equals(NAME_IMMERSIVE_PRECONFIRMATIONS)) {
    //             Filter f = Filter.parse(v);
    //             sImmersivePreconfirmationsFilter = f;
    //         }
    //     }
    // }
    // if (DEBUG) {
    //     Slog.d(TAG, "immersiveStatusFilter: " + sImmersiveStatusFilter);
    //     Slog.d(TAG, "immersiveNavigationFilter: " + sImmersiveNavigationFilter);
    //     Slog.d(TAG, "immersivePreconfirmationsFilter: " + sImmersivePreconfirmationsFilter);
    // }
    assert(0);
}

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos


