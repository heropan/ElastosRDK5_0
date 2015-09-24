// wuweizuo automatic build .h file from .java file.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_LOCATIONPROVIDERFACTORY_H_
#define _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_LOCATIONPROVIDERFACTORY_H_

#include "ext/frameworkext.h"

// package org.chromium.content.browser;
// import android.content.Context;
// import android.location.Criteria;
// import android.location.Location;
// import android.location.LocationListener;
// import android.location.LocationManager;
// import android.os.Bundle;
// import android.util.Log;
// import com.google.common.annotations.VisibleForTesting;
// import org.chromium.base.ThreadUtils;
// import java.util.List;

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Location::ICriteria;
using Elastos::Droid::Location::ILocation;
using Elastos::Droid::Location::ILocationListener;
using Elastos::Droid::Location::ILocationManager;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Util::ILog;
using Elastos::Droid::Webkit::Base::ThreadUtils;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
  * Factory to create a LocationProvider to allow us to inject
  * a mock for tests.
  */
class LocationProviderFactory : public Object
{
public:
    /**
      * LocationProviderFactory.get() returns an instance of this interface.
      */
    class LocationProvider
    {
    public:
        virtual CARAPI Start(
            /* [in] */ Boolean gpsEnabled) = 0;

        virtual CARAPI Stop() = 0;

        virtual CARAPI_(Boolean) IsRunning() = 0;
    };

    class InnerRunnable
        : public Object
        , public Runnable
    {
    public:
        InnerRunnable(
            /* [in] */ LocationProviderFactory* owner);

        // @Override
        CARAPI Run();

    private:
        LocationProviderFactory* mOwner;
    };

private:
    /**
      * This is the core of android location provider. It is a separate class for clarity
      * so that it can manage all processing completely in the UI thread. The container class
      * ensures that the start/stop calls into this class are done in the UI thread.
      */
    class LocationProviderImpl
        : public Object
        , public LocationListener
        , public LocationProviderFactory::LocationProvider
    {
    public:
        LocationProviderImpl(
            /* [in] */ IContext* context);

        /**
          * Start listening for location updates.
          * @param gpsEnabled Whether or not we're interested in high accuracy GPS.
          */
        // @Override
        CARAPI Start(
            /* [in] */ Boolean gpsEnabled);

        /**
          * Stop listening for location updates.
          */
        // @Override
        CARAPI Stop();

        /**
          * Returns true if we are currently listening for location updates, false if not.
          */
        // @Override
        CARAPI_(Boolean) IsRunning();

        // @Override
        CARAPI OnLocationChanged(
            /* [in] */ ILocation* location);

        // @Override
        CARAPI OnStatusChanged(
            /* [in] */ const String& provider,
            /* [in] */ Int32 status,
            /* [in] */ IBundle* extras);

        // @Override
        CARAPI OnProviderEnabled(
            /* [in] */ const String& provider);

        // @Override
        CARAPI OnProviderDisabled(
            /* [in] */ const String& provider);

    private:
        CARAPI UpdateNewLocation(
            /* [in] */ ILocation* location);

        CARAPI EnsureLocationManagerCreated();

        /**
          * Registers this object with the location service.
          */
        CARAPI RegisterForLocationUpdates(
            /* [in] */ Boolean isGpsEnabled);

        /**
          * Unregisters this object from the location service.
          */
        CARAPI UnregisterFromLocationUpdates();

        CARAPI_(Boolean) UsePassiveOneShotLocation();

        /*
          * Checks if the passive location provider is the only provider available
          * in the system.
          */
        CARAPI_(Boolean) IsOnlyPassiveLocationProviderEnabled();

    private:
        // Log tag
        static const String TAG;
        AutoPtr<IContext> mContext;
        AutoPtr<ILocationManager> mLocationManager;
        Boolean mIsRunning;
    };

public:
    // @VisibleForTesting
    static CARAPI SetLocationProviderImpl(
        /* [in] */  LocationProviderFactory);

    static CARAPI_(AutoPtr<LocationProvider>) Get(
        /* [in] */ IContext* context);

private:
    LocationProviderFactory();

private:
    static AutoPtr<LocationProviderFactory::LocationProvider> sProviderImpl;
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_LOCATIONPROVIDERFACTORY_H_
