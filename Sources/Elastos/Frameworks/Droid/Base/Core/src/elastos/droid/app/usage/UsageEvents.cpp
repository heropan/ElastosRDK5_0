#include "elastos/droid/app/usage/UsageEvents.h"


namespace Elastos {
namespace Droid {
namespace App {
namespace Usage {

//==================================================================
// UsageEvent
//==================================================================

CAR_INTERFACE_IMPL(UsageEvent, Object, IUsageEvent)

UsageEvent::UsageEvent()
    : mTimeStamp(0)
    , mEventType(0)
{}

UsageEvent::~UsageEvent()
{}

ECode UsageEvent::constructor()
{
    return NOERROR;
}

ECode UsageEvent::SetPackageName(
    /* [in] */ const String& value)
{
    mPackage = value;
    return NOERROR;
}

ECode UsageEvent::GetPackageName(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mPackage;
    return NOERROR;
}

ECode UsageEvent::SetClassName(
    /* [in] */ const String& value)
{
    mClass = value;
    return NOERROR;
}

ECode UsageEvent::GetClassName(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mClass;
    return NOERROR;
}

ECode UsageEvent::SetTimeStamp(
    /* [in] */ Int64 value)
{
    mTimeStamp = value;
    return NOERROR;
}

ECode UsageEvent::GetTimeStamp(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mTimeStamp;
    return NOERROR;
}

ECode UsageEvent::SetEventType(
    /* [in] */ Int32 value)
{
    mEventType = value;
    return NOERROR;
}

ECode UsageEvent::GetEventType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mEventType;
    return NOERROR;
}

ECode UsageEvent::SetConfiguration(
    /* [in] */ 　IConfiguration* value)
{
    mConfiguration = value;
    return NOERROR;
}

ECode UsageEvent::GetConfiguration(
    /* [out] */　IConfiguration** config)
{
    VALIDATE_NOT_NULL(result)
    *result = mConfiguration;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

//==================================================================
// UsageEvents
//==================================================================

CAR_INTERFACE_IMPL_2(UsageEvents, IUsageEvents, IParcelable)

UsageEvents::UsageEvents()
    : mEventCount(0)
    , mIndex(0)
{}

UsageEvents::~UsageEvents()
{
    Finalize();
}

ECode UsageEvents::constructor()
{
    return NOERROR;
}

ECode UsageEvents::constructor(
    /* [in] */ IList* events,
    /* [in] */ ArrayOf<String>* stringPool)
{
    mStringPool = stringPool;
    mEventCount = events.size();
    mEventsToWrite = events;
    return NOERROR;
}

ECode UsageEvents::HasNextEvent(
    /* [out] */ Boolean* result)
{
    return mIndex < mEventCount;
    return NOERROR;
}

ECode UsageEvents::GetNextEvent(
    /* [in] */ IUsageEvent* eventOut,
    /* [out] */ Boolean* result)
{
    if (mIndex >= mEventCount) {
        return FALSE;
    }

    readEventFromParcel(mParcel, eventOut);

    mIndex++;
    if (mIndex >= mEventCount) {
        mParcel.recycle();
        mParcel = null;
    }
    return true;
    return NOERROR;
}


ECode UsageEvents::ResetToStart()
{
    mIndex = 0;
    if (mParcel != null) {
        mParcel.setDataPosition(0);
    }

    return NOERROR;
}

ECode UsageEvents::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest.writeInt(mEventCount);
    dest.writeInt(mIndex);
    if (mEventCount > 0) {
        dest.writeStringArray(mStringPool);

        if (mEventsToWrite != null) {
            // Write out the events
            Parcel p = Parcel.obtain();
            try {
                p.setDataPosition(0);
                for (Int32 i = 0; i < mEventCount; i++) {
                    final Event event = mEventsToWrite.get(i);
                    writeEventToParcel(event, p, flags);
                }

                final Int32 listByteLength = p.dataPosition();

                // Write the total length of the data.
                dest.writeInt(listByteLength);

                // Write our current position into the data.
                dest.writeInt(0);

                // Write the data.
                dest.appendFrom(p, 0, listByteLength);
            } finally {
                p.recycle();
            }

        } else if (mParcel != null) {
            // Write the total length of the data.
            dest.writeInt(mParcel.dataSize());

            // Write out current position into the data.
            dest.writeInt(mParcel.dataPosition());

            // Write the data.
            dest.appendFrom(mParcel, 0, mParcel.dataSize());
        } else {
            throw new IllegalStateException(
                    "Either mParcel or mEventsToWrite must not be null");
        }
    }
    return NOERROR;
}

ECode UsageEvents::ReadFromParcel(
    /* [in] */ IParcel in)
{
    mEventCount = in->ReadInt();
    mIndex = in->ReadInt();
    if (mEventCount > 0) {
        mStringPool = in.createStringArray();

        final Int32 listByteLength = in->ReadInt();
        final Int32 positionInParcel = in->ReadInt();
        mParcel = Parcel.obtain();
        mParcel.setDataPosition(0);
        mParcel.appendFrom(in, in.dataPosition(), listByteLength);
        mParcel.setDataSize(mParcel.dataPosition());
        mParcel.setDataPosition(positionInParcel);
    }
    return NOERROR;
}

ECode UsageEvents::FindStringIndex(
    /* [in] */ const String& str,
    /* [out] */ Int32* index)
{
    final Int32 index = Arrays.binarySearch(mStringPool, str);
    if (index < 0) {
        throw new IllegalStateException("String '" + str + "' is not in the string pool");
    }
    return index;
    return NOERROR;
}

void UsageEvents::WriteEventToParcel(
    /* [in] */ IUsageEvent* event,
    /* [in] */ IParcel* p)
{
    final Int32 packageIndex;
    if (event.mPackage != null) {
        packageIndex = findStringIndex(event.mPackage);
    } else {
        packageIndex = -1;
    }

    final Int32 classIndex;
    if (event.mClass != null) {
        classIndex = findStringIndex(event.mClass);
    } else {
        classIndex = -1;
    }
    p.writeInt(packageIndex);
    p.writeInt(classIndex);
    p.writeInt(event.mEventType);
    p.writeLong(event.mTimeStamp);

    if (event.mEventType == Event.CONFIGURATION_CHANGE) {
        event.mConfiguration.writeToParcel(p, flags);
    }
}

void UsageEvents::ReadEventFromParcel(
    /* [in] */ IParcel* p,
    /* [in] */ IUsageEvent* eventOut)
{
    final Int32 packageIndex = p.readInt();
    if (packageIndex >= 0) {
        eventOut.mPackage = mStringPool[packageIndex];
    } else {
        eventOut.mPackage = null;
    }

    final Int32 classIndex = p.readInt();
    if (classIndex >= 0) {
        eventOut.mClass = mStringPool[classIndex];
    } else {
        eventOut.mClass = null;
    }
    eventOut.mEventType = p.readInt();
    eventOut.mTimeStamp = p.readLong();

    // Extract the configuration for configuration change events.
    if (eventOut.mEventType == Event.CONFIGURATION_CHANGE) {
        eventOut.mConfiguration = Configuration.CREATOR.createFromParcel(p);
    } else {
        eventOut.mConfiguration = null;
    }
}

void UsageEvents::Finalize()
{
    if (mParcel != NULL) {
        mParcel->Recycle();
        mParcel = NULL;
    }
}


} // namespace Usage
} // namespace App
} // namespace Droid
} // namespace Elastos
