
#ifndef __CAPPWIDGETPROVIDERINFO_H__
#define __CAPPWIDGETPROVIDERINFO_H__

#include "_CAppWidgetProviderInfo.h"

using Elastos::Droid::Content::IComponentName;

namespace Elastos {
namespace Droid {
namespace AppWidget {

CarClass(CAppWidgetProviderInfo)
{
public:
    CAppWidgetProviderInfo();

    CARAPI constructor();

    CARAPI SetProvider(
        /* [in] */ IComponentName* name);

    CARAPI GetProvider(
        /* [out] */ IComponentName** name);

    CARAPI SetMinWidth(
        /* [in] */ Int32 width);

    CARAPI GetMinWidth(
        /* [out] */ Int32* width);

    CARAPI SetMinHeight(
        /* [in] */ Int32 height);

    CARAPI GetMinHeight(
        /* [out] */ Int32* height);

    CARAPI SetMinResizeWidth(
        /* [in] */ Int32 width);

    CARAPI GetMinResizeWidth(
        /* [out] */ Int32* width);

    CARAPI SetMinResizeHeight(
        /* [in] */ Int32 height);

    CARAPI GetMinResizeHeight(
        /* [out] */ Int32* height);

    CARAPI SetUpdatePeriodMillis(
        /* [in] */ Int32 period);

    CARAPI GetUpdatePeriodMillis(
        /* [out] */ Int32* period);

    CARAPI SetInitialLayout(
        /* [in] */ Int32 layout);

    CARAPI GetInitialKeyguardLayout(
        /* [out] */ Int32* layout);

    CARAPI SetInitialKeyguardLayout(
        /* [in] */ Int32 layout);

    CARAPI GetInitialLayout(
        /* [out] */ Int32* layout);

    CARAPI SetConfigure(
        /* [in] */ IComponentName* name);

    CARAPI GetConfigure(
        /* [out] */ IComponentName** name);

    CARAPI SetLabel(
        /* [in] */ const String& label);

    CARAPI GetLabel(
        /* [out] */ String* label);

    CARAPI SetIcon(
        /* [in] */ Int32 icon);

    CARAPI GetIcon(
        /* [out] */ Int32* icon);

    CARAPI SetAutoAdvanceViewId(
        /* [in] */ Int32 id);

    CARAPI GetAutoAdvanceViewId(
        /* [out]*/ Int32* id);

    CARAPI SetPreviewImage(
        /* [in] */ Int32 image);

    CARAPI GetPreviewImage(
        /* [out]*/ Int32* image);

    CARAPI SetResizeMode(
        /* [in] */ Int32 mode);

    CARAPI GetResizeMode(
        /* [out]*/ Int32* mode);

    CARAPI SetWidgetCategory(
        /* [in] */ Int32 category);

    CARAPI GetWidgetCategory(
        /* [out]*/ Int32* category);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);

    CARAPI Clone(
        /* [out] */ IAppWidgetProviderInfo** info);

public:
    /**
     * Identity of this AppWidget component.  This component should be a {@link
     * android.content.BroadcastReceiver}, and it will be sent the AppWidget intents
     * {@link android.appwidget as described in the AppWidget package documentation}.
     *
     * <p>This field corresponds to the <code>android:name</code> attribute in
     * the <code>&lt;receiver&gt;</code> element in the AndroidManifest.xml file.
     */
    AutoPtr<IComponentName> mProvider;

    /**
     * The default height of the widget when added to a host, in dp. The widget will get
     * at least this width, and will often be given more, depending on the host.
     *
     * <p>This field corresponds to the <code>android:minWidth</code> attribute in
     * the AppWidget meta-data file.
     */
    Int32 mMinWidth;

    /**
     * The default height of the widget when added to a host, in dp. The widget will get
     * at least this height, and will often be given more, depending on the host.
     *
     * <p>This field corresponds to the <code>android:minHeight</code> attribute in
     * the AppWidget meta-data file.
     */
    Int32 mMinHeight;

    /**
     * Minimum width (in dp) which the widget can be resized to. This field has no effect if it
     * is greater than minWidth or if horizontal resizing isn't enabled (see {@link #resizeMode}).
     *
     * <p>This field corresponds to the <code>android:minResizeWidth</code> attribute in
     * the AppWidget meta-data file.
     */
    Int32 mMinResizeWidth;

    /**
     * Minimum height (in dp) which the widget can be resized to. This field has no effect if it
     * is greater than minHeight or if vertical resizing isn't enabled (see {@link #resizeMode}).
     *
     * <p>This field corresponds to the <code>android:minResizeHeight</code> attribute in
     * the AppWidget meta-data file.
     */
    Int32 mMinResizeHeight;

    /**
     * How often, in milliseconds, that this AppWidget wants to be updated.
     * The AppWidget manager may place a limit on how often a AppWidget is updated.
     *
     * <p>This field corresponds to the <code>android:updatePeriodMillis</code> attribute in
     * the AppWidget meta-data file.
     *
     * <p class="note"><b>Note:</b> Updates requested with <code>updatePeriodMillis</code>
     * will not be delivered more than once every 30 minutes.</p>
     */
    Int32 mUpdatePeriodMillis;

    /**
     * The resource id of the initial layout for this AppWidget.  This should be
     * displayed until the RemoteViews for the AppWidget is available.
     *
     * <p>This field corresponds to the <code>android:initialLayout</code> attribute in
     * the AppWidget meta-data file.
     */
    Int32 mInitialLayout;

    /**
     * The resource id of the initial layout for this AppWidget when it is displayed on keyguard.
     * This parameter only needs to be provided if the widget can be displayed on the keyguard,
     * see {@link #widgetCategory}.
     *
     * <p>This field corresponds to the <code>android:initialKeyguardLayout</code> attribute in
     * the AppWidget meta-data file.
     */
    Int32 mInitialKeyguardLayout;

    /**
     * The activity to launch that will configure the AppWidget.
     *
     * <p>This class name of field corresponds to the <code>android:configure</code> attribute in
     * the AppWidget meta-data file.  The package name always corresponds to the package containing
     * the AppWidget provider.
     */
    AutoPtr<IComponentName> mConfigure;

    /**
     * The label to display to the user in the AppWidget picker.  If not supplied in the
     * xml, the application label will be used.
     *
     * <p>This field corresponds to the <code>android:label</code> attribute in
     * the <code>&lt;receiver&gt;</code> element in the AndroidManifest.xml file.
     */
    String mLabel;

    /**
     * The icon to display for this AppWidget in the AppWidget picker.  If not supplied in the
     * xml, the application icon will be used.
     *
     * <p>This field corresponds to the <code>android:icon</code> attribute in
     * the <code>&lt;receiver&gt;</code> element in the AndroidManifest.xml file.
     */
    Int32 mIcon;

    /**
     * The view id of the AppWidget subview which should be auto-advanced by the widget's host.
     *
     * <p>This field corresponds to the <code>android:autoAdvanceViewId</code> attribute in
     * the AppWidget meta-data file.
     */
    Int32 mAutoAdvanceViewId;

    /**
     * A preview of what the AppWidget will look like after it's configured.
     * If not supplied, the AppWidget's icon will be used.
     *
     * <p>This field corresponds to the <code>android:previewImage</code> attribute in
     * the <code>&lt;receiver&gt;</code> element in the AndroidManifest.xml file.
     */
    Int32 mPreviewImage;

    /**
     * The rules by which a widget can be resized. See {@link #RESIZE_NONE},
     * {@link #RESIZE_NONE}, {@link #RESIZE_HORIZONTAL},
     * {@link #RESIZE_VERTICAL}, {@link #RESIZE_BOTH}.
     *
     * <p>This field corresponds to the <code>android:resizeMode</code> attribute in
     * the AppWidget meta-data file.
     */
    Int32 mResizeMode;

    /**
     * Determines whether this widget can be displayed on the home screen, the keyguard, or both.
     * A widget which is displayed on both needs to ensure that it follows the design guidelines
     * for both widget classes. This can be achieved by querying the AppWidget options in its
     * widget provider's update method.
     *
     * <p>This field corresponds to the <code>widgetCategory</code> attribute in
     * the AppWidget meta-data file.
     */
    Int32 mWidgetCategory;
};

} // namespace AppWidget
} // namespace Droid
} // namespace Elastos

#endif //__CAPPWIDGETPROVIDERINFO_H__
