
#include "widget/Chronometer.h"
#include "os/SystemClock.h"
#include "text/format/DateUtils.h"

using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Text::Format::DateUtils;
using Elastos::Core::CStringWrapper;

namespace Elastos {
namespace Droid {
namespace Widget {

const String Chronometer::TAG = String("Chronometer");
const Int32 Chronometer::TICK_WHAT;

ECode Chronometer::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    if (mHost->mRunning) {
        mHost->UpdateText(SystemClock::GetElapsedRealtime());
        mHost->DispatchChronometerTick();

        Boolean result;
        SendEmptyMessageDelayed(Chronometer::TICK_WHAT, 1000, &result);
    }

    return NOERROR;
}

Chronometer::Chronometer()
   : mBase(0)
   , mVisible(FALSE)
   , mStarted(FALSE)
   , mRunning(FALSE)
   , mLogged(FALSE)
   , mRecycle(8)
{
    mFormatterArgs = ArrayOf<IInterface*>::Alloc(1);
    mHandler = new MyHandler(this);
}

Chronometer::~Chronometer()
{
}

Chronometer::Chronometer(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : TextView(context, attrs, defStyle)
    , mBase(0)
    , mVisible(FALSE)
    , mStarted(FALSE)
    , mRunning(FALSE)
    , mLogged(FALSE)
    , mRecycle(8)
{
    mFormatterArgs = ArrayOf<IInterface*>::Alloc(1);
    mHandler = new MyHandler(this);
    ASSERT_SUCCEEDED(InitInternal(context, attrs, defStyle));
}

ECode Chronometer::InitInternal(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::Chronometer),
            ARRAY_SIZE(R::styleable::Chronometer));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(
            attrs, attrIds, defStyle, 0, (ITypedArray**)&a);

    String str;
    a->GetString(R::styleable::Chronometer_format, &str);
    SetFormat(str);
    a->Recycle();

    Init();
    return NOERROR;
}


ECode Chronometer::Init(
    /* [in] */ IContext* context)
{
    return Init(context, NULL, 0);
}

ECode Chronometer::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(context, attrs, 0);
}

ECode Chronometer::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(TextView::Init(context, attrs, defStyle));
    ASSERT_SUCCEEDED(InitInternal(context, attrs, defStyle));
    return NOERROR;
}

void Chronometer::Init()
{
    mBase = SystemClock::GetElapsedRealtime();
    UpdateText(mBase);
}

ECode Chronometer::SetBase(Int64 base)
{
    mBase = base;
    DispatchChronometerTick();
    UpdateText(SystemClock::GetElapsedRealtime());

    return NOERROR;
}

Int64 Chronometer::GetBase()
{
    return mBase;
}

ECode Chronometer::SetFormat(
    /* [in] */ const String& format)
{
    mFormat = format;
    if (!format.IsNull() && mFormatBuilder == NULL) {
        mFormatBuilder = new StringBuilder(format.GetByteLength() * 2);
    }

    return NOERROR;
}

String Chronometer::GetFormat()
{
    return mFormat;
}

ECode Chronometer::SetOnChronometerTickListener(
    /* [in] */ IOnChronometerTickListener* listener)
{
    mOnChronometerTickListener = listener;

    return NOERROR;
}

AutoPtr<IOnChronometerTickListener> Chronometer::GetOnChronometerTickListener()
{
    return mOnChronometerTickListener;
}

ECode Chronometer::Start()
{
    mStarted = TRUE;
    UpdateRunning();

    return NOERROR;
}

ECode Chronometer::Stop()
{
    mStarted = FALSE;
    UpdateRunning();

    return NOERROR;
}

ECode Chronometer::SetStarted(
    /* [in] */ Boolean started)
{
    mStarted = started;
    UpdateRunning();

    return NOERROR;
}

ECode Chronometer::OnDetachedFromWindow()
{
    TextView::OnDetachedFromWindow();
    mVisible = FALSE;
    UpdateRunning();
    return NOERROR;
}

void Chronometer::OnWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    TextView::OnWindowVisibilityChanged(visibility);
    mVisible = visibility == IView::VISIBLE;
    UpdateRunning();
}

void Chronometer::UpdateText(
    /* [in] */ Int64 now)
{
    Int64 seconds = now - mBase;
    seconds /= 1000;
    String sRecycle;
    mRecycle.ToString(&sRecycle);
    String text = DateUtils::FormatElapsedTime(sRecycle, seconds);

    //if (mFormat != NULL) {
        //AutoPtr<ILocale> loc;// = Locale.getDefault();
        //Boolean res;
        //if (mFormatter == NULL || (!loc->Equals(mFormatterLocale, &res), res)) {
        //    mFormatterLocale = loc;
        //    //mFormatter = new Formatter(mFormatBuilder, loc);
        //}
        //mFormatBuilder.setLength(0);
        //mFormatterArgs[0] = text;
        //try {
            //mFormatter.format(mFormat, mFormatterArgs);
            //text = mFormatBuilder.toString();
        /*} catch (IllegalFormatException ex) {
            if (!mLogged) {
                Log.w(TAG, "Illegal format string: " + mFormat);
                mLogged = TRUE;
            }
        }*/
    //}
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(text, (ICharSequence**)&seq);
    SetText(seq);
}

void Chronometer::UpdateRunning()
{
    Boolean running = mVisible && mStarted;
    if (running != mRunning) {
        if (running) {
            UpdateText(SystemClock::GetElapsedRealtime());
            DispatchChronometerTick();
            Boolean result;
            mHandler->SendEmptyMessageDelayed(TICK_WHAT, 1000, &result);
        }
        else {
            mHandler->RemoveMessages(TICK_WHAT);
        }
        mRunning = running;
    }
}

void Chronometer::DispatchChronometerTick()
{
    if (mOnChronometerTickListener != NULL) {
        mOnChronometerTickListener->OnChronometerTick((IChronometer*)this->Probe(EIID_IChronometer));
    }
}

ECode Chronometer::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    TextView::OnInitializeAccessibilityEvent(event);
    String classNameStr("Chronometer");
    AutoPtr<ICharSequence> className;
    FAIL_RETURN(CStringWrapper::New(classNameStr, (ICharSequence**)&className));
    event->SetClassName(className);
    return NOERROR;
}

ECode Chronometer::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    TextView::OnInitializeAccessibilityNodeInfo(info);
    String classNameStr("Chronometer");
    AutoPtr<ICharSequence> className;
    FAIL_RETURN(CStringWrapper::New(classNameStr, (ICharSequence**)&className));
    info->SetClassName(className);
    return NOERROR;
}

}// namespace Elastos
}// namespace Droid
}// namespace Widget
