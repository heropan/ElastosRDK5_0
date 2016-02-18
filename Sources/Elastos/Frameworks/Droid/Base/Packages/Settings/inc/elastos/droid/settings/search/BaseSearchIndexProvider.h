/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.android.settings.search;

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Provider::ISearchIndexableResource;

using Elastos::Utility::ICollections;
using Elastos::Utility::IList;

/**
 * A basic SearchIndexProvider that returns no data to index.
 */
public class BaseSearchIndexProvider implements Indexable.SearchIndexProvider {

    private static const List<String> EMPTY_LIST = Collections.<String>EmptyList();

    public BaseSearchIndexProvider() {
    }

    //@Override
    public List<SearchIndexableResource> GetXmlResourcesToIndex(Context context, Boolean enabled) {
        return NULL;
    }

    //@Override
    public List<SearchIndexableRaw> GetRawDataToIndex(Context context, Boolean enabled) {
        return NULL;
    }

    //@Override
    public List<String> GetNonIndexableKeys(Context context) {
        return EMPTY_LIST;
    }
}
