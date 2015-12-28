
#include "elastos/droid/widget/CLinearLayout.h"
#include "elastos/droid/widget/CLinearLayoutLayoutParams.h"
#include "elastos/droid/widget/CFrameLayout.h"
#include "elastos/droid/widget/CFrameLayoutLayoutParams.h"
#include "elastos/droid/widget/CRelativeLayout.h"
#include "elastos/droid/widget/CRelativeLayoutLayoutParams.h"
#include "elastos/droid/widget/CArrayAdapter.h"
#include "elastos/droid/widget/CSimpleAdapter.h"
#include "elastos/droid/widget/CSimpleCursorAdapter.h"
#include "elastos/droid/widget/CSimpleExpandableListAdapter.h"
#include "elastos/droid/widget/CSimpleMonthAdapter.h"
#include "elastos/droid/widget/CAbsoluteLayoutLayoutParams.h"
#include "elastos/droid/widget/CAbsoluteLayout.h"
#include "elastos/droid/widget/CCursorFilter.h"
#include "elastos/droid/widget/CImageView.h"
#include "elastos/droid/widget/CImageButton.h"
#include "elastos/droid/widget/CProgressBar.h"
#include "elastos/droid/widget/CGallery.h"
#include "elastos/droid/widget/CGalleryLayoutParams.h"
#include "elastos/droid/widget/CScroller.h"
#include "elastos/droid/widget/CViewAnimator.h"
#include "elastos/droid/widget/CViewSwitcher.h"
#include "elastos/droid/widget/CImageSwitcher.h"
#include "elastos/droid/widget/CAdapterViewFlipper.h"
#include "elastos/droid/widget/CTextView.h"
#include "elastos/droid/widget/CEditText.h"
#include "elastos/droid/widget/CButton.h"
#include "elastos/droid/widget/CSwitch.h"
#include "elastos/droid/widget/CGridView.h"
#include "elastos/droid/widget/CHorizontalScrollView.h"
#include "elastos/droid/widget/CHorizontalScrollViewSavedState.h"
#include "elastos/droid/widget/CScrollBarDrawable.h"
#include "elastos/droid/widget/CQuickContactBadge.h"
#include "elastos/droid/widget/CTextClock.h"
#include "elastos/droid/widget/CToggleButton.h"
#include "elastos/droid/widget/CTwoLineListItem.h"
#include "elastos/droid/widget/CAutoCompleteTextView.h"
#include "elastos/droid/widget/CSearchViewSearchAutoComplete.h"
#include "elastos/droid/widget/CSearchView.h"
#include "elastos/droid/widget/CRadialTimePickerView.h"
#include "elastos/droid/widget/CScrollView.h"
#include "elastos/droid/widget/CScrollViewSavedState.h"
#include "elastos/droid/widget/CActionMenuPresenter.h"
#include "elastos/droid/widget/CActionMenuPresenterSavedState.h"
#include "elastos/droid/widget/CAdapterView.h"
#include "elastos/droid/widget/CAdapterViewAnimator.h"
#include "elastos/droid/widget/COverScroller.h"
#include "elastos/droid/widget/CSimpleMonthView.h"
#include "elastos/droid/widget/CGridLayout.h"
#include "elastos/droid/widget/CGridLayoutLayoutParams.h"
#include "elastos/droid/widget/CActionMenuView.h"
#include "elastos/droid/widget/CActionMenuViewLayoutParams.h"
#include "elastos/droid/widget/CExpandableListView.h"
#include "elastos/droid/widget/CListPopupWindow.h"
#include "elastos/droid/widget/CMultiAutoCompleteTextView.h"
#include "elastos/droid/widget/CCheckBox.h"
#include "elastos/droid/widget/CTextSwitcher.h"
#include "elastos/droid/widget/CSpace.h"
#include "elastos/droid/widget/CSlidingDrawer.h"
#include "elastos/droid/widget/CSpellChecker.h"
#include "elastos/droid/widget/CAbsSpinnerSavedState.h"
#include "elastos/droid/widget/CSpinner.h"
#include "elastos/droid/widget/CSpinnerSavedState.h"
#include "elastos/droid/widget/CViewFlipper.h"
#include "elastos/droid/widget/CZoomControls.h"
#include "elastos/droid/widget/CZoomButton.h"
#include "elastos/droid/widget/CZoomButtonsController.h"
#include "elastos/droid/widget/CRemoteViews.h"
#include "elastos/droid/widget/CRemoteViews.h"
#include "elastos/droid/widget/CRemoteViewsAdapter.h"
#include "elastos/droid/widget/CRemoteViewsListAdapter.h"
#include "elastos/droid/widget/CRemoteViewsFactoryAdapter.h"
#include "elastos/droid/widget/CRemoteViewsAdapterServiceConnection.h"
#include "elastos/droid/widget/CTableLayout.h"
#include "elastos/droid/widget/CTableLayoutLayoutParams.h"
#include "elastos/droid/widget/CBlinkLayout.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CAR_OBJECT_IMPL(CGallery)
CAR_OBJECT_IMPL(CGalleryLayoutParams)
CAR_OBJECT_IMPL(CScroller)
CAR_OBJECT_IMPL(CViewAnimator)
CAR_OBJECT_IMPL(CViewSwitcher)
CAR_OBJECT_IMPL(CImageSwitcher)
CAR_OBJECT_IMPL(CTextView)
CAR_OBJECT_IMPL(CEditText)
CAR_OBJECT_IMPL(CButton)
CAR_OBJECT_IMPL(CSwitch)
CAR_OBJECT_IMPL(CGridView)
CAR_OBJECT_IMPL(CHorizontalScrollView)
CAR_OBJECT_IMPL(CHorizontalScrollViewSavedState)
CAR_OBJECT_IMPL(CScrollBarDrawable)
CAR_OBJECT_IMPL(CQuickContactBadge)
CAR_OBJECT_IMPL(CTextClock)
CAR_OBJECT_IMPL(CToggleButton)
CAR_OBJECT_IMPL(CTwoLineListItem)
CAR_OBJECT_IMPL(CAutoCompleteTextView)
CAR_OBJECT_IMPL(CSearchViewSearchAutoComplete)
CAR_OBJECT_IMPL(CSearchView)
CAR_OBJECT_IMPL(CRadialTimePickerView)
CAR_OBJECT_IMPL(CScrollView)
CAR_OBJECT_IMPL(CScrollViewSavedState)
CAR_OBJECT_IMPL(CLinearLayout)
CAR_OBJECT_IMPL(CLinearLayoutLayoutParams)
CAR_OBJECT_IMPL(CFrameLayout)
CAR_OBJECT_IMPL(CFrameLayoutLayoutParams)
CAR_OBJECT_IMPL(CRelativeLayout)
CAR_OBJECT_IMPL(CRelativeLayoutLayoutParams)
CAR_OBJECT_IMPL(CArrayAdapter)
CAR_OBJECT_IMPL(CSimpleAdapter)
CAR_OBJECT_IMPL(CSimpleCursorAdapter)
CAR_OBJECT_IMPL(CSimpleExpandableListAdapter)
CAR_OBJECT_IMPL(CSimpleMonthAdapter)
CAR_OBJECT_IMPL(CSimpleMonthView)
CAR_OBJECT_IMPL(CAbsoluteLayoutLayoutParams)
CAR_OBJECT_IMPL(CAbsoluteLayout)
CAR_OBJECT_IMPL(CCursorFilter)
CAR_OBJECT_IMPL(CImageView)
CAR_OBJECT_IMPL(CImageButton)
CAR_OBJECT_IMPL(CProgressBar)
CAR_OBJECT_IMPL(CActionMenuPresenter)
CAR_OBJECT_IMPL(CActionMenuPresenterSavedState)
CAR_OBJECT_IMPL(CAdapterView)
CAR_OBJECT_IMPL(CAdapterViewAnimator)
CAR_OBJECT_IMPL(COverScroller)
CAR_OBJECT_IMPL(CAdapterViewFlipper)
CAR_OBJECT_IMPL(CGridLayout)
CAR_OBJECT_IMPL(CGridLayoutLayoutParams)
CAR_OBJECT_IMPL(CActionMenuViewLayoutParams)
CAR_OBJECT_IMPL(CActionMenuView)
CAR_OBJECT_IMPL(CExpandableListView)
CAR_OBJECT_IMPL(CListPopupWindow)
CAR_OBJECT_IMPL(CMultiAutoCompleteTextView)
CAR_OBJECT_IMPL(CCheckBox)
CAR_OBJECT_IMPL(CTextSwitcher)
CAR_OBJECT_IMPL(CSpace)
CAR_OBJECT_IMPL(CSlidingDrawer)
CAR_OBJECT_IMPL(CSpellChecker)
CAR_OBJECT_IMPL(CAbsSpinnerSavedState)
CAR_OBJECT_IMPL(CSpinner)
CAR_OBJECT_IMPL(CSpinnerSavedState)
CAR_OBJECT_IMPL(CViewFlipper)
CAR_OBJECT_IMPL(CZoomControls)
CAR_OBJECT_IMPL(CZoomButton)
CAR_OBJECT_IMPL(CZoomButtonsController)
CAR_OBJECT_IMPL(CRemoteViews)
CAR_OBJECT_IMPL(CRemoteViewsAdapter)
CAR_OBJECT_IMPL(CRemoteViewsListAdapter)
CAR_OBJECT_IMPL(CRemoteViewsFactoryAdapter)
CAR_OBJECT_IMPL(CRemoteViewsAdapterServiceConnection)
CAR_OBJECT_IMPL(CTableLayout)
CAR_OBJECT_IMPL(CTableLayoutLayoutParams)
CAR_OBJECT_IMPL(CBlinkLayout)

} // namespace Widget
} // namespace Droid
} // namespace Elastos
