
#include "CPinyinIME.h"
#include "KeyMapDream.h"
#include "CInputModeSwitcher.h"
#include <elastos/core/Math.h>
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/logging/Logger.h>
#include "R.h"
#include <R.h>

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Core::Math;
using Elastos::Core::EIID_IRunnable;
using Elastos::Core::CStringWrapper;
using Elastos::Core::IStringBuffer;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::App::CAlertDialogBuilder;
using Elastos::Droid::App::IAlertDialogBuilder;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnDismissListener;
using Elastos::Droid::Content::EIID_IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::ISharedPreferences;
using Elastos::Droid::Content::EIID_IServiceConnection;
using Elastos::Droid::Inputmethods::PinyinIME::EIID_ICandidateViewListener;
using Elastos::Droid::Inputmethods::PinyinIME::ISettings;
using Elastos::Droid::Inputmethods::PinyinIME::CPinyinSettings;
using Elastos::Droid::Inputmethods::PinyinIME::IPinyinEnvironmentHelper;
using Elastos::Droid::Inputmethods::PinyinIME::CPinyinEnvironmentHelper;
using Elastos::Droid::Inputmethods::PinyinIME::CDecodingInfo;
using Elastos::Droid::Inputmethods::PinyinIME::CBalloonHint;
using Elastos::Droid::Inputmethods::PinyinIME::CPinyinSettings;
using Elastos::Droid::Inputmethods::PinyinIME::ISettings;
using Elastos::Droid::Inputmethods::PinyinIME::IPinyinDecoderService;
using Elastos::Droid::Inputmethods::PinyinIME::CPinyinGestureListener;
using Elastos::Droid::Inputmethods::PinyinIME::EIID_IPinyinIME;
using Elastos::Droid::InputMethodService::IInputMethodService;
using Elastos::Droid::InputMethodService::EIID_IInputMethodService;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Preference::IPreferenceManagerHelper;
using Elastos::Droid::Preference::CPreferenceManagerHelper;
using Elastos::Droid::View::CGestureDetector;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::CKeyEvent;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::Widget::CPopupWindow;

namespace Elastos {
namespace Droid {
namespace DevSamples {
namespace PinyinIME {

CPinyinIME::PopupTimer::PopupTimer(
    /* [in] */ IPinyinIME* host)
{
    AutoPtr<IWeakReferenceSource> source = (IWeakReferenceSource*)host->Probe(EIID_IWeakReferenceSource);
    assert(source != NULL);
    source->GetWeakReference((IWeakReference**)&mWeak);
    mParentLocation[0] = mParentLocation[1] = 0;
}

void CPinyinIME::PopupTimer::PostShowFloatingWindow()
{
    AutoPtr<IPinyinIME> host;
    mWeak->Resolve(EIID_IPinyinIME, (IInterface**)&host);
    if (host == NULL) {
        return;
    }
    AutoPtr<CPinyinIME> mHost = (CPinyinIME*)host.Get();

    mHost->mFloatingContainer->Measure(IViewGroupLayoutParams::WRAP_CONTENT,
            IViewGroupLayoutParams::WRAP_CONTENT);

    Int32 mW = 0, mH = 0;
    mHost->mFloatingContainer->GetMeasuredWidth(&mW);
    mHost->mFloatingContainer->GetMeasuredHeight(&mH);
    mHost->mFloatingWindow->SetWidth(mW);
    mHost->mFloatingWindow->SetHeight(mH);

    Boolean result = FALSE;
    Post(this, &result);
}

void CPinyinIME::PopupTimer::CancelShowing()
{
    AutoPtr<IPinyinIME> host;
    mWeak->Resolve(EIID_IPinyinIME, (IInterface**)&host);
    if (host == NULL) {
        return;
    }
    AutoPtr<CPinyinIME> mHost = (CPinyinIME*)host.Get();
    Boolean showing = FALSE;
    if (mHost->mFloatingWindow->IsShowing(&showing), showing) {
        mHost->mFloatingWindow->Dismiss();
    }

    RemoveCallbacks(this);
}

ECode CPinyinIME::PopupTimer::Run()
{
    AutoPtr<IPinyinIME> host;
    mWeak->Resolve(EIID_IPinyinIME, (IInterface**)&host);
    if (host == NULL) {
        return NOERROR;
    }
    AutoPtr<CPinyinIME> mHost = (CPinyinIME*)host.Get();

    Int32 x = 0, y = 0;
    mHost->mCandidatesContainer->GetLocationInWindow(&x, &y);
    mParentLocation[0] = x;
    mParentLocation[1] = y;

    Int32 height = 0;
    mHost->mFloatingWindow->GetHeight(&height);
    Boolean showing = FALSE;
    mHost->mFloatingWindow->IsShowing(&showing);
    if (!showing) {
        mHost->mFloatingWindow->ShowAtLocation(mHost->mCandidatesContainer,
                IGravity::LEFT | IGravity::TOP, mParentLocation[0],
                mParentLocation[1] - height);
    } else {
        Int32 width = 0;
        mHost->mFloatingWindow->GetWidth(&width);
        mHost->mFloatingWindow->Update(mParentLocation[0],
                mParentLocation[1] - height,
                width,
                height);
    }
    return NOERROR;
}

CAR_INTERFACE_IMPL_2(CPinyinIME::BuilderListener, IDialogInterfaceOnClickListener, IDialogInterfaceOnDismissListener);

CPinyinIME::BuilderListener::BuilderListener(
    /* [in] */ CPinyinIME* ime)
    : mHost(ime)
{
}

ECode CPinyinIME::BuilderListener::OnClick(
    /* [in] */ IDialogInterface* di,
    /* [in] */ Int32 position)
{
    assert(di != NULL);

    Logger::D("CPinyinIME", "点击了按钮: %d", position);
    switch (position) {
        case IDialogInterface::BUTTON_POSITIVE:
            mHost->LaunchSettings();
            break;

        case IDialogInterface::BUTTON_NEUTRAL:
        case IDialogInterface::BUTTON_NEGATIVE:
            assert(0 && "TODO");
            // InputMethodManager.getInstance(PinyinIME.this)
            //         .showInputMethodPicker();
            break;
    }

    // di->Dismiss();
    return NOERROR;
}

ECode CPinyinIME::BuilderListener::OnDismiss(
    /* [in] */ IDialogInterface* dialog)
{
    mHost->mOptionsDialog = NULL;
    return NOERROR;
}

CAR_INTERFACE_IMPL(CPinyinIME::ChoiceNotifier, ICandidateViewListener);
CPinyinIME::ChoiceNotifier::ChoiceNotifier(
    /* [in] */ CPinyinIME* ime)
    : mIme(ime)
{
}

ECode CPinyinIME::ChoiceNotifier::OnClickChoice(
    /* [in] */ Int32 choiceId)
{
    if (choiceId >= 0) {
        mIme->OnChoiceTouched(choiceId);
    }
    return NOERROR;
}

ECode CPinyinIME::ChoiceNotifier::OnToLeftGesture()
{
    if (Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_COMPOSING == mIme->mImeState) {
        mIme->ChangeToStateInput(TRUE);
    }

    Boolean result = FALSE;
    return mIme->mCandidatesContainer->PageForward(TRUE, FALSE, &result);
}

ECode CPinyinIME::ChoiceNotifier::OnToRightGesture()
{
    if (Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_COMPOSING == mIme->mImeState) {
        mIme->ChangeToStateInput(TRUE);
    }
    Boolean result = FALSE;
    return mIme->mCandidatesContainer->PageBackward(TRUE, FALSE, &result);
}

ECode CPinyinIME::ChoiceNotifier::OnToTopGesture()
{
    return NOERROR;
}

ECode CPinyinIME::ChoiceNotifier::OnToBottomGesture()
{
    return NOERROR;
}


//class PinyinDecoderServiceConnection
CAR_INTERFACE_IMPL(CPinyinIME::PinyinDecoderServiceConnection, IServiceConnection);
CPinyinIME::PinyinDecoderServiceConnection::PinyinDecoderServiceConnection(
    /* [in] */ CPinyinIME* ime)
    : mIme(ime)
{}

CPinyinIME::PinyinDecoderServiceConnection::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    return mIme->mDecInfo->SetPinyinDecoderService(IPinyinDecoderService::Probe(service));
}

CPinyinIME::PinyinDecoderServiceConnection::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    return NOERROR;
}


//class CPinyinIME
String CPinyinIME::TAG("PinyinIME");
const Boolean CPinyinIME::SIMULATE_KEY_DELETE = TRUE;
CPinyinIME::CPinyinIME()
    : mImeState(Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_IDLE)
{
}

CPinyinIME::~CPinyinIME()
{
}

ECode CPinyinIME::constructor()
{
    mFloatingWindowTimer = new PopupTimer(this);
    CDecodingInfo::New(this, (IDecodingInfo**)&mDecInfo);
    return NOERROR;
}

PInterface CPinyinIME::Probe(
    /* [in] */ REIID riid)
{
    return _CPinyinIME::Probe(riid);
}

UInt32 CPinyinIME::AddRef()
{
    return _CPinyinIME::AddRef();
}

UInt32 CPinyinIME::Release()
{
    return _CPinyinIME::Release();
}

ECode CPinyinIME::OnCreate()
{
    AutoPtr<IPinyinEnvironmentHelper> helper;
    CPinyinEnvironmentHelper::AcquireSingleton((IPinyinEnvironmentHelper**)&helper);
    helper->GetInstance((IPinyinEnvironment**)&mEnvironment);

    // if (mEnvironment.needDebug()) {
    //     Log.d(TAG, "onCreate.");
    // }
    InputMethodService::OnCreate();

    StartPinyinDecoderService();
    mImEn = new EnglishInputProcessor();

    AutoPtr<IContext> ctx;
    InputMethodService::GetApplicationContext((IContext**)&ctx);

    AutoPtr<IPreferenceManagerHelper> pmHelper;
    CPreferenceManagerHelper::AcquireSingleton((IPreferenceManagerHelper**)&pmHelper);
    AutoPtr<ISharedPreferences> sp;
    pmHelper->GetDefaultSharedPreferences(ctx, (ISharedPreferences**)&sp);

    AutoPtr<ISettings> settings;
    CPinyinSettings::AcquireSingleton((ISettings**)&settings);
    settings->GetInstance(sp);

    CInputModeSwitcher::New(this, (IInputModeSwitcher**)&mInputModeSwitcher);
    mChoiceNotifier = new ChoiceNotifier(this);

    CPinyinGestureListener::New(FALSE, this, (IGestureDetectorOnGestureListener**)&mGestureListenerSkb);
    CPinyinGestureListener::New(TRUE, this, (IGestureDetectorOnGestureListener**)&mGestureListenerCandidates);
    CGestureDetector::New(this, mGestureListenerSkb, (IGestureDetector**)&mGestureDetectorSkb);
    CGestureDetector::New(this, mGestureListenerCandidates, (IGestureDetector**)&mGestureDetectorCandidates);

    AutoPtr<IResources> res;
    InputMethodService::GetResources((IResources**)&res);
    assert(res != NULL);
    AutoPtr<IConfiguration> config;
    res->GetConfiguration((IConfiguration**)&config);
    return mEnvironment->OnConfigurationChanged(config, this);
}

//add by kinier for fix the candidate word can not display bug
ECode CPinyinIME::OnEvaluateFullscreenMode(
    /* [out] */ Boolean* screenMode)
{
    VALIDATE_NOT_NULL(screenMode);
    *screenMode = FALSE;
    return NOERROR;
}

ECode CPinyinIME::OnDestroy()
{
    // if (mEnvironment.needDebug()) {
    //     Log.d(TAG, "onDestroy.");
    // }
    InputMethodService::UnbindService(mPinyinDecoderServiceConnection);
    AutoPtr<ISettings> settings;
    CPinyinSettings::AcquireSingleton((ISettings**)&settings);
    settings->ReleaseInstance();
    return InputMethodService::OnDestroy();
}

ECode CPinyinIME::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    AutoPtr<IPinyinEnvironmentHelper> helper;
    CPinyinEnvironmentHelper::AcquireSingleton((IPinyinEnvironmentHelper**)&helper);
    AutoPtr<IPinyinEnvironment> env;
    helper->GetInstance((IPinyinEnvironment**)&env);
    // if (mEnvironment.needDebug()) {
    //     Log.d(TAG, "onConfigurationChanged");
    //     Log.d(TAG, "--last config: " + env.getConfiguration().toString());
    //     Log.d(TAG, "---new config: " + newConfig.toString());
    // }
    // We need to change the local environment first so that UI components
    // can get the environment instance to handle size issues. When
    // super.onConfigurationChanged() is called, onCreateCandidatesView()
    // and onCreateInputView() will be executed if necessary.
    env->OnConfigurationChanged(newConfig, this);

    // Clear related UI of the previous configuration.
    if (NULL != mSkbContainer) {
        mSkbContainer->DismissPopups();
    }
    if (NULL != mCandidatesBalloon) {
        mCandidatesBalloon->Dismiss();
    }

    InputMethodService::OnConfigurationChanged(newConfig);
    ResetToIdleState(FALSE);
    return NOERROR;
}

ECode CPinyinIME::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 repeat = 0;
    event->GetRepeatCount(&repeat);
    if (ProcessKey(event, 0 != repeat)) {
        *result = TRUE;
        return NOERROR;
    }

    return InputMethodService::OnKeyDown(keyCode, event, result);
}

ECode CPinyinIME::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (ProcessKey(event, TRUE)) {
        *result = TRUE;
        return NOERROR;
    }

    return InputMethodService::OnKeyUp(keyCode, event, result);
}

Boolean CPinyinIME::ProcessKey(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Boolean realAction)
{
    if (Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_BYPASS == mImeState) {
        return FALSE;
    }

    Int32 keyCode = 0;
    event->GetKeyCode(&keyCode);
    // SHIFT-SPACE is used to switch between Chinese and English
    // when HKB is on.
    Boolean pressed = FALSE;
    if (IKeyEvent::KEYCODE_SPACE == keyCode && (event->IsShiftPressed(&pressed), pressed)) {
        if (!realAction) {
            return TRUE;
        }

        Int32 icon = 0;
        mInputModeSwitcher->SwitchLanguageWithHkb(&icon);
        UpdateIcon(icon);
        ResetToIdleState(FALSE);

        Int32 allMetaState = IKeyEvent::META_ALT_ON | IKeyEvent::META_ALT_LEFT_ON
                | IKeyEvent::META_ALT_RIGHT_ON | IKeyEvent::META_SHIFT_ON
                | IKeyEvent::META_SHIFT_LEFT_ON
                | IKeyEvent::META_SHIFT_RIGHT_ON | IKeyEvent::META_SYM_ON;

        AutoPtr<IInputConnection> connection;
        InputMethodService::GetCurrentInputConnection((IInputConnection**)&connection);
        assert(connection != NULL);
        Boolean result = FALSE;
        connection->ClearMetaKeyStates(allMetaState, &result);
        return TRUE;
    }

    // If HKB is on to input English, by-pass the key event so that
    // default key listener will handle it.
    Boolean res = FALSE;
    if (mInputModeSwitcher->IsEnglishWithHkb(&res), res) {
        return FALSE;
    }

    if (ProcessFunctionKeys(keyCode, realAction)) {
        return TRUE;
    }

    Int32 keyChar = 0;
    if (keyCode >= IKeyEvent::KEYCODE_A && keyCode <= IKeyEvent::KEYCODE_Z) {
        keyChar = keyCode - IKeyEvent::KEYCODE_A + 'a';
    } else if (keyCode >= IKeyEvent::KEYCODE_0
            && keyCode <= IKeyEvent::KEYCODE_9) {
        keyChar = keyCode - IKeyEvent::KEYCODE_0 + '0';
    } else if (keyCode == IKeyEvent::KEYCODE_COMMA) {
        keyChar = ',';
    } else if (keyCode == IKeyEvent::KEYCODE_PERIOD) {
        keyChar = '.';
    } else if (keyCode == IKeyEvent::KEYCODE_SPACE) {
        keyChar = ' ';
    } else if (keyCode == IKeyEvent::KEYCODE_APOSTROPHE) {
        keyChar = '\'';
    }

    if (mInputModeSwitcher->IsEnglishWithSkb(&res), res) {
        AutoPtr<IInputConnection> connection;
        InputMethodService::GetCurrentInputConnection((IInputConnection**)&connection);
        mInputModeSwitcher->IsEnglishUpperCaseWithSkb(&res);
        return mImEn->ProcessKey(connection, event, res, realAction);
    } else if (mInputModeSwitcher->IsChineseText(&res), res) {
        if (mImeState == Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_IDLE ||
                mImeState == Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_APP_COMPLETION) {
            mImeState = Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_IDLE;
            return ProcessStateIdle(keyChar, keyCode, event, realAction);
        } else if (mImeState == Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_INPUT) {
            return ProcessStateInput(keyChar, keyCode, event, realAction);
        } else if (mImeState == Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_PREDICT) {
            return ProcessStatePredict(keyChar, keyCode, event, realAction);
        } else if (mImeState == Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_COMPOSING) {
            return ProcessStateEditComposing(keyChar, keyCode, event,
                    realAction);
        }
    } else {
        if (0 != keyChar && realAction) {
            StringBuilder builder;
            builder.AppendChar(keyChar);
            String result = builder.ToString();
            CommitResultText(result);
        }
    }

    return FALSE;
}

Boolean CPinyinIME::ProcessFunctionKeys(
    /* [in] */ Int32 keyCode,
    /* [in] */ Boolean realAction)
{
    // Back key is used to dismiss all popup UI in a soft keyboard.
    if (keyCode == IKeyEvent::KEYCODE_BACK) {
        Boolean result = FALSE;
        if (InputMethodService::IsInputViewShown(&result), result) {
            if (mSkbContainer->HandleBack(realAction, &result), result) return TRUE;
        }
    }

    // Chinese related input is handle separately.
    Boolean res = FALSE;
    if (mInputModeSwitcher->IsChineseText(&res), res) {
        return FALSE;
    }

    if (NULL != mCandidatesContainer && (mCandidatesContainer->IsShown(&res), res)
            && !(mDecInfo->IsCandidatesListEmpty(&res), res)) {
        if (keyCode == IKeyEvent::KEYCODE_DPAD_CENTER) {
            if (!realAction) return TRUE;

            ChooseCandidate(-1);
            return TRUE;
        }

        if (keyCode == IKeyEvent::KEYCODE_DPAD_LEFT) {
            if (!realAction) return TRUE;
            mCandidatesContainer->ActiveCurseBackward(&res);
            return TRUE;
        }

        if (keyCode == IKeyEvent::KEYCODE_DPAD_RIGHT) {
            if (!realAction) return TRUE;
            mCandidatesContainer->ActiveCurseForward(&res);
            return TRUE;
        }

        if (keyCode == IKeyEvent::KEYCODE_DPAD_UP) {
            if (!realAction) return TRUE;
            mCandidatesContainer->PageBackward(FALSE, TRUE, &res);
            return TRUE;
        }

        if (keyCode == IKeyEvent::KEYCODE_DPAD_DOWN) {
            if (!realAction) return TRUE;
            mCandidatesContainer->PageForward(FALSE, TRUE, &res);
            return TRUE;
        }

        if (keyCode == IKeyEvent::KEYCODE_DEL &&
                Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_PREDICT == mImeState) {
            if (!realAction) return TRUE;
            ResetToIdleState(FALSE);
            return TRUE;
        }
    } else {
        if (keyCode == IKeyEvent::KEYCODE_DEL) {
            if (!realAction) return TRUE;
            if (SIMULATE_KEY_DELETE) {
                SimulateKeyEventDownUp(keyCode);
            } else {
                AutoPtr<IInputConnection> connection;
                InputMethodService::GetCurrentInputConnection((IInputConnection**)&connection);
                assert(connection != NULL);
                connection->DeleteSurroundingText(1, 0, &res);
            }
            return TRUE;
        }
        if (keyCode == IKeyEvent::KEYCODE_ENTER) {
            if (!realAction) return TRUE;
            InputMethodService::SendKeyChar('\n');
            return TRUE;
        }
        if (keyCode == IKeyEvent::KEYCODE_SPACE) {
            if (!realAction) return TRUE;
            InputMethodService::SendKeyChar(' ');
            return TRUE;
        }
    }

    return FALSE;
}

Boolean CPinyinIME::ProcessStateIdle(
    /* [in] */ Int32 keyChar,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Boolean realAction)
{
    // In this status, when user presses keys in [a..z], the status will
    // change to input state.
    Boolean pressed = FALSE;
    event->IsAltPressed(&pressed);
    if (keyChar >= 'a' && keyChar <= 'z' && !pressed) {
        if (!realAction) return TRUE;
        mDecInfo->AddSplChar((Char32) keyChar, TRUE);
        ChooseAndUpdate(-1);
        return TRUE;
    } else if (keyCode == IKeyEvent::KEYCODE_DEL) {
        if (!realAction) return TRUE;
        if (SIMULATE_KEY_DELETE) {
            SimulateKeyEventDownUp(keyCode);
        } else {
            AutoPtr<IInputConnection> connection;
            InputMethodService::GetCurrentInputConnection((IInputConnection**)&connection);
            Boolean result = FALSE;
            connection->DeleteSurroundingText(1, 0, &result);
        }
        return TRUE;
    } else if (keyCode == IKeyEvent::KEYCODE_ENTER) {
        if (!realAction) return TRUE;
        InputMethodService::SendKeyChar('\n');
        return TRUE;
    } else if (keyCode == IKeyEvent::KEYCODE_ALT_LEFT
            || keyCode == IKeyEvent::KEYCODE_ALT_RIGHT
            || keyCode == IKeyEvent::KEYCODE_SHIFT_LEFT
            || keyCode == IKeyEvent::KEYCODE_SHIFT_RIGHT) {
        return TRUE;
    } else if (pressed) {
        Char32 fullwidth_char = KeyMapDream::GetChineseLabel(keyCode);
        if (0 != fullwidth_char) {
            if (realAction) {
                String result;
                result += fullwidth_char;
                CommitResultText(result);
            }
            return TRUE;
        } else {
            if (keyCode >= IKeyEvent::KEYCODE_A
                    && keyCode <= IKeyEvent::KEYCODE_Z) {
                return TRUE;
            }
        }
    } else if (keyChar != 0 && keyChar != '\t') {
        if (realAction) {
            if (keyChar == ',' || keyChar == '.') {
                InputCommaPeriod(String(""), keyChar, FALSE, Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_IDLE);
            } else {
                if (0 != keyChar) {
                    String result;
                    result += keyChar;
                    CommitResultText(result);
                }
            }
        }
        return TRUE;
    }
    return FALSE;
}

Boolean CPinyinIME::ProcessStateInput(
    /* [in] */ Int32 keyChar,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Boolean realAction)
{
    // If ALT key is pressed, input alternative key. But if the
    // alternative key is quote key, it will be used for input a splitter
    // in Pinyin string.
    Boolean pressed = FALSE;
    event->IsAltPressed(&pressed);
    if (pressed) {
        Int32 metaState = 0, unicodeChar = 0;
        event->GetMetaState(&metaState);
        if ('\'' != (event->GetUnicodeChar(metaState, &unicodeChar), unicodeChar)) {
            if (realAction) {
                Char32 fullwidth_char = KeyMapDream::GetChineseLabel(keyCode);
                if (0 != fullwidth_char) {
                    Int32 pos = 0;
                    mCandidatesContainer->GetActiveCandiatePos(&pos);
                    String str;
                    mDecInfo->GetCurrentFullSent(pos, &str);
                    CommitResultText(str + fullwidth_char);
                    ResetToIdleState(FALSE);
                }
            }
            return TRUE;
        } else {
            keyChar = '\'';
        }
    }

    Boolean state = FALSE;
    if (keyChar >= 'a' && keyChar <= 'z' || keyChar == '\''
            && !(mDecInfo->CharBeforeCursorIsSeparator(&state), state)
            || keyCode == IKeyEvent::KEYCODE_DEL) {
        if (!realAction) return TRUE;
        return ProcessSurfaceChange(keyChar, keyCode);
    } else if (keyChar == ',' || keyChar == '.') {
        if (!realAction) return TRUE;
        Int32 pos = 0;
        mCandidatesContainer->GetActiveCandiatePos(&pos);
        String str;
        mDecInfo->GetCurrentFullSent(pos, &str);
        InputCommaPeriod(str, keyChar, TRUE, Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_IDLE);
        return TRUE;
    } else if (keyCode == IKeyEvent::KEYCODE_DPAD_UP
            || keyCode == IKeyEvent::KEYCODE_DPAD_DOWN
            || keyCode == IKeyEvent::KEYCODE_DPAD_LEFT
            || keyCode == IKeyEvent::KEYCODE_DPAD_RIGHT) {
        if (!realAction) return TRUE;

        Boolean result = FALSE;
        if (keyCode == IKeyEvent::KEYCODE_DPAD_LEFT) {
            mCandidatesContainer->ActiveCurseBackward(&result);
        } else if (keyCode == IKeyEvent::KEYCODE_DPAD_RIGHT) {
            mCandidatesContainer->ActiveCurseForward(&result);
        } else if (keyCode == IKeyEvent::KEYCODE_DPAD_UP) {
            // If it has been the first page, a up key will shift
            // the state to edit composing string.
            if (!(mCandidatesContainer->PageBackward(FALSE, TRUE, &state), state)) {
                mCandidatesContainer->EnableActiveHighlight(FALSE);
                ChangeToStateComposing(TRUE);
                UpdateComposingText(TRUE);
            }
        } else if (keyCode == IKeyEvent::KEYCODE_DPAD_DOWN) {
            mCandidatesContainer->PageForward(FALSE, TRUE, &result);
        }
        return TRUE;
    } else if (keyCode >= IKeyEvent::KEYCODE_1
            && keyCode <= IKeyEvent::KEYCODE_9) {
        if (!realAction) return TRUE;

        Int32 activePos = keyCode - IKeyEvent::KEYCODE_1;
        Int32 currentPage = 0, value = 0;
        mCandidatesContainer->GetCurrentPage(&currentPage);
        if (activePos < (mDecInfo->GetCurrentPageSize(currentPage, &value), value)) {
            activePos = activePos + (mDecInfo->GetCurrentPageStart(currentPage, &value), value);
            if (activePos >= 0) {
                ChooseAndUpdate(activePos);
            }
        }
        return TRUE;
    } else if (keyCode == IKeyEvent::KEYCODE_ENTER) {
        if (!realAction) return TRUE;

        if (mInputModeSwitcher->IsEnterNoramlState(&state), state) {
            AutoPtr<IStringBuffer> buf;
            mDecInfo->GetOrigianlSplStr((IStringBuffer**)&buf);
            String text;
            buf->ToString(&text);
            CommitResultText(text);
            ResetToIdleState(FALSE);
        } else {
            Int32 pos = 0;
            mCandidatesContainer->GetActiveCandiatePos(&pos);
            String str;
            mDecInfo->GetCurrentFullSent(pos, &str);
            CommitResultText(str);
            InputMethodService::SendKeyChar('\n');
            ResetToIdleState(FALSE);
        }
        return TRUE;
    } else if (keyCode == IKeyEvent::KEYCODE_DPAD_CENTER
            || keyCode == IKeyEvent::KEYCODE_SPACE) {
        if (!realAction) return TRUE;
        ChooseCandidate(-1);
        return TRUE;
    } else if (keyCode == IKeyEvent::KEYCODE_BACK) {
        if (!realAction) return TRUE;
        ResetToIdleState(FALSE);
        RequestHideSelf(0);
        return TRUE;
    }
    return FALSE;
}

Boolean CPinyinIME::ProcessStatePredict(
    /* [in] */ Int32 keyChar,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Boolean realAction)
{
    if (!realAction) return TRUE;

    // If ALT key is pressed, input alternative key.
    Boolean pressed = FALSE;
    event->IsAltPressed(&pressed);
    if (pressed) {
        Char32 fullwidth_char = KeyMapDream::GetChineseLabel(keyCode);
        if (0 != fullwidth_char) {
            Int32 pos = 0;
            mCandidatesContainer->GetActiveCandiatePos(&pos);

            String str;
            mDecInfo->GetCandidate(pos, &str);
            CommitResultText(str + fullwidth_char);
            ResetToIdleState(FALSE);
        }
        return TRUE;
    }

    // In this status, when user presses keys in [a..z], the status will
    // change to input state.
    if (keyChar >= 'a' && keyChar <= 'z') {
        ChangeToStateInput(TRUE);
        mDecInfo->AddSplChar((Char32) keyChar, TRUE);
        ChooseAndUpdate(-1);
    } else if (keyChar == ',' || keyChar == '.') {
        InputCommaPeriod(String(""), keyChar, TRUE, Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_IDLE);
    } else if (keyCode == IKeyEvent::KEYCODE_DPAD_UP
            || keyCode == IKeyEvent::KEYCODE_DPAD_DOWN
            || keyCode == IKeyEvent::KEYCODE_DPAD_LEFT
            || keyCode == IKeyEvent::KEYCODE_DPAD_RIGHT) {
        Boolean result = FALSE;
        if (keyCode == IKeyEvent::KEYCODE_DPAD_LEFT) {
            mCandidatesContainer->ActiveCurseBackward(&result);
        }
        if (keyCode == IKeyEvent::KEYCODE_DPAD_RIGHT) {
            mCandidatesContainer->ActiveCurseForward(&result);
        }
        if (keyCode == IKeyEvent::KEYCODE_DPAD_UP) {
            mCandidatesContainer->PageBackward(FALSE, TRUE, &result);
        }
        if (keyCode == IKeyEvent::KEYCODE_DPAD_DOWN) {
            mCandidatesContainer->PageForward(FALSE, TRUE, &result);
        }
    } else if (keyCode == IKeyEvent::KEYCODE_DEL) {
        ResetToIdleState(FALSE);
    } else if (keyCode == IKeyEvent::KEYCODE_BACK) {
        ResetToIdleState(FALSE);
        RequestHideSelf(0);
    } else if (keyCode >= IKeyEvent::KEYCODE_1
            && keyCode <= IKeyEvent::KEYCODE_9) {
        Int32 activePos = keyCode - IKeyEvent::KEYCODE_1;
        Int32 currentPage = 0, value = 0;
        mCandidatesContainer->GetCurrentPage(&currentPage);
        if (activePos < (mDecInfo->GetCurrentPageSize(currentPage, &value), value)) {
            activePos = activePos + (mDecInfo->GetCurrentPageStart(currentPage, &value), value);
            if (activePos >= 0) {
                ChooseAndUpdate(activePos);
            }
        }
    } else if (keyCode == IKeyEvent::KEYCODE_ENTER) {
        InputMethodService::SendKeyChar('\n');
        ResetToIdleState(FALSE);
    } else if (keyCode == IKeyEvent::KEYCODE_DPAD_CENTER
            || keyCode == IKeyEvent::KEYCODE_SPACE) {
        ChooseCandidate(-1);
    }

    return TRUE;
}

Boolean CPinyinIME::ProcessStateEditComposing(
    /* [in] */ Int32 keyChar,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Boolean realAction)
{
    if (!realAction) return TRUE;

    Elastos::Droid::Inputmethods::PinyinIME::ComposingStatus cmpsvStatus;
    mComposingView->GetComposingStatus(&cmpsvStatus);

    // If ALT key is pressed, input alternative key. But if the
    // alternative key is quote key, it will be used for input a splitter
    // in Pinyin string.
    Boolean pressed = FALSE;
    if (event->IsAltPressed(&pressed), pressed) {
        Int32 metaState = 0, unicodeChar = 0;
        event->GetMetaState(&metaState);
        if ('\'' != (event->GetUnicodeChar(metaState, &unicodeChar), unicodeChar)) {
            Char32 fullwidth_char = KeyMapDream::GetChineseLabel(keyCode);
            if (0 != fullwidth_char) {
                String retStr;
                if (Elastos::Droid::Inputmethods::PinyinIME::ComposingStatus_SHOW_STRING_LOWERCASE == cmpsvStatus) {
                    AutoPtr<IStringBuffer> buf;
                    mDecInfo->GetOrigianlSplStr((IStringBuffer**)&buf);
                    buf->ToString(&retStr);
                } else {
                    mDecInfo->GetComposingStr(&retStr);
                }
                CommitResultText(retStr + fullwidth_char);
                ResetToIdleState(FALSE);
            }
            return TRUE;
        } else {
            keyChar = '\'';
        }
    }

    Boolean state = FALSE;
    if (keyCode == IKeyEvent::KEYCODE_DPAD_DOWN) {
        if (!(mDecInfo->SelectionFinished(&state), state)) {
            ChangeToStateInput(TRUE);
        }
    } else if (keyCode == IKeyEvent::KEYCODE_DPAD_LEFT
            || keyCode == IKeyEvent::KEYCODE_DPAD_RIGHT) {
        mComposingView->MoveCursor(keyCode, &state);
    } else if ((keyCode == IKeyEvent::KEYCODE_ENTER && (mInputModeSwitcher->IsEnterNoramlState(&state), state))
            || keyCode == IKeyEvent::KEYCODE_DPAD_CENTER
            || keyCode == IKeyEvent::KEYCODE_SPACE) {
        if (Elastos::Droid::Inputmethods::PinyinIME::ComposingStatus_SHOW_STRING_LOWERCASE == cmpsvStatus) {
            String str;
            AutoPtr<IStringBuffer> buf;
            mDecInfo->GetOrigianlSplStr((IStringBuffer**)&buf);
            buf->ToString(&str);
            if (!TryInputRawUnicode(str)) {
                CommitResultText(str);
            }
        } else if (Elastos::Droid::Inputmethods::PinyinIME::ComposingStatus_EDIT_PINYIN == cmpsvStatus) {
            String str;
            mDecInfo->GetComposingStr(&str);
            if (!TryInputRawUnicode(str)) {
                CommitResultText(str);
            }
        } else {
            String str;
            mDecInfo->GetComposingStr(&str);
            CommitResultText(str);
        }
        ResetToIdleState(FALSE);
    } else if (keyCode == IKeyEvent::KEYCODE_ENTER
            && !(mInputModeSwitcher->IsEnterNoramlState(&state), state)) {
        String retStr;
        if (!(mDecInfo->IsCandidatesListEmpty(&state), state)) {
            Int32 pos = 0;
            mCandidatesContainer->GetActiveCandiatePos(&pos);
            mDecInfo->GetCurrentFullSent(pos, &retStr);
        } else {
            mDecInfo->GetComposingStr(&retStr);
        }
        CommitResultText(retStr);
        InputMethodService::SendKeyChar('\n');
        ResetToIdleState(FALSE);
    } else if (keyCode == IKeyEvent::KEYCODE_BACK) {
        ResetToIdleState(FALSE);
        RequestHideSelf(0);
        return TRUE;
    } else {
        return ProcessSurfaceChange(keyChar, keyCode);
    }
    return TRUE;
}

Boolean CPinyinIME::TryInputRawUnicode(
    /* [in] */ const String& str)
{
    if (str.GetLength() > 7) {
        if (str.StartWith("unicode")) {
            // try {
            String digitStr = str.Substring(7);
            Int32 startPos = 0;
            Int32 radix = 10;
            if (digitStr.GetLength() > 2 && digitStr.StartWith("0x")) {
                startPos = 2;
                radix = 16;
            }
            digitStr = digitStr.Substring(startPos);
            Int32 unicode = StringUtils::ParseInt32(digitStr, radix);
            if (unicode > 0) {
                Char32 low = (Char32) (unicode & 0x0000ffff);
                Char32 high = (Char32) ((unicode & 0xffff0000) >> 16);
                String lowStr;
                lowStr += low;
                CommitResultText(lowStr);
                if (0 != high) {
                    String highStr;
                    highStr += high;
                    CommitResultText(highStr);
                }
            }
            return TRUE;
            // } catch (NumberFormatException e) {
            //     return FALSE;
            // }
        } else if (str.Substring(str.GetLength() - 7, str.GetLength()).Equals(String("unicode"))) {
            String resultStr = String("");
            AutoPtr<ArrayOf<Char32> > chars = str.GetChars();
            for (Int32 pos = 0; pos < chars->GetLength() - 7; pos++) {
                if (pos > 0) {
                    resultStr += String(" ");
                }

                resultStr += String("0x") + StringUtils::Int32ToHexString((*chars)[pos]);
            }
            CommitResultText(resultStr/*String.valueOf(resultStr)*/);
            return TRUE;
        }
    }
    return FALSE;
}

Boolean CPinyinIME::ProcessSurfaceChange(
    /* [in] */ Int32 keyChar,
    /* [in] */ Int32 keyCode)
{
    Boolean state = FALSE;
    if ((mDecInfo->IsSplStrFull(&state), state) && IKeyEvent::KEYCODE_DEL != keyCode) {
        return TRUE;
    }

    if ((keyChar >= 'a' && keyChar <= 'z')
            || (keyChar == '\'' && !(mDecInfo->CharBeforeCursorIsSeparator(&state), state))
            || (((keyChar >= '0' && keyChar <= '9') || keyChar == ' ') &&
                Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_COMPOSING == mImeState)) {
        mDecInfo->AddSplChar((Char32) keyChar, FALSE);
        ChooseAndUpdate(-1);
    } else if (keyCode == IKeyEvent::KEYCODE_DEL) {
        mDecInfo->PrepareDeleteBeforeCursor();
        ChooseAndUpdate(-1);
    }
    return TRUE;
}

void CPinyinIME::ChangeToStateComposing(
    /* [in] */ Boolean updateUi)
{
    mImeState = Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_COMPOSING;
    if (!updateUi) return;

    Boolean state = FALSE;
    if (NULL != mSkbContainer && (mSkbContainer->IsShown(&state), state)) {
        mSkbContainer->ToggleCandidateMode(TRUE);
    }
}

void CPinyinIME::ChangeToStateInput(
    /* [in] */ Boolean updateUi)
{
    mImeState = Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_INPUT;
    if (!updateUi) return;

    Boolean state = FALSE;
    if (NULL != mSkbContainer && (mSkbContainer->IsShown(&state), state)) {
        mSkbContainer->ToggleCandidateMode(TRUE);
    }
    ShowCandidateWindow(TRUE);
}

void CPinyinIME::SimulateKeyEventDownUp(
    /* [in] */ Int32 keyCode)
{
    AutoPtr<IInputConnection> ic;
    InputMethodService::GetCurrentInputConnection((IInputConnection**)&ic);
    if (NULL == ic) return;

    Boolean result = FALSE;
    AutoPtr<IKeyEvent> event1, event2;
    CKeyEvent::New(IKeyEvent::ACTION_DOWN, keyCode, (IKeyEvent**)&event1);
    ic->SendKeyEvent(event1, &result);

    CKeyEvent::New(IKeyEvent::ACTION_UP, keyCode, (IKeyEvent**)&event2);
    ic->SendKeyEvent(event2, &result);
}

void CPinyinIME::CommitResultText(
    /* [in] */ const String& resultText)
{
    AutoPtr<IInputConnection> ic;
    InputMethodService::GetCurrentInputConnection((IInputConnection**)&ic);
    if (NULL != ic) {
        Boolean result = FALSE;
        AutoPtr<ICharSequence> text;
        CStringWrapper::New(resultText, (ICharSequence**)&text);
        ic->CommitText(text, 1, &result);
    }
    if (NULL != mComposingView) {
        mComposingView->SetVisibility(IView::INVISIBLE);
        mComposingView->Invalidate();
    }
}

void CPinyinIME::UpdateComposingText(
    /* [in] */ Boolean visible)
{
    if (!visible) {
        mComposingView->SetVisibility(IView::INVISIBLE);
    } else {
        mComposingView->SetDecodingInfo(mDecInfo, mImeState);
        mComposingView->SetVisibility(IView::VISIBLE);
    }
    mComposingView->Invalidate();
}

void CPinyinIME::InputCommaPeriod(
    /* [in] */ const String& preEdit,
    /* [in] */ Int32 keyChar,
    /* [in] */ Boolean dismissCandWindow,
    /* [in] */ ImeState nextState)
{
    StringBuilder buffer(preEdit);
    if (keyChar == ',')
        buffer.AppendChar(0xff0c/*'\uff0c'*/);
    else if (keyChar == '.')
        buffer.AppendChar(0x3002/*'\u3002'*/);
    else
        return;
    CommitResultText(buffer.ToString());
    if (dismissCandWindow) ResetCandidateWindow();
    mImeState = nextState;
}

void CPinyinIME::ResetToIdleState(
    /* [in] */ Boolean resetInlineText)
{
    if (Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_IDLE == mImeState) return;

    mImeState = Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_IDLE;
    mDecInfo->Reset();

    if (NULL != mComposingView) mComposingView->Reset();
    if (resetInlineText) CommitResultText(String(""));
    ResetCandidateWindow();
}

void CPinyinIME::ChooseAndUpdate(
    /* [in] */ Int32 candId)
{
    Boolean state = FALSE;
    if (!(mInputModeSwitcher->IsChineseText(&state), state)) {
        String choice;
        mDecInfo->GetCandidate(candId, &choice);
        if (NULL != choice) {
            CommitResultText(choice);
        }
        ResetToIdleState(FALSE);
        return;
    }

    if (Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_PREDICT != mImeState) {
        // Get result candidate list, if choice_id < 0, do a new decoding.
        // If choice_id >=0, select the candidate, and get the new candidate
        // list.
        mDecInfo->ChooseDecodingCandidate(candId);
    } else {
        // Choose a prediction item.
        mDecInfo->ChoosePredictChoice(candId);
    }

    String comStr;
    mDecInfo->GetComposingStr(&comStr);
    if (!comStr.IsNullOrEmpty()) {
        String resultStr;
        mDecInfo->GetComposingStrActivePart(&resultStr);

        // choiceId >= 0 means user finishes a choice selection.
        if (candId >= 0 && (mDecInfo->CanDoPrediction(&state), state)) {
            CommitResultText(resultStr);
            mImeState = Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_PREDICT;
            if (NULL != mSkbContainer && (mSkbContainer->IsShown(&state), state)) {
                mSkbContainer->ToggleCandidateMode(FALSE);
            }
            AutoPtr<ISettings> settings;
            CPinyinSettings::AcquireSingleton((ISettings**)&settings);
            // Try to get the prediction list.
            if (settings->GetPrediction(&state), state) {
                AutoPtr<IInputConnection> ic;
                InputMethodService::GetCurrentInputConnection((IInputConnection**)&ic);
                if (NULL != ic) {
                    AutoPtr<ICharSequence> cs;
                    ic->GetTextBeforeCursor(3, 0, (ICharSequence**)&cs);
                    if (NULL != cs) {
                        mDecInfo->PreparePredicts(cs);
                    }
                }
            } else {
                mDecInfo->ResetCandidates();
            }

            AutoPtr<ArrayOf<String> > candiList;
            mDecInfo->GetCandidatesList((ArrayOf<String>**)&candiList);
            if (candiList != NULL && candiList->GetLength() > 0) {
                ShowCandidateWindow(FALSE);
            } else {
                ResetToIdleState(FALSE);
            }
        } else {
            if (Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_IDLE == mImeState) {
                Int32 value = 0;
                if ((mDecInfo->GetSplStrDecodedLen(&value), value) == 0) {
                    ChangeToStateComposing(TRUE);
                } else {
                    ChangeToStateInput(TRUE);
                }
            } else {
                Boolean state = FALSE;
                if ((mDecInfo->SelectionFinished(&state), state)) {
                    ChangeToStateComposing(TRUE);
                }
            }
            ShowCandidateWindow(TRUE);
        }
    } else {
        ResetToIdleState(FALSE);
    }
}

void CPinyinIME::ChooseCandidate(
    /* [in] */ Int32 activeCandNo)
{
    if (activeCandNo < 0) {
        mCandidatesContainer->GetActiveCandiatePos(&activeCandNo);
    }
    if (activeCandNo >= 0) {
        ChooseAndUpdate(activeCandNo);
    }
}

Boolean CPinyinIME::StartPinyinDecoderService()
{
    AutoPtr<IPinyinDecoderService> service;
    mDecInfo->GetPinyinDecoderService((IPinyinDecoderService**)&service);
    if (NULL == service) {
        AutoPtr<IIntent> serviceIntent;
        CIntent::New((IIntent**)&serviceIntent);
        serviceIntent->SetClassName(this, String("PinyinIME.CPinyinDecoderService"));

        if (NULL == mPinyinDecoderServiceConnection) {
            mPinyinDecoderServiceConnection = new PinyinDecoderServiceConnection(this);
        }

        Boolean result = FALSE;
        // Bind service
        if (InputMethodService::BindService(serviceIntent, mPinyinDecoderServiceConnection,
                    IContext::BIND_AUTO_CREATE, &result), result) {
            return TRUE;
        } else {
            return FALSE;
        }
    }
    return TRUE;
}

ECode CPinyinIME::OnCreateCandidatesView(
    /* [out] */ IView** retView)
{
    // if (mEnvironment.needDebug()) {
    //     Log.d(TAG, "onCreateCandidatesView.");
    // }

    AutoPtr<ILayoutInflater> inflater;
    InputMethodService::GetLayoutInflater((ILayoutInflater**)&inflater);
    // Inflate the floating container view
    AutoPtr<IView> view;
    inflater->Inflate(R::layout::floating_container, NULL, (IView**)&view);
    mFloatingContainer = ILinearLayout::Probe(view);

    // The first child is the composing view.
    mFloatingContainer->GetChildAt(0, (IView**)&mComposingView);

    inflater->Inflate(R::layout::candidates_container, NULL, (IView**)&mCandidatesContainer);

    // Create balloon hint for candidates view.
    const Int32 MODE_SHIFT = 30;
    const Int32 UNSPECIFIED = 0 << MODE_SHIFT;
    CBalloonHint::New(this, mCandidatesContainer, UNSPECIFIED/*MeasureSpec::UNSPECIFIED*/, (IBalloonHint**)&mCandidatesBalloon);

    AutoPtr<IResources> res;
    InputMethodService::GetResources((IResources**)&res);
    AutoPtr<IDrawable> dr;
    res->GetDrawable(R::drawable::candidate_balloon_bg, (IDrawable**)&dr);
    mCandidatesBalloon->SetBalloonBackground(dr);
    assert(mCandidatesContainer != NULL);
    mCandidatesContainer->Initialize(mChoiceNotifier, mCandidatesBalloon, mGestureDetectorCandidates);

    // The floating window
    Boolean state = FALSE;
    if (NULL != mFloatingWindow && (mFloatingWindow->IsShowing(&state), state)) {
        mFloatingWindowTimer->CancelShowing();
        mFloatingWindow->Dismiss();
    }
    CPopupWindow::New(this, (IPopupWindow**)&mFloatingWindow);
    mFloatingWindow->SetClippingEnabled(FALSE);
    mFloatingWindow->SetBackgroundDrawable(NULL);
    mFloatingWindow->SetInputMethodMode(IPopupWindow::INPUT_METHOD_NOT_NEEDED);
    mFloatingWindow->SetContentView(mFloatingContainer);

    InputMethodService::SetCandidatesViewShown(TRUE);
    *retView = mCandidatesContainer;
    REFCOUNT_ADD(*retView);
    return NOERROR;
}

ECode CPinyinIME::ResponseSoftKeyEvent(
    /* [in] */ ISoftKey* sKey)
{
    if (NULL == sKey) return NOERROR;

    AutoPtr<IInputConnection> ic;
    InputMethodService::GetCurrentInputConnection((IInputConnection**)&ic);
    if (ic == NULL) return NOERROR;

    Int32 keyCode = 0;
    sKey->GetKeyCode(&keyCode);
    Boolean result = FALSE;
    // Process some general keys, including KEYCODE_DEL, KEYCODE_SPACE,
    // KEYCODE_ENTER and KEYCODE_DPAD_CENTER.
    if (sKey->IsKeyCodeKey(&result), result) {
        if (ProcessFunctionKeys(keyCode, TRUE)) return NOERROR;
    }

    if (sKey->IsUserDefKey(&result), result) {
        Int32 icon = 0;
        mInputModeSwitcher->SwitchModeForUserKey(keyCode, &icon);
        UpdateIcon(icon);
        ResetToIdleState(FALSE);
        mSkbContainer->UpdateInputMode();
    } else {
        if (sKey->IsKeyCodeKey(&result), result) {
            AutoPtr<IKeyEvent> eDown;
            AutoPtr<IKeyEvent> eUp;
            CKeyEvent::New(0, 0, IKeyEvent::ACTION_DOWN,
                    keyCode, 0, 0, 0, 0, IKeyEvent::FLAG_SOFT_KEYBOARD, (IKeyEvent**)&eDown);

            CKeyEvent::New(0, 0, IKeyEvent::ACTION_UP, keyCode,
                    0, 0, 0, 0, IKeyEvent::FLAG_SOFT_KEYBOARD, (IKeyEvent**)&eUp);

            OnKeyDown(keyCode, eDown, &result);
            OnKeyUp(keyCode, eUp, &result);
        } else if (sKey->IsUniStrKey(&result), result) {
            Boolean kUsed = FALSE;
            String keyLabel;
            sKey->GetKeyLabel(&keyLabel);
            if ((mInputModeSwitcher->IsChineseTextWithSkb(&result), result)
                    && (Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_INPUT == mImeState
                        || Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_COMPOSING == mImeState)) {
                Int32 len = 0;
                if ((mDecInfo->GetLength(&len), len) > 0 && keyLabel.GetLength() == 1
                        && keyLabel.GetChar(0) == '\'') {
                    ProcessSurfaceChange('\'', 0);
                    kUsed = TRUE;
                }
            }
            if (!kUsed) {
                Int32 pos = 0;
                mCandidatesContainer->GetActiveCandiatePos(&pos);
                String str;
                if (Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_INPUT == mImeState) {
                    CommitResultText((mDecInfo->GetCurrentFullSent(pos, &str), str));
                } else if (Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_COMPOSING == mImeState) {
                    CommitResultText((mDecInfo->GetComposingStr(&str), str));
                }
                CommitResultText(keyLabel);
                ResetToIdleState(FALSE);
            }
        }

        // If the current soft keyboard is not sticky, IME needs to go
        // back to the previous soft keyboard automatically.
        Boolean sticky = FALSE;
        if (!(mSkbContainer->IsCurrentSkbSticky(&sticky), sticky)) {
            Int32 icon = 0;
            mInputModeSwitcher->RequestBackToPreviousSkb(&icon);
            UpdateIcon(icon);
            ResetToIdleState(FALSE);
            mSkbContainer->UpdateInputMode();
        }
    }
    return NOERROR;
}

void CPinyinIME::ShowCandidateWindow(
    /* [in] */ Boolean showComposingView)
{
    // if (mEnvironment.needDebug()) {
    //     Log.d(TAG, "Candidates window is shown. Parent = "
    //             + mCandidatesContainer);
    // }

    InputMethodService::SetCandidatesViewShown(TRUE);

    if (NULL != mSkbContainer) mSkbContainer->RequestLayout();

    if (NULL == mCandidatesContainer) {
        ResetToIdleState(FALSE);
        return;
    }

    UpdateComposingText(showComposingView);
    mCandidatesContainer->ShowCandidates(mDecInfo, Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_COMPOSING != mImeState);
    mFloatingWindowTimer->PostShowFloatingWindow();
}

void CPinyinIME::DismissCandidateWindow()
{
    // if (mEnvironment.needDebug()) {
    //     Log.d(TAG, "Candidates window is to be dismissed");
    // }
    if (NULL == mCandidatesContainer) return;
    // try {
    mFloatingWindowTimer->CancelShowing();
    mFloatingWindow->Dismiss();
    // } catch (Exception e) {
    //     Log.e(TAG, "Fail to show the PopupWindow.");
    // }
    InputMethodService::SetCandidatesViewShown(FALSE);

    Boolean shown = FALSE;
    if (NULL != mSkbContainer && (mSkbContainer->IsShown(&shown), shown)) {
        mSkbContainer->ToggleCandidateMode(FALSE);
    }
}

void CPinyinIME::ResetCandidateWindow()
{
    // if (mEnvironment.needDebug()) {
    //     Log.d(TAG, "Candidates window is to be reset");
    // }
    if (NULL == mCandidatesContainer) return;
    // try {
    mFloatingWindowTimer->CancelShowing();
    mFloatingWindow->Dismiss();
    // } catch (Exception e) {
    //     Log.e(TAG, "Fail to show the PopupWindow.");
    // }

    Boolean shown = FALSE;
    if (NULL != mSkbContainer && (mSkbContainer->IsShown(&shown), shown)) {
        mSkbContainer->ToggleCandidateMode(FALSE);
    }

    mDecInfo->ResetCandidates();

    if (NULL != mCandidatesContainer && (mCandidatesContainer->IsShown(&shown), shown)) {
        ShowCandidateWindow(FALSE);
    }
}

void CPinyinIME::UpdateIcon(
    /* [in] */ Int32 iconId)
{
    if (iconId > 0) {
        InputMethodService::ShowStatusIcon(iconId);
    } else {
        InputMethodService::HideStatusIcon();
    }
}

ECode CPinyinIME::OnCreateInputView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    // if (mEnvironment.needDebug()) {
    //     Log.d(TAG, "onCreateInputView.");
    // }
    AutoPtr<ILayoutInflater> inflater;
    InputMethodService::GetLayoutInflater((ILayoutInflater**)&inflater);
    inflater->Inflate(R::layout::skb_container, NULL, (IView**)&mSkbContainer);
    mSkbContainer->SetService(this);
    mSkbContainer->SetInputModeSwitcher(mInputModeSwitcher);
    mSkbContainer->SetGestureDetector(mGestureDetectorSkb);
    *view = mSkbContainer;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CPinyinIME::OnStartInput(
    /* [in] */ IEditorInfo* editorInfo,
    /* [in] */ Boolean restarting)
{
    // if (mEnvironment.needDebug()) {
    //     Log.d(TAG, "onStartInput " + " ccontentType: "
    //             + String.valueOf(editorInfo.inputType) + " Restarting:"
    //             + String.valueOf(restarting));
    // }
    Int32 icon = 0;
    mInputModeSwitcher->RequestInputWithHkb(editorInfo, &icon);
    UpdateIcon(icon);
    ResetToIdleState(FALSE);
    return NOERROR;
}

ECode CPinyinIME::OnStartInputView(
    /* [in] */ IEditorInfo* editorInfo,
    /* [in] */ Boolean restarting)
{
    // if (mEnvironment.needDebug()) {
    //     Log.d(TAG, "onStartInputView " + " contentType: "
    //             + String.valueOf(editorInfo.inputType) + " Restarting:"
    //             + String.valueOf(restarting));
    // }
    Int32 icon = 0;
    mInputModeSwitcher->RequestInputWithSkb(editorInfo, &icon);
    UpdateIcon(icon);
    ResetToIdleState(FALSE);
    mSkbContainer->UpdateInputMode();
    return InputMethodService::SetCandidatesViewShown(FALSE);
}

ECode CPinyinIME::OnFinishInputView(
    /* [in] */ Boolean finishingInput)
{
    // if (mEnvironment.needDebug()) {
    //     Log.d(TAG, "onFinishInputView.");
    // }
    ResetToIdleState(FALSE);
    return InputMethodService::OnFinishInputView(finishingInput);
}

ECode CPinyinIME::OnFinishInput()
{
    // if (mEnvironment.needDebug()) {
    //     Log.d(TAG, "onFinishInput.");
    // }
    ResetToIdleState(FALSE);
    return InputMethodService::OnFinishInput();
}

ECode CPinyinIME::OnFinishCandidatesView(
    /* [in] */ Boolean finishingInput)
{
    // if (mEnvironment.needDebug()) {
    //     Log.d(TAG, "onFinishCandidateView.");
    // }
    ResetToIdleState(FALSE);
    return InputMethodService::OnFinishCandidatesView(finishingInput);
}

ECode CPinyinIME::OnDisplayCompletions(
    /* [in] */ ArrayOf<ICompletionInfo *>* completions)
{
    Boolean state = FALSE;
    if (!(InputMethodService::IsFullscreenMode(&state), state)) return NOERROR;
    if (NULL == completions || completions->GetLength() <= 0) return NOERROR;
    if (NULL == mSkbContainer || !(mSkbContainer->IsShown(&state), state)) return NOERROR;

    if (!(mInputModeSwitcher->IsChineseText(&state), state) ||
            Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_IDLE == mImeState ||
            Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_PREDICT == mImeState) {
        mImeState = Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_APP_COMPLETION;
        mDecInfo->PrepareAppCompletions(completions);
        ShowCandidateWindow(FALSE);
    }
    return NOERROR;
}

void CPinyinIME::OnChoiceTouched(
    /* [in] */ Int32 activeCandNo)
{
    if (mImeState == Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_COMPOSING) {
        ChangeToStateInput(TRUE);
    } else if (mImeState == Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_INPUT
            || mImeState == Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_PREDICT) {
        ChooseCandidate(activeCandNo);
    } else if (mImeState == Elastos::Droid::Inputmethods::PinyinIME::ImeState_STATE_APP_COMPLETION) {
        AutoPtr<ArrayOf<ICompletionInfo*> > completions;
        mDecInfo->GetAppCompletions((ArrayOf<ICompletionInfo*>**)&completions);
        if (NULL != completions && activeCandNo >= 0 &&
                activeCandNo < completions->GetLength()) {
            AutoPtr<ICompletionInfo> ci = (*completions)[activeCandNo];
            if (NULL != ci) {
                AutoPtr<IInputConnection> ic;
                InputMethodService::GetCurrentInputConnection((IInputConnection**)&ic);
                Boolean result = FALSE;
                ic->CommitCompletion(ci, &result);
            }
        }
        ResetToIdleState(FALSE);
    }
}

ECode CPinyinIME::RequestHideSelf(
    /* [in] */ Int32 flags)
{
    // if (mEnvironment.needDebug()) {
    //     Log.d(TAG, "DimissSoftInput.");
    // }
    DismissCandidateWindow();
    Boolean shown = FALSE;
    if (NULL != mSkbContainer && (mSkbContainer->IsShown(&shown), shown)) {
        mSkbContainer->DismissPopups();
    }
    return InputMethodService::RequestHideSelf(flags);
}

ECode CPinyinIME::ShowOptionsMenu()
{
    assert(mOptionsDialog == NULL);

    AutoPtr<IAlertDialogBuilder> builder;
    CAlertDialogBuilder::New(this, (IAlertDialogBuilder**)&builder);
    builder->SetCancelable(TRUE);
    builder->SetIcon(Elastos::Droid::DevSamples::PinyinIME::R::drawable::app_icon);
    builder->SetNegativeButton(Elastos::Droid::R::string::cancel, NULL);

    String value;
    InputMethodService::GetString(Elastos::Droid::DevSamples::PinyinIME::R::string::ime_settings_activity_name, &value);
    AutoPtr<ICharSequence> itemSettings;
    CStringWrapper::New(value, (ICharSequence**)&itemSettings);

    AutoPtr<ICharSequence> itemInputMethod;
    InputMethodService::GetString(Elastos::Droid::R::string::inputMethod, &value);
    CStringWrapper::New(value, (ICharSequence**)&itemInputMethod);

    AutoPtr<ArrayOf<ICharSequence*> > items = ArrayOf<ICharSequence*>::Alloc(2);
    items->Set(0, itemSettings);
    items->Set(1, itemInputMethod);

    AutoPtr<BuilderListener> listener = new BuilderListener(this);
    builder->SetItems(items, listener);

    String name;
    InputMethodService::GetString(R::string::ime_name, &name);
    AutoPtr<ICharSequence> csName;
    CStringWrapper::New(name, (ICharSequence**)&csName);
    builder->SetTitle(csName);
    builder->Create((IAlertDialog**)&mOptionsDialog);
    mOptionsDialog->SetOnDismissListener((IDialogInterfaceOnDismissListener*)listener.Get());

    AutoPtr<IWindow> window;
    mOptionsDialog->GetWindow((IWindow**)&window);
    AutoPtr<IWindowManagerLayoutParams> lp;
    window->GetAttributes((IWindowManagerLayoutParams**)&lp);

    AutoPtr<IBinder> token;
    mSkbContainer->GetWindowToken((IBinder**)&token);
    lp->SetToken(token);

    lp->SetType(IWindowManagerLayoutParams::TYPE_APPLICATION_ATTACHED_DIALOG);
    window->SetAttributes(lp);
    window->AddFlags(IWindowManagerLayoutParams::FLAG_ALT_FOCUSABLE_IM);
    return mOptionsDialog->Show();
}

ECode CPinyinIME::GetImeState(
    /* [out] */ ImeState* state)
{
    VALIDATE_NOT_NULL(state);
    *state = mImeState;
    return NOERROR;
}

ECode CPinyinIME::GetCandidatesContainer(
    /* [out] */ ICandidatesContainer** container)
{
    VALIDATE_NOT_NULL(container);
    *container = mCandidatesContainer;
    REFCOUNT_ADD(*container);
    return NOERROR;
}

void CPinyinIME::LaunchSettings()
{
    AutoPtr<IIntent> intent;
    CIntent::New((IIntent**)&intent);
    assert(0 && "TODO: CSettingsActivity Not Implement...");
    intent->SetClassName(/*PinyinIME.this*/this, String("PinyinIME.CSettingsActivity"));
    intent->SetFlags(IIntent::FLAG_ACTIVITY_NEW_TASK);
    InputMethodService::StartActivity(intent);
}

ECode CPinyinIME::GetAssets(
    /* [out] */ IAssetManager** assetManager)
{
    return InputMethodService::GetAssets(assetManager);
}

ECode CPinyinIME::GetResources(
    /* [out] */ IResources** resources)
{
    return InputMethodService::GetResources(resources);
}

ECode CPinyinIME::GetPackageManager(
    /* [out] */ IPackageManager** packageManager)
{
    return InputMethodService::GetPackageManager(packageManager);
}

ECode CPinyinIME::GetContentResolver(
    /* [out] */ IContentResolver** resolver)
{
    return InputMethodService::GetContentResolver(resolver);
}

ECode CPinyinIME::GetApplicationContext(
    /* [out] */ IContext** ctx)
{
    return InputMethodService::GetApplicationContext(ctx);
}

ECode CPinyinIME::GetText(
    /* [in] */ Int32 resId,
    /* [out] */ ICharSequence** text)
{
    return InputMethodService::GetText(resId, text);
}

ECode CPinyinIME::GetString(
    /* [in] */ Int32 resId,
    /* [out] */ String* str)
{
    return InputMethodService::GetString(resId, str);
}

ECode CPinyinIME::SetTheme(
    /* [in] */ Int32 resid)
{
    return InputMethodService::SetTheme(resid);
}

ECode CPinyinIME::GetTheme(
    /* [out] */ IResourcesTheme** theme)
{
    return InputMethodService::GetTheme(theme);
}

ECode CPinyinIME::ObtainStyledAttributes(
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    return InputMethodService::ObtainStyledAttributes(attrs, styles);
}

ECode CPinyinIME::ObtainStyledAttributes(
    /* [in] */ Int32 resid,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    return InputMethodService::ObtainStyledAttributes(resid, attrs, styles);
}

ECode CPinyinIME::ObtainStyledAttributes(
    /* [in] */ IAttributeSet* set,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [out] */ ITypedArray** styles)
{
    return InputMethodService::ObtainStyledAttributes(set, attrs, styles);
}

ECode CPinyinIME::ObtainStyledAttributes(
    /* [in] */ IAttributeSet* set,
    /* [in] */ ArrayOf<Int32>* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes,
    /* [out] */ ITypedArray** styles)
{
    return InputMethodService::ObtainStyledAttributes(set, attrs, defStyleAttr, defStyleRes, styles);
}

ECode CPinyinIME::GetClassLoader(
    /* [out] */ IClassLoader** loader)
{
    return InputMethodService::GetClassLoader(loader);
}

ECode CPinyinIME::GetPackageName(
    /* [out] */ String* packageName)
{
    return InputMethodService::GetPackageName(packageName);
}

ECode CPinyinIME::GetApplicationInfo(
    /* [out] */ IApplicationInfo** info)
{
    return InputMethodService::GetApplicationInfo(info);
}

ECode CPinyinIME::GetPackageResourcePath(
    /* [out] */ String* path)
{
    return InputMethodService::GetPackageResourcePath(path);
}

ECode CPinyinIME::GetExternalFilesDir(
    /* [in] */ const String& type,
    /* [out] */ IFile** filesDir)
{
    return InputMethodService::GetExternalFilesDir(type, filesDir);
}

ECode CPinyinIME::GetCacheDir(
    /* [out] */ IFile** cacheDir)
{
    return InputMethodService::GetCacheDir(cacheDir);
}

ECode CPinyinIME::GetDir(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ IFile** dir)
{
    return InputMethodService::GetDir(name, mode, dir);
}

ECode CPinyinIME::StartActivity(
    /* [in] */ IIntent* intent)
{
    return InputMethodService::StartActivity(intent);
}

ECode CPinyinIME::StartIntentSender(
    /* [in] */ IIntentSender* intent,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags)
{
    return InputMethodService::StartIntentSender(intent, fillInIntent, flagsMask, flagsValues, extraFlags);
}

ECode CPinyinIME::SendBroadcast(
    /* [in] */ IIntent* intent)
{
    return InputMethodService::SendBroadcast(intent);
}

ECode CPinyinIME::StartService(
    /* [in] */ IIntent* service,
    /* [out] */ IComponentName** name)
{
    return InputMethodService::StartService(service, name);
}

ECode CPinyinIME::StopService(
    /* [in] */ IIntent* service,
    /* [out] */ Boolean* succeeded)
{
    return InputMethodService::StopService(service, succeeded);
}

ECode CPinyinIME::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
    return InputMethodService::BindService(service, conn, flags, succeeded);
}

ECode CPinyinIME::UnbindService(
    /* [in] */ IServiceConnection* conn)
{
    return InputMethodService::UnbindService(conn);
}

ECode CPinyinIME::GetSystemService(
    /* [in] */ const String& name,
    /* [out] */ IInterface** object)
{
    return InputMethodService::GetSystemService(name, object);
}

ECode CPinyinIME::CreatePackageContext(
    /* [in] */ const String& capsuleName,
    /* [in] */ Int32 flags,
    /* [out] */ IContext** ctx)
{
    return InputMethodService::CreatePackageContext(capsuleName, flags, ctx);
}

ECode CPinyinIME::CheckCallingPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* value)
{
    return InputMethodService::CheckCallingPermission(permission, value);
}

ECode CPinyinIME::EnforceCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [in] */ const String& message)
{
    return InputMethodService::EnforceCallingOrSelfPermission(permission, message);
}

ECode CPinyinIME::RevokeUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return InputMethodService::RevokeUriPermission(uri, modeFlags);
}

ECode CPinyinIME::CheckCallingOrSelfPermission(
    /* [in] */ const String& permission,
    /* [out] */ Int32* perm)
{
    return InputMethodService::CheckCallingOrSelfPermission(permission, perm);
}

ECode CPinyinIME::CheckPermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [out] */ Int32 * result)
{
    return InputMethodService::CheckPermission(permission, pid, uid, result);
}

ECode CPinyinIME::CheckUriPermission(
    /* [in] */ IUri * uri,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32 * result)
{
    return InputMethodService::CheckUriPermission(uri, readPermission, writePermission, pid, uid, modeFlags, result);
}

ECode CPinyinIME::CheckUriPermission(
    /* [in] */ IUri * uri,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32 * result)
{
    return InputMethodService::CheckUriPermission(uri, pid, uid, modeFlags, result);
}

ECode CPinyinIME::GrantUriPermission(
    /* [in] */ const String& toCapsule,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    return InputMethodService::GrantUriPermission(toCapsule, uri, modeFlags);
}

ECode CPinyinIME::GetBaseContext(
    /* [out] */ IContext** ctx)
{
    return InputMethodService::GetBaseContext(ctx);
}

ECode CPinyinIME::Attach(
    /* [in] */ IContext* ctx,
    /* [in] */ IActivityThread* apartment,
    /* [in] */ const String& className,
    /* [in] */ IBinder* token,
    /* [in] */ IApplication* application,
    /* [in] */ IIActivityManager* activityManager)
{
    return InputMethodService::Attach(ctx, apartment, className, token, application, activityManager);
}

ECode CPinyinIME::OnStart(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 startId)
{
    return InputMethodService::OnStart(intent, startId);
}

ECode CPinyinIME::OnStartCommand(
    /* [in] */ IIntent* intent,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 startId,
    /* [out] */ Int32* result)
{
    return InputMethodService::OnStartCommand(intent, flags, startId, result);
}

ECode CPinyinIME::OnBind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder);
    ECode ec = InputMethodService::OnBind(intent, binder);
    // assert(*binder != NULL);
    // AutoPtr<IProxy> proxy = (IProxy*)(*binder)->Probe(EIID_IProxy);
    // assert(proxy != NULL);
    return ec;
}

ECode CPinyinIME::OnUnbind(
    /* [in] */ IIntent* intent,
    /* [out] */ Boolean* succeeded)
{
    return InputMethodService::OnUnbind(intent, succeeded);
}

ECode CPinyinIME::OnRebind(
    /* [in] */ IIntent* intent)
{
    return InputMethodService::OnRebind(intent);
}

ECode CPinyinIME::GetClassName(
    /* [out] */ String* className)
{
    return InputMethodService::GetClassName(className);
}

ECode CPinyinIME::GetKeyDispatcherState(
    /* [out] */ IDispatcherState** dispatcherState)
{
    return InputMethodService::GetKeyDispatcherState(dispatcherState);
}

ECode CPinyinIME::OnCreateInputMethodInterface(
    /* [out] */ IAbstractInputMethodImpl** inputMethodImpl)
{
    return InputMethodService::OnCreateInputMethodInterface(inputMethodImpl);
}

ECode CPinyinIME::OnCreateInputMethodSessionInterface(
    /* [out] */ IAbstractInputMethodSessionImpl** abstractInputMethodSessionImpl)
{
    return InputMethodService::OnCreateInputMethodSessionInterface(abstractInputMethodSessionImpl);
}

ECode CPinyinIME::OnTrackballEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* state)
{
    return InputMethodService::OnTrackballEvent(event, state);
}

ECode CPinyinIME::OnInitializeInterface()
{
    return InputMethodService::OnInitializeInterface();
}

ECode CPinyinIME::GetLayoutInflater(
    /* [out] */ ILayoutInflater** inflater)
{
    return InputMethodService::GetLayoutInflater(inflater);
}

ECode CPinyinIME::GetWindow(
    /* [out] */ IDialog** dialog)
{
    return InputMethodService::GetWindow(dialog);
}

ECode CPinyinIME::GetMaxWidth(
    /* [out] */ Int32* maxWidth)
{
    return InputMethodService::GetMaxWidth(maxWidth);
}

ECode CPinyinIME::GetCurrentInputBinding(
    /* [out] */ IInputBinding** binding)
{
    return InputMethodService::GetCurrentInputBinding(binding);
}

ECode CPinyinIME::GetCurrentInputConnection(
    /* [out] */ IInputConnection** inputConnection)
{
    return InputMethodService::GetCurrentInputConnection(inputConnection);
}

ECode CPinyinIME::GetCurrentInputStarted(
    /* [out] */ Boolean* state)
{
    return InputMethodService::GetCurrentInputStarted(state);
}

ECode CPinyinIME::GetCurrentInputEditorInfo(
    /* [out] */ IEditorInfo** editorInfo)
{
    return InputMethodService::GetCurrentInputEditorInfo(editorInfo);
}

ECode CPinyinIME::UpdateFullscreenMode()
{
    return InputMethodService::UpdateFullscreenMode();
}

ECode CPinyinIME::OnConfigureWindow(
    /* [in] */ IWindow* win,
    /* [in] */ Boolean isFullscreen,
    /* [in] */ Boolean isCandidatesOnly)
{
    return InputMethodService::OnConfigureWindow(win, isFullscreen, isCandidatesOnly);
}

ECode CPinyinIME::IsFullscreenMode(
    /* [out] */ Boolean* fullScreenMode)
{
    return InputMethodService::IsFullscreenMode(fullScreenMode);
}

ECode CPinyinIME::SetExtractViewShown(
    /* [in] */ Boolean shown)
{
    return InputMethodService::SetExtractViewShown(shown);
}

ECode CPinyinIME::IsExtractViewShown(
    /* [out] */ Boolean* shown)
{
    return InputMethodService::IsExtractViewShown(shown);
}

ECode CPinyinIME::UpdateInputViewShown()
{
    return InputMethodService::UpdateInputViewShown();
}

ECode CPinyinIME::IsShowInputRequested(
    /* [out] */ Boolean* requested)
{
    return InputMethodService::IsShowInputRequested(requested);
}

ECode CPinyinIME::IsInputViewShown(
    /* [out] */ Boolean* shown)
{
    return InputMethodService::IsInputViewShown(shown);
}

ECode CPinyinIME::OnEvaluateInputViewShown(
    /* [out] */ Boolean* shown)
{
    return InputMethodService::OnEvaluateInputViewShown(shown);
}

ECode CPinyinIME::SetCandidatesViewShown(
    /* [in] */ Boolean shown)
{
    return InputMethodService::SetCandidatesViewShown(shown);
}

ECode CPinyinIME::GetCandidatesHiddenVisibility(
    /* [out] */ Int32* visibility)
{
    return InputMethodService::GetCandidatesHiddenVisibility(visibility);
}

ECode CPinyinIME::ShowStatusIcon(
    /* [in] */ Int32 iconResId)
{
    return InputMethodService::ShowStatusIcon(iconResId);
}

ECode CPinyinIME::HideStatusIcon()
{
    return InputMethodService::HideStatusIcon();
}

ECode CPinyinIME::SwitchInputMethod(
    /* [in] */ const String& id)
{
    return InputMethodService::SwitchInputMethod(id);
}

ECode CPinyinIME::SetExtractView(
    /* [in] */ IView* view)
{
    return InputMethodService::SetExtractView(view);
}

ECode CPinyinIME::SetCandidatesView(
    /* [in] */ IView* view)
{
    return InputMethodService::SetCandidatesView(view);
}

ECode CPinyinIME::SetInputView(
    /* [in] */ IView* view)
{
    return InputMethodService::SetInputView(view);
}

ECode CPinyinIME::OnCreateExtractTextView(
    /* [out] */ IView** view)
{
    return InputMethodService::OnCreateExtractTextView(view);
}

ECode CPinyinIME::OnStartCandidatesView(
    /* [in] */ IEditorInfo* info,
    /* [in] */ Boolean restarting)
{
    return InputMethodService::OnStartCandidatesView(info, restarting);
}

ECode CPinyinIME::OnShowInputRequested(
    /* [in] */ Int32 flags,
    /* [in] */ Boolean configChange,
    /* [out] */ Boolean* requested)
{
    return InputMethodService::OnShowInputRequested(flags, configChange, requested);
}

ECode CPinyinIME::ShowWindow(
    /* [in] */ Boolean showInput)
{
    return InputMethodService::ShowWindow(showInput);
}

ECode CPinyinIME::HideWindow()
{
    return InputMethodService::HideWindow();
}

ECode CPinyinIME::OnWindowShown()
{
    return InputMethodService::OnWindowShown();
}

ECode CPinyinIME::OnWindowHidden()
{
    return InputMethodService::OnWindowHidden();
}

ECode CPinyinIME::OnBindInput()
{
    return InputMethodService::OnBindInput();
}

ECode CPinyinIME::OnUnbindInput()
{
    return InputMethodService::OnUnbindInput();
}

ECode CPinyinIME::OnUpdateExtractedText(
    /* [in] */ Int32 token,
    /* [in] */ IExtractedText* text)
{
    return InputMethodService::OnUpdateExtractedText(token, text);
}

ECode CPinyinIME::OnUpdateSelection(
    /* [in] */ Int32 oldSelStart,
    /* [in] */ Int32 oldSelEnd,
    /* [in] */ Int32 newSelStart,
    /* [in] */ Int32 newSelEnd,
    /* [in] */ Int32 candidatesStart,
    /* [in] */ Int32 candidatesEnd)
{
    return InputMethodService::OnUpdateSelection(oldSelStart, oldSelEnd, newSelStart, newSelEnd, candidatesStart, candidatesEnd);
}

ECode CPinyinIME::OnUpdateCursor(
    /* [in] */ IRect* newCursor)
{
    return InputMethodService::OnUpdateCursor(newCursor);
}

ECode CPinyinIME::OnAppPrivateCommand(
    /* [in] */ const String& action,
    /* [in] */ IBundle* data)
{
    return InputMethodService::OnAppPrivateCommand(action, data);
}

ECode CPinyinIME::SendDownUpKeyEvents(
    /* [in] */ Int32 keyEventCode)
{
    return InputMethodService::SendDownUpKeyEvents(keyEventCode);
}

ECode CPinyinIME::SendDefaultEditorAction(
    /* [in] */ Boolean fromEnterKey,
    /* [out] */ Boolean* state)
{
    return InputMethodService::SendDefaultEditorAction(fromEnterKey, state);
}

ECode CPinyinIME::SendKeyChar(
    /* [in] */ Char32 charCode)
{
    return InputMethodService::SendKeyChar(charCode);
}

ECode CPinyinIME::OnExtractedSelectionChanged(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return InputMethodService::OnExtractedSelectionChanged(start, end);
}

ECode CPinyinIME::OnExtractedTextClicked()
{
    return InputMethodService::OnExtractedTextClicked();
}

ECode CPinyinIME::OnExtractedCursorMovement(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    return InputMethodService::OnExtractedCursorMovement(dx, dy);
}

ECode CPinyinIME::OnExtractTextContextMenuItem(
    /* [in] */ Int32 id,
    /* [out] */ Boolean* state)
{
    return InputMethodService::OnExtractTextContextMenuItem(id, state);
}

ECode CPinyinIME::GetTextForImeAction(
    /* [in] */ Int32 imeOptions,
    /* [out] */ ICharSequence** text)
{
    return InputMethodService::GetTextForImeAction(imeOptions, text);
}

ECode CPinyinIME::OnUpdateExtractingVisibility(
    /* [in] */ IEditorInfo* ei)
{
    return InputMethodService::OnUpdateExtractingVisibility(ei);
}

ECode CPinyinIME::OnUpdateExtractingViews(
    /* [in] */ IEditorInfo* ei)
{
    return InputMethodService::OnUpdateExtractingViews(ei);
}

ECode CPinyinIME::OnExtractingInputChanged(
    /* [in] */ IEditorInfo* ei)
{
    return InputMethodService::OnExtractingInputChanged(ei);
}

ECode CPinyinIME::OnLowMemory()
{
    return InputMethodService::OnLowMemory();
}

ECode CPinyinIME::OnTrimMemory(
    /* [in] */ Int32 level)
{
    return InputMethodService::OnTrimMemory(level);
}

ECode CPinyinIME::GetMainLooper(
    /* [out] */ ILooper** looper)
{
    return InputMethodService::GetMainLooper(looper);
}

ECode CPinyinIME::RegisterComponentCallbacks(
    /* [in] */ IComponentCallbacks* componentCallback)
{
    return InputMethodService::RegisterComponentCallbacks(componentCallback);
}

ECode CPinyinIME::UnregisterComponentCallbacks(
    /* [in] */ IComponentCallbacks* componentCallback)
{
    return InputMethodService::UnregisterComponentCallbacks(componentCallback);
}

ECode CPinyinIME::GetString(
    /* [in] */ Int32 resId,
    /* [in] */ ArrayOf<IInterface*>* formatArgs,
    /* [out] */ String* str)
{
    return InputMethodService::GetString(resId, formatArgs, str);
}

ECode CPinyinIME::GetThemeResId(
    /* [out] */ Int32* resId)
{
    return InputMethodService::GetThemeResId(resId);
}

ECode CPinyinIME::GetPackageCodePath(
    /* [out] */ String* codePath)
{
    return InputMethodService::GetPackageCodePath(codePath);
}

ECode CPinyinIME::GetSharedPrefsFile(
    /* [in] */ const String& name,
    /* [out] */ IFile** file)
{
    return InputMethodService::GetSharedPrefsFile(name, file);
}

ECode CPinyinIME::GetSharedPreferences(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */ ISharedPreferences** prefs)
{
    return InputMethodService::GetSharedPreferences(name, mode, prefs);
}

ECode CPinyinIME::OpenFileOutput(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [out] */IFileOutputStream** fileOutputStream)
{
    return InputMethodService::OpenFileOutput(name, mode, fileOutputStream);
}

ECode CPinyinIME::DeleteFile(
    /* [in] */ const String& name,
    /* [out] */ Boolean* succeeded)
{
    return InputMethodService::DeleteFile(name, succeeded);
}

ECode CPinyinIME::GetFileStreamPath(
    /* [in] */ const String& name,
    /* [out] */ IFile** file)
{
    return InputMethodService::GetFileStreamPath(name, file);
}

ECode CPinyinIME::GetFilesDir(
    /* [out] */ IFile** filesDir)
{
    return InputMethodService::GetFilesDir(filesDir);
}

ECode CPinyinIME::GetObbDir(
    /* [out] */ IFile** obbDir)
{
    return InputMethodService::GetObbDir(obbDir);
}

ECode CPinyinIME::GetFileList(
    /* [out, callee] */ ArrayOf<String>** fileList)
{
    return InputMethodService::GetFileList(fileList);
}

ECode CPinyinIME::OpenOrCreateDatabase(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [in] */ ISQLiteDatabaseCursorFactory* factory,
    /* [out] */ ISQLiteDatabase** sqliteDB)
{
    return InputMethodService::OpenOrCreateDatabase(name, mode, factory, sqliteDB);
}

ECode CPinyinIME::OpenOrCreateDatabase(
    /* [in] */ const String& name,
    /* [in] */ Int32 mode,
    /* [in] */ ISQLiteDatabaseCursorFactory* factory,
    /* [in] */ IDatabaseErrorHandler* errorHandler,
    /* [out] */ ISQLiteDatabase** sqliteDB)
{
    return InputMethodService::OpenOrCreateDatabase(name, mode, factory, errorHandler, sqliteDB);
}

ECode CPinyinIME::DeleteDatabase(
    /* [in] */ const String& name,
    /* [out] */ Boolean* succeeded)
{
    return InputMethodService::DeleteDatabase(name, succeeded);
}

ECode CPinyinIME::GetDatabasePath(
    /* [in] */ const String& name,
    /* [out] */ IFile** path)
{
    return InputMethodService::GetDatabasePath(name, path);
}

ECode CPinyinIME::GetDatabaseList(
    /* [out, callee] */ ArrayOf<String>** databaseList)
{
    return InputMethodService::GetDatabaseList(databaseList);
}

ECode CPinyinIME::GetWallpaper(
    /* [out] */ IDrawable** drawable)
{
    return InputMethodService::GetWallpaper(drawable);
}

ECode CPinyinIME::PeekWallpaper(
    /* [out] */ IDrawable** drawable)
{
    return InputMethodService::PeekWallpaper(drawable);
}

ECode CPinyinIME::GetWallpaperDesiredMinimumWidth(
    /* [out] */ Int32* minWidth)
{
    return InputMethodService::GetWallpaperDesiredMinimumWidth(minWidth);
}

ECode CPinyinIME::GetWallpaperDesiredMinimumHeight(
    /* [out] */ Int32* minHeight)
{
    return InputMethodService::GetWallpaperDesiredMinimumHeight(minHeight);
}

ECode CPinyinIME::SetWallpaper(
    /* [in] */ IBitmap* bitmap)
{
    return InputMethodService::SetWallpaper(bitmap);
}

ECode CPinyinIME::SetWallpaper(
    /* [in] */ IInputStream* data)
{
    return InputMethodService::SetWallpaper(data);
}

ECode CPinyinIME::ClearWallpaper()
{
    return InputMethodService::ClearWallpaper();
}

ECode CPinyinIME::StartActivityAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return InputMethodService::StartActivityAsUser(intent, user);
}

ECode CPinyinIME::StartActivityAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IBundle* options,
    /* [in] */ IUserHandle* user)
{
    return InputMethodService::StartActivityAsUser(intent, options, user);
}

ECode CPinyinIME::StartActivities(
    /* [in] */ ArrayOf<IIntent*>* intents,
    /* [in] */ IBundle* options)
{
    return InputMethodService::StartActivities(intents, options);
}

ECode CPinyinIME::StartIntentSender(
    /* [in] */ IIntentSender* intent,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ Int32 flagsMask,
    /* [in] */ Int32 flagsValues,
    /* [in] */ Int32 extraFlags,
    /* [in] */ IBundle* options)
{
    return InputMethodService::StartIntentSender(intent, fillInIntent, flagsMask, flagsValues, extraFlags, options);
}

ECode CPinyinIME::SendBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission)
{
    return InputMethodService::SendBroadcast(intent, receiverPermission);
}

ECode CPinyinIME::SendOrderedBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission)
{
    return InputMethodService::SendOrderedBroadcast(intent, receiverPermission);
}

ECode CPinyinIME::SendOrderedBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ const String& receiverPermission,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return InputMethodService::SendOrderedBroadcast(intent, receiverPermission, resultReceiver, scheduler, initialCode, initialData, initialExtras);
}

ECode CPinyinIME::SendBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return InputMethodService::SendBroadcastAsUser(intent, user);
}

ECode CPinyinIME::SendBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ const String& receiverPermission)
{
    return InputMethodService::SendBroadcastAsUser(intent, user, receiverPermission);
}

ECode CPinyinIME::SendOrderedBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ const String& receiverPermission,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return InputMethodService::SendOrderedBroadcastAsUser(intent, user, receiverPermission, resultReceiver, scheduler, initialCode, initialData, initialExtras);
}

ECode CPinyinIME::SendStickyBroadcast(
    /* [in] */ IIntent* intent)
{
    return InputMethodService::SendStickyBroadcast(intent);
}

ECode CPinyinIME::SendStickyOrderedBroadcast(
    /* [in] */ IIntent* intent,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return InputMethodService::SendStickyOrderedBroadcast(intent, resultReceiver, scheduler, initialCode, initialData, initialExtras);
}

ECode CPinyinIME::RemoveStickyBroadcast(
    /* [in] */ IIntent* intent)
{
    return InputMethodService::RemoveStickyBroadcast(intent);
}

ECode CPinyinIME::SendStickyBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return InputMethodService::SendStickyBroadcastAsUser(intent, user);
}

ECode CPinyinIME::SendStickyOrderedBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user,
    /* [in] */ IBroadcastReceiver* resultReceiver,
    /* [in] */ IHandler* scheduler,
    /* [in] */ Int32 initialCode,
    /* [in] */ const String& initialData,
    /* [in] */ IBundle* initialExtras)
{
    return InputMethodService::SendStickyOrderedBroadcastAsUser(intent, user, resultReceiver, scheduler, initialCode, initialData, initialExtras);
}

ECode CPinyinIME::RemoveStickyBroadcastAsUser(
    /* [in] */ IIntent* intent,
    /* [in] */ IUserHandle* user)
{
    return InputMethodService::RemoveStickyBroadcastAsUser(intent, user);
}

ECode CPinyinIME::StartInstrumentation(
    /* [in] */ IComponentName* className,
    /* [in] */ const String& profileFile,
    /* [in] */ IBundle* arguments,
    /* [out] */ Boolean* succeeded)
{
    return InputMethodService::StartInstrumentation(className, profileFile, arguments, succeeded);
}

ECode CPinyinIME::EnforcePermission(
    /* [in] */ const String& permission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ const String& message)
{
    return InputMethodService::EnforcePermission(permission, pid, uid, message);
}

ECode CPinyinIME::EnforceCallingPermission(
    /* [in] */ const String& permission,
    /* [in] */ const String& message)
{
    return InputMethodService::EnforceCallingPermission(permission, message);
}

ECode CPinyinIME::IsRestricted(
    /* [out] */ Boolean* isRestricted)
{
    return InputMethodService::IsRestricted(isRestricted);
}

ECode CPinyinIME::OpenFileInput(
    /* [in] */ const String& name,
    /* [out] */ IFileInputStream** fileInputStream)
{
    return InputMethodService::OpenFileInput(name, fileInputStream);
}

ECode CPinyinIME::GetExternalCacheDir(
    /* [out] */ IFile** externalDir)
{
    return InputMethodService::GetExternalCacheDir(externalDir);
}

ECode CPinyinIME::StartActivity(
    /* [in] */ IIntent* intent,
    /* [in] */ IBundle* options)
{
    return InputMethodService::StartActivity(intent, options);
}

ECode CPinyinIME::StartActivities(
    /* [in] */ ArrayOf<IIntent*>* intents)
{
    return InputMethodService::StartActivities(intents);
}

ECode CPinyinIME::StartActivitiesAsUser(
    /* [in] */ ArrayOf<IIntent*>* intents,
    /* [in] */ IBundle* options,
    /* [in] */ IUserHandle* userHandle)
{
    return InputMethodService::StartActivitiesAsUser(intents, options, userHandle);
}

ECode CPinyinIME::RegisterReceiverAsUser(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IUserHandle* user,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ const String& broadcastPermission,
    /* [in] */ IHandler* scheduler,
    /* [out] */ IIntent** stickyIntent)
{
    return InputMethodService::RegisterReceiverAsUser(receiver, user, filter, broadcastPermission, scheduler, stickyIntent);
}

ECode CPinyinIME::StartServiceAsUser(
    /* [in] */ IIntent* service,
    /* [in] */ IUserHandle* user,
    /* [out] */ IComponentName** name)
{
    return InputMethodService::StartServiceAsUser(service, user, name);
}

ECode CPinyinIME::StopServiceAsUser(
    /* [in] */ IIntent* service,
    /* [in] */ IUserHandle* user,
    /* [out] */ Boolean* succeeded)
{
    return InputMethodService::StopServiceAsUser(service, user, succeeded);
}

ECode CPinyinIME::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ Elastos::Droid::Content::IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* succeeded)
{
    return InputMethodService::BindService(service, conn, flags, userHandle, succeeded);
}

ECode CPinyinIME::CheckCallingUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* result)
{
    return InputMethodService::CheckCallingUriPermission(uri, modeFlags, result);
}

ECode CPinyinIME::CheckCallingOrSelfUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [out] */ Int32* result)
{
    return InputMethodService::CheckCallingOrSelfUriPermission(uri, modeFlags, result);
}

ECode CPinyinIME::EnforceUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return InputMethodService::EnforceUriPermission(uri, pid, uid, modeFlags, message);
}

ECode CPinyinIME::EnforceCallingUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return InputMethodService::EnforceCallingUriPermission(uri, modeFlags, message);
}

ECode CPinyinIME::EnforceCallingOrSelfUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return InputMethodService::EnforceCallingOrSelfUriPermission(uri, modeFlags, message);
}

ECode CPinyinIME::EnforceUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ const String& readPermission,
    /* [in] */ const String& writePermission,
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 modeFlags,
    /* [in] */ const String& message)
{
    return InputMethodService::EnforceUriPermission(uri, readPermission, writePermission, pid, uid, modeFlags, message);
}

ECode CPinyinIME::CreatePackageContextAsUser(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 flags,
    /* [in] */ IUserHandle* user,
    /* [out] */ IContext** ctx)
{
    return InputMethodService::CreatePackageContextAsUser(packageName, flags, user, ctx);
}

ECode CPinyinIME::CreateConfigurationContext(
    /* [in] */ IConfiguration* overrideConfiguration,
    /* [out] */ IContext** ctx)
{
    return InputMethodService::CreateConfigurationContext(overrideConfiguration, ctx);
}

ECode CPinyinIME::CreateDisplayContext(
    /* [in] */ IDisplay* display,
    /* [out] */ IContext** ctx)
{
    return InputMethodService::CreateDisplayContext(display, ctx);
}

ECode CPinyinIME::GetCompatibilityInfo(
    /* [in] */ Int32 displayId,
    /* [out] */ ICompatibilityInfoHolder** infoHolder)
{
    return InputMethodService::GetCompatibilityInfo(displayId, infoHolder);
}

ECode CPinyinIME::GetApplication(
    /* [out] */ IApplication** application)
{
    return InputMethodService::GetApplication(application);
}

ECode CPinyinIME::OnTaskRemoved(
    /* [in] */ IIntent* rootIntent)
{
    return InputMethodService::OnTaskRemoved(rootIntent);
}

ECode CPinyinIME::RegisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IIntentFilter* filter,
    /* [out] */ IIntent** intent)
{
    return InputMethodService::RegisterReceiver(receiver, filter, intent);
}

ECode CPinyinIME::RegisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ const String& broadcastPermission,
    /* [in] */ IHandler* scheduler,
    /* [out] */ IIntent** intent)
{
    return InputMethodService::RegisterReceiver(receiver, filter, broadcastPermission, scheduler, intent);
}

ECode CPinyinIME::StopSelf()
{
    return InputMethodService::StopSelf();
}

ECode CPinyinIME::StopSelf(
    /* [in] */ Int32 startId)
{
    return InputMethodService::StopSelf(startId);
}

ECode CPinyinIME::StopSelfResult(
    /* [in] */ Int32 startId,
    /* [out] */ Boolean* res)
{
    return InputMethodService::StopSelfResult(startId, res);
}

ECode CPinyinIME::SetForeground(
    /* [in] */ Boolean isForeground)
{
    return InputMethodService::SetForeground(isForeground);
}

ECode CPinyinIME::StartForeground(
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification)
{
    return InputMethodService::StartForeground(id, notification);
}

ECode CPinyinIME::StopForeground(
    /* [in] */ Boolean removeNotification)
{
    return InputMethodService::StopForeground(removeNotification);
}

ECode CPinyinIME::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* state)
{
    return InputMethodService::OnGenericMotionEvent(event, state);
}

ECode CPinyinIME::OnViewClicked(
    /* [in] */ Boolean focusChanged)
{
    return InputMethodService::OnViewClicked(focusChanged);
}

ECode CPinyinIME::EnableHardwareAcceleration(
    /* [out] */ Boolean* enable)
{
    return InputMethodService::EnableHardwareAcceleration(enable);
}

ECode CPinyinIME::SetBackDisposition(
    /* [in] */ Int32 disposition)
{
    return InputMethodService::SetBackDisposition(disposition);
}

ECode CPinyinIME::GetBackDisposition(
    /* [out] */ Int32* pos)
{
    return InputMethodService::GetBackDisposition(pos);
}

ECode CPinyinIME::OnExtractedReplaceText(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ICharSequence* text)
{
    return InputMethodService::OnExtractedReplaceText(start, end, text);
}

ECode CPinyinIME::OnExtractedDeleteText(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return InputMethodService::OnExtractedDeleteText(start, end);
}

ECode CPinyinIME::OnExtractedSetSpan(
    /* [in] */ IInterface* span,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 flags)
{
    return InputMethodService::OnExtractedSetSpan(span, start, end, flags);
}

ECode CPinyinIME::OnKeyLongPress(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    return InputMethodService::OnKeyLongPress(keyCode, event, result);
}

ECode CPinyinIME::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 count,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    return InputMethodService::OnKeyMultiple(keyCode, count, event, result);
}

ECode CPinyinIME::UnregisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver)
{
    return InputMethodService::UnregisterReceiver(receiver);
}

} // namespace PinyinIME
} // namespace DevSamples
} // namespace Droid
} // namespace Elastos
