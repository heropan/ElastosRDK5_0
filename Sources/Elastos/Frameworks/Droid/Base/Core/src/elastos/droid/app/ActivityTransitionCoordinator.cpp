
#include "elastos/droid/app/ActivityTransitionCoordinator.h"

namespace Elastos {
namespace Droid {
namespace App {

// import android.content.Context;
// import android.graphics.Matrix;
// import android.graphics.Rect;
// import android.graphics.RectF;
// import android.os.Bundle;
// import android.os.Handler;
// import android.os.Parcelable;
// import android.os.ResultReceiver;
// import android.transition.Transition;
// import android.transition.TransitionSet;
// import android.util.ArrayMap;
// import android.view.GhostView;
// import android.view.View;
// import android.view.ViewGroup;
// import android.view.ViewGroupOverlay;
// import android.view.ViewParent;
// import android.view.ViewTreeObserver;
// import android.view.Window;
// import android.widget.ImageView;

// import java.util.ArrayList;
// import java.util.Collection;


class ActivityTransitionCoordinator
    : ResultReceiver
    , IActivityTransitionCoordinator
{

    protected:
    class ContinueTransitionListener extends Transition.TransitionListenerAdapter {
        @Override
        void onTransitionStart(Transition transition) {
            mIsStartingTransition = false;
            Runnable pending = mPendingTransition;
            mPendingTransition = null;
            if (pending != null) {
                startTransition(pending);
            }
        }
    };



    private:
     static class FixedEpicenterCallback extends Transition.EpicenterCallback {
        Rect mEpicenter;

        void setEpicenter(Rect epicenter) { mEpicenter = epicenter; }

        @Override
        Rect onGetEpicenter(Transition transition) {
            return mEpicenter;
        }
    };

    static class GhostViewListeners implements ViewTreeObserver.OnPreDrawListener {
        View mView;
        ViewGroup mDecor;
        View mParent;
        Matrix mMatrix = new Matrix();

        GhostViewListeners(View view, View parent, ViewGroup decor) {
            mView = view;
            mParent = parent;
            mDecor = decor;
        }

        View getView() {
            return mView;
        }

        @Override
        boolean onPreDraw() {
            GhostView ghostView = GhostView.getGhost(mView);
            if (ghostView == null) {
                mParent.getViewTreeObserver().removeOnPreDrawListener(this);
            } else {
                GhostView.calculateMatrix(mView, mDecor, mMatrix);
                ghostView.setMatrix(mMatrix);
            }
            return true;
        }
    };

    static class SharedElementOriginalState {
        int mLeft;
        int mTop;
        int mRight;
        int mBottom;
        int mMeasuredWidth;
        int mMeasuredHeight;
        ImageView.ScaleType mScaleType;
        Matrix mMatrix;
        float mTranslationZ;
        float mElevation;
    };


//========================================================================
//

//========================================================================
static const String KEY_SCREEN_LEFT;// = "shared_element:screenLeft";
static const String KEY_SCREEN_TOP;// = "shared_element:screenTop";
static const String KEY_SCREEN_RIGHT;// = "shared_element:screenRight";
static const String KEY_SCREEN_BOTTOM;//= "shared_element:screenBottom";
static const String KEY_TRANSLATION_Z;// = "shared_element:translationZ";
static const String KEY_SNAPSHOT;// = "shared_element:bitmap";
static const String KEY_SCALE_TYPE;// = "shared_element:scaleType";
static const String KEY_IMAGE_MATRIX;// = "shared_element:imageMatrix";
static const String KEY_ELEVATION;// = "shared_element:elevation";

static const String TAG;// = "ActivityTransitionCoordinator";

/**
 * For Activity transitions, the called Activity's listener to receive calls
 * when transitions complete.
 */
static const String KEY_REMOTE_RECEIVER;// = "android:remoteReceiver";

static const AutoPtr<ArrayOf<ImageViewScaleType> > SCALE_TYPE_VALUES;// = ImageView.ScaleType.values();


CAR_INTERFACE_IMPL(ActivityTransitionCoordinator, ResultReceiver, IActivityTransitionCoordinator::)

ActivityTransitionCoordinator::ActivityTransitionCoordinator()
    : mIsReturning(FALSE)
    , mIsStartingTransition(FALSE)
{
    AutoPtr<IArrayList> mAllSharedElementNames; //ArrayList<String>
    AutoPtr<IArrayList> mSharedElements;// = new ArrayList<View>();
    AutoPtr<IArrayList> mSharedElementNames;// = new ArrayList<String>();
    AutoPtr<IArrayList> mTransitioningViews;// = new ArrayList<View>();
    mEpicenterCallback;// = new FixedEpicenterCallback();
}

ActivityTransitionCoordinator::~ActivityTransitionCoordinator()
{}

ECode ActivityTransitionCoordinator::constructor(
    /* [in] */ IWindow* window,
    /* [in] */ IArrayList* allSharedElementNames, //ArrayList<String>
    /* [in] */ ISharedElementCallback* listener,
    /* [in] */ Boolean isReturning)
{
    AutoPtr<IHandler> handler;
    CHandler::New((IHandler**)&handler);
    FAIL_RETURN(ResultReceiver::constructor(handler))
    mWindow = window;
    mListener = listener;
    mAllSharedElementNames = allSharedElementNames;
    mIsReturning = isReturning;
}

ECode ActivityTransitionCoordinator::ViewsReady(
    /* [in] */ IArrayMap* sharedElements) //ArrayMap<String, View>
{
    sharedElements.retainAll(mAllSharedElementNames);
    if (mListener != null) {
        mListener.onMapSharedElements(mAllSharedElementNames, sharedElements);
    }
    mSharedElementNames.addAll(sharedElements.keySet());
    mSharedElements.addAll(sharedElements.values());
    if (getViewsTransition() != null && mTransitioningViews != null) {
        ViewGroup decorView = getDecor();
        if (decorView != null) {
            decorView.captureTransitioningViews(mTransitioningViews);
        }
        mTransitioningViews.removeAll(mSharedElements);
    }
    setEpicenter();
}

ECode ActivityTransitionCoordinator::StripOffscreenViews()
{
    if (mTransitioningViews == null) {
        return;
    }
    Rect r = new Rect();
    for (int i = mTransitioningViews.size() - 1; i >= 0; i--) {
        View view = mTransitioningViews.get(i);
        if (!view.getGlobalVisibleRect(r)) {
            mTransitioningViews.remove(i);
            showView(view, true);
        }
    }
}

ECode ActivityTransitionCoordinator::GetWindow(
    /* [out] */ IWindow** window)
{
    return mWindow;
}

ECode ActivityTransitionCoordinator::GetDecor(
    /* [out] */ IViewGroup** vg)
{
    return (mWindow == null) ? null : (ViewGroup) mWindow.getDecorView();
}

ECode ActivityTransitionCoordinator::SetEpicenter()
{
    View epicenter = null;
    if (!mAllSharedElementNames.isEmpty() && !mSharedElementNames.isEmpty()) {
        int index = mSharedElementNames.indexOf(mAllSharedElementNames.get(0));
        if (index >= 0) {
            epicenter = mSharedElements.get(index);
        }
    }
    setEpicenter(epicenter);
}

ECode ActivityTransitionCoordinator::SetEpicenter(
    /* [in] */ IView* view)
{
    if (view == null) {
        mEpicenterCallback.setEpicenter(null);
    } else {
        Rect epicenter = new Rect();
        view.getBoundsOnScreen(epicenter);
        mEpicenterCallback.setEpicenter(epicenter);
    }
}

ECode ActivityTransitionCoordinator::GetAcceptedNames(
    /* [out] */ IArrayList** list) //ArrayList<String>
{
    return mSharedElementNames;
}

ECode ActivityTransitionCoordinator::GetMappedNames(
    /* [out] */ IArrayList** list) //ArrayList<String>
{
    ArrayList<String> names = new ArrayList<String>(mSharedElements.size());
    for (int i = 0; i < mSharedElements.size(); i++) {
        names.add(mSharedElements.get(i).getTransitionName());
    }
    return names;
}

ECode ActivityTransitionCoordinator::CopyMappedViews(
    /* [out] */ IArrayList** list) //ArrayList<View>
{
    return new ArrayList<View>(mSharedElements);
}

ECode ActivityTransitionCoordinator::GetAllSharedElementNames(
    /* [out] */ IArrayList** list) //ArrayList<String>
{ return mAllSharedElementNames; }

AutoPtr<ITransition> ActivityTransitionCoordinator::SetTargets(
    /* [in] */ ITransition* transition,
    /* [in] */ Boolean add)
{
    if (transition == null || (add &&
            (mTransitioningViews == null || mTransitioningViews.isEmpty()))) {
        return null;
    }
    // Add the targets to a set containing transition so that transition
    // remains unaffected. We don't want to modify the targets of transition itself.
    TransitionSet set = new TransitionSet();
    if (mTransitioningViews != null) {
        for (int i = mTransitioningViews.size() - 1; i >= 0; i--) {
            View view = mTransitioningViews.get(i);
            if (add) {
                set.addTarget(view);
            } else {
                set.excludeTarget(view, true);
            }
        }
    }
    // By adding the transition after addTarget, we prevent addTarget from
    // affecting transition.
    set.addTransition(transition);

    if (!add && mTransitioningViews != null && !mTransitioningViews.isEmpty()) {
        // Allow children of excluded transitioning views, but not the views themselves
        set = new TransitionSet().addTransition(set);
    }

    return set;
}

AutoPtr<ITransition> ActivityTransitionCoordinator::ConfigureTransition(
    /* [in] */ ITransition* transition,
    /* [in] */ Boolean includeTransitioningViews)
{
    if (transition != null) {
        transition = transition.clone();
        transition.setEpicenterCallback(mEpicenterCallback);
        transition = setTargets(transition, includeTransitioningViews);
    }
    return transition;
}

AutoPtr<ITransition> ActivityTransitionCoordinator::MergeTransitions(
    /* [in] */ ITransition* transition1,
    /* [in] */ ITransition* transition2)
{
    if (transition1 == null) {
        return transition2;
    } else if (transition2 == null) {
        return transition1;
    } else {
        TransitionSet transitionSet = new TransitionSet();
        transitionSet.addTransition(transition1);
        transitionSet.addTransition(transition2);
        return transitionSet;
    }
}

AutoPtr<IArrayMap> ActivityTransitionCoordinator::MapSharedElements( //ArrayMap<String, View>
    /* [in] */ IArrayList* accepted, //ArrayList<String>
    /* [in] */ IArrayList* localViews) //ArrayList<View>
{
    ArrayMap<String, View> sharedElements = new ArrayMap<String, View>();
    if (accepted != null) {
        for (int i = 0; i < accepted.size(); i++) {
            sharedElements.put(accepted.get(i), localViews.get(i));
        }
    } else {
        ViewGroup decorView = getDecor();
        if (decorView != null) {
            decorView.findNamedViews(sharedElements);
        }
    }
    return sharedElements;
}

ECode ActivityTransitionCoordinator::SetResultReceiver(
    /* [in] */ IResultReceiver* resultReceiver)
{
    mResultReceiver = resultReceiver;
}

ECode ActivityTransitionCoordinator::SetSharedElementState(
    /* [in] */ IView* view,
    /* [in] */ const String& name,
    /* [in] */ IBundle* transitionArgs,
    /* [in] */ IMatrix* tempMatrix,
    /* [in] */ IRectF* tempRect,
    /* [in] */ ArrayOf<Int32>* decorLoc)
{
    Bundle sharedElementBundle = transitionArgs.getBundle(name);
    if (sharedElementBundle == null) {
        return;
    }

    if (view instanceof ImageView) {
        int scaleTypeInt = sharedElementBundle.getInt(KEY_SCALE_TYPE, -1);
        if (scaleTypeInt >= 0) {
            ImageView imageView = (ImageView) view;
            ImageView.ScaleType scaleType = SCALE_TYPE_VALUES[scaleTypeInt];
            imageView.setScaleType(scaleType);
            if (scaleType == ImageView.ScaleType.MATRIX) {
                float[] matrixValues = sharedElementBundle.getFloatArray(KEY_IMAGE_MATRIX);
                tempMatrix.setValues(matrixValues);
                imageView.setImageMatrix(tempMatrix);
            }
        }
    }

    float z = sharedElementBundle.getFloat(KEY_TRANSLATION_Z);
    view.setTranslationZ(z);
    float elevation = sharedElementBundle.getFloat(KEY_ELEVATION);
    view.setElevation(elevation);

    float left = sharedElementBundle.getFloat(KEY_SCREEN_LEFT);
    float top = sharedElementBundle.getFloat(KEY_SCREEN_TOP);
    float right = sharedElementBundle.getFloat(KEY_SCREEN_RIGHT);
    float bottom = sharedElementBundle.getFloat(KEY_SCREEN_BOTTOM);

    if (decorLoc != null) {
        left -= decorLoc[0];
        top -= decorLoc[1];
        right -= decorLoc[0];
        bottom -= decorLoc[1];
    } else {
        // Find the location in the view's parent
        getSharedElementParentMatrix(view, tempMatrix);
        tempRect.set(left, top, right, bottom);
        tempMatrix.mapRect(tempRect);

        float leftInParent = tempRect.left;
        float topInParent = tempRect.top;

        // Find the size of the view
        view.getInverseMatrix().mapRect(tempRect);
        float width = tempRect.width();
        float height = tempRect.height();

        // Now determine the offset due to view transform:
        view.setLeft(0);
        view.setTop(0);
        view.setRight(Math.round(width));
        view.setBottom(Math.round(height));
        tempRect.set(0, 0, width, height);
        view.getMatrix().mapRect(tempRect);

        ViewGroup parent = (ViewGroup) view.getParent();
        left = leftInParent - tempRect.left + parent.getScrollX();
        top = topInParent - tempRect.top + parent.getScrollY();
        right = left + width;
        bottom = top + height;
    }

    int x = Math.round(left);
    int y = Math.round(top);
    int width = Math.round(right) - x;
    int height = Math.round(bottom) - y;
    int widthSpec = View.MeasureSpec.makeMeasureSpec(width, View.MeasureSpec.EXACTLY);
    int heightSpec = View.MeasureSpec.makeMeasureSpec(height, View.MeasureSpec.EXACTLY);
    view.measure(widthSpec, heightSpec);

    view.layout(x, y, x + width, y + height);
}

ECode ActivityTransitionCoordinator::GetSharedElementParentMatrix(
    /* [in] */ IView* view,
    /* [in] */ IMatrix* matrix)
{
    // Find the location in the view's parent
    ViewGroup parent = (ViewGroup) view.getParent();
    matrix.reset();
    parent.transformMatrixToLocal(matrix);
}

AutoPtr<List<SharedElementOriginalState> > ActivityTransitionCoordinator::SetSharedElementState(
    /* [in] */ IBundle* sharedElementState,
    /* [in] */ IArrayList* snapshots) //ArrayList<View>
{
    ArrayList<SharedElementOriginalState> originalImageState =
            new ArrayList<SharedElementOriginalState>();
    if (sharedElementState != null) {
        Matrix tempMatrix = new Matrix();
        RectF tempRect = new RectF();
        final int numSharedElements = mSharedElements.size();
        for (int i = 0; i < numSharedElements; i++) {
            View sharedElement = mSharedElements.get(i);
            String name = mSharedElementNames.get(i);
            SharedElementOriginalState originalState = getOldSharedElementState(sharedElement,
                    name, sharedElementState);
            originalImageState.add(originalState);
            setSharedElementState(sharedElement, name, sharedElementState,
                    tempMatrix, tempRect, null);
        }
    }
    if (mListener != null) {
        mListener.onSharedElementStart(mSharedElementNames, mSharedElements, snapshots);
    }
    return originalImageState;
}

ECode ActivityTransitionCoordinator::NotifySharedElementEnd(
    /* [in] */ IArrayList* snapshots) //ArrayList<View>
{
    if (mListener != null) {
        mListener.onSharedElementEnd(mSharedElementNames, mSharedElements, snapshots);
    }
}

ECode ActivityTransitionCoordinator::ScheduleSetSharedElementEnd(
    /* [in] */ IArrayList* snapshots) //ArrayList<View>
{
    final View decorView = getDecor();
    if (decorView != null) {
        decorView.getViewTreeObserver().addOnPreDrawListener(
                new ViewTreeObserver.OnPreDrawListener() {
                    @Override
                    boolean onPreDraw() {
                        decorView.getViewTreeObserver().removeOnPreDrawListener(this);
                        notifySharedElementEnd(snapshots);
                        return true;
                    }
                }
        );
    }
}

static AutoPtr<SharedElementOriginalState> ActivityTransitionCoordinator::GetOldSharedElementState(
    /* [in] */ IView* view,
    /* [in] */ const String& name,
    /* [in] */ IBundle* transitionArgs)
{

    SharedElementOriginalState state = new SharedElementOriginalState();
    state.mLeft = view.getLeft();
    state.mTop = view.getTop();
    state.mRight = view.getRight();
    state.mBottom = view.getBottom();
    state.mMeasuredWidth = view.getMeasuredWidth();
    state.mMeasuredHeight = view.getMeasuredHeight();
    state.mTranslationZ = view.getTranslationZ();
    state.mElevation = view.getElevation();
    if (!(view instanceof ImageView)) {
        return state;
    }
    Bundle bundle = transitionArgs.getBundle(name);
    if (bundle == null) {
        return state;
    }
    int scaleTypeInt = bundle.getInt(KEY_SCALE_TYPE, -1);
    if (scaleTypeInt < 0) {
        return state;
    }

    ImageView imageView = (ImageView) view;
    state.mScaleType = imageView.getScaleType();
    if (state.mScaleType == ImageView.ScaleType.MATRIX) {
        state.mMatrix = new Matrix(imageView.getImageMatrix());
    }
    return state;
}

AutoPtr<IArrayList> ActivityTransitionCoordinator::CreateSnapshots( //ArrayList<View>
    /* [in] */ IBundle* state,
    /* [in] */ ICollection* names) //Collection<String>
{
    int numSharedElements = names.size();
    if (numSharedElements == 0) {
        return null;
    }
    ArrayList<View> snapshots = new ArrayList<View>(numSharedElements);
    Context context = getWindow().getContext();
    int[] decorLoc = new int[2];
    ViewGroup decorView = getDecor();
    if (decorView != null) {
        decorView.getLocationOnScreen(decorLoc);
    }
    for (String name: names) {
        Bundle sharedElementBundle = state.getBundle(name);
        if (sharedElementBundle != null) {
            Parcelable parcelable = sharedElementBundle.getParcelable(KEY_SNAPSHOT);
            View snapshot = null;
            if (parcelable != null && mListener != null) {
                snapshot = mListener.onCreateSnapshotView(context, parcelable);
            }
            if (snapshot != null) {
                setSharedElementState(snapshot, name, state, null, null, decorLoc);
            }
            snapshots.add(snapshot);
        }
    }
    return snapshots;
}

static ECode ActivityTransitionCoordinator::SetOriginalSharedElementState(
    /* [in] */ IArrayList* sharedElements, //ArrayList<View>
    /* [in] */ List<SharedElementOriginalState>* originalState)
{
    for (int i = 0; i < originalState.size(); i++) {
        View view = sharedElements.get(i);
        SharedElementOriginalState state = originalState.get(i);
        if (view instanceof ImageView && state.mScaleType != null) {
            ImageView imageView = (ImageView) view;
            imageView.setScaleType(state.mScaleType);
            if (state.mScaleType == ImageView.ScaleType.MATRIX) {
              imageView.setImageMatrix(state.mMatrix);
            }
        }
        view.setElevation(state.mElevation);
        view.setTranslationZ(state.mTranslationZ);
        int widthSpec = View.MeasureSpec.makeMeasureSpec(state.mMeasuredWidth,
                View.MeasureSpec.EXACTLY);
        int heightSpec = View.MeasureSpec.makeMeasureSpec(state.mMeasuredHeight,
                View.MeasureSpec.EXACTLY);
        view.measure(widthSpec, heightSpec);
        view.layout(state.mLeft, state.mTop, state.mRight, state.mBottom);
    }
}

AutoPtr<IBundle> ActivityTransitionCoordinator::CaptureSharedElementState()
{
    Bundle bundle = new Bundle();
    RectF tempBounds = new RectF();
    Matrix tempMatrix = new Matrix();
    for (int i = 0; i < mSharedElements.size(); i++) {
        View sharedElement = mSharedElements.get(i);
        String name = mSharedElementNames.get(i);
        captureSharedElementState(sharedElement, name, bundle, tempMatrix, tempBounds);
    }
    return bundle;
}

ECode ActivityTransitionCoordinator::ClearState() {
    // Clear the state so that we can't hold any references accidentally and leak memory.
    mWindow = null;
    mSharedElements.clear();
    mTransitioningViews = null;
    mOriginalAlphas.clear();
    mResultReceiver = null;
    mPendingTransition = null;
    mListener = null;
}

Int64 ActivityTransitionCoordinator::GetFadeDuration() {
    return getWindow().getTransitionBackgroundFadeDuration();
}

ECode ActivityTransitionCoordinator::HideViews(
    /* [in] */ IArrayList* views) //ArrayList<View>
{
    int count = views.size();
    for (int i = 0; i < count; i++) {
        View view = views.get(i);
        if (!mOriginalAlphas.containsKey(view)) {
            mOriginalAlphas.put(view, view.getAlpha());
        }
        view.setAlpha(0f);
    }
}

ECode ActivityTransitionCoordinator::ShowViews(
    /* [in] */ IArrayList* views, //ArrayList<View>
    /* [in] */ Boolean setTransitionAlpha)
{
    int count = views.size();
    for (int i = 0; i < count; i++) {
        showView(views.get(i), setTransitionAlpha);
    }
}

ECode ActivityTransitionCoordinator::ShowView(
    /* [in] */ IView* view,
    /* [in] */ Boolean setTransitionAlpha)
{
    Float alpha = mOriginalAlphas.remove(view);
    if (alpha != null) {
        view.setAlpha(alpha);
    }
    if (setTransitionAlpha) {
        view.setTransitionAlpha(1f);
    }
}

ECode ActivityTransitionCoordinator::CaptureSharedElementState(
    /* [in] */ IView* view,
    /* [in] */ const String& name,
    /* [in] */ IBundle* transitionArgs,
    /* [in] */ IMatrix* tempMatrix,
    /* [in] */ IRectF* tempBounds)
{
    Bundle sharedElementBundle = new Bundle();
    tempMatrix.reset();
    view.transformMatrixToGlobal(tempMatrix);
    tempBounds.set(0, 0, view.getWidth(), view.getHeight());
    tempMatrix.mapRect(tempBounds);

    sharedElementBundle.putFloat(KEY_SCREEN_LEFT, tempBounds.left);
    sharedElementBundle.putFloat(KEY_SCREEN_RIGHT, tempBounds.right);
    sharedElementBundle.putFloat(KEY_SCREEN_TOP, tempBounds.top);
    sharedElementBundle.putFloat(KEY_SCREEN_BOTTOM, tempBounds.bottom);
    sharedElementBundle.putFloat(KEY_TRANSLATION_Z, view.getTranslationZ());
    sharedElementBundle.putFloat(KEY_ELEVATION, view.getElevation());

    Parcelable bitmap = null;
    if (mListener != null) {
        bitmap = mListener.onCaptureSharedElementSnapshot(view, tempMatrix, tempBounds);
    }

    if (bitmap != null) {
        sharedElementBundle.putParcelable(KEY_SNAPSHOT, bitmap);
    }

    if (view instanceof ImageView) {
        ImageView imageView = (ImageView) view;
        int scaleTypeInt = scaleTypeToInt(imageView.getScaleType());
        sharedElementBundle.putInt(KEY_SCALE_TYPE, scaleTypeInt);
        if (imageView.getScaleType() == ImageView.ScaleType.MATRIX) {
            float[] matrix = new float[9];
            imageView.getImageMatrix().getValues(matrix);
            sharedElementBundle.putFloatArray(KEY_IMAGE_MATRIX, matrix);
        }
    }

    transitionArgs.putBundle(name, sharedElementBundle);
}

ECode ActivityTransitionCoordinator::StartTransition(
    /* [in] */ IRunnable* runnable)
{
    if (mIsStartingTransition) {
        mPendingTransition = runnable;
    } else {
        mIsStartingTransition = true;
        runnable.run();
    }
}

ECode ActivityTransitionCoordinator::TransitionStarted()
{
    mIsStartingTransition = false;
}

ECode ActivityTransitionCoordinator::MoveSharedElementsToOverlay()
{
    if (!mWindow.getSharedElementsUseOverlay()) {
        return;
    }
    int numSharedElements = mSharedElements.size();
    ViewGroup decor = getDecor();
    if (decor != null) {
        boolean moveWithParent = moveSharedElementWithParent();
        for (int i = 0; i < numSharedElements; i++) {
            View view = mSharedElements.get(i);
            GhostView.addGhost(view, decor);
            ViewGroup parent = (ViewGroup) view.getParent();
            if (moveWithParent && !isInTransitionGroup(parent, decor)) {
                GhostViewListeners listener = new GhostViewListeners(view, parent, decor);
                parent.getViewTreeObserver().addOnPreDrawListener(listener);
                mGhostViewListeners.add(listener);
            }
        }
    }
}

Boolean ActivityTransitionCoordinator::MoveSharedElementWithParent()
{
    return true;
}

static Boolean ActivityTransitionCoordinator::IsInTransitionGroup(
    /* [in] */ IViewParent* viewParent,
    /* [in] */ IViewGroup* decor)
{
    if (viewParent == decor || !(viewParent instanceof ViewGroup)) {
        return false;
    }
    ViewGroup parent = (ViewGroup) viewParent;
    if (parent.isTransitionGroup()) {
        return true;
    } else {
        return isInTransitionGroup(parent.getParent(), decor);
    }
}

ECode ActivityTransitionCoordinator::MoveSharedElementsFromOverlay()
{
    int numListeners = mGhostViewListeners.size();
    for (int i = 0; i < numListeners; i++) {
        GhostViewListeners listener = mGhostViewListeners.get(i);
        ViewGroup parent = (ViewGroup) listener.getView().getParent();
        parent.getViewTreeObserver().removeOnPreDrawListener(listener);
    }
    mGhostViewListeners.clear();

    if (mWindow == null || !mWindow.getSharedElementsUseOverlay()) {
        return;
    }
    ViewGroup decor = getDecor();
    if (decor != null) {
        ViewGroupOverlay overlay = decor.getOverlay();
        int count = mSharedElements.size();
        for (int i = 0; i < count; i++) {
            View sharedElement = mSharedElements.get(i);
            GhostView.removeGhost(sharedElement);
        }
    }
}

ECode ActivityTransitionCoordinator::SetGhostVisibility(
    /* [in] */ Int32 visibility)
{
    int numSharedElements = mSharedElements.size();
    for (int i = 0; i < numSharedElements; i++) {
        GhostView ghostView = GhostView.getGhost(mSharedElements.get(i));
        if (ghostView != null) {
            ghostView.setVisibility(visibility);
        }
    }
}

ECode ActivityTransitionCoordinator::ScheduleGhostVisibilityChange(
    /* [in] */ Int32 visibility)
{
    final View decorView = getDecor();
    if (decorView != null) {
        decorView.getViewTreeObserver()
                .addOnPreDrawListener(new ViewTreeObserver.OnPreDrawListener() {
                    @Override
                    boolean onPreDraw() {
                        decorView.getViewTreeObserver().removeOnPreDrawListener(this);
                        setGhostVisibility(visibility);
                        return true;
                    }
                });
    }
}

static Int32 ActivityTransitionCoordinator::ScaleTypeToInt(
    /* [in] */ ImageViewScaleType scaleType)
{
    for (int i = 0; i < SCALE_TYPE_VALUES.length; i++) {
        if (scaleType == SCALE_TYPE_VALUES[i]) {
            return i;
        }
    }
    return -1;
}


} // namespace App
} // namespace Droid
} // namespace Elastos
