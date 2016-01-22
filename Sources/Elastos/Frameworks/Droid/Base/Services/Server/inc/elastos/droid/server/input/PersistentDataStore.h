#ifndef __ELASTOS_DROID_Server_Input_PersistentDataStore_H__
#define __ELASTOS_DROID_Server_Input_PersistentDataStore_H__

#include "_Elastos.Droid.Server.h"
#include "elastos/core/Object.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Droid::Hardware::Input::ITouchCalibration;
using Elastos::Droid::Utility::IAtomicFile;
using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::IArrayList;
using Org::Xmlpull::V1::IXmlPullParser;
using Org::Xmlpull::V1::IXmlSerializer;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Input {

class PersistentDataStore
    : public Object
{
public:

    class InputDeviceState
        : public Object
    {
    public:
        static AutoPtr<ArrayOf<String> > CALIBRATION_NAME;
        static ECode initThis;              // initialize this class's static member through initialize this variable

    private:
        AutoPtr<ArrayOf<ITouchCalibration*> > mTouchCalibration;
        String mCurrentKeyboardLayout;
        AutoPtr<IArrayList> mKeyboardLayouts;

    public:
        InputDeviceState();

        static CARAPI InitializeClass();

        ITouchCalibration* GetTouchCalibration(Int32 surfaceRotation);

        Boolean SetTouchCalibration(Int32 surfaceRotation, ITouchCalibration* calibration);

        String GetCurrentKeyboardLayout();

        Boolean SetCurrentKeyboardLayout(const String& keyboardLayout);

        ArrayOf<ICharSequence*>* GetKeyboardLayouts();

        Boolean AddKeyboardLayout(const String& keyboardLayout);

        Boolean RemoveKeyboardLayout(const String& keyboardLayout);

        void UpdateCurrentKeyboardLayoutIfRemoved(
                const String& removedKeyboardLayout, Int32 removedIndex);

        Boolean SwitchKeyboardLayout(Int32 direction);

        Boolean RemoveUninstalledKeyboardLayouts(ArrayOf<String>* availableKeyboardLayouts);

        // throws IOException, XmlPullParserException
        CARAPI LoadFromXml(IXmlPullParser* parser);

        // throws IOException
        CARAPI SaveToXml(IXmlSerializer* serializer);

        CARAPI SurfaceRotationToString(Int32 surfaceRotation, String* str);

        CARAPI StringToSurfaceRotation(const String& str, Int32 *r);
    };

public:

    PersistentDataStore();

    CARAPI SaveIfNeeded();

    CARAPI GetTouchCalibration(
        /* [in] */ const String& inputDeviceDescriptor,
        /* [in] */ Int32 surfaceRotation,
        /* [out] */ ITouchCalibration** touchCalibration);

    CARAPI SetTouchCalibration(
        /* [in] */ const String& inputDeviceDescriptor,
        /* [in] */ Int32 surfaceRotation,
        /* [in] */ ITouchCalibration* calibration,
        /* [out] */ Boolean* setTouchCalibration);

    CARAPI GetCurrentKeyboardLayout(
        /* [in] */ const String& inputDeviceDescriptor,
        /* [out] */ String* keyboardLayout);

    CARAPI SetCurrentKeyboardLayout(
        /* [in] */ const String& inputDeviceDescriptor,
        /* [in] */ const String& keyboardLayoutDescriptor,
        /* [out] */ Boolean* setIt);

    CARAPI GetKeyboardLayouts(
        /* [in] */ const String& inputDeviceDescriptor,
        /* [out] */ ArrayOf<ICharSequence*>** keyboardLayouts);

    CARAPI AddKeyboardLayout(
        /* [in] */ const String& inputDeviceDescriptor,
        /* [in] */ const String& keyboardLayoutDescriptor,
        /* [out] */ Boolean* addIt);

    CARAPI RemoveKeyboardLayout(
        /* [in] */ const String& inputDeviceDescriptor,
        /* [in] */ const String& keyboardLayoutDescriptor,
        /* [out] */ Boolean* removeIt);

    CARAPI SwitchKeyboardLayout(
        /* [in] */ const String& inputDeviceDescriptor,
        /* [in] */ Int32 direction,
        /* [out] */ Boolean* switchIt);

    CARAPI RemoveUninstalledKeyboardLayouts(
        /* [in] */ ArrayOf<String>* availableKeyboardLayouts,
        /* [out] */ Boolean* removeIt);

private:
    InputDeviceState* GetInputDeviceState(
        /* [in] */ const String& inputDeviceDescriptor,
        /* [in] */ Boolean createIfAbsent);

    void LoadIfNeeded();

    void SetDirty();

    void ClearState();

    void Load();

    void Save();

    // throws IOException, XmlPullParserException
    ECode LoadFromXml(IXmlPullParser* parser);

    // throws IOException, XmlPullParserException
    ECode LoadInputDevicesFromXml(IXmlPullParser* parser);

    //  throws IOException
    ECode SaveToXml(IXmlSerializer* serializer);

private:
    static String TAG;          // = "InputManager";

    // Input device state by descriptor.
    AutoPtr<HashMap<String, AutoPtr<InputDeviceState> > > mInputDevices;
    AutoPtr<IAtomicFile> mAtomicFile;

    // True if the data has been loaded.
    Boolean mLoaded;

    // True if there are changes to be saved.
    Boolean mDirty;

};

} // Input
} // Server
} // Droid
} // Elastos

#endif // __ELASTOS_DROID_Server_Input_PersistentDataStore_H__


