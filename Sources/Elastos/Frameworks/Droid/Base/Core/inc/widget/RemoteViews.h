
#ifndef __REMOTEVIEWS_H__
#define __REMOTEVIEWS_H__

#include "widget/Filter.h"
#include "ext/frameworkext.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::Graphics::PorterDuffMode;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::Widget::IAdapterViewOnItemClickListener;

namespace Elastos {
namespace Droid {
namespace Widget {

extern "C" const InterfaceID EIID_RemoteViews;

class RemoteViewsOnClickHandler
    : public ElRefBase
    , public IRemoteViewsOnClickHandler
{
public:
    CAR_INTERFACE_DECL()

    CARAPI OnClickHandler(
        /* [in] */ IView* view,
        /* [in] */ IPendingIntent* pending,
        /* [in] */ IIntent* fillInIntent,
        /* [out] */ Boolean* succeed);
};

/**
 * Simple class used to keep track of memory usage in a RemoteViews.
 *
 */
class MemoryUsageCounter : public ElRefBase
{
public:
    MemoryUsageCounter();

    CARAPI_(void) Clear();

    CARAPI_(void) Increment(
        /* [in */ Int32 numBytes);

    CARAPI_(Int32) GetMemoryUsage();

    CARAPI_(void) AddBitmapMemory(
        /* [in */ IBitmap* b);

public:
    Int32 mMemoryUsage;
};


/**
 * A class that describes a view hierarchy that can be displayed in
 * another process. The hierarchy is inflated from a layout resource
 * file, and this class provides some basic operations for modifying
 * the content of the inflated hierarchy.
 */
class RemoteViews
{
private:
    /**
     * Base class for all actions that can be performed on an
     * inflated view.
     *
     *  SUBCLASSES MUST BE IMMUTABLE SO CLONE WORKS!!!!!
     */
    class Action
        : public ElRefBase
        , public IParcelable
    {
    public:
        CAR_INTERFACE_DECL()

        Action();

        Action(
            /* [in] */ Int32 viewId);

        virtual CARAPI_(void) Apply(
            /* [in] */ IView* root,
            /* [in] */ IViewGroup* rootParent,
            /* [in] */ IRemoteViewsOnClickHandler* handler) = 0;

        CARAPI_(Int32) DescribeContents();

        /**
         * Overridden by each class to report on it's own memory usage
         */
        CARAPI_(void) UpdateMemoryUsageEstimate(
            /* [in] */ MemoryUsageCounter* counter);

        CARAPI_(void) SetBitmapCache(
            /* [in] */ IRemoteViewsBitmapCache* bitmapCache);

        CARAPI_(Int32) MergeBehavior();

        virtual CARAPI_(String) GetActionName() = 0;

        CARAPI_(String) GetUniqueKey();

    public:
        static const Int32 MERGE_REPLACE = 0;
        static const Int32 MERGE_APPEND = 1;
        static const Int32 MERGE_IGNORE = 2;

        Int32 mViewId;
    };

    class _SetEmptyView : public Action
    {
    public:
        _SetEmptyView();

        _SetEmptyView(
            /* [in] */ Int32 viewId,
            /* [in] */ Int32 emptyViewId);

        CARAPI_(void) Apply(
            /* [in] */ IView* root,
            /* [in] */ IViewGroup* rootParent,
            /* [in] */ IRemoteViewsOnClickHandler* handler);

        CARAPI_(String) GetActionName();

        // interface IParcelable
        CARAPI ReadFromParcel(
            /* [in] */ IParcel* source);

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

    public:
        static const Int32 TAG = 6;

        Int32 mEmptyViewId;
    };

    class _SetOnClickFillInIntent : public Action
    {
    public:
        _SetOnClickFillInIntent(
            /* [in] */ RemoteViews* host);

        _SetOnClickFillInIntent(
            /* [in] */ Int32 id,
            /* [in] */ IIntent* fillInIntent,
            /* [in] */ RemoteViews* host);

        CARAPI_(void) Apply(
            /* [in] */ IView* root,
            /* [in] */ IViewGroup* rootParent,
            /* [in] */ IRemoteViewsOnClickHandler* handler);

        CARAPI_(String) GetActionName();

        // interface IParcelable
        CARAPI ReadFromParcel(
            /* [in] */ IParcel* source);

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

    public:
        static const Int32 TAG = 9;

        AutoPtr<IIntent> mFillInIntent;
        RemoteViews* mHost;
    };

    class _SetPendingIntentTemplate : public Action
    {
    public:
        _SetPendingIntentTemplate();

        _SetPendingIntentTemplate(
            /* [in] */ Int32 id,
            /* [in] */ IPendingIntent* pendingIntentTemplate);

        CARAPI_(void) Apply(
            /* [in] */ IView* root,
            /* [in] */ IViewGroup* rootParent,
            /* [in] */ IRemoteViewsOnClickHandler* handler);

        CARAPI_(String) GetActionName();

        // interface IParcelable
        CARAPI ReadFromParcel(
            /* [in] */ IParcel* source);

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

    public:
        static const Int32 TAG = 8;

        AutoPtr<IPendingIntent> mPendingIntentTemplate;
    };

    class SetRemoteViewsAdapterIntent : public Action
    {
    public:
        SetRemoteViewsAdapterIntent();

        SetRemoteViewsAdapterIntent(
            /* [in] */ Int32 id,
            /* [in] */ IIntent* intent);

        CARAPI_(void) Apply(
            /* [in] */ IView* root,
            /* [in] */ IViewGroup* rootParent,
            /* [in] */ IRemoteViewsOnClickHandler* handler);

        CARAPI_(String) GetActionName();

        // interface IParcelable
        CARAPI ReadFromParcel(
            /* [in] */ IParcel* source);

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

    public:
        static const Int32 TAG = 10;

        AutoPtr<IIntent> mIntent;
    };

    class _SetOnClickPendingIntent : public Action
    {
    public:
        _SetOnClickPendingIntent(
            /* [in] */ RemoteViews* host);

        _SetOnClickPendingIntent(
            /* [in] */ Int32 id,
            /* [in] */ IPendingIntent* pendingIntent,
            /* [in] */ RemoteViews* host);

        CARAPI_(void) Apply(
            /* [in] */ IView* root,
            /* [in] */ IViewGroup* rootParent,
            /* [in] */ IRemoteViewsOnClickHandler* handler);

        CARAPI_(String) GetActionName();

        // interface IParcelable
        CARAPI ReadFromParcel(
            /* [in] */ IParcel* source);

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

    public:
        static const Int32 TAG = 1;

        AutoPtr<IPendingIntent> mPendingIntent;
        RemoteViews* mHost;
    };

    /**
     * Equivalent to calling a combination of {@link Drawable#setAlpha(int)},
     * {@link Drawable#setColorFilter(int, android.graphics.PorterDuff.Mode)},
     * and/or {@link Drawable#setLevel(int)} on the {@link Drawable} of a given view.
     * <p>
     * These operations will be performed on the {@link Drawable} returned by the
     * target {@link View#getBackground()} by default.  If targetBackground is false,
     * we assume the target is an {@link ImageView} and try applying the operations
     * to {@link ImageView#getDrawable()}.
     * <p>
     * You can omit specific calls by marking their values with null or -1.
     */
    class _SetDrawableParameters : public Action
    {
    public:
        _SetDrawableParameters();

        _SetDrawableParameters(
            /* [in] */ Int32 id,
            /* [in] */ Boolean targetBackground,
            /* [in] */ Int32 alpha,
            /* [in] */ Int32 colorFilter,
            /* [in] */ PorterDuffMode mode,
            /* [in] */ Int32 level);

        CARAPI_(void) Apply(
            /* [in] */ IView* root,
            /* [in] */ IViewGroup* rootParent,
            /* [in] */ IRemoteViewsOnClickHandler* handler);

        CARAPI_(String) GetActionName();

        // interface IParcelable
        CARAPI ReadFromParcel(
            /* [in] */ IParcel* source);

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

    public:
        static const Int32 TAG = 3;

        Boolean mTargetBackground;
        Int32 mAlpha;
        Int32 mColorFilter;
        PorterDuffMode mFilterMode;
        Int32 mLevel;
    };

    class ReflectionActionWithoutParams : public Action
    {
    public:
        ReflectionActionWithoutParams();

        ReflectionActionWithoutParams(
            /* [in] */ Int32 id,
            /* [in] */ const String& methodName);

        CARAPI_(void) Apply(
            /* [in] */ IView* root,
            /* [in] */ IViewGroup* rootParent,
            /* [in] */ IRemoteViewsOnClickHandler* handler);

        CARAPI_(String) GetActionName();

        // interface IParcelable
        CARAPI ReadFromParcel(
            /* [in] */ IParcel* source);

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

        CARAPI_(Int32) MergeBehavior();

    public:
        static const Int32 TAG = 5;

        String mMethodName;
    };

    class BitmapReflectionAction : public Action
    {
    public:
        BitmapReflectionAction(
            /* [in] */ RemoteViews* host);

        BitmapReflectionAction(
            /* [in] */ Int32 id,
            /* [in] */ const String& methodName,
            /* [in] */ IBitmap* bitmap,
            /* [in] */ RemoteViews* host);

        CARAPI_(void) Apply(
            /* [in] */ IView* root,
            /* [in] */ IViewGroup* rootParent,
            /* [in] */ IRemoteViewsOnClickHandler* handler);

        CARAPI_(String) GetActionName();

        CARAPI_(void) SetBitmapCache(
            /* [in] */ IRemoteViewsBitmapCache* bitmapCache);

        // interface IParcelable
        CARAPI ReadFromParcel(
            /* [in] */ IParcel* source);

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

    public:
        static const Int32 TAG = 12;

        Int32 mBitmapId;
        AutoPtr<IBitmap> mBitmap;
        String mMethodName;
        RemoteViews* mHost;
    };

    /**
     * Base class for the reflection actions.
     */
    class ReflectionAction : public Action
    {
    public:
        ReflectionAction();

        ReflectionAction(
            /* [in] */ Int32 id,
            /* [in] */ const String& methodName,
            /* [in] */ Int32 type,
            /* [in] */ IInterface* value);

        CARAPI_(void) Apply(
            /* [in] */ IView* root,
            /* [in] */ IViewGroup* rootParent,
            /* [in] */ IRemoteViewsOnClickHandler* handler);

        CARAPI_(String) GetActionName();

        CARAPI_(String) GetParameterType();

        CARAPI_(Int32) MergeBehavior();

        // interface IParcelable
        CARAPI ReadFromParcel(
            /* [in] */ IParcel* source);

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

    public:
        static const Int32 TAG = 2;

        static const Int32 BOOLEAN = 1;
        static const Int32 BYTE = 2;
        static const Int32 SHORT = 3;
        static const Int32 INT = 4;
        static const Int32 LONG = 5;
        static const Int32 FLOAT = 6;
        static const Int32 DOUBLE = 7;
        static const Int32 CHAR = 8;
        static const Int32 STRING = 9;
        static const Int32 CHAR_SEQUENCE = 10;
        static const Int32 URI = 11;
        // BITMAP actions are never stored in the list of actions. They are only used locally
        // to implement BitmapReflectionAction, which eliminates duplicates using BitmapCache.
        static const Int32 BITMAP = 12;
        static const Int32 BUNDLE = 13;
        static const Int32 INTENT = 14;

        String mMethodName;
        Int32 mType;
        AutoPtr<IInterface> mValue;
    };

    /**
     * Equivalent to calling {@link ViewGroup#addView(View)} after inflating the
     * given {@link RemoteViews}, or calling {@link ViewGroup#removeAllViews()}
     * when null. This allows users to build "nested" {@link RemoteViews}.
     */
    class ViewGroupAction : public Action
    {
    public:
        ViewGroupAction(
            /* [in] */ RemoteViews* host);

        ViewGroupAction(
            /* [in] */ Int32 viewId,
            /* [in] */ IRemoteViews* nestedViews,
            /* [in] */ RemoteViews* host);

        CARAPI_(void) Apply(
            /* [in] */ IView* root,
            /* [in] */ IViewGroup* rootParent,
            /* [in] */ IRemoteViewsOnClickHandler* handler);

        CARAPI_(String) GetActionName();

        CARAPI_(Int32) MergeBehavior();

        CARAPI_(void) UpdateMemoryUsageEstimate(
            /* [in] */ MemoryUsageCounter* counter);

        CARAPI_(void) SetBitmapCache(
            /* [in] */ IRemoteViewsBitmapCache* bitmapCache);

        // interface IParcelable
        CARAPI ReadFromParcel(
            /* [in] */ IParcel* source);

        CARAPI ReadFromParcel(
            /* [in] */ IParcel* source,
            /* [in] */ IRemoteViewsBitmapCache* bitmapCache);

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

    public:
        static const Int32 TAG = 4;

        AutoPtr<IRemoteViews> mNestedViews;
        RemoteViews* mHost;
    };

    /**
     * Helper action to set compound drawables on a TextView. Supports relative
     * (s/t/e/b) or cardinal (l/t/r/b) arrangement.
     */
    class TextViewDrawableAction : public Action
    {
    public:
        TextViewDrawableAction();

        TextViewDrawableAction(
            /* [in] */ Int32 viewId,
            /* [in] */ Boolean isRelative,
            /* [in] */ Int32 d1,
            /* [in] */ Int32 d2,
            /* [in] */ Int32 d3,
            /* [in] */ Int32 d4);

        CARAPI_(void) Apply(
            /* [in] */ IView* root,
            /* [in] */ IViewGroup* rootParent,
            /* [in] */ IRemoteViewsOnClickHandler* handler);

        CARAPI_(String) GetActionName();

        // interface IParcelable
        CARAPI ReadFromParcel(
            /* [in] */ IParcel* source);

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

    public:
        static const Int32 TAG = 11;

        Boolean mIsRelative;
        Int32 mD1;
        Int32 mD2;
        Int32 mD3;
        Int32 mD4;
    };

    /**
     * Helper action to set text size on a TextView in any supported units.
     */
    class TextViewSizeAction : public Action
    {
    public:
        TextViewSizeAction();

        TextViewSizeAction(
            /* [in] */ Int32 viewId,
            /* [in] */ Int32 units,
            /* [in] */ Float size);

        CARAPI_(void) Apply(
            /* [in] */ IView* root,
            /* [in] */ IViewGroup* rootParent,
            /* [in] */ IRemoteViewsOnClickHandler* handler);

        CARAPI_(String) GetActionName();

        // interface IParcelable
        CARAPI ReadFromParcel(
            /* [in] */ IParcel* source);

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

    public:
        static const Int32 TAG = 13;

        Int32 mUnits;
        Float mSize;
    };

    /**
     * Helper action to set padding on a View.
     */
    class ViewPaddingAction : public Action
    {
    public:
        ViewPaddingAction();

        ViewPaddingAction(
            /* [in] */ Int32 viewId,
            /* [in] */ Int32 left,
            /* [in] */ Int32 top,
            /* [in] */ Int32 right,
            /* [in] */ Int32 bottom);

        CARAPI_(void) Apply(
            /* [in] */ IView* root,
            /* [in] */ IViewGroup* rootParent,
            /* [in] */ IRemoteViewsOnClickHandler* handler);

        CARAPI_(String) GetActionName();

        // interface IParcelable
        CARAPI ReadFromParcel(
            /* [in] */ IParcel* source);

        CARAPI WriteToParcel(
            /* [in] */ IParcel* dest);

    public:
        static const Int32 TAG = 14;

        Int32 mLeft;
        Int32 mTop;
        Int32 mRight;
        Int32 mBottom;
    };

    class FillInIntentClickListener
        : public IViewOnClickListener
        , public ElRefBase
    {
    public:
        FillInIntentClickListener(
            /* [in] */ IIntent* fillInIntent,
            /* [in] */ IRemoteViewsOnClickHandler* handler);

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        AutoPtr<IIntent> mIntent;
        AutoPtr<IRemoteViewsOnClickHandler> mHandler;
    };

    class PendingIntentClickListener
        : public IViewOnClickListener
        , public ElRefBase
    {
    public:
        PendingIntentClickListener(
            /* [in] */ IPendingIntent* intent,
            /* [in] */ IRemoteViewsOnClickHandler* handler);

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        AutoPtr<IPendingIntent> mIntent;
        AutoPtr<IRemoteViewsOnClickHandler> mHandler;
    };

    class IntentTemplateOnItemClickListener
        : public IAdapterViewOnItemClickListener
        , public ElRefBase
    {
    public:
        IntentTemplateOnItemClickListener(
            /* [in] */ IPendingIntent* intent,
            /* [in] */ IRemoteViewsOnClickHandler* handler);

        CAR_INTERFACE_DECL()

        CARAPI OnItemClick(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* view,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

    private:
        AutoPtr<IPendingIntent> mIntent;
        AutoPtr<IRemoteViewsOnClickHandler> mHandler;
    };

public:
    RemoteViews(
        /* [in] */ IParcel* parcel);

    /**
     * Create a new RemoteViews object that will display the views contained
     * in the specified layout file.
     *
     * @param packageName Name of the package that contains the layout resource
     * @param layoutId The id of the layout resource
     */
    RemoteViews(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 layoutId);

    /**
     * Create a new RemoteViews object that will inflate as the specified
     * landspace or portrait RemoteViews, depending on the current configuration.
     *
     * @param landscape The RemoteViews to inflate in landscape configuration
     * @param portrait The RemoteViews to inflate in portrait configuration
     */
    RemoteViews(
        /* [in] */ IRemoteViews* landscape,
        /* [in] */ IRemoteViews* portrait);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    /**
     * Merges the passed RemoteViews actions with this RemoteViews actions according to
     * action-specific merge rules.
     *
     * @param newRv
     *
     * @hide
     */
    CARAPI MergeRemoteViews(
        /* [in] */ IRemoteViews* newRv);

    CARAPI SetUser(
        /* [in] */ IUserHandle* user);

    CARAPI_(AutoPtr<IRemoteViews>) Clone();

    CARAPI_(String) GetPackage();

    /**
     * Reutrns the layout id of the root layout associated with this RemoteViews. In the case
     * that the RemoteViews has both a landscape and portrait root, this will return the layout
     * id associated with the portrait layout.
     *
     * @return the layout id.
     */
    CARAPI_(Int32) GetLayoutId();

    /**
     * Returns an estimate of the bitmap heap memory usage for this RemoteViews.
     */
    /** @hide */
    CARAPI_(Int32) EstimateMemoryUsage();

    /**
     * Equivalent to calling {@link ViewGroup#addView(View)} after inflating the
     * given {@link RemoteViews}. This allows users to build "nested"
     * {@link RemoteViews}. In cases where consumers of {@link RemoteViews} may
     * recycle layouts, use {@link #removeAllViews(Int32)} to clear any existing
     * children.
     *
     * @param viewId The id of the parent {@link ViewGroup} to add child into.
     * @param nestedView {@link RemoteViews} that describes the child.
     */
    CARAPI AddView(
        /* [in] */ Int32 viewId,
        /* [in] */ IRemoteViews* nestedView);

    /**
     * Equivalent to calling {@link ViewGroup#removeAllViews()}.
     *
     * @param viewId The id of the parent {@link ViewGroup} to remove all
     *            children from.
     */
    CARAPI RemoveAllViews(
        /* [in] */ Int32 viewId);

    /**
     * Equivalent to calling {@link AdapterViewAnimator#showNext()}
     *
     * @param viewId The id of the view on which to call {@link AdapterViewAnimator#showNext()}
     */
    CARAPI ShowNext(
        /* [in] */ Int32 viewId);

    /**
     * Equivalent to calling {@link AdapterViewAnimator#showPrevious()}
     *
     * @param viewId The id of the view on which to call {@link AdapterViewAnimator#showPrevious()}
     */
    CARAPI ShowPrevious(
        /* [in] */ Int32 viewId);

    /**
     * Equivalent to calling {@link AdapterViewAnimator#setDisplayedChild(Int32)}
     *
     * @param viewId The id of the view on which to call
     *               {@link AdapterViewAnimator#setDisplayedChild(Int32)}
     */
    CARAPI SetDisplayedChild(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 childIndex);

    /**
     * Equivalent to calling View.setVisibility
     *
     * @param viewId The id of the view whose visibility should change
     * @param visibility The new visibility for the view
     */
    CARAPI SetViewVisibility(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 visibility);

    /**
     * Equivalent to calling TextView.setText
     *
     * @param viewId The id of the view whose text should change
     * @param text The new text for the view
     */
    CARAPI SetTextViewText(
        /* [in] */ Int32 viewId,
        /* [in] */ ICharSequence* text);

    /**
     * Equivalent to calling {@link TextView#setTextSize(Int32, float)}
     *
     * @param viewId The id of the view whose text size should change
     * @param units The units of size (e.g. COMPLEX_UNIT_SP)
     * @param size The size of the text
     */
    CARAPI SetTextViewTextSize(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 units,
        /* [in] */ Float size);

    /**
     * Equivalent to calling
     * {@link TextView#setCompoundDrawablesWithIntrinsicBounds(Int32, Int32, Int32, Int32)}.
     *
     * @param viewId The id of the view whose text should change
     * @param left The id of a drawable to place to the left of the text, or 0
     * @param top The id of a drawable to place above the text, or 0
     * @param right The id of a drawable to place to the right of the text, or 0
     * @param bottom The id of a drawable to place below the text, or 0
     */
    CARAPI SetTextViewCompoundDrawables(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    /**
     * Equivalent to calling {@link
     * TextView#setCompoundDrawablesRelativeWithIntrinsicBounds(Int32, Int32, Int32, Int32)}.
     *
     * @param viewId The id of the view whose text should change
     * @param start The id of a drawable to place before the text (relative to the
     * layout direction), or 0
     * @param top The id of a drawable to place above the text, or 0
     * @param end The id of a drawable to place after the text, or 0
     * @param bottom The id of a drawable to place below the text, or 0
     */
    CARAPI SetTextViewCompoundDrawablesRelative(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 start,
        /* [in] */ Int32 top,
        /* [in] */ Int32 end,
        /* [in] */ Int32 bottom);

    /**
     * Equivalent to calling ImageView.setImageResource
     *
     * @param viewId The id of the view whose drawable should change
     * @param srcId The new resource id for the drawable
     */
    CARAPI SetImageViewResource(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 srcId);

    /**
     * Equivalent to calling ImageView.setImageURI
     *
     * @param viewId The id of the view whose drawable should change
     * @param uri The Uri for the image
     */
    CARAPI SetImageViewUri(
        /* [in] */ Int32 viewId,
        /* [in] */ IUri* uri);

    /**
     * Equivalent to calling ImageView.setImageBitmap
     *
     * @param viewId The id of the view whose bitmap should change
     * @param bitmap The new Bitmap for the drawable
     */
    CARAPI SetImageViewBitmap(
        /* [in] */ Int32 viewId,
        /* [in] */ IBitmap* bitmap);

    /**
     * Equivalent to calling AdapterView.setEmptyView
     *
     * @param viewId The id of the view on which to set the empty view
     * @param emptyViewId The view id of the empty view
     */
    CARAPI SetEmptyView(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 emptyViewId);

    /**
     * Equivalent to calling {@link Chronometer#setBase Chronometer.setBase},
     * {@link Chronometer#setFormat Chronometer.setFormat},
     * and {@link Chronometer#start Chronometer.start()} or
     * {@link Chronometer#stop Chronometer.stop()}.
     *
     * @param viewId The id of the {@link Chronometer} to change
     * @param base The time at which the timer would have read 0:00.  This
     *             time should be based off of
     *             {@link android.os.SystemClock#elapsedRealtime SystemClock.elapsedRealtime()}.
     * @param format The Chronometer format string, or null to
     *               simply display the timer value.
     * @param started True if you want the clock to be started, false if not.
     */
    CARAPI SetChronometer(
        /* [in] */ Int32 viewId,
        /* [in] */ Int64 base,
        /* [in] */ const String& format,
        /* [in] */ Boolean started);

    /**
     * Equivalent to calling {@link ProgressBar#setMax ProgressBar.setMax},
     * {@link ProgressBar#setProgress ProgressBar.setProgress}, and
     * {@link ProgressBar#setIndeterminate ProgressBar.setIndeterminate}
     *
     * If indeterminate is true, then the values for max and progress are ignored.
     *
     * @param viewId The id of the {@link ProgressBar} to change
     * @param max The 100% value for the progress bar
     * @param progress The current value of the progress bar.
     * @param indeterminate True if the progress bar is indeterminate,
     *                false if not.
     */
    CARAPI SetProgressBar(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 max,
        /* [in] */ Int32 progress,
        /* [in] */ Boolean indeterminate);

    /**
     * Equivalent to calling
     * {@link android.view.View#setOnClickListener(android.view.View.OnClickListener)}
     * to launch the provided {@link PendingIntent}.
     *
     * When setting the on-click action of items within collections (eg. {@link ListView},
     * {@link StackView} etc.), this method will not work. Instead, use {@link
     * RemoteViews#setPendingIntentTemplate(Int32, PendingIntent) in conjunction with
     * RemoteViews#setOnClickFillInIntent(Int32, Intent).
     *
     * @param viewId The id of the view that will trigger the {@link PendingIntent} when clicked
     * @param pendingIntent The {@link PendingIntent} to send when user clicks
     */
    CARAPI SetOnClickPendingIntent(
        /* [in] */ Int32 viewId,
        /* [in] */ IPendingIntent* pendingIntent);

    /**
     * When using collections (eg. {@link ListView}, {@link StackView} etc.) in widgets, it is very
     * costly to set PendingIntents on the individual items, and is hence not permitted. Instead
     * this method should be used to set a single PendingIntent template on the collection, and
     * individual items can differentiate their on-click behavior using
     * {@link RemoteViews#setOnClickFillInIntent(Int32, Intent)}.
     *
     * @param viewId The id of the collection who's children will use this PendingIntent template
     *          when clicked
     * @param pendingIntentTemplate The {@link PendingIntent} to be combined with extras specified
     *          by a child of viewId and executed when that child is clicked
     */
    CARAPI SetPendingIntentTemplate(
        /* [in] */ Int32 viewId,
        /* [in] */ IPendingIntent* pendingIntentTemplate);

    /**
     * When using collections (eg. {@link ListView}, {@link StackView} etc.) in widgets, it is very
     * costly to set PendingIntents on the individual items, and is hence not permitted. Instead
     * a single PendingIntent template can be set on the collection, see {@link
     * RemoteViews#setPendingIntentTemplate(Int32, PendingIntent)}, and the individual on-click
     * action of a given item can be distinguished by setting a fillInIntent on that item. The
     * fillInIntent is then combined with the PendingIntent template in order to determine the final
     * intent which will be executed when the item is clicked. This works as follows: any fields
     * which are left blank in the PendingIntent template, but are provided by the fillInIntent
     * will be overwritten, and the resulting PendingIntent will be used.
     *
     *
     * of the PendingIntent template will then be filled in with the associated fields that are
     * set in fillInIntent. See {@link Intent#fillIn(Intent, Int32)} for more details.
     *
     * @param viewId The id of the view on which to set the fillInIntent
     * @param fillInIntent The intent which will be combined with the parent's PendingIntent
     *        in order to determine the on-click behavior of the view specified by viewId
     */
    CARAPI SetOnClickFillInIntent(
        /* [in] */ Int32 viewId,
        /* [in] */ IIntent* fillInIntent);

    /**
     * @hide
     * Equivalent to calling a combination of {@link Drawable#setAlpha(Int32)},
     * {@link Drawable#setColorFilter(Int32, android.graphics.PorterDuff.Mode)},
     * and/or {@link Drawable#setLevel(Int32)} on the {@link Drawable} of a given
     * view.
     * <p>
     * You can omit specific calls by marking their values with null or -1.
     *
     * @param viewId The id of the view that contains the target
     *            {@link Drawable}
     * @param targetBackground If true, apply these parameters to the
     *            {@link Drawable} returned by
     *            {@link android.view.View#getBackground()}. Otherwise, assume
     *            the target view is an {@link ImageView} and apply them to
     *            {@link ImageView#getDrawable()}.
     * @param alpha Specify an alpha value for the drawable, or -1 to leave
     *            unchanged.
     * @param colorFilter Specify a color for a
     *            {@link android.graphics.ColorFilter} for this drawable, or -1
     *            to leave unchanged.
     * @param mode Specify a PorterDuff mode for this drawable, or null to leave
     *            unchanged.
     * @param level Specify the level for the drawable, or -1 to leave
     *            unchanged.
     */
    CARAPI SetDrawableParameters(
        /* [in] */ Int32 viewId,
        /* [in] */ Boolean targetBackground,
        /* [in] */ Int32 alpha,
        /* [in] */ Int32 colorFilter,
        /* [in] */ PorterDuffMode mode,
        /* [in] */ Int32 level);

    /**
     * Equivalent to calling {@link android.widget.TextView#setTextColor(Int32)}.
     *
     * @param viewId The id of the view whose text color should change
     * @param color Sets the text color for all the states (normal, selected,
     *            focused) to be this color.
     */
    CARAPI SetTextColor(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 color);

    /**
     * Equivalent to calling {@link android.widget.AbsListView#setRemoteViewsAdapter(Intent)}.
     *
     * @param appWidgetId The id of the app widget which contains the specified view. (This
     *      parameter is ignored in this deprecated method)
     * @param viewId The id of the {@link AbsListView}
     * @param intent The intent of the service which will be
     *            providing data to the RemoteViewsAdapter
     * @deprecated This method has been deprecated. See
     *      {@link android.widget.RemoteViews#setRemoteAdapter(Int32, Intent)}
     */
    CARAPI SetRemoteAdapter(
        /* [in] */ Int32 viewId,
        /* [in] */ IIntent* intent);

    //    @Deprecated
    CARAPI SetRemoteAdapter(
        /* [in] */ Int32 appWidgetId,
        /* [in] */ Int32 viewId,
        /* [in] */ IIntent* intent);

    /**
     * Equivalent to calling {@link android.widget.AbsListView#smoothScrollToPosition(Int32, Int32)}.
     *
     * @param viewId The id of the view to change
     * @param position Scroll to this adapter position
     */
    CARAPI SetScrollPosition(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 position);

    /**
     * Equivalent to calling {@link android.widget.AbsListView#smoothScrollToPosition(Int32, Int32)}.
     *
     * @param viewId The id of the view to change
     * @param offset Scroll by this adapter position offset
     */
    CARAPI SetRelativeScrollPosition(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 offset);

    /**
     * Equivalent to calling {@link android.view.View#setPadding(Int32, Int32, Int32, Int32)}.
     *
     * @param viewId The id of the view to change
     * @param left the left padding in pixels
     * @param top the top padding in pixels
     * @param right the right padding in pixels
     * @param bottom the bottom padding in pixels
     */
    CARAPI SetViewPadding(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    /**
     * Call a method taking one Boolean on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetBoolean(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Boolean value);

    /**
     * Call a method taking one byte on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetByte(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Byte value);

    /**
     * Call a method taking one short on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetInt16(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Int16 value);

    /**
     * Call a method taking one Int32 on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetInt32(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Int32 value);

    /**
     * Call a method taking one long on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetInt64(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Int64 value);

    /**
     * Call a method taking one float on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetFloat(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Float value);

    /**
     * Call a method taking one double on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetDouble(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Double value);

    /**
     * Call a method taking one char on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetChar(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ Char32 value);

    /**
     * Call a method taking one String on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetString(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ const String& value);

    /**
     * Call a method taking one CharSequence on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetCharSequence(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ ICharSequence* value);

    /**
     * Call a method taking one Uri on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetUri(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ IUri* value);

    /**
     * Call a method taking one Bitmap on a view in the layout for this RemoteViews.
     * @more
     * <p class="note">The bitmap will be flattened into the parcel if this object is
     * sent across processes, so it may end up using a lot of memory, and may be fairly slow.</p>
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetBitmap(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ IBitmap* value);

    /**
     * Call a method taking one Bundle on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The value to pass to the method.
     */
    CARAPI SetBundle(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ IBundle* value);

    /**
     * Call a method taking one Intent on a view in the layout for this RemoteViews.
     *
     * @param viewId The id of the view on which to call the method.
     * @param methodName The name of the method to call.
     * @param value The {@link android.content.Intent} to pass the method.
     */
    CARAPI SetIntent(
        /* [in] */ Int32 viewId,
        /* [in] */ const String& methodName,
        /* [in] */ IIntent* value);

    /**
     * Equivalent to calling View.setContentDescription(CharSequence).
     *
     * @param viewId The id of the view whose content description should change.
     * @param contentDescription The new content description for the view.
     */
    CARAPI SetContentDescription(
        /* [in] */ Int32 viewId,
        /* [in] */ ICharSequence* contentDescription);

    /**
     * Equivalent to calling View.setLabelFor(Int32).
     *
     * @param viewId The id of the view whose property to set.
     * @param labeledId The id of a view for which this view serves as a label.
     */
    CARAPI SetLabelFor(
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 labeledId);

    /**
     * Inflates the view hierarchy represented by this object and applies
     * all of the actions.
     *
     * <p><strong>Caller beware: this may throw</strong>
     *
     * @param context Default context to use
     * @param parent Parent that the resulting view hierarchy will be attached to. This method
     * does <strong>not</strong> attach the hierarchy. The caller should do so when appropriate.
     * @return The inflated view hierarchy
     */
    CARAPI_(AutoPtr<IView>) Apply(
        /* [in] */ IContext* context,
        /* [in] */ IViewGroup* parent);

    /** @hide */
    CARAPI_(AutoPtr<IView>) Apply(
        /* [in] */ IContext* context,
        /* [in] */ IViewGroup* parent,
        /* [in] */ IRemoteViewsOnClickHandler* handler);

    /**
     * Applies all of the actions to the provided view.
     *
     * <p><strong>Caller beware: this may throw</strong>
     *
     * @param v The view to apply the actions to.  This should be the result of
     * the {@link #apply(Context,ViewGroup)} call.
     */
    CARAPI Reapply(
        /* [in] */ IContext* context,
        /* [in] */ IView* v);

    /** @hide */
    CARAPI Reapply(
        /* [in] */ IContext* context,
        /* [in] */ IView* v,
        /* [in] */ IRemoteViewsOnClickHandler* handler);

    /* (non-Javadoc)
     * Used to restrict the views which can be inflated
     *
     * @see android.view.LayoutInflater.Filter#onLoadClass(java.lang.Class)
     */
    CARAPI OnLoadClass(
        /* [in] */ Handle32 clazz,
        /* [out] */ Boolean* res);

    CARAPI SetNotRoot();

    virtual CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    /*
     * This flag indicates whether this RemoteViews object is being created from a
     * RemoteViewsService for use as a child of a widget collection. This flag is used
     * to determine whether or not certain features are available, in particular,
     * setting on click extras and setting on click pending intents. The former is enabled,
     * and the latter disabled when this flag is true.
     */
    CARAPI SetIsWidgetCollectionChild(
        /* [in] */ Boolean isWidgetCollectionChild);

    CARAPI_(String) ToString();

protected:
    RemoteViews();

    CARAPI Init(
        /* [in] */ IParcel* parcel);

    CARAPI Init(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 layoutId);

    CARAPI Init(
        /* [in] */ IRemoteViews* landscape,
        /* [in] */ IRemoteViews* portrait);

private:
    RemoteViews(
        /* [in] */ IParcel* parcel,
        /* [in] */ IRemoteViewsBitmapCache* bitmapCache);

    CARAPI InitBitmapCache(
        /* [in] */ IParcel* parcel,
        /* [in] */ IRemoteViewsBitmapCache* bitmapCache);

    CARAPI Init(
        /* [in] */ IParcel* parcel,
        /* [in] */ IRemoteViewsBitmapCache* bitmapCache);

    CARAPI_(void) ConfigureRemoteViewsAsChild(
        /* [in] */ IRemoteViews* rv);

    CARAPI_(Boolean) HasLandscapeAndPortraitLayouts();

    CARAPI_(AutoPtr<IRemoteViews>) GetRemoteViewsToApply(
        /* [in] */ IContext* context);

    CARAPI_(void) PerformApply(
        /* [in] */ IView* v,
        /* [in] */ IViewGroup* parent,
        /* [in] */ IRemoteViewsOnClickHandler* handler);

    CARAPI_(AutoPtr<IContext>) PrepareContext(
        /* [in] */ IContext* context);

    CARAPI_(void) RecalculateMemoryUsage();

    CARAPI_(void) SetBitmapCache(
        /* [in] */ IRemoteViewsBitmapCache* bitmapCache);

    /**
     * Add an action to be executed on the remote side when apply is called.
     *
     * @param a The action to add
     */
    CARAPI AddAction(
        /* [in] */ Action* a);

public:
    static const String EXTRA_REMOTEADAPTER_APPWIDGET_ID;

private:
    /**
     * User that these views should be applied as. Requires
     * {@link android.Manifest.permission#INTERACT_ACROSS_USERS_FULL} when
     * crossing user boundaries.
     */
    AutoPtr<IUserHandle> mUser;

    /**
     * The package name of the package containing the layout
     * resource. (Added to the parcel)
     */
    String mPackage;

    /**
     * The resource ID of the layout file. (Added to the parcel)
     */
    Int32 mLayoutId;

    /**
     * An array of actions to perform on the view tree once it has been
     * inflated
     */
    List< AutoPtr<Action> > mActions;

    /**
     * A class to keep track of memory usage by this RemoteViews
     */
    AutoPtr<MemoryUsageCounter> mMemoryUsageCounter;

    /**
     * Maps bitmaps to unique indicies to avoid Bitmap duplication.
     */
    AutoPtr<IRemoteViewsBitmapCache> mBitmapCache;

    /**
     * Indicates whether or not this RemoteViews object is contained as a child of any other
     * RemoteViews.
     */
    Boolean mIsRoot;// = true;

    /**
     * Constants to whether or not this RemoteViews is composed of a landscape and portrait
     * RemoteViews.
     */
    static const Int32 MODE_NORMAL = 0;
    static const Int32 MODE_HAS_LANDSCAPE_AND_PORTRAIT = 1;

    /**
     * Used in conjunction with the special constructor
     * {@link #RemoteViews(RemoteViews, RemoteViews)} to keep track of the landscape and portrait
     * RemoteViews.
     */
    AutoPtr<IRemoteViews> mLandscape;
    AutoPtr<IRemoteViews> mPortrait;

    /**
     * This flag indicates whether this RemoteViews object is being created from a
     * RemoteViewsService for use as a child of a widget collection. This flag is used
     * to determine whether or not certain features are available, in particular,
     * setting on click extras and setting on click pending intents. The former is enabled,
     * and the latter disabled when this flag is true.
     */
    Boolean mIsWidgetCollectionChild;

    const static AutoPtr<IRemoteViewsOnClickHandler> DEFAULT_ON_CLICK_HANDLER;

    List< AutoPtr<IContext> > mContextCache;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif
