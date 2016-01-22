#include "elastos/droid/server/input/PersistentDataStore.h"
#include "elastos/droid/server/input/InputManagerService.h"
//#include "elastos/droid/utility/CAtomicFile.h"
#include "Elastos.Droid.Utility.h"
#include "elastos/droid/hardware/input/TouchCalibration.h"
#include <libcore/utility/EmptyArray.h>
#include <Elastos.CoreLibrary.Utility.h>
#include "Elastos.CoreLibrary.External.h"
#include "Elastos.CoreLibrary.IO.h"
#include "Elastos.CoreLibrary.Libcore.h"
#include "Elastos.Droid.Internal.h"
#include "Elastos.Droid.Utility.h"
#include "Elastos.CoreLibrary.Extensions.h"
#include "elastos/droid/internal/utility/XmlUtils.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringUtils.h>

//using Elastos::Droid::Hardware::Input;
using Elastos::Core::IComparable;
using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Core::StringUtils;
using Elastos::IO::CBufferedInputStream;
using Elastos::IO::CBufferedInputStream;
using Elastos::IO::CFile;
using Elastos::IO::IFile;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::IBufferedInputStream;
using Elastos::IO::CBufferedInputStream;
using Elastos::IO::ICloseable;
using Libcore::IO::IIoUtils;
using Libcore::IO::CIoUtils;
using Libcore::Utility::EmptyArray;
using Elastos::Droid::Internal::Utility::CFastXmlSerializer;
using Elastos::Droid::Internal::Utility::IFastXmlSerializer;
using Elastos::Droid::Internal::Utility::XmlUtils;
using Elastos::Droid::Utility::CAtomicFile;
using Elastos::Droid::Hardware::Input::TouchCalibration;
using Elastos::Droid::Hardware::Input::ITouchCalibration;
using Elastos::Droid::View::ISurface;
using Elastos::Utility::CArrayList;
using Elastos::Utility::CCollections;
using Elastos::Utility::ICollections;
using Elastos::Utility::Logging::Slogger;
using Org::Xml::Sax::CInputSource;
using Org::Xml::Sax::Helpers::CDefaultHandler;
using Org::Xmlpull::V1::IXmlPullParserFactory;
using Org::Xmlpull::V1::IXmlPullParserFactoryHelper;
using Org::Xmlpull::V1::CXmlPullParserFactoryHelper;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Input {

String PersistentDataStore::TAG("InputManager");

PersistentDataStore::PersistentDataStore()
{
    AutoPtr<IFile> ifile;

    CFile::New(String("/data/system/input-manager-state.xml"), (IFile**)&ifile);

    CAtomicFile::New(ifile, (IAtomicFile**)&mAtomicFile);
}

ECode PersistentDataStore::SaveIfNeeded()
{
    if (mDirty) {
        Save();
        mDirty = FALSE;
    }

    return NOERROR;
}

ECode PersistentDataStore::GetTouchCalibration(
    /* [in] */ const String& inputDeviceDescriptor,
    /* [in] */ Int32 surfaceRotation,
    /* [out] */ ITouchCalibration** touchCalibration)
{
    InputDeviceState* state = GetInputDeviceState(inputDeviceDescriptor, FALSE);
    if (state == NULL) {
        *touchCalibration = TouchCalibration::IDENTITY;
    }

    AutoPtr<ITouchCalibration> cal = state->GetTouchCalibration(surfaceRotation);
    if (cal == NULL) {
        *touchCalibration = TouchCalibration::IDENTITY;
        return NOERROR;
    }
    *touchCalibration = cal;
    return NOERROR;
}

ECode PersistentDataStore::SetTouchCalibration(
    /* [in] */ const String& inputDeviceDescriptor,
    /* [in] */ Int32 surfaceRotation,
    /* [in] */ ITouchCalibration* calibration,
    /* [out] */ Boolean* setTouchCalibration)
{
    InputDeviceState* state = GetInputDeviceState(inputDeviceDescriptor, TRUE);

    if (state->SetTouchCalibration(surfaceRotation, calibration)) {
        SetDirty();
        return TRUE;
    }

    return FALSE;
}

ECode PersistentDataStore::GetCurrentKeyboardLayout(
    /* [in] */ const String& inputDeviceDescriptor,
    /* [out] */ String* keyboardLayout)
{
    InputDeviceState* state = GetInputDeviceState(inputDeviceDescriptor, FALSE);
    if (state != NULL) {
        *keyboardLayout = state->GetCurrentKeyboardLayout();
    } else {
        *keyboardLayout = NULL;
    }
    return NOERROR;

}

ECode PersistentDataStore::SetCurrentKeyboardLayout(
    /* [in] */ const String& inputDeviceDescriptor,
    /* [in] */ const String& keyboardLayoutDescriptor,
    /* [out] */ Boolean* setIt)
{
    InputDeviceState* state = GetInputDeviceState(inputDeviceDescriptor, TRUE);
    if (state->SetCurrentKeyboardLayout(keyboardLayoutDescriptor)) {
        SetDirty();
        *setIt = TRUE;
        return NOERROR;
    }
    *setIt = FALSE;
    return NOERROR;
}

ECode PersistentDataStore::GetKeyboardLayouts(
    /* [in] */ const String& inputDeviceDescriptor,
    /* [out] */ ArrayOf<ICharSequence*>** keyboardLayouts)
{
    AutoPtr<ArrayOf<ICharSequence*> > array;

    array = ArrayOf<ICharSequence*>::Alloc(0);

    InputDeviceState* state = GetInputDeviceState(inputDeviceDescriptor, FALSE);
    if (state == NULL) {
        *keyboardLayouts = array;
        return NOERROR;
    }
    *keyboardLayouts = state->GetKeyboardLayouts();
    return NOERROR;
}

ECode PersistentDataStore::AddKeyboardLayout(
    /* [in] */ const String& inputDeviceDescriptor,
    /* [in] */ const String& keyboardLayoutDescriptor,
    /* [out] */ Boolean* addIt)
{
    InputDeviceState* state = GetInputDeviceState(inputDeviceDescriptor, TRUE);
    if (state->AddKeyboardLayout(keyboardLayoutDescriptor)) {
        SetDirty();
        *addIt = TRUE;
        return NOERROR;
    }
    *addIt = FALSE;
    return NOERROR;
}

ECode PersistentDataStore::RemoveKeyboardLayout(
    /* [in] */ const String& inputDeviceDescriptor,
    /* [in] */ const String& keyboardLayoutDescriptor,
    /* [out] */ Boolean* removeIt)
{
    InputDeviceState* state = GetInputDeviceState(inputDeviceDescriptor, TRUE);
    if (state->RemoveKeyboardLayout(keyboardLayoutDescriptor)) {
        SetDirty();
        *removeIt = TRUE;
        return NOERROR;
    }
    *removeIt = FALSE;
    return NOERROR;
}

ECode PersistentDataStore::SwitchKeyboardLayout(
    /* [in] */ const String& inputDeviceDescriptor,
    /* [in] */ Int32 direction,
    /* [out] */ Boolean* switchIt)
{
    InputDeviceState* state = GetInputDeviceState(inputDeviceDescriptor, FALSE);
    if (state != NULL && state->SwitchKeyboardLayout(direction)) {
        SetDirty();
        *switchIt = TRUE;
        return NOERROR;
    }
    *switchIt = FALSE;
    return NOERROR;
}

ECode PersistentDataStore::RemoveUninstalledKeyboardLayouts(
    /* [in] */ ArrayOf<String>* availableKeyboardLayouts,
    /* [out] */ Boolean* removeIt)
{
    Boolean changed = FALSE;

    HashMap<String, AutoPtr<InputDeviceState> >::Iterator it = mInputDevices->Begin();
    for (; it != mInputDevices->End(); it++) {
        InputDeviceState* state = it->mSecond;
        if (state->RemoveUninstalledKeyboardLayouts(availableKeyboardLayouts)) {
            changed = TRUE;
        }
    }

    if (changed) {
        SetDirty();
        *removeIt = TRUE;
        return NOERROR;
    }
    *removeIt = FALSE;
    return NOERROR;
}

PersistentDataStore::InputDeviceState* PersistentDataStore::GetInputDeviceState(
    /* [in] */ const String& inputDeviceDescriptor,
    /* [in] */ Boolean createIfAbsent)
{
    LoadIfNeeded();
    HashMap<String, AutoPtr<InputDeviceState> >::Iterator it = mInputDevices->Find(inputDeviceDescriptor);
    AutoPtr<InputDeviceState> state = it->mSecond;
    //InputDeviceState* state = mInputDevices[inputDeviceDescriptor];
    if (state == NULL && createIfAbsent) {
        state = new InputDeviceState();
        (*mInputDevices)[inputDeviceDescriptor] = state;
        SetDirty();
    }
    return state;
}

void PersistentDataStore::LoadIfNeeded()
{
    if (!mLoaded) {
        Load();
        mLoaded = TRUE;
    }
}

void PersistentDataStore::SetDirty()
{
    mDirty = TRUE;
}

void PersistentDataStore::ClearState()
{
    mInputDevices->Clear();
}

void PersistentDataStore::Load()
{
    ClearState();

    AutoPtr<IInputStream> is;
    AutoPtr<IFileInputStream> stream;
    //try {
    mAtomicFile->OpenRead((IFileInputStream**)&stream);
    is = IInputStream::Probe(stream);
    //} catch (FileNotFoundException ex) {
    //    return;
    //}

    AutoPtr<IXmlPullParser> parser;
    //try {

    AutoPtr<IXmlPullParserFactoryHelper> helper;
    AutoPtr<IXmlPullParserFactory> factory;
    CXmlPullParserFactoryHelper::AcquireSingleton((IXmlPullParserFactoryHelper**)&helper);
    helper->NewInstance((IXmlPullParserFactory**)&factory);
    factory->SetNamespaceAware(TRUE);
    factory->NewPullParser((IXmlPullParser**)&parser);

    AutoPtr<IBufferedInputStream> bis;
    CBufferedInputStream::New(is, (IBufferedInputStream**)&bis);

    parser->SetInput(IInputStream::Probe(bis), (String)NULL);
    LoadFromXml(parser);
    /*
    } catch (IOException ex) {
        Slog.w(InputManagerService.TAG, "Failed to load input manager persistent store data.", ex);
        clearState();
    } catch (XmlPullParserException ex) {
        Slog.w(InputManagerService.TAG, "Failed to load input manager persistent store data.", ex);
        clearState();
    } finally {
    */
    AutoPtr<IIoUtils> ioUtils;
    CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
    AutoPtr<ICloseable> closeable = ICloseable::Probe(is);
    ioUtils->CloseQuietly(closeable);
}

void PersistentDataStore::Save()
{
    ECode ec;
    AutoPtr<IFileOutputStream> fos;

    mAtomicFile->StartWrite((IFileOutputStream**)&fos);

    Boolean success = FALSE;

    AutoPtr<IXmlSerializer> serializer;
    CFastXmlSerializer::New((IXmlSerializer**)&serializer);

    AutoPtr<IOutputStream> os;
    os = IOutputStream::Probe(fos);
    do {
        ec = serializer->SetOutput(os, String("utf-8"));
        if (FAILED(ec))
            break;

        ec = SaveToXml(serializer);
        if (FAILED(ec))
            break;

        ec = serializer->Flush();
        if (FAILED(ec))
            break;

        success = TRUE;
    } while (0);

    if (success) {
        ec = mAtomicFile->FinishWrite(IFileOutputStream::Probe(os));
    } else {
        ec = mAtomicFile->FailWrite(IFileOutputStream::Probe(os));
    }

    if (FAILED(ec)) {
        Slogger::W(InputManagerService::TAG, "Failed to save input manager persistent store data.");
    }
}

// throws IOException, XmlPullParserException
ECode PersistentDataStore::LoadFromXml(IXmlPullParser* parser)
{
    XmlUtils::BeginDocument(parser, String("input-manager-state"));

    Int32 outerDepth;
    parser->GetDepth(&outerDepth);

    while (XmlUtils::NextElementWithin(parser, outerDepth)) {
        String name;
        parser->GetName(&name);
        if (name.Equals("input-devices")) {
            return LoadInputDevicesFromXml(parser);
        }
    }

    return NOERROR;
}

// throws IOException, XmlPullParserException
ECode PersistentDataStore::LoadInputDevicesFromXml(IXmlPullParser* parser)
{
    Int32 outerDepth;
    parser->GetDepth(&outerDepth);

    while (XmlUtils::NextElementWithin(parser, outerDepth)) {

        String name;
        parser->GetName(&name);
        if (name.Equals("input-devices")) {

            String descriptor;
            parser->GetAttributeValue((String)NULL, String("descriptor"), &descriptor);
            if (descriptor == NULL) {
                //throw new XmlPullParserException(
                //        "Missing descriptor attribute on input-device.");
                return E_XML_PULL_PARSER_EXCEPTION;
            }

            HashMap<String, AutoPtr<InputDeviceState> >::Iterator it = mInputDevices->Find(descriptor);

            if (it->mSecond != NULL) {
                //throw new XmlPullParserException("Found duplicate input device.");
                return E_XML_PULL_PARSER_EXCEPTION;
            }

            InputDeviceState* state = new InputDeviceState();
            state->LoadFromXml(parser);
            (*mInputDevices)[descriptor] = state;
        }
    }

    return NOERROR;
}

//  throws IOException
ECode PersistentDataStore::SaveToXml(IXmlSerializer* serializer)
{
    serializer->StartDocument((String)NULL, TRUE);
    serializer->SetFeature(String("http://xmlpull.org/v1/doc/features.html#indent-output"), TRUE);
    serializer->WriteStartTag((String)NULL, String("input-manager-state"));
    serializer->WriteStartTag((String)NULL, String("input-devices"));


    HashMap<String, AutoPtr<InputDeviceState> >::Iterator it = mInputDevices->Begin();
    for (; it != mInputDevices->End(); it++) {
        InputDeviceState* state = it->mSecond;
        String descriptor = it->mFirst;

        serializer->WriteStartTag((String)NULL, String("input-device"));
        serializer->WriteAttribute((String)NULL, String("descriptor"), descriptor);
        state->SaveToXml(serializer);
        serializer->WriteEndTag((String)NULL, String("input-device"));
    }

    serializer->WriteEndTag((String)NULL, String("input-devices"));
    serializer->WriteEndTag((String)NULL, String("input-manager-state"));
    serializer->EndDocument();

    return NOERROR;
}


/**
 * PersistentDataStore::InputDeviceState
 *********************************************************************************************************/
#define NUM_mTouchCalibration   4

AutoPtr<ArrayOf<String> > PersistentDataStore::InputDeviceState::CALIBRATION_NAME = NULL;
ECode PersistentDataStore::InputDeviceState::initThis = PersistentDataStore::InputDeviceState::InitializeClass();

PersistentDataStore::InputDeviceState::InputDeviceState()
{
    //AutoPtr<ArrayOf<ITouchCalibration> > mTouchCalibration;
    mTouchCalibration = ArrayOf<ITouchCalibration*>::Alloc(NUM_mTouchCalibration);
    (*mTouchCalibration)[0] = new TouchCalibration();
    (*mTouchCalibration)[1] = new TouchCalibration();
    (*mTouchCalibration)[2] = new TouchCalibration();
    (*mTouchCalibration)[3] = new TouchCalibration();

    //AutoPtr<IArrayList> mKeyboardLayouts;
    CArrayList::New((IArrayList**)&mKeyboardLayouts);
}

ECode PersistentDataStore::InputDeviceState::InitializeClass()
{
    //const ArrayOf<String> CALIBRATION_NAME;
    CALIBRATION_NAME = ArrayOf<String>::Alloc(6);
    (*CALIBRATION_NAME)[0] = String("x_scale");
    (*CALIBRATION_NAME)[1] = String("x_ymix");
    (*CALIBRATION_NAME)[2] = String("x_offset");
    (*CALIBRATION_NAME)[3] = String("y_xmix");
    (*CALIBRATION_NAME)[4] = String("y_scale");
    (*CALIBRATION_NAME)[5] = String("y_offset");

    return NOERROR;
}

ITouchCalibration* PersistentDataStore::InputDeviceState::GetTouchCalibration(
    /* [in] */ Int32 surfaceRotation)
{
    if (surfaceRotation < NUM_mTouchCalibration) {
        return (*mTouchCalibration)[surfaceRotation];
    }

    Slogger::W(InputManagerService::TAG, "Cannot get touch calibration.");
    return NULL;
}

Boolean PersistentDataStore::InputDeviceState::SetTouchCalibration(Int32 surfaceRotation, ITouchCalibration* calibration)
{
    if (surfaceRotation < NUM_mTouchCalibration) {
        Int32 result;
        IComparable::Probe(calibration)->CompareTo((*mTouchCalibration)[surfaceRotation], &result);
        if (result == 0) {
            (*mTouchCalibration)[surfaceRotation] = calibration;
            return TRUE;
        }
        return FALSE;
    }

    Slogger::W(InputManagerService::TAG, "Cannot set touch calibration.");
    return FALSE;
}

String PersistentDataStore::InputDeviceState::GetCurrentKeyboardLayout()
{
    return mCurrentKeyboardLayout;
}

Boolean PersistentDataStore::InputDeviceState::SetCurrentKeyboardLayout(const String& keyboardLayout)
{
    if (mCurrentKeyboardLayout.Equals(keyboardLayout)) {
        return FALSE;
    }
    AddKeyboardLayout(keyboardLayout);
    mCurrentKeyboardLayout = keyboardLayout;
    return TRUE;
}

ArrayOf<ICharSequence*>* PersistentDataStore::InputDeviceState::GetKeyboardLayouts()
{
    Boolean empty;
    AutoPtr<ArrayOf<ICharSequence*> > temp;

    mKeyboardLayouts->IsEmpty(&empty);
    if (empty) {
        return ArrayOf<ICharSequence*>::Alloc(0);
    }
    mKeyboardLayouts->ToArray((ArrayOf<IInterface*>**)&temp);
    return temp;
}

Boolean PersistentDataStore::InputDeviceState::AddKeyboardLayout(const String& keyboardLayout)
{
    Int32 index;
    AutoPtr<ICollections> coll;
    CCollections::AcquireSingleton((ICollections**)&coll);
    AutoPtr<ICharSequence> csq;
    CString::New(keyboardLayout, (ICharSequence**)&csq);
    coll->BinarySearch(IList::Probe(mKeyboardLayouts), csq, &index);
    if (index >= 0) {
        return FALSE;
    }
    mKeyboardLayouts->Add(-index - 1, csq);
    if (mCurrentKeyboardLayout == NULL) {
        mCurrentKeyboardLayout = keyboardLayout;
    }
    return TRUE;
}

Boolean PersistentDataStore::InputDeviceState::RemoveKeyboardLayout(const String& keyboardLayout)
{
    Int32 index;
    AutoPtr<ICollections> coll;
    CCollections::AcquireSingleton((ICollections**)&coll);
    AutoPtr<ICharSequence> csq;
    CString::New(keyboardLayout, (ICharSequence**)&csq);
    coll->BinarySearch(IList::Probe(mKeyboardLayouts), csq, &index);
    if (index < 0) {
        return FALSE;
    }

    mKeyboardLayouts->Remove(index);
    UpdateCurrentKeyboardLayoutIfRemoved(keyboardLayout, index);
    return TRUE;
}

void PersistentDataStore::InputDeviceState::UpdateCurrentKeyboardLayoutIfRemoved(
        const String& removedKeyboardLayout, Int32 removedIndex)
{
    if (mCurrentKeyboardLayout.Equals(removedKeyboardLayout)) {

        Boolean empty;
        AutoPtr<ArrayOf<ICharSequence*> > temp;

        mKeyboardLayouts->IsEmpty(&empty);
        if (empty) {

            Int32 index = removedIndex;
            Int32 size;

            mKeyboardLayouts->GetSize(&size);
            if (index == size) {
                index = 0;
            }

            AutoPtr<ICharSequence> csq;
            mKeyboardLayouts->Get(index, (IInterface**)&csq);
            csq->ToString(&mCurrentKeyboardLayout);
        } else {
            mCurrentKeyboardLayout = NULL;
        }
    }
}

Boolean PersistentDataStore::InputDeviceState::SwitchKeyboardLayout(Int32 direction)
{
    Int32 size;

    mKeyboardLayouts->GetSize(&size);
    if (size < 2) {
        return FALSE;
    }

    Int32 index;
    AutoPtr<ICollections> coll;
    CCollections::AcquireSingleton((ICollections**)&coll);
    AutoPtr<ICharSequence> csq;
    CString::New(mCurrentKeyboardLayout, (ICharSequence**)&csq);
    coll->BinarySearch(IList::Probe(mKeyboardLayouts), csq, &index);
    if (direction > 0) {
        index = (index + 1) % size;
    } else {
        index = (index + size - 1) % size;
    }
    AutoPtr<ICharSequence> csq2;
    mKeyboardLayouts->Get(index, (IInterface**)&csq2);
    csq2->ToString(&mCurrentKeyboardLayout);
    return TRUE;

}

Boolean PersistentDataStore::InputDeviceState::RemoveUninstalledKeyboardLayouts(
    ArrayOf<String>* availableKeyboardLayouts)
{
    Boolean changed = FALSE;
    Int32 size;

    mKeyboardLayouts->GetSize(&size);
    for (int i = size; i-- > 0; ) {
        AutoPtr<ICharSequence> csq;
        String keyboardLayout;
        mKeyboardLayouts->Get(i, (IInterface**)&csq);
        csq->ToString(&keyboardLayout);

        if (!availableKeyboardLayouts->Contains(keyboardLayout)) {
            Slogger::I(TAG, String("Removing uninstalled keyboard layout %s") + keyboardLayout);
            mKeyboardLayouts->Remove(i);
            UpdateCurrentKeyboardLayoutIfRemoved(keyboardLayout, i);
            changed = TRUE;
        }
    }
    return changed;
}

// throws IOException, XmlPullParserException
ECode PersistentDataStore::InputDeviceState::LoadFromXml(IXmlPullParser* parser)
{
    Int32 outerDepth;

    parser->GetDepth(&outerDepth);
    while (XmlUtils::NextElementWithin(parser, outerDepth)) {
        String name;

        parser->GetName(&name);
        if (name.Equals("keyboard-layout")) {
            String descriptor;

            parser->GetAttributeValue((String)NULL, String("descriptor"), &descriptor);
            if (descriptor == NULL) {
                //throw new XmlPullParserException(
                //        "Missing descriptor attribute on keyboard-layout.");
                return E_XML_PULL_PARSER_EXCEPTION;
            }
            String current;
            parser->GetAttributeValue((String)NULL, String("current"), &current);
            Boolean result;
            AutoPtr<ICharSequence> csq;
            CString::New(descriptor, (ICharSequence**)&csq);
            mKeyboardLayouts->Contains(csq, &result);
            if (result) {
                //throw new XmlPullParserException(
                //        "Found duplicate keyboard layout.");
                return E_XML_PULL_PARSER_EXCEPTION;
            }

            mKeyboardLayouts->Add(csq);
            if (current != NULL && current.Equals("TRUE")) {
                if (mCurrentKeyboardLayout != NULL) {
                    //throw new XmlPullParserException(
                    //        "Found multiple current keyboard layouts.");
                    return E_XML_PULL_PARSER_EXCEPTION;
                }
                mCurrentKeyboardLayout = descriptor;
            }
        } else if (name.Equals("calibration")) {
            String format;
            String rotation;
            Int32 r = -1;

            parser->GetAttributeValue((String)NULL, String("format"), &format);
            parser->GetAttributeValue((String)NULL, String("rotation"), &rotation);
            if (format == NULL) {
                //throw new XmlPullParserException(
                //        "Missing format attribute on calibration.");
                return E_XML_PULL_PARSER_EXCEPTION;
            }
            if (!format.Equals("affine")) {
                //throw new XmlPullParserException(
                //        "Unsupported format for calibration.");
                return E_XML_PULL_PARSER_EXCEPTION;
            }
            if (rotation != NULL) {
                ECode ec = StringToSurfaceRotation(rotation, &r);
                if (FAILED(ec)) {
                    //throw new XmlPullParserException(
                    //        "Unsupported rotation for calibration.");
                    return E_ILLEGAL_ARGUMENT_EXCEPTION;
                }
            }

            AutoPtr<ArrayOf<Float> > matrix;
            TouchCalibration::IDENTITY->GetAffineTransform((ArrayOf<Float>**)&matrix);

            Int32 depth;
            //why GetDepth() twince?
            parser->GetDepth(&depth);

            while (XmlUtils::NextElementWithin(parser, depth)) {
                String tag;
                String value;

                parser->GetName(&tag);
                tag = tag.ToLowerCase();
                parser->NextText(&value);

                for (Int32 i = 0; i < matrix->GetLength() && i < CALIBRATION_NAME->GetLength(); i++) {
                    if (tag.Equals((*CALIBRATION_NAME)[i])) {
                        (*matrix)[i] = StringUtils::ParseFloat(value);
                        break;
                    }
                }

                if (r == -1) {
                    // Assume calibration applies to all rotations
                    for (r = 0; r < mTouchCalibration->GetLength(); r++) {
                        (*mTouchCalibration)[r] = new TouchCalibration((*matrix)[0],
                            (*matrix)[1], (*matrix)[2], (*matrix)[3], (*matrix)[4], (*matrix)[5]);
                    }
                } else {
                    (*mTouchCalibration)[r] = new TouchCalibration((*matrix)[0],
                        (*matrix)[1], (*matrix)[2], (*matrix)[3], (*matrix)[4], (*matrix)[5]);
                }
            }
        }
    }

    // Maintain invariant that layouts are sorted.
    AutoPtr<ICollections> coll;
    CCollections::AcquireSingleton((ICollections**)&coll);
    coll->Sort(IList::Probe(mKeyboardLayouts));

    // Maintain invariant that there is always a current keyboard layout unless
    // there are none installed.
    Boolean b;
    mKeyboardLayouts->IsEmpty(&b);
    if (mCurrentKeyboardLayout == NULL && !b) {
        mKeyboardLayouts->Get(0, (IInterface**)&mCurrentKeyboardLayout);
    }

    return NOERROR;
}

// throws IOException
ECode PersistentDataStore::InputDeviceState::SaveToXml(IXmlSerializer* serializer)
{
    Int32 size;

    mKeyboardLayouts->GetSize(&size);
    for (Int32 i = 0;  i < size;  i++) {
        AutoPtr<ICharSequence> object;
        String layout;

        mKeyboardLayouts->Get(i, (IInterface**)&object);
        object->ToString(&layout);
        serializer->WriteStartTag((String)NULL, String("keyboard-layout"));
        serializer->WriteAttribute((String)NULL, String("descriptor"), layout);
        if (layout.Equals(mCurrentKeyboardLayout)) {
            serializer->WriteAttribute((String)NULL, String("current"), String("TRUE"));
        }
        serializer->WriteEndTag((String)NULL, String("keyboard-layout"));
    }


    for (Int32 i = 0;  i < mTouchCalibration->GetLength();  i++) {
        if (mTouchCalibration[i] != NULL) {
            String rotation;

            SurfaceRotationToString(i, &rotation);
            AutoPtr<ArrayOf<Float> > transform;
            (*mTouchCalibration)[i]->GetAffineTransform((ArrayOf<Float>**)&transform);

            serializer->WriteStartTag((String)NULL, String("calibration"));
            serializer->WriteAttribute((String)NULL, String("format"), String("affine"));
            serializer->WriteAttribute((String)NULL, String("rotation"), rotation);
            for (Int32 j = 0;  j < transform->GetLength() && j < CALIBRATION_NAME->GetLength();  j++) {
                serializer->WriteStartTag((String)NULL, (*CALIBRATION_NAME)[j]);
                serializer->WriteText(StringUtils::ToString((*transform)[j]));
                serializer->WriteEndTag((String)NULL, (*CALIBRATION_NAME)[j]);
            }
            serializer->WriteEndTag((String)NULL, String("calibration"));
        }
    }

    return NOERROR;
}

ECode PersistentDataStore::InputDeviceState::SurfaceRotationToString(Int32 surfaceRotation, String* str)
{
    switch (surfaceRotation) {
        case ISurface::ROTATION_0:
            *str = String("0");
            return NOERROR;
        case ISurface::ROTATION_90:
            *str = String("90");
            return NOERROR;
        case ISurface::ROTATION_180:
            *str = String("180");
            return NOERROR;
        case ISurface::ROTATION_270:
            *str = String("270");
            return NOERROR;
    }
    //throw new IllegalArgumentException("Unsupported surface rotation value" + surfaceRotation);
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode PersistentDataStore::InputDeviceState::StringToSurfaceRotation(const String& str, Int32 *r)
{
    if (str.Equals("0")) {
        *r = ISurface::ROTATION_0;
        return NOERROR;
    }
    if (str.Equals("90")) {
        *r = ISurface::ROTATION_90;
        return NOERROR;
    }
    if (str.Equals("180")) {
        *r = ISurface::ROTATION_180;
        return NOERROR;
    }
    if (str.Equals("270")) {
        *r = ISurface::ROTATION_270;
        return NOERROR;
    }

    //throw new IllegalArgumentException("Unsupported surface rotation string '" + s + "'");
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

} // Input
} // Server
} // Droid
} // Elastos
