
#ifndef __ELASTOS_DROID_APP_VOICE_INTERACTOR_H__
#define __ELASTOS_DROID_APP_VOICE_INTERACTOR_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Utility::IArrayMap;
using Elastos::Droid::Internal::App::IIVoiceInteractor;
using Elastos::Droid::Internal::App::IIVoiceInteractorCallback;
using Elastos::Droid::Internal::App::IIVoiceInteractorRequest;
using Elastos::Droid::Internal::Os::HandlerCaller;
using Elastos::Droid::Internal::Os::SomeArgs;

using Elastos::Utility::IArrayList;

namespace Elastos {
namespace Droid {
namespace App {

/**
 * @hide
 * Interface for an {@link Activity} to interact with the user through voice.  Use
 * {@link android.app.Activity#getVoiceInteractor() Activity.getVoiceInteractor}
 * to retrieve the interface, if the activity is currently involved in a voice interaction.
 *
 * <p>The voice interactor revolves around submitting voice interaction requests to the
 * back-end voice interaction service that is working with the user.  These requests are
 * submitted with {@link #submitRequest}, providing a new instance of a
 * {@link Request} subclass describing the type of operation to perform -- currently the
 * possible requests are {@link ConfirmationRequest} and {@link CommandRequest}.
 *
 * <p>Once a request is submitted, the voice system will process it and eventually deliver
 * the result to the request object.  The application can cancel a pending request at any
 * time.
 *
 * <p>The VoiceInteractor is integrated with Activity's state saving mechanism, so that
 * if an activity is being restarted with retained state, it will retain the current
 * VoiceInteractor and any outstanding requests.  Because of this, you should always use
 * {@link Request#getActivity() Request.getActivity} to get back to the activity of a
 * request, rather than holding on to the activity instance yourself, either explicitly
 * or implicitly through a non-static inner class.
 */
//@SystemApi
class VoiceInteractor
    : public Object
    , public IVoiceInteractor
{
private:
    class HandlerCallerCallback
        : public Object
        , public IHandlerCallerCallback
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI ExecuteMessage(
            /* [in] */ IMessage* msg)
        {
            SomeArgs args = (SomeArgs)msg.obj;
            Request request;
            switch (msg.what) {
                case MSG_CONFIRMATION_RESULT:
                    request = pullRequest((IVoiceInteractorRequest)args.arg1, TRUE);
                    if (DEBUG) Log.d(TAG, "onConfirmResult: req="
                            + ((IVoiceInteractorRequest)args.arg1).asBinder() + "/" + request
                            + " confirmed=" + msg.arg1 + " result=" + args.arg2);
                    if (request != NULL) {
                        ((ConfirmationRequest)request).onConfirmationResult(msg.arg1 != 0,
                                (Bundle) args.arg2);
                        request.clear();
                    }
                    break;
                case MSG_COMPLETE_VOICE_RESULT:
                    request = pullRequest((IVoiceInteractorRequest)args.arg1, TRUE);
                    if (DEBUG) Log.d(TAG, "onCompleteVoice: req="
                            + ((IVoiceInteractorRequest)args.arg1).asBinder() + "/" + request
                            + " result=" + args.arg1);
                    if (request != NULL) {
                        ((CompleteVoiceRequest)request).onCompleteResult((Bundle) args.arg2);
                        request.clear();
                    }
                    break;
                case MSG_ABORT_VOICE_RESULT:
                    request = pullRequest((IVoiceInteractorRequest)args.arg1, TRUE);
                    if (DEBUG) Log.d(TAG, "onAbortVoice: req="
                            + ((IVoiceInteractorRequest)args.arg1).asBinder() + "/" + request
                            + " result=" + args.arg1);
                    if (request != NULL) {
                        ((AbortVoiceRequest)request).onAbortResult((Bundle) args.arg2);
                        request.clear();
                    }
                    break;
                case MSG_COMMAND_RESULT:
                    request = pullRequest((IVoiceInteractorRequest)args.arg1, msg.arg1 != 0);
                    if (DEBUG) Log.d(TAG, "onCommandResult: req="
                            + ((IVoiceInteractorRequest)args.arg1).asBinder() + "/" + request
                            + " result=" + args.arg2);
                    if (request != NULL) {
                        ((CommandRequest)request).onCommandResult((Bundle) args.arg2);
                        if (msg.arg1 != 0) {
                            request.clear();
                        }
                    }
                    break;
                case MSG_CANCEL_RESULT:
                    request = pullRequest((IVoiceInteractorRequest)args.arg1, TRUE);
                    if (DEBUG) Log.d(TAG, "onCancelResult: req="
                            + ((IVoiceInteractorRequest)args.arg1).asBinder() + "/" + request);
                    if (request != NULL) {
                        request.onCancel();
                        request.clear();
                    }
                    break;
            }
        }
    };

    class VoiceInteractorCallback
        : public Object
        , public IIVoiceInteractorCallback
        , public IBinder
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI constructor();

        //@Override
        CARAPI DeliverConfirmationResult(
            /* [in] */ IIVoiceInteractorRequest* request,
            /* [in] */ Boolean confirmed,
            /* [in] */ IBundle* result)
        {
            mHandlerCaller.sendMessage(mHandlerCaller.obtainMessageIOO(
                    MSG_CONFIRMATION_RESULT, confirmed ? 1 : 0, request, result));
        }

        //@Override
        CARAPI DeliverCompleteVoiceResult(
            /* [in] */ IIVoiceInteractorRequest* request,
            /* [in] */ IBundle* result)
        {
            mHandlerCaller.sendMessage(mHandlerCaller.obtainMessageOO(
                    MSG_COMPLETE_VOICE_RESULT, request, result));
        }

        //@Override
        CARAPI DeliverAbortVoiceResult(
            /* [in] */ IIVoiceInteractorRequest* request,
            /* [in] */ IBundle* result)
        {
            mHandlerCaller.sendMessage(mHandlerCaller.obtainMessageOO(
                    MSG_ABORT_VOICE_RESULT, request, result));
        }

        //@Override
        CARAPI DeliverCommandResult(
            /* [in] */ IIVoiceInteractorRequest* request,
            /* [in] */ Boolean complete,
            /* [in] */ IBundle* result)
        {
            mHandlerCaller.sendMessage(mHandlerCaller.obtainMessageIOO(
                    MSG_COMMAND_RESULT, complete ? 1 : 0, request, result));
        }

        //@Override
        CARAPI DeliverCancel(
            /* [in] */ IIVoiceInteractorRequest* request)
        {
            mHandlerCaller.sendMessage(mHandlerCaller.obtainMessageO(
                    MSG_CANCEL_RESULT, request));
        }
    };

    class Request
        : public Object
        , public IIVoiceInteractorRequest
    {
    public:
        CAR_INTERFACE_DECL()

        Request();

        CARAPI constructor();

        CARAPI Cancel()
        {
            try {
                mRequestInterface.cancel();
            } catch (RemoteException e) {
                Log.w(TAG, "Voice interactor has died", e);
            }
        }

        CARAPI GetContext(
            /* [out] */ IContext** ctx)
        {
            return mContext;
        }

        CARAPI GetActivity(
            /* [out] */ IActivity** activity)
        {
            return mActivity;
        }

        CARAPI OnCancel()
        {
        }

        CARAPI OnAttached(
            /* [in] */ IActivity* activity)
        {
        }

        CARAPI OnDetached()
        {
        }

    protected:
        CARAPI Clear()
        {
            mRequestInterface = NULL;
            mContext = NULL;
            mActivity = NULL;
            return NOERROR;
        }

        virtual CARAPI Submit(
            /* [in] */ IIVoiceInteractor* interactor,
            /* [in] */ const String& packageName,
            /* [in] */ IIVoiceInteractorCallback* callback,
            /* [out] */ IIVoiceInteractorRequest** request) = 0;

    private:
        AutoPtr<IIVoiceInteractorRequest> mRequestInterface;
        AutoPtr<IContext> mContext;
        AutoPtr<IActivity> mActivity;
    };

    class ConfirmationRequest
        : public Request
        , public IConfirmationRequest
    {
    public:
        CAR_INTERFACE_DECL();

        /**
         * Confirms an operation with the user via the trusted system
         * VoiceInteractionService.  This allows an Activity to complete an unsafe operation that
         * would require the user to touch the screen when voice interaction mode is not enabled.
         * The result of the confirmation will be returned through an asynchronous call to
         * either {@link #onConfirmationResult(Boolean, android.os.Bundle)} or
         * {@link #onCancel()}.
         *
         * <p>In some cases this may be a simple yes / no confirmation or the confirmation could
         * include context information about how the action will be completed
         * (e.g. booking a cab might include details about how long until the cab arrives)
         * so the user can give a confirmation.
         * @param prompt Optional confirmation text to read to the user as the action being
         * confirmed.
         * @param extras Additional optional information.
         */
        CARAPI constructor(
            /* [in] */ ICharSequence* prompt,
            /* [in] */ IBundle* extras)
        {
            mPrompt = prompt;
            mExtras = extras;
        }

        CARAPI OnConfirmationResult(
            /* [in] */ Boolean confirmed,
            /* [in] */ IBundle* result)
        {
        }

        CARAPI Submit(
            /* [in] */ IIVoiceInteractor* interactor,
            /* [in] */ const String& packageName,
            /* [in] */ IIVoiceInteractorCallback* callback,
            /* [out] */ IIVoiceInteractorRequest** request)
        {
            return interactor.startConfirmation(packageName, callback, mPrompt, mExtras);
        }
    private:
        AutoPtr<ICharSequence> mPrompt;
        AutoPtr<IBundle> mExtras;
    };

    class CompleteVoiceRequest
        : public Request
        , public ICompleteVoiceRequest
    {
    public:
        CAR_INTERFACE_DECL();

        /**
         * Reports that the current interaction was successfully completed with voice, so the
         * application can report the final status to the user. When the response comes back, the
         * voice system has handled the request and is ready to switch; at that point the
         * application can start a new non-voice activity or finish.  Be sure when starting the new
         * activity to use {@link android.content.Intent#FLAG_ACTIVITY_NEW_TASK
         * Intent.FLAG_ACTIVITY_NEW_TASK} to keep the new activity out of the current voice
         * interaction task.
         *
         * @param message Optional message to tell user about the completion status of the task.
         * @param extras Additional optional information.
         */
        CARAPI constructor(
            /* [in] */ ICharSequence* message,
            /* [in] */ IBundle extras) {
            mMessage = message;
            mExtras = extras;
        }

        CARAPI OnCompleteResult(
            /* [in] */ IBundle* result)
        {
        }

        CARAPI Submit(
            /* [in] */ IIVoiceInteractor* interactor,
            /* [in] */ const String& packageName,
            /* [in] */ IIVoiceInteractorCallback* callback,
            /* [out] */ IIVoiceInteractorRequest** request)
        {
            return interactor.startCompleteVoice(packageName, callback, mMessage, mExtras);
        }

    private:
        AutoPtr<ICharSequence> mMessage;
        AutoPtr<IBundle> mExtras;
    };

    class CompleteVoiceRequest
        : public Request
        , public IAbortVoiceRequest
    {
    public:
        CAR_INTERFACE_DECL();

        /**
         * Reports that the current interaction can not be complete with voice, so the
         * application will need to switch to a traditional input UI.  Applications should
         * only use this when they need to completely bail out of the voice interaction
         * and switch to a traditional UI.  When the response comes back, the voice
         * system has handled the request and is ready to switch; at that point the application
         * can start a new non-voice activity.  Be sure when starting the new activity
         * to use {@link android.content.Intent#FLAG_ACTIVITY_NEW_TASK
         * Intent.FLAG_ACTIVITY_NEW_TASK} to keep the new activity out of the current voice
         * interaction task.
         *
         * @param message Optional message to tell user about not being able to complete
         * the interaction with voice.
         * @param extras Additional optional information.
         */
        CARAPI constructor(
            /* [in] */ ICharSequence* message,
            /* [in] */ IBundle* extras)
        {
            mMessage = message;
            mExtras = extras;
        }

        CARAPI OnAbortResult(Bundle result) {
        }

        CARAPI Submit(
            /* [in] */ IIVoiceInteractor* interactor,
            /* [in] */ const String& packageName,
            /* [in] */ IIVoiceInteractorCallback* callback,
            /* [out] */ IIVoiceInteractorRequest** request)
        {
            return interactor.startAbortVoice(packageName, callback, mMessage, mExtras);
        }

    private:
        AutoPtr<ICharSequence> mMessage;
        AutoPtr<IBundle> mExtras;
    };

    class CommandRequest
        : public Request
        , public ICommandRequest
    {
    public:
        CAR_INTERFACE_DECL();

        /**
         * Execute a command using the trusted system VoiceInteractionService.
         * This allows an Activity to request additional information from the user needed to
         * complete an action (e.g. booking a table might have several possible times that the
         * user could select from or an app might need the user to agree to a terms of service).
         * The result of the confirmation will be returned through an asynchronous call to
         * either {@link #onCommandResult(android.os.Bundle)} or
         * {@link #onCancel()}.
         *
         * <p>The command is a string that describes the generic operation to be performed.
         * The command will determine how the properties in extras are interpreted and the set of
         * available commands is expected to grow over time.  An example might be
         * "com.google.voice.commands.REQUEST_NUMBER_BAGS" to request the number of bags as part of
         * airline check-in.  (This is not an actual working example.)
         *
         * @param command The desired command to perform.
         * @param args Additional arguments to control execution of the command.
         */
        CARAPI constructor(
            /* [in] */ const String& command,
            /* [in] */ IBundle* args)
        {
            mCommand = command;
            mArgs = args;
        }

        CARAPI OnCommandResult(
            /* [in] */ IBundle* result)
        {
        }

        CARAPI Submit(
            /* [in] */ IIVoiceInteractor* interactor,
            /* [in] */ const String& packageName,
            /* [in] */ IIVoiceInteractorCallback* callback,
            /* [out] */ IIVoiceInteractorRequest** request)
        {
            return interactor.startCommand(packageName, callback, mCommand, mArgs);
        }

    private:
        String mCommand;
        AutoPtr<IBundle> mArgs;
   };

public:
    CAR_INTERFACE_DECL()

    VoiceInteractor();

    virtual ~VoiceInteractor();

    CARAPI constructor(
        /* [in] */ IIVoiceInteractor* interactor,
        /* [in] */ IContext* context,
        /* [in] */ IActivity* activity,
        /* [in] */ ILooper* looper)
    {
        mInteractor = interactor;
        mContext = context;
        mActivity = activity;
        mHandlerCaller = new HandlerCaller(context, looper, mHandlerCallerCallback, TRUE);
    }

    AutoPtr<IVoiceInteractorRequest> PullRequest(
        /* [in] */ IIVoiceInteractorRequest* request,
        /* [in] */ Boolean complete)
    {
        synchronized (mActiveRequests) {
            Request req = mActiveRequests.get(request.asBinder());
            if (req != NULL && complete) {
                mActiveRequests.remove(request.asBinder());
            }
            return req;
        }
    }

    AutoPtr<IArrayList> MakeRequestList()
    {
        final Int32 N = mActiveRequests.size();
        if (N < 1) {
            return NULL;
        }
        ArrayList<Request> list = new ArrayList<Request>(N);
        for (Int32 i=0; i<N; i++) {
            list.add(mActiveRequests.valueAt(i));
        }
        return list;
    }

    CARAPI AttachActivity(Activity activity)
    {
        if (mActivity == activity) {
            return;
        }
        mContext = activity;
        mActivity = activity;
        ArrayList<Request> reqs = makeRequestList();
        if (reqs != NULL) {
            for (Int32 i=0; i<reqs.size(); i++) {
                Request req = reqs.get(i);
                req.mContext = activity;
                req.mActivity = activity;
                req.onAttached(activity);
            }
        }
    }

    CARAPI DetachActivity()
    {
        ArrayList<Request> reqs = makeRequestList();
        if (reqs != NULL) {
            for (Int32 i=0; i<reqs.size(); i++) {
                Request req = reqs.get(i);
                req.onDetached();
                req.mActivity = NULL;
                req.mContext = NULL;
            }
        }
        mContext = NULL;
        mActivity = NULL;
    }

    public CARAPI SubmitRequest(
        /* [in] */ IVoiceInteractorRequest* request,
        /* [out] */ Boolean* result)
    {
        try {
            AutoPtr<IIVoiceInteractorRequest> ireq = request.submit(mInteractor,
                    mContext.getOpPackageName(), mCallback);
            request.mRequestInterface = ireq;
            request.mContext = mContext;
            request.mActivity = mActivity;
            synchronized (mActiveRequests) {
                mActiveRequests.put(ireq.asBinder(), request);
            }
            return TRUE;
        } catch (RemoteException e) {
            Log.w(TAG, "Remove voice interactor service died", e);
            return false;
        }
    }

    /**
     * Queries the supported commands available from the VoiceinteractionService.
     * The command is a string that describes the generic operation to be performed.
     * An example might be "com.google.voice.commands.REQUEST_NUMBER_BAGS" to request the number
     * of bags as part of airline check-in.  (This is not an actual working example.)
     *
     * @param commands
     */
    public CARAPI SupportsCommands(
        /* [in] */ ArrayOf<String>* commands,
        /* [out, callee] */ ArrayOf<String>** results)
    {
        try {
            Boolean[] res = mInteractor.supportsCommands(mContext.getOpPackageName(), commands);
            if (DEBUG) Log.d(TAG, "supportsCommands: cmds=" + commands + " res=" + res);
            return res;
        } catch (RemoteException e) {
            throw new RuntimeException("Voice interactor has died", e);
        }
    }

private:
    static const String TAG = "VoiceInteractor";
    static const Boolean DEBUG = TRUE;

    AutoPtr<IIVoiceInteractor> mInteractor;

    AutoPtr<IContext> mContext;
    AutoPtr<IActivity> mActivity;

    AutoPtr<IHandlerCaller> mHandlerCaller;
    AutoPtr<IHandlerCallerCallback> mHandlerCallerCallback;

    AutoPtr<IIVoiceInteractorCallback> mCallback;

    HashMap<AutoPtr<IBinder>, AutoPtr<Request> > mActiveRequests;

    static const Int32 MSG_CONFIRMATION_RESULT = 1;
    static const Int32 MSG_COMPLETE_VOICE_RESULT = 2;
    static const Int32 MSG_ABORT_VOICE_RESULT = 3;
    static const Int32 MSG_COMMAND_RESULT = 4;
    static const Int32 MSG_CANCEL_RESULT = 5;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_VOICE_INTERACTOR_H__

