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

package com.android.settings.dashboard;

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::IView;
using Elastos::Droid::Widget::IFrameLayout;

using Elastos::Droid::Widget::IImageView;
using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::Settings::IR;
using Elastos::Droid::Settings::IUtils;

public class DashboardTileView extends FrameLayout implements View.OnClickListener {

    private static const Int32 DEFAULT_COL_SPAN = 1;

    private ImageView mImageView;
    private TextView mTitleTextView;
    private TextView mStatusTextView;
    private View mDivider;

    private Int32 mColSpan = DEFAULT_COL_SPAN;

    private DashboardTile mTile;

    public DashboardTileView(Context context) {
        This(context, NULL);
    }

    public DashboardTileView(Context context, AttributeSet attrs) {
        Super(context, attrs);

        final View view = LayoutInflater->From(context).Inflate(R.layout.dashboard_tile, this);

        mImageView = (ImageView) view->FindViewById(R.id.icon);
        mTitleTextView = (TextView) view->FindViewById(R.id.title);
        mStatusTextView = (TextView) view->FindViewById(R.id.status);
        mDivider = view->FindViewById(R.id.tile_divider);

        SetOnClickListener(this);
        SetBackgroundResource(R.drawable.dashboard_tile_background);
        SetFocusable(TRUE);
    }

    public TextView GetTitleTextView() {
        return mTitleTextView;
    }

    public TextView GetStatusTextView() {
        return mStatusTextView;
    }

    public ImageView GetImageView() {
        return mImageView;
    }

    CARAPI SetTile(DashboardTile tile) {
        mTile = tile;
    }

    CARAPI SetDividerVisibility(Boolean visible) {
        mDivider->SetVisibility(visible ? View.VISIBLE : View.GONE);
    }

    void SetColumnSpan(Int32 span) {
        mColSpan = span;
    }

    Int32 GetColumnSpan() {
        return mColSpan;
    }

    //@Override
    CARAPI OnClick(View v) {
        if (mTile.fragment != NULL) {
            Utils->StartWithFragment(GetContext(), mTile.fragment, mTile.fragmentArguments, NULL, 0,
                    mTile.titleRes, mTile->GetTitle(GetResources()));
        } else if (mTile.intent != NULL) {
            GetContext()->StartActivity(mTile.intent);
        }
    }
}
