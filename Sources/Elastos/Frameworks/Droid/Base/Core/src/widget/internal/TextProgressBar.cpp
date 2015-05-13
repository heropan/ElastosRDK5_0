
#include "internal/TextProgressBar.h"
#include "os/SystemClock.h"

using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::View::IViewGroupLayoutParams;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

const Int32 TextProgressBar::PROGRESSBAR_ID;
const Int32 TextProgressBar::CHRONOMETER_ID;
const String TextProgressBar::TAG("TextProgressBar");

TextProgressBar::TextProgressBar()
   : mDurationBase(-1)
   , mDuration(-1)
   , mChronometerFollow(FALSE)
   , mChronometerGravity(IGravity::NO_GRAVITY)
{}

TextProgressBar::TextProgressBar(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
   : RelativeLayout(context, attrs, defStyle)
   , mDurationBase(-1)
   , mDuration(-1)
   , mChronometerFollow(FALSE)
   , mChronometerGravity(IGravity::NO_GRAVITY)
{}

TextProgressBar::TextProgressBar(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
   : RelativeLayout(context, attrs)
   , mDurationBase(-1)
   , mDuration(-1)
   , mChronometerFollow(FALSE)
   , mChronometerGravity(IGravity::NO_GRAVITY)
{}

TextProgressBar::TextProgressBar(
    /* [in] */ IContext* context)
   : RelativeLayout(context)
   , mDurationBase(-1)
   , mDuration(-1)
   , mChronometerFollow(FALSE)
   , mChronometerGravity(IGravity::NO_GRAVITY)
{}

ECode TextProgressBar:: Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(RelativeLayout::Init(context, attrs, defStyle));
    return NOERROR;
}

ECode TextProgressBar:: Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    ASSERT_SUCCEEDED(RelativeLayout::Init(context, attrs));
    return NOERROR;
}

ECode TextProgressBar:: Init(
    /* [in] */ IContext* context)
{
    ASSERT_SUCCEEDED(RelativeLayout::Init(context));
    return NOERROR;
}

/**
 * Catch any interesting children when they are added.
 */
ECode TextProgressBar::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    RelativeLayout::AddView(child, index, params);

    Int32 childId;
    child->GetId(&childId);

    if (childId == CHRONOMETER_ID && child->Probe(EIID_IChronometer)) {
        mChronometer = (IChronometer*) child->Probe(EIID_IChronometer);
        mChronometer->SetOnChronometerTickListener(THIS_PROBE(IOnChronometerTickListener));

        // Check if Chronometer should move with with ProgressBar
        Int32 width;
        params->GetWidth(&width);
        mChronometerFollow = (width== IViewGroupLayoutParams::WRAP_CONTENT);

        Int32 gravity;
        mChronometer->GetGravity(&gravity);
        mChronometerGravity = (gravity &
                IGravity::RELATIVE_HORIZONTAL_GRAVITY_MASK);

    } else if (childId == PROGRESSBAR_ID && child->Probe(EIID_IProgressBar)) {
        mProgressBar = (IProgressBar*) child->Probe(EIID_IProgressBar);
    }

    return NOERROR;
}

/**
 * Set the expected termination time of the running {@link Chronometer}.
 * This value is used to adjust the {@link ProgressBar} against the elapsed
 * time.
 * <p>
 * Call this <b>after</b> adjusting the {@link Chronometer} base, if
 * necessary.
 *
 * @param durationBase Use the {@link SystemClock#elapsedRealtime} time
 *            base.
 */
ECode TextProgressBar::SetDurationBase(
    /* [in] */ Int64 durationBase)
{
    mDurationBase = durationBase;

    if (mProgressBar == NULL || mChronometer == NULL) {
        return E_RUNTIME_EXCEPTION;
    }

    // Update the ProgressBar maximum relative to Chronometer base

    Int64 base;
    mChronometer->GetBase(&base);
    mDuration = (Int32) (durationBase - base);
    if (mDuration <= 0) {
        mDuration = 1;
    }
    mProgressBar->SetMax(mDuration);

    return NOERROR;
}

/**
 * Callback when {@link Chronometer} changes, indicating that we should
 * update the {@link ProgressBar} and change the layout if necessary.
 */
ECode TextProgressBar::OnChronometerTick(
    /* [in] */ IChronometer* chronometer)
{
    if (mProgressBar == NULL) {
        return E_RUNTIME_EXCEPTION;
    }

    // Stop Chronometer if we're past duration
    Int64 now = SystemClock::GetElapsedRealtime();
    if (now >= mDurationBase) {
        mChronometer->Stop();
    }

    // Update the ProgressBar status
    Int32 remaining = (Int32) (mDurationBase - now);
    mProgressBar->SetProgress(mDuration - remaining);

    // Move the Chronometer if gravity is set correctly
    if (mChronometerFollow) {
        AutoPtr<IRelativeLayoutLayoutParams> params;

        // Calculate estimate of ProgressBar leading edge position
        mProgressBar->GetLayoutParams((IViewGroupLayoutParams**)&params);

        Int32 contentWidth;
        mProgressBar->GetWidth(&contentWidth);
        Int32 leftMargin, rightMargin;
        params->GetLeftMargin(&leftMargin);
        params->GetRightMargin(&rightMargin);
        contentWidth -= leftMargin + rightMargin;

        Int32 progress, max;

        mProgressBar->GetProgress(&progress);

        mProgressBar->GetMax(&max);

        Int32 leadingEdge = ((contentWidth * progress) /
                max) + leftMargin;

        // Calculate any adjustment based on gravity
        Int32 adjustLeft = 0;
        Int32 textWidth;
        mChronometer->GetWidth(&textWidth);
        if (mChronometerGravity == IGravity::END) {
            adjustLeft = -textWidth;
        } else if (mChronometerGravity == IGravity::CENTER_HORIZONTAL) {
            adjustLeft = -(textWidth / 2);
        }

        // Limit margin to keep text inside ProgressBar bounds
        leadingEdge += adjustLeft;
        Int32 rightLimit = contentWidth - rightMargin - textWidth;
        if (leadingEdge < leftMargin) {
            leadingEdge = leftMargin;
        } else if (leadingEdge > rightLimit) {
            leadingEdge = rightLimit;
        }
        params = NULL;
        mChronometer->GetLayoutParams((IViewGroupLayoutParams**)&params);
        params->SetLeftMargin(leadingEdge);

        // Request layout to move Chronometer
        mChronometer->RequestLayout();

    }
    return NOERROR;
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
