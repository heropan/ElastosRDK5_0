
#include "view/View.h"
#include "view/ViewGroup.h"
#include "view/ViewRootImpl.h"
#include "view/CDispatcherState.h"
#include "view/ViewTreeObserver.h"
#include "view/CViewConfiguration.h"
#include "view/LayoutInflater.h"
#include "view/SoundEffectConstants.h"
#include "view/CChoreographerHelper.h"
#include "view/CViewPropertyAnimator.h"
#include "view/animation/Animation.h"
#include "view/animation/AnimationUtils.h"
#include "view/animation/CTransformation.h"
#include "view/accessibility/CAccessibilityManager.h"
#include "view/accessibility/CAccessibilityEvent.h"
#include "view/accessibility/CAccessibilityNodeInfo.h"
#include "view/inputmethod/CInputMethodManager.h"
#include "graphics/CCanvas.h"
#include "graphics/CPaint.h"
#include "graphics/CPoint.h"
#include "graphics/CMatrix.h"
#include "graphics/CCamera.h"
#include "graphics/CLinearGradient.h"
#include "graphics/CPorterDuffXfermode.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/Insets.h"
#include "graphics/drawable/CColorDrawable.h"
#include "graphics/CInterpolator.h"
#include "widget/CScrollBarDrawable.h"
#include "utility/CParcelableObjectContainer.h"
#include "utility/Pools.h"
#include "utility/CTypedValue.h"
#include "os/SystemClock.h"
#include "os/SystemProperties.h"
#include "os/Build.h"
#include "hardware/display/DisplayManagerGlobal.h"
#include "text/TextUtils.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringBuilder.h>
#include "R.h"

using Libcore::ICU::CLocaleHelper;
using Libcore::ICU::ILocaleHelper;
using Libcore::ICU::ILocale;
using Elastos::Core::StringBuilder;
using Elastos::Core::CString;

using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Res::IResourcesTheme;
using Elastos::Droid::Hardware::Display::DisplayManagerGlobal;
using Elastos::Droid::Graphics::IInterpolator;
using Elastos::Droid::Graphics::CInterpolator;
using Elastos::Droid::Graphics::Drawable::CColorDrawable;
using Elastos::Droid::Graphics::Drawable::IColorDrawable;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::View::InputMethod::IInputMethodManager;
using Elastos::Droid::View::InputMethod::CInputMethodManager;
using Elastos::Droid::View::Menu::IMenuBuilder;
using Elastos::Droid::View::Animation::Animation;
using Elastos::Droid::View::Animation::EIID_Animation;
using Elastos::Droid::View::Animation::AnimationUtils;
using Elastos::Droid::View::Animation::CTransformation;
using Elastos::Droid::View::CChoreographerHelper;
using Elastos::Droid::View::Accessibility::CAccessibilityNodeInfo;
using Elastos::Droid::View::Accessibility::CAccessibilityEvent;
using Elastos::Droid::View::Accessibility::CAccessibilityManager;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::SystemProperties;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Utility::Pools;
using Elastos::Droid::Utility::EIID_IPoolable;
using Elastos::Droid::Utility::EIID_IPoolableManager;
using Elastos::Droid::Utility::CParcelableObjectContainer;
using Elastos::Droid::Utility::CTypedValue;
using Elastos::Droid::Internal::Utility::EIID_IPredicate;
using Elastos::Droid::Widget::IScrollBarDrawable;
using Elastos::Droid::Widget::CScrollBarDrawable;


namespace Elastos {
namespace Droid {
namespace View {

extern "C" const InterfaceID EIID_View =
        { 0x47439045, 0x473d, 0x4d3c, { 0x87, 0x6b, 0x42, 0x42, 0x0, 0xc8, 0x9e, 0x19 } };

const Boolean View::DBG = FALSE;
const Int32 View::NOT_FOCUSABLE = 0x00000000;
const Int32 View::FOCUSABLE = 0x00000001;
const Int32 View::FOCUSABLE_MASK = 0x00000001;
const Int32 View::FITS_SYSTEM_WINDOWS = 0x00000002;
const Int32 View::VISIBILITY_MASK;
const Int32 View::ENABLED = 0x00000000;
const Int32 View::DISABLED = 0x00000020;
const Int32 View::ENABLED_MASK = 0x00000020;
const Int32 View::WILL_NOT_DRAW = 0x00000080;
const Int32 View::DRAW_MASK = 0x00000080;
const Int32 View::SCROLLBARS_NONE = 0x00000000;
const Int32 View::SCROLLBARS_HORIZONTAL = 0x00000100;
const Int32 View::SCROLLBARS_VERTICAL = 0x00000200;
const Int32 View::SCROLLBARS_MASK = 0x00000300;
const Int32 View::FILTER_TOUCHES_WHEN_OBSCURED = 0x00000400;
const Int32 View::OPTIONAL_FITS_SYSTEM_WINDOWS = 0x00000800;
const Int32 View::FADING_EDGE_NONE = 0x00000000;
const Int32 View::FADING_EDGE_HORIZONTAL = 0x00001000;
const Int32 View::FADING_EDGE_VERTICAL = 0x00002000;
const Int32 View::FADING_EDGE_MASK = 0x00003000;
const Int32 View::CLICKABLE = 0x00004000;
const Int32 View::DRAWING_CACHE_ENABLED = 0x00008000;
const Int32 View::SAVE_DISABLED = 0x000010000;
const Int32 View::SAVE_DISABLED_MASK = 0x000010000;
const Int32 View::WILL_NOT_CACHE_DRAWING = 0x000020000;
const Int32 View::FOCUSABLE_IN_TOUCH_MODE = 0x00040000;
const Int32 View::DRAWING_CACHE_QUALITY_MASK = 0x00180000;
const Int32 View::LONG_CLICKABLE = 0x00200000;
const Int32 View::DUPLICATE_PARENT_STATE = 0x00400000;
const Int32 View::SCROLLBARS_INSET_MASK = 0x01000000;
const Int32 View::SCROLLBARS_OUTSIDE_MASK = 0x02000000;
const Int32 View::SCROLLBARS_STYLE_MASK = 0x03000000;
const Int32 View::PARENT_SAVE_DISABLED = 0x20000000;
const Int32 View::PARENT_SAVE_DISABLED_MASK = 0x20000000;

const Int32 View::PFLAG_WANTS_FOCUS = 0x00000001;
const Int32 View::PFLAG_FOCUSED = 0x00000002;
const Int32 View::PFLAG_SELECTED = 0x00000004;
const Int32 View::PFLAG_IS_ROOT_NAMESPACE = 0x00000008;
const Int32 View::PFLAG_HAS_BOUNDS = 0x00000010;
const Int32 View::PFLAG_DRAWN = 0x00000020;
const Int32 View::PFLAG_DRAW_ANIMATION = 0x00000040;
const Int32 View::PFLAG_SKIP_DRAW = 0x00000080;
const Int32 View::PFLAG_ONLY_DRAWS_BACKGROUND = 0x00000100;
const Int32 View::PFLAG_REQUEST_TRANSPARENT_REGIONS = 0x00000200;
const Int32 View::PFLAG_DRAWABLE_STATE_DIRTY = 0x00000400;
const Int32 View::PFLAG_MEASURED_DIMENSION_SET = 0x00000800;
const Int32 View::PFLAG_FORCE_LAYOUT = 0x00001000;
const Int32 View::PFLAG_LAYOUT_REQUIRED = 0x00002000;
const Int32 View::PFLAG_PRESSED = 0x00004000;
const Int32 View::PFLAG_DRAWING_CACHE_VALID = 0x00008000;
const Int32 View::PFLAG_ANIMATION_STARTED = 0x00010000;
const Int32 View::PFLAG_SAVE_STATE_CALLED = 0x00020000;
const Int32 View::PFLAG_ALPHA_SET = 0x00040000;
const Int32 View::PFLAG_SCROLL_CONTAINER = 0x00080000;
const Int32 View::PFLAG_SCROLL_CONTAINER_ADDED = 0x00100000;
const Int32 View::PFLAG_DIRTY = 0x00200000;
const Int32 View::PFLAG_DIRTY_OPAQUE = 0x00400000;
const Int32 View::PFLAG_DIRTY_MASK = 0x00600000;
const Int32 View::PFLAG_OPAQUE_BACKGROUND = 0x00800000;
const Int32 View::PFLAG_OPAQUE_SCROLLBARS = 0x01000000;
const Int32 View::PFLAG_OPAQUE_MASK = 0x01800000;
const Int32 View::PFLAG_PREPRESSED = 0x02000000;
const Int32 View::PFLAG_CANCEL_NEXT_UP_EVENT = 0x04000000;


const Int32 View::PFLAG_AWAKEN_SCROLL_BARS_ON_ATTACH = 0x08000000;
const Int32 View::MeasureSpec::MODE_SHIFT;
const Int32 View::MeasureSpec::MODE_MASK;
const Int32 View::MeasureSpec::UNSPECIFIED;
const Int32 View::MeasureSpec::EXACTLY;
const Int32 View::MeasureSpec::AT_MOST;

const Int32 View::ScrollabilityCache::OFF;
const Int32 View::ScrollabilityCache::ON;
const Int32 View::ScrollabilityCache::FADING;

const char* View::VIEW_LOG_TAG = "View";

AutoPtr<ArrayOf<Int32> > View::EMPTY_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::ENABLED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::SELECTED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::PRESSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::WINDOW_FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::ENABLED_FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::ENABLED_SELECTED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::ENABLED_WINDOW_FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::FOCUSED_SELECTED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::FOCUSED_WINDOW_FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::SELECTED_WINDOW_FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::ENABLED_FOCUSED_SELECTED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::ENABLED_FOCUSED_WINDOW_FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::ENABLED_SELECTED_WINDOW_FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::FOCUSED_SELECTED_WINDOW_FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::ENABLED_FOCUSED_SELECTED_WINDOW_FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::PRESSED_WINDOW_FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::PRESSED_SELECTED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::PRESSED_SELECTED_WINDOW_FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::PRESSED_FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::PRESSED_FOCUSED_WINDOW_FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::PRESSED_FOCUSED_SELECTED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::PRESSED_FOCUSED_SELECTED_WINDOW_FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::PRESSED_ENABLED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::PRESSED_ENABLED_WINDOW_FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::PRESSED_ENABLED_SELECTED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::PRESSED_ENABLED_SELECTED_WINDOW_FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::PRESSED_ENABLED_FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::PRESSED_ENABLED_FOCUSED_WINDOW_FOCUSED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::PRESSED_ENABLED_FOCUSED_SELECTED_STATE_SET;
AutoPtr<ArrayOf<Int32> > View::PRESSED_ENABLED_FOCUSED_SELECTED_WINDOW_FOCUSED_STATE_SET;

const Int32 View::PFLAG_ACTIVATED                   = 0x40000000;
const Int32 View::PFLAG_INVALIDATED                 = 0x80000000;
const Int32 View::PFLAG2_DRAG_CAN_ACCEPT            = 0x00000001;
const Int32 View::PFLAG2_DRAG_HOVERED               = 0x00000002;
const Int32 View::PFLAG2_LAYOUT_DIRECTION_MASK_SHIFT = 2;
const Int32 View::PFLAG2_LAYOUT_DIRECTION_MASK = 0x00000003 << PFLAG2_LAYOUT_DIRECTION_MASK_SHIFT;
const Int32 View::PFLAG2_LAYOUT_DIRECTION_RESOLVED_RTL = 4 << PFLAG2_LAYOUT_DIRECTION_MASK_SHIFT;
const Int32 View::PFLAG2_LAYOUT_DIRECTION_RESOLVED = 8 << PFLAG2_LAYOUT_DIRECTION_MASK_SHIFT;
const Int32 View::PFLAG2_LAYOUT_DIRECTION_RESOLVED_MASK = 0x0000000C
            << PFLAG2_LAYOUT_DIRECTION_MASK_SHIFT;

const Int32 View::PFLAG2_HAS_TRANSIENT_STATE = 0x80000000;
const Int32 View::LAYOUT_DIRECTION_RESOLVED_DEFAULT = IView::LAYOUT_DIRECTION_LTR;
const Int32 View::PFLAG2_TEXT_DIRECTION_MASK_SHIFT = 6;
const Int32 View::PFLAG2_TEXT_DIRECTION_MASK = 0x00000007
            << PFLAG2_TEXT_DIRECTION_MASK_SHIFT;
const Int32 View::PFLAG2_TEXT_DIRECTION_RESOLVED = 0x00000008
            << PFLAG2_TEXT_DIRECTION_MASK_SHIFT;
const Int32 View::PFLAG2_TEXT_DIRECTION_RESOLVED_MASK_SHIFT = 10;
const Int32 View::PFLAG2_TEXT_DIRECTION_RESOLVED_MASK = 0x00000007
            << PFLAG2_TEXT_DIRECTION_RESOLVED_MASK_SHIFT;
const Int32 View::PFLAG2_TEXT_DIRECTION_RESOLVED_DEFAULT =
            IView::TEXT_DIRECTION_FIRST_STRONG << PFLAG2_TEXT_DIRECTION_RESOLVED_MASK_SHIFT;
const Int32 View::PFLAG2_TEXT_ALIGNMENT_MASK_SHIFT = 13;
const Int32 View::PFLAG2_TEXT_ALIGNMENT_MASK = 0x00000007 << PFLAG2_TEXT_ALIGNMENT_MASK_SHIFT;
const Int32 View::PFLAG2_TEXT_ALIGNMENT_FLAGS[] = {
            IView::TEXT_ALIGNMENT_INHERIT << PFLAG2_TEXT_ALIGNMENT_MASK_SHIFT,
            IView::TEXT_ALIGNMENT_GRAVITY << PFLAG2_TEXT_ALIGNMENT_MASK_SHIFT,
            IView::TEXT_ALIGNMENT_TEXT_START << PFLAG2_TEXT_ALIGNMENT_MASK_SHIFT,
            IView::TEXT_ALIGNMENT_TEXT_END << PFLAG2_TEXT_ALIGNMENT_MASK_SHIFT,
            IView::TEXT_ALIGNMENT_CENTER << PFLAG2_TEXT_ALIGNMENT_MASK_SHIFT,
            IView::TEXT_ALIGNMENT_VIEW_START << PFLAG2_TEXT_ALIGNMENT_MASK_SHIFT,
            IView::TEXT_ALIGNMENT_VIEW_END << PFLAG2_TEXT_ALIGNMENT_MASK_SHIFT
    };
const Int32 View::PFLAG2_TEXT_ALIGNMENT_RESOLVED = 0x00000008 << PFLAG2_TEXT_ALIGNMENT_MASK_SHIFT;
const Int32 View::PFLAG2_TEXT_ALIGNMENT_RESOLVED_MASK_SHIFT = 17;
const Int32 View::PFLAG2_TEXT_ALIGNMENT_RESOLVED_MASK = 0x00000007
            << PFLAG2_TEXT_ALIGNMENT_RESOLVED_MASK_SHIFT;
const Int32 View::PFLAG2_TEXT_ALIGNMENT_RESOLVED_DEFAULT =
            IView::TEXT_ALIGNMENT_GRAVITY << PFLAG2_TEXT_ALIGNMENT_RESOLVED_MASK_SHIFT;
const Int32 View::PFLAG2_IMPORTANT_FOR_ACCESSIBILITY_SHIFT = 20;
const Int32 View::IMPORTANT_FOR_ACCESSIBILITY_DEFAULT = IView::IMPORTANT_FOR_ACCESSIBILITY_AUTO;
const Int32 View::PFLAG2_IMPORTANT_FOR_ACCESSIBILITY_MASK = (IView::IMPORTANT_FOR_ACCESSIBILITY_AUTO
        | IView::IMPORTANT_FOR_ACCESSIBILITY_YES | IView::IMPORTANT_FOR_ACCESSIBILITY_NO
        | IView::IMPORTANT_FOR_ACCESSIBILITY_NO_HIDE_DESCENDANTS ) << PFLAG2_IMPORTANT_FOR_ACCESSIBILITY_SHIFT;
const Int32 View::PFLAG2_ACCESSIBILITY_LIVE_REGION_SHIFT = 23;
const Int32 View::ACCESSIBILITY_LIVE_REGION_DEFAULT = IView::ACCESSIBILITY_LIVE_REGION_NONE;
const Int32 View::PFLAG2_ACCESSIBILITY_LIVE_REGION_MASK = (IView::ACCESSIBILITY_LIVE_REGION_NONE
            | IView::ACCESSIBILITY_LIVE_REGION_POLITE | IView::ACCESSIBILITY_LIVE_REGION_ASSERTIVE)
            << PFLAG2_ACCESSIBILITY_LIVE_REGION_SHIFT;
const Int32 View::PFLAG2_ACCESSIBILITY_FOCUSED = 0x04000000;
const Int32 View::PFLAG2_SUBTREE_ACCESSIBILITY_STATE_CHANGED = 0x00000080;
const Int32 View::PFLAG2_VIEW_QUICK_REJECTED = 0x10000000;
const Int32 View::PFLAG2_PADDING_RESOLVED = 0x20000000;
const Int32 View::PFLAG2_DRAWABLE_RESOLVED = 0x40000000;
const Int32 View::PFLAG2_HAS_TRANSIENT_STATE = 0x80000000;
const Int32 View::ALL_RTL_PROPERTIES_RESOLVED = PFLAG2_LAYOUT_DIRECTION_RESOLVED |
            PFLAG2_TEXT_DIRECTION_RESOLVED |
            PFLAG2_TEXT_ALIGNMENT_RESOLVED |
            PFLAG2_PADDING_RESOLVED |
            PFLAG2_DRAWABLE_RESOLVED;
const Int32 View::PFLAG3_VIEW_IS_ANIMATING_TRANSFORM = 0x1;
const Int32 View::PFLAG3_VIEW_IS_ANIMATING_ALPHA = 0x2;
const Int32 View::PFLAG3_IS_LAID_OUT = 0x4;
const Int32 View::PFLAG3_MEASURE_NEEDED_BEFORE_LAYOUT = 0x8;
const Int32 View::PFLAG3_CALLED_SUPER = 0x10;
const Int32 View::PFLAG3_APPLYING_INSETS = 0x20;
const Int32 View::PFLAG3_FITTING_SYSTEM_WINDOWS = 0x40;
const Int32 View::PFLAG3_NESTED_SCROLLING_ENABLED = 0x80;
const Int32 View::PFLAG3_OUTLINE_INVALID = 0x100;
const Int32 View::DRAG_MASK = PFLAG2_DRAG_CAN_ACCEPT | PFLAG2_DRAG_HOVERED;

const Int32 View::POPULATING_ACCESSIBILITY_EVENT_TYPES =
        IAccessibilityEvent::TYPE_VIEW_CLICKED
        | IAccessibilityEvent::TYPE_VIEW_LONG_CLICKED
        | IAccessibilityEvent::TYPE_VIEW_SELECTED
        | IAccessibilityEvent::TYPE_VIEW_FOCUSED
        | IAccessibilityEvent::TYPE_WINDOW_STATE_CHANGED
        | IAccessibilityEvent::TYPE_VIEW_HOVER_ENTER
        | IAccessibilityEvent::TYPE_VIEW_HOVER_EXIT
        | IAccessibilityEvent::TYPE_VIEW_TEXT_CHANGED
        | IAccessibilityEvent::TYPE_VIEW_TEXT_SELECTION_CHANGED
        | IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUSED
        | IAccessibilityEvent::TYPE_VIEW_TEXT_TRAVERSED_AT_MOVEMENT_GRANULARITY;

const Int32 View::PFLAG_HOVERED             = 0x10000000;
const Int32 View::PFLAG_PIVOT_EXPLICITLY_SET = 0x20000000;
const Int32 View::LAYOUT_DIRECTION_FLAGS[] = {
            IView::LAYOUT_DIRECTION_LTR,
            IView::LAYOUT_DIRECTION_RTL,
            IView::LAYOUT_DIRECTION_INHERIT,
            IView::LAYOUT_DIRECTION_LOCALE
    };
const Int32 View::LAYOUT_DIRECTION_DEFAULT = IView::LAYOUT_DIRECTION_INHERIT;
const Int32 View::TEXT_ALIGNMENT_DEFAULT = IView::TEXT_ALIGNMENT_GRAVITY;
const Int32 View::TEXT_ALIGNMENT_RESOLVED_DEFAULT = IView::TEXT_ALIGNMENT_GRAVITY;
const Int32 View::PFLAG2_TEXT_DIRECTION_FLAGS[] = {
            IView::TEXT_DIRECTION_INHERIT << PFLAG2_TEXT_DIRECTION_MASK_SHIFT,
            IView::TEXT_DIRECTION_FIRST_STRONG << PFLAG2_TEXT_DIRECTION_MASK_SHIFT,
            IView::TEXT_DIRECTION_ANY_RTL << PFLAG2_TEXT_DIRECTION_MASK_SHIFT,
            IView::TEXT_DIRECTION_LTR << PFLAG2_TEXT_DIRECTION_MASK_SHIFT,
            IView::TEXT_DIRECTION_RTL << PFLAG2_TEXT_DIRECTION_MASK_SHIFT,
            IView::TEXT_DIRECTION_LOCALE << PFLAG2_TEXT_DIRECTION_MASK_SHIFT
    };
const Int32 View::TEXT_DIRECTION_DEFAULT = IView::TEXT_DIRECTION_INHERIT;
const Int32 View::TEXT_DIRECTION_RESOLVED_DEFAULT = IView::TEXT_DIRECTION_FIRST_STRONG;

const Int32 View::VIEW_STATE_WINDOW_FOCUSED = 1;
const Int32 View::VIEW_STATE_SELECTED = 1 << 1;
const Int32 View::VIEW_STATE_FOCUSED = 1 << 2;
const Int32 View::VIEW_STATE_ENABLED = 1 << 3;
const Int32 View::VIEW_STATE_PRESSED = 1 << 4;
const Int32 View::VIEW_STATE_ACTIVATED = 1 << 5;
const Int32 View::VIEW_STATE_ACCELERATED = 1 << 6;
const Int32 View::VIEW_STATE_HOVERED = 1 << 7;
const Int32 View::VIEW_STATE_DRAG_CAN_ACCEPT = 1 << 8;
const Int32 View::VIEW_STATE_DRAG_HOVERED = 1 << 9;
AutoPtr<ArrayOf<Int32> > View::VIEW_STATE_IDS;

const Int32 View::VISIBILITY_FLAGS[] = {IView::VISIBLE, IView::INVISIBLE, IView::GONE};

const Int32 View::DRAWING_CACHE_QUALITY_FLAGS[] = {
    IView::DRAWING_CACHE_QUALITY_AUTO, IView::DRAWING_CACHE_QUALITY_LOW, IView::DRAWING_CACHE_QUALITY_HIGH
};

const AutoPtr<ArrayOf<Int32Array> > View::VIEW_STATE_SETS = InitViewStateSets();
const Int32 View::UNDEFINED_PADDING = Elastos::Core::Math::INT32_MIN_VALUE;

Boolean View::mDebugViewAttributes = FALSE;
Boolean View::sCompatibilityDone = FALSE;
Boolean View::sUseBrokenMakeMeasureSpec = FALSE;
Boolean View::sIgnoreMeasureCache = FALSE;

Int32 View::sNextAccessibilityViewId = 0;
Int32 View::sNextGeneratedId = 1;
Mutex View::sNextGeneratedIdLock;

const Int32 View::PROVIDER_BACKGROUND = 0;
const Int32 View::PROVIDER_NONE = 1;
const Int32 View::PROVIDER_BOUNDS = 2;
const Int32 View::PROVIDER_PADDED_BOUNDS = 3;

//property
const AutoPtr<IProperty> View::ALPHA = new View::AlpahFloatProperty();
const AutoPtr<IProperty> View::TRANSLATION_X = new View::TranslationXFloatProperty();
const AutoPtr<IProperty> View::TRANSLATION_Y = new View::TranslationYFloatProperty();
const AutoPtr<IProperty> View::TRANSLATION_Z = new View::TranslationZFloatProperty();
const AutoPtr<IProperty> View::X = new View::XFloatProperty();
const AutoPtr<IProperty> View::Y = new View::YFloatProperty();
const AutoPtr<IProperty> View::Z = new View::ZFloatProperty();
const AutoPtr<IProperty> View::ROTATION = new View::RotationFloatProperty();
const AutoPtr<IProperty> View::ROTATION_X = new View::RotationXFloatProperty();
const AutoPtr<IProperty> View::ROTATION_Y = new View::RotationYFloatProperty();
const AutoPtr<IProperty> View::SCALE_X = new View::ScaleXFloatProperty();
const AutoPtr<IProperty> View::SCALE_Y = new View::ScaleYFloatProperty();

pthread_key_t View::sKey;
pthread_once_t View::sKeyOnce = PTHREAD_ONCE_INIT;

static void CRectDestructor(void* st)
{
    CRect* obj = static_cast<CRect*>(st);
    if (obj) {
        obj->Release();
    }
}

static void MakeKey()
{
    ASSERT_TRUE(pthread_key_create(&View::sKey, CRectDestructor) == 0);
}

const AutoPtr<ArrayOf<Int32Array> > View::InitViewStateSets()
{
    pthread_once(&sKeyOnce, MakeKey);
    pthread_setspecific(sKey, NULL);

    Int32 TEMP_VIEW_STATE_IDS[] = {
        R::attr::state_window_focused,    VIEW_STATE_WINDOW_FOCUSED,
        R::attr::state_selected,          VIEW_STATE_SELECTED,
        R::attr::state_focused,           VIEW_STATE_FOCUSED,
        R::attr::state_enabled,           VIEW_STATE_ENABLED,
        R::attr::state_pressed,           VIEW_STATE_PRESSED,
        R::attr::state_activated,         VIEW_STATE_ACTIVATED,
        R::attr::state_accelerated,       VIEW_STATE_ACCELERATED,
        R::attr::state_hovered,           VIEW_STATE_HOVERED,
        R::attr::state_drag_can_accept,   VIEW_STATE_DRAG_CAN_ACCEPT,
        R::attr::state_drag_hovered,      VIEW_STATE_DRAG_HOVERED
    };

    VIEW_STATE_IDS = ArrayOf<Int32>::Alloc(TEMP_VIEW_STATE_IDS, 20);

    const Int32 vdsLen = sizeof(R::styleable::ViewDrawableStates) / sizeof(Int32);
    if ((VIEW_STATE_IDS->GetLength() / 2) != vdsLen) {
        assert(0);
        // throw new IllegalStateException(
        //         "VIEW_STATE_IDs array length does not match ViewDrawableStates style array");
    }

    AutoPtr<ArrayOf<Int32> > orderedIds = ArrayOf<Int32>::Alloc(VIEW_STATE_IDS->GetLength());
    for (Int32 i = 0; i < vdsLen; i++) {
        Int32 viewState = R::styleable::ViewDrawableStates[i];
        for (Int32 j = 0; j<VIEW_STATE_IDS->GetLength(); j += 2) {
            if ((*VIEW_STATE_IDS)[j] == viewState) {
                (*orderedIds)[i * 2] = viewState;
                (*orderedIds)[i * 2 + 1] = (*VIEW_STATE_IDS)[j + 1];
            }
        }
    }

    const Int32 NUM_BITS = VIEW_STATE_IDS->GetLength() / 2;
    AutoPtr<ArrayOf<Int32Array> > stateSets = ArrayOf<Int32Array>::Alloc(1 << NUM_BITS);
    for (Int32 i = 0; i < stateSets->GetLength(); i++) {
        //Int32 numBits = Integer.bitCount(i);
        Int32 numBits = 0;

        Int32 tmp = i;
        while (tmp != 0) {
            tmp &=(tmp - 1);
            numBits ++;
        }

        AutoPtr<ArrayOf<Int32> > temp = ArrayOf<Int32>::Alloc(numBits);
        Int32 pos = 0;
        for (Int32 j = 0; j < orderedIds->GetLength(); j += 2) {
            if ((i & (*orderedIds)[j+1]) != 0) {
                (*temp)[pos++] = (*orderedIds)[j];
            }
        }

        stateSets->Set(i, temp);
    }

    EMPTY_STATE_SET = (*stateSets)[0];
    WINDOW_FOCUSED_STATE_SET = (*stateSets)[VIEW_STATE_WINDOW_FOCUSED];
    SELECTED_STATE_SET = (*stateSets)[VIEW_STATE_SELECTED];
    SELECTED_WINDOW_FOCUSED_STATE_SET = (*stateSets)[
            VIEW_STATE_WINDOW_FOCUSED | VIEW_STATE_SELECTED];
    FOCUSED_STATE_SET = (*stateSets)[VIEW_STATE_FOCUSED];
    FOCUSED_WINDOW_FOCUSED_STATE_SET = (*stateSets)[
            VIEW_STATE_WINDOW_FOCUSED | VIEW_STATE_FOCUSED];
    FOCUSED_SELECTED_STATE_SET = (*stateSets)[
            VIEW_STATE_SELECTED | VIEW_STATE_FOCUSED];
    FOCUSED_SELECTED_WINDOW_FOCUSED_STATE_SET = (*stateSets)[
            VIEW_STATE_WINDOW_FOCUSED | VIEW_STATE_SELECTED
            | VIEW_STATE_FOCUSED];
    ENABLED_STATE_SET = (*stateSets)[VIEW_STATE_ENABLED];
    ENABLED_WINDOW_FOCUSED_STATE_SET = (*stateSets)[
            VIEW_STATE_WINDOW_FOCUSED | VIEW_STATE_ENABLED];
    ENABLED_SELECTED_STATE_SET = (*stateSets)[
            VIEW_STATE_SELECTED | VIEW_STATE_ENABLED];
    ENABLED_SELECTED_WINDOW_FOCUSED_STATE_SET = (*stateSets)[
            VIEW_STATE_WINDOW_FOCUSED | VIEW_STATE_SELECTED
            | VIEW_STATE_ENABLED];
    ENABLED_FOCUSED_STATE_SET = (*stateSets)[
            VIEW_STATE_FOCUSED | VIEW_STATE_ENABLED];
    ENABLED_FOCUSED_WINDOW_FOCUSED_STATE_SET = (*stateSets)[
            VIEW_STATE_WINDOW_FOCUSED | VIEW_STATE_FOCUSED
            | VIEW_STATE_ENABLED];
    ENABLED_FOCUSED_SELECTED_STATE_SET = (*stateSets)[
            VIEW_STATE_SELECTED | VIEW_STATE_FOCUSED
            | VIEW_STATE_ENABLED];
    ENABLED_FOCUSED_SELECTED_WINDOW_FOCUSED_STATE_SET = (*stateSets)[
            VIEW_STATE_WINDOW_FOCUSED | VIEW_STATE_SELECTED
            | VIEW_STATE_FOCUSED| VIEW_STATE_ENABLED];

    PRESSED_STATE_SET = (*stateSets)[VIEW_STATE_PRESSED];
    PRESSED_WINDOW_FOCUSED_STATE_SET = (*stateSets)[
            VIEW_STATE_WINDOW_FOCUSED | VIEW_STATE_PRESSED];
    PRESSED_SELECTED_STATE_SET = (*stateSets)[
            VIEW_STATE_SELECTED | VIEW_STATE_PRESSED];
    PRESSED_SELECTED_WINDOW_FOCUSED_STATE_SET = (*stateSets)[
            VIEW_STATE_WINDOW_FOCUSED | VIEW_STATE_SELECTED
            | VIEW_STATE_PRESSED];
    PRESSED_FOCUSED_STATE_SET = (*stateSets)[
            VIEW_STATE_FOCUSED | VIEW_STATE_PRESSED];
    PRESSED_FOCUSED_WINDOW_FOCUSED_STATE_SET = (*stateSets)[
            VIEW_STATE_WINDOW_FOCUSED | VIEW_STATE_FOCUSED
            | VIEW_STATE_PRESSED];
    PRESSED_FOCUSED_SELECTED_STATE_SET = (*stateSets)[
            VIEW_STATE_SELECTED | VIEW_STATE_FOCUSED
            | VIEW_STATE_PRESSED];
    PRESSED_FOCUSED_SELECTED_WINDOW_FOCUSED_STATE_SET = (*stateSets)[
            VIEW_STATE_WINDOW_FOCUSED | VIEW_STATE_SELECTED
            | VIEW_STATE_FOCUSED | VIEW_STATE_PRESSED];
    PRESSED_ENABLED_STATE_SET = (*stateSets)[
            VIEW_STATE_ENABLED | VIEW_STATE_PRESSED];
    PRESSED_ENABLED_WINDOW_FOCUSED_STATE_SET = (*stateSets)[
            VIEW_STATE_WINDOW_FOCUSED | VIEW_STATE_ENABLED
            | VIEW_STATE_PRESSED];
    PRESSED_ENABLED_SELECTED_STATE_SET = (*stateSets)[
            VIEW_STATE_SELECTED | VIEW_STATE_ENABLED
            | VIEW_STATE_PRESSED];
    PRESSED_ENABLED_SELECTED_WINDOW_FOCUSED_STATE_SET = (*stateSets)[
            VIEW_STATE_WINDOW_FOCUSED | VIEW_STATE_SELECTED
            | VIEW_STATE_ENABLED | VIEW_STATE_PRESSED];
    PRESSED_ENABLED_FOCUSED_STATE_SET = (*stateSets)[
            VIEW_STATE_FOCUSED | VIEW_STATE_ENABLED
            | VIEW_STATE_PRESSED];
    PRESSED_ENABLED_FOCUSED_WINDOW_FOCUSED_STATE_SET = (*stateSets)[
            VIEW_STATE_WINDOW_FOCUSED | VIEW_STATE_FOCUSED
            | VIEW_STATE_ENABLED | VIEW_STATE_PRESSED];
    PRESSED_ENABLED_FOCUSED_SELECTED_STATE_SET = (*stateSets)[
            VIEW_STATE_SELECTED | VIEW_STATE_FOCUSED
            | VIEW_STATE_ENABLED | VIEW_STATE_PRESSED];
    PRESSED_ENABLED_FOCUSED_SELECTED_WINDOW_FOCUSED_STATE_SET = (*stateSets)[
            VIEW_STATE_WINDOW_FOCUSED | VIEW_STATE_SELECTED
            | VIEW_STATE_FOCUSED| VIEW_STATE_ENABLED
            | VIEW_STATE_PRESSED];

    return stateSets;
}

// AutoPtr<IFloatProperty> View::InitProperty(
// //    /* [in] */ const String& name)
// {
//     return _FloatProperty(name);
// }

View::TransformationInfo::TransformationInfo()
    : mAlpha(1.0f)
    , mTransitionAlpha(1.0f)
{
    CMatrix::New((IMatrix**)&mMatrix);
}

Int32 View::MeasureSpec::MakeMeasureSpec(
    /* [in] */ Int32 size,
    /* [in] */ Int32 mode)
{
    if (sUseBrokenMakeMeasureSpec) {
        return size + mode;
    } else {
        return (size & ~MODE_MASK) | (mode & MODE_MASK);
    }
}

/**
 * Extracts the mode from the supplied measure specification.
 *
 * @param measureSpec the measure specification to extract the mode from
 * @return {@link android.view.View.MeasureSpec#UNSPECIFIED},
 *         {@link android.view.View.MeasureSpec#AT_MOST} or
 *         {@link android.view.View.MeasureSpec#EXACTLY}
 */
Int32 View::MeasureSpec::GetMode(
    /* [in] */ Int32 measureSpec)
{
    return (measureSpec & MODE_MASK);
}

/**
 * Extracts the size from the supplied measure specification.
 *
 * @param measureSpec the measure specification to extract the size from
 * @return the size in pixels defined in the supplied measure specification
 */
Int32 View::MeasureSpec::GetSize(
    /* [in] */ Int32 measureSpec)
{
    return (measureSpec & ~MODE_MASK);
}

Int32 View::MeasureSpec::Adjust(
    /* [in] */ Int32 measureSpec,
    /* [in] */ Int32 delta)
{
    Int32 mode = GetMode(measureSpec);
    if (mode == UNSPECIFIED) {
        // No need to adjust size for UNSPECIFIED mode.
        return MakeMeasureSpec(0, UNSPECIFIED);
    }
    Int32 size = GetSize(measureSpec) + delta;
    if (size < 0) {
        /*Log.e(VIEW_LOG_TAG, "MeasureSpec.adjust: new size would be negative! (" + size +
                ") spec: " + toString(measureSpec) + " delta: " + delta);*/
        size = 0;
    }
    return MakeMeasureSpec(size, mode);
}

/**
 * Returns a String representation of the specified measure
 * specification.
 *
 * @param measureSpec the measure specification to convert to a String
 * @return a String with the following format: "MeasureSpec: MODE SIZE"
 */
ECode View::MeasureSpec::GetDescription(
    /* [in] */ Int32 measureSpec,
    /* [out] */ String* description)
{
    if (description == NULL) return E_INVALID_ARGUMENT;

    Int32 mode = GetMode(measureSpec);
    Int32 size = GetSize(measureSpec);

    StringBuilder sb("MeasureSpec: ");

    if (mode == UNSPECIFIED) {
        sb += "UNSPECIFIED ";
    }
    else if (mode == EXACTLY) {
        sb += "EXACTLY ";
    }
    else if (mode == AT_MOST) {
        sb += "AT_MOST ";
    }
    else {
        sb += mode + " ";
    }

    sb += size;
    *description = sb.ToString();
    return NOERROR;
}

ECode View::CheckForLongPress::Run()
{
    if (mView->IsPressed() && (mView->mParent != NULL)
        && mOriginalWindowAttachCount == mView->mWindowAttachCount) {
            if (mView->PerformLongClick()) {
                mView->mHasPerformedLongPress = TRUE;
            }
    }

    return NOERROR;
}

void View::CheckForLongPress::RememberWindowAttachCount()
{
    mOriginalWindowAttachCount = mView->mWindowAttachCount;
}

CAR_INTERFACE_IMPL(View::AccessibilityDelegate, IAccessibilityDelegate);

ECode View::AccessibilityDelegate::SendAccessibilityEvent(
    /* [in] */ IView* host,
    /* [in] */ Int32 eventType)
{
    VIEW_PROBE(host)->SendAccessibilityEventInternal(eventType);
    return NOERROR;
}

ECode View::AccessibilityDelegate::PerformAccessibilityAction(
    /* [in] */ IView* host,
    /* [in] */ Int32 action,
    /* [in] */ IBundle* args,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = VIEW_PROBE(host)->PerformAccessibilityActionInternal(action, args);
    return NOERROR;
}

ECode View::AccessibilityDelegate::SendAccessibilityEventUnchecked(
    /* [in] */ IView* host,
    /* [in] */ IAccessibilityEvent* event)
{
    VIEW_PROBE(host)->SendAccessibilityEventUncheckedInternal(event);
    return NOERROR;
}

ECode View::AccessibilityDelegate::DispatchPopulateAccessibilityEvent(
    /* [in] */ IView* host,
    /* [in] */ IAccessibilityEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = VIEW_PROBE(host)->DispatchPopulateAccessibilityEventInternal(event);
    return NOERROR;
}

ECode View::AccessibilityDelegate::OnPopulateAccessibilityEvent(
    /* [in] */ IView* host,
    /* [in] */ IAccessibilityEvent* event)
{
    VIEW_PROBE(host)->OnPopulateAccessibilityEventInternal(event);
    return NOERROR;
}

ECode View::AccessibilityDelegate::OnInitializeAccessibilityEvent(
    /* [in] */ IView* host,
    /* [in] */ IAccessibilityEvent* event)
{
    VIEW_PROBE(host)->OnInitializeAccessibilityEventInternal(event);
    return NOERROR;
}

ECode View::AccessibilityDelegate::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IView* host,
    /* [in] */ IAccessibilityNodeInfo* info)
{
    VIEW_PROBE(host)->OnInitializeAccessibilityNodeInfoInternal(info);
    return NOERROR;
}

ECode View::AccessibilityDelegate::OnRequestSendAccessibilityEvent(
    /* [in] */ IViewGroup* host,
    /* [in] */ IView* child,
    /* [in] */ IAccessibilityEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    ViewGroup* group = reinterpret_cast<ViewGroup*>(host->Probe(EIID_ViewGroup));
    *res = group->OnRequestSendAccessibilityEventInternal(child, event);
    return NOERROR;
}

ECode View::AccessibilityDelegate::GetAccessibilityNodeProvider(
    /* [in] */ IView* host,
    /* [out] */ IAccessibilityNodeProvider** anp)
{
    VALIDATE_NOT_NULL(anp);
    *anp = NULL;
    return NOERROR;
}

ECode View::AccessibilityDelegate::CreateAccessibilityNodeInfo(
    /* [in] */ IView* host,
    /* [out] */ IAccessibilityNodeInfo** res)
{
    VALIDATE_NOT_NULL(anp);
    View* temp = (View*)host;
    *res = temp->CreateAccessibilityNodeInfoInternal();
    return NOERROR;
}

ECode View::CheckForTap::Run()
{
    mView->mPrivateFlags &= ~PFLAG_PREPRESSED;
    mView->SetPressed(TRUE, mX, mY);
    mView->CheckForLongClick(CViewConfiguration::GetTapTimeout());

    return NOERROR;
}

ECode View::ViewPerformClick::Run()
{
    mView->PerformClick();
    return NOERROR;
}

ECode View::UnsetPressedState::Run()
{
    mView->SetPressed(FALSE);
    return NOERROR;
}

const Int32 View::AttachInfo::InvalidateInfo::POOL_LIMIT = 10;
AutoPtr<SynchronizedPool<InvalidateInfo> > View::AttachInfo::InvalidateInfo::sPool =
    new SynchronizedPool(View::AttachInfo::InvalidateInfo::POOL_LIMIT);

AutoPtr<InvalidateInfo> View::AttachInfo::InvalidateInfo::Obtain()
{
    AutoPtr<InvalidateInfo> instance = sPool->Acquire();
    return (instance != NULL) ? instance : new InvalidateInfo();
}

ECode View::AttachInfo::InvalidateInfo::Recycle()
{
    mTarget = NULL;
    sPool->Release(this);

    return NOERROR;
}

View::AttachInfo::AttachInfo(
    /* [in] */ IWindowSession* session,
    /* [in] */ IIWindow* window,
    /* [in] */ IDisplay* display,
    /* [in] */ ViewRootImpl* viewRootImpl,
    /* [in] */ IHandler* handler,
    /* [in] */ Callbacks* effectPlayer)
    : mSession(session)
    , mWindow(window)
    , mWindowToken(IBinder::Probe(window))
    , mDisplay(display)
    , mRootCallbacks(effectPlayer)
    , mHardwareAccelerated(FALSE)
    , mHardwareAccelerationRequested(FALSE)
    , mScreenOn(FALSE)
    , mDisplayState(IDisplay::STATE_UNKNOWN)
    , mApplicationScale(0.0f)
    , mScalingRequired(FALSE)
    , mTurnOffWindowResizeAnim(FALSE)
    , mWindowLeft(0)
    , mWindowTop(0)
    , mUse32BitDrawingCache(FALSE)
    , mGivenInternalInsets(NULL)
    , mHasNonEmptyGivenInternalInsets(FALSE)
    , mHasWindowFocus(FALSE)
    , mWindowVisibility(0)
    , mDrawingTime(0)
    , mIgnoreDirtyState(FALSE)
    , mSetIgnoreDirtyState(FALSE)
    , mInTouchMode(FALSE)
    , mUnbufferedDispatchRequested(FALSE)
    , mRecomputeGlobalAttributes(FALSE)
    , mForceReportNewAttributes(FALSE)
    , mKeepScreenOn(FALSE)
    , mSystemUiVisibility(0)
    , mDisabledSystemUiVisibility(0)
    , mGlobalSystemUiVisibility(0)
    , mHasSystemUiListeners(FALSE)
    , mOverscanRequested(FALSE)
    , mViewVisibilityChanged(FALSE)
    , mViewScrollChanged(FALSE)
    , mHighContrastText(FALSE)
    , mViewRootImpl(viewRootImpl)
    , mHandler(handler)
    , mAccessibilityWindowId(IAccessibilityNodeInfo::UNDEFINED_ITEM_ID)
{
    mDebugLayout = SystemProperties::GetBoolean(IView::DEBUG_LAYOUT_PROPERTY, FALSE);
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mOverscanInsets));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mContentInsets));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mVisibleInsets));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mStableInsets));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mTmpInvalRect));
    ASSERT_SUCCEEDED(CRectF::New((CRect**)&mTmpTransformRect));
    ASSERT_SUCCEEDED(CRectF::New((CRect**)&mTmpTransformRect1));
    ASSERT_SUCCEEDED(CMatrix::New((IMatrix**)&mTmpMatrix));
    ASSERT_SUCCEEDED(CTransformation::New((ITransformation**)&mTmpTransformation));
    ASSERT_SUCCEEDED(COutline::New((IOutline**)&mTmpOutline));
    ASSERT_SUCCEEDED(CPaint::New((IPaint**)&mPoint));
    CArrayList::New(24, (IArrayList**)&mTmpRectList);
    CArrayList::New(24, (IArrayList**)&mTempArrayList);

    mTreeObserver = new ViewTreeObserver();
    mGivenInternalInsets = new ViewTreeObserver::InternalInsetsInfo();

    ASSERT_SUCCEEDED(CDispatcherState::New((IDispatcherState**)&mKeyDispatchState));
    mTransparentLocation[0] = mTransparentLocation[1] = 0;
    mInvalidateChildLocation[0] = mInvalidateChildLocation[1] = 0;
    mTmpTransformLocation[0] = mTmpTransformLocation[1] = 0;
    mTmpLocation = ArrayOf<Int32>::Alloc(2);
    (*mTmpLocation)[0] = (*mTmpLocation)[1] = 0;

}

View::AttachInfo::~AttachInfo()
{
    mGivenInternalInsets = NULL;
}

static AutoPtr<ArrayOf<Float> > InitOPAQUE()
{
    AutoPtr<ArrayOf<Float> > array = ArrayOf<Float>::Alloc(1);
    (*array)[0] = 255;
    return array;
}

static AutoPtr<ArrayOf<Float> > InitTRANSPARENT()
{
    AutoPtr<ArrayOf<Float> > array = ArrayOf<Float>::Alloc(1);
    (*array)[0] = 0.0;
    return array;
}

AutoPtr<ArrayOf<Float> > View::ScrollabilityCache::OPAQUE = InitOPAQUE();
AutoPtr<ArrayOf<Float> > View::ScrollabilityCache::TRANSPARENT = InitTRANSPARENT();

View::ScrollabilityCache::ScrollabilityCache(
    /* [in] */ IViewConfiguration* configuration,
    /* [in] */ View* host)
    : mHost(host)
    , mFadeScrollBars(FALSE)
    , mFadingEdgeLength(0)
    , mScrollBarDefaultDelayBeforeFade(0)
    , mScrollBarFadeDuration(0)
    , mScrollBarSize(0)
    , mFadeStartTime(0)
    , mState(OFF)
    , mLastColor(0)
{
    CInterpolator::New(1, 2, (IInterpolator**)&mScrollBarInterpolator);
    configuration->GetScaledFadingEdgeLength(&mFadingEdgeLength);
    configuration->GetScaledScrollBarSize(&mScrollBarSize);
    mScrollBarDefaultDelayBeforeFade = CViewConfiguration::GetScrollDefaultDelay();
    mScrollBarFadeDuration = CViewConfiguration::GetScrollBarFadeDuration();

    CPaint::New((IPaint**)&mPaint);
    CMatrix::New((IMatrix**)&mMatrix);
    // use use a height of 1, and then wack the matrix each time we
    // actually use it.
    AutoPtr<ILinearGradient> lg;
    CLinearGradient::New(0, 0, 0, 1, 0xFF000000, 0, ShaderTileMode_CLAMP, (ILinearGradient**)&lg);
    mShader = IShader::Probe(lg.Get());

    mPaint->SetShader(mShader.Get());
    AutoPtr<IPorterDuffXfermode> mode;
    CPorterDuffXfermode::New(PorterDuffMode_DST_OUT, (IPorterDuffXfermode**)&mode);
    mPaint->SetXfermode(mode.Get());
}

ECode View::ScrollabilityCache::Run()
{
    Int64 now = AnimationUtils::CurrentAnimationTimeMillis();
    if (now >= mFadeStartTime) {
        // the animation fades the scrollbars out by changing
        // the opacity (alpha) from fully opaque to fully
        // transparent
        Int32 nextFrame = (Int32)now;
        Int32 framesCount = 0;

        // Start opaque
        AutoPtr<ArrayOf<Float> > tmp1 = ArrayOf<Float>::Alloc(1);
        (*tmp1)[0] = (*OPAQUE)[0];
        if(mScrollBarInterpolator)
        {
            mScrollBarInterpolator->SetKeyFrame(framesCount++, nextFrame, *tmp1);
        }

        // End transparent
        nextFrame += mScrollBarFadeDuration;
        (*tmp1)[0] = (*TRANSPARENT)[0];
        mScrollBarInterpolator->SetKeyFrame(framesCount, nextFrame, *tmp1);

        mState = FADING;

        // Kick off the fade animation
        mHost->Invalidate(TRUE);
    }
    return NOERROR;
}

void View::ScrollabilityCache::SetFadeColor(
    /* [in] */ Int32 color)
{
    if (color != mLastColor) {
        mLastColor = color;

        AutoPtr<ILinearGradient> lg;
        if (color != 0) {
            CLinearGradient::New(0, 0, 0, 1, color | 0xFF000000,
                    color & 0x00FFFFFF, ShaderTileMode_CLAMP, (ILinearGradient**)&lg);
            mShader = IShader::Probe(lg.Get());
            mPaint->SetShader(mShader);
            // Restore the default transfer mode (src_over)
            mPaint->SetXfermode(NULL);
        }
        else {
            CLinearGradient::New(0, 0, 0, 1, 0xFF000000, 0, ShaderTileMode_CLAMP, (ILinearGradient**)&lg);
            mShader = IShader::Probe(lg.Get());
            mPaint->SetShader(mShader);

            AutoPtr<IPorterDuffXfermode> mode;
            CPorterDuffXfermode::New(PorterDuffMode_DST_OUT, (IPorterDuffXfermode**)&mode);
            mPaint->SetXfermode(mode);
        }
    }
}

CAR_INTERFACE_IMPL(View::_Predicate, IPredicate);

View::_Predicate::_Predicate(
    /* [in] */ Int32 id)
    : mId(id)
{}

ECode View::_Predicate::Apply(
    /* [in] */ IInterface* t,
    /* [out] */ Boolean* result)
{
    assert(result != NULL && t != NULL && IView::Probe(t) != NULL);
    View* v = VIEW_PROBE(t);
    assert(v != NULL);
    *result = v->mNextFocusForwardId == mId;
    return NOERROR;
}

// View::_FloatProperty::_FloatProperty(
//     /* [in] */ const String& name)
//     : mName(name)
// {
// }

// UInt32 View::_FloatProperty::AddRef()
// {
//     return ElRefBas::AddRef();
// }

// UInt32 View::_FloatProperty::Release()
// {
//     return ElRefBas::Release();
// }

// ECode View::_FloatProperty::SetValue(
//     /* [in] */ IView* object,
//     /* [in] */ Float value)
// {
//     assert(object != NULL);
//     if (mName.Equals("alpha") == 0) {
//         object->SetAlpha(value);
//     }
//     else if (mName.Equals("translationX") == 0) {
//         object->SetTranslationX(value);
//     }
//     else if (mName.Equals("translationY") == 0) {
//         object->SetTranslationY(value);
//     }
//     else if (mName.Equals("x") == 0) {
//         object->SetX(value);
//     }
//     else if (mName.Equals("y") == 0) {
//         object->SetY(value);
//     }
//     else if (mName.Equals("rotation") == 0) {
//         object->SetRotation(value);
//     }
//     else if (mName.Equals("rotationX") == 0) {
//         object->SetRotationX(value);
//     }
//     else if (mName.Equals("rotationY") == 0) {
//         object->SetRotationY(value);
//     }
//     else if (mName.Equals("scaleX") == 0) {
//         object->SetScaleX(value);
//     }
//     else if (mName.Equals("scaleY") == 0) {
//         object->SetScaleY(value);
//     }
//     else {
//         assert(0);
//     }

//     return NOERROR;
// }

// ECode View::_FloatProperty::Get(
//     /* [in] */ IView* object,
//     /* [out] */ Float* value)
// {
//     assert(value != NULL && object != NULL);
//     if (mName.Equals("alpha") == 0) {
//         object->GetAlpha(value);
//     }
//     else if (mName.Equals("translationX") == 0) {
//         object->GetTranslationX(value);
//     }
//     else if (mName.Equals("translationY") == 0) {
//         object->GetTranslationY(value);
//     }
//     else if (mName.Equals("x") == 0) {
//         object->GetX(value);
//     }
//     else if (mName.Equals("y") == 0) {
//         object->GetY(value);
//     }
//     else if (mName.Equals("rotation") == 0) {
//         object->GetRotation(value);
//     }
//     else if (mName.Equals("rotationX") == 0) {
//         object->GetRotationX(value);
//     }
//     else if (mName.Equals("rotationY") == 0) {
//         object->GetRotationY(value);
//     }
//     else if (mName.Equals("scaleX") == 0) {
//         object->GetScaleX(value);
//     }
//     else if (mName.Equals("scaleY") == 0) {
//         object->GetScaleY(value);
//     }
//     else {
//         assert(0);
//     }

//     return NOERROR;
// }

View::SendViewScrolledAccessibilityEvent::SendViewScrolledAccessibilityEvent(
    /* [in] */ View* host)
    : mHost(host)
{
}

ECode View::SendViewScrolledAccessibilityEvent::Run()
{
    mHost->SendAccessibilityEvent(IAccessibilityEvent::TYPE_VIEW_SCROLLED);
    mIsPending = FALSE;
    return NOERROR;
}

CAR_INTERFACE_IMPL(View::MatchIdPredicate, IPredicate);

View::MatchIdPredicate::MatchIdPredicate()
    : mId(0)
{
}

ECode View::MatchIdPredicate::Apply(
    /* [in] */ IInterface* view,
    /* [out] */ Boolean* apply)
{
    assert(apply != NULL && view != NULL && IView::Probe(view) != NULL);
    *apply = VIEW_PROBE(view)->mID == mId;
    return NOERROR;
}

CAR_INTERFACE_IMPL(View::MatchLabelForPredicate, IPredicate);

View::MatchLabelForPredicate::MatchLabelForPredicate()
    : mLabeledId(0)
{
}

ECode View::MatchLabelForPredicate::Apply(
    /* [in] */ IInterface* view,
    /* [out] */ Boolean* apply)
{
    assert(apply != NULL && view != NULL && IView::Probe(view) != NULL);
    *apply = VIEW_PROBE(view)->mLabelForId == mLabeledId;
    return NOERROR;
}

/////////////////////////////////////////////////////////////////////
//           View::SendViewStateChangedAccessibilityEvent
/////////////////////////////////////////////////////////////////////
View::SendViewStateChangedAccessibilityEvent::SendViewStateChangedAccessibilityEvent(
    /* [in] */ View* host)
    : Runnable()
    , mHost(host)
    , mChangeTypes(0)
    , mPosted(FALSE)
    , mPostedWithDelay(FALSE)
    , mLastEventTimeMillis(0)
{
}

ECode View::SendViewStateChangedAccessibilityEvent::Run()
{
    mPosted = FALSE;
    mPostedWithDelay = FALSE;
    mLastEventTimeMillis = SystemClock::GetUptimeMillis();
    AutoPtr<IAccessibilityManager> manager;
    CAccessibilityManager::GetInstance(mContext, (IAccessibilityManager**)&manager);
    Boolean isEnabled;
    manager->IsEnabled(&isEnabled);
    if (isEnabled) {
        AutoPtr<IAccessibilityEvent> event;
        CAccessibilityEvent::Obtain((IAccessibilityEvent**)&event);
        event->SetEventType(IAccessibilityEvent::TYPE_WINDOW_CONTENT_CHANGED);
        event->SetContentChangeTypes(mChangeTypes);
        mHost->SendAccessibilityEventUnchecked(event);
    }
    mChangeTypes = 0;
    return NOERROR;
}

ECode View::SendViewStateChangedAccessibilityEvent::RunOrPost(
    /* [in] */ Int32 changeType)
{
    mChangeTypes |= changeType;

    // If this is a live region or the child of a live region, collect
    // all events from this frame and send them on the next frame.
    if (mHost->InLiveRegion()) {
        // If we're already posted with a delay, remove that.
        if (mPostedWithDelay) {
            mHost->RemoveCallbacks(THIS_PROBE(IRunnable));
            mPostedWithDelay = FALSE;
        }
        // Only post if we're not already posted.
        if (!mPosted) {
            mHost->Post(THIS_PROBE(IRunnable));
            mPosted = TRUE;
        }
        return NOERROR;
    }

    if (mPosted) {
        return NOERROR;
    }

    Int64 timeSinceLastMillis = SystemClock::GetUptimeMillis() - mLastEventTimeMillis;
    Int64 minEventIntevalMillis = CViewConfiguration::GetSendRecurringAccessibilityEventsInterval();
    if (timeSinceLastMillis >= minEventIntevalMillis) {
        mHost->RemoveCallbacks(THIS_PROBE(IRunnable));
        Run();
    } else {
        mHost->PostDelayed(THIS_PROBE(IRunnable), minEventIntevalMillis - timeSinceLastMillis);
        mPostedWithDelay = TRUE;
    }
    return NOERROR;
}

/////////////////////////////////////////////////////////////////////
//                          View
/////////////////////////////////////////////////////////////////////
View::View()
    : mCachingFailed(FALSE)
    , mMeasuredWidth(0)
    , mMeasuredHeight(0)
    , mRecreateDisplayList(FALSE)
    , mID(IView::NO_ID)
    , mAccessibilityViewId(IView::NO_ID)
    , mAccessibilityCursorPosition(IView::ACCESSIBILITY_CURSOR_POSITION_UNDEFINED)
    , mTag(NULL)
    , mOverScrollMode(0)
    , mParent(NULL)
    , mPrivateFlags(0)
    , mPrivateFlags2(0)
    , mPrivateFlags3(0)
    , mSystemUiVisibility(0)
    , mTransientStateCount(0)
    , mWindowAttachCount(0)
    , mViewFlags(0)
    , mLastIsOpaque(FALSE)
    , mLeft(0)
    , mRight(0)
    , mTop(0)
    , mBottom(0)
    , mScrollX(0)
    , mScrollY(0)
    , mPaddingLeft(0)
    , mPaddingRight(0)
    , mPaddingTop(0)
    , mPaddingBottom(0)
    , mUserPaddingRight(0)
    , mUserPaddingBottom(0)
    , mUserPaddingLeft(0)
    , mUserPaddingStart(0)
    , mUserPaddingEnd(0)
    , mUserPaddingLeftInitial(0)
    , mUserPaddingRightInitial(0)
    , mOldWidthMeasureSpec(Elastos::Core::Math::INT32_MIN_VALUE)
    , mOldHeightMeasureSpec(Elastos::Core::Math::INT32_MIN_VALUE)
    , mLabelForId(IView::NO_ID)
    , mBackgroundResource(0)
    , mBackgroundSizeChanged(FALSE)
    , mNextFocusLeftId(IView::NO_ID)
    , mNextFocusRightId(IView::NO_ID)
    , mNextFocusUpId(IView::NO_ID)
    , mNextFocusDownId(IView::NO_ID)
    , mNextFocusForwardId(IView::NO_ID)
    , mHasPerformedLongPress(FALSE)
    , mMinHeight(0)
    , mMinWidth(0)
    , mDrawingCacheBackgroundColor(0)
    , mTouchSlop(0)
    , mVerticalScrollFactor(0.0f)
    , mVerticalScrollbarPosition(0)
    , mLayerType(IView::LAYER_TYPE_NONE)
    , mSendingHoverAccessibilityEvents(FALSE)
    , mLeftPaddingDefined(FALSE)
    , mRightPaddingDefined(FALSE)
    //, mOutlineProvider(ViewOutlineProvider::BACKGROUND)
{
    if (InputEventConsistencyVerifier::IsInstrumentationEnabled()) {
        mInputEventConsistencyVerifier = new InputEventConsistencyVerifier((IInterface*)this, 0);
    }

    mRenderNode = RenderNode::Create(String("View"), THIS_PROBE(IView));
}

View::View(
    /* [in] */ IContext* context)
    : mCachingFailed(FALSE)
    , mMeasuredWidth(0)
    , mMeasuredHeight(0)
    , mRecreateDisplayList(FALSE)
    , mID(IView::NO_ID)
    , mAccessibilityViewId(IView::NO_ID)
    , mAccessibilityCursorPosition(IView::ACCESSIBILITY_CURSOR_POSITION_UNDEFINED)
    , mOverScrollMode(0)
    , mParent(NULL)
    , mPrivateFlags(0)
    , mPrivateFlags2(0)
    , mPrivateFlags3(0)
    , mSystemUiVisibility(0)
    , mTransientStateCount(0)
    , mWindowAttachCount(0)
    , mViewFlags(0)
    , mLastIsOpaque(FALSE)
    , mLeft(0)
    , mRight(0)
    , mTop(0)
    , mBottom(0)
    , mScrollX(0)
    , mScrollY(0)
    , mPaddingLeft(0)
    , mPaddingRight(0)
    , mPaddingTop(0)
    , mPaddingBottom(0)
    , mUserPaddingRight(0)
    , mUserPaddingBottom(0)
    , mUserPaddingLeft(0)
    , mUserPaddingStart(0)
    , mUserPaddingEnd(0)
    , mUserPaddingLeftInitial(0)
    , mUserPaddingRightInitial(0)
    , mOldWidthMeasureSpec(Elastos::Core::Math::INT32_MIN_VALUE)
    , mOldHeightMeasureSpec(Elastos::Core::Math::INT32_MIN_VALUE)
    , mLabelForId(IView::NO_ID)
    , mBackgroundResource(0)
    , mBackgroundSizeChanged(FALSE)
    , mNextFocusLeftId(IView::NO_ID)
    , mNextFocusRightId(IView::NO_ID)
    , mNextFocusUpId(IView::NO_ID)
    , mNextFocusDownId(IView::NO_ID)
    , mNextFocusForwardId(IView::NO_ID)
    , mHasPerformedLongPress(FALSE)
    , mMinHeight(0)
    , mMinWidth(0)
    , mDrawingCacheBackgroundColor(0)
    , mTouchSlop(0)
    , mVerticalScrollFactor(0.0f)
    , mVerticalScrollbarPosition(0)
    , mLayerType(IView::LAYER_TYPE_NONE)
    , mSendingHoverAccessibilityEvents(FALSE)
    , mLeftPaddingDefined(FALSE)
    , mRightPaddingDefined(FALSE)
    //, mOutlineProvider(ViewOutlineProvider::BACKGROUND)
{
    if (InputEventConsistencyVerifier::IsInstrumentationEnabled()) {
        mInputEventConsistencyVerifier = new InputEventConsistencyVerifier((IInterface*)this, 0);
    }

    Init(context);
}

View::View(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mCachingFailed(FALSE)
    , mMeasuredWidth(0)
    , mMeasuredHeight(0)
    , mRecreateDisplayList(FALSE)
    , mID(IView::NO_ID)
    , mAccessibilityViewId(IView::NO_ID)
    , mAccessibilityCursorPosition(IView::ACCESSIBILITY_CURSOR_POSITION_UNDEFINED)
    , mOverScrollMode(0)
    , mParent(NULL)
    , mPrivateFlags(0)
    , mPrivateFlags2(0)
    , mPrivateFlags3(0)
    , mSystemUiVisibility(0)
    , mTransientStateCount(0)
    , mWindowAttachCount(0)
    , mViewFlags(0)
    , mLastIsOpaque(FALSE)
    , mLeft(0)
    , mRight(0)
    , mTop(0)
    , mBottom(0)
    , mScrollX(0)
    , mScrollY(0)
    , mPaddingLeft(0)
    , mPaddingRight(0)
    , mPaddingTop(0)
    , mPaddingBottom(0)
    , mUserPaddingRight(0)
    , mUserPaddingBottom(0)
    , mUserPaddingLeft(0)
    , mUserPaddingStart(0)
    , mUserPaddingEnd(0)
    , mUserPaddingLeftInitial(0)
    , mUserPaddingRightInitial(0)
    , mOldWidthMeasureSpec(Elastos::Core::Math::INT32_MIN_VALUE)
    , mOldHeightMeasureSpec(Elastos::Core::Math::INT32_MIN_VALUE)
    , mLabelForId(IView::NO_ID)
    , mBackgroundResource(0)
    , mBackgroundSizeChanged(FALSE)
    , mNextFocusLeftId(IView::NO_ID)
    , mNextFocusRightId(IView::NO_ID)
    , mNextFocusUpId(IView::NO_ID)
    , mNextFocusDownId(IView::NO_ID)
    , mNextFocusForwardId(IView::NO_ID)
    , mHasPerformedLongPress(FALSE)
    , mMinHeight(0)
    , mMinWidth(0)
    , mDrawingCacheBackgroundColor(0)
    , mTouchSlop(0)
    , mVerticalScrollFactor(0.0f)
    , mVerticalScrollbarPosition(0)
    , mLayerType(IView::LAYER_TYPE_NONE)
    , mSendingHoverAccessibilityEvents(FALSE)
    , mLeftPaddingDefined(FALSE)
    , mRightPaddingDefined(FALSE)
    //, mOutlineProvider(ViewOutlineProvider::BACKGROUND)
{
    if (InputEventConsistencyVerifier::IsInstrumentationEnabled()) {
        mInputEventConsistencyVerifier = new InputEventConsistencyVerifier((IInterface*)this, 0);
    }

    Init(context, attrs, 0);
}

View::View(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
    : mCachingFailed(FALSE)
    , mMeasuredWidth(0)
    , mMeasuredHeight(0)
    , mRecreateDisplayList(FALSE)
    , mID(IView::NO_ID)
    , mAccessibilityViewId(IView::NO_ID)
    , mAccessibilityCursorPosition(IView::ACCESSIBILITY_CURSOR_POSITION_UNDEFINED)
    , mOverScrollMode(0)
    , mParent(NULL)
    , mPrivateFlags(0)
    , mPrivateFlags2(0)
    , mPrivateFlags3(0)
    , mSystemUiVisibility(0)
    , mTransientStateCount(0)
    , mWindowAttachCount(0)
    , mViewFlags(0)
    , mLastIsOpaque(FALSE)
    , mLeft(0)
    , mRight(0)
    , mTop(0)
    , mBottom(0)
    , mScrollX(0)
    , mScrollY(0)
    , mPaddingLeft(0)
    , mPaddingRight(0)
    , mPaddingTop(0)
    , mPaddingBottom(0)
    , mUserPaddingRight(0)
    , mUserPaddingBottom(0)
    , mUserPaddingLeft(0)
    , mUserPaddingStart(0)
    , mUserPaddingEnd(0)
    , mUserPaddingLeftInitial(0)
    , mUserPaddingRightInitial(0)
    , mOldWidthMeasureSpec(Elastos::Core::Math::INT32_MIN_VALUE)
    , mOldHeightMeasureSpec(Elastos::Core::Math::INT32_MIN_VALUE)
    , mLabelForId(IView::NO_ID)
    , mBackgroundResource(0)
    , mBackgroundSizeChanged(FALSE)
    , mNextFocusLeftId(IView::NO_ID)
    , mNextFocusRightId(IView::NO_ID)
    , mNextFocusUpId(IView::NO_ID)
    , mNextFocusDownId(IView::NO_ID)
    , mNextFocusForwardId(IView::NO_ID)
    , mHasPerformedLongPress(FALSE)
    , mMinHeight(0)
    , mMinWidth(0)
    , mDrawingCacheBackgroundColor(0)
    , mTouchSlop(0)
    , mVerticalScrollFactor(0.0f)
    , mVerticalScrollbarPosition(0)
    , mLayerType(IView::LAYER_TYPE_NONE)
    , mSendingHoverAccessibilityEvents(FALSE)
    , mLeftPaddingDefined(FALSE)
    , mRightPaddingDefined(FALSE)
    //, mOutlineProvider(ViewOutlineProvider::BACKGROUND)
{
    if (InputEventConsistencyVerifier::IsInstrumentationEnabled()) {
        mInputEventConsistencyVerifier = new InputEventConsistencyVerifier((IInterface*)this, 0);
    }

    Init(context, attrs, defStyleAttr, 0);
}

View::View(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
    : mCachingFailed(FALSE)
    , mMeasuredWidth(0)
    , mMeasuredHeight(0)
    , mRecreateDisplayList(FALSE)
    , mID(IView::NO_ID)
    , mAccessibilityViewId(IView::NO_ID)
    , mAccessibilityCursorPosition(IView::ACCESSIBILITY_CURSOR_POSITION_UNDEFINED)
    , mOverScrollMode(0)
    , mParent(NULL)
    , mPrivateFlags(0)
    , mPrivateFlags2(0)
    , mPrivateFlags3(0)
    , mSystemUiVisibility(0)
    , mTransientStateCount(0)
    , mWindowAttachCount(0)
    , mViewFlags(0)
    , mLastIsOpaque(FALSE)
    , mLeft(0)
    , mRight(0)
    , mTop(0)
    , mBottom(0)
    , mScrollX(0)
    , mScrollY(0)
    , mPaddingLeft(0)
    , mPaddingRight(0)
    , mPaddingTop(0)
    , mPaddingBottom(0)
    , mUserPaddingRight(0)
    , mUserPaddingBottom(0)
    , mUserPaddingLeft(0)
    , mUserPaddingStart(0)
    , mUserPaddingEnd(0)
    , mUserPaddingLeftInitial(0)
    , mUserPaddingRightInitial(0)
    , mOldWidthMeasureSpec(Elastos::Core::Math::INT32_MIN_VALUE)
    , mOldHeightMeasureSpec(Elastos::Core::Math::INT32_MIN_VALUE)
    , mLabelForId(IView::NO_ID)
    , mBackgroundResource(0)
    , mBackgroundSizeChanged(FALSE)
    , mNextFocusLeftId(IView::NO_ID)
    , mNextFocusRightId(IView::NO_ID)
    , mNextFocusUpId(IView::NO_ID)
    , mNextFocusDownId(IView::NO_ID)
    , mNextFocusForwardId(IView::NO_ID)
    , mHasPerformedLongPress(FALSE)
    , mMinHeight(0)
    , mMinWidth(0)
    , mDrawingCacheBackgroundColor(0)
    , mTouchSlop(0)
    , mVerticalScrollFactor(0.0f)
    , mVerticalScrollbarPosition(0)
    , mLayerType(IView::LAYER_TYPE_NONE)
    , mSendingHoverAccessibilityEvents(FALSE)
    , mLeftPaddingDefined(FALSE)
    , mRightPaddingDefined(FALSE)
    //, mOutlineProvider(ViewOutlineProvider::BACKGROUND)
{
    if (InputEventConsistencyVerifier::IsInstrumentationEnabled()) {
        mInputEventConsistencyVerifier = new InputEventConsistencyVerifier((IInterface*)this, 0);
    }

    Init(context, attrs, defStyleAttr, defStyleRes);
}

View::~View()
{
    if (!mKeyedTags.IsEmpty()) {
        Logger::D("View", "===========View::~View() %p, mID: %08x", this, mID);
    }

    mKeyedTags.Clear();
    mInputEventConsistencyVerifier = NULL;
    mAccessibilityDelegate = NULL;
    mLayerPaint = NULL;
    mAnimator = NULL;
    mFloatingTreeObserver = NULL;
    mTouchDelegate = NULL;
    mUnsetPressedState = NULL;
    mSendViewScrolledAccessibilityEvent = NULL;
    mPerformClick = NULL;
    mPendingCheckForTap = NULL;
    mPendingCheckForLongPress = NULL;
    mUnscaledDrawingCache = NULL;
    mDrawingCache = NULL;
    mDrawableState = NULL;
    mScrollCache = NULL;
    mListenerInfo = NULL;
    mBackground = NULL;
    mBGDrawable = NULL;
    mResources = NULL;
    mMatchIdPredicate = NULL;
    mMatchLabelForPredicate = NULL;
    mContentDescription = NULL;
    mLayoutInsets = NULL;
    mTransformationInfo = NULL;
    mLayoutParams = NULL;
    mTag = NULL;
    mCurrentAnimation = NULL;
}

/**
 * <p>
 * Initializes the fading edges from a given set of styled attributes. This
 * method should be called by subclasses that need fading edges and when an
 * instance of these subclasses is created programmatically rather than
 * being inflated from XML. This method is automatically called when the XML
 * is inflated.
 * </p>
 *
 * @param a the styled attributes set to initialize the fading edges from
 */
void View::InitializeFadingEdge(
    /* [in] */ ITypedArray* a)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::View),
        ARRAY_SIZE(R::styleable::View));
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(mContext->ObtainStyledAttributes(attrIds, (ITypedArray**)&a));

    InitializeFadingEdgeInternal(a);

    a->Recycle();
}

/**
 * <p>
 * Initializes the fading edges from a given set of styled attributes. This
 * method should be called by subclasses that need fading edges and when an
 * instance of these subclasses is created programmatically rather than
 * being inflated from XML. This method is automatically called when the XML
 * is inflated.
 * </p>
 *
 * @param a the styled attributes set to initialize the fading edges from
 * @hide This is the real method; the public one is shimmed to be safe to call from apps.
 */
void View::InitializeFadingEdgeInternal(
    /* [in] */ ITypedArray* a)
{
    InitScrollCache();

    Int32 length;
    CViewConfiguration::Get(mContext)->GetScaledFadingEdgeLength(&length);
    ASSERT_SUCCEEDED(a->GetDimensionPixelSize(
        R::styleable::View_fadingEdgeLength, length,
        &mScrollCache->mFadingEdgeLength));
}

/**
 * Returns the size of the vertical faded edges used to indicate that more
 * content in this view is visible.
 *
 * @return The size in pixels of the vertical faded edge or 0 if vertical
 *         faded edges are not enabled for this view.
 * @attr ref android.R.styleable#View_fadingEdgeLength
 */
Int32 View::GetVerticalFadingEdgeLength()
{
    if (IsVerticalFadingEdgeEnabled()) {
        AutoPtr<ScrollabilityCache> cache = mScrollCache;
        if (cache != NULL) {
            return cache->mFadingEdgeLength;
        }
    }
    return 0;
}

/**
 * Set the size of the faded edge used to indicate that more content in this
 * view is available.  Will not change whether the fading edge is enabled; use
 * {@link #setVerticalFadingEdgeEnabled} or {@link #setHorizontalFadingEdgeEnabled}
 * to enable the fading edge for the vertical or horizontal fading edges.
 *
 * @param length The size in pixels of the faded edge used to indicate that more
 *        content in this view is visible.
 */
ECode View::SetFadingEdgeLength(
    /* [in] */ Int32 length)
{
    InitScrollCache();
    mScrollCache->mFadingEdgeLength = length;

    return NOERROR;
}

/**
 * Returns the size of the horizontal faded edges used to indicate that more
 * content in this view is visible.
 *
 * @return The size in pixels of the horizontal faded edge or 0 if horizontal
 *         faded edges are not enabled for this view.
 * @attr ref android.R.styleable#View_fadingEdgeLength
 */
Int32 View::GetHorizontalFadingEdgeLength()
{
    if (IsHorizontalFadingEdgeEnabled()) {
        AutoPtr<ScrollabilityCache> cache = mScrollCache;
        if (cache != NULL) {
            return cache->mFadingEdgeLength;
        }
    }
    return 0;
}

/**
 * Returns the width of the vertical scrollbar.
 *
 * @return The width in pixels of the vertical scrollbar or 0 if there
 *         is no vertical scrollbar.
 */
Int32 View::GetVerticalScrollbarWidth()
{
    AutoPtr<ScrollabilityCache> cache = mScrollCache;
    if (cache != NULL) {
        AutoPtr<IScrollBarDrawable> scrollBar = cache->mScrollBar;
        if (scrollBar != NULL) {
            Int32 size;
            scrollBar->GetSize(TRUE, &size);
            if (size <= 0) {
                size = cache->mScrollBarSize;
            }
            return size;
        }
        return 0;
    }
    return 0;
}

/**
 * Returns the height of the horizontal scrollbar.
 *
 * @return The height in pixels of the horizontal scrollbar or 0 if
 *         there is no horizontal scrollbar.
 */
Int32 View::GetHorizontalScrollbarHeight()
{
    AutoPtr<ScrollabilityCache> cache = mScrollCache;
    if (cache != NULL) {
        AutoPtr<IScrollBarDrawable> scrollBar = cache->mScrollBar;
        if (scrollBar != NULL) {
            Int32 size;
            scrollBar->GetSize(FALSE, &size);
            if (size <= 0) {
                size = cache->mScrollBarSize;
            }
            return size;
        }
        return 0;
    }
    return 0;
}

/**
  * <p>
  * Initializes the scrollbars from a given set of styled attributes. This
  * method should be called by subclasses that need scrollbars and when an
  * instance of these subclasses is created programmatically rather than
  * being inflated from XML. This method is automatically called when the XML
  * is inflated.
  * </p>
  *
  * @param a the styled attributes set to initialize the scrollbars from
  */
void View::InitializeScrollbars(
    /* [in] */ ITypedArray* a)
{
    // It's not safe to use this method from apps. The parameter 'a' must have been obtained
    // using the View filter array which is not available to the SDK. As such, internal
    // framework usage now uses initializeScrollbarsInternal and we grab a default
    // TypedArray with the right filter instead here.
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::View),
        ARRAY_SIZE(R::styleable::View));
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(mContext->ObtainStyledAttributes(attrIds, (ITypedArray**)&a));

    InitializeScrollbarsInternal(a);

    a->Recycle();
}

/**
 * <p>
 * Initializes the scrollbars from a given set of styled attributes. This
 * method should be called by subclasses that need scrollbars and when an
 * instance of these subclasses is created programmatically rather than
 * being inflated from XML. This method is automatically called when the XML
 * is inflated.
 * </p>
 *
 * @param a the styled attributes set to initialize the scrollbars from
 * @hide
 */
void View::InitializeScrollbarsInternal(
    /* [in] */ ITypedArray* a)
{
    InitScrollCache();

    AutoPtr<ScrollabilityCache> scrollabilityCache = mScrollCache;

    if (scrollabilityCache->mScrollBar == NULL) {
         ASSERT_SUCCEEDED(CScrollBarDrawable::New(
                 (IScrollBarDrawable**)&(scrollabilityCache->mScrollBar)));
    }

    Boolean fadeScrollbars;
    a->GetBoolean(R::styleable::View_fadeScrollbars, TRUE, &fadeScrollbars);

    if (!fadeScrollbars) {
        scrollabilityCache->mState = ScrollabilityCache::ON;
    }
    scrollabilityCache->mFadeScrollBars = fadeScrollbars;


    a->GetInt32(R::styleable::View_scrollbarFadeDuration,
            CViewConfiguration::GetScrollBarFadeDuration(),
            &(scrollabilityCache->mScrollBarFadeDuration));
    a->GetInt32(R::styleable::View_scrollbarDefaultDelayBeforeFade,
            CViewConfiguration::GetScrollDefaultDelay(),
            &(scrollabilityCache->mScrollBarDefaultDelayBeforeFade));

    Int32 scrollBarSize;
    CViewConfiguration::Get(mContext)->GetScaledScrollBarSize(&scrollBarSize);
    a->GetDimensionPixelSize(R::styleable::View_scrollbarSize,
            scrollBarSize, &(scrollabilityCache->mScrollBarSize));

    AutoPtr<IDrawable> track;
    a->GetDrawable(R::styleable::View_scrollbarTrackHorizontal, (IDrawable**)&track);
    scrollabilityCache->mScrollBar->SetHorizontalTrackDrawable(track.Get());

    AutoPtr<IDrawable> thumb;
    a->GetDrawable(R::styleable::View_scrollbarThumbHorizontal, (IDrawable**)&thumb);
    if (thumb != NULL) {
        scrollabilityCache->mScrollBar->SetHorizontalThumbDrawable(thumb.Get());
    }

    Boolean alwaysDraw;
    a->GetBoolean(R::styleable::View_scrollbarAlwaysDrawHorizontalTrack,
            FALSE, &alwaysDraw);
    if (alwaysDraw) {
        scrollabilityCache->mScrollBar->SetAlwaysDrawHorizontalTrack(TRUE);
    }

    track = NULL;
    a->GetDrawable(R::styleable::View_scrollbarTrackVertical, (IDrawable**)&track);
    scrollabilityCache->mScrollBar->SetVerticalTrackDrawable(track.Get());

    thumb = NULL;
    a->GetDrawable(R::styleable::View_scrollbarThumbVertical, (IDrawable**)&thumb);
    if (thumb != NULL) {
        scrollabilityCache->mScrollBar->SetVerticalThumbDrawable(thumb.Get());
    }

    a->GetBoolean(R::styleable::View_scrollbarAlwaysDrawVerticalTrack,
            FALSE, &alwaysDraw);
    if (alwaysDraw) {
        scrollabilityCache->mScrollBar->SetAlwaysDrawVerticalTrack(TRUE);
    }

    // Apply layout direction to the new Drawables if needed
    Int32 layoutDirection = GetLayoutDirection();
    if (track != NULL) {
        track->SetLayoutDirection(layoutDirection);
    }
    if (thumb != NULL) {
        thumb->SetLayoutDirection(layoutDirection);
    }

    // Re-apply user/background padding so that scrollbar(s) get added
    ResolvePadding();
}

/**
 * <p>
 * Initalizes the scrollability cache if necessary.
 * </p>
 */
void View::InitScrollCache()
{
    if (mScrollCache == NULL) {
        AutoPtr<CViewConfiguration> configuration = CViewConfiguration::Get(mContext);
        mScrollCache = new ScrollabilityCache(configuration, this);
    }
}

AutoPtr<View::ScrollabilityCache> View::GetScrollCache()
{
    InitScrollCache();
    return mScrollCache;
}

/**
 * Set the position of the vertical scroll bar. Should be one of
 * {@link #SCROLLBAR_POSITION_DEFAULT}, {@link #SCROLLBAR_POSITION_LEFT} or
 * {@link #SCROLLBAR_POSITION_RIGHT}.
 *
 * @param position Where the vertical scroll bar should be positioned.
 */
ECode View::SetVerticalScrollbarPosition(
    /* [in] */ Int32 position)
{
    if (mVerticalScrollbarPosition != position) {
        mVerticalScrollbarPosition = position;
        ComputeOpaqueFlags();
        ResolvePadding();
    }

    return NOERROR;
}

/**
 * @return The position where the vertical scroll bar will show, if applicable.
 * @see #setVerticalScrollbarPosition(int)
 */
Int32 View::GetVerticalScrollbarPosition()
{
    return mVerticalScrollbarPosition;
}

AutoPtr<View::ListenerInfo> View::GetListenerInfo()
{
    if (mListenerInfo != NULL) {
        return mListenerInfo;
    }

    mListenerInfo = new ListenerInfo();
    return mListenerInfo;
}

/**
 * Register a callback to be invoked when focus of this view changed.
 *
 * @param l The callback that will run.
 */
ECode View::SetOnFocusChangeListener(
    /* [in] */ IViewOnFocusChangeListener* l)
{
    GetListenerInfo()->mOnFocusChangeListener = l;

    return NOERROR;
}

/**
 * Add a listener that will be called when the bounds of the view change due to
 * layout processing.
 *
 * @param listener The listener that will be called when layout bounds change.
 */
ECode View::AddOnLayoutChangeListener(
    /* [in] */ IViewOnLayoutChangeListener* listener)
{
    AutoPtr<ListenerInfo> li = GetListenerInfo();
    List<AutoPtr<IViewOnLayoutChangeListener> >::Iterator item = li->mOnLayoutChangeListeners.Begin();
    Boolean has = FALSE;
    for (; item != li->mOnLayoutChangeListeners.End(); ++item) {
        if (item->Get() == listener) {
            has = TRUE;
            break;
        }
    }

    if (!has) {
        li->mOnLayoutChangeListeners.PushBack(listener);
    }

    return NOERROR;
}

/**
 * Remove a listener for layout changes.
 *
 * @param listener The listener for layout bounds change.
 */
ECode View::RemoveOnLayoutChangeListener(
    /* [in] */ IViewOnLayoutChangeListener* listener)
{
    AutoPtr<ListenerInfo> li = mListenerInfo;
    if (li == NULL || li->mOnLayoutChangeListeners.IsEmpty()) {
        return NOERROR;
    }

    li->mOnLayoutChangeListeners.Remove(listener);
    return NOERROR;
}

/**
 * Add a listener for attach state changes.
 *
 * This listener will be called whenever this view is attached or detached
 * from a window. Remove the listener using
 * {@link #removeOnAttachStateChangeListener(OnAttachStateChangeListener)}.
 *
 * @param listener Listener to attach
 * @see #removeOnAttachStateChangeListener(OnAttachStateChangeListener)
 */
ECode View::AddOnAttachStateChangeListener(
    /* [in] */ IViewOnAttachStateChangeListener* listener)
{
    GetListenerInfo()->mOnAttachStateChangeListeners.PushBack(listener);
    return NOERROR;
}

/**
 * Remove a listener for attach state changes. The listener will receive no further
 * notification of window attach/detach events.
 *
 * @param listener Listener to remove
 * @see #addOnAttachStateChangeListener(OnAttachStateChangeListener)
 */
ECode View::RemoveOnAttachStateChangeListener(
    /* [in] */ IViewOnAttachStateChangeListener* listener)
{
    AutoPtr<ListenerInfo> li = mListenerInfo;
    if (li == NULL || li->mOnAttachStateChangeListeners.IsEmpty()) {
        return NOERROR;
    }

    li->mOnAttachStateChangeListeners.Remove(listener);
    return NOERROR;
}

/**
 * Returns the focus-change callback registered for this view.
 *
 * @return The callback, or NULL if one is not registered.
 */
ECode View::GetOnFocusChangeListener(
    /* [out] */ IViewOnFocusChangeListener** l)
{
    VALIDATE_NOT_NULL(l);
    *l = NULL;

    AutoPtr<ListenerInfo> li = mListenerInfo;
    if (li != NULL) {
        *l = li->mOnFocusChangeListener;
        REFCOUNT_ADD(*l);
    }

    return NOERROR;
}

/**
 * Register a callback to be invoked when this view is clicked. If this view is not
 * clickable, it becomes clickable.
 *
 * @param l The callback that will run
 *
 * @see #setClickable(Boolean)
 */
ECode View::SetOnClickListener(
    /* [in] */ IViewOnClickListener* l)
{
    if (!IsClickable()) {
        SetClickable(TRUE);
    }
    GetListenerInfo()->mOnClickListener = l;

    return NOERROR;
}

/**
 * Return whether this view has an attached OnClickListener.  Returns
 * TRUE if there is a listener, false if there is none.
 */
Boolean View::HasOnClickListeners()
{
    AutoPtr<ListenerInfo> li = mListenerInfo;
    return (li != NULL && li->mOnClickListener != NULL);
}

/**
 * Register a callback to be invoked when this view is clicked and held. If this view is not
 * long clickable, it becomes long clickable.
 *
 * @param l The callback that will run
 *
 * @see #setLongClickable(Boolean)
 */
ECode View::SetOnLongClickListener(
    /* [in] */ IViewOnLongClickListener* l)
{
    if (!IsLongClickable()) {
        SetLongClickable(TRUE);
    }
    GetListenerInfo()->mOnLongClickListener = l;

    return NOERROR;
}

/**
 * Register a callback to be invoked when the context menu for this view is
 * being built. If this view is not long clickable, it becomes long clickable.
 *
 * @param l The callback that will run
 *
 */
ECode View::SetOnCreateContextMenuListener(
    /* [in] */ IViewOnCreateContextMenuListener* l)
{
    if (!IsLongClickable()) {
        SetLongClickable(TRUE);
    }
    GetListenerInfo()->mOnCreateContextMenuListener = l;

    return NOERROR;
}

/**
 * Call this view's OnClickListener, if it is defined.
 *
 * @return True there was an assigned OnClickListener that was called, FALSE
 *         otherwise is returned.
 */
Boolean View::PerformClick()
{

    Boolean result;

    AutoPtr<ListenerInfo> li = mListenerInfo;
    if (li != NULL && li->mOnClickListener != NULL) {
        PlaySoundEffect(SoundEffectConstants::CLICK);
        li->mOnClickListener->OnClick(IVIEW_PROBE(this));
        result = TRUE;
    } else {
        result = FALSE;
    }

    SendAccessibilityEvent(IAccessibilityEvent::TYPE_VIEW_CLICKED);
    return result;
}

Boolean View::CallOnClick()
{
    AutoPtr<ListenerInfo> li = mListenerInfo;
    if (li != NULL && li->mOnClickListener != NULL) {
        li->mOnClickListener->OnClick(IVIEW_PROBE(this));
        return TRUE;
    }
    return FALSE;
}

/**
 * Call this view's OnLongClickListener, if it is defined. Invokes the context menu if the
 * OnLongClickListener did not consume the event.
 *
 * @return True if one of the above receivers consumed the event, FALSE otherwise.
 */
Boolean View::PerformLongClick()
{
    SendAccessibilityEvent(IAccessibilityEvent::TYPE_VIEW_LONG_CLICKED);

    Boolean handled = FALSE;
    AutoPtr<ListenerInfo> li = mListenerInfo;
    if (li != NULL && li->mOnLongClickListener != NULL) {
        li->mOnLongClickListener->OnLongClick(IVIEW_PROBE(this), &handled);
    }

    if (!handled) {
        handled = ShowContextMenu();
    }

    if (handled) {
        PerformHapticFeedback(IHapticFeedbackConstants::LONG_PRESS);
    }

    return handled;
}

/**
 * Performs button-related actions during a touch down event.
 *
 * @param event The event.
 * @return True if the down was consumed.
 *
 * @hide
 */
Boolean View::PerformButtonActionOnTouchDown(
    /* [in] */ IMotionEvent* event)
{
    assert(event != NULL);
    Int32 state = 0;
    event->GetButtonState(&state);
    if ((state & IMotionEvent::BUTTON_SECONDARY) != 0) {
        Float x = 0, y = 0;
        event->GetX(&x);
        event->GetY(&y);
        event->GetMetaState(&state);
        if (ShowContextMenu(x, y, state)) {
            return TRUE;
        }
    }
    return FALSE;
}

/**
 * Bring up the context menu for this view.
 *
 * @return Whether a context menu was displayed.
 */
Boolean View::ShowContextMenu()
{
    Boolean result;
    GetParent()->ShowContextMenuForChild(
        IVIEW_PROBE(this), &result);

    return result;
}

Boolean View::ShowContextMenu(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 metaState)
{
    return ShowContextMenu();
}

AutoPtr<IActionMode> View::StartActionMode(
    /* [in] */ IActionModeCallback* callback)
{
    AutoPtr<IViewParent> parent = GetParent();
    if (parent == NULL)
        return NULL;
    AutoPtr<IActionMode> mode;
    parent->StartActionModeForChild(
        IVIEW_PROBE(this), callback, (IActionMode**)&mode);

    return mode;
}

/***
 * Register a callback to be invoked when a key is pressed in this view.
 * @param l the key listener to attach to this view
 */
ECode View::SetOnKeyListener(
    /* [in] */ IViewOnKeyListener* l)
{
    GetListenerInfo()->mOnKeyListener = l;

    return NOERROR;
}

/**
 * Register a callback to be invoked when a touch event is sent to this view.
 * @param l the touch listener to attach to this view
 */
ECode View::SetOnTouchListener(
    /* [in] */ IViewOnTouchListener* l)
{
    GetListenerInfo()->mOnTouchListener = l;

    return NOERROR;
}

ECode View::SetOnGenericMotionListener(
    /* [in] */ IViewOnGenericMotionListener* l)
{
    GetListenerInfo()->mOnGenericMotionListener = l;
    return NOERROR;
}

ECode View::SetOnHoverListener(
    /* [in] */ IViewOnHoverListener* l)
{
    GetListenerInfo()->mOnHoverListener = l;
    return NOERROR;
}

ECode View::SetOnDragListener(
    /* [in] */ IViewOnDragListener* l)
{
    GetListenerInfo()->mOnDragListener = l;
    return NOERROR;
}

/**
 * Give this view focus. This will cause {@link #onFocusChanged} to be called.
 *
 * Note: this does not check whether this {@link View} should get focus, it just
 * gives it focus no matter what.  It should only be called internally by framework
 * code that knows what it is doing, namely {@link #requestFocus(int, Rect)}.
 *
 * @param direction values are View.FOCUS_UP, View.FOCUS_DOWN,
 *        View.FOCUS_LEFT or View.FOCUS_RIGHT. This is the direction which
 *        focus moved when requestFocus() is called. It may not always
 *        apply, in which case use the default View.FOCUS_DOWN.
 * @param previouslyFocusedRect The rectangle of the view that had focus
 *        prior in this View's coordinate system.
 */
ECode View::HandleFocusGainInternal(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    if (DBG) {
       Logger::D("View", " requestFocus()");
    }

    if ((mPrivateFlags & PFLAG_FOCUSED) == 0) {
        mPrivateFlags |= PFLAG_FOCUSED;

        AutoPtr<IView> oldFocus;
        if (mAttachInfo) {
            GetRootView()->FindFocus((IView**)&oldFocus);
        }

        if (mParent != NULL) {
            mParent->RequestChildFocus(
                IVIEW_PROBE(this),
                IVIEW_PROBE(this));
        }

        if (mAttachInfo) {
                mAttachInfo->mTreeObserver->DispatchOnGlobalFocusChange(oldFocus, THIS_PROBE(IView));
        }

        OnFocusChanged(TRUE, direction, previouslyFocusedRect);
        RefreshDrawableState();
    }

    return NOERROR;
}

/**
 * Populates <code>outRect</code> with the hotspot bounds. By default,
 * the hotspot bounds are identical to the screen bounds.
 *
 * @param outRect rect to populate with hotspot bounds
 * @hide Only for internal use by views and widgets.
 */
void View::GetHotspotBounds(
    /* [in] */ IRect* outRect)
{
    AutoPtr<IDrawable> background = GetBackground();
    if (background) {
        background->GetHotspotBounds(outRect);
    } else {
        GetBoundsOnScreen(outRect);
    }
}

/**
 * Request that a rectangle of this view be visible on the screen,
 * scrolling if necessary just enough.
 *
 * <p>A View should call this if it maintains some notion of which part
 * of its content is interesting.  For example, a text editing view
 * should call this when its cursor moves.
 *
 * @param rectangle The rectangle.
 * @return Whether any parent scrolled.
 */
Boolean View::RequestRectangleOnScreen(
    /* [in] */ IRect* rectangle)
{
    return RequestRectangleOnScreen(rectangle, FALSE);
}

/**
 * Request that a rectangle of this view be visible on the screen,
 * scrolling if necessary just enough.
 *
 * <p>A View should call this if it maintains some notion of which part
 * of its content is interesting.  For example, a text editing view
 * should call this when its cursor moves.
 *
 * <p>When <code>immediate</code> is set to TRUE, scrolling will not be
 * animated.
 *
 * @param rectangle The rectangle.
 * @param immediate True to forbid animated scrolling, FALSE otherwise
 * @return Whether any parent scrolled.
 */
Boolean View::RequestRectangleOnScreen(
    /* [in] */ IRect* rectangle,
    /* [in] */ Boolean immediate)
{
    // View* child = this;
    // AutoPtr<IViewParent> parent = mParent;
    // Boolean scrolled = FALSE;
    // while (parent != NULL) {
    //     Boolean result;

    //     parent->RequestChildRectangleOnScreen(
    //             IVIEW_PROBE(child),
    //             rectangle, immediate, &result);

    //     scrolled |= result;

    //     // offset rect so next call has the rectangle in the
    //     // coordinate system of its direct child.
    //     rectangle->Offset(child->GetLeft(), child->GetTop());
    //     rectangle->Offset(-child->GetScrollX(), -child->GetScrollY());

    //     if (parent->Probe(EIID_View) == NULL) {
    //         break;
    //     }

    //     child = VIEW_PROBE(parent);
    //     parent = child->GetParent();
    // }

    if (mParent == NULL) {
        return FALSE;
    }

    View* child = this;
    AutoPtr<IRectF> position;
    if (mAttachInfo != NULL) {
        position = mAttachInfo->mTmpTransformRect;
    }
    else {
        CRectF::New((IRectF**)&position);
    }

    position->Set(rectangle);
    AutoPtr<IViewParent> parent = mParent;
    Boolean scrolled = FALSE;
    while (parent != NULL) {
        Float l, r, t, b;
        position->GetLeft(&l);
        position->GetRight(&r);
        position->GetTop(&t);
        position->GetBottom(&b);
        rectangle->Set((Int32)l, (Int32)t, (Int32)r, (Int32)b);

        Boolean result = FALSE;
        parent->RequestChildRectangleOnScreen(
            IVIEW_PROBE(child), rectangle, immediate, &result);

        scrolled |= result;

        if (!child->HasIdentityMatrix()) {
            Boolean res;
            child->GetMatrix()->MapRect(position.Get(), &res);
        }

        position->Offset(child->mLeft, child->mTop);

        if (!IView::Probe(parent)) {
            break;
        }

        View* parentView = VIEW_PROBE(parent);

        position->Offset(-parentView->GetScrollX(), -parentView->GetScrollY());

        child = parentView;
        parent = child->GetParent();
    }

    return scrolled;
}

/**
 * Called when this view wants to give up focus. If focus is cleared
 * {@link #onFocusChanged(boolean, int, android.graphics.Rect)} is called.
 * <p>
 * <strong>Note:</strong> When a View clears focus the framework is trying
 * to give focus to the first focusable View from the top. Hence, if this
 * View is the first from the top that can take focus, then all callbacks
 * related to clearing focus will be invoked after wich the framework will
 * give focus to this view.
 * </p>
 */
ECode View::ClearFocus()
{
//    if (DBG) {
//        System.out.println(this + " clearFocus()");
//    }

    ClearFocusInternal(NULL, TRUE, TRUE);
    return NOERROR;
}

/**
 * Clears focus from the view, optionally propagating the change up through
 * the parent hierarchy and requesting that the root view place new focus.
 *
 * @param propagate whether to propagate the change up through the parent
 *            hierarchy
 * @param refocus when propagate is true, specifies whether to request the
 *            root view place new focus
 */
ECode View::ClearFocusInternal(
    /* [in] */ IView* focused,
    /* [in] */ Boolean propagate,
    /* [in] */ Boolean refocus)
{
    if ((mPrivateFlags & PFLAG_FOCUSED) != 0) {
        mPrivateFlags &= ~PFLAG_FOCUSED;

        if (propagate && mParent) {
            mParent->ClearChildFocus(THIS_PROBE(IView));
        }

        OnFocusChanged(FALSE, 0, NULL);
        RefreshDrawableState();

        if (propagate && (!refocus || !RootViewRequestFocus())) {
            NotifyGlobalFocusCleared(THIS_PROBE(IView));
        }
    }
}

/**
 * Called to clear the focus of a view that is about to be removed.
 * Doesn't call clearChildFocus, which prevents this view from taking
 * focus again before it has been removed from the parent
 */
ECode View::NotifyGlobalFocusCleared(
    /* [in] */ IView* oldFocus)
{
    if (oldFocus && mAttachInfo) {
            mAttachInfo->mTreeObserver->DispatchOnGlobalFocusChange(oldFocus, NULL);
        }

    return NOERROR;
}

Boolean View::RootViewRequestFocus()
{
    AutoPtr<IView> root = GetRootView();
    Boolean res;
    root->RequestFocus(&res);
    return root != NULL && res;
}

/**
 * Called internally by the view system when a new view is getting focus.
 * This is what clears the old focus.
 */
ECode View::UnFocus(
     /* [in] */ IView* focused)
{
    //if (DBG) {
    //    System.out.println(this + " unFocus()");
    //}

    ClearFocusInternal(focused, FALSE, FALSE);

    return NOERROR;
}

/**
 * Returns TRUE if this view has focus iteself, or is the ancestor of the
 * view that has focus.
 *
 * @return True if this view has or contains focus, FALSE otherwise.
 */
Boolean View::HasFocus()
{
    return (mPrivateFlags & PFLAG_FOCUSED) != 0;
}

/**
 * Returns TRUE if this view is focusable or if it contains a reachable View
 * for which {@link #hasFocusable()} returns TRUE. A "reachable hasFocusable()"
 * is a View whose parents do not block descendants focus.
 *
 * Only {@link #VISIBLE} views are considered focusable.
 *
 * @return True if the view is focusable or if the view contains a focusable
 *         View, FALSE otherwise.
 *
 * @see ViewGroup#FOCUS_BLOCK_DESCENDANTS
 */
Boolean View::HasFocusable()
{
    if (!IsFocusableInTouchMode()) {
        while (IViewGroup::Probe(mParent)) {
            AutoPtr<IViewGroup> g = (IViewGroup*)IViewGroup::Probe(mParent);
            Boolean res;
            if (g->ShouldBlockFocusForTouchscreen(&res), res) {
                return FALSE;
            }
            mParent->GetParent((IViewParent**)&mParent);
        }
    }
    return (mViewFlags & VISIBILITY_MASK) == IView::VISIBLE && IsFocusable();
}

/**
 * Called by the view system when the focus state of this view changes.
 * When the focus change event is caused by directional navigation, direction
 * and previouslyFocusedRect provide insight into where the focus is coming from.
 * When overriding, be sure to call up through to the super class so that
 * the standard focus handling will occur.
 *
 * @param gainFocus True if the View has focus; false otherwise.
 * @param direction The direction focus has moved when requestFocus()
 *                  is called to give this view focus. Values are
 *                  {@link #FOCUS_UP}, {@link #FOCUS_DOWN}, {@link #FOCUS_LEFT},
 *                  {@link #FOCUS_RIGHT}, {@link #FOCUS_FORWARD}, or {@link #FOCUS_BACKWARD}.
 *                  It may not always apply, in which case use the default.
 * @param previouslyFocusedRect The rectangle, in this view's coordinate
 *        system, of the previously focused view.  If applicable, this will be
 *        passed in as finer grained information about where the focus is coming
 *        from (in addition to direction).  Will be <code>NULL</code> otherwise.
 */
void View::OnFocusChanged(
    /* [in] */ Boolean gainFocus,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    if (gainFocus) {
        SendAccessibilityEvent(IAccessibilityEvent::TYPE_VIEW_FOCUSED);
    } else {
        NotifyViewAccessibilityStateChangedIfNeeded(
                    IAccessibilityEvent::CONTENT_CHANGE_TYPE_UNDEFINED);
    }

    AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
    if (!gainFocus) {
        if (IsPressed()) {
            SetPressed(FALSE);
        }

        if (imm.Get() != NULL && mAttachInfo.Get() != NULL
               && mAttachInfo->mHasWindowFocus) {
            imm->FocusOut(IVIEW_PROBE(this));
        }

        OnFocusLost();
    }
    else if (imm != NULL && mAttachInfo != NULL
           && mAttachInfo->mHasWindowFocus) {
       imm->FocusIn(IVIEW_PROBE(this));
    }

    Invalidate(TRUE);
    AutoPtr<ListenerInfo> li = mListenerInfo;
    if (li != NULL && li->mOnFocusChangeListener != NULL) {
        li->mOnFocusChangeListener->OnFocusChange(
            IVIEW_PROBE(this), gainFocus);
    }

    if (mAttachInfo != NULL) {
        mAttachInfo->mKeyDispatchState->Reset(this->Probe(EIID_IInterface));
    }
}

ECode View::SendAccessibilityEvent(
    /* [in] */ Int32 eventType)
{
    if (mAccessibilityDelegate != NULL) {
        mAccessibilityDelegate->SendAccessibilityEvent(
            IVIEW_PROBE(this), eventType);
    }
    else {
        SendAccessibilityEventInternal(eventType);
    }

    return NOERROR;
}

ECode View::AnnounceForAccessibility(
    /* [in] */ ICharSequence* text)
{
        AutoPtr<IAccessibilityManager> manger;
        CAccessibilityManager::GetInstance(mContext, (IAccessibilityManager**)&manger);
        Boolean bval;
        manger->IsEnabled(&bval);
        if (bval && mParent != NULL) {
            AutoPtr<IAccessibilityEvent> event;
            CAccessibilityEvent::Obtain(IAccessibilityEvent::TYPE_ANNOUNCEMENT,
                (IAccessibilityEvent**)&event);
            OnInitializeAccessibilityEvent(event);
            AutoPtr<IObjectContainer> bc;
            event->GetText((IObjectContainer**)&bc);
            bc->Add(text);
            event->SetContentDescription(NULL);
            Boolean result;
            mParent->RequestSendAccessibilityEvent(IVIEW_PROBE(this), event, &result);
        }

    return NOERROR;
}

/**
 * @see #sendAccessibilityEvent(int)
 *
 * Note: Called from the default {@link AccessibilityDelegate}.
 */
void View::SendAccessibilityEventInternal(
    /* [in] */ Int32 eventType)
{
    AutoPtr<IAccessibilityManager> manger;
    CAccessibilityManager::GetInstance(mContext, (IAccessibilityManager**)&manger);
    Boolean bval;
    if (manger->IsEnabled(&bval),  bval) {
        AutoPtr<IAccessibilityEvent> event;
        CAccessibilityEvent::Obtain(eventType, (IAccessibilityEvent**)&event);
        SendAccessibilityEventUnchecked(event);
    }
}

ECode View::SendAccessibilityEventUnchecked(
    /* [in] */ IAccessibilityEvent* event)
{
    if (mAccessibilityDelegate != NULL) {
        mAccessibilityDelegate->SendAccessibilityEventUnchecked(
                IVIEW_PROBE(this), event);
    }
    else {
        SendAccessibilityEventUncheckedInternal(event);
    }

    return NOERROR;
}

void View::SendAccessibilityEventUncheckedInternal(
    /* [in] */ IAccessibilityEvent* event)
{
    if (!IsShown()) {
        return;
    }

    OnInitializeAccessibilityEvent(event);
    Int32 type = 0;
    //event->GetEventType(&type);
    // Only a subset of accessibility events populates text content.
    if ((type & POPULATING_ACCESSIBILITY_EVENT_TYPES) != 0) {
        DispatchPopulateAccessibilityEvent(event);
    }
    // In the beginning we called #isShown(), so we know that getParent() is not NULL.
    Boolean res;
    GetParent()->RequestSendAccessibilityEvent(IVIEW_PROBE(this), event, &res);
}

Boolean View::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    if (mAccessibilityDelegate != NULL) {
        Boolean res;
        mAccessibilityDelegate->DispatchPopulateAccessibilityEvent(
            IVIEW_PROBE(this), event, &res);
        return NOERROR;
    }
    else {
        return DispatchPopulateAccessibilityEventInternal(event);
    }
}


Boolean View::DispatchPopulateAccessibilityEventInternal(
    /* [in] */ IAccessibilityEvent* event)
{
    OnPopulateAccessibilityEvent(event);
    return FALSE;
}

ECode View::OnPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    if (mAccessibilityDelegate != NULL) {
        mAccessibilityDelegate->OnPopulateAccessibilityEvent(
            IVIEW_PROBE(this), event);
    }
    else {
        OnPopulateAccessibilityEventInternal(event);
    }

    return NOERROR;
}

void View::OnPopulateAccessibilityEventInternal(
    /* [in] */ IAccessibilityEvent* event)
{
}

ECode View::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    if (mAccessibilityDelegate != NULL) {
        mAccessibilityDelegate->OnInitializeAccessibilityEvent(
            IVIEW_PROBE(this), event);
    }
    else {
        OnInitializeAccessibilityEventInternal(event);
    }

    return NOERROR;
}

void View::OnInitializeAccessibilityEventInternal(
    /* [in] */ IAccessibilityEvent* event)
{
    event->SetSource(IVIEW_PROBE(this));
    AutoPtr<ICharSequence> seq;
    CString::New(String("View"), (ICharSequence**)&seq);
    event->SetClassName(seq);
    AutoPtr<IContext> context = GetContext();
    String pkgName;
    context->GetPackageName(&pkgName);
    seq = NULL;
    CString::New(pkgName, (ICharSequence**)&seq);
    event->SetPackageName(seq);
    event->SetEnabled(IsEnabled());
    event->SetContentDescription(mContentDescription);

    Int32 type;
    switch (event->GetEventType(&type), type) {
        case IAccessibilityEvent::TYPE_VIEW_FOCUSED:
        {
            AutoPtr<IArrayList> focusablesTempList;
            if (mAttachInfo) {
                focusablesTempList = mAttachInfo->mTempArrayList;
            } else {
                CArrayList::New((IArrayList**)&focusablesTempList);
            }

            GetRootView()->AddFocusables(focusablesTempList, IView::FOCUS_FORWARD, IView::FOCUSABLES_ALL);
            (IAccessibilityRecord::Probe(event))->SetItemCount(focusablesTempList.GetSize());
            Int32 index;
            temp->IndexOf(THIS_PROBE(IInterface), &index);
            (IAccessibilityRecord::Probe(event))->SetCurrentItemIndex(index);

            if (mAttachInfo) {
                focusablesTempList->Clear();
            }

        } break;
        case IAccessibilityEvent::TYPE_VIEW_TEXT_SELECTION_CHANGED:
        {
            AutoPtr<ICharSequence> text = GetIterableTextForAccessibility();
            if (text) {
                Int32 len;
                text->GetLength(&len);
                if (len > 0) {
                    (IAccessibilityRecord::Probe(event))->SetFromIndex(GetAccessibilitySelectionStart());
                    (IAccessibilityRecord::Probe(event))->SetToIndex(GetAccessibilitySelectionEnd());
                    (IAccessibilityRecord::Probe(event))->SetItemCount(len);
                }
            }
        } break;
    }
}

AutoPtr<IAccessibilityNodeInfo> View::CreateAccessibilityNodeInfo()
{
    if (mAccessibilityDelegate) {
        return mAccessibilityDelegate->CreateAccessibilityNodeInfo(THIS_PROBE(IAccessibilityNodeInfo));
    } else {
        return CreateAccessibilityNodeInfoInternal();
    }
}

AutoPtr<IAccessibilityNodeInfo> View::CreateAccessibilityNodeInfoInternal()
{
    AutoPtr<IAccessibilityNodeInfo> info;
    AutoPtr<IAccessibilityNodeProvider> provider = GetAccessibilityNodeProvider();
    if (provider != NULL) {
        provider->CreateAccessibilityNodeInfo(IView::NO_ID, (IAccessibilityNodeInfo**)&info);
    }
    else {
        CAccessibilityNodeInfo::Obtain(IVIEW_PROBE(this), (IAccessibilityNodeInfo**)&info);
        OnInitializeAccessibilityNodeInfo(info);
    }
    return info;
}

ECode View::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    if (mAccessibilityDelegate != NULL) {
        mAccessibilityDelegate->OnInitializeAccessibilityNodeInfo(
            IVIEW_PROBE(this), info);
    }
    else {
        OnInitializeAccessibilityNodeInfoInternal(info);
    }

    return NOERROR;
}

void View::GetBoundsOnScreen(
    /* [in] */ IRect* outRect)
{
    if (mAttachInfo == NULL) {
        return;
    }

    AutoPtr<IRectF> position = mAttachInfo->mTmpTransformRect;
    position->Set(0, 0, mRight - mLeft, mBottom - mTop);

    if (!HasIdentityMatrix()) {
        Boolean res;
        GetMatrix()->MapRect(position, &res);
    }

    position->Offset(mLeft, mTop);

    AutoPtr<IViewParent> parent = mParent;
    while (IView::Probe(parent) != NULL) {
        View* parentView = VIEW_PROBE(parent);

        position->Offset(-parentView->mScrollX, -parentView->mScrollY);

        if (!parentView->HasIdentityMatrix()) {
            Boolean res;
            parentView->GetMatrix()->MapRect(position, &res);
        }

        position->Offset(parentView->mLeft, parentView->mTop);

        parent = parentView->mParent;
    }

    if (parent) {
        ViewRootImpl* viewRootImpl = reinterpret_cast<ViewRootImpl*>(parent->Probe(EIID_ViewRootImpl));
        position->Offset(0, -viewRootImpl->mCurScrollY);
    }

    position->Offset(mAttachInfo->mWindowLeft, mAttachInfo->mWindowTop);

    Float l, r, t, b;
    position->GetLeft(&l);
    position->GetRight(&r);
    position->GetTop(&t);
    position->GetBottom(&b);
    outRect->Set((Int32)(l + 0.5f), (Int32)(t + 0.5f), (Int32)(r + 0.5f), (Int32)(b + 0.5f));
}

void View::OnInitializeAccessibilityNodeInfoInternal(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    AutoPtr<IRect> bounds = mAttachInfo->mTmpInvalRect;

    GetDrawingRect(bounds);
    info->SetBoundsInParent(bounds);

    GetBoundsOnScreen(bounds);
    info->SetBoundsInScreen(bounds);

    AutoPtr<IViewParent> parent = GetParentForAccessibility();
    if (IView::Probe(parent) != NULL) {
        info->SetParent(IView::Probe(parent));
    }

    if (mID != IView::NO_ID) {
        AutoPtr<IView> rootView = GetRootView();
        if (rootView == NULL) {
            rootView = IVIEW_PROBE(this);
        }

        AutoPtr<IView> label = VIEW_PROBE(rootView)->FindLabelForView(IVIEW_PROBE(this), mID);
        if (label != NULL) {
            info->SetLabeledBy(label);
        }

        if ((mAttachInfo->mAccessibilityFetchFlags & IAccessibilityNodeInfo::FLAG_REPORT_VIEW_IDS) != 0
               /* && Resources::ResourceHasPackage(mID)*/) {
        //    try {
            String viewId;
            GetResources()->GetResourceName(mID, &viewId);
            //info->SetViewIdResourceName(viewId);
        //    } catch (Resources.NotFoundException nfe) {
                /* ignore */
        //    }
        }
    }

    if (mLabelForId != IView::NO_ID) {
        AutoPtr<IView> rootView = GetRootView();
        if (rootView == NULL) {
            rootView = IVIEW_PROBE(this);
        }
        AutoPtr<IView> labeled = VIEW_PROBE(rootView)->FindViewInsideOutShouldExist(IVIEW_PROBE(this), mLabelForId);
        if (labeled != NULL) {
            info->SetLabelFor(labeled);
        }
    }

    info->SetVisibleToUser(IsVisibleToUser());

    AutoPtr<ICharSequence> seq;
    CString::New(String("View"), (ICharSequence**)&seq);
    info->SetClassName(seq);

    String pkgName;
    mContext->GetPackageName(&pkgName);
    seq = NULL;
    CString::New(pkgName, (ICharSequence**)&seq);
    info->SetPackageName(seq);
    info->SetContentDescription(GetContentDescription());

    info->SetEnabled(IsEnabled());
    info->SetClickable(IsClickable());
    info->SetFocusable(IsFocusable());
    info->SetFocused(IsFocused());
    info->SetAccessibilityFocused(IsAccessibilityFocused());
    info->SetSelected(IsSelected());
    info->SetLongClickable(IsLongClickable());
    info->SetLiveRegion(GetAccessibilityLiveRegion());

    // TODO: These make sense only if we are in an AdapterView but all
    // views can be selected. Maybe from accessibility perspective
    // we should report as selectable view in an AdapterView.
    info->AddAction(IAccessibilityNodeInfo::ACTION_SELECT);
    info->AddAction(IAccessibilityNodeInfo::ACTION_CLEAR_SELECTION);

    if (IsFocusable()) {
        if (IsFocused()) {
            info->AddAction(IAccessibilityNodeInfo::ACTION_CLEAR_FOCUS);
        }
        else {
            info->AddAction(IAccessibilityNodeInfo::ACTION_FOCUS);
        }
    }

    if (!IsAccessibilityFocused()) {
        info->AddAction(IAccessibilityNodeInfo::ACTION_ACCESSIBILITY_FOCUS);
    }
    else {
        info->AddAction(IAccessibilityNodeInfo::ACTION_CLEAR_ACCESSIBILITY_FOCUS);
    }

    if (IsClickable() && IsEnabled()) {
        info->AddAction(IAccessibilityNodeInfo::ACTION_CLICK);
    }

    if (IsLongClickable() && IsEnabled()) {
        info->AddAction(IAccessibilityNodeInfo::ACTION_LONG_CLICK);
    }

    AutoPtr<ICharSequence> text = GetIterableTextForAccessibility();
    if (text) {
        Int32 length;
        text->GetLength(&length);
        if (length > 0) {
            info->SetTextSelection(GetAccessibilitySelectionStart(), GetAccessibilitySelectionEnd());
            info->AddAction(IAccessibilityNodeInfo::ACTION_SET_SELECTION);
            info->AddAction(IAccessibilityNodeInfo::ACTION_NEXT_AT_MOVEMENT_GRANULARITY);
            info->AddAction(IAccessibilityNodeInfo::ACTION_PREVIOUS_AT_MOVEMENT_GRANULARITY);
            info->SetMovementGranularities(
                IAccessibilityNodeInfo::MOVEMENT_GRANULARITY_CHARACTER
                | IAccessibilityNodeInfo::MOVEMENT_GRANULARITY_WORD
                | IAccessibilityNodeInfo::MOVEMENT_GRANULARITY_PARAGRAPH);
        }
    }
}

AutoPtr<IView> View::FindLabelForView(
    /* [in] */ IView* view,
    /* [in] */ Int32 labeledId)
{
    if (mMatchLabelForPredicate == NULL) {
        mMatchLabelForPredicate = new MatchLabelForPredicate();
    }
    mMatchLabelForPredicate->mLabeledId = labeledId;
    return FindViewByPredicateInsideOut(view, mMatchLabelForPredicate);
}

Boolean View::IsVisibleToUser()
{
    return IsVisibleToUser(NULL);
}

Boolean View::IsVisibleToUser(
    /* [in] */ IRect* boundInView)
{
    if (mAttachInfo != NULL) {
        // Attached to invisible window means this view is not visible.
        if (mAttachInfo->mWindowVisibility != IView::VISIBLE) {
            return FALSE;
        }
        // An invisible predecessor or one with alpha zero means
        // that this view is not visible to the user.
        AutoPtr<IInterface> current = IVIEW_PROBE(this);
        while (IView::Probe(current) != NULL) {
            View* view = VIEW_PROBE(current);
            // We have attach info so this view is attached and there is no
            // need to check whether we reach to ViewRootImpl on the way up.
            if (view->GetAlpha() <= 0 || view->GetTransitionAlpha() <= 0 || view->GetVisibility() != IView::VISIBLE) {
                return FALSE;
            }

            current = view->mParent;
        }

        // Check if the view is entirely covered by its predecessors.
        AutoPtr<IRect> visibleRect = mAttachInfo->mTmpInvalRect;
        AutoPtr<IPoint> offset = mAttachInfo->mPoint;
        if (!GetGlobalVisibleRect(visibleRect, offset)) {
            return FALSE;
        }
        // Check if the visible portion intersects the rectangle of interest.
        if (boundInView != NULL) {
            Int32 x, y;
            offset->GetX(&x);
            offset->GetY(&y);
            visibleRect->Offset(-x, -y);
            Boolean res;
            boundInView->Intersect(visibleRect, &res);
            return res;
        }
        return TRUE;
    }
    return FALSE;
}


/**
 * Computes a point on which a sequence of a down/up event can be sent to
 * trigger clicking this view. This method is for the exclusive use by the
 * accessibility layer to determine where to send a click event in explore
 * by touch mode.
 *
 * @param interactiveRegion The interactive portion of this window.
 * @param outPoint The point to populate.
 * @return True of such a point exists.
 */
Boolean View::ComputeClickPointInScreenForAccessibility(
    /* [in] */ IRegion* interactiveRegion,
    /* [in] */ IPoint* outPoint)
{
    // Since the interactive portion of the view is a region but as a view
    // may have a transformation matrix which cannot be applied to a
    // region we compute the view bounds rectangle and all interactive
    // predecessor's and sibling's (siblings of predecessors included)
    // rectangles that intersect the view bounds. At the
    // end if the view was partially covered by another interactive
    // view we compute the view's interactive region and pick a point
    // on its boundary path as regions do not offer APIs to get inner
    // points. Note that the the code is optimized to fail early and
    // avoid unnecessary allocations plus computations.

    // The current approach has edge cases that may produce false
    // positives or false negatives. For example, a portion of the
    // view may be covered by an interactive descendant of a
    // predecessor, which we do not compute. Also a view may be handling
    // raw touch events instead registering click listeners, which
    // we cannot compute. Despite these limitations this approach will
    // work most of the time and it is a huge improvement over just
    // blindly sending the down and up events in the center of the
    // view.

    // Cannot click on an unattached view.
    if (mAttachInfo == NULL) {
        return FALSE;
    }

    // Attached to an invisible window means this view is not visible.
    if (mAttachInfo->mWindowVisibility != IView::VISIBLE) {
        return FALSE;
    }

    AutoPtr<IRectF> bounds = mAttachInfo->mTmpTransformRect;
    bounds->Set(0, 0, GetWidth(), GetHeight());
    AutoPtr<IArrayList> intersections = mAttachInfo->mTmpRectList;
    intersections->Clear();

    if (IViewGroup::Probe(mParent)) {
        AutoPtr<IViewGroup> parentGroup = (IViewGroup*)IViewGroup::Probe(mParent);

        Boolean res;
        parentGroup->TranslateBoundsAndIntersectionsInWindowCoordinates(THIS_PROBE(IView), bounds, intersections, &res);
        if (!res) {
            intersections->Clear();
            return FALSE;
        }
    }

    // Take into account the window location.
    Int32 dx = mAttachInfo->mWindowLeft;
    Int32 dy = mAttachInfo->mWindowTop;
    bounds->Offset(dx, dy);
    OffsetRects(intersections, dx, dy);

    Boolean isEmpty;
    intersections->IsEmpty(&isEmpty);
    if (isEmpty && interactiveRegion) {
        Float x, y;
        bounds->GetCenterX(&x);
        bounds->GetCenterY(&y);
        outPoint->Set(x, y);
    } else {
        // This view is partially covered by other views, then compute
        // the not covered region and pick a point on its boundary.
        AutoPtr<IRegion> region;
        CRegion::New((IRegion**)&region);
        Float left, top, right, bottom;
        bounds->GetLeft(&left);
        bounds->GetTop(&top);
        bounds->GetRight(&right);
        bounds->GetBottom(&bottom);
        Boolean res;
        region->Set(left, top, right, bottom, &res);

        Int32 intersectionCount =  intersections->GetSize();

        for (Int32 i = intersectionCount - 1; i >= 0; i--) {
            AutoPtr<IInterface> temp;
            intersections->Remove(i, (IInterface**)&temp);
            AutoPtr<IRectF> intersection = IRectF::Probe(temp);

            Float rleft, rtop, rright, rbottom;
            intersection->GetLeft(&rleft);
            intersection->GetTop(&rtop);
            intersection->GetRight(&rright);
            intersection->GetBottom(&rbottom);
            region->Op(rleft, rtop, rright, rbottom, RegionOp_DIFFERENCE, &res);
        }

        // If the view is completely covered, done.
        if (region->IsEmpty(&res), res) {
            return FALSE;
        }

        // Take into account the interactive portion of the window
        // as the rest is covered by other windows. If no such a region
        // then the whole window is interactive.
        if (interactiveRegion) {
            region->Op(interactiveRegion, RegionOp_INTERSECT, &res);
        }

        // If the view is completely covered, done.
        if (region->IsEmpty(&res), res) {
            return FALSE;
        }

        // Try a shortcut here.
        if (region->IsRect(&res), res) {
            AutoPtr<IRect> regionBounds = mAttachInfo->mTmpInvalRect;
            region->GetBounds((IRect**)&regionBounds);
            Int32 x, y;
            regionBounds->GetCenterX(&x);
            regionBounds->GetCenterY(&y);
            outPoint->Set(x, y);
            return TRUE;
        }

        // Get the a point on the region boundary path.
        AutoPtr<IPath> path;
        region->GetBoundaryPath((IPath**)&path);

        AutoPtr<IPathMeasure> pathMeasure;
        CPathMeasure::New(path, FALSE, (IPathMeasure**)&pathMeasure);

        AutoPtr< ArrayOf<Float> > coordinates = mAttachInfo->mTmpTransformLocation;

        // Without loss of generality pick a point.
        Float p;
        pathMeasure->GetLength(&p);
        p *= 0.01f;
        Boolean posTan;
        pathMeasure->GetPosTan(p, coordinates, NULL, &posTan);
        if (!posTan) {
            return FALSE;
        }

        outPoint->Set(Elastos::Core::Math::Round((*coordinates)[0]), Elastos::Core::Math::Round((*coordinates)[1]));
    }

    return TRUE;
}

void View::OffsetRects(
        /* [in] */ IArrayList* rects,
        /* [in] */ Float offsetX,
        /* [in] */ Float offsetY)
{
    Int32 rectCount;
    rects->GetSize(&rectCount);
    for (Int32 i = 0; i < rectCount; i++) {
        AutoPtr<IInterface> temp;
        rects->Get(i, (IInterface**)&temp);

        AutoPtr<IRectF> intersection = IRectF::Probe(temp);

        intersection->Offset(offsetX, offsetY);
    }
}



AutoPtr<IAccessibilityDelegate> View::GetAccessibilityDelegate()
{
    return mAccessibilityDelegate;
}

ECode View::SetAccessibilityDelegate(
    /* [in] */ IAccessibilityDelegate* delegate)
{
    mAccessibilityDelegate = delegate;
    return NOERROR;
}

AutoPtr<IAccessibilityNodeProvider> View::GetAccessibilityNodeProvider()
{
    if (mAccessibilityDelegate != NULL) {
        AutoPtr<IAccessibilityNodeProvider> anp;
        mAccessibilityDelegate->GetAccessibilityNodeProvider(
            IVIEW_PROBE(this), (IAccessibilityNodeProvider**)&anp);
        return anp;
    }
    else {
        return NULL;
    }
}

Int32 View::GetAccessibilityViewId()
{
    if (mAccessibilityViewId == IView::NO_ID) {
        mAccessibilityViewId = sNextAccessibilityViewId++;
    }
    return mAccessibilityViewId;
}

Int32 View::GetAccessibilityWindowId()
{
    return mAttachInfo != NULL ? mAttachInfo->mAccessibilityWindowId : IAccessibilityNodeInfo::UNDEFINED_ITEM_ID;;
}

AutoPtr<ICharSequence> View::GetContentDescription()
{
    return mContentDescription;
}

/**
 * Sets the {@link View} description. It briefly describes the view and is
 * primarily used for accessibility support. Set this property to enable
 * better accessibility support for your application. This is especially
 * TRUE for views that do not have textual representation (For example,
 * ImageButton).
 *
 * @param contentDescription The content description.
 *
 * @attr ref android.R.styleable#View_contentDescription
 */
ECode View::SetContentDescription(
    /* [in] */ ICharSequence* contentDescription)
{
    if (mContentDescription == NULL && contentDescription == NULL) {
        return NOERROR;
    }
    else if (mContentDescription.Get() == contentDescription) {
        return NOERROR;
    }

    if (mContentDescription != NULL && contentDescription != NULL) {
        String lhs, rhs;
        mContentDescription->ToString(&lhs);
        contentDescription->ToString(&rhs);
        if (lhs.Equals(rhs)) {
            return NOERROR;
        }
    }

    mContentDescription = contentDescription;

    Int32 len = 0;
    Boolean nonEmptyDesc = contentDescription != NULL && (contentDescription->GetLength(&len), len) > 0;
    if (nonEmptyDesc && GetImportantForAccessibility() == IView::IMPORTANT_FOR_ACCESSIBILITY_AUTO) {
        SetImportantForAccessibility(IView::IMPORTANT_FOR_ACCESSIBILITY_YES);
        NotifySubtreeAccessibilityStateChangedIfNeeded();
    } else {
        NotifyViewAccessibilityStateChangedIfNeeded(
                IAccessibilityEvent::CONTENT_CHANGE_TYPE_CONTENT_DESCRIPTION);

    return NOERROR;
}

Int32 View::GetLabelFor()
{
    return mLabelForId;
}

ECode View::SetLabelFor(
    /* [in] */ Int32 id)
{
    mLabelForId = id;
    if (mLabelForId != IView::NO_ID && mID == IView::NO_ID) {
        mID = GenerateViewId();
    }
    return NOERROR;
}

/**
 * Invoked whenever this view loses focus, either by losing window focus or by losing
 * focus within its window. This method can be used to clear any state tied to the
 * focus. For instance, if a button is held pressed with the trackball and the window
 * loses focus, this method can be used to cancel the press.
 *
 * Subclasses of View overriding this method should always call super.onFocusLost().
 *
 * @see #onFocusChanged(Boolean, int, android.graphics.Rect)
 * @see #onWindowFocusChanged(Boolean)
 *
 * @hide pending API council approval
 */
void View::OnFocusLost()
{
    ResetPressedState();
}

void View::ResetPressedState()
{
    if ((mViewFlags & ENABLED_MASK) == DISABLED) {
        return;
    }

    if (IsPressed()) {
        SetPressed(FALSE);

        if (!mHasPerformedLongPress) {
            RemoveLongPressCallback();
        }
    }
}

/**
 * Returns TRUE if this view has focus
 *
 * @return True if this view has focus, FALSE otherwise.
 */
Boolean View::IsFocused()
{
    return (mPrivateFlags & PFLAG_FOCUSED) != 0;
}

/**
 * Find the view in the hierarchy rooted at this view that currently has
 * focus.
 *
 * @return The view that currently has focus, or NULL if no focused view can
 *         be found.
 */
AutoPtr<IView> View::FindFocus()
{
    return (mPrivateFlags & PFLAG_FOCUSED) != 0 ?
            AutoPtr<IView>(IVIEW_PROBE(this)) : AutoPtr<IView>(NULL);
}

Boolean View::IsScrollContainer()
{
    return (mPrivateFlags & PFLAG_SCROLL_CONTAINER_ADDED) != 0;
}

/**
 * Change whether this view is one of the set of scrollable containers in
 * its window.  This will be used to determine whether the window can
 * resize or must pan when a soft input area is open -- scrollable
 * containers allow the window to use resize mode since the container
 * will appropriately shrink.
 */
ECode View::SetScrollContainer(
    /* [in] */ Boolean isScrollContainer)
{
    if (isScrollContainer) {
        if (mAttachInfo != NULL && (mPrivateFlags & PFLAG_SCROLL_CONTAINER_ADDED) == 0) {
            mAttachInfo->mScrollContainers.PushBack(this);
            mPrivateFlags |= PFLAG_SCROLL_CONTAINER_ADDED;
        }
        mPrivateFlags |= PFLAG_SCROLL_CONTAINER;
    }
    else {
        if ((mPrivateFlags & PFLAG_SCROLL_CONTAINER_ADDED) != 0) {
            mAttachInfo->mScrollContainers.Remove(this);
        }
        mPrivateFlags &= ~(PFLAG_SCROLL_CONTAINER | PFLAG_SCROLL_CONTAINER_ADDED);
    }

    return NOERROR;
}

/**
 * Returns the quality of the drawing cache.
 *
 * @return One of {@link #DRAWING_CACHE_QUALITY_AUTO},
 *         {@link #DRAWING_CACHE_QUALITY_LOW}, or {@link #DRAWING_CACHE_QUALITY_HIGH}
 *
 * @see #setDrawingCacheQuality(int)
 * @see #setDrawingCacheEnabled(Boolean)
 * @see #isDrawingCacheEnabled()
 *
 * @attr ref android.R.styleable#View_drawingCacheQuality
 */
Int32 View::GetDrawingCacheQuality()
{
    return mViewFlags & DRAWING_CACHE_QUALITY_MASK;
}

/**
 * Set the drawing cache quality of this view. This value is used only when the
 * drawing cache is enabled
 *
 * @param quality One of {@link #DRAWING_CACHE_QUALITY_AUTO},
 *        {@link #DRAWING_CACHE_QUALITY_LOW}, or {@link #DRAWING_CACHE_QUALITY_HIGH}
 *
 * @see #getDrawingCacheQuality()
 * @see #setDrawingCacheEnabled(Boolean)
 * @see #isDrawingCacheEnabled()
 *
 * @attr ref android.R.styleable#View_drawingCacheQuality
 */
ECode View::SetDrawingCacheQuality(
    /* [in] */ Int32 quality)
{
    SetFlags(quality, DRAWING_CACHE_QUALITY_MASK);

    return NOERROR;
}

/**
 * Returns whether the screen should remain on, corresponding to the current
 * value of {@link #KEEP_SCREEN_ON}.
 *
 * @return Returns TRUE if {@link #KEEP_SCREEN_ON} is set.
 *
 * @see #setKeepScreenOn(Boolean)
 *
 * @attr ref android.R.styleable#View_keepScreenOn
 */
Boolean View::GetKeepScreenOn()
{
    return (mViewFlags & IView::KEEP_SCREEN_ON) != 0;
}

/**
 * Controls whether the screen should remain on, modifying the
 * value of {@link #KEEP_SCREEN_ON}.
 *
 * @param keepScreenOn Supply TRUE to set {@link #KEEP_SCREEN_ON}.
 *
 * @see #getKeepScreenOn()
 *
 * @attr ref android.R.styleable#View_keepScreenOn
 */
ECode View::SetKeepScreenOn(
    /* [in] */ Boolean keepScreenOn)
{
    SetFlags(keepScreenOn ? IView::KEEP_SCREEN_ON : 0, IView::KEEP_SCREEN_ON);

    return NOERROR;
}

/**
 * @return The user specified next focus ID.
 *
 * @attr ref android.R.styleable#View_nextFocusLeft
 */
Int32 View::GetNextFocusLeftId()
{
    return mNextFocusLeftId;
}

/**
 * Set the id of the view to use for the next focus
 *
 * @param nextFocusLeftId
 *
 * @attr ref android.R.styleable#View_nextFocusLeft
 */
ECode View::SetNextFocusLeftId(
    /* [in] */ Int32 nextFocusLeftId)
{
    mNextFocusLeftId = nextFocusLeftId;

    return NOERROR;
}

/**
 * @return The user specified next focus ID.
 *
 * @attr ref android.R.styleable#View_nextFocusRight
 */
Int32 View::GetNextFocusRightId()
{
    return mNextFocusRightId;
}

/**
 * Set the id of the view to use for the next focus
 *
 * @param nextFocusRightId
 *
 * @attr ref android.R.styleable#View_nextFocusRight
 */
ECode View::SetNextFocusRightId(
    /* [in] */ Int32 nextFocusRightId)
{
    mNextFocusRightId = nextFocusRightId;

    return NOERROR;
}

/**
 * @return The user specified next focus ID.
 *
 * @attr ref android.R.styleable#View_nextFocusUp
 */
Int32 View::GetNextFocusUpId()
{
    return mNextFocusUpId;
}

/**
 * Set the id of the view to use for the next focus
 *
 * @param nextFocusUpId
 *
 * @attr ref android.R.styleable#View_nextFocusUp
 */
ECode View::SetNextFocusUpId(
    /* [in] */ Int32 nextFocusUpId)
{
    mNextFocusUpId = nextFocusUpId;

    return NOERROR;
}

/**
 * @return The user specified next focus ID.
 *
 * @attr ref android.R.styleable#View_nextFocusDown
 */
Int32 View::GetNextFocusDownId()
{
    return mNextFocusDownId;
}

/**
 * Set the id of the view to use for the next focus
 *
 * @param nextFocusDownId
 *
 * @attr ref android.R.styleable#View_nextFocusDown
 */
ECode View::SetNextFocusDownId(
    /* [in] */ Int32 nextFocusDownId)
{
    mNextFocusDownId = nextFocusDownId;

    return NOERROR;
}

Int32 View::GetNextFocusForwardId()
{
    return mNextFocusForwardId;
}

ECode View::SetNextFocusForwardId(
    /* [in] */ Int32 nextFocusForwardId)
{
    mNextFocusForwardId = nextFocusForwardId;
    return NOERROR;
}

/**
 * Returns the visibility of this view and all of its ancestors
 *
 * @return True if this view and all of its ancestors are {@link #VISIBLE}
 */
Boolean View::IsShown()
{
    View* current = this;
    //noinspection ConstantConditions
    do {
        if ((current->mViewFlags & VISIBILITY_MASK) != IView::VISIBLE) {
            return FALSE;
        }
        AutoPtr<IViewParent> parent = current->mParent;
        if (parent == NULL) {
            return FALSE; // We are not attached to the view root
        }
        if (parent->Probe(EIID_View) == NULL) {
            return TRUE;
        }
        current = VIEW_PROBE(parent);
    } while (current != NULL);

    return FALSE;
}

ECode View::FitSystemWindows(
    /* [in] */ IRect* _insets,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    if ((mPrivateFlags3 & PFLAG3_APPLYING_INSETS) == 0) {
        if (_insets == NULL) {
            // Null insets by definition have already been consumed.
            // This call cannot apply insets since there are none to apply,
            // so return false.
            *res = FALSE;
            return NOERROR;
        }
        // If we're not in the process of dispatching the newer apply insets call,
        // that means we're not in the compatibility path. Dispatch into the newer
        // apply insets path and take things from there.
        //try {
            ECode ec;
            mPrivateFlags3 |= PFLAG3_FITTING_SYSTEM_WINDOWS;
            AutoPtr<IWindowInsets> sets;
            CWindowInsets::New(_insets, (IWindowInsets**)&sets);
            ec = DispatchApplyWindowInsets(sets)->sConsumed(res);
            return ec;
        //} finally {
            mPrivateFlags3 &= ~PFLAG3_FITTING_SYSTEM_WINDOWS;
        //}
    } else {
        // We're being called from the newer apply insets path.
        // Perform the standard fallback behavior.
        ec = FitSystemWindowsInt(_insets, res);
        return ec;
    }
}

ECode View::FitSystemWindowsInt(
        /* [in] */ IRect* insets,
        /* [in] */ Boolean* res)
{
    if ((mViewFlags & FITS_SYSTEM_WINDOWS) == FITS_SYSTEM_WINDOWS) {
        mUserPaddingStart = UNDEFINED_PADDING;
        mUserPaddingEnd = UNDEFINED_PADDING;
        AutoPtr<CRect> localInsets = (CRect*)pthread_getspecific(sKey);
        if (!localInsets) {
            CRect::NewByFriend((CRect**)&localInsets);
            pthread_setspecific(sKey, localInsets.Get());
        }

        Boolean result = ComputeFitSystemWindows(insets, localInsets);
        mUserPaddingLeftInitial = localInsets->mLeft;
        mUserPaddingRightInitial = localInsets->mRight;
        InternalSetPadding(localInsets->mLeft, localInsets->mTop,
                localInsets->mRight, localInsets->mBottom);
        *res = result;
        return NOERROR;
    }
    *res = FALSE;
    return NOERROR;
}

/**
 * Called when the view should apply {@link WindowInsets} according to its internal policy.
 *
 * <p>This method should be overridden by views that wish to apply a policy different from or
 * in addition to the default behavior. Clients that wish to force a view subtree
 * to apply insets should call {@link #dispatchApplyWindowInsets(WindowInsets)}.</p>
 *
 * <p>Clients may supply an {@link OnApplyWindowInsetsListener} to a view. If one is set
 * it will be called during dispatch instead of this method. The listener may optionally
 * call this method from its own implementation if it wishes to apply the view's default
 * insets policy in addition to its own.</p>
 *
 * <p>Implementations of this method should either return the insets parameter unchanged
 * or a new {@link WindowInsets} cloned from the supplied insets with any insets consumed
 * that this view applied itself. This allows new inset types added in future platform
 * versions to pass through existing implementations unchanged without being erroneously
 * consumed.</p>
 *
 * <p>By default if a view's {@link #setFitsSystemWindows(boolean) fitsSystemWindows}
 * property is set then the view will consume the system window insets and apply them
 * as padding for the view.</p>
 *
 * @param insets Insets to apply
 * @return The supplied insets with any applied insets consumed
 */
AutoPtr<IWindowInsets> View::OnApplyWindowInsets(
        /* [in] */ IWindowInsets* insets)
{
    if ((mPrivateFlags3 & PFLAG3_FITTING_SYSTEM_WINDOWS) == 0) {
        // We weren't called from within a direct call to fitSystemWindows,
        // call into it as a fallback in case we're in a class that overrides it
        // and has logic to perform.
        AutoPtr<IRect> rect;
        insets->GetSystemWindowInsets((IRect**)&rect);
        Boolean res;
        FitSystemWindows(rect, &res);
        if (res) {
            AutoPtr<IWindowInsets> result;
            insets->ConsumeSystemWindowInsets((IWindowInsets**)&result);
            return result;
        }
    } else {
        AutoPtr<IRect> rect;
        insets->GetSystemWindowInsets((IRect**)&rect);
        Boolean res;
        FitSystemWindowsInt(rect, &res);
        // We were called from within a direct call to fitSystemWindows.
        if (res) {
            AutoPtr<IWindowInsets> result;
            insets->ConsumeSystemWindowInsets((IWindowInsets**)&result);
            return result;
        }
    }
    return insets;
}

/**
 * Set an {@link OnApplyWindowInsetsListener} to take over the policy for applying
 * window insets to this view. The listener's
 * {@link OnApplyWindowInsetsListener#onApplyWindowInsets(View, WindowInsets) onApplyWindowInsets}
 * method will be called instead of the view's
 * {@link #onApplyWindowInsets(WindowInsets) onApplyWindowInsets} method.
 *
 * @param listener Listener to set
 *
 * @see #onApplyWindowInsets(WindowInsets)
 */
void View::SetOnApplyWindowInsetsListener(
    /* [in] */ IViewOnApplyWindowInsetsListener listener)
{
    GetListenerInfo()->mOnApplyWindowInsetsListener = listener;
}

/**
 * Request to apply the given window insets to this view or another view in its subtree.
 *
 * <p>This method should be called by clients wishing to apply insets corresponding to areas
 * obscured by window decorations or overlays. This can include the status and navigation bars,
 * action bars, input methods and more. New inset categories may be added in the future.
 * The method returns the insets provided minus any that were applied by this view or its
 * children.</p>
 *
 * <p>Clients wishing to provide custom behavior should override the
 * {@link #onApplyWindowInsets(WindowInsets)} method or alternatively provide a
 * {@link OnApplyWindowInsetsListener} via the
 * {@link #setOnApplyWindowInsetsListener(View.OnApplyWindowInsetsListener) setOnApplyWindowInsetsListener}
 * method.</p>
 *
 * <p>This method replaces the older {@link #fitSystemWindows(Rect) fitSystemWindows} method.
 * </p>
 *
 * @param insets Insets to apply
 * @return The provided insets minus the insets that were consumed
 */
ECode View::DispatchApplyWindowInsets(
    /* [in] */ IWindowInsets* insets,
    /* [out] */ IWindowInsets** res)
{
    //try {
    mPrivateFlags3 |= PFLAG3_APPLYING_INSETS;
    if (mListenerInfo != NULL && mListenerInfo->mOnApplyWindowInsetsListener != NULL) {
        return mListenerInfo->mOnApplyWindowInsetsListener->OnApplyWindowInsets(THIS_PROBE(IView), insets);
    } else {
        return OnApplyWindowInsets(insets);
    }
    //} finally {
    mPrivateFlags3 &= ~PFLAG3_APPLYING_INSETS;
    //}
}

/**
 * @hide Compute the insets that should be consumed by this view and the ones
 * that should propagate to those under it.
 */
Boolean View::ComputeFitSystemWindows(
    /* [in] */ IRect* inoutInsets,
    /* [in] */ IRect* outLocalInsets)
{
    if ((mViewFlags & OPTIONAL_FITS_SYSTEM_WINDOWS) == 0
            || mAttachInfo == NULL
            || ((mAttachInfo->mSystemUiVisibility & SYSTEM_UI_LAYOUT_FLAGS) == 0
                    && !mAttachInfo->mOverscanRequested)) {
        outLocalInsets->Set(inoutInsets.Get());
        inoutInsets->Set(0, 0, 0, 0);
        return TRUE;
    } else {
        // The application wants to take care of fitting system window for
        // the content...  however we still need to take care of any overscan here.
        AutoPtr<CRect> overscan = mAttachInfo->mOverscanInsets;
        outLocalInsets->Set(overscan.Get());
        inoutInsets->SetLeft(overscan->mLeft);
        inoutInsets->SetTop(overscan->mTop);
        inoutInsets->SetRight(overscan->mRight);
        inoutInsets->SetBottom(overscan->mBottom);
        return FALSE;
    }
}

/**
 * Compute insets that should be consumed by this view and the ones that should propagate
 * to those under it.
 *
 * @param in Insets currently being processed by this View, likely received as a parameter
 *           to {@link #onApplyWindowInsets(WindowInsets)}.
 * @param outLocalInsets A Rect that will receive the insets that should be consumed
 *                       by this view
 * @return Insets that should be passed along to views under this one
 */
ECode ComputeSystemWindowInsets(
    /* [in] */ IWindowInsets* in,
    /* [in] */ IRect* outLocalInsets,
    /* [out] */ IWindowInsets** res)
{
    VALIDATE_NOT_NULL(res);
    if ((mViewFlags & OPTIONAL_FITS_SYSTEM_WINDOWS) == 0 || mAttachInfo == NULL
            || (mAttachInfo->mSystemUiVisibility & SYSTEM_UI_LAYOUT_FLAGS) == 0) {
        AutoPtr<IRect> rect;
        in->GetSystemWindowInsets((IRect**)&rect);
        outLocalInsets->Set(rect);
        return in->ConsumeSystemWindowInsets(res);
    } else {
        outLocalInsets->Set(0, 0, 0, 0);
        *res = in;
        REFCOUNT_ADD(*res);
        return NOERROR;
    }
}



/**
 * Sets whether or not this view should account for system screen decorations
 * such as the status bar and inset its content; that is, controlling whether
 * the default implementation of {@link #fitSystemWindows(Rect)} will be
 * executed.  See that method for more details.
 *
 * <p>Note that if you are providing your own implementation of
 * {@link #fitSystemWindows(Rect)}, then there is no need to set this
 * flag to TRUE -- your implementation will be overriding the default
 * implementation that checks this flag.
 *
 * @param fitSystemWindows If TRUE, then the default implementation of
 * {@link #fitSystemWindows(Rect)} will be executed.
 *
 * @attr ref android.R.styleable#View_fitsSystemWindows
 * @see #getFitsSystemWindows()
 * @see #fitSystemWindows(Rect)
 * @see #setSystemUiVisibility(int)
 */
ECode View::SetFitsSystemWindows(
    /* [in] */ Boolean fitSystemWindows)
{
    SetFlags(fitSystemWindows ? FITS_SYSTEM_WINDOWS : 0, FITS_SYSTEM_WINDOWS);
    return NOERROR;
}

/**
 * Check for state of {@link #setFitsSystemWindows(boolean). If this method
 * returns TRUE, the default implementation of {@link #fitSystemWindows(Rect)}
 * will be executed.
 *
 * @return Returns TRUE if the default implementation of
 * {@link #fitSystemWindows(Rect)} will be executed.
 *
 * @attr ref android.R.styleable#View_fitsSystemWindows
 * @see #setFitsSystemWindows()
 * @see #fitSystemWindows(Rect)
 * @see #setSystemUiVisibility(int)
 */
Boolean View::GetFitsSystemWindows()
{
    return (mViewFlags & FITS_SYSTEM_WINDOWS) == FITS_SYSTEM_WINDOWS;
}

/** @hide */
Boolean View::FitsSystemWindows()
{
    return GetFitsSystemWindows();
}

/**
 * Ask that a new dispatch of {@link #fitSystemWindows(Rect)} be performed.
 */
ECode View::RequestFitSystemWindows()
{
    if (mParent != NULL) {
        return mParent->RequestFitSystemWindows();
    }
    return NOERROR;
}

/**
 * Ask that a new dispatch of {@link #onApplyWindowInsets(WindowInsets)} be performed.
 */
ECode View::RequestApplyInsets()
{
    RequestFitSystemWindows();
    return NOERROR;
}

/**
 * For use by PhoneWindow to make its own system window fitting optional.
 * @hide
 */
ECode View::MakeOptionalFitsSystemWindows()
{
    SetFlags(OPTIONAL_FITS_SYSTEM_WINDOWS, OPTIONAL_FITS_SYSTEM_WINDOWS);
    return NOERROR;
}

Int32 View::GetVisibility()
{
    return mViewFlags & VISIBILITY_MASK;
}

/**
 * Set the enabled state of this view.
 *
 * @param visibility One of {@link #VISIBLE}, {@link #INVISIBLE}, or {@link #GONE}.
 * @attr ref android.R.styleable#View_visibility
 */
ECode View::SetVisibility(
    /* [in] */ Int32 visibility)
{
    SetFlags(visibility, VISIBILITY_MASK);
    if (mBackground != NULL) {
        Boolean result;
        mBackground->SetVisible(visibility == IView::VISIBLE, FALSE, &result);
    }
    return NOERROR;
}

/**
 * Returns the enabled status for this view. The interpretation of the
 * enabled state varies by subclass.
 *
 * @return True if this view is enabled, FALSE otherwise.
 */
Boolean View::IsEnabled()
{
    return (mViewFlags & ENABLED_MASK) == ENABLED;
}

/**
 * Set the enabled state of this view. The interpretation of the enabled
 * state varies by subclass.
 *
 * @param enabled True if this view is enabled, FALSE otherwise.
 */
ECode View::SetEnabled(
    /* [in] */ Boolean enabled)
{
    if (enabled == IsEnabled()) return NOERROR;

    Int32 flags = DISABLED;
    if (enabled) flags = ENABLED;
    SetFlags(flags, ENABLED_MASK);

    /*
     * The View most likely has to change its appearance, so refresh
     * the drawable state.
     */
    RefreshDrawableState();

    // Invalidate too, since the default behavior for views is to be
    // be drawn at 50% alpha rather than to change the drawable.
    Invalidate(TRUE);

    if (!enabled) {
        CancelPendingInputEvents();
    }

    return NOERROR;
}

/**
 * Set whether this view can receive the focus.
 *
 * Setting this to FALSE will also ensure that this view is not focusable
 * in touch mode.
 *
 * @param focusable If TRUE, this view can receive the focus.
 *
 * @see #setFocusableInTouchMode(Boolean)
 * @attr ref android.R.styleable#View_focusable
 */
ECode View::SetFocusable(
    /* [in] */ Boolean focusable)
{
    if (!focusable) {
        SetFlags(0, FOCUSABLE_IN_TOUCH_MODE);
    }
    Int32 flags = NOT_FOCUSABLE;
    if (focusable) flags = FOCUSABLE;
    SetFlags(flags, FOCUSABLE_MASK);

    return NOERROR;
}

/**
 * Set whether this view can receive focus while in touch mode.
 *
 * Setting this to TRUE will also ensure that this view is focusable.
 *
 * @param focusableInTouchMode If TRUE, this view can receive the focus while
 *   in touch mode.
 *
 * @see #setFocusable(Boolean)
 * @attr ref android.R.styleable#View_focusableInTouchMode
 */
ECode View::SetFocusableInTouchMode(
    /* [in] */ Boolean focusableInTouchMode)
{
    // Focusable in touch mode should always be set before the focusable flag
    // otherwise, setting the focusable flag will trigger a focusableViewAvailable()
    // which, in touch mode, will not successfully request focus on this view
    // because the focusable in touch mode flag is not set
    SetFlags(focusableInTouchMode ? FOCUSABLE_IN_TOUCH_MODE : 0, FOCUSABLE_IN_TOUCH_MODE);
    if (focusableInTouchMode) {
        SetFlags(FOCUSABLE, FOCUSABLE_MASK);
    }

    return NOERROR;
}

/**
 * Set whether this view should have sound effects enabled for events such as
 * clicking and touching.
 *
 * <p>You may wish to disable sound effects for a view if you already play sounds,
 * for instance, a dial key that plays dtmf tones.
 *
 * @param soundEffectsEnabled whether sound effects are enabled for this view.
 * @see #isSoundEffectsEnabled()
 * @see #playSoundEffect(int)
 * @attr ref android.R.styleable#View_soundEffectsEnabled
 */
ECode View::SetSoundEffectsEnabled(
    /* [in] */ Boolean soundEffectsEnabled)
{
    SetFlags(soundEffectsEnabled ? IView::SOUND_EFFECTS_ENABLED: 0, IView::SOUND_EFFECTS_ENABLED);

    return NOERROR;
}

/**
 * @return whether this view should have sound effects enabled for events such as
 *     clicking and touching.
 *
 * @see #setSoundEffectsEnabled(Boolean)
 * @see #playSoundEffect(int)
 * @attr ref android.R.styleable#View_soundEffectsEnabled
 */
Boolean View::IsSoundEffectsEnabled()
{
    return IView::SOUND_EFFECTS_ENABLED == (mViewFlags & IView::SOUND_EFFECTS_ENABLED);
}

/**
 * Set whether this view should have haptic feedback for events such as
 * long presses.
 *
 * <p>You may wish to disable haptic feedback if your view already controls
 * its own haptic feedback.
 *
 * @param hapticFeedbackEnabled whether haptic feedback enabled for this view.
 * @see #isHapticFeedbackEnabled()
 * @see #performHapticFeedback(int)
 * @attr ref android.R.styleable#View_hapticFeedbackEnabled
 */
ECode View::SetHapticFeedbackEnabled(
    /* [in] */ Boolean hapticFeedbackEnabled)
{
    SetFlags(hapticFeedbackEnabled ? IView::HAPTIC_FEEDBACK_ENABLED: 0, IView::HAPTIC_FEEDBACK_ENABLED);

    return NOERROR;
}

/**
 * @return whether this view should have haptic feedback enabled for events
 * long presses.
 *
 * @see #setHapticFeedbackEnabled(Boolean)
 * @see #performHapticFeedback(int)
 * @attr ref android.R.styleable#View_hapticFeedbackEnabled
 */
Boolean View::IsHapticFeedbackEnabled()
{
    return IView::HAPTIC_FEEDBACK_ENABLED == (mViewFlags & IView::HAPTIC_FEEDBACK_ENABLED);
}

Int32 View::GetRawLayoutDirection()
{
    return (mPrivateFlags2 & PFLAG2_LAYOUT_DIRECTION_MASK) >> PFLAG2_LAYOUT_DIRECTION_MASK_SHIFT;
}

ECode View::SetLayoutDirection(
    /* [in] */ Int32 layoutDirection)
{
    if (GetRawLayoutDirection() != layoutDirection) {
        // Reset the current layout direction and the resolved one
        mPrivateFlags2 &= ~PFLAG2_LAYOUT_DIRECTION_MASK;
        ResetRtlProperties();
        // Set the new layout direction (filtered)
        mPrivateFlags2 |=
                ((layoutDirection << PFLAG2_LAYOUT_DIRECTION_MASK_SHIFT) & PFLAG2_LAYOUT_DIRECTION_MASK);
        // We need to resolve all RTL properties as they all depend on layout direction
        ResolveRtlPropertiesIfNeeded();
        RequestLayout();
        Invalidate(TRUE);
    }

    return NOERROR;
}

Int32 View::GetLayoutDirection()
{
    Int32 targetSdkVersion = 0;
    AutoPtr<IApplicationInfo> info;
    GetContext()->GetApplicationInfo((IApplicationInfo**)&info);
    assert(info != NULL);
    info->GetTargetSdkVersion(&targetSdkVersion);
    if (targetSdkVersion < Build::VERSION_CODES::JELLY_BEAN_MR1) {
        mPrivateFlags2 |= PFLAG2_LAYOUT_DIRECTION_RESOLVED;
        return IView::LAYOUT_DIRECTION_RESOLVED_DEFAULT;
    }

    return ((mPrivateFlags2 & PFLAG2_LAYOUT_DIRECTION_RESOLVED_RTL) ==
            PFLAG2_LAYOUT_DIRECTION_RESOLVED_RTL) ? IView::LAYOUT_DIRECTION_RTL : IView::LAYOUT_DIRECTION_LTR;
}

Boolean View::IsLayoutRtl()
{
    return (GetLayoutDirection() == IView::LAYOUT_DIRECTION_RTL);
}

Boolean View::HasTransientState()
{
    return (mPrivateFlags2 & PFLAG2_HAS_TRANSIENT_STATE) == PFLAG2_HAS_TRANSIENT_STATE;
}

ECode View::SetHasTransientState(
    /* [in] */ Boolean hasTransientState)
{
    mTransientStateCount = hasTransientState ? mTransientStateCount + 1 :
            mTransientStateCount - 1;
    if (mTransientStateCount < 0) {
        mTransientStateCount = 0;
        // Log.e(VIEW_LOG_TAG, "hasTransientState decremented below 0: " +
        //         "unmatched pair of setHasTransientState calls");
    } else if ((hasTransientState && mTransientStateCount == 1) ||
            (!hasTransientState && mTransientStateCount == 0)) {
        // update flag if we've just incremented up from 0 or decremented down to 0
        mPrivateFlags2 = (mPrivateFlags2 & ~PFLAG2_HAS_TRANSIENT_STATE) |
                (hasTransientState ? PFLAG2_HAS_TRANSIENT_STATE : 0);
        if (mParent != NULL) {
            mParent->ChildHasTransientStateChanged(IVIEW_PROBE(this), hasTransientState);
            // } catch (AbstractMethodError e) {
            //     Log.e(VIEW_LOG_TAG, mParent.getClass().getSimpleName() +
            //             " does not fully implement ViewParent", e);
            // }
        }
    }

    return NOERROR;
}

/**
 * Returns true if this view is currently attached to a window.
 */
ECode View::IsAttachedToWindow(
        /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)

    *res = mAttachInfo != NULL;
    return NOERROR;
}

/**
 * Returns true if this view has been through at least one layout since it
 * was last attached to or detached from a window.
 */
ECode View::IsLaidOut(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = (mPrivateFlags3 & PFLAG3_IS_LAID_OUT) == PFLAG3_IS_LAID_OUT;
    return NOERROR;
}

/**
 * If this view doesn't do any drawing on its own, set this flag to
 * allow further optimizations. By default, this flag is not set on
 * View, but could be set on some View subclasses such as ViewGroup.
 *
 * Typically, if you override {@link #onDraw} you should clear this flag.
 *
 * @param willNotDraw whether or not this View draw on its own
 */
ECode View::SetWillNotDraw(
    /* [in] */ Boolean willNotDraw)
{
    SetFlags(willNotDraw ? WILL_NOT_DRAW : 0, DRAW_MASK);

    return NOERROR;
}

/**
 * Returns whether or not this View draws on its own.
 *
 * @return TRUE if this view has nothing to draw, FALSE otherwise
 */
Boolean View::WillNotDraw()
{
    return (mViewFlags & DRAW_MASK) == WILL_NOT_DRAW;
}

/**
 * When a View's drawing cache is enabled, drawing is redirected to an
 * offscreen bitmap. Some views, like an ImageView, must be able to
 * bypass this mechanism if they already draw a single bitmap, to avoid
 * unnecessary usage of the memory.
 *
 * @param willNotCacheDrawing TRUE if this view does not cache its
 *        drawing, FALSE otherwise
 */
ECode View::SetWillNotCacheDrawing(
    /* [in] */ Boolean willNotCacheDrawing)
{
    SetFlags(willNotCacheDrawing ? WILL_NOT_CACHE_DRAWING : 0, WILL_NOT_CACHE_DRAWING);

    return NOERROR;
}

/**
 * Returns whether or not this View can cache its drawing or not.
 *
 * @return TRUE if this view does not cache its drawing, FALSE otherwise
 */
Boolean View::WillNotCacheDrawing()
{
    return (mViewFlags & WILL_NOT_CACHE_DRAWING) == WILL_NOT_CACHE_DRAWING;
}

/**
 * Indicates whether this view reacts to click events or not.
 *
 * @return TRUE if the view is clickable, FALSE otherwise
 *
 * @see #setClickable(Boolean)
 * @attr ref android.R.styleable#View_clickable
 */
Boolean View::IsClickable()
{
    return (mViewFlags & CLICKABLE) == CLICKABLE;
}

/**
 * Enables or disables click events for this view. When a view
 * is clickable it will change its state to "pressed" on every click.
 * Subclasses should set the view clickable to visually react to
 * user's clicks.
 *
 * @param clickable TRUE to make the view clickable, FALSE otherwise
 *
 * @see #isClickable()
 * @attr ref android.R.styleable#View_clickable
 */
ECode View::SetClickable(
    /* [in] */ Boolean clickable)
{
    SetFlags(clickable ? CLICKABLE : 0, CLICKABLE);

    return NOERROR;
}

/**
 * Indicates whether this view reacts to long click events or not.
 *
 * @return TRUE if the view is long clickable, FALSE otherwise
 *
 * @see #setLongClickable(Boolean)
 * @attr ref android.R.styleable#View_longClickable
 */
Boolean View::IsLongClickable()
{
    return (mViewFlags & LONG_CLICKABLE) == LONG_CLICKABLE;
}

/**
 * Enables or disables long click events for this view. When a view is long
 * clickable it reacts to the user holding down the button for a longer
 * duration than a tap. This event can either launch the listener or a
 * context menu.
 *
 * @param longClickable TRUE to make the view long clickable, FALSE otherwise
 * @see #isLongClickable()
 * @attr ref android.R.styleable#View_longClickable
 */
ECode View::SetLongClickable(
    /* [in] */ Boolean longClickable)
{
    SetFlags(longClickable ? LONG_CLICKABLE : 0, LONG_CLICKABLE);

    return NOERROR;
}

/**
 * Sets the pressed state for this view and provides a touch coordinate for
 * animation hinting.
 *
 * @param pressed Pass true to set the View's internal state to "pressed",
 *            or false to reverts the View's internal state from a
 *            previously set "pressed" state.
 * @param x The x coordinate of the touch that caused the press
 * @param y The y coordinate of the touch that caused the press
 */
void View::SetPressed(
    /* [in] */ Boolean pressed,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    if (pressed) {
        DrawableHotspotChanged(x, y);
    }

    SetPressed(pressed);
}

/**
 * Sets the pressed that for this view.
 *
 * @see #isClickable()
 * @see #setClickable(Boolean)
 *
 * @param pressed Pass TRUE to set the View's internal state to "pressed", or FALSE to reverts
 *        the View's internal state from a previously set "pressed" state.
 */
ECode View::SetPressed(
    /* [in] */ Boolean pressed)
{
    Boolean needsRefresh = pressed != ((mPrivateFlags & PFLAG_PRESSED) == PFLAG_PRESSED);

    if (pressed) {
        mPrivateFlags |= PFLAG_PRESSED;
    }
    else {
        mPrivateFlags &= ~PFLAG_PRESSED;
    }

    if (needsRefresh) {
        RefreshDrawableState();
    }

    DispatchSetPressed(pressed);

    return NOERROR;
}

/**
 * Dispatch setPressed to all of this View's children.
 *
 * @see #setPressed(Boolean)
 *
 * @param pressed The new pressed state
 */
void View::DispatchSetPressed(
    /* [in] */ Boolean pressed)
{
}

/**
 * Indicates whether the view is currently in pressed state. Unless
 * {@link #setPressed(Boolean)} is explicitly called, only clickable views can enter
 * the pressed state.
 *
 * @see #setPressed
 * @see #isClickable()
 * @see #setClickable(Boolean)
 *
 * @return TRUE if the view is currently pressed, FALSE otherwise
 */
Boolean View::IsPressed()
{
    return (mPrivateFlags & PFLAG_PRESSED) == PFLAG_PRESSED;
}

/**
 * Indicates whether this view will save its state (that is,
 * whether its {@link #onSaveInstanceState} method will be called).
 *
 * @return Returns TRUE if the view state saving is enabled, else FALSE.
 *
 * @see #setSaveEnabled(Boolean)
 * @attr ref android.R.styleable#View_saveEnabled
 */
Boolean View::IsSaveEnabled()
{
    return (mViewFlags & SAVE_DISABLED_MASK) != SAVE_DISABLED;
}

/**
 * Controls whether the saving of this view's state is
 * enabled (that is, whether its {@link #onSaveInstanceState} method
 * will be called).  Note that even if freezing is enabled, the
 * view still must have an id assigned to it (via {@link #setId setId()})
 * for its state to be saved.  This flag can only disable the
 * saving of this view; any child views may still have their state saved.
 *
 * @param enabled Set to FALSE to <em>disable</em> state saving, or TRUE
 * (the default) to allow it.
 *
 * @see #isSaveEnabled()
 * @see #setId(int)
 * @see #onSaveInstanceState()
 * @attr ref android.R.styleable#View_saveEnabled
 */
ECode View::SetSaveEnabled(
    /* [in] */ Boolean enabled)
{
    SetFlags(enabled ? 0 : SAVE_DISABLED, SAVE_DISABLED_MASK);

    return NOERROR;
}

/**
 * Gets whether the framework should discard touches when the view's
 * window is obscured by another visible window.
 * Refer to the {@link View} security documentation for more details.
 *
 * @return True if touch filtering is enabled.
 *
 * @see #setFilterTouchesWhenObscured(Boolean)
 * @attr ref android.R.styleable#View_filterTouchesWhenObscured
 */
Boolean View::GetFilterTouchesWhenObscured()
{
    return (mViewFlags & FILTER_TOUCHES_WHEN_OBSCURED) != 0;
}

/**
 * Sets whether the framework should discard touches when the view's
 * window is obscured by another visible window.
 * Refer to the {@link View} security documentation for more details.
 *
 * @param enabled True if touch filtering should be enabled.
 *
 * @see #getFilterTouchesWhenObscured
 * @attr ref android.R.styleable#View_filterTouchesWhenObscured
 */
ECode View::SetFilterTouchesWhenObscured(
    /* [in] */ Boolean enabled)
{
    SetFlags(enabled ? FILTER_TOUCHES_WHEN_OBSCURED : 0,
            FILTER_TOUCHES_WHEN_OBSCURED);

    return NOERROR;
}

Boolean View::IsSaveFromParentEnabled()
{
    return (mViewFlags & PARENT_SAVE_DISABLED_MASK) != PARENT_SAVE_DISABLED;
}

ECode View::SetSaveFromParentEnabled(
    /* [in] */ Boolean enabled)
{
    SetFlags(enabled ? 0 : PARENT_SAVE_DISABLED, PARENT_SAVE_DISABLED_MASK);
    return NOERROR;
}

/**
 * Returns whether this View is able to take focus.
 *
 * @return True if this view can take focus, or FALSE otherwise.
 * @attr ref android.R.styleable#View_focusable
 */
Boolean View::IsFocusable()
{
    return FOCUSABLE == (mViewFlags & FOCUSABLE_MASK);
}

/**
 * When a view is focusable, it may not want to take focus when in touch mode.
 * For example, a button would like focus when the user is navigating via a D-pad
 * so that the user can click on it, but once the user starts touching the screen,
 * the button shouldn't take focus
 * @return Whether the view is focusable in touch mode.
 * @attr ref android.R.styleable#View_focusableInTouchMode
 */
Boolean View::IsFocusableInTouchMode()
{
    return FOCUSABLE_IN_TOUCH_MODE == (mViewFlags & FOCUSABLE_IN_TOUCH_MODE);
}

/**
 * Find the nearest view in the specified direction that can take focus.
 * This does not actually give focus to that view.
 *
 * @param direction One of FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, and FOCUS_RIGHT
 *
 * @return The nearest focusable in the specified direction, or NULL if none
 *         can be found.
 */
AutoPtr<IView> View::FocusSearch(
    /* [in] */ Int32 direction)
{
    if (mParent != NULL) {
        AutoPtr<IView> view;
        mParent->FocusSearch(IVIEW_PROBE(this), direction, (IView**)&view);
        return view;
    }
    else {
        return AutoPtr<IView>(NULL);
    }
}

/**
 * This method is the last chance for the focused view and its ancestors to
 * respond to an arrow key. This is called when the focused view did not
 * consume the key internally, nor could the view system find a new view in
 * the requested direction to give focus to.
 *
 * @param focused The currently focused view.
 * @param direction The direction focus wants to move. One of FOCUS_UP,
 *        FOCUS_DOWN, FOCUS_LEFT, and FOCUS_RIGHT.
 * @return True if the this view consumed this unhandled move.
 */
Boolean View::DispatchUnhandledMove(
    /* [in] */ IView* focused,
    /* [in] */ Int32 direction)
{
    return FALSE;
}

/**
 * If a user manually specified the next view id for a particular direction,
 * use the root to look up the view.
 * @param root The root view of the hierarchy containing this view.
 * @param direction One of FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT, FOCUS_FORWARD,
 * or FOCUS_BACKWARD.
 * @return The user specified next view, or NULL if there is none.
 */
AutoPtr<IView> View::FindUserSetNextFocus(
    /* [in] */ IView* root,
    /* [in] */ Int32 direction)
{
    switch (direction) {
        case IView::FOCUS_LEFT:
            if (mNextFocusLeftId == IView::NO_ID) return NULL;
            return FindViewInsideOutShouldExist(root, mNextFocusLeftId);
        case IView::FOCUS_RIGHT:
            if (mNextFocusRightId == IView::NO_ID) return NULL;
            return FindViewInsideOutShouldExist(root, mNextFocusRightId);
        case IView::FOCUS_UP:
            if (mNextFocusUpId == IView::NO_ID) return NULL;
            return FindViewInsideOutShouldExist(root, mNextFocusUpId);
        case IView::FOCUS_DOWN:
            if (mNextFocusDownId == IView::NO_ID) return NULL;
            return FindViewInsideOutShouldExist(root, mNextFocusDownId);
        case IView::FOCUS_FORWARD:
            if (mNextFocusForwardId == IView::NO_ID) return NULL;
            return FindViewInsideOutShouldExist(root, mNextFocusForwardId);
        case IView::FOCUS_BACKWARD: {
            if (mID == IView::NO_ID) return NULL;
            const Int32 id = mID;
            AutoPtr<IView> view;
            AutoPtr<_Predicate> p = new _Predicate(id);
            root->FindViewByPredicateInsideOut(
                IVIEW_PROBE(this), p, (IView**)&view);
            return view;
        }
    }

    return AutoPtr<IView>(NULL);
}

AutoPtr<IView> View::FindViewInsideOutShouldExist(
    /* [in] */ IView* root,
    /* [in] */ Int32 id)
{
    if (mMatchIdPredicate == NULL) {
        mMatchIdPredicate = new MatchIdPredicate();
    }

    mMatchIdPredicate->mId = id;
    AutoPtr<IView> result;
    root->FindViewByPredicateInsideOut(
        IVIEW_PROBE(this), mMatchIdPredicate, (IView**)&result);
    if (result == NULL) {
        //Log.w(VIEW_LOG_TAG, "couldn't find view with id " + id);
    }

    return result;
}

AutoPtr<IView> View::FindViewShouldExist(
    /* [in] */ IView* root,
    /* [in] */ Int32 childViewId)
{
    AutoPtr<IView> result;
    root->FindViewById(childViewId, (IView**)&result);
    if (result == NULL) {
        //Log.w(VIEW_LOG_TAG, "couldn't find next focus view specified "
        //        + "by user for id " + childViewId);
    }
    return result;
}

/**
 * Find and return all focusable views that are descendants of this view,
 * possibly including this view if it is focusable itself.
 *
 * @param direction The direction of the focus
 * @return A list of focusable views
 */
ECode View::GetFocusables(
    /* [in] */ Int32 direction,
    /* [out] */ IObjectContainer** views)
{
    assert(views != NULL);

    FAIL_RETURN(CParcelableObjectContainer::New(views));
    return AddFocusables(*views, direction);
}

/**
 * Add any focusable views that are descendants of this view (possibly
 * including this view if it is focusable itself) to views.  If we are in touch mode,
 * only add views that are also focusable in touch mode.
 *
 * @param views Focusable views found so far
 * @param direction The direction of the focus
 */
ECode View::AddFocusables(
    /* [in] */ IObjectContainer* views,
    /* [in] */ Int32 direction)
{
    return AddFocusables(views, direction, IView::FOCUSABLES_TOUCH_MODE);
}

/**
 * Adds any focusable views that are descendants of this view (possibly
 * including this view if it is focusable itself) to views. This method
 * adds all focusable views regardless if we are in touch mode or
 * only views focusable in touch mode if we are in touch mode depending on
 * the focusable mode paramater.
 *
 * @param views Focusable views found so far or NULL if all we are interested is
 *        the number of focusables.
 * @param direction The direction of the focus.
 * @param focusableMode The type of focusables to be added.
 *
 * @see #FOCUSABLES_ALL
 * @see #FOCUSABLES_TOUCH_MODE
 */
ECode View::AddFocusables(
    /* [in, out] */ IObjectContainer* views,
    /* [in] */ Int32 direction,
    /* [in] */ Int32 focusableMode)
{
    if (views == NULL) {
        return NOERROR;
    }

    if (!IsFocusable()) {
        return NOERROR;
    }

    if ((focusableMode & IView::FOCUSABLES_TOUCH_MODE) == IView::FOCUSABLES_TOUCH_MODE &&
            IsInTouchMode() && !IsFocusableInTouchMode()) {
        return NOERROR;
    }

    views->Add(IVIEW_PROBE(this));

    return NOERROR;
}

ECode View::FindViewsWithText(
    /* [in, out] */ IObjectContainer* outViews,
    /* [in] */ ICharSequence* searched,
    /* [in] */ Int32 flags)
{
    Int32 len;
    if (GetAccessibilityNodeProvider() != NULL) {
        if ((flags & IView::FIND_VIEWS_WITH_ACCESSIBILITY_NODE_PROVIDERS) != 0) {
            outViews->Add(IVIEW_PROBE(this));
        }
    }
    else if ((flags & IView::FIND_VIEWS_WITH_CONTENT_DESCRIPTION) != 0
            && (searched != NULL && (searched->GetLength(&len), len) > 0)
            && (mContentDescription != NULL && (mContentDescription->GetLength(&len), len) > 0)) {
        String searchedLowerCase;
        searched->ToString(&searchedLowerCase);
        searchedLowerCase = searchedLowerCase.ToLowerCase();

        String contentDescriptionLowerCase;
        mContentDescription->ToString(&contentDescriptionLowerCase);
        contentDescriptionLowerCase = contentDescriptionLowerCase.ToLowerCase();
        if (contentDescriptionLowerCase.Contains(searchedLowerCase)) {
            outViews->Add(IVIEW_PROBE(this));
        }
    }

    return NOERROR;
}

/**
 * Find and return all touchable views that are descendants of this view,
 * possibly including this view if it is touchable itself.
 *
 * @return A list of touchable views
 */
ECode View::GetTouchables(
    /* [out] */ IObjectContainer** views)
{
    assert(views != NULL);

    FAIL_RETURN(CParcelableObjectContainer::New(views));
    return AddTouchables(*views);
}

/**
 * Add any touchable views that are descendants of this view (possibly
 * including this view if it is touchable itself) to views.
 *
 * @param views Touchable views found so far
 */
ECode View::AddTouchables(
    /* [in, out] */ IObjectContainer* views)
{
    Int32 viewFlags = mViewFlags;

    if (((viewFlags & CLICKABLE) == CLICKABLE || (viewFlags & LONG_CLICKABLE) == LONG_CLICKABLE)
            && (viewFlags & ENABLED_MASK) == ENABLED) {
        views->Add(IVIEW_PROBE(this));
    }

    return NOERROR;
}

Boolean View::IsAccessibilityFocused()
{
    return (mPrivateFlags2 & PFLAG2_ACCESSIBILITY_FOCUSED) != 0;
}

Boolean View::RequestAccessibilityFocus()
{
    AutoPtr<IAccessibilityManager> manger;
    CAccessibilityManager::GetInstance(mContext, (IAccessibilityManager**)&manger);
    Boolean bval1, bval2;
    if ((manger->IsEnabled(&bval1), !bval1)
        || (manger->IsTouchExplorationEnabled(&bval2), !bval2)) {
        return FALSE;
    }

    if ((mViewFlags & VISIBILITY_MASK) != IView::VISIBLE) {
        return FALSE;
    }
    if ((mPrivateFlags2 & PFLAG2_ACCESSIBILITY_FOCUSED) == 0) {
        mPrivateFlags2 |= PFLAG2_ACCESSIBILITY_FOCUSED;
        ViewRootImpl* viewRootImpl = GetViewRootImpl();
        if (viewRootImpl != NULL) {
            viewRootImpl->SetAccessibilityFocus(IVIEW_PROBE(this), NULL);
        }
        Invalidate();
        SendAccessibilityEvent(IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUSED);
        return TRUE;
    }
    return FALSE;
}

ECode View::ClearAccessibilityFocus()
{
    ClearAccessibilityFocusNoCallbacks();
    // Clear the global reference of accessibility focus if this
    // view or any of its descendants had accessibility focus.
    ViewRootImpl* viewRootImpl = GetViewRootImpl();
    if (viewRootImpl != NULL) {
        AutoPtr<IView> focusHost = viewRootImpl->GetAccessibilityFocusedHost();
        if (focusHost != NULL && ViewRootImpl::IsViewDescendantOf(focusHost, IVIEW_PROBE(this))) {
            viewRootImpl->SetAccessibilityFocus(NULL, NULL);
        }
    }

    return NOERROR;
}

void View::SendAccessibilityHoverEvent(
    /* [in] */ Int32 eventType)
{
    // Since we are not delivering to a client accessibility events from not
    // important views (unless the clinet request that) we need to fire the
    // event from the deepest view exposed to the client. As a consequence if
    // the user crosses a not exposed view the client will see enter and exit
    // of the exposed predecessor followed by and enter and exit of that same
    // predecessor when entering and exiting the not exposed descendant. This
    // is fine since the client has a clear idea which view is hovered at the
    // price of a couple more events being sent. This is a simple and
    // working solution.
    View* source = this;
    while (TRUE) {
        if (source->IncludeForAccessibility()) {
            source->SendAccessibilityEvent(eventType);
            return;
        }
        AutoPtr<IViewParent> parent = source->GetParent();
        if (IView::Probe(parent)) {
            source = VIEW_PROBE(parent);
        }
        else {
            return;
        }
    }
}

/**
 * Clears accessibility focus without calling any callback methods
 * normally invoked in {@link #clearAccessibilityFocus()}. This method
 * is used for clearing accessibility focus when giving this focus to
 * another view.
 */
void View::ClearAccessibilityFocusNoCallbacks()
{
    if ((mPrivateFlags2 & PFLAG2_ACCESSIBILITY_FOCUSED) != 0) {
        mPrivateFlags2 &= ~PFLAG2_ACCESSIBILITY_FOCUSED;
        Invalidate();
        SendAccessibilityEvent(IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUS_CLEARED);
    }
}

/**
 * Call this to try to give focus to a specific view or to one of its
 * descendants.
 *
 * A view will not actually take focus if it is not focusable ({@link #isFocusable} returns FALSE),
 * or if it is focusable and it is not focusable in touch mode ({@link #isFocusableInTouchMode})
 * while the device is in touch mode.
 *
 * See also {@link #focusSearch}, which is what you call to say that you
 * have focus, and you want your parent to look for the next one.
 *
 * This is equivalent to calling {@link #requestFocus(int, Rect)} with arguments
 * {@link #FOCUS_DOWN} and <code>NULL</code>.
 *
 * @return Whether this view or one of its descendants actually took focus.
 */
Boolean View::RequestFocus()
{
    return RequestFocus(IView::FOCUS_DOWN);
}

/**
 * Call this to try to give focus to a specific view or to one of its
 * descendants and give it a hint about what direction focus is heading.
 *
 * A view will not actually take focus if it is not focusable ({@link #isFocusable} returns FALSE),
 * or if it is focusable and it is not focusable in touch mode ({@link #isFocusableInTouchMode})
 * while the device is in touch mode.
 *
 * See also {@link #focusSearch}, which is what you call to say that you
 * have focus, and you want your parent to look for the next one.
 *
 * This is equivalent to calling {@link #requestFocus(int, Rect)} with
 * <code>NULL</code> set for the previously focused rectangle.
 *
 * @param direction One of FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, and FOCUS_RIGHT
 * @return Whether this view or one of its descendants actually took focus.
 */
Boolean View::RequestFocus(
    /* [in] */ Int32 direction)
{
    return RequestFocus(direction, NULL);
}

/**
 * Call this to try to give focus to a specific view or to one of its descendants
 * and give it hints about the direction and a specific rectangle that the focus
 * is coming from.  The rectangle can help give larger views a finer grained hint
 * about where focus is coming from, and therefore, where to show selection, or
 * forward focus change internally.
 *
 * A view will not actually take focus if it is not focusable ({@link #isFocusable} returns FALSE),
 * or if it is focusable and it is not focusable in touch mode ({@link #isFocusableInTouchMode})
 * while the device is in touch mode.
 *
 * A View will not take focus if it is not visible.
 *
 * A View will not take focus if one of its parents has {@link android.view.ViewGroup#getDescendantFocusability()}
 * equal to {@link ViewGroup#FOCUS_BLOCK_DESCENDANTS}.
 *
 * See also {@link #focusSearch}, which is what you call to say that you
 * have focus, and you want your parent to look for the next one.
 *
 * You may wish to override this method if your custom {@link View} has an internal
 * {@link View} that it wishes to forward the request to.
 *
 * @param direction One of FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, and FOCUS_RIGHT
 * @param previouslyFocusedRect The rectangle (in this View's coordinate system)
 *        to give a finer grained hint about where focus is coming from.  May be NULL
 *        if there is no hint.
 * @return Whether this view or one of its descendants actually took focus.
 */
Boolean View::RequestFocus(
    /* [in] */ Int32 direction,
    /* [in] */IRect* previouslyFocusedRect)
{
    return RequestFocusNoSearch(direction, previouslyFocusedRect);
}

Boolean View::RequestFocusNoSearch(
    /* [in] */ Int32 direction,
    /* [in] */IRect* previouslyFocusedRect)
{
    // need to be focusable
    if ((mViewFlags & FOCUSABLE_MASK) != FOCUSABLE ||
            (mViewFlags & VISIBILITY_MASK) != IView::VISIBLE) {
        return FALSE;
    }

    // need to be focusable in touch mode if in touch mode
    if (IsInTouchMode() &&
            (FOCUSABLE_IN_TOUCH_MODE != (mViewFlags & FOCUSABLE_IN_TOUCH_MODE))) {
        return FALSE;
    }

    // need to not have any parents blocking us
    if (HasAncestorThatBlocksDescendantFocus()) {
        return FALSE;
    }

    HandleFocusGainInternal(direction, previouslyFocusedRect);

    return TRUE;
}

/**
 * Call this to try to give focus to a specific view or to one of its descendants. This is a
 * special variant of {@link #requestFocus() } that will allow views that are not focuable in
 * touch mode to request focus when they are touched.
 *
 * @return Whether this view or one of its descendants actually took focus.
 *
 * @see #isInTouchMode()
 *
 */
Boolean View::RequestFocusFromTouch()
{
    // Leave touch mode if we need to
    if (IsInTouchMode()) {
        ViewRootImpl* viewRoot = GetViewRootImpl();
        if (viewRoot != NULL) {
            viewRoot->EnsureTouchMode(FALSE);
        }
    }
    return RequestFocus(IView::FOCUS_DOWN);
}

Int32 View::GetImportantForAccessibility()
{
    return (mPrivateFlags2 & PFLAG2_IMPORTANT_FOR_ACCESSIBILITY_MASK)
            >> PFLAG2_IMPORTANT_FOR_ACCESSIBILITY_SHIFT;
}

/**
 * Sets the live region mode for this view. This indicates to accessibility
 * services whether they should automatically notify the user about changes
 * to the view's content description or text, or to the content descriptions
 * or text of the view's children (where applicable).
 * <p>
 * For example, in a login screen with a TextView that displays an "incorrect
 * password" notification, that view should be marked as a live region with
 * mode {@link #ACCESSIBILITY_LIVE_REGION_POLITE}.
 * <p>
 * To disable change notifications for this view, use
 * {@link #ACCESSIBILITY_LIVE_REGION_NONE}. This is the default live region
 * mode for most views.
 * <p>
 * To indicate that the user should be notified of changes, use
 * {@link #ACCESSIBILITY_LIVE_REGION_POLITE}.
 * <p>
 * If the view's changes should interrupt ongoing speech and notify the user
 * immediately, use {@link #ACCESSIBILITY_LIVE_REGION_ASSERTIVE}.
 *
 * @param mode The live region mode for this view, one of:
 *        <ul>
 *        <li>{@link #ACCESSIBILITY_LIVE_REGION_NONE}
 *        <li>{@link #ACCESSIBILITY_LIVE_REGION_POLITE}
 *        <li>{@link #ACCESSIBILITY_LIVE_REGION_ASSERTIVE}
 *        </ul>
 * @attr ref android.R.styleable#View_accessibilityLiveRegion
 */
ECode View::SetAccessibilityLiveRegion(
    /* [in] */ Int32 mode)
{
    Int32 region;

    if (mode != (GetAccessibilityLiveRegion(&region), region)) {
        mPrivateFlags2 &= ~PFLAG2_ACCESSIBILITY_LIVE_REGION_MASK;
        mPrivateFlags2 |= (mode << PFLAG2_ACCESSIBILITY_LIVE_REGION_SHIFT)
                & PFLAG2_ACCESSIBILITY_LIVE_REGION_MASK;
        NotifyViewAccessibilityStateChangedIfNeeded(
                IAccessibilityEvent::CONTENT_CHANGE_TYPE_UNDEFINED);
    }
}

/**
 * Gets the live region mode for this View.
 *
 * @return The live region mode for the view.
 *
 * @attr ref android.R.styleable#View_accessibilityLiveRegion
 *
 * @see #setAccessibilityLiveRegion(int)
 */
ECode View::GetAccessibilityLiveRegion(
    /* [out] */ Int32* res)
{
    VALIDATE_NOT_NULL(res)
    *res = (mPrivateFlags2 & PFLAG2_ACCESSIBILITY_LIVE_REGION_MASK)
            >> PFLAG2_ACCESSIBILITY_LIVE_REGION_SHIFT;
    return NOERROR;
}

ECode View::SetImportantForAccessibility(
    /* [in] */ Int32 mode)
{
    Int32 oldMode;
    GetImportantForAccessibility(&oldMode);
    if (mode != oldMode) {
        // If we're moving between AUTO and another state, we might not need
        // to send a subtree changed notification. We'll store the computed
        // importance, since we'll need to check it later to make sure.
        Boolean maySkipNotify = oldMode == IView::IMPORTANT_FOR_ACCESSIBILITY_AUTO
                || mode == IView::IMPORTANT_FOR_ACCESSIBILITY_AUTO;
        Boolean res;
        IncludeForAccessibility(&res);
        Boolean oldIncludeForAccessibility = maySkipNotify && res;
        mPrivateFlags2 &= ~PFLAG2_IMPORTANT_FOR_ACCESSIBILITY_MASK;
        mPrivateFlags2 |= (mode << PFLAG2_IMPORTANT_FOR_ACCESSIBILITY_SHIFT)
                & PFLAG2_IMPORTANT_FOR_ACCESSIBILITY_MASK;
        if (!maySkipNotify || oldIncludeForAccessibility != res) {
            NotifySubtreeAccessibilityStateChangedIfNeeded();
        } else {
            NotifyViewAccessibilityStateChangedIfNeeded(
                    IAccessibilityEvent::CONTENT_CHANGE_TYPE_UNDEFINED);
        }
    }

    return NOERROR;
}

/**
 * Computes whether this view should be exposed for accessibility. In
 * general, views that are interactive or provide information are exposed
 * while views that serve only as containers are hidden.
 * <p>
 * If an ancestor of this view has importance
 * {@link #IMPORTANT_FOR_ACCESSIBILITY_NO_HIDE_DESCENDANTS}, this method
 * returns <code>false</code>.
 * <p>
 * Otherwise, the value is computed according to the view's
 * {@link #getImportantForAccessibility()} value:
 * <ol>
 * <li>{@link #IMPORTANT_FOR_ACCESSIBILITY_NO} or
 * {@link #IMPORTANT_FOR_ACCESSIBILITY_NO_HIDE_DESCENDANTS}, return <code>false
 * </code>
 * <li>{@link #IMPORTANT_FOR_ACCESSIBILITY_YES}, return <code>true</code>
 * <li>{@link #IMPORTANT_FOR_ACCESSIBILITY_AUTO}, return <code>true</code> if
 * view satisfies any of the following:
 * <ul>
 * <li>Is actionable, e.g. {@link #isClickable()},
 * {@link #isLongClickable()}, or {@link #isFocusable()}
 * <li>Has an {@link AccessibilityDelegate}
 * <li>Has an interaction listener, e.g. {@link OnTouchListener},
 * {@link OnKeyListener}, etc.
 * <li>Is an accessibility live region, e.g.
 * {@link #getAccessibilityLiveRegion()} is not
 * {@link #ACCESSIBILITY_LIVE_REGION_NONE}.
 * </ul>
 * </ol>
 *
 * @return Whether the view is exposed for accessibility.
 * @see #setImportantForAccessibility(int)
 * @see #getImportantForAccessibility()
 */
ECode View::IsImportantForAccessibility(
    /* [out] */ boolean* isImportant)
{
    VALIDATE_NOT_NULL(isImportant)
    const Int32 mode = (mPrivateFlags2 & PFLAG2_IMPORTANT_FOR_ACCESSIBILITY_MASK)
            >> PFLAG2_IMPORTANT_FOR_ACCESSIBILITY_SHIFT;

    if (mode == IView::IMPORTANT_FOR_ACCESSIBILITY_NO
            || mode == IView::IMPORTANT_FOR_ACCESSIBILITY_NO_HIDE_DESCENDANTS) {
        *isImportant = FALSE;
        return NOERROR;
    }

    // Check parent mode to ensure we're not hidden.
    AutoPtr<IViewParent> parent = mParent;
    while (IView::Probe(parent)) {
        Int32 res;
        IView::Probe(parent)->GetImportantForAccessibility(res);
        if (res == IView::IMPORTANT_FOR_ACCESSIBILITY_NO_HIDE_DESCENDANTS) {
            *isImportant = FALSE;
            return NOERROR;
        }
        IView::Probe(parent)->GetParent((IViewParent**)&parent);
    }
    Boolean isActionable;
    IsActionableForAccessibility(&isActionable);
    AutoPtr<IAccessibilityNodeProvider> provider;
    GetAccessibilityNodeProvider((IAccessibilityNodeProvider**)&provider);
    Int32 region;
    GetAccessibilityLiveRegion(&region)
    *res = mode == IView::IMPORTANT_FOR_ACCESSIBILITY_YES || isActionable
            || HasListenersForAccessibility() || provider != NULL
            || region != IView::ACCESSIBILITY_LIVE_REGION_NONE;
    return NOERROR;
}

AutoPtr<IViewParent> View::GetParentForAccessibility()
{
    if (IView::Probe(mParent) != NULL) {
        View* parentView = VIEW_PROBE(mParent);
        if (parentView->IncludeForAccessibility()) {
            return mParent;
        }
        else {
            AutoPtr<IViewParent> parent;
            mParent->GetParentForAccessibility((IViewParent**)&parent);
            return parent;
        }
    }
    return NULL;
}

ECode View::AddChildrenForAccessibility(
    /* [in] */ IObjectContainer* children) {

    return NOERROR;
}

ECode View::IncludeForAccessibility(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    if (mAttachInfo != NULL) {
        Boolean isImportant;
        IsImportantForAccessibility(&isImportant);
        *res = (mAttachInfo->mAccessibilityFetchFlags
            & IAccessibilityNodeInfo::FLAG_INCLUDE_NOT_IMPORTANT_VIEWS) != 0 || isImportant;
        return NOERROR;
    }
    *res = FALSE;
    return NOERROR;
}

Boolean View::IsActionableForAccessibility()
{
    return (IsClickable() || IsLongClickable() || IsFocusable());
}

Boolean View::HasListenersForAccessibility()
{
    AutoPtr<ListenerInfo> info = GetListenerInfo();
    return mTouchDelegate != NULL || info->mOnKeyListener != NULL
            || info->mOnTouchListener != NULL || info->mOnGenericMotionListener != NULL
            || info->mOnHoverListener != NULL || info->mOnDragListener != NULL;
}

/**
 * Notifies that the accessibility state of this view changed. The change
 * is local to this view and does not represent structural changes such
 * as children and parent. For example, the view became focusable. The
 * notification is at at most once every
 * {@link ViewConfiguration#getSendRecurringAccessibilityEventsInterval()}
 * to avoid unnecessary load to the system. Also once a view has a pending
 * notification this method is a NOP until the notification has been sent.
 *
 * @hide
 */
ECode View::NotifyViewAccessibilityStateChangedIfNeeded(
    /* [in] */ Int32 changeType)
{
    AutoPtr<IAccessibilityManager> manger;
    AccessibilityManager::GetInstance(mContext, (IAccessibilityManager**)&manager);
    Boolean isEnabled;
    manager->IsEnabled(&isEnabled);
    if (!isEnabled) {
        return NOERROR;
    }
    if (mSendViewStateChangedAccessibilityEvent == NULL) {
        mSendViewStateChangedAccessibilityEvent =
                new SendViewStateChangedAccessibilityEvent();
    }
    mSendViewStateChangedAccessibilityEvent->RunOrPost(changeType);
}

/**
 * Notifies that the accessibility state of this view changed. The change
 * is *not* local to this view and does represent structural changes such
 * as children and parent. For example, the view size changed. The
 * notification is at at most once every
 * {@link ViewConfiguration#getSendRecurringAccessibilityEventsInterval()}
 * to avoid unnecessary load to the system. Also once a view has a pending
 * notification this method is a NOP until the notification has been sent.
 *
 * @hide
 */
ECode View::NotifySubtreeAccessibilityStateChangedIfNeeded()
{
    AutoPtr<IAccessibilityManager> manger;
    CAccessibilityManager::GetInstance(mContext, (IAccessibilityManager**)&manger);
    Boolean bval;
    manger->IsEnabled(&bval);
    if (!bval) {
        return NOERROR;
    }

    if ((mPrivateFlags2 & PFLAG2_SUBTREE_ACCESSIBILITY_STATE_CHANGED) == 0) {
        mPrivateFlags2 |= PFLAG2_SUBTREE_ACCESSIBILITY_STATE_CHANGED;
        //try {
            ECode ec = mParent->NotifySubtreeAccessibilityStateChanged(
                    THIS_PROBE(IView), THIS_PROBE(IView), IAccessibilityEvent::CONTENT_CHANGE_TYPE_SUBTREE);
        //} catch (AbstractMethodError e) {
        //    Log.e(VIEW_LOG_TAG, mParent.getClass().getSimpleName() +
        //            " does not fully implement ViewParent", e);
        //}
        return ec;
    }

    return NOERROR;
}

ECode View::ResetSubtreeAccessibilityStateChanged()
{
    mPrivateFlags2 &= ~PFLAG2_SUBTREE_ACCESSIBILITY_STATE_CHANGED;
    return NOERROR;
}

Boolean View::PerformAccessibilityAction(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments)
{
    if (mAccessibilityDelegate != NULL) {
        Boolean res;
        mAccessibilityDelegate->PerformAccessibilityAction(
            IVIEW_PROBE(this), action, arguments, &res);
        return res;
    }
    else {
      return PerformAccessibilityActionInternal(action, arguments);
    }
}

Boolean View::PerformAccessibilityActionInternal(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments)
{
    switch (action) {
        case IAccessibilityNodeInfo::ACTION_CLICK: {
            if (IsClickable()) {
                PerformClick();
                return TRUE;
            }
        } break;
        case IAccessibilityNodeInfo::ACTION_LONG_CLICK: {
            if (IsLongClickable()) {
                PerformLongClick();
                return TRUE;
            }
        } break;
        case IAccessibilityNodeInfo::ACTION_FOCUS: {
            if (!HasFocus()) {
                // Get out of touch mode since accessibility
                // wants to move focus around.
                GetViewRootImpl()->EnsureTouchMode(FALSE);
                return RequestFocus();
            }
        } break;
        case IAccessibilityNodeInfo::ACTION_CLEAR_FOCUS: {
            if (HasFocus()) {
                ClearFocus();
                return !IsFocused();
            }
        } break;
        case IAccessibilityNodeInfo::ACTION_SELECT: {
            if (!IsSelected()) {
                SetSelected(TRUE);
                return IsSelected();
            }
        } break;
        case IAccessibilityNodeInfo::ACTION_CLEAR_SELECTION: {
            if (IsSelected()) {
                SetSelected(FALSE);
                return !IsSelected();
            }
        } break;
        case IAccessibilityNodeInfo::ACTION_ACCESSIBILITY_FOCUS: {
            if (!IsAccessibilityFocused()) {
                return RequestAccessibilityFocus();
            }
        } break;
        case IAccessibilityNodeInfo::ACTION_CLEAR_ACCESSIBILITY_FOCUS: {
            if (IsAccessibilityFocused()) {
                ClearAccessibilityFocus();
                return TRUE;
            }
        } break;
        case IAccessibilityNodeInfo::ACTION_NEXT_AT_MOVEMENT_GRANULARITY: {
            if (arguments != NULL) {
                Int32 granularity = 0;
                arguments->GetInt32(
                        IAccessibilityNodeInfo::ACTION_ARGUMENT_MOVEMENT_GRANULARITY_INT, &granularity);
                Boolean extendSelection;
                (IBaseBundle::Probe(arguments))->GetBoolean(
                        IAccessibilityNodeInfo::ACTION_ARGUMENT_EXTEND_SELECTION_BOOLEAN, &extendSelection);
                return TraverseAtGranularity(granularity, FALSE, extendSelection);
            }
        } break;
        case IAccessibilityNodeInfo::ACTION_PREVIOUS_AT_MOVEMENT_GRANULARITY: {
            if (arguments != NULL) {
                Int32 granularity = 0;
                arguments->GetInt32(
                        IAccessibilityNodeInfo::ACTION_ARGUMENT_MOVEMENT_GRANULARITY_INT, &granularity);
                Boolean extendSelection;
                (IBaseBundle::Probe(arguments))->GetBoolean(
                        IAccessibilityNodeInfo::ACTION_ARGUMENT_EXTEND_SELECTION_BOOLEAN, &extendSelection);
                return TraverseAtGranularity(granularity, FALSE, extendSelection);
            }
        } break;
        case IAccessibilityNodeInfo::ACTION_SET_SELECTION: {
            AutoPtr<ICharSequence> text;
            GetIterableTextForAccessibility((ICharSequence**)&text);
            if (!text) {
                return FALSE;
            }
            Int32 start = -1, end= -1;
            if (arguments) {
                (IBaseBundle::Probe(arguments))->GetInt(
                        IAccessibilityNodeInfo::ACTION_ARGUMENT_SELECTION_START_INT, -1, &start);
                (IBaseBundle::Probe(arguments))->GetInt(
                        IAccessibilityNodeInfo::ACTION_ARGUMENT_SELECTION_END_INT, -1, &end);
            }
            // Only cursor position can be specified (selection length == 0)
            Int32 tempStart, tempEnd;
            GetAccessibilitySelectionStart(&tempStart);
            GetAccessibilitySelectionEnd(&tempEnd);
            if ((tempStart != start || tempEnd != end) && (start == end)) {
                SetAccessibilitySelection(start, end);
                NotifyViewAccessibilityStateChangedIfNeeded(
                        IAccessibilityEvent::CONTENT_CHANGE_TYPE_UNDEFINED);
                return true;
            }
        } break;
    }
    return FALSE;
}

Boolean View::TraverseAtGranularity(
    /* [in] */ Int32 granularity,
    /* [in] */ Boolean forward,
    /* [in] */ Boolean extendSelection)
{
    AutoPtr<ICharSequence> text = GetIterableTextForAccessibility();
    Int32 len;
    if (text == NULL || (text->GetLength(&len), len) == 0) {
        return FALSE;
    }

    AutoPtr<ITextSegmentIterator> iterator = GetIteratorForGranularity(granularity);
    if (iterator == NULL) {
        return FALSE;
    }
    Int32 current;
    GetAccessibilitySelectionEnd(&current);
    if (current == IView::ACCESSIBILITY_CURSOR_POSITION_UNDEFINED) {
        if (forward) {
            current = 0;
        }
        text->GetLength(&current);
    }
    AutoPtr< ArrayOf<Int32> > range;
    if (forward) {
        iterator->Following(current, (ArrayOf<Int32>**)&range);
    } else {
        iterator->Preceding(current, (ArrayOf<Int32>**)&range);
    }
    if (!range) {
        return FALSE;
    }
    Int32 segmentStart = (*range)[0];
    Int32 segmentEnd = (*range)[1];
    Int32 selectionStart, selectionEnd;
    Boolean extendable;
    IsAccessibilitySelectionExtendable(&extendable);
    if (extendSelection && extendable) {
        GetAccessibilitySelectionStart(&selectionStart);
        if (selectionStart == IView::ACCESSIBILITY_CURSOR_POSITION_UNDEFINED) {
            selectionStart = forward ? segmentStart : segmentEnd;
        }
        selectionEnd = forward ? segmentEnd : segmentStart;
    } else {
        selectionStart = selectionEnd = forward ? segmentEnd : segmentStart;
    }
    SetAccessibilitySelection(selectionStart, selectionEnd);
    Int32 action = forward ? IAccessibilityNodeInfo::ACTION_NEXT_AT_MOVEMENT_GRANULARITY
            : IAccessibilityNodeInfo::ACTION_PREVIOUS_AT_MOVEMENT_GRANULARITY;
    SendViewTextTraversedAtGranularityEvent(action, granularity, segmentStart, segmentEnd);
    return TRUE;
}

Boolean View::PreviousAtGranularity(
    /* [in] */ Int32 granularity)
{
    AutoPtr<ICharSequence> text = GetIterableTextForAccessibility();
    Int32 len;
    if (text == NULL || (text->GetLength(&len), len) == 0) {
        return FALSE;
    }

    AutoPtr<ITextSegmentIterator> iterator = GetIteratorForGranularity(granularity);
    if (iterator == NULL) {
        return FALSE;
    }

    Int32 current = GetAccessibilityCursorPosition();
    if (current == IView::ACCESSIBILITY_CURSOR_POSITION_UNDEFINED) {
        text->GetLength(&current);
        SetAccessibilityCursorPosition(current);
    }
    else if (granularity == IAccessibilityNodeInfo::MOVEMENT_GRANULARITY_CHARACTER) {
        // When traversing by character we always put the cursor after the character
        // to ease edit and have to compensate before asking the for previous segment.
        current--;
        SetAccessibilityCursorPosition(current);
    }

    AutoPtr<ArrayOf<Int32> > range;
    iterator->Preceding(current, (ArrayOf<Int32>**)&range);
    if (range == NULL) {
        return FALSE;
    }

    const Int32 start = (*range)[0];
    const Int32 end = (*range)[1];
    // Always put the cursor after the character to ease edit.
    if (granularity == IAccessibilityNodeInfo::MOVEMENT_GRANULARITY_CHARACTER) {
        SetAccessibilityCursorPosition(end);
    }
    else {
        SetAccessibilityCursorPosition(start);
    }

    SendViewTextTraversedAtGranularityEvent(
            IAccessibilityNodeInfo::ACTION_PREVIOUS_AT_MOVEMENT_GRANULARITY,
            granularity, start, end);
    return TRUE;
}

AutoPtr<ICharSequence> View::GetIterableTextForAccessibility()
{
    return GetContentDescription();
}

/**
 * Gets whether accessibility selection can be extended.
 *
 * @return If selection is extensible.
 *
 * @hide
 */
ECode View::IsAccessibilitySelectionExtendable(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = FALSE
    return NOERROR;
}

Int32 View::GetAccessibilitySelectionStart()
{
    return mAccessibilityCursorPosition;
}

/**
 * @hide
 */
ECode View::GetAccessibilitySelectionEnd(
    /* [out] */ Int32* res)
{
    VALIDATE_NOT_NULL(res)
    *res = GetAccessibilitySelectionStart();
    return NOERROR;
}

ECode View::SetAccessibilitySelection(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (start ==  end && end == mAccessibilityCursorPosition) {
        return NOERROR;
    }
    AutoPtr<ICharSequence> csq;
    GetIterableTextForAccessibility((ICharSequence**)&csq);
    Int32 len;
    csq->GetLength(&len);
    if (start >= 0 && start == end && end <= len) {
        mAccessibilityCursorPosition = start;
    } else {
        mAccessibilityCursorPosition = IView::ACCESSIBILITY_CURSOR_POSITION_UNDEFINED;
    }
    SendAccessibilityEvent(IAccessibilityEvent::TYPE_VIEW_TEXT_SELECTION_CHANGED);
    return NOERROR;
}

void View::SendViewTextTraversedAtGranularityEvent(
    /* [in] */ Int32 action,
    /* [in] */ Int32 granularity,
    /* [in] */ Int32 fromIndex,
    /* [in] */ Int32 toIndex)
{
    if (mParent == NULL) {
        return;
    }

    AutoPtr<IAccessibilityEvent> event;
    CAccessibilityEvent::Obtain(
        IAccessibilityEvent::TYPE_VIEW_TEXT_TRAVERSED_AT_MOVEMENT_GRANULARITY,
        (IAccessibilityEvent**)&event);
    OnInitializeAccessibilityEvent(event);
    OnPopulateAccessibilityEvent(event);
    event->SetFromIndex(fromIndex);
    event->SetToIndex(toIndex);
    event->SetAction(action);
    event->SetMovementGranularity(granularity);
    Boolean result;
    mParent->RequestSendAccessibilityEvent(IVIEW_PROBE(this), event, &result);
}

AutoPtr<ITextSegmentIterator> View::GetIteratorForGranularity(
    /* [in] */ Int32 granularity)
{
    Int32 len;
    switch (granularity) {
        case IAccessibilityNodeInfo::MOVEMENT_GRANULARITY_CHARACTER: {
            AutoPtr<ICharSequence> text = GetIterableTextForAccessibility();
            if (text != NULL && (text->GetLength(&len), len) > 0) {
                AutoPtr<IResources> res;
                mContext->GetResources((IResources**)&res);
                assert(res != NULL);

                AutoPtr<IConfiguration> config;
                res->GetConfiguration((IConfiguration**)&config);
                assert(config != NULL);

                AutoPtr<ILocale> locale;
                config->GetLocale((ILocale**)&locale);
                // AutoPtr<ICharacterTextSegmentIterator> iterator =
                //     CharacterTextSegmentIterator::GetInstance(locale);

                // String str;
                // text->ToString(&str);
                // iterator->Initialize(str);
                // return iterator;
            }
        } break;
        case IAccessibilityNodeInfo::MOVEMENT_GRANULARITY_WORD: {
            AutoPtr<ICharSequence> text = GetIterableTextForAccessibility();
            if (text != NULL && (text->GetLength(&len), len) > 0) {
                AutoPtr<IResources> res;
                mContext->GetResources((IResources**)&res);
                assert(res != NULL);

                AutoPtr<IConfiguration> config;
                res->GetConfiguration((IConfiguration**)&config);
                assert(config != NULL);

                AutoPtr<ILocale> locale;
                config->GetLocale((ILocale**)&locale);

                // AutoPtr<IWordTextSegmentIterator> iterator =
                //     WordTextSegmentIterator::GetInstance(locale);

                // String str;
                // text->ToString(&str);
                // iterator->Initialize(str);
                // return iterator;
            }
        } break;
        case IAccessibilityNodeInfo::MOVEMENT_GRANULARITY_PARAGRAPH: {
            AutoPtr<ICharSequence> text = GetIterableTextForAccessibility();
            if (text != NULL && (text->GetLength(&len), len) > 0) {
                // AutoPtr<IParagraphTextSegmentIterator> iterator =
                //     ParagraphTextSegmentIterator::GetInstance();

                // String str;
                // text->ToString(&str);
                // iterator->Initialize(str);
                // return iterator;
            }
        } break;
    }
    return NULL;
}

/**
 * @return Whether any ancestor of this view blocks descendant focus.
 */
Boolean View::HasAncestorThatBlocksDescendantFocus()
{
    Boolean focusableInTouchMode;
    IsFocusableInTouchMode(&focusableInTouchMode);
    AutoPtr<IViewParent> ancestor = mParent;
    while (ancestor != NULL && ancestor->Probe(EIID_IViewGroup) != NULL) {
        AutoPtr<IViewGroup> vgAncestor = (IViewGroup*)ancestor->Probe(EIID_IViewGroup);
        Int32 focusability;
        vgAncestor->GetDescendantFocusability(&focusability);
        Boolean res;
        vgAncestor->ShouldBlockFocusForTouchscreen(&res);
        if (focusability == IViewGroup::FOCUS_BLOCK_DESCENDANTS
            || (!focusableInTouchMode && res)) {
            return TRUE;
        }
        else {
            ancestor = NULL;
            vgAncestor->GetParent((IViewParent**)&ancestor);
        }
    }
    return FALSE;
}

/**
 * @hide
 */
ECode View::DispatchStartTemporaryDetach()
{
    return OnStartTemporaryDetach();
}

/**
 * This is called when a container is going to temporarily detach a child, with
 * {@link ViewGroup#detachViewFromParent(View) ViewGroup.detachViewFromParent}.
 * It will either be followed by {@link #onFinishTemporaryDetach()} or
 * {@link #onDetachedFromWindow()} when the container is done.
 */
ECode View::OnStartTemporaryDetach()
{
    RemoveUnsetPressCallback();
    mPrivateFlags |= PFLAG_CANCEL_NEXT_UP_EVENT;

    return NOERROR;
}

/**
 * @hide
 */
ECode View::DispatchFinishTemporaryDetach()
{
    return OnFinishTemporaryDetach();
}

/**
 * Called after {@link #onStartTemporaryDetach} when the container is done
 * changing the view.
 */
ECode View::OnFinishTemporaryDetach()
{
    return NOERROR;
}

/**
 * capture information of this view for later analysis: developement only
 * check dynamic switch to make sure we only dump view
 * when ViewDebug.SYSTEM_PROPERTY_CAPTURE_VIEW) is set
 */
void View::CaptureViewInfo(
    /* [in] */ const char* subTag,
    /* [in] */ IView* v)
{
    //if (v == NULL || SystemProperties.getInt(ViewDebug.SYSTEM_PROPERTY_CAPTURE_VIEW, 0) == 0) {
     //   return;
    //}
    //ViewDebug.dumpCapturedView(subTag, v);
}

/**
 * Return the global {@link KeyEvent.DispatcherState KeyEvent.DispatcherState}
 * for this view's window.  Returns NULL if the view is not currently attached
 * to the window.  Normally you will not need to use this directly, but
 * just use the standard high-level event callbacks like
 * {@link #onKeyDown(int, KeyEvent)}.
 */
ECode View::GetKeyDispatcherState(
    /* [out] */ IDispatcherState** state)
{
    VALIDATE_NOT_NULL(state);
    *state = mAttachInfo != NULL ? mAttachInfo->mKeyDispatchState : NULL;
    REFCOUNT_ADD(*state);

    return NOERROR;
}

/**
 * Dispatch a key event before it is processed by any input method
 * associated with the view hierarchy.  This can be used to intercept
 * key events in special situations before the IME consumes them; a
 * typical example would be handling the BACK key to update the application's
 * UI instead of allowing the IME to see it and close itself.
 *
 * @param event The key event to be dispatched.
 * @return True if the event was handled, FALSE otherwise.
 */
Boolean View::DispatchKeyEventPreIme(
    /* [in] */ IKeyEvent* event)
{
    Int32 keyCode;
    event->GetKeyCode(&keyCode);
    return OnKeyPreIme(keyCode, event);
}

/**
 * Dispatch a key event to the next view on the focus path. This path runs
 * from the top of the view tree down to the currently focused view. If this
 * view has focus, it will dispatch to itself. Otherwise it will dispatch
 * the next node down the focus path. This method also fires any key
 * listeners.
 *
 * @param event The key event to be dispatched.
 * @return True if the event was handled, FALSE otherwise.
 */
Boolean View::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnKeyEvent(event, 0);
    }

    // Give any attached key listener a first crack at the event.
    //noinspection SimplifiableIfStatement
    AutoPtr<ListenerInfo> li = mListenerInfo;
    assert(event != NULL);
    Int32 keyCode = 0;
    event->GetKeyCode(&keyCode);
    Boolean res;
    if (li != NULL && li->mOnKeyListener != NULL && (mViewFlags & ENABLED_MASK) == ENABLED
        && (li->mOnKeyListener->OnKey(IVIEW_PROBE(this), keyCode, event, &res), res)) {
        return TRUE;
    }

    Boolean result = FALSE;
    event->Dispatch(
        THIS_PROBE(IKeyEventCallback),
        mAttachInfo != NULL ? mAttachInfo->mKeyDispatchState : NULL,
        THIS_PROBE(IInterface), &result);
    if (result) {
        return TRUE;
    }

    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnUnhandledEvent(event, 0);
    }
    return FALSE;
}

/**
 * Dispatches a key shortcut event.
 *
 * @param event The key event to be dispatched.
 * @return True if the event was handled by the view, FALSE otherwise.
 */
Boolean View::DispatchKeyShortcutEvent(
    /* [in] */ IKeyEvent* event)
{
    Int32 keyCode;
    event->GetKeyCode(&keyCode);
    return OnKeyShortcut(keyCode, event);
}

/**
 * Pass the touch screen motion event down to the target view, or this
 * view if it is the target.
 *
 * @param event The motion event to be dispatched.
 * @return True if the event was handled by the view, FALSE otherwise.
 */
Boolean View::DispatchTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    Boolean result = FALSE;
    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnTouchEvent(event, 0);
    }

    Int32 actionMasked;
    event->GetActionMasked(&actionMasked);
    if (actionMasked == IMotionEvent::ACTION_DOWN) {
        // Defensive cleanup for new gesture
        StopNestedScroll();
    }

    if (OnFilterTouchEventForSecurity(event)) {
        //noinspection SimplifiableIfStatement
        AutoPtr<ListenerInfo> li = mListenerInfo;
        Boolean result = FALSE;
        if (li != NULL && li->mOnTouchListener != NULL && (mViewFlags & ENABLED_MASK) == ENABLED
            && (li->mOnTouchListener->OnTouch(IVIEW_PROBE(this), event, &result), result)) {
            result = TRUE;
        }

        if (!result && OnTouchEvent(event)) {
            result = TRUE;
        }
    }

    if (!result && mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnUnhandledEvent(event, 0);
    }

    // Clean up after nested scrolls if this is the end of a gesture;
    // also cancel it if we tried an ACTION_DOWN but we didn't want the rest
    // of the gesture.
    if (actionMasked == IMotionEvent::ACTION_UP ||
            actionMasked == IMotionEvent::ACTION_CANCEL ||
            (actionMasked == IMotionEvent::ACTION_DOWN && !result)) {
        StopNestedScroll();
    }

    return result;
}

/**
 * Filter the touch event to apply security policies.
 *
 * @param event The motion event to be filtered.
 * @return True if the event should be dispatched, FALSE if the event should be dropped.
 *
 * @see #getFilterTouchesWhenObscured
 */
Boolean View::OnFilterTouchEventForSecurity(
    /* [in] */ IMotionEvent* event)
{
    //noinspection RedundantIfStatement
    Int32 flag;
    event->GetFlags(&flag);
    if ((mViewFlags & FILTER_TOUCHES_WHEN_OBSCURED) != 0
        && (flag & IMotionEvent::FLAG_WINDOW_IS_OBSCURED) != 0) {
        // Window is obscured, drop this touch.
        return FALSE;
    }
    return TRUE;
}

/**
 * Pass a trackball motion event down to the focused view.
 *
 * @param event The motion event to be dispatched.
 * @return True if the event was handled by the view, FALSE otherwise.
 */
Boolean View::DispatchTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnTrackballEvent(event, 0);
    }
    return OnTrackballEvent(event);
}

Boolean View::DispatchGenericMotionEvent(
    /* [in] */ IMotionEvent* event)
{
    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnGenericMotionEvent(event, 0);
    }

    Int32 source = 0;
    event->GetSource(&source);
    if ((source & IInputDevice::SOURCE_CLASS_POINTER) != 0) {
        Int32 action = 0;
        event->GetAction(&action);
        if (action == IMotionEvent::ACTION_HOVER_ENTER
                || action == IMotionEvent::ACTION_HOVER_MOVE
                || action == IMotionEvent::ACTION_HOVER_EXIT) {
            if (DispatchHoverEvent(event)) {
                return TRUE;
            }
        }
        else if (DispatchGenericPointerEvent(event)) {
            return TRUE;
        }
    }
    else if (DispatchGenericFocusedEvent(event)) {
        return TRUE;
    }

    if (DispatchGenericMotionEventInternal(event)) {
        return TRUE;
    }

    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnUnhandledEvent(event, 0);
    }

    return FALSE;
}

Boolean View::DispatchGenericMotionEventInternal(
    /* [in] */ IMotionEvent* event)
{
    //noinspection SimplifiableIfStatement
    AutoPtr<ListenerInfo> li = mListenerInfo;
    Boolean res = FALSE;
    if (li != NULL && li->mOnGenericMotionListener != NULL
            && (mViewFlags & ENABLED_MASK) == ENABLED
            && (li->mOnGenericMotionListener->OnGenericMotion(IVIEW_PROBE(this), event, &res), res)) {
        return TRUE;
    }

    if (OnGenericMotionEvent(event)) {
        return TRUE;
    }

    if (mInputEventConsistencyVerifier != NULL) {
        mInputEventConsistencyVerifier->OnUnhandledEvent(event, 0);
    }
    return FALSE;
}

Boolean View::DispatchHoverEvent(
    /* [in] */ IMotionEvent* event)
{
    //noinspection SimplifiableIfStatement
    AutoPtr<ListenerInfo> li = mListenerInfo;
    Boolean res = FALSE;
    if (li != NULL && li->mOnHoverListener != NULL
            && (mViewFlags & ENABLED_MASK) == ENABLED
            && (li->mOnHoverListener->OnHover(IVIEW_PROBE(this), event, &res), res)) {
        return TRUE;
    }

    return OnHoverEvent(event);
}

Boolean View::HasHoveredChild()
{
    return FALSE;
}

Boolean View::DispatchGenericPointerEvent(
    /* [in] */ IMotionEvent* event)
{
    return FALSE;
}

Boolean View::DispatchGenericFocusedEvent(
    /* [in] */ IMotionEvent* event)
{
    return FALSE;
}

Boolean View::DispatchPointerEvent(
    /* [in] */ IMotionEvent* event)
{
    Boolean res = FALSE;
    if ((event->IsTouchEvent(&res), res)) {
        return DispatchTouchEvent(event);
    }
    else {
        return DispatchGenericMotionEvent(event);
    }
}

/**
 * Called when the window containing this view gains or loses window focus.
 * ViewGroups should override to route to their children.
 *
 * @param hasFocus True if the window containing this view now has focus,
 *        FALSE otherwise.
 */
ECode View::DispatchWindowFocusChanged(
    /* [in] */ Boolean hasFocus)
{
    return OnWindowFocusChanged(hasFocus);
}

/**
 * Called when the window containing this view gains or loses focus.  Note
 * that this is separate from view focus: to receive key events, both
 * your view and its window must have focus.  If a window is displayed
 * on top of yours that takes input focus, then your own window will lose
 * focus but the view focus will remain unchanged.
 *
 * @param hasWindowFocus True if the window containing this view now has
 *        focus, FALSE otherwise.
 */
ECode View::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
    if (!hasWindowFocus) {
        if (IsPressed()) {
            SetPressed(FALSE);
        }

        if (imm != NULL && (mPrivateFlags & PFLAG_FOCUSED) != 0) {
            imm->FocusOut(IVIEW_PROBE(this));
        }

        RemoveLongPressCallback();
        RemoveTapCallback();
        OnFocusLost();
    }
    else if (imm != NULL && (mPrivateFlags & PFLAG_FOCUSED) != 0) {
       imm->FocusIn(IVIEW_PROBE(this));
    }

    RefreshDrawableState();

    return NOERROR;
}

/**
 * Returns TRUE if this view is in a window that currently has window focus.
 * Note that this is not the same as the view itself having focus.
 *
 * @return True if this view is in a window that currently has window focus.
 */
Boolean View::HasWindowFocus()
{
    return mAttachInfo != NULL && mAttachInfo->mHasWindowFocus;
}

/**
 * Dispatch a view visibility change down the view hierarchy.
 * ViewGroups should override to route to their children.
 * @param changedView The view whose visibility changed. Could be 'this' or
 * an ancestor view.
 * @param visibility The new visibility of changedView: {@link #VISIBLE},
 * {@link #INVISIBLE} or {@link #GONE}.
 */
void View::DispatchVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 visibility)
{
    OnVisibilityChanged(changedView, visibility);
}

/**
 * Called when the visibility of the view or an ancestor of the view is changed.
 * @param changedView The view whose visibility changed. Could be 'this' or
 * an ancestor view.
 * @param visibility The new visibility of changedView: {@link #VISIBLE},
 * {@link #INVISIBLE} or {@link #GONE}.
 */
void View::OnVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 visibility)
{
    if (visibility == IView::VISIBLE) {
        if (mAttachInfo != NULL) {
            InitialAwakenScrollBars();
        }
        else {
            mPrivateFlags |= PFLAG_AWAKEN_SCROLL_BARS_ON_ATTACH;
        }
    }
}

/**
 * Dispatch a hint about whether this view is displayed. For instance, when
 * a View moves out of the screen, it might receives a display hint indicating
 * the view is not displayed. Applications should not <em>rely</em> on this hint
 * as there is no guarantee that they will receive one.
 *
 * @param hint A hint about whether or not this view is displayed:
 * {@link #VISIBLE} or {@link #INVISIBLE}.
 */
ECode View::DispatchDisplayHint(
    /* [in] */ Int32 hint)
{
    OnDisplayHint(hint);

    return NOERROR;
}

/**
 * Gives this view a hint about whether is displayed or not. For instance, when
 * a View moves out of the screen, it might receives a display hint indicating
 * the view is not displayed. Applications should not <em>rely</em> on this hint
 * as there is no guarantee that they will receive one.
 *
 * @param hint A hint about whether or not this view is displayed:
 * {@link #VISIBLE} or {@link #INVISIBLE}.
 */
void View::OnDisplayHint(
    /* [in] */ Int32 hint)
{
}

/**
 * Dispatch a window visibility change down the view hierarchy.
 * ViewGroups should override to route to their children.
 *
 * @param visibility The new visibility of the window.
 *
 * @see #onWindowVisibilityChanged
 */
ECode View::DispatchWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    OnWindowVisibilityChanged(visibility);

    return NOERROR;
}

/**
 * Called when the window containing has change its visibility
 * (between {@link #GONE}, {@link #INVISIBLE}, and {@link #VISIBLE}).  Note
 * that this tells you whether or not your window is being made visible
 * to the window manager; this does <em>not</em> tell you whether or not
 * your window is obscured by other windows on the screen, even if it
 * is itself visible.
 *
 * @param visibility The new visibility of the window.
 */
void View::OnWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    if (visibility == IView::VISIBLE) {
        InitialAwakenScrollBars();
    }
}

/**
 * Returns the current visibility of the window this view is attached to
 * (either {@link #GONE}, {@link #INVISIBLE}, or {@link #VISIBLE}).
 *
 * @return Returns the current visibility of the view's window.
 */
Int32 View::GetWindowVisibility()
{
    return mAttachInfo != NULL ? mAttachInfo->mWindowVisibility : IView::GONE;
}

/**
 * Retrieve the overall visible display size in which the window this view is
 * attached to has been positioned in.  This takes into account screen
 * decorations above the window, for both cases where the window itself
 * is being position inside of them or the window is being placed under
 * then and covered insets are used for the window to position its content
 * inside.  In effect, this tells you the available area where content can
 * be placed and remain visible to users.
 *
 * <p>This function requires an IPC back to the window manager to retrieve
 * the requested information, so should not be used in performance critical
 * code like drawing.
 *
 * @param outRect Filled in with the visible display frame.  If the view
 * is not attached to a window, this is simply the raw display size.
 */
ECode View::GetWindowVisibleDisplayFrame(
    /* [in, out] */ IRect* outRect)
{
    assert(outRect != NULL);

    if (mAttachInfo != NULL)
    {
        AutoPtr<IRect> r;
        FAIL_RETURN(mAttachInfo->mSession->GetDisplayFrame(
                mAttachInfo->mWindow.Get(), (IRect**)&r));
        // XXX This is really broken, and probably all needs to be done
        // in the window manager, and we need to know more about whether
        // we want the area behind or in front of the IME.
        CRect* insets = mAttachInfo->mVisibleInsets.Get();
        CRect* _r = (CRect*)r.Get();
        CRect* _outRect = (CRect*)outRect;
        _outRect->mLeft = _r->mLeft + insets->mLeft;
        _outRect->mTop = _r->mTop + insets->mTop;
        _outRect->mRight = _r->mRight - insets->mRight;
        _outRect->mBottom = _r->mBottom - insets->mBottom;
        return NOERROR;
    }
    AutoPtr<IDisplay> d;
    // The view is not attached to a display so we don't have a context.
    // Make a best guess about the display size.
    DisplayManagerGlobal::GetInstance()->GetRealDisplay(IDisplay::DEFAULT_DISPLAY, (IDisplay**)&d);
    assert(d != NULL);
    d->GetRectSize(outRect);

    return NOERROR;
}

ECode View::DispatchConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    OnConfigurationChanged(newConfig);

    return NOERROR;
}

/**
 * Called when the current configuration of the resources being used
 * by the application have changed.  You can use this to decide when
 * to reload resources that can changed based on orientation and other
 * configuration characterstics.  You only need to use this if you are
 * not relying on the normal {@link android.app.Activity} mechanism of
 * recreating the activity instance upon a configuration change.
 *
 * @param newConfig The new resource configuration.
 */
void View::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
}

/**
 * Private function to aggregate all per-view attributes in to the view
 * root.
 */
ECode View::DispatchCollectViewAttributes(
    /* [in] */ AttachInfo* attachInfo,
    /* [in] */ Int32 visibility)
{
    return PerformCollectViewAttributes(attachInfo, visibility);
}

ECode View::PerformCollectViewAttributes(
    /* [in] */ AttachInfo* attachInfo,
    /* [in] */ Int32 visibility)
{
    if ((visibility & VISIBILITY_MASK) == IView::VISIBLE) {
        if ((mViewFlags & IView::KEEP_SCREEN_ON) == IView::KEEP_SCREEN_ON) {
            attachInfo->mKeepScreenOn = TRUE;
        }
        attachInfo->mSystemUiVisibility |= mSystemUiVisibility;
        AutoPtr<ListenerInfo> li = mListenerInfo;
        if (li != NULL && li->mOnSystemUiVisibilityChangeListener != NULL) {
            attachInfo->mHasSystemUiListeners = TRUE;
        }
    }

    return NOERROR;
}

ECode View::NeedGlobalAttributesUpdate(
    /* [in] */ Boolean force)
{
    AttachInfo* ai = mAttachInfo;
    if (ai != NULL && !ai->mRecomputeGlobalAttributes) {
        if (force || ai->mKeepScreenOn || (ai->mSystemUiVisibility != 0)
                || ai->mHasSystemUiListeners) {
            ai->mRecomputeGlobalAttributes = TRUE;
        }
    }

    return NOERROR;
}

/**
 * Returns whether the device is currently in touch mode.  Touch mode is entered
 * once the user begins interacting with the device by touch, and affects various
 * things like whether focus is always visible to the user.
 *
 * @return Whether the device is in touch mode.
 */
Boolean View::IsInTouchMode()
{
    if (mAttachInfo != NULL) {
        return mAttachInfo->mInTouchMode;
    }
    else {
        return ViewRootImpl::IsInTouchMode();
    }
}

AutoPtr<IContext> View::GetContext()
{
    return mContext;
}

/**
 * Handle a key event before it is processed by any input method
 * associated with the view hierarchy.  This can be used to intercept
 * key events in special situations before the IME consumes them; a
 * typical example would be handling the BACK key to update the application's
 * UI instead of allowing the IME to see it and close itself.
 *
 * @param keyCode The value in event.getKeyCode().
 * @param event Description of the key event.
 * @return If you handled the event, return TRUE. If you want to allow the
 *         event to be handled by the next receiver, return FALSE.
 */
Boolean View::OnKeyPreIme(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return FALSE;
}

/**
 * Default implementation of {@link KeyEvent.Callback#onKeyMultiple(int, int, KeyEvent)
 * KeyEvent.Callback.onKeyMultiple()}: perform press of the view
 * when {@link KeyEvent#KEYCODE_DPAD_CENTER} or {@link KeyEvent#KEYCODE_ENTER}
 * is released, if the view is enabled and clickable.
 *
 * @param keyCode A key code that represents the button pressed, from
 *                {@link android.view.KeyEvent}.
 * @param event   The KeyEvent object that defines the button action.
 */
Boolean View::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Boolean res, result = FALSE;
    event->IsConfirmKey(keyCode, &res);
    if (res) {
        if ((mViewFlags & ENABLED_MASK) == DISABLED) {
            return TRUE;
        }

        Int32 repeatCount;
        event->GetRepeatCount(&repeatCount);
        // Long clickable items don't necessarily have to be clickable
        if (((mViewFlags & CLICKABLE) == CLICKABLE ||
            (mViewFlags & LONG_CLICKABLE) == LONG_CLICKABLE) &&
            (repeatCount == 0)) {
            SetPressed(TRUE);

            CheckForLongClick(0);
            return TRUE;
        }
    }
    return result;
}

/**
 * Default implementation of {@link KeyEvent.Callback#onKeyLongPress(int, KeyEvent)
 * KeyEvent.Callback.onKeyLongPress()}: always returns FALSE (doesn't handle
 * the event).
 */
Boolean View::OnKeyLongPress(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return FALSE;
}

/**
 * Default implementation of {@link KeyEvent.Callback#onKeyMultiple(int, int, KeyEvent)
 * KeyEvent.Callback.onKeyMultiple()}: perform clicking of the view
 * when {@link KeyEvent#KEYCODE_DPAD_CENTER} or
 * {@link KeyEvent#KEYCODE_ENTER} is released.
 *
 * @param keyCode A key code that represents the button pressed, from
 *                {@link android.view.KeyEvent}.
 * @param event   The KeyEvent object that defines the button action.
 */
Boolean View::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Boolean result = FALSE;

    Boolean res;
    event->IsConfirmKey(keyCode, &res);
    if (res) {
        if ((mViewFlags & ENABLED_MASK) == DISABLED) {
            return TRUE;
        }

        if ((mViewFlags & CLICKABLE) == CLICKABLE && IsPressed()) {
            SetPressed(FALSE);

            if (!mHasPerformedLongPress) {
                // This is a tap, so remove the longpress check
                RemoveLongPressCallback();

                return PerformClick();
            }
        }
    }
    return FALSE;
}

/**
 * Default implementation of {@link KeyEvent.Callback#onKeyMultiple(int, int, KeyEvent)
 * KeyEvent.Callback.onKeyMultiple()}: always returns FALSE (doesn't handle
 * the event).
 *
 * @param keyCode     A key code that represents the button pressed, from
 *                    {@link android.view.KeyEvent}.
 * @param repeatCount The number of times the action was made.
 * @param event       The KeyEvent object that defines the button action.
 */
Boolean View::OnKeyMultiple(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 repeatCount,
    /* [in] */ IKeyEvent* event)
{
    return FALSE;
}

/**
 * Called when an unhandled key shortcut event occurs.
 *
 * @param keyCode The value in event.getKeyCode().
 * @param event Description of the key event.
 * @return If you handled the event, return TRUE. If you want to allow the
 *         event to be handled by the next receiver, return FALSE.
 */
Boolean View::OnKeyShortcut(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return FALSE;
}

/**
 * Check whether the called view is a text editor, in which case it
 * would make sense to automatically display a soft input window for
 * it.  Subclasses should override this if they implement
 * {@link #onCreateInputConnection(EditorInfo)} to return TRUE if
 * a call on that method would return a non-NULL InputConnection, and
 * they are really a first-class editor that the user would normally
 * start typing on when the go into a window containing your view.
 *
 * <p>The default implementation always returns FALSE.  This does
 * <em>not</em> mean that its {@link #onCreateInputConnection(EditorInfo)}
 * will not be called or the user can not otherwise perform edits on your
 * view; it is just a hint to the system that this is not the primary
 * purpose of this view.
 *
 * @return Returns TRUE if this view is a text editor, else FALSE.
 */
Boolean View::OnCheckIsTextEditor()
{
    return FALSE;
}

/**
 * Create a new InputConnection for an InputMethod to interact
 * with the view.  The default implementation returns NULL, since it doesn't
 * support input methods.  You can override this to implement such support.
 * This is only needed for views that take focus and text input.
 *
 * <p>When implementing this, you probably also want to implement
 * {@link #onCheckIsTextEditor()} to indicate you will return a
 * non-NULL InputConnection.
 *
 * @param outAttrs Fill in with attribute information about the connection.
 */
AutoPtr<IInputConnection> View::OnCreateInputConnection(
   /* [in] */ IEditorInfo* outAttrs)
{
   return NULL;
}

/**
 * Called by the {@link android.view.inputmethod.InputMethodManager}
 * when a view who is not the current
 * input connection target is trying to make a call on the manager.  The
 * default implementation returns FALSE; you can override this to return
 * TRUE for certain views if you are performing InputConnection proxying
 * to them.
 * @param view The View that is making the InputMethodManager call.
 * @return Return TRUE to allow the call, FALSE to reject.
 */
Boolean View::CheckInputConnectionProxy(
    /* [in] */ IView* view)
{
    return FALSE;
}

/**
 * Show the context menu for this view. It is not safe to hold on to the
 * menu after returning from this method.
 *
 * You should normally not overload this method. Overload
 * {@link #onCreateContextMenu(ContextMenu)} or define an
 * {@link OnCreateContextMenuListener} to add items to the context menu.
 *
 * @param menu The context menu to populate
 */
ECode View::CreateContextMenu(
    /* [in] */ IContextMenu* menu)
{
    AutoPtr<IContextMenuInfo> menuInfo = GetContextMenuInfo();

    // Sets the current menu info so all items added to menu will have
    // my extra info set.
    IMenuBuilder::Probe(menu)->SetCurrentMenuInfo(menuInfo);

    OnCreateContextMenu(menu);
    AutoPtr<ListenerInfo> li = mListenerInfo;
    if (li != NULL && li->mOnCreateContextMenuListener != NULL) {
        li->mOnCreateContextMenuListener->OnCreateContextMenu(menu,
                IVIEW_PROBE(this), menuInfo);
    }

    // Clear the extra information so subsequent items that aren't mine don't
    // have my extra info.
    IMenuBuilder::Probe(menu)->SetCurrentMenuInfo(NULL);

    if (mParent != NULL) {
        mParent->CreateContextMenu(menu);
    }

    return NOERROR;
}

/**
 * Views should implement this if they have extra information to associate
 * with the context menu. The return result is supplied as a parameter to
 * the {@link OnCreateContextMenuListener#onCreateContextMenu(ContextMenu, View, ContextMenuInfo)}
 * callback.
 *
 * @return Extra information about the item for which the context menu
 *         should be shown. This information will vary across different
 *         subclasses of View.
 */
AutoPtr<IContextMenuInfo> View::GetContextMenuInfo()
{
    return AutoPtr<IContextMenuInfo>(NULL);
}

/**
 * Views should implement this if the view itself is going to add items to
 * the context menu.
 *
 * @param menu the context menu to populate
 */
void View::OnCreateContextMenu(
    /* [in] */ IContextMenu* menu)
{
}

/**
 * Implement this method to handle trackball motion events.  The
 * <em>relative</em> movement of the trackball since the last event
 * can be retrieve with {@link MotionEvent#getX MotionEvent.getX()} and
 * {@link MotionEvent#getY MotionEvent.getY()}.  These are normalized so
 * that a movement of 1 corresponds to the user pressing one DPAD key (so
 * they will often be fractional values, representing the more fine-grained
 * movement information available from a trackball).
 *
 * @param event The motion event.
 * @return True if the event was handled, FALSE otherwise.
 */
Boolean View::OnTrackballEvent(
    /* [in] */ IMotionEvent* event)
{
    return FALSE;
}


Boolean View::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event)
{
    return FALSE;
}

Boolean View::OnHoverEvent(
    /* [in] */ IMotionEvent* event)
{
    // The root view may receive hover (or touch) events that are outside the bounds of
    // the window.  This code ensures that we only send accessibility events for
    // hovers that are actually within the bounds of the root view.
    Int32 action = 0;
    event->GetActionMasked(&action);
    Float x = 0.0f, y = 0.0f;
    event->GetX(&x);
    event->GetY(&y);
    if (!mSendingHoverAccessibilityEvents) {
        if ((action == IMotionEvent::ACTION_HOVER_ENTER
                || action == IMotionEvent::ACTION_HOVER_MOVE)
                && !HasHoveredChild()
                && PointInView(x, y)) {
            SendAccessibilityHoverEvent(IAccessibilityEvent::TYPE_VIEW_HOVER_ENTER);
            mSendingHoverAccessibilityEvents = TRUE;
        }
    }
    else {
        if (action == IMotionEvent::ACTION_HOVER_EXIT
                || (action == IMotionEvent::ACTION_MOVE
                        && !PointInView(x, y))) {
            mSendingHoverAccessibilityEvents = FALSE;
            SendAccessibilityHoverEvent(IAccessibilityEvent::TYPE_VIEW_HOVER_EXIT);
        }
    }

    if (IsHoverable()) {
        switch (action) {
            case IMotionEvent::ACTION_HOVER_ENTER:
                SetHovered(TRUE);
                break;
            case IMotionEvent::ACTION_HOVER_EXIT:
                SetHovered(FALSE);
                break;
        }

        // Dispatch the event to onGenericMotionEvent before returning TRUE.
        // This is to provide compatibility with existing applications that
        // handled HOVER_MOVE events in onGenericMotionEvent and that would
        // break because of the new default handling for hoverable views
        // in onHoverEvent.
        // Note that onGenericMotionEvent will be called by default when
        // onHoverEvent returns false (refer to dispatchGenericMotionEvent).
        DispatchGenericMotionEventInternal(event);
        return TRUE;
    }

    return FALSE;
}

Boolean View::IsHoverable()
{
    const Int32 viewFlags = mViewFlags;
    if ((viewFlags & ENABLED_MASK) == DISABLED) {
        return FALSE;
    }

    return (viewFlags & CLICKABLE) == CLICKABLE
            || (viewFlags & LONG_CLICKABLE) == LONG_CLICKABLE;
}

Boolean View::IsHovered()
{
    return (mPrivateFlags & PFLAG_HOVERED) != 0;
}

ECode View::SetHovered(
    /* [in] */ Boolean hovered)
{
    if (hovered) {
        if ((mPrivateFlags & PFLAG_HOVERED) == 0) {
            mPrivateFlags |= PFLAG_HOVERED;
            RefreshDrawableState();
            OnHoverChanged(TRUE);
        }
    }
    else {
        if ((mPrivateFlags & PFLAG_HOVERED) != 0) {
            mPrivateFlags &= ~PFLAG_HOVERED;
            RefreshDrawableState();
            OnHoverChanged(FALSE);
        }
    }
    return NOERROR;
}

ECode View::OnHoverChanged(
    /* [in] */ Boolean hovered)
{
    return NOERROR;
}

/**
 * Implement this method to handle touch screen motion events.
 *
 * @param event The motion event.
 * @return True if the event was handled, FALSE otherwise.
 */
Boolean View::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    Float x, y;
    event->GetX(&x);
    event->GetY(&y);
    Int32 viewFlags = mViewFlags;

    if ((viewFlags & ENABLED_MASK) == DISABLED) {
        Int32 action;
        event->GetAction(&action);
        if (action == IMotionEvent::ACTION_UP && (mPrivateFlags & PFLAG_PRESSED) != 0) {
            SetPressed(FALSE);
        }

        // A disabled view that is clickable still consumes the touch
        // events, it just doesn't respond to them.
        return (((viewFlags & CLICKABLE) == CLICKABLE ||
                (viewFlags & LONG_CLICKABLE) == LONG_CLICKABLE));
    }

    if (mTouchDelegate != NULL) {
        Boolean result;
        mTouchDelegate->OnTouchEvent(event, &result);
        if (result) {
            return TRUE;
        }
    }

    if (((viewFlags & CLICKABLE) == CLICKABLE ||
        (viewFlags & LONG_CLICKABLE) == LONG_CLICKABLE)) {
        Int32 action;
        event->GetAction(&action);
        switch (action) {
        case IMotionEvent::ACTION_UP:
            {
                Boolean prepressed = (mPrivateFlags & PFLAG_PREPRESSED) != 0;
                if ((mPrivateFlags & PFLAG_PRESSED) != 0 || prepressed) {
                    // take focus if we don't have it already and we should in
                    // touch mode.
                    Boolean focusTaken = FALSE;
                    if (IsFocusable() && IsFocusableInTouchMode() && !IsFocused()) {
                        focusTaken = RequestFocus();
                    }

                    if (prepressed) {
                        // The button is being released before we actually
                        // showed it as pressed.  Make it show the pressed
                        // state now (before scheduling the click) to ensure
                        // the user sees it.
                        SetPressed(TRUE, x, y);
                    }

                    if (!mHasPerformedLongPress) {
                        // This is a tap, so remove the longpress check
                        RemoveLongPressCallback();

                        // Only perform take click actions if we were in the pressed state
                        if (!focusTaken) {
                            // Use a Runnable and post this rather than calling
                            // performClick directly. This lets other visual state
                            // of the view update before click actions start.
                            if (mPerformClick == NULL) {
                                mPerformClick = new ViewPerformClick(this);
                            }

                            if (!Post(mPerformClick)) {
                                PerformClick();
                            }
                        }
                    }

                    if (mUnsetPressedState == NULL) {
                        mUnsetPressedState = new UnsetPressedState(this);
                    }

                    if (prepressed) {
                        PostDelayed(mUnsetPressedState,
                            CViewConfiguration::GetPressedStateDuration());
                    }
                    else if (!Post(mUnsetPressedState)) {
                        // If the post failed, unpress right now
                        mUnsetPressedState->Run();
                    }
                    RemoveTapCallback();
                }
            }
            break;
        case IMotionEvent::ACTION_DOWN:
            {
                mHasPerformedLongPress = FALSE;

                if (PerformButtonActionOnTouchDown(event)) {
                    break;
                }

                // Walk up the hierarchy to determine if we're inside a scrolling container.
                Boolean isInScrollingContainer = IsInScrollingContainer();

                // For views inside a scrolling container, delay the pressed feedback for
                // a short period in case this is a scroll.
                if (isInScrollingContainer) {
                    mPrivateFlags |= PFLAG_PREPRESSED;
                    if (mPendingCheckForTap == NULL) {
                        mPendingCheckForTap = new CheckForTap(this);
                    }
                    event->GetX(&(mPendingCheckForTap->mX));
                    event->GetY(&(mPendingCheckForTap->mY));
                    PostDelayed(mPendingCheckForTap.Get(), CViewConfiguration::GetTapTimeout());
                }
                else {
                    // Not inside a scrolling container, so show the feedback right away
                    SetPressed(TRUE, x, y);
                    CheckForLongClick(0);
                }
            }
            break;
        case IMotionEvent::ACTION_CANCEL:
            {
                SetPressed(FALSE);
                RemoveTapCallback();
                RemoveLongPressCallback();
            }
            break;
        case IMotionEvent::ACTION_MOVE:
            {
                DrawableHotspotChanged(x, y);
                Float fx, fy;
                event->GetX(&fx);
                event->GetY(&fy);

                Int32 x = (Int32)fx;
                Int32 y = (Int32)fy;

                // Be lenient about moving outside of buttons
                if (!PointInView(x, y, mTouchSlop)) {
                    // Outside button
                    RemoveTapCallback();
                    if ((mPrivateFlags & PFLAG_PRESSED) != 0) {
                        // Remove any future long press/tap checks
                        RemoveLongPressCallback();

                        SetPressed(FALSE);
                    }
                }
            }
            break;
        default:
            break;
        }
        return TRUE;
    }

    return FALSE;
}

/**
 * @hide
 */
Boolean View::IsInScrollingContainer()
{
    AutoPtr<IViewParent> p = GetParent();
    while (p != NULL && IViewGroup::Probe(p) != NULL) {
        Boolean should = FALSE;
        IViewGroup::Probe(p)->ShouldDelayChildPressedState(&should);
        if (should) {
            return TRUE;
        }
        AutoPtr<IViewParent> temp;
        p->GetParent((IViewParent**)&temp);
        p = temp;
    }

    return FALSE;
}

/**
 * Remove the longpress detection timer.
 */
void View::RemoveLongPressCallback()
{
    if (mPendingCheckForLongPress != NULL) {
        RemoveCallbacks(mPendingCheckForLongPress);
    }
}

void View::RemovePerformClickCallback()
{
    if (mPerformClick != NULL) {
        RemoveCallbacks(mPerformClick);
    }
}

/**
 * Remove the prepress detection timer.
 */
void View::RemoveUnsetPressCallback()
{
    if ((mPrivateFlags & PFLAG_PRESSED) != 0 && mUnsetPressedState != NULL) {
        SetPressed(FALSE);
        RemoveCallbacks(mUnsetPressedState);
    }
}

/**
 * Remove the tap detection timer.
 */
void View::RemoveTapCallback()
{
    if (mPendingCheckForTap != NULL) {
        mPrivateFlags &= ~PFLAG_PREPRESSED;
        RemoveCallbacks(mPendingCheckForTap);
    }
}

/**
 * Cancels a pending long press.  Your subclass can use this if you
 * want the context menu to come up if the user presses and holds
 * at the same place, but you don't want it to come up if they press
 * and then move around enough to cause scrolling.
 */
ECode View::CancelLongPress()
{
    RemoveLongPressCallback();

    /*
        * The prepressed state handled by the tap callback is a display
        * construct, but the tap callback will post a long press callback
        * less its own timeout. Remove it here.
        */
    RemoveTapCallback();

    return NOERROR;
}

/**
 * Remove the pending callback for sending a
 * {@link AccessibilityEvent#TYPE_VIEW_SCROLLED} accessibility event.
 */
void View::RemoveSendViewScrolledAccessibilityEventCallback()
{
    if (mSendViewScrolledAccessibilityEvent != NULL) {
        RemoveCallbacks(mSendViewScrolledAccessibilityEvent);
        mSendViewScrolledAccessibilityEvent->mIsPending = FALSE;
    }
}

/**
 * Sets the TouchDelegate for this View.
 */
ECode View::SetTouchDelegate(
    /* [in] */ ITouchDelegate* touchDelegate)
{
    mTouchDelegate = touchDelegate;

    return NOERROR;
}

/**
 * Gets the TouchDelegate for this View.
 */
ECode View::GetTouchDelegate(
    /* [out] */ ITouchDelegate** touchDelegate)
{
    VALIDATE_NOT_NULL(touchDelegate);
    *touchDelegate = mTouchDelegate;
    REFCOUNT_ADD(*touchDelegate);

    return NOERROR;
}

/**
 * Request unbuffered dispatch of the given stream of MotionEvents to this View.
 *
 * Until this View receives a corresponding {@link MotionEvent#ACTION_UP}, ask that the input
 * system not batch {@link MotionEvent}s but instead deliver them as soon as they're
 * available. This method should only be called for touch events.
 *
 * <p class="note">This api is not intended for most applications. Buffered dispatch
 * provides many of benefits, and just requesting unbuffered dispatch on most MotionEvent
 * streams will not improve your input latency. Side effects include: increased latency,
 * jittery scrolls and inability to take advantage of system resampling. Talk to your input
 * professional to see if {@link #requestUnbufferedDispatch(MotionEvent)} is right for
 * you.</p>
 */
ECode View::RequestUnbufferedDispatch(
    /* [in] */ IMotionEvent* event)
{
    Int32 action;
    event->GetAction(&action);
    Boolean res;
    event->IsTouchEvent(&res);
    if (mAttachInfo == NULL
            || action != IMotionEvent::ACTION_DOWN && action != IMotionEvent::ACTION_MOVE
            || !res) {
        return NOERROR;
    }
    mAttachInfo->mUnbufferedDispatchRequested = TRUE;
    return NOERROR;
}

/**
 * Set flags controlling behavior of this view.
 *
 * @param flags Constant indicating the value which should be set
 * @param mask Constant indicating the bit range that should be changed
 */
void View::SetFlags(
    /* [in] */ Int32 flags,
    /* [in] */ Int32 mask)
{
    AutoPtr<IAccessibilityManager> manager;
    CAccessibilityManager::GetInstance(mContext, (IAccessibilityManager**)&manager);
    Boolean accessibilityEnabled, accessibility;
    manager->IsEnabled(&accessibilityEnabled);
    IncludeForAccessibility(&accessibility);
    Boolean oldIncludeForAccessibility = accessibilityEnabled && accessibility;

    Int32 old = mViewFlags;
    mViewFlags = (mViewFlags & ~mask) | (flags & mask);

    Int32 changed = mViewFlags ^ old;
    if (changed == 0) {
        return;
    }

    Int32 privateFlags = mPrivateFlags;

    /* Check if the FOCUSABLE bit has changed */
    if (((changed & FOCUSABLE_MASK) != 0) &&
            ((privateFlags & PFLAG_HAS_BOUNDS) !=0)) {
        if (((old & FOCUSABLE_MASK) == FOCUSABLE)
                && ((privateFlags & PFLAG_FOCUSED) != 0)) {
            /* Give up focus if we are no longer focusable */
            ClearFocus();
        }
        else if (((old & FOCUSABLE_MASK) == NOT_FOCUSABLE)
                && ((privateFlags & PFLAG_FOCUSED) == 0)) {
            /*
             * Tell the view system that we are now available to take focus
             * if no one else already has it.
             */
            if (mParent != NULL) {
                mParent->FocusableViewAvailable(IVIEW_PROBE(this));
            }
        }
    }

    Int32 newVisibility = flags & VISIBILITY_MASK;
    if (newVisibility == IView::VISIBLE) {
        if ((changed & VISIBILITY_MASK) != 0) {
            /*
             * If this view is becoming visible, invalidate it in case it changed while
             * it was not visible. Marking it drawn ensures that the invalidation will
             * go through.
             */
            mPrivateFlags |= PFLAG_DRAWN;
            Invalidate(TRUE);

            NeedGlobalAttributesUpdate(TRUE);

            // a view becoming visible is worth notifying the parent
            // about in case nothing has focus.  even if this specific view
            // isn't focusable, it may contain something that is, so let
            // the root view try to give this focus if nothing else does.
            if ((mParent != NULL) && (mBottom > mTop) && (mRight > mLeft)) {
                mParent->FocusableViewAvailable(IVIEW_PROBE(this));
            }
        }
    }

    /* Check if the GONE bit has changed */
    if ((changed & IView::GONE) != 0) {
        NeedGlobalAttributesUpdate(FALSE);
        RequestLayout();

        if (((mViewFlags & VISIBILITY_MASK) == IView::GONE)) {
            if (HasFocus()) ClearFocus();
            ClearAccessibilityFocus();
            DestroyDrawingCache();

            if (mParent != NULL && VIEW_PROBE(mParent) != NULL) {
                // GONE views noop invalidation, so invalidate the parent
                VIEW_PROBE(mParent)->Invalidate(TRUE);
            }
            // Mark the view drawn to ensure that it gets invalidated properly the next
            // time it is visible and gets invalidated
            mPrivateFlags |= PFLAG_DRAWN;
        }
        if (mAttachInfo != NULL) {
            mAttachInfo->mViewVisibilityChanged = TRUE;
        }
    }

    /* Check if the VISIBLE bit has changed */
    if ((changed & IView::INVISIBLE) != 0) {
        NeedGlobalAttributesUpdate(FALSE);
        /*
         * If this view is becoming invisible, set the DRAWN flag so that
         * the next invalidate() will not be skipped.
         */
        mPrivateFlags |= PFLAG_DRAWN;

        if (((mViewFlags & VISIBILITY_MASK) == IView::INVISIBLE)) {
            // root view becoming invisible shouldn't clear focus and accessibility focus
            if (GetRootView().Get() != IVIEW_PROBE(this)) {
                if (HasFocus()) ClearFocus();
                ClearAccessibilityFocus();
            }
        }
        if (mAttachInfo != NULL) {
            mAttachInfo->mViewVisibilityChanged = TRUE;
        }
    }

    if ((changed & VISIBILITY_MASK) != 0) {
        // If the view is invisible, cleanup its display list to free up resources
        if (newVisibility != IView::VISIBLE && mAttachInfo != NULL) {
            CleanupDraw();
        }

        if (mParent != NULL && IViewGroup::Probe(mParent) != NULL) {
            VIEWGROUP_PROBE(mParent)->OnChildVisibilityChanged(IVIEW_PROBE(this),
                    (changed & VISIBILITY_MASK), newVisibility);
            VIEW_PROBE(mParent)->Invalidate(TRUE);
        }
        else if (mParent != NULL) {
            mParent->InvalidateChild(IVIEW_PROBE(this), NULL);
        }

        DispatchVisibilityChanged(
            IVIEW_PROBE(this), newVisibility);

        NotifySubtreeAccessibilityStateChangedIfNeeded();
    }

    if ((changed & WILL_NOT_CACHE_DRAWING) != 0) {
        DestroyDrawingCache();
    }

    if ((changed & DRAWING_CACHE_ENABLED) != 0) {
        DestroyDrawingCache();
        mPrivateFlags &= ~PFLAG_DRAWING_CACHE_VALID;
        InvalidateParentCaches();
    }

    if ((changed & DRAWING_CACHE_QUALITY_MASK) != 0) {
        DestroyDrawingCache();
        mPrivateFlags &= ~PFLAG_DRAWING_CACHE_VALID;
    }

    if ((changed & DRAW_MASK) != 0) {
        if ((mViewFlags & WILL_NOT_DRAW) != 0) {
            if (mBackground != NULL) {
                mPrivateFlags &= ~PFLAG_SKIP_DRAW;
                mPrivateFlags |= PFLAG_ONLY_DRAWS_BACKGROUND;
            }
            else {
                mPrivateFlags |= PFLAG_SKIP_DRAW;
            }
        }
        else {
            mPrivateFlags &= ~PFLAG_SKIP_DRAW;
        }
        RequestLayout();
        Invalidate(TRUE);
    }

    if ((changed & IView::KEEP_SCREEN_ON) != 0) {
        if (mParent != NULL && mAttachInfo != NULL && !mAttachInfo->mRecomputeGlobalAttributes) {
            mParent->RecomputeViewAttributes(IVIEW_PROBE(this));
        }
    }

    if (accessibilityEnabled) {
        if ((changed & FOCUSABLE_MASK) != 0 || (changed & VISIBILITY_MASK) != 0
                || (changed & CLICKABLE) != 0 || (changed & LONG_CLICKABLE) != 0) {
            Boolean res;
            IncludeForAccessibility(&res);
            if (oldIncludeForAccessibility != res) {
                NotifySubtreeAccessibilityStateChangedIfNeeded();
            } else {
                NotifyViewAccessibilityStateChangedIfNeeded(
                        IAccessibilityEvent::CONTENT_CHANGE_TYPE_UNDEFINED);
            }
        } else if ((changed & ENABLED_MASK) != 0) {
            NotifyViewAccessibilityStateChangedIfNeeded(
                    IAccessibilityEvent::CONTENT_CHANGE_TYPE_UNDEFINED);
        }
    }
}

/**
 * Change the view's z order in the tree, so it's on top of other sibling
 * views. This ordering change may affect layout, if the parent container
 * uses an order-dependent layout scheme (e.g., LinearLayout). Prior
 * to {@link android.os.Build.VERSION_CODES#KITKAT} this
 * method should be followed by calls to {@link #requestLayout()} and
 * {@link View#invalidate()} on the view's parent to force the parent to redraw
 * with the new child ordering.
 *
 * @see ViewGroup#bringChildToFront(View)
 */
ECode View::BringToFront()
{
    if (mParent != NULL) {
        mParent->BringChildToFront(IVIEW_PROBE(this));
    }

    return NOERROR;
}

/**
 * This is called in response to an internal scroll in this view (i.e., the
 * view scrolled its own contents). This is typically as a result of
 * {@link #scrollBy(int, int)} or {@link #scrollTo(int, int)} having been
 * called.
 *
 * @param l Current horizontal scroll origin.
 * @param t Current vertical scroll origin.
 * @param oldl Previous horizontal scroll origin.
 * @param oldt Previous vertical scroll origin.
 */
void View::OnScrollChanged(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 oldl,
    /* [in] */ Int32 oldt)
{
    AutoPtr<IAccessibilityManager> manger;
    CAccessibilityManager::GetInstance(mContext, (IAccessibilityManager**)&manger);
    Boolean bval;
    if (manger->IsEnabled(&bval),  bval) {
        PostSendViewScrolledAccessibilityEventCallback();
    }

    mBackgroundSizeChanged = TRUE;

    AttachInfo* ai = mAttachInfo;
    if (ai != NULL) {
        ai->mViewScrollChanged = TRUE;
    }
}

/**
 * This is called during layout when the size of this view has changed. If
 * you were just added to the view hierarchy, you're called with the old
 * values of 0.
 *
 * @param w Current width of this view.
 * @param h Current height of this view.
 * @param oldw Old width of this view.
 * @param oldh Old height of this view.
 */
void View::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{}

/**
 * Called by draw to draw the child views. This may be overridden
 * by derived classes to gain control just before its children are drawn
 * (but after its own view has been drawn).
 * @param canvas the canvas on which to draw the view
 */
void View::DispatchDraw(
    /* [in] */ ICanvas* canvas)
{}

/**
 * Gets the parent of this view. Note that the parent is a
 * ViewParent and not necessarily a View.
 *
 * @return Parent of this view.
 */
AutoPtr<IViewParent> View::GetParent()
{
    return mParent;
}

ECode View::SetScrollX(
    /* [in] */ Int32 value)
{
    ScrollTo(value, mScrollY);
    return NOERROR;
}

ECode View::SetScrollY(
    /* [in] */ Int32 value)
{
    ScrollTo(mScrollX, value);
    return NOERROR;
}

/**
 * Return the scrolled left position of this view. This is the left edge of
 * the displayed part of your view. You do not need to draw any pixels
 * farther left, since those are outside of the frame of your view on
 * screen.
 *
 * @return The left edge of the displayed part of your view, in pixels.
 */
Int32 View::GetScrollX()
{
    return mScrollX;
}

/**
 * Return the scrolled top position of this view. This is the top edge of
 * the displayed part of your view. You do not need to draw any pixels above
 * it, since those are outside of the frame of your view on screen.
 *
 * @return The top edge of the displayed part of your view, in pixels.
 */
Int32 View::GetScrollY()
{
    return mScrollY;
}

/**
 * Return the width of the your view.
 *
 * @return The width of your view, in pixels.
 */
Int32 View::GetWidth()
{
    return mRight - mLeft;
}

/**
 * Return the height of your view.
 *
 * @return The height of your view, in pixels.
 */
Int32 View::GetHeight()
{
    return mBottom - mTop;
}

/**
 * Return the visible drawing bounds of your view. Fills in the output
 * rectangle with the values from getScrollX(), getScrollY(),
 * getWidth(), and getHeight().
 *
 * @param outRect The (scrolled) drawing bounds of the view.
 */
ECode View::GetDrawingRect(
    /* [out] */ IRect* outRect)
{
    VALIDATE_NOT_NULL(outRect);
    return outRect->Set(
        mScrollX, mScrollY,
        mScrollX + (mRight - mLeft),
        mScrollY + (mBottom - mTop));
}

/**
 * The width of this view as measured in the most recent call to measure().
 * This should be used during measurement and layout calculations only. Use
 * {@link #getWidth()} to see how wide a view is after layout.
 *
 * @return The measured width of this view.
 */
Int32 View::GetMeasuredWidth()
{
    return mMeasuredWidth & IView::MEASURED_SIZE_MASK;
}

Int32 View::GetMeasuredWidthAndState()
{
    return mMeasuredWidth;
}

/**
 * The height of this view as measured in the most recent call to measure().
 * This should be used during measurement and layout calculations only. Use
 * {@link #getHeight()} to see how tall a view is after layout.
 *
 * @return The measured height of this view.
 */
Int32 View::GetMeasuredHeight()
{
    return mMeasuredHeight & IView::MEASURED_SIZE_MASK;
}

Int32 View::GetMeasuredHeightAndState()
{
    return mMeasuredHeight;
}


Int32 View::GetMeasuredState()
{
    return (mMeasuredWidth&IView::MEASURED_STATE_MASK)
            | ((mMeasuredHeight>>IView::MEASURED_HEIGHT_STATE_SHIFT)
                    & (IView::MEASURED_STATE_MASK>>IView::MEASURED_HEIGHT_STATE_SHIFT));
}

AutoPtr<IMatrix> View::GetMatrix()
{
    EnsureTransformationInfo();
    AutoPtr<IMatrix> matrix = mTransformationInfo->mMatrix;
    mRenderNode->GetMatrix((IMatrix**)&matrix);
    return matrix;
}

Boolean View::HasIdentityMatrix()
{
    Boolean res;
    mRenderNode->HasIdentityMatrix(&res);
    return res;
}

void View::EnsureTransformationInfo()
{
    if (mTransformationInfo == NULL) {
        mTransformationInfo = new TransformationInfo();
    }
}

AutoPtr<IMatrix> View::GetInverseMatrix()
{
    EnsureTransformationInfo();
    if (mTransformationInfo->mInverseMatrix == NULL) {

        CMatrix::New((IMatrix**)&(mTransformationInfo->mInverseMatrix));
    }
    AutoPtr<IMatrix> matrix = mTransformationInfo->mInverseMatrix;
    mRenderNode->GetInverseMatrix((IMatrix**)&matrix);
    return matrix;
}

Float View::GetCameraDistance()
{
    AutoPtr<IDisplayMetrics> metrics;
    mResources->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    Float dpi, distance;
    metrics->GetDensityDpi(&dpi);
    mRenderNode->GetCameraDistance(&distance);
    return -(distance * dpi);
}

ECode View::SetCameraDistance(
    /* [in] */ Float distance)
{
    Float dpi;
    AutoPtr<IDisplayMetrics> metrics;
    mResources->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    metrics->GetDensityDpi(&dpi);

    InvalidateViewProperty(TRUE, FALSE);
    Boolean res;

    mRenderNode->etCameraDistance(-Elastos::Core::Math::Abs(distance) / dpi, &res);
    invalidateViewProperty(FALSE, FALSE);

    InvalidateParentIfNeededAndWasQuickRejected();

    return NOERROR;
}

Float View::GetRotation()
{
    Float rotation;
    mRenderNode->GetRotation(&rotation);
    return rotation;
}

ECode View::SetRotation(
    /* [in] */ Float rotation)
{
    if (rotation != GetRotation()) {
        // Double-invalidation is necessary to capture view's old and new areas
        InvalidateViewProperty(TRUE, FALSE);
        Boolean res;
        mRenderNode->SetRotation(rotation, &res);
        InvalidateViewProperty(FALSE, TRUE);

        InvalidateParentIfNeededAndWasQuickRejected();
        NotifySubtreeAccessibilityStateChangedIfNeeded();
    }

    return NOERROR;
}

Float View::GetRotationY()
{
    Float rotationY;
    mRenderNode->GetRotationY(&rotationY);
    return rotationY;
}

ECode View::SetRotationY(
    /* [in] */ Float rotationY)
{
    if (rotationY != GetRotationY()) {
        InvalidateViewProperty(TRUE, FALSE);
        Boolean res;
        mRenderNode->SetRotationY(rotationY, &res);
        InvalidateViewProperty(FALSE, TRUE);

        InvalidateParentIfNeededAndWasQuickRejected();
        NotifySubtreeAccessibilityStateChangedIfNeeded();
    }

    return NOERROR;
}

Float View::GetRotationX()
{
    Float rotationX;
    mRenderNode->GetRotationX(&rotationX);
    return rotationX;
}

ECode View::SetRotationX(
    /* [in] */ Float rotationX)
{
    if (rotationX != GetRotationX()) {
        InvalidateViewProperty(TRUE, FALSE);
        Boolean res;
        mRenderNode->SetRotationX(rotationX, &res);
        InvalidateViewProperty(FALSE, TRUE);

        InvalidateParentIfNeededAndWasQuickRejected();
        NotifySubtreeAccessibilityStateChangedIfNeeded();
    }

    return NOERROR;
}

Float View::GetScaleX()
{
    Float scaleX;
    mRenderNode->GetScaleX(&scaleX);
    return scaleX;
}

ECode View::SetScaleX(
    /* [in] */ Float scaleX)
{
    if (scaleX != GetScaleX()) {
        InvalidateViewProperty(TRUE, FALSE);
        Boolean res;
        mRenderNode->setScaleX(scaleX, &res);
        InvalidateViewProperty(FALSE, TRUE);

        InvalidateParentIfNeededAndWasQuickRejected();
        NotifySubtreeAccessibilityStateChangedIfNeeded();
    }

    return NOERROR;
}

Float View::GetScaleY()
{
    Float scaleY;
    mRenderNode->GetScaleY(&scaleY);
    return scaleY;
}

ECode View::SetScaleY(
    /* [in] */ Float scaleY)
{
    if (scaleY != GetScaleY()) {
        InvalidateViewProperty(TRUE, FALSE);
        Boolean res;
        mRenderNode->setScaleY(scaleY, &res);
        InvalidateViewProperty(FALSE, TRUE);

        InvalidateParentIfNeededAndWasQuickRejected();
        NotifySubtreeAccessibilityStateChangedIfNeeded();
    }

    return NOERROR;
}

Float View::GetPivotX()
{
    Float pivotX;
    mRenderNode->GetPivotX(&pivotX);
    return pivotX;
}

ECode View::SetPivotX(
    /* [in] */ Float pivotX)
{
    Boolean set;
    mRenderNode->IsPivotExplicitlySet(&set);
    if (!set || pivotX != GetPivotX()) {
        InvalidateViewProperty(TRUE, FALSE);
        Boolean res;
        mRenderNode->SetPivotX(pivotX, &res);
        InvalidateViewProperty(FALSE, TRUE);

        InvalidateParentIfNeededAndWasQuickRejected();
    }

    return NOERROR;
}

Float View::GetPivotY()
{
    Float pivotY;
    mRenderNode->GetPivotY(&pivotY);
    return pivotY;
}

ECode View::SetPivotY(
    /* [in] */ Float pivotY)
{
    Boolean set;
    mRenderNode->IsPivotExplicitlySet(&set);
    if (!set || pivotY != GetPivotY()) {
        InvalidateViewProperty(TRUE, FALSE);
        Boolean res;
        mRenderNode->setPivotY(pivotY, &res);
        InvalidateViewProperty(FALSE, TRUE);

        InvalidateParentIfNeededAndWasQuickRejected();
    }

    return NOERROR;
}

Float View::GetAlpha()
{
    return mTransformationInfo != NULL ? mTransformationInfo->mAlpha : 1.0f;
}

Boolean View::HasOverlappingRendering()
{
    return TRUE;
}

ECode View::SetAlpha(
    /* [in] */ Float alpha)
{
    EnsureTransformationInfo();
    if (mTransformationInfo->mAlpha != alpha) {
        mTransformationInfo->mAlpha = alpha;
        if (OnSetAlpha((Int32) (alpha * 255))) {
            mPrivateFlags |= PFLAG_ALPHA_SET;
            // subclass is handling alpha - don't optimize rendering cache invalidation
            InvalidateParentCaches();
            Invalidate(TRUE);
        }
        else {
            mPrivateFlags &= ~PFLAG_ALPHA_SET;
            InvalidateViewProperty(TRUE, FALSE);
            Boolean res;
            mRenderNode->SetAlpha(GetFinalAlpha(), &res);
            NotifyViewAccessibilityStateChangedIfNeeded(
                    IAccessibilityEvent::CONTENT_CHANGE_TYPE_UNDEFINED);
        }
    }

    return NOERROR;
}

Boolean View::SetAlphaNoInvalidation(
    /* [in] */ Float alpha)
{
    EnsureTransformationInfo();
    if (mTransformationInfo->mAlpha != alpha) {
        mTransformationInfo->mAlpha = alpha;
        Boolean subclassHandlesAlpha = OnSetAlpha((Int32) (alpha * 255));
        if (subclassHandlesAlpha) {
            mPrivateFlags |= PFLAG_ALPHA_SET;
            return TRUE;
        }
        else {
            mPrivateFlags &= ~PFLAG_ALPHA_SET;
            Boolean res;
            mRenderNode->SetAlpha(GetFinalAlpha(), &res);
        }
    }
    return FALSE;
}

/**
 * This property is hidden and intended only for use by the Fade transition, which
 * animates it to produce a visual translucency that does not side-effect (or get
 * affected by) the real alpha property. This value is composited with the other
 * alpha value (and the AlphaAnimation value, when that is present) to produce
 * a final visual translucency result, which is what is passed into the DisplayList.
 *
 * @hide
 */
ECode View::SetTransitionAlpha(
    /* [in] */ Float alpha)
{
    EnsureTransformationInfo();
    if (mTransformationInfo->mTransitionAlpha != alpha) {
        mTransformationInfo->mTransitionAlpha = alpha;
        mPrivateFlags &= ~PFLAG_ALPHA_SET;
        InvalidateViewProperty(TRUE, FALSE);
        Boolean res;
        mRenderNode->SetAlpha(GetFinalAlpha(), &res);
    }
}

/**
 * Calculates the visual alpha of this view, which is a combination of the actual
 * alpha value and the transitionAlpha value (if set).
 */
Float View::GetFinalAlpha()
{
    if (mTransformationInfo != NULL) {
        return mTransformationInfo->mAlpha * mTransformationInfo->mTransitionAlpha;
    }
    return 1;
}

/**
 * This property is hidden and intended only for use by the Fade transition, which
 * animates it to produce a visual translucency that does not side-effect (or get
 * affected by) the real alpha property. This value is composited with the other
 * alpha value (and the AlphaAnimation value, when that is present) to produce
 * a final visual translucency result, which is what is passed into the DisplayList.
 *
 * @hide
 */
ECode View::GetTransitionAlpha(
    /* [out] */ Float* alpha)
{
    return mTransformationInfo != NULL ? mTransformationInfo->mTransitionAlpha : 1;
}

/**
 * Top position of this view relative to its parent.
 *
 * @return The top of this view, in pixels.
 */
Int32 View::GetTop()
{
    return mTop;
}

ECode View::SetTop(
    /* [in] */ Int32 top)
{
    if (top != mTop) {
        Boolean matrixIsIdentity = HasIdentityMatrix();
        if (matrixIsIdentity) {
            if (mAttachInfo != NULL) {
                Int32 minTop = 0;
                Int32 yLoc = 0;
                if (top < mTop) {
                    minTop = top;
                    yLoc = top - mTop;
                }
                else {
                    minTop = mTop;
                    yLoc = 0;
                }
                Invalidate(0, yLoc, mRight - mLeft, mBottom - minTop);
            }
        }
        else {
            // Double-invalidation is necessary to capture view's old and new areas
            Invalidate(TRUE);
        }

        Int32 width = mRight - mLeft;
        Int32 oldHeight = mBottom - mTop;

        mTop = top;
        Boolean res;
        mRenderNode->SetTop(mTop, &res);

        SizeChange(width, mBottom - mTop, width, oldHeight);

        if (!matrixIsIdentity) {
            mPrivateFlags |= PFLAG_DRAWN; // force another invalidation with the new orientation
            Invalidate(TRUE);
        }
        mBackgroundSizeChanged = TRUE;
        InvalidateParentIfNeeded();
        if ((mPrivateFlags2 & PFLAG2_VIEW_QUICK_REJECTED) == PFLAG2_VIEW_QUICK_REJECTED) {
            // View was rejected last time it was drawn by its parent; this may have changed
            InvalidateParentIfNeeded();
        }
    }

    return NOERROR;
}

/**
 * Bottom position of this view relative to its parent.
 *
 * @return The bottom of this view, in pixels.
 */
Int32 View::GetBottom()
{
    return mBottom;
}

Boolean View::IsDirty()
{
    return (mPrivateFlags & PFLAG_DIRTY_MASK) != 0;
}

ECode View::SetBottom(
    /* [in] */ Int32 bottom)
{
    if (bottom != mBottom) {
        Boolean matrixIsIdentity = HasIdentityMatrix();
        if (matrixIsIdentity) {
            if (mAttachInfo != NULL) {
                Int32 maxBottom;
                if (bottom < mBottom) {
                    maxBottom = mBottom;
                }
                else {
                    maxBottom = bottom;
                }
                Invalidate(0, 0, mRight - mLeft, maxBottom - mTop);
            }
        }
        else {
            // Double-invalidation is necessary to capture view's old and new areas
            Invalidate(TRUE);
        }

        Int32 width = mRight - mLeft;
        Int32 oldHeight = mBottom - mTop;

        mBottom = bottom;
        Boolean res;
        mRenderNode->SetBottom(mBottom, &res);

        SizeChange(width, mBottom - mTop, width, oldHeight);

        if (!matrixIsIdentity) {
            mPrivateFlags |= PFLAG_DRAWN; // force another invalidation with the new orientation
            Invalidate(TRUE);
        }
        mBackgroundSizeChanged = TRUE;
        InvalidateParentIfNeeded();
        if ((mPrivateFlags2 & PFLAG2_VIEW_QUICK_REJECTED) == PFLAG2_VIEW_QUICK_REJECTED) {
            // View was rejected last time it was drawn by its parent; this may have changed
            InvalidateParentIfNeeded();
        }
    }

    return NOERROR;
}

/**
 * Left position of this view relative to its parent.
 *
 * @return The left edge of this view, in pixels.
 */
Int32 View::GetLeft()
{
    return mLeft;
}

ECode View::SetLeft(
    /* [in] */ Int32 left)
{
    if (left != mLeft) {
        Boolean matrixIsIdentity = HasIdentityMatrix();
        if (matrixIsIdentity) {
            if (mAttachInfo != NULL) {
                Int32 minLeft;
                Int32 xLoc;
                if (left < mLeft) {
                    minLeft = left;
                    xLoc = left - mLeft;
                }
                else {
                    minLeft = mLeft;
                    xLoc = 0;
                }
                Invalidate(xLoc, 0, mRight - minLeft, mBottom - mTop);
            }
        }
        else {
            // Double-invalidation is necessary to capture view's old and new areas
            Invalidate(TRUE);
        }

        Int32 oldWidth = mRight - mLeft;
        Int32 height = mBottom - mTop;

        mLeft = left;
        Boolean res;
        mRenderNode->SetLeft(left, &res);

        SizeChange(mRight - mLeft, height, oldWidth, height);

        if (!matrixIsIdentity) {
            mPrivateFlags |= PFLAG_DRAWN; // force another invalidation with the new orientation
            Invalidate(TRUE);
        }
        mBackgroundSizeChanged = TRUE;
        InvalidateParentIfNeeded();
        if ((mPrivateFlags2 & PFLAG2_VIEW_QUICK_REJECTED) == PFLAG2_VIEW_QUICK_REJECTED) {
            // View was rejected last time it was drawn by its parent; this may have changed
            InvalidateParentIfNeeded();
        }
    }
    return NOERROR;
}

/**
 * Right position of this view relative to its parent.
 *
 * @return The right edge of this view, in pixels.
 */
Int32 View::GetRight()
{
    return mRight;
}

ECode View::SetRight(
    /* [in] */ Int32 right)
{
    if (right != mRight) {
        Boolean matrixIsIdentity = HasIdentityMatrix();
        if (matrixIsIdentity) {
            if (mAttachInfo != NULL) {
                Int32 maxRight;
                if (right < mRight) {
                    maxRight = mRight;
                }
                else {
                    maxRight = right;
                }
                Invalidate(0, 0, maxRight - mLeft, mBottom - mTop);
            }
        }
        else {
            // Double-invalidation is necessary to capture view's old and new areas
            Invalidate(TRUE);
        }

        Int32 oldWidth = mRight - mLeft;
        Int32 height = mBottom - mTop;

        mRight = right;
        Boolean res;
        mRenderNode->SetRight(mRight, &res);

        SizeChange(mRight - mLeft, height, oldWidth, height);

        if (!matrixIsIdentity) {
            mPrivateFlags |= PFLAG_DRAWN; // force another invalidation with the new orientation
            Invalidate(TRUE);
        }
        mBackgroundSizeChanged = TRUE;
        InvalidateParentIfNeeded();
        if ((mPrivateFlags2 & PFLAG2_VIEW_QUICK_REJECTED) == PFLAG2_VIEW_QUICK_REJECTED) {
            // View was rejected last time it was drawn by its parent; this may have changed
            InvalidateParentIfNeeded();
        }
    }

    return NOERROR;
}

ECode View::GetX(
    /* [out] */ Float* x)
{
    VALIDATE_NOT_NULL(x)
    Float translationX;
    GetTranslationX(&translationX);
    *x = mLeft + translationX;
    return NOERROR;
}

ECode View::SetX(
    /* [in] */ Float x)
{
    SetTranslationX(x - mLeft);
    return NOERROR;
}

ECode View::GetY(
    /* [out] */ Float* y)
{
    VALIDATE_NOT_NULL(y)
    Float translationY;
    GetTranslationY(&translationY);
    *y = mLeft + translationY;
    return NOERROR;
}

ECode View::SetY(
    /* [in] */ Float y)
{
    SetTranslationY(y - mTop);
    return NOERROR;
}

/**
 * The visual z position of this view, in pixels. This is equivalent to the
 * {@link #setTranslationZ(float) translationZ} property plus the current
 * {@link #getElevation() elevation} property.
 *
 * @return The visual z position of this view, in pixels.
 */
ECode View::GetZ(
   /* [out] */ Float* z)
{
    VALIDATE_NOT_NULL(z)
    Float elevation, translationZ;
    GetElevation(&elevation);
    GetTranslationZ(&translationZ);
    *z = elevation + translationZ;
    return NOERROR;
}

/**
 * Sets the visual z position of this view, in pixels. This is equivalent to setting the
 * {@link #setTranslationZ(float) translationZ} property to be the difference between
 * the x value passed in and the current {@link #getElevation() elevation} property.
 *
 * @param z The visual z position of this view, in pixels.
 */
ECode View::SetZ(
    /* [in] */ Float z)
{
    Float elevation;
    GetElevation(&elevation);
    SetTranslationZ(z - elevation);
}

/**
 * The base elevation of this view relative to its parent, in pixels.
 *
 * @return The base depth position of the view, in pixels.
 */
ECode View::GetElevation(
    /* [out] */ Float* elevation)
{
    VALIDATE_NOT_NULL(elevation)
    return mRenderNode->GetElevation(elevation);
}

/**
 * Sets the base elevation of this view, in pixels.
 *
 * @attr ref android.R.styleable#View_elevation
 */
ECode View::SetElevation(
    /* [in] */ Float elevation)
{
    Float res;
    GetElevation(&res);
    if (elevation != res) {
        InvalidateViewProperty(TRUE, FALSE);
        Boolean res;
        mRenderNode->SetElevation(elevation, &res);
        InvalidateViewProperty(FALSE, TRUE);

        InvalidateParentIfNeededAndWasQuickRejected();
    }
}

ECode View::GetTranslationX(
    /* [out] */ Float* translationX)
{
    VALIDATE_NOT_NULL(translationX)
    return mRenderNode->GetTranslationX(translationX);
}

ECode View::SetTranslationX(
    /* [in] */ Float translationX)
{
    Float temp;
    GetTranslationX(&temp);
    if (translationX != temp) {
        // Double-invalidation is necessary to capture view's old and new areas
        InvalidateViewProperty(TRUE, FALSE);
        Boolean res;
        mRenderNode->SetTranslationX(translationX, &res);
        InvalidateViewProperty(FALSE, TRUE);

        InvalidateParentIfNeededAndWasQuickRejected();
        NotifySubtreeAccessibilityStateChangedIfNeeded();
    }

    return NOERROR;
}

Float View::GetTranslationY(
    /* [out] */ Float* translationY)
{
    VALIDATE_NOT_NULL(translationY)
    return  mRenderNode->GetTranslationY(translationY);
}

ECode View::SetTranslationY(
    /* [in] */ Float translationY)
{
    Float temp;
    GetTranslationY(&temp)
    if (translationY != temp) {
        InvalidateViewProperty(TRUE, FALSE);
        Boolean res;
        mRenderNode->SetTranslationY(translationY, &res);
        InvalidateViewProperty(FALSE, TRUE);

        InvalidateParentIfNeededAndWasQuickRejected();
    }

    return NOERROR;
}

/**
 * The depth location of this view relative to its {@link #getElevation() elevation}.
 *
 * @return The depth of this view relative to its elevation.
 */
ECode View::GetTranslationZ(
    /* [out] */ Float* translationZ)
{
    VALIDATE_NOT_NULL(translationZ)
    return mRenderNode->GetTranslationZ(translationZ);
}

/**
 * Sets the depth location of this view relative to its {@link #getElevation() elevation}.
 *
 * @attr ref android.R.styleable#View_translationZ
 */
ECode View::SetTranslationZ(
     /* [in] */ Float translationZ)
{
    Float temp;
    GetTranslationZ(&temp);
    if (translationZ != temp) {
        InvalidateViewProperty(TRUE, FALSE);
        Boolean res;
        mRenderNode->SetTranslationZ(translationZ, &res);
        InvalidateViewProperty(FALSE, TRUE);

        InvalidateParentIfNeededAndWasQuickRejected();
    }
}

/** @hide */
ECode View::SetAnimationMatrix(
    /* [in] */ IMatrix* matrix)
{
    InvalidateViewProperty(TRUE, FALSE);
    Boolean res;
    mRenderNode->SetAnimationMatrix(matrix, &res);
    InvalidateViewProperty(FALSE, TRUE);

    InvalidateParentIfNeededAndWasQuickRejected();
}

/**
 * Returns the current StateListAnimator if exists.
 *
 * @return StateListAnimator or null if it does not exists
 * @see    #setStateListAnimator(android.animation.StateListAnimator)
 */
ECode View::GetStateListAnimator(
    /* [out] */ IStateListAnimator** animator)
{
    VALIDATE_NOT_NULL(animator)
    *animator = mStateListAnimator;
    REFCOUNT_ADD(*animator);
    return NOERROR;
}

/**
 * Attaches the provided StateListAnimator to this View.
 * <p>
 * Any previously attached StateListAnimator will be detached.
 *
 * @param stateListAnimator The StateListAnimator to update the view
 * @see {@link android.animation.StateListAnimator}
 */
ECode View::SetStateListAnimator(
    /* [in] */ IStateListAnimator* stateListAnimator)
{
    if (mStateListAnimator == stateListAnimator) {
        return NOERROR;
    }
    if (mStateListAnimator != NULL) {
        mStateListAnimator->SetTarget(NULL);
    }
    mStateListAnimator = stateListAnimator;
    if (stateListAnimator != NULL) {
        stateListAnimator->SetTarget(THIS_PROBE(IView));
        Boolean res;
        IsAttachedToWindow(&res);
        if (res) {
            AutoPtr<ArrayOf<Int32> > temp;
            GetDrawableState((ArrayOf<Int32>**)&temp);
            stateListAnimator->SetState(temp);
        }
    }
    return NOERROR;
}

/**
 * Returns whether the Outline should be used to clip the contents of the View.
 * <p>
 * Note that this flag will only be respected if the View's Outline returns true from
 * {@link Outline#canClip()}.
 *
 * @see #setOutlineProvider(ViewOutlineProvider)
 * @see #setClipToOutline(boolean)
 */
ECode View::GetClipToOutline(
    /* [out] */ Boolean* res)
{
    return mRenderNode->GetClipToOutline(&res);
}

/**
 * Sets whether the View's Outline should be used to clip the contents of the View.
 * <p>
 * Only a single non-rectangular clip can be applied on a View at any time.
 * Circular clips from a {@link ViewAnimationUtils#createCircularReveal(View, int, int, float, float)
 * circular reveal} animation take priority over Outline clipping, and
 * child Outline clipping takes priority over Outline clipping done by a
 * parent.
 * <p>
 * Note that this flag will only be respected if the View's Outline returns true from
 * {@link Outline#canClip()}.
 *
 * @see #setOutlineProvider(ViewOutlineProvider)
 * @see #getClipToOutline()
 */
ECode View::SetClipToOutline(
    /* [in] */ Boolean clipToOutline)
{
    DamageInParent();
    Boolean res;
    GetClipToOutline(&res);
    if (res != clipToOutline) {
        mRenderNode->SetClipToOutline(clipToOutline, &res);
    }
    return NOERROR;
}


void View::SetOutlineProviderFromAttribute(
    /* [in] */ Int32 providerInt)
{
    switch (providerInt) {
        case PROVIDER_BACKGROUND:
            SetOutlineProvider(ViewOutlineProvider::BACKGROUND);
            break;
        case PROVIDER_NONE:
            SetOutlineProvider(NULL);
            break;
        case PROVIDER_BOUNDS:
            SetOutlineProvider(ViewOutlineProvider::BOUNDS);
            break;
        case PROVIDER_PADDED_BOUNDS:
            SetOutlineProvider(ViewOutlineProvider::PADDED_BOUNDS);
            break;
    }
}

/**
 * Sets the {@link ViewOutlineProvider} of the view, which generates the Outline that defines
 * the shape of the shadow it casts, and enables outline clipping.
 * <p>
 * The default ViewOutlineProvider, {@link ViewOutlineProvider#BACKGROUND}, queries the Outline
 * from the View's background drawable, via {@link Drawable#getOutline(Outline)}. Changing the
 * outline provider with this method allows this behavior to be overridden.
 * <p>
 * If the ViewOutlineProvider is null, if querying it for an outline returns false,
 * or if the produced Outline is {@link Outline#isEmpty()}, shadows will not be cast.
 * <p>
 * Only outlines that return true from {@link Outline#canClip()} may be used for clipping.
 *
 * @see #setClipToOutline(boolean)
 * @see #getClipToOutline()
 * @see #getOutlineProvider()
 */
ECode View::SetOutlineProvider(
    /* [in] */ IViewOutlineProvider* provider)
{
    mOutlineProvider = provider;
    InvalidateOutline();
    return NOERROR;
}

/**
 * Returns the current {@link ViewOutlineProvider} of the view, which generates the Outline
 * that defines the shape of the shadow it casts, and enables outline clipping.
 *
 * @see #setOutlineProvider(ViewOutlineProvider)
 */
ECode View::GetOutlineProvider(
    /* [out] */ IViewOutlineProvider** res)
{
    VALIDATE_NOT_NULL(res)
    *res = mOutlineProvider;
    REFCOUNT_ADD(res)
    return NOERROR;
}

/**
 * Called to rebuild this View's Outline from its {@link ViewOutlineProvider outline provider}
 *
 * @see #setOutlineProvider(ViewOutlineProvider)
 */
ECode View::InvalidateOutline()
{
    mPrivateFlags3 |= PFLAG3_OUTLINE_INVALID;

    NotifySubtreeAccessibilityStateChangedIfNeeded();
    InvalidateViewProperty(FALSE, FALSE);
}

/**
 * Internal version of {@link #invalidateOutline()} which invalidates the
 * outline without invalidating the view itself. This is intended to be called from
 * within methods in the View class itself which are the result of the view being
 * invalidated already. For example, when we are drawing the background of a View,
 * we invalidate the outline in case it changed in the meantime, but we do not
 * need to invalidate the view because we're already drawing the background as part
 * of drawing the view in response to an earlier invalidation of the view.
 */
ECode View::RebuildOutline()
{
    // Unattached views ignore this signal, and outline is recomputed in onAttachedToWindow()
    if (mAttachInfo == NULL) return;

    if (mOutlineProvider == NULL) {
        Boolean res;
        // no provider, remove outline
        mRenderNode->SetOutline(NULL, &res);
    } else {
        AutoPtr<IOutline> outline = mAttachInfo->mTmpOutline;
        outline->SetEmpty();
        outline->SetAlpha(1.0f);

        mOutlineProvider->GetOutline(this, outline);
        Boolean res;
        mRenderNode->SetOutline(outline, &res);
    }
}

/**
 * HierarchyViewer only
 *
 * @hide
 */
ECode View::HasShadow(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    return mRenderNode->HasShadow(res);
}


/** @hide */
ECode View::SetRevealClip(
    /* [in] */ Boolean shouldClip,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius)
{
    Boolean res;
    mRenderNode->SetRevealClip(shouldClip, x, y, radius, &res);
    invalidateViewProperty(FALSE, FALSE);
}

/**
 * Hit rectangle in parent's coordinates
 *
 * @param outRect The hit rectangle of the view.
 */
ECode View::GetHitRect(
    /* [in, out] */ IRect* outRect)
{
    assert(outRect != NULL);
    Boolean hasIdentityMatrix;
    HasIdentityMatrix(&hasIdentityMatrix);
    if (hasIdentityMatrix || mAttachInfo == NULL) {
        outRect->Set(mLeft, mTop, mRight, mBottom);
    }
    else {
        AutoPtr<IRectF> tmpRect = mAttachInfo->mTmpTransformRect;
        Int32 width, height;
        GetWidth(&width);
        GetHeight(&height);
        tmpRect->Set(0, 0, width, height);

        AutoPtr<IMatrix> matrix;
        GetMatrix((IMatrix**)&matrix);
        Boolean res;
        matrix->MapRect(tmpRect, &res); // TODO: mRenderNode.mapRect(tmpRect)

        Float l, r, t, b;
        tmpRect->GetLeft(&l);
        tmpRect->GetRight(&r);
        tmpRect->GetTop(&t);
        tmpRect->GetBottom(&b);
        outRect->Set((Int32)(l + mLeft), (Int32)(t + mTop),
                (Int32)(r + mLeft), (Int32)(b + mTop));
    }

    return NOERROR;
}

Boolean View::PointInView(
    /* [in] */ Float localX,
    /* [in] */ Float localY)
{
    return *res = localX >= 0 && localX < (mRight - mLeft)
            && localY >= 0 && localY < (mBottom - mTop);
}

ECode View::PointInView(
    /* [in] */ Float localX,
    /* [in] */ Float localY,
    /* [in] */ Float slop,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = localX >= -slop && localY >= -slop && localX < ((mRight - mLeft) + slop) &&
            localY < ((mBottom - mTop) + slop);
    return NOERROR;
}

/**
 * When a view has focus and the user navigates away from it, the next view is searched for
 * starting from the rectangle filled in by this method.
 *
 * By default, the rectange is the {@link #getDrawingRect})of the view.  However, if your
 * view maintains some idea of internal selection, such as a cursor, or a selected row
 * or column, you should override this method and fill in a more specific rectangle.
 *
 * @param r The rectangle to fill in, in this view's coordinates.
 */
ECode View::GetFocusedRect(
    /* [in, out] */ IRect* outRect)
{
    return GetDrawingRect(outRect);
}

/**
 * If some part of this view is not clipped by any of its parents, then
 * return that area in r in global (root) coordinates. To convert r to local
 * coordinates, offset it by -globalOffset (e.g. r.offset(-globalOffset.x,
 * -globalOffset.y)) If the view is completely clipped or translated out,
 * return FALSE.
 *
 * @param r If TRUE is returned, r holds the global coordinates of the
 *        visible portion of this view.
 * @param globalOffset If TRUE is returned, globalOffset holds the dx,dy
 *        between this view and its root. globalOffet may be NULL.
 * @return TRUE if r is non-empty (i.e. part of the view is visible at the
 *         root level.
 */
Boolean View::GetGlobalVisibleRect(
    /* [in, out] */ IRect* outRect,
    /* [in, out] */ IPoint* outGlobalOffset)
{
    Int32 width = mRight - mLeft;
    Int32 height = mBottom - mTop;
    if (width > 0 && height > 0) {
        outRect->Set(0, 0, width, height);
        if (outGlobalOffset != NULL) {
            outGlobalOffset->Set(-mScrollX, -mScrollY);
        }

        if (mParent == NULL) return TRUE;

        Boolean result;
        mParent->GetChildVisibleRect(IVIEW_PROBE(this),
                outRect, outGlobalOffset, &result);
        return result;
    }
    return FALSE;
}

Boolean View::GetGlobalVisibleRect(
    /* [in, out] */ IRect* outRect)
{
    return GetGlobalVisibleRect(outRect, NULL);
}

Boolean View::GetLocalVisibleRect(
    /* [in, out] */ IRect* outRect)
{
    AutoPtr<IPoint> offset;
    if (mAttachInfo != NULL) {
        offset = mAttachInfo->mPoint;
    }
    else {
        CPoint::New((IPoint**)&offset);
    }

    if (GetGlobalVisibleRect(outRect, offset)) {
        Int32 x, y;
        offset->GetX(&x);
        offset->GetY(&y);
        outRect->Offset(-x, -y); // make r local
        return TRUE;
    }
    return FALSE;
}

/**
 * Offset this view's vertical location by the specified number of pixels.
 *
 * @param offset the number of pixels to offset the view by
 */
ECode View::OffsetTopAndBottom(
    /* [in] */ Int32 offset)
{
    if (offset != 0) {
        Boolean matrixIsIdentity;
        HasIdentityMatrix(&matrixIsIdentity);
        if (matrixIsIdentity) {
            Boolean isHardwareAccelerated;
            IsHardwareAccelerated(&isHardwareAccelerated);
            if (isHardwareAccelerated) {
                InvalidateViewProperty(FALSE, FALSE);
            }
            else {
                AutoPtr<IViewParent> p = mParent;
                if (p != NULL && mAttachInfo != NULL) {
                    AutoPtr<IRect> r = mAttachInfo->mTmpInvalRect;
                    Int32 minTop = 0;
                    Int32 maxBottom = 0;
                    Int32 yLoc = 0;
                    if (offset < 0) {
                        minTop = mTop + offset;
                        maxBottom = mBottom;
                        yLoc = offset;
                    }
                    else {
                        minTop = mTop;
                        maxBottom = mBottom + offset;
                        yLoc = 0;
                    }
                    r->Set(0, yLoc, mRight - mLeft, maxBottom - minTop);
                    p->InvalidateChild(IVIEW_PROBE(this), r);
                }
            }
        }
        else {
            InvalidateViewProperty(FALSE, FALSE);
        }

        mTop += offset;
        mBottom += offset;

        Boolean res;
        mRenderNode->OffsetTopAndBottom(offset, &res);
        Boolean isHardwareAccelerated;
        IsHardwareAccelerated(&isHardwareAccelerated);
        if (isHardwareAccelerated) {
            InvalidateViewProperty(FALSE, FALSE);
        }
        else {
            if (!matrixIsIdentity) {
                InvalidateViewProperty(FALSE, TRUE);
            }
            InvalidateParentIfNeeded();
        }
        NotifySubtreeAccessibilityStateChangedIfNeeded();
    }

    return NOERROR;
}

/**
 * Offset this view's horizontal location by the specified amount of pixels.
 *
 * @param offset the numer of pixels to offset the view by
 */
ECode View::OffsetLeftAndRight(
    /* [in] */ Int32 offset)
{
    if (offset != 0) {
        Boolean matrixIsIdentity;
        HasIdentityMatrix(&matrixIsIdentity);
        if (matrixIsIdentity) {
            Boolean isHardwareAccelerated;
            IsHardwareAccelerated(&isHardwareAccelerated);
            if (isHardwareAccelerated) {
                InvalidateViewProperty(FALSE, FALSE);
            }
            else {
                AutoPtr<IViewParent> p = mParent;
                if (p != NULL && mAttachInfo != NULL) {
                    AutoPtr<IRect> r = mAttachInfo->mTmpInvalRect;
                    Int32 minLeft = 0;
                    Int32 maxRight = 0;
                    if (offset < 0) {
                        minLeft = mLeft + offset;
                        maxRight = mRight;
                    }
                    else {
                        minLeft = mLeft;
                        maxRight = mRight + offset;
                    }
                    r->Set(0, 0, maxRight - minLeft, mBottom - mTop);
                    p->InvalidateChild(IVIEW_PROBE(this), r);
                }
            }
        }
        else {
            InvalidateViewProperty(FALSE, FALSE);
        }

        mLeft += offset;
        mRight += offset;
        Boolean res;
        mRenderNode->OffsetLeftAndRight(offset, &res);
        Boolean isHardwareAccelerated;
        IsHardwareAccelerated(&isHardwareAccelerated);
        if (isHardwareAccelerated) {
            InvalidateViewProperty(FALSE, FALSE);
        }
        else {
            if (!matrixIsIdentity) {
                InvalidateViewProperty(FALSE, TRUE);
            }
            InvalidateParentIfNeeded();
        }
        NotifySubtreeAccessibilityStateChangedIfNeeded();
    }

    return NOERROR;
}

AutoPtr<IViewGroupLayoutParams> View::GetLayoutParams()
{
    return mLayoutParams;
}

ECode View::SetLayoutParams(
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (params == NULL) {
//        throw new NullPointerException("params == NULL");
        return E_NULL_POINTER_EXCEPTION;
    }
    mLayoutParams = params;

    ResolveLayoutParams();
    if (mParent != NULL && VIEWGROUP_PROBE(mParent) != NULL) {
        VIEWGROUP_PROBE(mParent)->OnSetLayoutParams(IVIEW_PROBE(this), params);
    }

    RequestLayout();
    return NOERROR;
}

ECode View::ResolveLayoutParams()
{
    if (mLayoutParams != NULL) {
        mLayoutParams->ResolveLayoutDirection(GetLayoutDirection());
    }

    return NOERROR;
}

/**
 * Set the scrolled position of your view. This will cause a call to
 * {@link #onScrollChanged(int, int, int, int)} and the view will be
 * invalidated.
 * @param x the x position to scroll to
 * @param y the y position to scroll to
 */
ECode View::ScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (mScrollX != x || mScrollY != y) {
        Int32 oldX = mScrollX;
        Int32 oldY = mScrollY;
        mScrollX = x;
        mScrollY = y;

        InvalidateParentCaches();
        OnScrollChanged(mScrollX, mScrollY, oldX, oldY);
        if (!AwakenScrollBars()) {
            PostInvalidateOnAnimation();
        }
    }

    return NOERROR;
}

/**
 * Move the scrolled position of your view. This will cause a call to
 * {@link #onScrollChanged(int, int, int, int)} and the view will be
 * invalidated.
 * @param x the amount of pixels to scroll by horizontally
 * @param y the amount of pixels to scroll by vertically
 */
ECode View::ScrollBy(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return ScrollTo(mScrollX + x, mScrollY + y);
}

/**
 * <p>Trigger the scrollbars to draw. When invoked this method starts an
 * animation to fade the scrollbars out after a default delay. If a subclass
 * provides animated scrolling, the start delay should equal the duration
 * of the scrolling animation.</p>
 *
 * <p>The animation starts only if at least one of the scrollbars is
 * enabled, as specified by {@link #isHorizontalScrollBarEnabled()} and
 * {@link #isVerticalScrollBarEnabled()}. When the animation is started,
 * this method returns TRUE, and FALSE otherwise. If the animation is
 * started, this method calls {@link #invalidate()}; in that case the
 * caller should not call {@link #invalidate()}.</p>
 *
 * <p>This method should be invoked every time a subclass directly updates
 * the scroll parameters.</p>
 *
 * <p>This method is automatically invoked by {@link #scrollBy(int, int)}
 * and {@link #scrollTo(int, int)}.</p>
 *
 * @return TRUE if the animation is played, FALSE otherwise
 *
 * @see #awakenScrollBars(int)
 * @see #scrollBy(int, int)
 * @see #scrollTo(int, int)
 * @see #isHorizontalScrollBarEnabled()
 * @see #isVerticalScrollBarEnabled()
 * @see #setHorizontalScrollBarEnabled(Boolean)
 * @see #setVerticalScrollBarEnabled(Boolean)
 */
Boolean View::AwakenScrollBars()
{
    return mScrollCache != NULL &&
            AwakenScrollBars(mScrollCache->mScrollBarDefaultDelayBeforeFade, TRUE);
}

/**
 * Trigger the scrollbars to draw.
 * This method differs from awakenScrollBars() only in its default duration.
 * initialAwakenScrollBars() will show the scroll bars for longer than
 * usual to give the user more of a chance to notice them.
 *
 * @return TRUE if the animation is played, FALSE otherwise.
 */
Boolean View::InitialAwakenScrollBars()
{
    return mScrollCache != NULL &&
            AwakenScrollBars(mScrollCache->mScrollBarDefaultDelayBeforeFade * 4, TRUE);
}

/**
 * <p>
 * Trigger the scrollbars to draw. When invoked this method starts an
 * animation to fade the scrollbars out after a fixed delay. If a subclass
 * provides animated scrolling, the start delay should equal the duration of
 * the scrolling animation.
 * </p>
 *
 * <p>
 * The animation starts only if at least one of the scrollbars is enabled,
 * as specified by {@link #isHorizontalScrollBarEnabled()} and
 * {@link #isVerticalScrollBarEnabled()}. When the animation is started,
 * this method returns TRUE, and FALSE otherwise. If the animation is
 * started, this method calls {@link #invalidate()}; in that case the caller
 * should not call {@link #invalidate()}.
 * </p>
 *
 * <p>
 * This method should be invoked everytime a subclass directly updates the
 * scroll parameters.
 * </p>
 *
 * @param startDelay the delay, in milliseconds, after which the animation
 *        should start; when the delay is 0, the animation starts
 *        immediately
 * @return TRUE if the animation is played, FALSE otherwise
 *
 * @see #scrollBy(int, int)
 * @see #scrollTo(int, int)
 * @see #isHorizontalScrollBarEnabled()
 * @see #isVerticalScrollBarEnabled()
 * @see #setHorizontalScrollBarEnabled(Boolean)
 * @see #setVerticalScrollBarEnabled(Boolean)
 */
Boolean View::AwakenScrollBars(
    /* [in] */ Int32 startDelay)
{
    return AwakenScrollBars(startDelay, TRUE);
}

/**
 * <p>
 * Trigger the scrollbars to draw. When invoked this method starts an
 * animation to fade the scrollbars out after a fixed delay. If a subclass
 * provides animated scrolling, the start delay should equal the duration of
 * the scrolling animation.
 * </p>
 *
 * <p>
 * The animation starts only if at least one of the scrollbars is enabled,
 * as specified by {@link #isHorizontalScrollBarEnabled()} and
 * {@link #isVerticalScrollBarEnabled()}. When the animation is started,
 * this method returns TRUE, and FALSE otherwise. If the animation is
 * started, this method calls {@link #invalidate()} if the invalidate parameter
 * is set to TRUE; in that case the caller
 * should not call {@link #invalidate()}.
 * </p>
 *
 * <p>
 * This method should be invoked everytime a subclass directly updates the
 * scroll parameters.
 * </p>
 *
 * @param startDelay the delay, in milliseconds, after which the animation
 *        should start; when the delay is 0, the animation starts
 *        immediately
 *
 * @param invalidate Wheter this method should call invalidate
 *
 * @return TRUE if the animation is played, FALSE otherwise
 *
 * @see #scrollBy(int, int)
 * @see #scrollTo(int, int)
 * @see #isHorizontalScrollBarEnabled()
 * @see #isVerticalScrollBarEnabled()
 * @see #setHorizontalScrollBarEnabled(Boolean)
 * @see #setVerticalScrollBarEnabled(Boolean)
 */
Boolean View::AwakenScrollBars(
    /* [in] */ Int32 startDelay,
    /* [in] */ Boolean invalidate)
{
    AutoPtr<ScrollabilityCache> scrollCache = mScrollCache;

    if (scrollCache == NULL || !scrollCache->mFadeScrollBars) {
        return FALSE;
    }

    if (scrollCache->mScrollBar == NULL) {
        CScrollBarDrawable::New((IScrollBarDrawable**)&(scrollCache->mScrollBar));
    }

    if (IsHorizontalScrollBarEnabled() || IsVerticalScrollBarEnabled()) {

        if (invalidate) {
            // Invalidate to show the scrollbars
            Invalidate();
        }

        if (scrollCache->mState == ScrollabilityCache::OFF) {
            // FIXME: this is copied from WindowManagerService.
            // We should get this value from the system when it
            // is possible to do so.
            Int32 KEY_REPEAT_FIRST_DELAY = 750;
            startDelay = Elastos::Core::Math::Max(KEY_REPEAT_FIRST_DELAY, startDelay);
        }

        // Tell mScrollCache when we should start fading. This may
        // extend the fade start time if one was already scheduled
        Int64 fadeStartTime = AnimationUtils::CurrentAnimationTimeMillis() + startDelay;
        scrollCache->mFadeStartTime = fadeStartTime;
        scrollCache->mState = ScrollabilityCache::ON;

        // Schedule our fader to run, unscheduling any old ones first
        if (mAttachInfo != NULL) {
            mAttachInfo->mHandler->RemoveCallbacks(scrollCache);
            Boolean result;
            mAttachInfo->mHandler->PostAtTime(scrollCache, fadeStartTime, &result);
        }

        return TRUE;
    }

    return FALSE;
}

/**
 * Do not invalidate views which are not visible and which are not running an animation. They
 * will not get drawn and they should not set dirty flags as if they will be drawn
 */
Boolean View::SkipInvalidate()
{
    Boolean res = FALSE;
    return (mViewFlags & VISIBILITY_MASK) != IView::VISIBLE && mCurrentAnimation == NULL &&
        (mParent == NULL || IViewGroup::Probe(mParent) == NULL || res);
        //! (IViewGroup::Probe(mParent)->IsViewTransitioning(IVIEW_PROBE(this), &res), res));
}

/**
 * Mark the the area defined by dirty as needing to be drawn. If the view is
 * visible, {@link #onDraw} will be called at some point in the future.
 * This must be called from a UI thread. To call from a non-UI thread, call
 * {@link #postInvalidate()}.
 *
 * WARNING: This method is destructive to dirty.
 * @param dirty the rectangle representing the bounds of the dirty region
 */
ECode View::Invalidate(
    /* [in] */ IRect* dirty)
{
    Int32 scrollX = mScrollX;
    Int32 scrollY = mScrollY;
    CRect* temp = (CRect*)dirty;
    InvalidateInternal(temp->mLeft - scrollX, temp->mTop - scrollY,
            temp->mRight - scrollX, temp->mBottom - scrollY, TRUE, FALSE);
    return NOERROR;
}

ECode View::Invalidate(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    Int32 scrollX = mScrollX;
    Int32 scrollY = mScrollY;
    InvalidateInternal(left - scrollX, top - scrollY, right - scrollX, bottom - scrollY, TRUE, FALSE);
    return NOERROR;
}

ECode View::Invalidate()
{
    Invalidate(TRUE);
    return NOERROR;
}

void View::Invalidate(
    /* [in] */ Boolean invalidateCache)
{
    InvalidateInternal(0, 0, mRight - mLeft, mBottom - mTop, invalidateCache, TRUE);
}

void View::InvalidateInternal(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b,
    /* [in] */ Boolean invalidateCache,
    /* [in] */ Boolean fullInvalidate)
{
    if (mGhostView != NULL) {
        mGhostView->Invalidate(invalidateCache);
        return;
    }

    if (SkipInvalidate()) {
        return;
    }

    Boolean isOpaque;
    IsOpaque(&isOpaque);
    if ((mPrivateFlags & (PFLAG_DRAWN | PFLAG_HAS_BOUNDS)) == (PFLAG_DRAWN | PFLAG_HAS_BOUNDS)
            || (invalidateCache && (mPrivateFlags & PFLAG_DRAWING_CACHE_VALID) == PFLAG_DRAWING_CACHE_VALID)
            || (mPrivateFlags & PFLAG_INVALIDATED) != PFLAG_INVALIDATED
            || (fullInvalidate && isOpaque != mLastIsOpaque)) {
        if (fullInvalidate) {
            IsOpaque(&mLastIsOpaque);
            mPrivateFlags &= ~PFLAG_DRAWN;
        }

        mPrivateFlags |= PFLAG_DIRTY;

        if (invalidateCache) {
            mPrivateFlags |= PFLAG_INVALIDATED;
            mPrivateFlags &= ~PFLAG_DRAWING_CACHE_VALID;
        }

        // Propagate the damage rectangle to the parent view.
        AutoPtr<AttachInfo> ai = mAttachInfo;
        AutoPtr<IViewParent> p = mParent;
        if (p && ai && l < r && t < b) {
            AutoPtr<CRect> damage = ai->mTmpInvalRect;
            damage->Set(l, t, r, b);
            p->InvalidateChild(THIS_PROBE(IView), (IRect*)damage);
        }

        // Damage the entire projection receiver, if necessary.
        Boolean isProjected;
        if (mBackground && (mBackground->IsProjected(&isProjected), isProjected)) {
            AutoPtr<IView> receiver = GetProjectionReceiver();
            if (receiver) {
                receiver->DamageInParent();
            }
        }

        // Damage the entire IsolatedZVolume receiving this view's shadow.
        Boolean isHardwareAccelerated;
        IsHardwareAccelerated(&isHardwareAccelerated);
        Float z;
        GetZ(&z);
        if (isHardwareAccelerated && z != 0) {
            DamageShadowReceiver();
        }
    }
}

/**
 * @return this view's projection receiver, or {@code null} if none exists
 */
AutoPtr<IView> View::GetProjectionReceiver()
{
    AutoPtr<IViewParent> p;
    GetParent((IViewParent**)&p);
    while (p != NULL && IView::Probe(p)) {
        AutoPtr<View> v = (View*)IView::Probe(p);
        if (v->IsProjectionReceiver()) {
            return v;
        }
        p->GetParent(&p);
    }

    return NULL;
}

/**
 * @return whether the view is a projection receiver
 */
Boolean View::IsProjectionReceiver()
{
    return mBackground != NULL;
}

/**
 * Damage area of the screen that can be covered by this View's shadow.
 *
 * This method will guarantee that any changes to shadows cast by a View
 * are damaged on the screen for future redraw.
 */
void View::DamageShadowReceiver()
{
    AutoPtr<AttachInfo> ai = mAttachInfo;
    if (ai != NULL) {
        AutoPtr<IViewParent> p;
        GetParent((IViewParent**)&p);
        if (p != NULL && IViewGroup::Probe(p)) {
            AutoPtr<IView> v = IView::Probe(p);
            v->DamageInParent();
        }
    }
}

void View::InvalidateViewProperty(
    /* [in] */ Boolean invalidateParent,
    /* [in] */ Boolean forceRedraw)
{
    Boolean isHardwareAccelerated, isValid;
    IsHardwareAccelerated(&isHardwareAccelerated);
    mRenderNode->IsValid(&isValid);
    if (!isHardwareAccelerated || !isValid || (mPrivateFlags & PFLAG_DRAW_ANIMATION) != 0) {
            if (invalidateParent) {
                InvalidateParentCaches();
            }
            if (forceRedraw) {
                mPrivateFlags |= PFLAG_DRAWN; // force another invalidation with the new orientation
            }
            Invalidate(FALSE);
        } else {
            DamageInParent();
        }
        Float z;
        GetZ(&z);
        if (isHardwareAccelerated && invalidateParent && z != 0) {
            DamageShadowReceiver();
        }
}

ECode View::DamageInParent()
{
    AutoPtr<AttachInfo> ai = mAttachInfo;
    AutoPtr<IViewParent> p = mParent;
    if (p != NULL && ai != NULL) {
        AutoPtr<CRect> r = ai->mTmpInvalRect;
        r->Set(0, 0, mRight - mLeft, mBottom - mTop);
        if (IViewGroup::Probe(mParent)) {
            (IViewGroup::Probe(mParent))->DamageChild(THIS_PROBE(IView), (IRect*)r);
        } else {
            mParent->InvalidateChild(THIS_PROBE(IView), (IRect*)r);
        }
    }
}

void View::TransformRect(
    /* [in] */ IRect* rect)
{
    AutoPtr<IMatrix> matrix =GetMatrix();
    assert(matrix != NULL);
    Boolean res = FALSE;
    matrix->IsIdentity(&res);

    if (!res) {
        AutoPtr<IRectF> boundingRect = mAttachInfo->mTmpTransformRect;
        boundingRect->Set(rect);

        matrix =GetMatrix();
        matrix->MapRect(boundingRect, &res);
        Float l, r, t, b;
        boundingRect->GetLeft(&l);
        boundingRect->GetRight(&r);
        boundingRect->GetTop(&t);
        boundingRect->GetBottom(&b);
        rect->Set((Int32)Elastos::Core::Math::Floor(l), (Int32)Elastos::Core::Math::Floor(t),
                (Int32)Elastos::Core::Math::Ceil(r), (Int32)Elastos::Core::Math::Ceil(b));
    }
}

void View::InvalidateParentCaches()
{
    if (IView::Probe(mParent) != NULL) {
        VIEW_PROBE(mParent)->mPrivateFlags |= PFLAG_INVALIDATED;
    }
}

void View::InvalidateParentIfNeeded()
{
    if (IsHardwareAccelerated() && IView::Probe(mParent) != NULL) {
        VIEW_PROBE(mParent)->Invalidate(TRUE);
    }
}

/**
 * @hide
 */
void View::InvalidateParentIfNeededAndWasQuickRejected()
{
    if ((mPrivateFlags2 & PFLAG2_VIEW_QUICK_REJECTED) != 0) {
        // View was rejected last time it was drawn by its parent; this may have changed
        InvalidateParentIfNeeded();
    }
}


/**
 * Indicates whether this View is opaque. An opaque View guarantees that it will
 * draw all the pixels overlapping its bounds using a fully opaque color.
 *
 * Subclasses of View should override this method whenever possible to indicate
 * whether an instance is opaque. Opaque Views are treated in a special way by
 * the View hierarchy, possibly allowing it to perform optimizations during
 * invalidate/draw passes.
 *
 * @return True if this View is guaranteed to be fully opaque, FALSE otherwise.
 */
Boolean View::IsOpaque()
{
    return (mPrivateFlags & PFLAG_OPAQUE_MASK) == PFLAG_OPAQUE_MASK &&
            GetFinalAlpha() >= 1.0f;
}

void View::ComputeOpaqueFlags()
{
    // Opaque if:
    //   - Has a background
    //   - Background is opaque
    //   - Doesn't have scrollbars or scrollbars are inside overlay
    if (mBackground != NULL) {
        Int32 opacity;
        mBackground->GetOpacity(&opacity);
        if (opacity == IPixelFormat::OPAQUE) {
            mPrivateFlags |= PFLAG_OPAQUE_BACKGROUND;
        }
        else {
            mPrivateFlags &= ~PFLAG_OPAQUE_BACKGROUND;
        }
    }
    else {
        mPrivateFlags &= ~PFLAG_OPAQUE_BACKGROUND;
    }

    Int32 flags = mViewFlags;
    if (((flags & SCROLLBARS_VERTICAL) == 0 && (flags & SCROLLBARS_HORIZONTAL) == 0) ||
            (flags & SCROLLBARS_STYLE_MASK) == IView::SCROLLBARS_INSIDE_OVERLAY) ||
            (flags & SCROLLBARS_STYLE_MASK) == IView::SCROLLBARS_OUTSIDE_OVERLAY) {
        mPrivateFlags |= PFLAG_OPAQUE_SCROLLBARS;
    }
    else {
        mPrivateFlags &= ~PFLAG_OPAQUE_SCROLLBARS;
    }
}

/**
 * @hide
 */
Boolean View::HasOpaqueScrollbars()
{
    return (mPrivateFlags & PFLAG_OPAQUE_SCROLLBARS) == PFLAG_OPAQUE_SCROLLBARS;
}

/**
 * @return A handler associated with the thread running the View. This
 * handler can be used to pump events in the UI events queue.
 */
AutoPtr<IHandler> View::GetHandler()
{
    if (mAttachInfo != NULL) {
        return mAttachInfo->mHandler;
    }
    return AutoPtr<IHandler>(NULL);
}

/**
 * Gets the view root associated with the View.
 * @return The view root, or NULL if none.
 * @hide
 */
ECode View::GetViewRootImpl(
    /* [out] */ ViewRootImpl** impl)
{
    VALIDATE_NOT_NULL(impl)
    if (mAttachInfo != NULL) {
        *impl = mAttachInfo->mViewRootImpl;
        REFCOUNT_ADD(*impl)
        return NOERROR;
    }
    *impl = NULL;
    return NOERROR;
}

/**
 * Causes the Runnable to be added to the message queue.
 * The runnable will be run on the user interface thread.
 *
 * @param action The Runnable that will be executed.
 *
 * @return Returns TRUE if the Runnable was successfully placed in to the
 *         message queue.  Returns FALSE on failure, usually because the
 *         looper processing the message queue is exiting.
 */
Boolean View::Post(
    /* [in] */ IRunnable* action)
{
    if (mAttachInfo != NULL) {
        Boolean result;
        mAttachInfo->mHandler->Post(action, &result);
        return result;
    }
    // Assume that post will succeed later
    ViewRootImpl::GetRunQueue()->Post(action);
    return TRUE;
}

/**
 * Causes the Runnable to be added to the message queue, to be run
 * after the specified amount of time elapses.
 * The runnable will be run on the user interface thread.
 *
 * @param action The Runnable that will be executed.
 * @param delayMillis The delay (in milliseconds) until the Runnable
 *        will be executed.
 *
 * @return TRUE if the Runnable was successfully placed in to the
 *         message queue.  Returns FALSE on failure, usually because the
 *         looper processing the message queue is exiting.  Note that a
 *         result of TRUE does not mean the Runnable will be processed --
 *         if the looper is quit before the delivery time of the message
 *         occurs then the message will be dropped.
 */
Boolean View::PostDelayed(
    /* [in] */ IRunnable* action,
    /* [in] */ Int64 delayMillis)
{
    if (mAttachInfo != NULL) {
        Boolean result;
        mAttachInfo->mHandler->PostDelayed(action, delayMillis, &result);
        return result;
    }
    // Assume that post will succeed later
    ViewRootImpl::GetRunQueue()->PostDelayed(action, delayMillis);
    return TRUE;
}

void View::PostOnAnimation(
    /* [in] */ IRunnable* action)
{
    if (mAttachInfo != NULL) {
        mAttachInfo->mViewRootImpl->mChoreographer->PostCallback(
            IChoreographer::CALLBACK_ANIMATION, action, NULL);
    }
    else {
        // Assume that post will succeed later
        ViewRootImpl::GetRunQueue()->Post(action);
    }
}

/**
 * <p>Causes the Runnable to execute on the next animation time step,
 * after the specified amount of time elapses.
 * The runnable will be run on the user interface thread.</p>
 *
 * @param action The Runnable that will be executed.
 * @param delayMillis The delay (in milliseconds) until the Runnable
 *        will be executed.
 *
 * @see #postOnAnimation
 * @see #removeCallbacks
 */
void View::PostOnAnimationDelayed(
    /* [in] */ IRunnable* action,
    /* [in] */ Int64 delayMillis)
{
    if (mAttachInfo != NULL) {
        mAttachInfo->mViewRootImpl->mChoreographer->PostCallbackDelayed(
            IChoreographer::CALLBACK_ANIMATION, action, NULL, delayMillis);
    }
    else {
        // Assume that post will succeed later
        ViewRootImpl::GetRunQueue()->PostDelayed(action, delayMillis);
    }
}

Boolean View::RemoveCallbacks(
    /* [in] */ IRunnable* action)
{
    if (action != NULL) {
        if (mAttachInfo != NULL) {
            mAttachInfo->mHandler->RemoveCallbacks(action);

            mAttachInfo->mViewRootImpl->mChoreographer->RemoveCallbacks(
                    IChoreographer::CALLBACK_ANIMATION, action, NULL);
        }
        // Assume that post will succeed later
        ViewRootImpl::GetRunQueue()->RemoveCallbacks(action);
    }
    return TRUE;
}

/**
 * Cause an invalidate to happen on a subsequent cycle through the event loop.
 * Use this to invalidate the View from a non-UI thread.
 *
 * @see #invalidate()
 */
ECode View::PostInvalidate()
{
    PostInvalidateDelayed(0);

    return NOERROR;
}

/**
 * Cause an invalidate of the specified area to happen on a subsequent cycle
 * through the event loop. Use this to invalidate the View from a non-UI thread.
 *
 * @param left The left coordinate of the rectangle to invalidate.
 * @param top The top coordinate of the rectangle to invalidate.
 * @param right The right coordinate of the rectangle to invalidate.
 * @param bottom The bottom coordinate of the rectangle to invalidate.
 *
 * @see #invalidate(int, int, int, int)
 * @see #invalidate(Rect)
 */
ECode View::PostInvalidate(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return PostInvalidateDelayed(0, left, top, right, bottom);
}

/**
 * Cause an invalidate to happen on a subsequent cycle through the event
 * loop. Waits for the specified amount of time.
 *
 * @param delayMilliseconds the duration in milliseconds to delay the
 *         invalidation by
 */
ECode View::PostInvalidateDelayed(
    /* [in] */ Int64 delayMilliseconds)
{
    // We try only with the AttachInfo because there's no point in invalidating
    // if we are not attached to our window
    if (mAttachInfo != NULL) {
        mAttachInfo->mViewRootImpl->DispatchInvalidateDelayed(IVIEW_PROBE(this), delayMilliseconds);
    }
    return NOERROR;
}

/**
 * Cause an invalidate of the specified area to happen on a subsequent cycle
 * through the event loop. Waits for the specified amount of time.
 *
 * @param delayMilliseconds the duration in milliseconds to delay the
 *         invalidation by
 * @param left The left coordinate of the rectangle to invalidate.
 * @param top The top coordinate of the rectangle to invalidate.
 * @param right The right coordinate of the rectangle to invalidate.
 * @param bottom The bottom coordinate of the rectangle to invalidate.
 */
ECode View::PostInvalidateDelayed(
    /* [in] */ Int64 delayMilliseconds,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    // We try only with the AttachInfo because there's no point in invalidating
    // if we are not attached to our window
    if (mAttachInfo != NULL) {
        AutoPtr<AttachInfo::InvalidateInfo> info = AttachInfo::InvalidateInfo::Obtain();
        info->mTarget = IVIEW_PROBE(this);
        info->mLeft = left;
        info->mTop = top;
        info->mRight = right;
        info->mBottom = bottom;

        mAttachInfo->mViewRootImpl->DispatchInvalidateRectDelayed(info, delayMilliseconds);
    }
    return NOERROR;
}

ECode View::PostInvalidateOnAnimation()
{
    // We try only with the AttachInfo because there's no point in invalidating
    // if we are not attached to our window
    if (mAttachInfo != NULL) {
        mAttachInfo->mViewRootImpl->DispatchInvalidateOnAnimation(IVIEW_PROBE(this));
    }

    return NOERROR;
}

ECode View::PostInvalidateOnAnimation(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    // We try only with the AttachInfo because there's no point in invalidating
    // if we are not attached to our window
    if (mAttachInfo != NULL) {
        AutoPtr<AttachInfo::InvalidateInfo> info = AttachInfo::InvalidateInfo::Obtain();
        info->mTarget = IVIEW_PROBE(this);
        info->mLeft = left;
        info->mTop = top;
        info->mRight = right;
        info->mBottom = bottom;

        mAttachInfo->mViewRootImpl->DispatchInvalidateRectOnAnimation(info);
    }

    return NOERROR;
}

void View::PostSendViewScrolledAccessibilityEventCallback()
{
    if (mSendViewScrolledAccessibilityEvent == NULL) {
        mSendViewScrolledAccessibilityEvent = new SendViewScrolledAccessibilityEvent(this);
    }
    if (!mSendViewScrolledAccessibilityEvent->mIsPending) {
        mSendViewScrolledAccessibilityEvent->mIsPending = TRUE;
        PostDelayed(mSendViewScrolledAccessibilityEvent,
                CViewConfiguration::GetSendRecurringAccessibilityEventsInterval());
    }
}

/**
 * Called by a parent to request that a child update its values for mScrollX
 * and mScrollY if necessary. This will typically be done if the child is
 * animating a scroll using a {@link android.widget.Scroller Scroller}
 * object.
 */
ECode View::ComputeScroll()
{
    return NOERROR;
}

/**
 * <p>Indicate whether the horizontal edges are faded when the view is
 * scrolled horizontally.</p>
 *
 * @return TRUE if the horizontal edges should are faded on scroll, FALSE
 *         otherwise
 *
 * @see #setHorizontalFadingEdgeEnabled(Boolean)
 * @attr ref android.R.styleable#View_fadingEdge
 */
Boolean View::IsHorizontalFadingEdgeEnabled()
{
    return (mViewFlags & FADING_EDGE_HORIZONTAL) == FADING_EDGE_HORIZONTAL;
}

/**
 * <p>Define whether the horizontal edges should be faded when this view
 * is scrolled horizontally.</p>
 *
 * @param horizontalFadingEdgeEnabled TRUE if the horizontal edges should
 *                                    be faded when the view is scrolled
 *                                    horizontally
 *
 * @see #isHorizontalFadingEdgeEnabled()
 * @attr ref android.R.styleable#View_fadingEdge
 */
ECode View::SetHorizontalFadingEdgeEnabled(
    /* [in] */ Boolean horizontalFadingEdgeEnabled)
{
    if (IsHorizontalFadingEdgeEnabled() != horizontalFadingEdgeEnabled) {
        if (horizontalFadingEdgeEnabled) {
            InitScrollCache();
        }

        mViewFlags ^= FADING_EDGE_HORIZONTAL;

    }
    return NOERROR;
}

/**
 * <p>Indicate whether the vertical edges are faded when the view is
 * scrolled horizontally.</p>
 *
 * @return TRUE if the vertical edges should are faded on scroll, FALSE
 *         otherwise
 *
 * @see #setVerticalFadingEdgeEnabled(Boolean)
 * @attr ref android.R.styleable#View_fadingEdge
 */
Boolean View::IsVerticalFadingEdgeEnabled()
{
    return (mViewFlags & FADING_EDGE_VERTICAL) == FADING_EDGE_VERTICAL;
}

/**
 * <p>Define whether the vertical edges should be faded when this view
 * is scrolled vertically.</p>
 *
 * @param verticalFadingEdgeEnabled TRUE if the vertical edges should
 *                                  be faded when the view is scrolled
 *                                  vertically
 *
 * @see #isVerticalFadingEdgeEnabled()
 * @attr ref android.R.styleable#View_fadingEdge
 */
ECode View::SetVerticalFadingEdgeEnabled(
    /* [in] */ Boolean verticalFadingEdgeEnabled)
{
    if (IsVerticalFadingEdgeEnabled() != verticalFadingEdgeEnabled) {
        if (verticalFadingEdgeEnabled) {
            InitScrollCache();
        }

        mViewFlags ^= FADING_EDGE_VERTICAL;
    }

    return NOERROR;
}

/**
 * Returns the strength, or intensity, of the top faded edge. The strength is
 * a value between 0.0 (no fade) and 1.0 (full fade). The default implementation
 * returns 0.0 or 1.0 but no value in between.
 *
 * Subclasses should override this method to provide a smoother fade transition
 * when scrolling occurs.
 *
 * @return the intensity of the top fade as a float between 0.0f and 1.0f
 */
Float View::GetTopFadingEdgeStrength()
{
    return ComputeVerticalScrollOffset() > 0 ? 1.0f : 0.0f;
}

/**
 * Returns the strength, or intensity, of the bottom faded edge. The strength is
 * a value between 0.0 (no fade) and 1.0 (full fade). The default implementation
 * returns 0.0 or 1.0 but no value in between.
 *
 * Subclasses should override this method to provide a smoother fade transition
 * when scrolling occurs.
 *
 * @return the intensity of the bottom fade as a float between 0.0f and 1.0f
 */
Float View::GetBottomFadingEdgeStrength()
{
    return ComputeVerticalScrollOffset() + ComputeVerticalScrollExtent() <
            ComputeVerticalScrollRange() ? 1.0f : 0.0f;
}

/**
 * Returns the strength, or intensity, of the left faded edge. The strength is
 * a value between 0.0 (no fade) and 1.0 (full fade). The default implementation
 * returns 0.0 or 1.0 but no value in between.
 *
 * Subclasses should override this method to provide a smoother fade transition
 * when scrolling occurs.
 *
 * @return the intensity of the left fade as a float between 0.0f and 1.0f
 */
Float View::GetLeftFadingEdgeStrength()
{
    return ComputeHorizontalScrollOffset() > 0 ? 1.0f : 0.0f;
}

/**
 * Returns the strength, or intensity, of the right faded edge. The strength is
 * a value between 0.0 (no fade) and 1.0 (full fade). The default implementation
 * returns 0.0 or 1.0 but no value in between.
 *
 * Subclasses should override this method to provide a smoother fade transition
 * when scrolling occurs.
 *
 * @return the intensity of the right fade as a float between 0.0f and 1.0f
 */
Float View::GetRightFadingEdgeStrength()
{
    return ComputeHorizontalScrollOffset() + ComputeHorizontalScrollExtent() <
            ComputeHorizontalScrollRange() ? 1.0f : 0.0f;
}

/**
 * <p>Indicate whether the horizontal scrollbar should be drawn or not. The
 * scrollbar is not drawn by default.</p>
 *
 * @return TRUE if the horizontal scrollbar should be painted, FALSE
 *         otherwise
 *
 * @see #setHorizontalScrollBarEnabled(Boolean)
 */
Boolean View::IsHorizontalScrollBarEnabled()
{
    return (mViewFlags & SCROLLBARS_HORIZONTAL) == SCROLLBARS_HORIZONTAL;
}

/**
 * <p>Define whether the horizontal scrollbar should be drawn or not. The
 * scrollbar is not drawn by default.</p>
 *
 * @param horizontalScrollBarEnabled TRUE if the horizontal scrollbar should
 *                                   be painted
 *
 * @see #isHorizontalScrollBarEnabled()
 */
ECode View::SetHorizontalScrollBarEnabled(
    /* [in] */ Boolean horizontalScrollBarEnabled)
{
    if (IsHorizontalScrollBarEnabled() != horizontalScrollBarEnabled) {
        mViewFlags ^= SCROLLBARS_HORIZONTAL;
        ComputeOpaqueFlags();
        ResolvePadding();
    }

    return NOERROR;
}

/**
 * <p>Indicate whether the vertical scrollbar should be drawn or not. The
 * scrollbar is not drawn by default.</p>
 *
 * @return TRUE if the vertical scrollbar should be painted, FALSE
 *         otherwise
 *
 * @see #setVerticalScrollBarEnabled(Boolean)
 */
Boolean View::IsVerticalScrollBarEnabled()
{
    return (mViewFlags & SCROLLBARS_VERTICAL) == SCROLLBARS_VERTICAL;
}

/**
 * <p>Define whether the vertical scrollbar should be drawn or not. The
 * scrollbar is not drawn by default.</p>
 *
 * @param verticalScrollBarEnabled TRUE if the vertical scrollbar should
 *                                 be painted
 *
 * @see #isVerticalScrollBarEnabled()
 */
ECode View::SetVerticalScrollBarEnabled(
    /* [in] */ Boolean verticalScrollBarEnabled)
{
    if (IsVerticalScrollBarEnabled() != verticalScrollBarEnabled) {
        mViewFlags ^= SCROLLBARS_VERTICAL;
        ComputeOpaqueFlags();
        ResolvePadding();
    }

    return NOERROR;
}

void View::RecomputePadding()
{
    InternalSetPadding(mUserPaddingLeft, mPaddingTop, mUserPaddingRight, mUserPaddingBottom);
}

/**
 * Define whether scrollbars will fade when the view is not scrolling.
 *
 * @param fadeScrollbars wheter to enable fading
 *
 */
ECode View::SetScrollbarFadingEnabled(
    /* [in] */ Boolean fadeScrollbars)
{
    InitScrollCache();
    AutoPtr<ScrollabilityCache> scrollabilityCache = mScrollCache;
    scrollabilityCache->mFadeScrollBars = fadeScrollbars;
    if (fadeScrollbars) {
        scrollabilityCache->mState = ScrollabilityCache::OFF;
    }
    else {
        scrollabilityCache->mState = ScrollabilityCache::ON;
    }

    return NOERROR;
}

/**
 *
 * Returns TRUE if scrollbars will fade when this view is not scrolling
 *
 * @return TRUE if scrollbar fading is enabled
 */
Boolean View::IsScrollbarFadingEnabled()
{
    return mScrollCache != NULL && mScrollCache->mFadeScrollBars;
}

Int32 View::GetScrollBarDefaultDelayBeforeFade()
{
    return mScrollCache == NULL ? CViewConfiguration::GetScrollDefaultDelay() :
            mScrollCache->mScrollBarDefaultDelayBeforeFade;
}

ECode View::SetScrollBarDefaultDelayBeforeFade(
    /* [in] */ Int32 scrollBarDefaultDelayBeforeFade)
{
    GetScrollCache()->mScrollBarDefaultDelayBeforeFade = scrollBarDefaultDelayBeforeFade;
    return NOERROR;
}

Int32 View::GetScrollBarFadeDuration()
{
    return mScrollCache == NULL ? CViewConfiguration::GetScrollBarFadeDuration() :
            mScrollCache->mScrollBarFadeDuration;
}

ECode View::SetScrollBarFadeDuration(
    /* [in] */ Int32 scrollBarFadeDuration)
{
    GetScrollCache()->mScrollBarFadeDuration = scrollBarFadeDuration;
    return NOERROR;
}

Int32 View::GetScrollBarSize()
{
    Int32 scrollBarSize;
    return mScrollCache == NULL ?
        (CViewConfiguration::Get(mContext)->GetScaledScrollBarSize(&scrollBarSize), scrollBarSize)
        : mScrollCache->mScrollBarSize;
}

ECode View::SetScrollBarSize(
    /* [in] */ Int32 scrollBarSize)
{
    GetScrollCache()->mScrollBarSize = scrollBarSize;
    return NOERROR;
}

/**
 * <p>Specify the style of the scrollbars. The scrollbars can be overlaid or
 * inset. When inset, they add to the padding of the view. And the scrollbars
 * can be drawn inside the padding area or on the edge of the view. For example,
 * if a view has a background drawable and you want to draw the scrollbars
 * inside the padding specified by the drawable, you can use
 * SCROLLBARS_INSIDE_OVERLAY or SCROLLBARS_INSIDE_INSET. If you want them to
 * appear at the edge of the view, ignoring the padding, then you can use
 * SCROLLBARS_OUTSIDE_OVERLAY or SCROLLBARS_OUTSIDE_INSET.</p>
 * @param style the style of the scrollbars. Should be one of
 * SCROLLBARS_INSIDE_OVERLAY, SCROLLBARS_INSIDE_INSET,
 * SCROLLBARS_OUTSIDE_OVERLAY or SCROLLBARS_OUTSIDE_INSET.
 * @see #SCROLLBARS_INSIDE_OVERLAY
 * @see #SCROLLBARS_INSIDE_INSET
 * @see #SCROLLBARS_OUTSIDE_OVERLAY
 * @see #SCROLLBARS_OUTSIDE_INSET
 */
ECode View::SetScrollBarStyle(
    /* [in] */ Int32 style)
{
    if (style != (mViewFlags & SCROLLBARS_STYLE_MASK)) {
        mViewFlags = (mViewFlags & ~SCROLLBARS_STYLE_MASK) | (style & SCROLLBARS_STYLE_MASK);
        ComputeOpaqueFlags();
        ResolvePadding();
    }

    return NOERROR;
}

/**
 * <p>Returns the current scrollbar style.</p>
 * @return the current scrollbar style
 * @see #SCROLLBARS_INSIDE_OVERLAY
 * @see #SCROLLBARS_INSIDE_INSET
 * @see #SCROLLBARS_OUTSIDE_OVERLAY
 * @see #SCROLLBARS_OUTSIDE_INSET
 */
Int32 View::GetScrollBarStyle()
{
    return mViewFlags & SCROLLBARS_STYLE_MASK;
}

/**
 * <p>Compute the horizontal range that the horizontal scrollbar
 * represents.</p>
 *
 * <p>The range is expressed in arbitrary units that must be the same as the
 * units used by {@link #computeHorizontalScrollExtent()} and
 * {@link #computeHorizontalScrollOffset()}.</p>
 *
 * <p>The default range is the drawing width of this view.</p>
 *
 * @return the total horizontal range represented by the horizontal
 *         scrollbar
 *
 * @see #computeHorizontalScrollExtent()
 * @see #computeHorizontalScrollOffset()
 * @see android.widget.ScrollBarDrawable
 */
Int32 View::ComputeHorizontalScrollRange()
{
    return GetWidth();
}

/**
 * <p>Compute the horizontal offset of the horizontal scrollbar's thumb
 * within the horizontal range. This value is used to compute the position
 * of the thumb within the scrollbar's track.</p>
 *
 * <p>The range is expressed in arbitrary units that must be the same as the
 * units used by {@link #computeHorizontalScrollRange()} and
 * {@link #computeHorizontalScrollExtent()}.</p>
 *
 * <p>The default offset is the scroll offset of this view.</p>
 *
 * @return the horizontal offset of the scrollbar's thumb
 *
 * @see #computeHorizontalScrollRange()
 * @see #computeHorizontalScrollExtent()
 * @see android.widget.ScrollBarDrawable
 */
Int32 View::ComputeHorizontalScrollOffset()
{
    return mScrollX;
}

/**
 * <p>Compute the horizontal extent of the horizontal scrollbar's thumb
 * within the horizontal range. This value is used to compute the length
 * of the thumb within the scrollbar's track.</p>
 *
 * <p>The range is expressed in arbitrary units that must be the same as the
 * units used by {@link #computeHorizontalScrollRange()} and
 * {@link #computeHorizontalScrollOffset()}.</p>
 *
 * <p>The default extent is the drawing width of this view.</p>
 *
 * @return the horizontal extent of the scrollbar's thumb
 *
 * @see #computeHorizontalScrollRange()
 * @see #computeHorizontalScrollOffset()
 * @see android.widget.ScrollBarDrawable
 */
Int32 View::ComputeHorizontalScrollExtent()
{
    return GetWidth();
}

/**
 * <p>Compute the vertical range that the vertical scrollbar represents.</p>
 *
 * <p>The range is expressed in arbitrary units that must be the same as the
 * units used by {@link #computeVerticalScrollExtent()} and
 * {@link #computeVerticalScrollOffset()}.</p>
 *
 * @return the total vertical range represented by the vertical scrollbar
 *
 * <p>The default range is the drawing height of this view.</p>
 *
 * @see #computeVerticalScrollExtent()
 * @see #computeVerticalScrollOffset()
 * @see android.widget.ScrollBarDrawable
 */
Int32 View::ComputeVerticalScrollRange()
{
    return GetHeight();
}

/**
 * <p>Compute the vertical offset of the vertical scrollbar's thumb
 * within the horizontal range. This value is used to compute the position
 * of the thumb within the scrollbar's track.</p>
 *
 * <p>The range is expressed in arbitrary units that must be the same as the
 * units used by {@link #computeVerticalScrollRange()} and
 * {@link #computeVerticalScrollExtent()}.</p>
 *
 * <p>The default offset is the scroll offset of this view.</p>
 *
 * @return the vertical offset of the scrollbar's thumb
 *
 * @see #computeVerticalScrollRange()
 * @see #computeVerticalScrollExtent()
 * @see android.widget.ScrollBarDrawable
 */
Int32 View::ComputeVerticalScrollOffset()
{
    return mScrollY;
}

/**
 * <p>Compute the vertical extent of the horizontal scrollbar's thumb
 * within the vertical range. This value is used to compute the length
 * of the thumb within the scrollbar's track.</p>
 *
 * <p>The range is expressed in arbitrary units that must be the same as the
 * units used by {@link #computeVerticalScrollRange()} and
 * {@link #computeVerticalScrollOffset()}.</p>
 *
 * <p>The default extent is the drawing height of this view.</p>
 *
 * @return the vertical extent of the scrollbar's thumb
 *
 * @see #computeVerticalScrollRange()
 * @see #computeVerticalScrollOffset()
 * @see android.widget.ScrollBarDrawable
 */
Int32 View::ComputeVerticalScrollExtent()
{
    return GetHeight();
}

Boolean View::CanScrollHorizontally(
    /* [in] */ Int32 direction)
{
    const Int32 offset = ComputeHorizontalScrollOffset();
    const Int32 range = ComputeHorizontalScrollRange() - ComputeHorizontalScrollExtent();
    if (range == 0) return FALSE;
    if (direction < 0) {
        return offset > 0;
    }
    else {
        return offset < range - 1;
    }
}

Boolean View::CanScrollVertically(
    /* [in] */ Int32 direction)
{
    const Int32 offset = ComputeVerticalScrollOffset();
    const Int32 range = ComputeVerticalScrollRange() - ComputeVerticalScrollExtent();
    if (range == 0) return FALSE;
    if (direction < 0) {
        return offset > 0;
    }
    else {
        return offset < range - 1;
    }
}

/**
 * <p>Request the drawing of the horizontal and the vertical scrollbar. The
 * scrollbars are painted only if they have been awakened first.</p>
 *
 * @param canvas the canvas on which to draw the scrollbars
 *
 * @see #awakenScrollBars(Int32)
 */
void View::OnDrawScrollBars(
    /* [in] */ ICanvas* canvas)
{
    // scrollbars are drawn only when the animation is running
    AutoPtr<ScrollabilityCache> cache = mScrollCache;
    if (cache != NULL) {

        Int32 state = cache->mState;

        if (state == ScrollabilityCache::OFF) {
            return;
        }

        Boolean invalidate = FALSE;

        if (state == ScrollabilityCache::FADING) {
            // We're fading -- get our fade interpolation
            if (cache->mInterpolatorValues == NULL) {
                cache->mInterpolatorValues = ArrayOf<Float>::Alloc(1);
            }

            ArrayOf<Float>* values = cache->mInterpolatorValues;

            // Stops the animation if we're done
            InterpolatorResult result;
            cache->mScrollBarInterpolator->TimeToValues(values, &result);
            if (result == InterpolatorResult_FREEZE_END) {
                cache->mState = ScrollabilityCache::OFF;
            }
            else {
                cache->mScrollBar->SetAlpha(Elastos::Core::Math::Round((*values)[0]));
            }

            // This will make the scroll bars inval themselves after
            // drawing. We only want this when we're fading so that
            // we prevent excessive redraws
            invalidate = TRUE;
        }
        else {
            // We're just on -- but we may have been fading before so
            // reset alpha
            cache->mScrollBar->SetAlpha(255);
        }

        Int32 viewFlags = mViewFlags;

        Boolean drawHorizontalScrollBar =
            (viewFlags & SCROLLBARS_HORIZONTAL) == SCROLLBARS_HORIZONTAL;
        Boolean drawVerticalScrollBar =
            (viewFlags & SCROLLBARS_VERTICAL) == SCROLLBARS_VERTICAL
            && !IsVerticalScrollBarHidden();

        if (drawVerticalScrollBar || drawHorizontalScrollBar) {
            Int32 width = mRight - mLeft;
            Int32 height = mBottom - mTop;

            AutoPtr<IScrollBarDrawable> scrollBar = cache->mScrollBar;
            Int32 scrollX = mScrollX;
            Int32 scrollY = mScrollY;
            Int32 inside = (viewFlags & SCROLLBARS_OUTSIDE_MASK) == 0 ? ~0 : 0;

            Int32 left, top, right, bottom;

            if (drawHorizontalScrollBar) {
                Int32 size;
                scrollBar->GetSize(FALSE, &size);
                if (size <= 0) {
                    size = cache->mScrollBarSize;
                }

                scrollBar->SetParameters(ComputeHorizontalScrollRange(),
                                         ComputeHorizontalScrollOffset(),
                                         ComputeHorizontalScrollExtent(), FALSE);
                Int32 verticalScrollBarGap = drawVerticalScrollBar ?
                        GetVerticalScrollbarWidth() : 0;
                top = scrollY + height - size - (mUserPaddingBottom & inside);
                left = scrollX + (mPaddingLeft & inside);
                right = scrollX + width - (mUserPaddingRight & inside) - verticalScrollBarGap;
                bottom = top + size;
                OnDrawHorizontalScrollBar(canvas, scrollBar, left, top, right, bottom);
                if (invalidate) {
                    Invalidate(left, top, right, bottom);
                }
            }

            if (drawVerticalScrollBar) {
                Int32 size;
                scrollBar->GetSize(FALSE, &size);
                if (size <= 0) {
                    size = cache->mScrollBarSize;
                }

                scrollBar->SetParameters(ComputeVerticalScrollRange(),
                                         ComputeVerticalScrollOffset(),
                                         ComputeVerticalScrollExtent(), TRUE);

                Int32 verticalScrollbarPosition = mVerticalScrollbarPosition;
                if (verticalScrollbarPosition == IView::SCROLLBAR_POSITION_DEFAULT) {
                    verticalScrollbarPosition = IsLayoutRtl() ?
                            IView::SCROLLBAR_POSITION_LEFT : IView::SCROLLBAR_POSITION_RIGHT;
                }
                switch (verticalScrollbarPosition) {
                    default:
                    case IView::SCROLLBAR_POSITION_RIGHT:
                        left = scrollX + width - size - (mUserPaddingRight & inside);
                        break;
                    case IView::SCROLLBAR_POSITION_LEFT:
                        left = scrollX + (mUserPaddingLeft & inside);
                        break;
                }

                top = scrollY + (mPaddingTop & inside);
                right = left + size;
                bottom = scrollY + height - (mUserPaddingBottom & inside);
                OnDrawVerticalScrollBar(canvas, scrollBar, left, top, right, bottom);
                if (invalidate) {
                    Invalidate(left, top, right, bottom);
                }
            }
        }
    }
}

/**
 * Override this if the vertical scrollbar needs to be hidden in a subclass, like when
 * FastScroller is visible.
 * @return whether to temporarily hide the vertical scrollbar
 * @hide
 */
Boolean View::IsVerticalScrollBarHidden()
{
    return FALSE;
}

/**
 * <p>Draw the horizontal scrollbar if
 * {@link #isHorizontalScrollBarEnabled()} returns TRUE.</p>
 *
 * @param canvas the canvas on which to draw the scrollbar
 * @param scrollBar the scrollbar's drawable
 *
 * @see #isHorizontalScrollBarEnabled()
 * @see #computeHorizontalScrollRange()
 * @see #computeHorizontalScrollExtent()
 * @see #computeHorizontalScrollOffset()
 * @see android.widget.ScrollBarDrawable
 * @hide
 */
void View::OnDrawHorizontalScrollBar(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IDrawable* scrollBar,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    scrollBar->SetBounds(left, top, right, bottom);
    scrollBar->Draw(canvas);
}

/**
 * <p>Draw the vertical scrollbar if {@link #isVerticalScrollBarEnabled()}
 * returns TRUE.</p>
 *
 * @param canvas the canvas on which to draw the scrollbar
 * @param scrollBar the scrollbar's drawable
 *
 * @see #isVerticalScrollBarEnabled()
 * @see #computeVerticalScrollRange()
 * @see #computeVerticalScrollExtent()
 * @see #computeVerticalScrollOffset()
 * @see android.widget.ScrollBarDrawable
 * @hide
 */
void View::OnDrawVerticalScrollBar(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IDrawable* scrollBar,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    scrollBar->SetBounds(left, top, right, bottom);
    scrollBar->Draw(canvas);
}

/**
 * Implement this to do your drawing.
 *
 * @param canvas the canvas on which the background will be drawn
 */
void View::OnDraw(
    /* [in] */ ICanvas* canvas)
{}

ECode View::AssignParent(
    /* [in] */ IViewParent* parent)
{
    if (mParent == NULL) {
        mParent = parent;
    }
    else if (parent == NULL) {
        mParent = NULL;
    }
    else {
//          throw new RuntimeException("view " + this + " being added, but"
//                  + " it already has a parent");
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

/**
 * This is called when the view is attached to a window.  At this point it
 * has a Surface and will start drawing.  Note that this function is
 * guaranteed to be called before {@link #onDraw(android.graphics.Canvas)},
 * however it may be called any time before the first onDraw -- including
 * before or after {@link #onMeasure(int, int)}.
 *
 * @see #onDetachedFromWindow()
 */
ECode View::OnAttachedToWindow()
{
    if ((mPrivateFlags & PFLAG_REQUEST_TRANSPARENT_REGIONS) != 0) {
        assert(mParent != NULL);
        mParent->RequestTransparentRegion(IVIEW_PROBE(this));
    }
    if ((mPrivateFlags & PFLAG_AWAKEN_SCROLL_BARS_ON_ATTACH) != 0) {
        InitialAwakenScrollBars();
        mPrivateFlags &= ~PFLAG_AWAKEN_SCROLL_BARS_ON_ATTACH;
    }

    mPrivateFlags3 &= ~PFLAG3_IS_LAID_OUT;

    JumpDrawablesToCurrentState();

    ResetSubtreeAccessibilityStateChanged();

    // rebuild, since Outline not maintained while View is detached
    RebuildOutline();

    if (IsFocused()) {
        AutoPtr<IInputMethodManager> imm = CInputMethodManager::PeekInstance();
        imm->FocusIn(IVIEW_PROBE(this));
    }
    return NOERROR;
}

ECode View::ResolveRtlPropertiesIfNeeded(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    if (!NeedRtlPropertiesResolution()) {
        *res = FALSE;
        return NOERROR;
    }

    // Order is important here: LayoutDirection MUST be resolved first
    if (!IsLayoutDirectionResolved()) {
        ResolveLayoutDirection();
        ResolveLayoutParams();
    }
    // ... then we can resolve the others properties depending on the resolved LayoutDirection.
    if (!IsTextDirectionResolved()) {
        ResolveTextDirection();
    }
    if (!IsTextAlignmentResolved()) {
        ResolveTextAlignment();
    }
    if (!IsDrawablesResolved()) {
        ResolveDrawables();
    }
    if (!IsPaddingResolved()) {
        ResolvePadding();
    }
    OnRtlPropertiesChanged(GetLayoutDirection());
    *res = TRUE;
    return NOERROR;
}

ECode View::ResetRtlProperties()
{
    ResetResolvedLayoutDirection();
    ResetResolvedTextDirection();
    ResetResolvedTextAlignment();
    ResetResolvedPadding();
    ResetResolvedDrawables();
    return NOERROR;
}

void View::DispatchScreenStateChanged(
    /* [in] */ Int32 screenState)
{
    OnScreenStateChanged(screenState);
}

ECode View::OnScreenStateChanged(
    /* [in] */ Int32 screenState)
{
    return NOERROR;
}

Boolean View::HasRtlSupport()
{
    AutoPtr<IApplicationInfo> appInfo;
    mContext->GetApplicationInfo((IApplicationInfo**)&appInfo);
    assert(appInfo != NULL);
    Boolean res = FALSE;
    appInfo->HasRtlSupport(&res);
    return res;
}

Boolean View::IsRtlCompatibilityMode()
{
    Int32 targetSdkVersion = 0;

    AutoPtr<IApplicationInfo> appInfo;
    mContext->GetApplicationInfo((IApplicationInfo**)&appInfo);
    assert(appInfo != NULL);
    appInfo->GetTargetSdkVersion(&targetSdkVersion);
    return targetSdkVersion < Build::VERSION_CODES::JELLY_BEAN_MR1 || !HasRtlSupport();
}

Boolean View::NeedRtlPropertiesResolution()
{
    return (mPrivateFlags2 & ALL_RTL_PROPERTIES_RESOLVED) != ALL_RTL_PROPERTIES_RESOLVED;
}

ECode View::OnRtlPropertiesChanged(
    /* [in] */ Int32 layoutDirection)
{
    return NOERROR;
}

ECode View::ResolveLayoutDirection(
    /* [out] */ Boolean* res);
{
    VALIDATE_NOT_NULL(res);
    // Clear any previous layout direction resolution
    mPrivateFlags2 &= ~PFLAG2_LAYOUT_DIRECTION_RESOLVED_MASK;

    if (HasRtlSupport()) {
        // Set resolved depending on layout direction
        switch ((mPrivateFlags2 & PFLAG2_LAYOUT_DIRECTION_MASK) >>
                PFLAG2_LAYOUT_DIRECTION_MASK_SHIFT)
        {
            case IView::LAYOUT_DIRECTION_INHERIT:
                {
                    // We cannot resolve yet. LTR is by default and let the resolution happen again
                    // later to get the correct resolved value
                    Boolean direction;
                    ECode ec = CanResolveLayoutDirection(&direction);
                    if (!direction) {
                        *direction = FALSE;
                        return ec;
                    }

                    // Parent has not yet resolved, LTR is still the default
                    if (!parent->IsLayoutDirectionResolved())  {
                        *direction = FALSE;
                        return NOERROR;
                    }

                    if (parent->GetLayoutDirection() == IView::LAYOUT_DIRECTION_RTL) {
                        mPrivateFlags2 |= PFLAG2_LAYOUT_DIRECTION_RESOLVED_RTL;
                    }
                }
                break;
            case IView::LAYOUT_DIRECTION_RTL:
                mPrivateFlags2 |= PFLAG2_LAYOUT_DIRECTION_RESOLVED_RTL;
                break;
            case IView::LAYOUT_DIRECTION_LOCALE:
                {
                    AutoPtr<ILocaleHelper> helper;
                    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
                    AutoPtr<ILocale> locale;
                    helper->GetDefault((ILocale**)&locale);
                    if(IView::LAYOUT_DIRECTION_RTL ==
                        TextUtils::GetLayoutDirectionFromLocale(locale)) {
                        mPrivateFlags2 |= PFLAG2_LAYOUT_DIRECTION_RESOLVED_RTL;
                    }
                }
                break;
            default:
                // Nothing to do, LTR by default
                break;
        }
    }

    // Set to resolved
    mPrivateFlags2 |= PFLAG2_LAYOUT_DIRECTION_RESOLVED;
    *res = TRUE;
    return NOERROR;
}

ECode View::CanResolveLayoutDirection(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    Int32 direction;
    GetRawLayoutDirection(&direction);
    switch (direction) {
        case IView::LAYOUT_DIRECTION_INHERIT:
            if (mParent != NULL) {
                //try {
                ECode ec = mParent->CanResolveLayoutDirection(res);
                return ec;
                //} catch (AbstractMethodError e) {
                //    Log.e(VIEW_LOG_TAG, mParent.getClass().getSimpleName() +
                //            " does not fully implement ViewParent", e);
                //}
            }
            *res = FALSE;
            return NOERROR;

        default:
            *res = TRUE;
            return NOERROR;
    }
}

ECode View::ResetResolvedLayoutDirection()
{
    // Reset the current resolved bits
    mPrivateFlags2 &= ~PFLAG2_LAYOUT_DIRECTION_RESOLVED_MASK;
    return NOERROR;
}

Boolean View::IsLayoutDirectionInherited()
{
    return (GetRawLayoutDirection() == IView::LAYOUT_DIRECTION_INHERIT);
}

ECode View::IsLayoutDirectionResolved(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = (mPrivateFlags2 & PFLAG2_LAYOUT_DIRECTION_RESOLVED) == PFLAG2_LAYOUT_DIRECTION_RESOLVED;
    return NOERROR;
}

Boolean View::IsPaddingResolved()
{
    return (mPrivateFlags2 & PFLAG2_PADDING_RESOLVED) == PFLAG2_PADDING_RESOLVED;
}

ECode View::ResolvePadding()
{
    Int32 resolvedLayoutDirection;
    GetLayoutDirection(&resolvedLayoutDirection);

    if (!IsRtlCompatibilityMode()) {
        // Post Jelly Bean MR1 case: we need to take the resolved layout direction into account.
        // If start / end padding are defined, they will be resolved (hence overriding) to
        // left / right or right / left depending on the resolved layout direction.
        // If start / end padding are not defined, use the left / right ones.
        if (mBackground != NULL && (!mLeftPaddingDefined || !mRightPaddingDefined)) {
            AutoPtr<CRect> padding = pthread_getspecific(sKey);
            if (padding == NULL) {
                CRect::NewByFriend((CRect**)&padding);
                pthread_setspecific(sKey, padding.Get());
            }
            Boolean res;
            mBackground->GetPadding((IRect*)padding, &res);
            if (!mLeftPaddingDefined) {
                mUserPaddingLeftInitial = padding->mLeft;
            }
            if (!mRightPaddingDefined) {
                mUserPaddingRightInitial = padding->mRight;
            }
        }
        switch (resolvedLayoutDirection) {
            case IView::LAYOUT_DIRECTION_RTL:
                if (mUserPaddingStart != UNDEFINED_PADDING) {
                    mUserPaddingRight = mUserPaddingStart;
                } else {
                    mUserPaddingRight = mUserPaddingRightInitial;
                }
                if (mUserPaddingEnd != UNDEFINED_PADDING) {
                    mUserPaddingLeft = mUserPaddingEnd;
                } else {
                    mUserPaddingLeft = mUserPaddingLeftInitial;
                }
                break;
            case IView::LAYOUT_DIRECTION_LTR:
            default:
                if (mUserPaddingStart != UNDEFINED_PADDING) {
                    mUserPaddingLeft = mUserPaddingStart;
                } else {
                    mUserPaddingLeft = mUserPaddingLeftInitial;
                }
                if (mUserPaddingEnd != UNDEFINED_PADDING) {
                    mUserPaddingRight = mUserPaddingEnd;
                } else {
                    mUserPaddingRight = mUserPaddingRightInitial;
                }
        }

        mUserPaddingBottom = (mUserPaddingBottom >= 0) ? mUserPaddingBottom : mPaddingBottom;
    }
    InternalSetPadding(mUserPaddingLeft, mPaddingTop, mUserPaddingRight, mUserPaddingBottom);
    OnRtlPropertiesChanged(resolvedLayoutDirection);


    mPrivateFlags2 |= PFLAG2_PADDING_RESOLVED;
    return NOERROR;
}

ECode View::ResetResolvedPadding()
{
    mPrivateFlags2 &= ~PFLAG2_PADDING_RESOLVED;
    return NOERROR;
}

ECode View::OnDetachedFromWindow()
{
}

/**
 * This is called when the view is detached from a window.  At this point it
 * no longer has a surface for drawing.
 *
 * @see #onAttachedToWindow()
 */
ECode View::OnDetachedFromWindowInternal()
{
    mPrivateFlags &= ~PFLAG_CANCEL_NEXT_UP_EVENT;
    mPrivateFlags3 &= ~PFLAG3_IS_LAID_OUT;

    RemoveUnsetPressCallback();
    RemoveLongPressCallback();
    RemovePerformClickCallback();
    RemoveSendViewScrolledAccessibilityEventCallback();
    StopNestedScroll();

    // Anything that started animating right before detach should already
    // be in its final state when re-attached.
    JumpDrawablesToCurrentState();

    DestroyDrawingCache();

    CleanupDraw();
    mCurrentAnimation = NULL;
    return NOERROR;
}

void View::CleanupDraw()
{
    ResetDisplayList();
    if (mAttachInfo != NULL) {
        mAttachInfo->mViewRootImpl->CancelInvalidate(THIS_PROBE(IView));
    }
}

void View::InvalidateInheritedLayoutMode(
    /* [in] */ Int32 layoutModeOfRoot)
{
}

/**
 * @return The number of times this view has been attached to a window
 */
Int32 View::GetWindowAttachCount()
{
    return mWindowAttachCount;
}

/**
 * Retrieve a unique token identifying the window this view is attached to.
 * @return Return the window's token for use in
 * {@link WindowManager.LayoutParams#token WindowManager.LayoutParams.token}.
 */
AutoPtr<IBinder> View::GetWindowToken()
{
    return mAttachInfo != NULL ? mAttachInfo->mWindowToken : AutoPtr<IBinder>(NULL);
}

/**
 * Retrieve the {@link WindowId} for the window this view is
 * currently attached to.
 */
ECode View::GetWindowId(
    /* [out] */ IWindowId** id)
{
    VALIDATE_NOT_NULL(id)
    if (mAttachInfo == NULL) {
        *id = NULL;
        return NOERROR;
    }
    if (mAttachInfo->mWindowId == NULL) {
        //try {
        ECode ec = mAttachInfo->mSession->GetWindowId(mAttachInfo->mWindowToken, (IIWindowId**)&mAttachInfo->mIWindowId);
        WindowId::New(mAttachInfo->mIWindowId, (IWindowId**)&mAttachInfo->mWindowId);
        //} catch (RemoteException e) {
        //}
    }
    *id = mAttachInfo->mWindowId;
    return ec;
}

/**
 * Retrieve a unique token identifying the top-level "real" window of
 * the window that this view is attached to.  That is, this is like
 * {@link #getWindowToken}, except if the window this view in is a panel
 * window (attached to another containing window), then the token of
 * the containing window is returned instead.
 *
 * @return Returns the associated window token, either
 * {@link #getWindowToken()} or the containing window's token.
 */
AutoPtr<IBinder> View::GetApplicationWindowToken()
{
    AttachInfo* ai = mAttachInfo;
    if (ai != NULL) {
        AutoPtr<IBinder> appWindowToken = ai->mPanelParentWindowToken;
        if (appWindowToken == NULL) {
            appWindowToken = ai->mWindowToken;
        }
        return appWindowToken;
    }
    return AutoPtr<IBinder>(NULL);
}

AutoPtr<IDisplay> View::GetDisplay()
{
    return mAttachInfo != NULL ? mAttachInfo->mDisplay : NULL;
}

/**
 * Retrieve private session object this view hierarchy is using to
 * communicate with the window manager.
 * @return the session object to communicate with the window manager
 */
AutoPtr<IWindowSession> View::GetWindowSession()
{
    return mAttachInfo != NULL ? mAttachInfo->mSession : AutoPtr<IWindowSession>(NULL);
}

/**
 * @param info the {@link android.view.View.AttachInfo} to associated with
 *        this view
 */
void View::DispatchAttachedToWindow(
    /* [in] */ AttachInfo* info,
    /* [in] */ Int32 visibility)
{
    //System.out.println("Attached! " + this);
    mAttachInfo = info;
    if (mOverlay != NULL) {
        AutoPtr<IViewGroup> group;
        mOverlay->GetOverlayView((IViewGroup**)&group);
        (IView::Probe(group))->DispatchAttachedToWindow(info, visibility);
    }
    mAttachInfo->mViewRootImpl->AddRef();
    mWindowAttachCount++;
    // We will need to evaluate the drawable state at least once.
    mPrivateFlags |= PFLAG_DRAWABLE_STATE_DIRTY;
    if (mFloatingTreeObserver != NULL) {
        info->mTreeObserver->Merge(mFloatingTreeObserver);
        mFloatingTreeObserver = NULL;
    }
    if ((mPrivateFlags & PFLAG_SCROLL_CONTAINER) != 0) {
        mAttachInfo->mScrollContainers.PushBack(this);
        mPrivateFlags |= PFLAG_SCROLL_CONTAINER_ADDED;
    }

    PerformCollectViewAttributes(mAttachInfo, visibility);

    OnAttachedToWindow();

    AutoPtr<ListenerInfo> li = mListenerInfo;
    if (li != NULL) {
        List<AutoPtr<IViewOnAttachStateChangeListener> > listeners(li->mOnAttachStateChangeListeners);
        if (!listeners.IsEmpty()) {
            // NOTE: because of the use of CopyOnWriteArrayList, we *must* use an iterator to
            // perform the dispatching. The iterator is a safe guard against listeners that
            // could mutate the list by calling the various add/remove methods. This prevents
            // the array from being modified while we iterate it.
            List<AutoPtr<IViewOnAttachStateChangeListener> >::Iterator iter;
            for (iter = listeners.Begin(); iter != listeners.End(); ++iter) {
                (*iter)->OnViewAttachedToWindow(IVIEW_PROBE(this));
            }
        }
    }

    Int32 vis = info->mWindowVisibility;
    if (vis != IView::GONE) {
        OnWindowVisibilityChanged(vis);
    }
    if ((mPrivateFlags&PFLAG_DRAWABLE_STATE_DIRTY) != 0) {
        // If nobody has evaluated the drawable state yet, then do it now.
        RefreshDrawableState();
    }
    NeedGlobalAttributesUpdate(FALSE);
}

ECode View::DispatchDetachedFromWindow()
{
    AttachInfo* info = mAttachInfo;
    if (info != NULL) {
        Int32 vis = info->mWindowVisibility;
        if (vis != IView::GONE) {
            OnWindowVisibilityChanged(IView::GONE);
        }
    }

    OnDetachedFromWindow();
    OnDetachedFromWindowInternal();

    AutoPtr<ListenerInfo> li = mListenerInfo;
    if (li != NULL) {
        List<AutoPtr<IViewOnAttachStateChangeListener> > listeners(li->mOnAttachStateChangeListeners);
        if (!listeners.IsEmpty()) {
            // NOTE: because of the use of CopyOnWriteArrayList, we *must* use an iterator to
            // perform the dispatching. The iterator is a safe guard against listeners that
            // could mutate the list by calling the various add/remove methods. This prevents
            // the array from being modified while we iterate it.
            List<AutoPtr<IViewOnAttachStateChangeListener> >::Iterator iter;
            for (iter = listeners.Begin(); iter != listeners.End(); ++iter) {
                (*iter)->OnViewDetachedFromWindow(IVIEW_PROBE(this));
            }
        }
    }

    if ((mPrivateFlags & PFLAG_SCROLL_CONTAINER_ADDED) != 0) {
        mAttachInfo->mScrollContainers.Remove(this);
        mPrivateFlags &= ~PFLAG_SCROLL_CONTAINER_ADDED;
    }
    mAttachInfo->mViewRootImpl->Release();
    if (mAttachInfo != NULL && mScrollCache != NULL) {
        mAttachInfo->mHandler->RemoveCallbacks(mScrollCache);
    }
    mAttachInfo = NULL;

    if (mOverlay != NULL) {
        AutoPtr<IViewGroup> group;
        mOverlay->GetOverlayView((IViewGroup**)&group);
        (IView::Probe(group))->DispatchDetachedFromWindow();
    }

    return NOERROR;
}

/**
 * Cancel any deferred high-level input events that were previously posted to the event queue.
 *
 * <p>Many views post high-level events such as click handlers to the event queue
 * to run deferred in order to preserve a desired user experience - clearing visible
 * pressed states before executing, etc. This method will abort any events of this nature
 * that are currently in flight.</p>
 *
 * <p>Custom views that generate their own high-level deferred input events should override
 * {@link #onCancelPendingInputEvents()} and remove those pending events from the queue.</p>
 *
 * <p>This will also cancel pending input events for any child views.</p>
 *
 * <p>Note that this may not be sufficient as a debouncing strategy for clicks in all cases.
 * This will not impact newer events posted after this call that may occur as a result of
 * lower-level input events still waiting in the queue. If you are trying to prevent
 * double-submitted  events for the duration of some sort of asynchronous transaction
 * you should also take other steps to protect against unexpected double inputs e.g. calling
 * {@link #setEnabled(boolean) setEnabled(false)} and re-enabling the view when
 * the transaction completes, tracking already submitted transaction IDs, etc.</p>
 */
ECode View::CancelPendingInputEvents()
{
    DispatchCancelPendingInputEvents();
    return NOERROR;
}

/**
 * Called by {@link #cancelPendingInputEvents()} to cancel input events in flight.
 * Overridden by ViewGroup to dispatch. Package scoped to prevent app-side meddling.
 */
ECode View::DispatchCancelPendingInputEvents()
{
    mPrivateFlags3 &= ~PFLAG3_CALLED_SUPER;
    OnCancelPendingInputEvents();
    if ((mPrivateFlags3 & PFLAG3_CALLED_SUPER) != PFLAG3_CALLED_SUPER) {
        return E_SUPER_NOT_CALLED_EXCEPTION;
        //throw new SuperNotCalledException("View " + getClass().getSimpleName() +
        //        " did not call through to super.onCancelPendingInputEvents()");
    }
    return NOERROR;
}

/**
 * Called as the result of a call to {@link #cancelPendingInputEvents()} on this view or
 * a parent view.
 *
 * <p>This method is responsible for removing any pending high-level input events that were
 * posted to the event queue to run later. Custom view classes that post their own deferred
 * high-level events via {@link #post(Runnable)}, {@link #postDelayed(Runnable, long)} or
 * {@link android.os.Handler} should override this method, call
 * <code>super.onCancelPendingInputEvents()</code> and remove those callbacks as appropriate.
 * </p>
 */
ECode View::OnCancelPendingInputEvents()
{
    RemovePerformClickCallback();
    CancelLongPress();
    mPrivateFlags3 |= PFLAG3_CALLED_SUPER;
}

/**
 * Store this view hierarchy's frozen state into the given container.
 *
 * @param container The SparseArray in which to save the view's state.
 *
 * @see #restoreHierarchyState
 * @see #dispatchSaveInstanceState
 * @see #onSaveInstanceState
 */
ECode View::SaveHierarchyState(
    /* [in, out] */ IObjectInt32Map* container)
{
    return DispatchSaveInstanceState(container);
}

/**
 * Called by {@link #saveHierarchyState(android.util.SparseArray)} to store the state for
 * this view and its children. May be overridden to modify how freezing happens to a
 * view's children; for example, some views may want to not store state for their children.
 *
 * @param container The SparseArray in which to save the view's state.
 *
 * @see #dispatchRestoreInstanceState(android.util.SparseArray)
 * @see #saveHierarchyState(android.util.SparseArray)
 * @see #onSaveInstanceState()
 */
ECode View::DispatchSaveInstanceState(
    /* [in, out] */ IObjectInt32Map* container)
{
    if (mID != IView::NO_ID && (mViewFlags & SAVE_DISABLED_MASK) == 0) {
        mPrivateFlags &= ~PFLAG_SAVE_STATE_CALLED;
        AutoPtr<IParcelable> state = OnSaveInstanceState();
        if ((mPrivateFlags & PFLAG_SAVE_STATE_CALLED) == 0) {
            //throw new IllegalStateException(
              //      "Derived class did not call super.onSaveInstanceState()");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        if (state != NULL) {
            // Log.i(VIEW_LOG_TAG, "Freezing #" + Integer.toHexString(mID)
            // + ": " + state);
            container->Put(mID, state.Get());
        }
    }
    return NOERROR;
}

/**
 * Hook allowing a view to generate a representation of its internal state
 * that can later be used to create a new instance with that same state.
 * This state should only contain information that is not persistent or can
 * not be reconstructed later. For example, you will never store your
 * current position on screen because that will be computed again when a
 * new instance of the view is placed in its view hierarchy.
 * <p>
 * Some examples of things you may store here: the current cursor position
 * in a text view (but usually not the text itself since that is stored in a
 * content provider or other persistent storage), the currently selected
 * item in a list view.
 *
 * @return Returns a Parcelable object containing the view's current dynamic
 *         state, or NULL if there is nothing interesting to save. The
 *         default implementation returns NULL.
 * @see #onRestoreInstanceState(android.os.Parcelable)
 * @see #saveHierarchyState(android.util.SparseArray)
 * @see #dispatchSaveInstanceState(android.util.SparseArray)
 * @see #setSaveEnabled(boolean)
 */
AutoPtr<IParcelable> View::OnSaveInstanceState()
{
    //mPrivateFlags |= PFLAG_SAVE_STATE_CALLED;
    //return BaseSavedState::EMPTY_STATE;
    return AutoPtr<IParcelable>(NULL);
}

/**
 * Restore this view hierarchy's frozen state from the given container.
 *
 * @param container The SparseArray which holds previously frozen states.
 *
 * @see #saveHierarchyState
 * @see #dispatchRestoreInstanceState
 * @see #onRestoreInstanceState
 */
ECode View::RestoreHierarchyState(
    /* [in] */ IObjectInt32Map* container)
{
    return DispatchRestoreInstanceState(container);
}

/**
 * Called by {@link #restoreHierarchyState(android.util.SparseArray)} to retrieve the
 * state for this view and its children. May be overridden to modify how restoring
 * happens to a view's children; for example, some views may want to not store state
 * for their children.
 *
 * @param container The SparseArray which holds previously saved state.
 *
 * @see #dispatchSaveInstanceState(android.util.SparseArray)
 * @see #restoreHierarchyState(android.util.SparseArray)
 * @see #onRestoreInstanceState(android.os.Parcelable)
 */
ECode View::DispatchRestoreInstanceState(
    /* [in] */ IObjectInt32Map* container)
{
    if (mID != IView::NO_ID) {
        AutoPtr<IParcelable> state;
        container->Get(mID, (IInterface**)&state);
        if (state != NULL) {
            // Log.i(VIEW_LOG_TAG, "Restoreing #" + Integer.toHexString(mID)
            // + ": " + state);
            mPrivateFlags &= ~PFLAG_SAVE_STATE_CALLED;
            OnRestoreInstanceState(state);
            if ((mPrivateFlags & PFLAG_SAVE_STATE_CALLED) == 0) {
                //throw new IllegalStateException(
                  //      "Derived class did not call super.onRestoreInstanceState()");
                return E_ILLEGAL_STATE_EXCEPTION;
            }
        }
    }
    return NOERROR;
}

/**
 * Hook allowing a view to re-apply a representation of its internal state that had previously
 * been generated by {@link #onSaveInstanceState}. This function will never be called with a
 * NULL state.
 *
 * @param state The frozen state that had previously been returned by
 *        {@link #onSaveInstanceState}.
 *
 * @see #onSaveInstanceState()
 * @see #restoreHierarchyState(android.util.SparseArray)
 * @see #dispatchRestoreInstanceState(android.util.SparseArray)
 */
void View::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    mPrivateFlags |= PFLAG_SAVE_STATE_CALLED;
    //if (state != BaseSavedState::EMPTY_STATE && state != NULL) {
        /*throw new IllegalArgumentException("Wrong state class, expecting View State but "
                + "received " + state.getClass().toString() + " instead. This usually happens "
                + "when two views of different type have the same id in the same hierarchy. "
                + "This view's id is " + ViewDebug.resolveId(mContext, getId()) + ". Make sure "
                + "other views do not use the same id.");*/
    //}
}

/**
 * <p>Return the time at which the drawing of the view hierarchy started.</p>
 *
 * @return the drawing start time in milliseconds
 */
Int64 View::GetDrawingTime()
{
    return mAttachInfo != NULL ? mAttachInfo->mDrawingTime : 0;
}

/**
 * <p>Enables or disables the duplication of the parent's state into this view. When
 * duplication is enabled, this view gets its drawable state from its parent rather
 * than from its own internal properties.</p>
 *
 * <p>Note: in the current implementation, setting this property to TRUE after the
 * view was added to a ViewGroup might have no effect at all. This property should
 * always be used from XML or set to TRUE before adding this view to a ViewGroup.</p>
 *
 * <p>Note: if this view's parent addStateFromChildren property is enabled and this
 * property is enabled, an exception will be thrown.</p>
 *
 * @param enabled True to enable duplication of the parent's drawable state, FALSE
 *                to disable it.
 *
 * @see #getDrawableState()
 * @see #isDuplicateParentStateEnabled()
 */
ECode View::SetDuplicateParentStateEnabled(
    /* [in] */ Boolean enabled)
{
    SetFlags(enabled ? DUPLICATE_PARENT_STATE : 0, DUPLICATE_PARENT_STATE);

    return NOERROR;
}

Boolean View::IsDuplicateParentStateEnabled()
{
    return (mViewFlags & DUPLICATE_PARENT_STATE) == DUPLICATE_PARENT_STATE;
}

ECode View::SetLayerType(
    /* [in] */ Int32 layerType,
    /* [in] */ IPaint* paint)
{
    if (layerType < IView::LAYER_TYPE_NONE || layerType > IView::LAYER_TYPE_HARDWARE) {
        assert(0);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalArgumentException("Layer type can only be one of: LAYER_TYPE_NONE, "
        //         + "LAYER_TYPE_SOFTWARE or LAYER_TYPE_HARDWARE");
    }

    boolean typeChanged;
    mRenderNode->SetLayerType(layerType, &typeChanged);

    if (!typeChanged) {
        SetLayerPaint(paint);
        return NOERROR;
    }

    // Destroy any previous software drawing cache if needed
    if (mLayerType == IView::LAYER_TYPE_SOFTWARE) {
        DestroyDrawingCache();
    }

    mLayerType = layerType;
    Boolean layerDisabled = (mLayerType == IView::LAYER_TYPE_NONE);
    if (!layerDisabled) {
        if (paint == NULL) {
            CPaint::New((IPaint**)&mLayerPaint);
        } else {
            mLayerPaint = paint;
        }

    }
    Boolean res;
    mRenderNode->SetLayerPaint(mLayerPaint, &res);

    InvalidateParentCaches();
    Invalidate(TRUE);
    return NOERROR;
}

ECode View::SetLayerPaint(
    /* [in] */ IPaint* paint)
{
    Int32 layerType = GetLayerType();
    if (layerType != IView::LAYER_TYPE_NONE) {
        if (paint == NULL) {
            mLayerPaint = NULL;
            CPaint::New((IPaint**)&mLayerPaint);
        }
        else {
            mLayerPaint = paint;
        }

        if (layerType == IView::LAYER_TYPE_HARDWARE) {
            Boolean res;
            mRenderNode->SetLayerPaint(mLayerPaint, &res);
            if (res) {
                    InvalidateViewProperty(FALSE, FALSE);
                }
        }
        else {
            Invalidate();
        }
    }

    return NOERROR;
}

Boolean View::HasStaticLayer()
{
    return TRUE;
}

Int32 View::GetLayerType()
{
    return mLayerType;
}

ECode View::BuildLayer()
{
    if (mLayerType == IView::LAYER_TYPE_NONE) return NOERROR;

    if (mAttachInfo == NULL) {
        assert(0);
        return E_ILLEGAL_STATE_EXCEPTION;
        //throw new IllegalStateException("This view must be attached to a window first");
    }
    Int32 width, height;
    GetWidth(&width);
    GetHeight(&height);
    if (width == 0 || height == 0) {
        return NOERROR;
    }

    switch (mLayerType) {
        case IView::LAYER_TYPE_HARDWARE:
            UpdateDisplayListIfDirty();
            Boolean isValid;
            mRenderNode->IsValid(&isValid);
            if (mAttachInfo->mHardwareRenderer != NULL && isValid) {
                mAttachInfo->mHardwareRenderer->BuildLayer(mRenderNode);
            }
            break;
        case IView::LAYER_TYPE_SOFTWARE:
            BuildDrawingCache(TRUE);
            break;
    }

    return NOERROR;
}

AutoPtr<IHardwareLayer> View::GetHardwareLayer()
{
    return NULL;
}

void View::DestroyHardwareResources()
{
    // Although the Layer will be destroyed by RenderNode, we want to release
    // the staging display list, which is also a signal to RenderNode that it's
    // safe to free its copy of the display list as it knows that we will
    // push an updated DisplayList if we try to draw again
    ResetDisplayList();
}

/**
 * <p>Enables or disables the drawing cache. When the drawing cache is enabled, the next call
 * to {@link #getDrawingCache()} or {@link #buildDrawingCache()} will draw the view in a
 * bitmap. Calling {@link #draw(android.graphics.Canvas)} will not draw from the cache when
 * the cache is enabled. To benefit from the cache, you must request the drawing cache by
 * calling {@link #getDrawingCache()} and draw it on screen if the returned bitmap is not
 * NULL.</p>
 *
 * @param enabled TRUE to enable the drawing cache, FALSE otherwise
 *
 * @see #isDrawingCacheEnabled()
 * @see #getDrawingCache()
 * @see #buildDrawingCache()
 */
ECode View::SetDrawingCacheEnabled(
    /* [in] */ Boolean enabled)
{
    mCachingFailed = FALSE;
    SetFlags(enabled ? DRAWING_CACHE_ENABLED : 0, DRAWING_CACHE_ENABLED);

    return NOERROR;
}

/**
 * <p>Indicates whether the drawing cache is enabled for this view.</p>
 *
 * @return TRUE if the drawing cache is enabled
 *
 * @see #setDrawingCacheEnabled(Boolean)
 * @see #getDrawingCache()
 */
Boolean View::IsDrawingCacheEnabled()
{
    return (mViewFlags & DRAWING_CACHE_ENABLED) == DRAWING_CACHE_ENABLED;
}

/**
 * Debugging utility which recursively outputs the dirty state of a view and its
 * descendants.
 *
 * @hide
 */
// @SuppressWarnings({"UnusedDeclaration"})
// public void OutputDirtyFlags(String indent, boolean clear, int clearMask) {
//     Log.d(VIEW_LOG_TAG, indent + this + "             DIRTY(" + (mPrivateFlags & View.PFLAG_DIRTY_MASK) +
//             ") DRAWN(" + (mPrivateFlags & PFLAG_DRAWN) + ")" + " CACHE_VALID(" +
//             (mPrivateFlags & View.PFLAG_DRAWING_CACHE_VALID) +
//             ") INVALIDATED(" + (mPrivateFlags & PFLAG_INVALIDATED) + ")");
//     if (clear) {
//         mPrivateFlags &= clearMask;
//     }
//     if (this instanceof ViewGroup) {
//         ViewGroup parent = (ViewGroup) this;
//         final int count = parent.getChildCount();
//         for (int i = 0; i < count; i++) {
//             final View child = parent.getChildAt(i);
//             child.outputDirtyFlags(indent + "  ", clear, clearMask);
//         }
//     }
// }

void View::DispatchGetDisplayList()
{
}

Boolean View::CanHaveDisplayList()
{
    return !(mAttachInfo == NULL || mAttachInfo->mHardwareRenderer == NULL);
}

ECode View::UpdateDisplayListIfDirty()
{
    AutoPtr<IRenderNode> renderNode = mRenderNode;
    Boolean canHaveDisplayList;
    CanHaveDisplayList(&canHaveDisplayList);
    if (!canHaveDisplayList {
        // can't populate RenderNode, don't try
        return NOERROR;
    }
    Boolean isValid;
    renderNode->IsValid(&isValid);
    if ((mPrivateFlags & PFLAG_DRAWING_CACHE_VALID) == 0
            || ! isValid || (mRecreateDisplayList)) {
        // Don't need to recreate the display list, just need to tell our
        // children to restore/recreate theirs
        if (isValid && !mRecreateDisplayList) {
            mPrivateFlags |= PFLAG_DRAWN | PFLAG_DRAWING_CACHE_VALID;
            mPrivateFlags &= ~PFLAG_DIRTY_MASK;
            DispatchGetDisplayList();

            return NOERROR; // no work needed
        }

        // If we got here, we're recreating it. Mark it as such to ensure that
        // we copy in child display lists into ours in drawChild()
        mRecreateDisplayList = TRUE;

        Int32 width = mRight - mLeft;
        Int32 height = mBottom - mTop;
        Int32 layerType;
        GetLayerType(&layerType);

        AutoPtr<IHardwareCanvas> canvas;
        renderNode->Start(width, height, (IHardwareCanvas**)&canvas);
        canvas->SetHighContrastText(mAttachInfo->mHighContrastText);

        //try {
        AutoPtr<IHardwareLayer> layer;
        GetHardwareLayer((IHardwareLayer**)&layer);
        Boolean layerIsValid;
        layer->IsValid(&layerIsValid);
        if (layer != NULL && layerIsValid) {
            canvas->DrawHardwareLayer(layer, 0, 0, mLayerPaint);
        } else if (layerType == IView::LAYER_TYPE_SOFTWARE) {
            BuildDrawingCache(TRUE);
            AutoPtr<IBitmap> cache;
            GetDrawingCache(TRUE, (IBitmap**)&cache);
            if (cache != NULL) {
                canvas->DrawBitmap(cache, 0, 0, mLayerPaint);
            }
        } else {
            ComputeScroll();

            canvas->Translate(-mScrollX, -mScrollY);
            mPrivateFlags |= PFLAG_DRAWN | PFLAG_DRAWING_CACHE_VALID;
            mPrivateFlags &= ~PFLAG_DIRTY_MASK;

            // Fast path for layouts with no backgrounds
            if ((mPrivateFlags & PFLAG_SKIP_DRAW) == PFLAG_SKIP_DRAW) {
                DispatchDraw(canvas);
                Boolean isEmpty;
                if (mOverlay != NULL && (mOverlay->IsEmpty(&isEmpty), !isEmpty)) {
                    AutoPtr<IViewGroup> group;
                    mOverlay->GetOverlayView((IViewGroup**)&group);
                    (IView::Probe(group))->Draw(canvas);
                }
            } else {
                Draw(canvas);
            }
            DrawAccessibilityFocus(canvas);
        }
        //} finally {
        renderNode->End(canvas);
        SetDisplayListProperties(renderNode);
        //}
    } else {
        mPrivateFlags |= PFLAG_DRAWN | PFLAG_DRAWING_CACHE_VALID;
        mPrivateFlags &= ~PFLAG_DIRTY_MASK;
    }
}

ECode View::GetHardwareRenderer(
    /* [out] */ HardwareRenderer** res);
{
    VALIDATE_NOT_NULL(res)
    if (mAttachInfo != NULL) {
        *res = mAttachInfo->mHardwareRenderer;
        REFCOUNT_ADD(*res)
    return NOERROR;
    }
    *res = NULL
    return NOERROR;
}

AutoPtr<IDisplayList> View::GetDisplayList(
    /* [in] */ IDisplayList* displayList,
    /* [in] */ Boolean isLayer)
{
    return NULL;
    // if (!CanHaveDisplayList()) {
    //     return NULL;
    // }

    // Boolean valid = FALSE;
    // assert(displayList != NULL);
    // displayList->IsValid(&valid);
    // if (((mPrivateFlags & PFLAG_DRAWING_CACHE_VALID) == 0 ||
    //         displayList == NULL || !valid ||
    //         (!isLayer && mRecreateDisplayList))) {
    //     // Don't need to recreate the display list, just need to tell our
    //     // children to restore/recreate theirs
    //     if (displayList != NULL && valid &&
    //             !isLayer && !mRecreateDisplayList) {
    //         mPrivateFlags |= PFLAG_DRAWN | PFLAG_DRAWING_CACHE_VALID;
    //         mPrivateFlags &= ~PFLAG_DIRTY_MASK;
    //         DispatchGetDisplayList();

    //         return displayList;
    //     }

    //     if (!isLayer) {
    //         // If we got here, we're recreating it. Mark it as such to ensure that
    //         // we copy in child display lists into ours in drawChild()
    //         mRecreateDisplayList = TRUE;
    //     }
    //     if (displayList == NULL) {
    //         assert(0);
    //         //TODO
    //         // final String name = getClass().getSimpleName();
    //         //displayList = mAttachInfo->mHardwareRenderer->CreateDisplayList(name);
    //         // If we're creating a new display list, make sure our parent gets invalidated
    //         // since they will need to recreate their display list to account for this
    //         // new child display list.
    //         InvalidateParentCaches();
    //     }

    //     Boolean caching = FALSE;
    //     AutoPtr<IHardwareCanvas> canvas;
    //     displayList->Start((IHardwareCanvas**)&canvas);
    //     Int32 width = mRight - mLeft;
    //     Int32 height = mBottom - mTop;

    //     canvas->SetViewport(width, height);
    //     // The dirty rect should always be NULL for a display list
    //     canvas->OnPreDraw(NULL);
    //     Int32 layerType = GetLayerType();
    //     if (!isLayer && layerType != IView::LAYER_TYPE_NONE) {
    //         if (layerType == IView::LAYER_TYPE_HARDWARE) {
    //             AutoPtr<IHardwareLayer> layer = GetHardwareLayer();

    //             Boolean tmp = FALSE;
    //             if (layer != NULL && (layer->IsValid(&tmp), tmp)) {
    //                 canvas->DrawHardwareLayer(layer, 0, 0, mLayerPaint);
    //             }
    //             else {
    //                 canvas->SaveLayer(0, 0, mRight - mLeft, mBottom - mTop, mLayerPaint,
    //                         Canvas.HAS_ALPHA_LAYER_SAVE_FLAG |
    //                                 Canvas.CLIP_TO_LAYER_SAVE_FLAG);
    //             }
    //             caching = TRUE;
    //         }
    //         else {
    //             BuildDrawingCache(TRUE);
    //             AutoPtr<IBitmap> cache = GetDrawingCache(TRUE);
    //             if (cache != NULL) {
    //                 canvas->DrawBitmap(cache, 0, 0, mLayerPaint);
    //                 caching = TRUE;
    //             }
    //         }
    //     }
    //     else {
    //         ComputeScroll();

    //         canvas->Translate(-mScrollX, -mScrollY);
    //         if (!isLayer) {
    //             mPrivateFlags |= PFLAG_DRAWN | PFLAG_DRAWING_CACHE_VALID;
    //             mPrivateFlags &= ~PFLAG_DIRTY_MASK;
    //         }

    //         // Fast path for layouts with no backgrounds
    //         if ((mPrivateFlags & PFLAG_SKIP_DRAW) == PFLAG_SKIP_DRAW) {
    //             DispatchDraw(canvas);
    //         }
    //         else {
    //             Draw(canvas);
    //         }
    //     }

    //     canvas->OnPostDraw();

    //     displayList->End();
    //     displayList->SetCaching(caching);
    //     if (isLayer) {
    //         displayList->SetLeftTopRightBottom(0, 0, width, height);
    //     }
    //     else {
    //         SetDisplayListProperties(displayList);
    //     }
    // }
    // else if (!isLayer) {
    //     mPrivateFlags |= PFLAG_DRAWN | PFLAG_DRAWING_CACHE_VALID;
    //     mPrivateFlags &= ~PFLAG_DIRTY_MASK;
    // }

    // return displayList;
}

ECode View::GetDisplayList(
    /* [out] */ IRenderNode** node)
{
    VALIDATE_NOT_NULL(node)
    UpdateDisplayListIfDirty();
    *node = mRenderNode;
    REFCOUNT_ADD(*node)
    return NOERROR;
}

void View::ResetDisplayList()
{
    Boolean isValid;
    mRenderNode->IsValid(&isValid);
    if (isValid) {
        mRenderNode->DestroyDisplayListData();
    }
    mBackgroundRenderNode->IsValid(&isValid);
    if (mBackgroundRenderNode != NULL && isValid) {
        mBackgroundRenderNode->DestroyDisplayListData();
    }
}

/**
 * <p>Calling this method is equivalent to calling <code>getDrawingCache(FALSE)</code>.</p>
 *
 * @return A non-scaled bitmap representing this view or NULL if cache is disabled.
 *
 * @see #getDrawingCache(Boolean)
 */
AutoPtr<IBitmap> View::GetDrawingCache()
{
    return GetDrawingCache(FALSE);
}

/**
 * <p>Returns the bitmap in which this view drawing is cached. The returned bitmap
 * is NULL when caching is disabled. If caching is enabled and the cache is not ready,
 * this method will create it. Calling {@link #draw(android.graphics.Canvas)} will not
 * draw from the cache when the cache is enabled. To benefit from the cache, you must
 * request the drawing cache by calling this method and draw it on screen if the
 * returned bitmap is not NULL.</p>
 *
 * <p>Note about auto scaling in compatibility mode: When auto scaling is not enabled,
 * this method will create a bitmap of the same size as this view. Because this bitmap
 * will be drawn scaled by the parent ViewGroup, the result on screen might show
 * scaling artifacts. To avoid such artifacts, you should call this method by setting
 * the auto scaling to TRUE. Doing so, however, will generate a bitmap of a different
 * size than the view. This implies that your application must be able to handle this
 * size.</p>
 *
 * @param autoScale Indicates whether the generated bitmap should be scaled based on
 *        the current density of the screen when the application is in compatibility
 *        mode.
 *
 * @return A bitmap representing this view or NULL if cache is disabled.
 *
 * @see #setDrawingCacheEnabled(Boolean)
 * @see #isDrawingCacheEnabled()
 * @see #buildDrawingCache(Boolean)
 * @see #destroyDrawingCache()
 */
AutoPtr<IBitmap> View::GetDrawingCache(
    /* [in] */ Boolean autoScale)
{
    if ((mViewFlags & WILL_NOT_CACHE_DRAWING) == WILL_NOT_CACHE_DRAWING) {
        return AutoPtr<IBitmap>(NULL);
    }

    if ((mViewFlags & DRAWING_CACHE_ENABLED) == DRAWING_CACHE_ENABLED) {
        BuildDrawingCache(autoScale);
    }

    return autoScale ? mDrawingCache : mUnscaledDrawingCache;
}

/**
 * <p>Frees the resources used by the drawing cache. If you call
 * {@link #buildDrawingCache()} manually without calling
 * {@link #setDrawingCacheEnabled(Boolean) setDrawingCacheEnabled(TRUE)}, you
 * should cleanup the cache with this method afterwards.</p>
 *
 * @see #setDrawingCacheEnabled(Boolean)
 * @see #buildDrawingCache()
 * @see #getDrawingCache()
 */
ECode View::DestroyDrawingCache()
{
    if (mDrawingCache != NULL) {
        mDrawingCache->Recycle();
        mDrawingCache = NULL;
    }
    if (mUnscaledDrawingCache != NULL) {
        mUnscaledDrawingCache->Recycle();
        mUnscaledDrawingCache = NULL;
    }

    return NOERROR;
}

/**
 * Setting a solid background color for the drawing cache's bitmaps will improve
 * perfromance and memory usage. Note, though that this should only be used if this
 * view will always be drawn on top of a solid color.
 *
 * @param color The background color to use for the drawing cache's bitmap
 *
 * @see #setDrawingCacheEnabled(Boolean)
 * @see #buildDrawingCache()
 * @see #getDrawingCache()
 */
ECode View::SetDrawingCacheBackgroundColor(
    /* [in] */ Int32 color)
{
    if (color != mDrawingCacheBackgroundColor) {
        mDrawingCacheBackgroundColor = color;
        mPrivateFlags &= ~PFLAG_DRAWING_CACHE_VALID;
    }

    return NOERROR;
}

/**
 * @see #setDrawingCacheBackgroundColor(int)
 *
 * @return The background color to used for the drawing cache's bitmap
 */
Int32 View::GetDrawingCacheBackgroundColor()
{
    return mDrawingCacheBackgroundColor;
}

/**
 * <p>Calling this method is equivalent to calling <code>buildDrawingCache(FALSE)</code>.</p>
 *
 * @see #buildDrawingCache(Boolean)
 */
ECode View::BuildDrawingCache()
{
    BuildDrawingCache(FALSE);

    return NOERROR;
}

/**
 * <p>Forces the drawing cache to be built if the drawing cache is invalid.</p>
 *
 * <p>If you call {@link #buildDrawingCache()} manually without calling
 * {@link #setDrawingCacheEnabled(Boolean) setDrawingCacheEnabled(TRUE)}, you
 * should cleanup the cache by calling {@link #destroyDrawingCache()} afterwards.</p>
 *
 * <p>Note about auto scaling in compatibility mode: When auto scaling is not enabled,
 * this method will create a bitmap of the same size as this view. Because this bitmap
 * will be drawn scaled by the parent ViewGroup, the result on screen might show
 * scaling artifacts. To avoid such artifacts, you should call this method by setting
 * the auto scaling to TRUE. Doing so, however, will generate a bitmap of a different
 * size than the view. This implies that your application must be able to handle this
 * size.</p>
 *
 * @see #getDrawingCache()
 * @see #destroyDrawingCache()
 */
void View::BuildDrawingCache(
    /* [in] */ Boolean autoScale)
{
    if ((mPrivateFlags & PFLAG_DRAWING_CACHE_VALID) == 0 || (autoScale ?
            mDrawingCache == NULL : mUnscaledDrawingCache == NULL)) {
        mCachingFailed = FALSE;

        Int32 width = mRight - mLeft;
        Int32 height = mBottom - mTop;

        AttachInfo* attachInfo = mAttachInfo;
        Boolean scalingRequired = attachInfo != NULL && attachInfo->mScalingRequired;

        if (autoScale && scalingRequired) {
            width = (Int32)((width * attachInfo->mApplicationScale) + 0.5f);
            height = (Int32)((height * attachInfo->mApplicationScale) + 0.5f);
        }

        Int32 drawingCacheBackgroundColor = mDrawingCacheBackgroundColor;
        Boolean opaque = drawingCacheBackgroundColor != 0 || IsOpaque();
        Boolean use32BitCache = attachInfo != NULL && attachInfo->mUse32BitDrawingCache;

        const Int64 projectedBitmapSize = width * height * (opaque && !use32BitCache ? 2 : 4);
        Int32 size;
        CViewConfiguration::Get(mContext)->GetScaledMaximumDrawingCacheSize(&size);
        Int64 drawingCacheSize = size;
        if (width <= 0 || height <= 0 || projectedBitmapSize > drawingCacheSize) {
            if (width > 0 && height > 0) {
                // Log.w(VIEW_LOG_TAG, "View too large to fit into drawing cache, needs "
                //         + projectedBitmapSize + " bytes, only "
                //         + drawingCacheSize + " available");
            }
            DestroyDrawingCache();
            mCachingFailed = TRUE;
            return;
        }

        Boolean clear = TRUE;
        AutoPtr<IBitmap> bitmap = autoScale ? mDrawingCache : mUnscaledDrawingCache;

        Int32 bitmapWidth, bitmapHeight;
        if (bitmap == NULL || (bitmap->GetWidth(&bitmapWidth), bitmapWidth) != width
            || (bitmap->GetHeight(&bitmapHeight), bitmapHeight) != height) {
            BitmapConfig quality;
            if (!opaque) {
                // Never pick ARGB_4444 because it looks awful
                // Keep the DRAWING_CACHE_QUALITY_LOW flag just in case
                switch (mViewFlags & DRAWING_CACHE_QUALITY_MASK) {
                    case IView::DRAWING_CACHE_QUALITY_AUTO:
                    case IView::DRAWING_CACHE_QUALITY_LOW:
                    case IView::DRAWING_CACHE_QUALITY_HIGH:
                    default:
                        quality = BitmapConfig_ARGB_8888;
                        break;
                }
            }
            else {
                // Optimization for translucent windows
                // If the window is translucent, use a 32 bits bitmap to benefit from memcpy()
                quality = use32BitCache ? BitmapConfig_ARGB_8888 : BitmapConfig_RGB_565;
            }

            // Try to cleanup memory
            if (bitmap != NULL) {
                bitmap->Recycle();
            }

            AutoPtr<IBitmapFactory> factory;
            CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory);
            bitmap = NULL;

            AutoPtr<IDisplayMetrics> displayMetrics;
            mResources->GetDisplayMetrics((IDisplayMetrics**)&displayMetrics);
            ECode ec = factory->CreateBitmap(displayMetrics, width, height, quality, (IBitmap**)&bitmap);
            if (FAILED(ec)) {
                if (ec == (ECode)E_OUT_OF_MEMORY_ERROR) {
                    // If there is not enough memory to create the bitmap cache, just
                    // ignore the issue as bitmap caches are not required to draw the
                    // view hierarchy
                    if (autoScale) {
                        mDrawingCache = NULL;
                    }
                    else {
                        mUnscaledDrawingCache = NULL;
                    }
                    mCachingFailed = TRUE;
                }
                return;
            }

            AutoPtr<IResources> resource = GetResources();
            AutoPtr<IDisplayMetrics> metrics;
            resource->GetDisplayMetrics((IDisplayMetrics**)&metrics);
            Int32 dpi;
            metrics->GetDensityDpi(&dpi);
            bitmap->SetDensity(dpi);
            if (autoScale) {
                mDrawingCache = bitmap;
            }
            else {
                mUnscaledDrawingCache = bitmap;
            }
            if (opaque && use32BitCache) bitmap->SetHasAlpha(FALSE);

            clear = drawingCacheBackgroundColor != 0;
        }

        AutoPtr<ICanvas> canvas;
        if (attachInfo != NULL) {
            canvas = attachInfo->mCanvas;
            if (canvas == NULL) {
                CCanvas::New((ICanvas**)&canvas);
            }
            canvas->SetBitmap(bitmap);
            // Temporarily clobber the cached Canvas in case one of our children
            // is also using a drawing cache. Without this, the children would
            // steal the canvas by attaching their own bitmap to it and bad, bad
            // thing would happen (invisible views, corrupted drawings, etc.)
            attachInfo->mCanvas = NULL;
        }
        else {
            // This case should hopefully never or seldom happen
            CCanvas::New(bitmap, (ICanvas**)&canvas);
        }

        if (clear) {
            bitmap->EraseColor(drawingCacheBackgroundColor);
        }

        ComputeScroll();
        Int32 restoreCount;
        canvas->Save(&restoreCount);

        if (autoScale && scalingRequired) {
            Float scale = attachInfo->mApplicationScale;
            canvas->Scale(scale, scale);
        }

        canvas->Translate(-mScrollX, -mScrollY);

        mPrivateFlags |= PFLAG_DRAWN;
        if (mAttachInfo == NULL || !mAttachInfo->mHardwareAccelerated ||
                mLayerType != IView::LAYER_TYPE_NONE) {
            mPrivateFlags |= PFLAG_DRAWING_CACHE_VALID;
        }

        // Fast path for layouts with no backgrounds
        if ((mPrivateFlags & PFLAG_SKIP_DRAW) == PFLAG_SKIP_DRAW) {
            mPrivateFlags &= ~PFLAG_DIRTY_MASK;
            DispatchDraw(canvas);
            Boolean mOverlayIsEmpty;
            if (mOverlay != NULL && (mOverlay->IsEmpty(&mOverlayIsEmpty), !mOverlayIsEmpty)) {
                AutoPtr<IViewGroup> group;
                mOverlay->GetOverlayView((IViewGroup**)&group);
                (IView::Probe(group))->Draw(canvas);
            }
        }
        else {
            Draw(canvas);
            //TODO: delete
            //
            //AutoPtr<IPaintEx> paint;
            //CPaintEx::New((IPaintEx**)&paint);
            //paint->SetColor(0xFF000000 | Int32(this));
            //Int32 w, h;
            //canvas->GetWidth(&w);
            //canvas->GetHeight(&h);
            //printf("canvas w = %d, h = %d\n", w, h);
            ////canvas->DrawLine(0.0, 0.0, 200.0, 20.0, paint);
            ////canvas->DrawCircle(20.0, 20.0, 10.0, paint);
            //AutoPtr<IRect> rc;
            //CRect::New(0, 0, w/2, h/2, (IRect**)&rc);
            //canvas->DrawRect(rc, paint);
        }
        DrawAccessibilityFocus(canvas);

        canvas->RestoreToCount(restoreCount);
        canvas->SetBitmap(NULL);

        if (attachInfo != NULL) {
            // Restore the cached Canvas for our siblings
            attachInfo->mCanvas = canvas;
        }
    }
}

/**
 * Create a snapshot of the view into a bitmap.  We should probably make
 * some form of this public, but should think about the API.
 */
ECode View::CreateSnapshot(
    /* [in] */ BitmapConfig quality,
    /* [in] */ Int32 backgroundColor,
    /* [in] */ Boolean skipChildren,
    /* [out] */ IBitmap** bm)
{
    assert(bm != NULL);

    Int32 width = mRight - mLeft;
    Int32 height = mBottom - mTop;

    AttachInfo* attachInfo = mAttachInfo;
    Float scale = attachInfo != NULL ? attachInfo->mApplicationScale : 1.0f;
    width = (Int32) ((width * scale) + 0.5f);
    height = (Int32) ((height * scale) + 0.5f);

    AutoPtr<IBitmap> bitmap;
    AutoPtr<IBitmapFactory> factory;
    CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory);

    AutoPtr<IDisplayMetrics> displayMetrics;
    mResources->GetDisplayMetrics((IDisplayMetrics**)&displayMetrics);
    factory->CreateBitmap(displayMetrics, width > 0 ? width : 1, height > 0 ? height : 1, quality, (IBitmap**)&bitmap);
    if (bitmap == NULL) {
        //throw new OutOfMemoryError();
        return E_OUT_OF_MEMORY_ERROR;
    }

    AutoPtr<IResources> resource = GetResources();
    if (resource != NULL) {
        AutoPtr<IDisplayMetrics> metrics;
        resource->GetDisplayMetrics((IDisplayMetrics**)&metrics);
        Int32 dpi;
        metrics->GetDensityDpi(&dpi);
        bitmap->SetDensity(dpi);
    }

    AutoPtr<ICanvas> canvas;
    if (attachInfo != NULL) {
        canvas = attachInfo->mCanvas;
        if (canvas == NULL) {
            CCanvas::New((ICanvas**)&canvas);
        }
        canvas->SetBitmap(bitmap.Get());
        // Temporarily clobber the cached Canvas in case one of our children
        // is also using a drawing cache. Without this, the children would
        // steal the canvas by attaching their own bitmap to it and bad, bad
        // things would happen (invisible views, corrupted drawings, etc.)
        attachInfo->mCanvas = NULL;
    }
    else {
        // This case should hopefully never or seldom happen
        CCanvas::New(bitmap.Get(), (ICanvas**)&canvas);
    }

    if ((backgroundColor & 0xff000000) != 0) {
        bitmap->EraseColor(backgroundColor);
    }

    ComputeScroll();
    Int32 restoreCount;
    canvas->Save(&restoreCount);
    canvas->Scale(scale, scale);
    canvas->Translate(-mScrollX, -mScrollY);

    // Temporarily remove the dirty mask
    Int32 flags = mPrivateFlags;
    mPrivateFlags &= ~PFLAG_DIRTY_MASK;

    // Fast path for layouts with no backgrounds
    if ((mPrivateFlags & PFLAG_SKIP_DRAW) == PFLAG_SKIP_DRAW) {
        DispatchDraw(canvas);
        Boolean mOverlayIsEmpty;
        if (mOverlay != NULL && (mOverlay->IsEmpty(&mOverlayIsEmpty), !mOverlayIsEmpty)) {
            AutoPtr<IViewGroup> group;
            mOverlay->GetOverlayView((IViewGroup**)&group);
            (IView::Probe(group))->Draw(canvas);
        }
    }
    else {
        Draw(canvas);
    }
    DrawAccessibilityFocus(canvas);

    mPrivateFlags = flags;

    canvas->RestoreToCount(restoreCount);
    canvas->SetBitmap(NULL);

    if (attachInfo != NULL) {
        // Restore the cached Canvas for our siblings
        attachInfo->mCanvas = canvas;
    }

    *bm = bitmap;
    REFCOUNT_ADD(*bm);
    return NOERROR;
}

/**
 * Indicates whether this View is currently in edit mode. A View is usually
 * in edit mode when displayed within a developer tool. For instance, if
 * this View is being drawn by a visual user interface builder, this method
 * should return TRUE.
 *
 * Subclasses should check the return value of this method to provide
 * different behaviors if their normal behavior might interfere with the
 * host environment. For instance: the class spawns a thread in its
 * constructor, the drawing code relies on device-specific features, etc.
 *
 * This method is usually checked in the drawing code of custom widgets.
 *
 * @return True if this View is in edit mode, FALSE otherwise.
 */
Boolean View::IsInEditMode()
{
    return FALSE;
}

/**
 * If the View draws content inside its padding and enables fading edges,
 * it needs to support padding offsets. Padding offsets are added to the
 * fading edges to extend the length of the fade so that it covers pixels
 * drawn inside the padding.
 *
 * Subclasses of this class should override this method if they need
 * to draw content inside the padding.
 *
 * @return True if padding offset must be applied, FALSE otherwise.
 *
 * @see #getLeftPaddingOffset()
 * @see #getRightPaddingOffset()
 * @see #getTopPaddingOffset()
 * @see #getBottomPaddingOffset()
 *
 * @since CURRENT
 */
Boolean View::IsPaddingOffsetRequired()
{
    return FALSE;
}

/**
 * Amount by which to extend the left fading region. Called only when
 * {@link #isPaddingOffsetRequired()} returns TRUE.
 *
 * @return The left padding offset in pixels.
 *
 * @see #isPaddingOffsetRequired()
 *
 * @since CURRENT
 */
Int32 View::GetLeftPaddingOffset()
{
    return 0;
}

/**
 * Amount by which to extend the right fading region. Called only when
 * {@link #isPaddingOffsetRequired()} returns TRUE.
 *
 * @return The right padding offset in pixels.
 *
 * @see #isPaddingOffsetRequired()
 *
 * @since CURRENT
 */
Int32 View::GetRightPaddingOffset()
{
    return 0;
}

/**
 * Amount by which to extend the top fading region. Called only when
 * {@link #isPaddingOffsetRequired()} returns TRUE.
 *
 * @return The top padding offset in pixels.
 *
 * @see #isPaddingOffsetRequired()
 *
 * @since CURRENT
 */
Int32 View::GetTopPaddingOffset()
{
    return 0;
}

/**
 * Amount by which to extend the bottom fading region. Called only when
 * {@link #isPaddingOffsetRequired()} returns TRUE.
 *
 * @return The bottom padding offset in pixels.
 *
 * @see #isPaddingOffsetRequired()
 *
 * @since CURRENT
 */
Int32 View::GetBottomPaddingOffset()
{
    return 0;
}

Int32 View::GetFadeTop(
    /* [in] */ Boolean offsetRequired)
{
    Int32 top = mPaddingTop;
    if (offsetRequired) top += GetTopPaddingOffset();
    return top;
}

Int32 View::GetFadeHeight(
    /* [in] */ Boolean offsetRequired)
{
    Int32 padding = mPaddingTop;
    if (offsetRequired) padding += GetTopPaddingOffset();
    return mBottom - mTop - mPaddingBottom - padding;
}

Boolean View::IsHardwareAccelerated()
{
    return mAttachInfo != NULL && mAttachInfo->mHardwareAccelerated;
}

/**
 * Sets a rectangular area on this view to which the view will be clipped
 * when it is drawn. Setting the value to null will remove the clip bounds
 * and the view will draw normally, using its full bounds.
 *
 * @param clipBounds The rectangular area, in the local coordinates of
 * this view, to which future drawing operations will be clipped.
 */
ECode View::SetClipBounds(
    /* [in] */ IRect* clipBounds)
{
    if (clipBounds != NULL) {
        Boolean isEquals;
        if (clipBounds->Equals(mClipBounds, &isEquals), isEquals) {
            return NOERROR;
        }
        if (mClipBounds == NULL) {
            Invalidate();
            CRect::NewByFriend((CRect**)&mClipBounds);
        } else {
            CRect* temp = (CRect*)clipBounds
            Invalidate(Elastos::Core::Math::Min(mClipBounds->mLeft, temp->mLeft),
                    Elastos::Core::Math::Min(mClipBounds->mTop, temp->mTop),
                    Elastos::Core::Math::Min(mClipBounds->mRight, temp->mRight),
                    Elastos::Core::Math::Min(mClipBounds->mBottom, temp->mBottom));
            mClipBounds->Set(clipBounds);
        }
    } else {
        if (mClipBounds != NULL) {
            Invalidate();
            mClipBounds = NULL;
        }
    }
    Boolean res;
    mRenderNode->SetClipBounds(mClipBounds, &res);
    return NOERROR;
}

/**
 * Returns a copy of the current {@link #setClipBounds(Rect) clipBounds}.
 *
 * @return A copy of the current clip bounds if clip bounds are set,
 * otherwise null.
 */
ECode View::GetClipBounds(
    /* [out] */ IRect** clipBounds)
{
    VALIDATE_NOT_NULL(clipBounds)
    if (mClipBounds) {
        CRect::New(clipBounds);
    } else {
        *clipBounds = NULL;
    }
    return NOERROR;
}

Boolean View::DrawAnimation(
    /* [in] */ IViewGroup* parentObj,
    /* [in] */ Int64 drawingTime,
    /* [in] */ IAnimation* a,
    /* [in] */ Boolean scalingRequired)
{
     AutoPtr<ITransformation> invalidationTransform;
     ViewGroup* parent = VIEWGROUP_PROBE(parentObj);
     Int32 flags = parent->mGroupFlags;
     Boolean initialized;
     a->IsInitialized(&initialized);
     if (!initialized) {
         Int32 w = parent->GetWidth();
         Int32 h = parent->GetHeight();
         a->Initialize(mRight - mLeft, mBottom - mTop, w, h);
         a->InitializeInvalidateRegion(0, 0, mRight - mLeft, mBottom - mTop);
         if (mAttachInfo != NULL) a->SetListenerHandler(mAttachInfo->mHandler);
         OnAnimationStart();
     }

    AutoPtr<ITransformation> t;
    parent->GetChildTransformation((ITransformation**)&t);
    Boolean more;
    a->GetTransformation(drawingTime, t, 1f, &more);
     if (scalingRequired && mAttachInfo->mApplicationScale != 1.0f) {
         if (parent->mInvalidationTransformation == NULL) {
             CTransformation::New((ITransformation**)&(parent->mInvalidationTransformation));
         }

         invalidationTransform = parent->mInvalidationTransformation;
         Boolean res;
         a->GetTransformation(drawingTime, invalidationTransform, 1.0f, &res);
     }
     else {
         invalidationTransform = t;
     }

     Int32 tmpFlags = parent->mGroupFlags;
     if (more) {
         Boolean change = FALSE;
         if (!(a->WillChangeBounds(&change), change)) {
             if ((flags & (ViewGroup::FLAG_OPTIMIZE_INVALIDATE | ViewGroup::FLAG_ANIMATION_DONE)) ==
                     ViewGroup::FLAG_OPTIMIZE_INVALIDATE) {
                 tmpFlags |= ViewGroup::FLAG_INVALIDATE_REQUIRED;
             }
             else if ((flags & ViewGroup::FLAG_INVALIDATE_REQUIRED) == 0) {
                 // The child need to draw an animation, potentially offscreen, so
                 // make sure we do not cancel invalidate requests
                 parent->mPrivateFlags |= PFLAG_DRAW_ANIMATION;
                 parent->Invalidate(mLeft, mTop, mRight, mBottom);
             }
         }
         else {
             if (parent->mInvalidateRegion == NULL) {
                 CRectF::New((IRectF**)&(parent->mInvalidateRegion));
             }
             AutoPtr<IRectF> region = parent->mInvalidateRegion;
             a->GetInvalidateRegion(0, 0, mRight - mLeft, mBottom - mTop, region,
                     invalidationTransform);
             // The child need to draw an animation, potentially offscreen, so
             // make sure we do not cancel invalidate requests
             parent->mPrivateFlags |= PFLAG_DRAW_ANIMATION;
             Float l, t;
             region->GetLeft(&l);
             region->GetTop(&t);
             const Int32 left = mLeft + (Int32)l;
             const Int32 top = mTop + (Int32)t;

             Float w = 0, h = 0;
             region->GetWidth(&w);
             region->GetHeight(&h);
             parent->Invalidate(left, top, left + (Int32)(w + .5f),
                     top + (Int32)(h + .5f));
         }
     }
     return more;
}

void View::SetDisplayListProperties(
    /* [in] */ IRenderNode* renderNode)
{
    if (renderNode != NULL) {
        if ((mPrivateFlags3 & PFLAG3_OUTLINE_INVALID) != 0) {
            RebuildOutline();
            mPrivateFlags3 &= ~PFLAG3_OUTLINE_INVALID;
        }
        Boolean res, hasOverlappingRendering;
        HasOverlappingRendering(&hasOverlappingRendering);
        renderNode->SetHasOverlappingRendering(hasOverlappingRendering, &res);
        if (IViewGroup::Probe(mParent)) {
            renderNode.setClipToBounds(
                    (ViewGroup*(IViewGroup::Probe(mParent)))->mGroupFlags & IViewGroup::FLAG_CLIP_CHILDREN) != 0);
        }
        Float alpha = 1;
        if (IViewGroup::Probe(mParent) && (ViewGroup*(IViewGroup::Probe(mParent)))->mGroupFlagss &
                IViewGroup::FLAG_SUPPORT_STATIC_TRANSFORMATIONS) != 0) {
            AutoPtr<ViewGroup> parentVG = IViewGroup::Probe(mParent);
            AutoPtr<ITransformation> t;
            parentVG->GetChildTransformation((ITransformation**)&t);
            if (parentVG->GetChildStaticTransformation(THIS_PROBE(IView), t)) {
                Int32 transformType;
                t->GetTransformationType(&transformType);
                if (transformType != ITransformation::TYPE_IDENTITY) {
                    if ((transformType & ITransformation::TYPE_ALPHA) != 0) {
                        t>GetAlpha(&alpha);
                    }
                    if ((transformType & ITransformation::TYPE_MATRIX) != 0) {
                        AutoPtr<IMatrix> temp;
                        t->GetMatrix((IMatrix**)&temp);
                        renderNode->SetStaticMatrix(temp, &res);
                    }
                }
            }
        }
        if (mTransformationInfo != NULL) {
            alpha *= GetFinalAlpha();
            if (alpha < 1) {
                Int32 multipliedAlpha = (Int32) (255 * alpha);
                if (onSetAlpha(multipliedAlpha)) {
                    alpha = 1;
                }
            }
            renderNode->SetAlpha(alpha, &res);
        } else if (alpha < 1) {
            renderNode->SetAlpha(alpha, &res);
        }
    }
}

Boolean View::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IViewGroup* parentObj,
    /* [in] */ Int64 drawingTime)
{
    Boolean usingRenderNodeProperties = mAttachInfo != NULL && mAttachInfo->mHardwareAccelerated;
    Boolean more = FALSE;
    const Boolean childHasIdentityMatrix = HasIdentityMatrix();
    ViewGroup* parent = VIEWGROUP_PROBE(parentObj);
    const Int32 flags = parent->mGroupFlags;

    if ((flags & ViewGroup::FLAG_CLEAR_TRANSFORMATION) == ViewGroup::FLAG_CLEAR_TRANSFORMATION) {
        AutoPtr<ITransformation> temp;
        parent->GetChildTransformation((ITransformation**)&temp)
        temp->Clear();
        parent->mGroupFlags &= ~ViewGroup::FLAG_CLEAR_TRANSFORMATION;
    }

    AutoPtr<ITransformation> transformToApply;
    Boolean concatMatrix = FALSE;

    Boolean scalingRequired = FALSE;
    Boolean caching;
    Int32 layerType = GetLayerType();

    Boolean hardwareAccelerated = FALSE;
    canvas->IsHardwareAccelerated(&hardwareAccelerated);
    if ((flags & ViewGroup::FLAG_CHILDREN_DRAWN_WITH_CACHE) != 0 ||
            (flags & ViewGroup::FLAG_ALWAYS_DRAWN_WITH_CACHE) != 0) {
        caching = TRUE;
        // Auto-scaled apps are not hw-accelerated, no need to set scaling flag on DisplayList
        if (mAttachInfo != NULL) scalingRequired = mAttachInfo->mScalingRequired;
    }
    else {
        caching = (layerType != IView::LAYER_TYPE_NONE) || hardwareAccelerated;
    }

    AutoPtr<IAnimation> a = GetAnimation();
    if (a != NULL) {
        more = DrawAnimation(parentObj, drawingTime, a, scalingRequired);
        a->WillChangeTransformationMatrix(&concatMatrix);
        if (concatMatrix) {
            mPrivateFlags3 |= PFLAG3_VIEW_IS_ANIMATING_TRANSFORM;
        }
        parent->GetChildTransformation((ITransformation**)&transformToApply);
    }
    else {
        if ((mPrivateFlags3 & PFLAG3_VIEW_IS_ANIMATING_TRANSFORM) != 0) {
            // No longer animating: clear out old animation matrix
            Boolean res;
            mRenderNode->SetAnimationMatrix(NULL, &res);
            mPrivateFlags3 &= ~PFLAG3_VIEW_IS_ANIMATING_TRANSFORM;
        }
        if (!usingRenderNodeProperties &&
                (flags & ViewGroup::FLAG_SUPPORT_STATIC_TRANSFORMATIONS) != 0) {
            AutoPtr<ITransformation> t;
            parent->GetChildTransformation((ITransformation**)&t);
            Boolean hasTransform = parent->GetChildStaticTransformation(THIS_PROBE(IView), t);
            if (hasTransform) {
                Int32 transformType = 0;
                t->GetTransformationType(&transformType);
                transformToApply = transformType != ITransformation::TYPE_IDENTITY ? t : NULL;
                concatMatrix = (transformType & ITransformation::TYPE_MATRIX) != 0;
            }
        }
    }

    concatMatrix |= !childHasIdentityMatrix;

    // Sets the flag as early as possible to allow draw() implementations
    // to call invalidate() successfully when doing animations
    mPrivateFlags |= PFLAG_DRAWN;

    Boolean isNotIntersect = FALSE;
    if (!concatMatrix && (flags & (ViewGroup::FLAG_SUPPORT_STATIC_TRANSFORMATIONS |
        ViewGroup::FLAG_CLIP_CHILDREN)) == ViewGroup::FLAG_CLIP_CHILDREN &&
        (canvas->QuickReject(mLeft, mTop, mRight, mBottom, CanvasEdgeType_BW, &isNotIntersect),
        isNotIntersect) && (mPrivateFlags & PFLAG_DRAW_ANIMATION) == 0) {
        mPrivateFlags2 |= PFLAG2_VIEW_QUICK_REJECTED;
        return more;
    }
    mPrivateFlags2 &= ~PFLAG2_VIEW_QUICK_REJECTED;

    if (hardwareAccelerated) {
        // Clear INVALIDATED flag to allow invalidation to occur during rendering, but
        // retain the flag's value temporarily in the mRecreateDisplayList flag
        mRecreateDisplayList = (mPrivateFlags & PFLAG_INVALIDATED) == PFLAG_INVALIDATED;
        mPrivateFlags &= ~PFLAG_INVALIDATED;
    }

    AutoPtr<IRenderNode> renderNode;
    AutoPtr<IBitmap> cache;
    Boolean hasDisplayList = FALSE;
    if (caching) {
        if (!hardwareAccelerated) {
            if (layerType != IView::LAYER_TYPE_NONE) {
                layerType = IView::LAYER_TYPE_SOFTWARE;
                BuildDrawingCache(TRUE);
            }
            cache = GetDrawingCache(TRUE);
        }
        else {
            switch (layerType) {
                case IView::LAYER_TYPE_SOFTWARE:
                    if (usingRenderNodeProperties) {
                        hasDisplayList = CanHaveDisplayList();
                    }
                    else {
                        BuildDrawingCache(TRUE);
                        cache = GetDrawingCache(TRUE);
                    }
                    break;
                case IView::LAYER_TYPE_HARDWARE:
                    if (usingRenderNodeProperties) {
                        hasDisplayList = CanHaveDisplayList();
                    }
                    break;
                case IView::LAYER_TYPE_NONE:
                    // Delay getting the display list until animation-driven alpha values are
                    // set up and possibly passed on to the view
                    hasDisplayList = CanHaveDisplayList();
                    break;
            }
        }
    }

    usingRenderNodeProperties &= hasDisplayList;
    if (usingRenderNodeProperties) {
        renderNode = GetDisplayList();
        assert(renderNode != NULL);
        Boolean valid = FALSE;
        if (!(renderNode->IsValid(&valid), valid)) {
            // Uncommon, but possible. If a view is removed from the hierarchy during the call
            // to getDisplayList(), the display list will be marked invalid and we should not
            // try to use it again.
            renderNode = NULL;
            hasDisplayList = FALSE;
            usingRenderNodeProperties = FALSE;
        }
    }

    Int32 sx = 0;
    Int32 sy = 0;
    if (!hasDisplayList) {
        ComputeScroll();
        sx = mScrollX;
        sy = mScrollY;
    }

    const Boolean hasNoCache = cache == NULL || hasDisplayList;
    const Boolean offsetForScroll = cache == NULL && !hasDisplayList &&
            layerType != IView::LAYER_TYPE_HARDWARE;

    Int32 restoreTo = -1;
    if (!usingRenderNodeProperties || transformToApply != NULL) {
        canvas->Save(&restoreTo);
    }
    if (offsetForScroll) {
        canvas->Translate(mLeft - sx, mTop - sy);
    }
    else {
        if (!usingRenderNodeProperties) {
            canvas->Translate(mLeft, mTop);
        }
        if (scalingRequired) {
            if (usingRenderNodeProperties) {
                // TODO: Might not need this if we put everything inside the DL
                canvas->Save(&restoreTo);
            }
            // mAttachInfo cannot be NULL, otherwise scalingRequired == false
            const Float scale = 1.0f / mAttachInfo->mApplicationScale;
            canvas->Scale(scale, scale);
        }
    }
    Float tempAlpha, transitionAlpha;
    GetAlpha(&tempAlpha);
    GetTransitionAlpha(&transitionAlpha);
    Float alpha = usingRenderNodeProperties ? 1 : (tempAlpha * transitionAlpha);
    if (transformToApply != NULL || alpha < 1 || !HasIdentityMatrix() ||
            (mPrivateFlags3 & PFLAG3_VIEW_IS_ANIMATING_ALPHA) == PFLAG3_VIEW_IS_ANIMATING_ALPHA) {
        if (transformToApply != NULL || !childHasIdentityMatrix) {
            Int32 transX = 0;
            Int32 transY = 0;

            if (offsetForScroll) {
                transX = -sx;
                transY = -sy;
            }

            if (transformToApply != NULL) {
                if (concatMatrix) {
                    if (usingRenderNodeProperties) {
                        Boolean res;
                        renderNode->SetAnimationMatrix(matrix, &res);
                    }
                    else {
                        // Undo the scroll translation, apply the transformation matrix,
                        // then redo the scroll translate to get the correct result.
                        canvas->Translate(-transX, -transY);
                        canvas->Concat(matrix);
                        canvas->Translate(transX, transY);
                    }
                    parent->mGroupFlags |= ViewGroup::FLAG_CLEAR_TRANSFORMATION;
                }

                Float transformAlpha = 0.0f;
                transformToApply->GetAlpha(&transformAlpha);
                if (transformAlpha < 1) {
                    alpha *= transformAlpha;
                    parent->mGroupFlags |= ViewGroup::FLAG_CLEAR_TRANSFORMATION;
                }
            }

            if (!childHasIdentityMatrix && !usingRenderNodeProperties) {
                canvas->Translate(-transX, -transY);
                canvas->Concat(GetMatrix());
                canvas->Translate(transX, transY);
            }
        }

        // Deal with alpha if it is or used to be <1
        if (alpha < 1 ||
                (mPrivateFlags3 & PFLAG3_VIEW_IS_ANIMATING_ALPHA) == PFLAG3_VIEW_IS_ANIMATING_ALPHA) {
            if (alpha < 1) {
                mPrivateFlags3 |= PFLAG3_VIEW_IS_ANIMATING_ALPHA;
            }
            else {
                mPrivateFlags3 &= ~PFLAG3_VIEW_IS_ANIMATING_ALPHA;
            }
            parent->mGroupFlags |= ViewGroup::FLAG_CLEAR_TRANSFORMATION;
            if (hasNoCache) {
                const Int32 multipliedAlpha = (Int32) (255 * alpha);
                if (!OnSetAlpha(multipliedAlpha)) {
                    Int32 layerFlags = ICanvas::HAS_ALPHA_LAYER_SAVE_FLAG;
                    if ((flags & ViewGroup::FLAG_CLIP_CHILDREN) != 0 ||
                            layerType != IView::LAYER_TYPE_NONE) {
                        layerFlags |= ICanvas::CLIP_TO_LAYER_SAVE_FLAG;
                    }
                    if (usingRenderNodeProperties) {
                        Float tempAlpha, transformAlpha;
                        GetAlpha(&tempAlpha);
                        GetTransitionAlpha(&transformAlpha);
                        renderNode->SetAlpha(alpha * tempAlpha * transformAlpha);
                    }
                    else  if (layerType == IView::LAYER_TYPE_NONE) {
                        const Int32 scrollX = hasDisplayList ? 0 : sx;
                        const Int32 scrollY = hasDisplayList ? 0 : sy;
                        Int32 result;
                        canvas->SaveLayerAlpha(
                            (Float)scrollX, (Float)scrollY, scrollX + (mRight - mLeft),
                            scrollY + (mBottom - mTop), multipliedAlpha, layerFlags, &result);
                    }
                }
                else {
                    // Alpha is handled by the child directly, clobber the layer's alpha
                    mPrivateFlags |= PFLAG_ALPHA_SET;
                }
            }
        }
    }
    else if ((mPrivateFlags & PFLAG_ALPHA_SET) == PFLAG_ALPHA_SET) {
        OnSetAlpha(255);
        mPrivateFlags &= ~PFLAG_ALPHA_SET;
    }

    if (!usingRenderNodeProperties) {
        // apply clips directly, since RenderNode won't do it for this draw
        if ((flags & IViewGroup::FLAG_CLIP_CHILDREN) == IViewGroup::FLAG_CLIP_CHILDREN
             && cache == NULL) {
            Boolean res;
            if (offsetForScroll) {
                canvas->ClipRect(sx, sy, sx + (mRight - mLeft), sy + (mBottom - mTop), &res);
            }
            else {
                if (!scalingRequired || cache == NULL) {
                    canvas->ClipRect(0, 0, mRight - mLeft, mBottom - mTop, &res);
                }
                else {
                    Int32 w = 0, h = 0;
                    cache->GetWidth(&w);
                    cache->GetHeight(&h);
                    canvas->ClipRect(0, 0, w, h, &res);
                }
            }
        }

        if (mClipBounds != NULL) {
            // clip bounds ignore scroll
            Boolean res;
            canvas->ClipRect((IRect*)mClipBounds, &res);
        }
    }

    if (!usingRenderNodeProperties && hasDisplayList) {
        renderNode = GetDisplayList();
        assert(renderNode != NULL);
        Boolean valid = FALSE;
        if (!(renderNode->IsValid(&valid), valid)) {
            // Uncommon, but possible. If a view is removed from the hierarchy during the call
            // to getDisplayList(), the display list will be marked invalid and we should not
            // try to use it again.
            renderNode = NULL;
            hasDisplayList = FALSE;
        }
    }

    if (hasNoCache) {
        Boolean layerRendered = FALSE;
        if (layerType == IView::LAYER_TYPE_HARDWARE && !usingRenderNodeProperties) {
            // AutoPtr<IHardwareLayer> layer = GetHardwareLayer();
            // Boolean valid = FALSE;
            // if (layer != NULL && (layer->IsValid(&valid), valid)) {
            //    Int32 restoreAlpha;
            //    mLayerPaint->GetAlpha(&restoreAlpha);
            //    mLayerPaint->SetAlpha((Int32) (alpha * 255));
            //    canvas->DrawHardwareLayer(layer, 0, 0, mLayerPaint);
            //    mLayerPaint->SetAlpha(restoreAlpha);
            //    layerRendered = TRUE;
            // }
            // else {
                const Int32 scrollX = hasDisplayList ? 0 : sx;
                const Int32 scrollY = hasDisplayList ? 0 : sy;
                Int32 result;
                canvas->SaveLayer(scrollX, scrollY,
                    scrollX + mRight - mLeft, scrollY + mBottom - mTop, mLayerPaint,
                    ICanvas::HAS_ALPHA_LAYER_SAVE_FLAG | ICanvas::CLIP_TO_LAYER_SAVE_FLAG, &result);
            //}
        }

        if (!layerRendered) {
            if (!hasDisplayList) {
                // Fast path for layouts with no backgrounds
                if ((mPrivateFlags & PFLAG_SKIP_DRAW) == PFLAG_SKIP_DRAW) {
                    mPrivateFlags &= ~PFLAG_DIRTY_MASK;
                    DispatchDraw(canvas);
                    Boolean isEmpty;
                    if (mOverlay != NULL && (mOverlay->IsEmpty(&isEmpty), !isEmpty)) {
                        AutoPtr<IViewGroup> group;
                        mOverlay->GetOverlayView((IViewGroup**)&group);
                        (IView::Probe(group))->Draw(canvas);
                    }
                }
                else {
                    Draw(canvas);
                }
                DrawAccessibilityFocus(canvas);
            }
            else {
                mPrivateFlags &= ~PFLAG_DIRTY_MASK;
                //(IHardwareCanvas::Probe(canvas))->DrawRenderNode(renderNode, NULL, flags);
            }
        }
    }
    else if (cache != NULL) {
        mPrivateFlags &= ~PFLAG_DIRTY_MASK;
        AutoPtr<IPaint> cachePaint;
        Int32 restoreAlpha = 0;

        if (layerType == IView::LAYER_TYPE_NONE) {
            cachePaint = parent->mCachePaint;
            if (cachePaint == NULL) {
                CPaint::New((IPaint**)&cachePaint);
                cachePaint->SetDither(FALSE);
                parent->mCachePaint = cachePaint;
            }
        }
        else {
            cachePaint = mLayerPaint;
            mLayerPaint->GetAlpha(&restoreAlpha);
        }
        cachePaint->SetAlpha((Int32) (alpha * 255));
        canvas->DrawBitmap(cache, 0.0f, 0.0f, cachePaint);
        cachePaint->SetAlpha(restoreAlpha);
    }

    if (restoreTo >= 0) {
        canvas->RestoreToCount(restoreTo);
    }

    if (a != NULL && !more) {
        Boolean after = FALSE;
        if (!hardwareAccelerated && !(a->GetFillAfter(&after), after)) {
            OnSetAlpha(255);
        }
        parent->FinishAnimatingView(IVIEW_PROBE(this), a);
    }

    if (more && hardwareAccelerated) {
        Boolean has = FALSE;
        assert(a != NULL);
        if ((a->HasAlpha(&has), has) && (mPrivateFlags & PFLAG_ALPHA_SET) == PFLAG_ALPHA_SET) {
            // alpha animations should cause the child to recreate its display list
            Invalidate(TRUE);
        }
    }

    mRecreateDisplayList = FALSE;

    return more;
}

/**
 * Manually render this view (and all of its children) to the given Canvas.
 * The view must have already done a full layout before this function is
 * called.  When implementing a view, do not override this method; instead,
 * you should implement {@link #onDraw}.
 *
 * @param canvas The Canvas to which the View is rendered.
 */
ECode View::Draw(
    /* [in] */ ICanvas* canvas)
{
    Int32 privateFlags = mPrivateFlags;
    Boolean dirtyOpaque = (privateFlags & PFLAG_DIRTY_MASK) == PFLAG_DIRTY_OPAQUE &&
            (mAttachInfo == NULL || !mAttachInfo->mIgnoreDirtyState);
    mPrivateFlags = (privateFlags & ~PFLAG_DIRTY_MASK) | PFLAG_DRAWN;

    /*
     * Draw traversal performs several drawing steps which must be executed
     * in the appropriate order:
     *
     *      1. Draw the background
     *      2. If necessary, save the canvas' layers to prepare for fading
     *      3. Draw view's content
     *      4. Draw children
     *      5. If necessary, draw the fading edges and restore layers
     *      6. Draw decorations (scrollbars for instance)
     */

    // Step 1, draw the background, if needed
    Int32 saveCount;

    if (!dirtyOpaque) {
        DrawBackground(canvas);
    }

    // skip step 2 & 5 if possible (common case)
    Int32 viewFlags = mViewFlags;
    Boolean horizontalEdges = (viewFlags & FADING_EDGE_HORIZONTAL) != 0;
    Boolean verticalEdges = (viewFlags & FADING_EDGE_VERTICAL) != 0;
    if (!verticalEdges && !horizontalEdges) {
        //PRINT_FILE_LINE_EX("View::Draw ID = 0x%08x, this = 0x%08x----------------2", mID, this);
        // Step 3, draw the content
        if (!dirtyOpaque) {
            OnDraw(canvas);
        }

        //PRINT_FILE_LINE_EX("View::Draw ID = 0x%08x, this = 0x%08x----------------3", mID, this);
        // Step 4, draw the children
        DispatchDraw(canvas);

        // Step 6, draw decorations (scrollbars)
        OnDrawScrollBars(canvas);

        Boolean isEmpty;
        if (mOverlay != NULL && (mOverlay->IsEmpty(&isEmpty), !isEmpty)) {
            AutoPtr<IViewGroup> group;
            mOverlay->GetOverlayView((IViewGroup**)&group);
            ((ViewGroup*)group)->DispatchDraw(canvas);
        }

        return NOERROR;
    }

    /*
     * Here we do the full fledged routine...
     * (this is an uncommon case where speed matters less,
     * this is why we repeat some of the tests that have been
     * done above)
     */

    Boolean drawTop = FALSE;
    Boolean drawBottom = FALSE;
    Boolean drawLeft = FALSE;
    Boolean drawRight = FALSE;

    Float topFadeStrength = 0.0f;
    Float bottomFadeStrength = 0.0f;
    Float leftFadeStrength = 0.0f;
    Float rightFadeStrength = 0.0f;

    // Step 2, save the canvas' layers
    Int32 paddingLeft = mPaddingLeft;

    Boolean offsetRequired = IsPaddingOffsetRequired();
    if (offsetRequired) {
        paddingLeft += GetLeftPaddingOffset();
    }

    Int32 left = mScrollX + paddingLeft;
    Int32 right = left + mRight - mLeft - mPaddingRight - paddingLeft;
    Int32 top = mScrollY + GetFadeTop(offsetRequired);;
    Int32 bottom = top + GetFadeHeight(offsetRequired);

    if (offsetRequired) {
        right += GetRightPaddingOffset();
        bottom += GetBottomPaddingOffset();
    }

    AutoPtr<ScrollabilityCache> scrollabilityCache = mScrollCache;
    const Float fadeHeight = scrollabilityCache->mFadingEdgeLength;
    Int32 length = (Int32) fadeHeight;

    // clip the fade length if top and bottom fades overlap
    // overlapping fades produce odd-looking artifacts
    if (verticalEdges && (top + length > bottom - length)) {
        length = (bottom - top) / 2;
    }

    // also clip horizontal fades if necessary
    if (horizontalEdges && (left + length > right - length)) {
        length = (right - left) / 2;
    }

    if (verticalEdges) {
        topFadeStrength = Elastos::Core::Math::Max(0.0f, Elastos::Core::Math::Min(1.0f, GetTopFadingEdgeStrength()));
        drawTop = topFadeStrength * fadeHeight > 1.0f;
        bottomFadeStrength = Elastos::Core::Math::Max(0.0f, Elastos::Core::Math::Min(1.0f, GetBottomFadingEdgeStrength()));
        drawBottom = bottomFadeStrength * fadeHeight > 1.0f;
    }

    if (horizontalEdges) {
        leftFadeStrength = Elastos::Core::Math::Max(0.0f, Elastos::Core::Math::Min(1.0f, GetLeftFadingEdgeStrength()));
        drawLeft = leftFadeStrength * fadeHeight > 1.0f;
        rightFadeStrength = Elastos::Core::Math::Max(0.0f, Elastos::Core::Math::Min(1.0f, GetRightFadingEdgeStrength()));
        drawRight = rightFadeStrength * fadeHeight > 1.0f;
    }

    canvas->GetSaveCount(&saveCount);

    Int32 solidColor = GetSolidColor();
    if (solidColor == 0) {
        Int32 count;
        Int32 flags = CCanvas::HAS_ALPHA_LAYER_SAVE_FLAG;

        if (drawTop) {
            canvas->SaveLayer(left, top, right, top + length, NULL, flags, &count);
        }

        if (drawBottom) {
            canvas->SaveLayer(left, bottom - length, right, bottom, NULL, flags, &count);
        }

        if (drawLeft) {
            canvas->SaveLayer(left, top, left + length, bottom, NULL, flags, &count);
        }

        if (drawRight) {
            canvas->SaveLayer(right - length, top, right, bottom, NULL, flags, &count);
        }
    }
    else {
        scrollabilityCache->SetFadeColor(solidColor);
    }

    // Step 3, draw the content
    if (!dirtyOpaque) OnDraw(canvas);

    //PRINT_FILE_LINE_EX("View::Draw ID = 0x%08x, this = 0x%08x----------------5", mID, this);
    // Step 4, draw the children
    DispatchDraw(canvas);
    //PRINT_FILE_LINE_EX("View::Draw ID = 0x%08x, this = 0x%08x----------------6", mID, this);

    // Step 5, draw the fade effect and restore layers
    AutoPtr<IPaint> p = scrollabilityCache->mPaint;
    AutoPtr<IMatrix> matrix = scrollabilityCache->mMatrix;
    AutoPtr<IShader> fade = scrollabilityCache->mShader;

    Boolean result;
    if (drawTop) {
        matrix->SetScale(1, fadeHeight * topFadeStrength);
        matrix->PostTranslate(left, top, &result);
        fade->SetLocalMatrix(matrix);
        p->SetShader(fade);
        canvas->DrawRect(left, top, right, top + length, p);
    }

    if (drawBottom) {
        matrix->SetScale(1, fadeHeight * bottomFadeStrength);
        matrix->PostRotate(180, &result);
        matrix->PostTranslate(left, bottom, &result);
        fade->SetLocalMatrix(matrix);
        p->SetShader(fade);
        canvas->DrawRect(left, bottom - length, right, bottom, p);
    }

    if (drawLeft) {
        matrix->SetScale(1, fadeHeight * leftFadeStrength);
        matrix->PostRotate(-90, &result);
        matrix->PostTranslate(left, top, &result);
        fade->SetLocalMatrix(matrix);
        p->SetShader(fade);
        canvas->DrawRect(left, top, left + length, bottom, p);
    }

    if (drawRight) {
        matrix->SetScale(1, fadeHeight * rightFadeStrength);
        matrix->PostRotate(90, &result);
        matrix->PostTranslate(right, top, &result);
        fade->SetLocalMatrix(matrix);
        p->SetShader(fade);
        canvas->DrawRect(right - length, top, right, bottom, p);
    }

    canvas->RestoreToCount(saveCount);

    //PRINT_FILE_LINE_EX("View::Draw ID = 0x%08x, this = 0x%08x----------------7", mID, this);
    // Step 6, draw decorations (scrollbars)
    OnDrawScrollBars(canvas);
    //PRINT_FILE_LINE_EX("View::Draw ID = 0x%08x, this = 0x%08x----------------8", mID, this);

    Boolean mOverlayIsEmpty;
    if (mOverlay != NULL && (mOverlay->IsEmpty(&mOverlayIsEmpty), !mOverlayIsEmpty)) {
        AutoPtr<IViewGroup> group;
        mOverlay->GetOverlayView((IViewGroup**)&group);
        ((ViewGroup*)group)->DispatchDraw(canvas);
    }

    return NOERROR;
}

/**
 * Draws the accessibility focus rect onto the specified canvas.
 *
 * @param canvas Canvas on which to draw the focus rect
 */
void View::DrawAccessibilityFocus(
    /* [in] */ ICanvas* canvas)
{
    if (mAttachInfo == NULL) {
        return;
    }

    AutoPtr<CRect> bounds = mAttachInfo->mTmpInvalRect;
    AutoPtr<ViewRootImpl> viewRoot;
    GetViewRootImpl((ViewRootImpl**)&viewRoot);
    if (viewRoot == NULL || viewRoot->GetAccessibilityFocusedHost() != THIS_PROBE(IView)) {
        return;
    }

    AutoPtr<IAccessibilityManager> manager;
    CAccessibilityManager::GetInstance(mContext, (IAccessibilityManager**)&manager);
    Boolean isEnabled, isTouchExplorationEnabled;
    manager->IsEnabled(&isEnabled);
    manager->IsTouchExplorationEnabled(&isTouchExplorationEnabled);
    if (!isEnabled || !isTouchExplorationEnabled) {
        return;
    }

    AutoPtr<IDrawable> drawable = viewRoot->GetAccessibilityFocusedDrawable();
    if (drawable == NULL) {
        return;
    }

    AutoPtr<IAccessibilityNodeInfo> virtualView = viewRoot->GetAccessibilityFocusedVirtualView();
    if (virtualView != NULL) {
        virtualView->GetBoundsInScreen((IRect*)bounds);
        GetLocationOnScreen((ArrayOf<Int32>**)&(mAttachInfo->mTmpLocation));
        bounds->Offset((*mAttachInfo->mTmpLocation)[0], (*mAttachInfo->mTmpLocation)[1]);
    } else {
        bounds->Set(0, 0, mRight - mLeft, mBottom - mTop);
    }

    canvas->Save();
    canvas->Translate(mScrollX, mScrollY);
    canvas->ClipRect(bounds, Region::Op::REPLACE);
    drawable->SetBounds(bounds);
    drawable->Draw(canvas);
    canvas->Restore();
}

/**
 * Draws the background onto the specified canvas.
 *
 * @param canvas Canvas on which to draw the background
 */
void View::DrawBackground(
    /* [in] */ ICanvas* canvas)
{
    AutoPtr<IDrawable> background = mBackground;
    if (background == NULL) {
        return;
    }

    if (mBackgroundSizeChanged) {
        background->SetBounds(0, 0,  mRight - mLeft, mBottom - mTop);
        mBackgroundSizeChanged = FALSE;
        mPrivateFlags3 |= PFLAG3_OUTLINE_INVALID;
    }

    // Attempt to use a display list if requested.
    Boolean isHardwareAccelerated;
    if ((canvas->IsHardwareAccelerated(&isHardwareAccelerated), isHardwareAccelerated) && mAttachInfo != NULL
            && mAttachInfo->mHardwareRenderer != NULL) {
        mBackgroundRenderNode = GetDrawableRenderNode(background, mBackgroundRenderNode);

        AutoPtr<IRenderNode> displayList = mBackgroundRenderNode;
        Boolean isValid;
        if (displayList != NULL && (displayList->IsValid(&isValid), isValid)) {
            SetBackgroundDisplayListProperties(displayList);
            (IHardwareCanvas::Probe(canvas))->DrawRenderNode(displayList);
            return;
        }
    }

    Int32 scrollX = mScrollX;
    Int32 scrollY = mScrollY;
    if ((scrollX | scrollY) == 0) {
        background->Draw(canvas);
    } else {
        canvas->Translate(scrollX, scrollY);
        background->Draw(canvas);
        canvas->Translate(-scrollX, -scrollY);
    }
}

/**
 * Set up background drawable display list properties.
 *
 * @param displayList Valid display list for the background drawable
 */
void View::SetBackgroundDisplayListProperties(
    /* [in] */ IRenderNode* displayList)
{
    Boolean res;
    displayList->SetTranslationX(mScrollX, &res);
    displayList->SetTranslationY(mScrollY, &res);
}

/**
 * Creates a new display list or updates the existing display list for the
 * specified Drawable.
 *
 * @param drawable Drawable for which to create a display list
 * @param renderNode Existing RenderNode, or {@code null}
 * @return A valid display list for the specified drawable
 */
AutoPtr<IRenderNode> View::GetDrawableRenderNode(
    /* [in] */ IDrawable* drawable,
    /* [in] */ IRenderNode* renderNode)
{
    if (renderNode == NULL) {
        assert(0 && "TODO");
        renderNode = RenderNode::Create(/*drawable.getClass().getName()*/, THIS_PROBE(IView));
    }

    AutoPtr<IRect> bounds;
    drawable->GetBounds((IRect**)&bounds);
    Int32 width, height;
    bounds->GetWidth(&width);
    bounds->GetHeight(&height);
    AutoPtr<IHardwareCanvas> canvas;
    renderNode->Start(width, height, (IHardwareCanvas**)&canvas);
    //try {
        drawable->Draw(canvas);
    //} finally {
        renderNode->End(canvas);
    //}

    // Set up drawable properties that are view-independent.
    AutoPtr<CRect> temp = (CRect*)bounds;
    Boolean res, isProjected;
    renderNode->SetLeftTopRightBottom(temp->mLeft, temp->mTop, temp->mRight, temp->mBottom, &res);
    drawable->IsProjected(&isProjected);
    renderNode->SetProjectBackwards(isProjected, &res);
    renderNode->SetProjectionReceiver(TRUE, &res);
    renderNode->SetClipToBounds(FALSE);
    return renderNode;
}

/**
 * Returns the overlay for this view, creating it if it does not yet exist.
 * Adding drawables to the overlay will cause them to be displayed whenever
 * the view itself is redrawn. Objects in the overlay should be actively
 * managed: remove them when they should not be displayed anymore. The
 * overlay will always have the same size as its host view.
 *
 * <p>Note: Overlays do not currently work correctly with {@link
 * SurfaceView} or {@link TextureView}; contents in overlays for these
 * types of views may not display correctly.</p>
 *
 * @return The ViewOverlay object for this view.
 * @see ViewOverlay
 */
ECode View::GetOverlay(
    /* [out] */ IViewOverlay** clipBounds)
{
    if (mOverlay == NULL) {
        CViewOverlay::New(mContext, THIS_PROBE(IView), (ViewOverlay**)&mOverlay);
    }
    return mOverlay;
}




/**
 * Override this if your view is known to always be drawn on top of a solid color background,
 * and needs to draw fading edges. Returning a non-zero color enables the view system to
 * optimize the drawing of the fading edges. If you do return a non-zero color, the alpha
 * should be set to 0xFF.
 *
 * @see #setVerticalFadingEdgeEnabled
 * @see #setHorizontalFadingEdgeEnabled
 *
 * @return The known solid color background for this view, or 0 if the color may vary
 */
Int32 View::GetSolidColor()
{
    return 0;
}

/**
 * Build a human readable string representation of the specified view flags.
 *
 * @param flags the view flags to convert to a string
 * @return a String representing the supplied flags
 */
ECode View::PrintFlags(
    /* [in] */ Int32 flags,
    /* [out] */ String* output)
{
    VALIDATE_NOT_NULL(output);

    StringBuilder sb("");
    Int32 numFlags = 0;
    if ((flags & FOCUSABLE_MASK) == FOCUSABLE) {
        sb += "TAKES_FOCUS";
        numFlags++;
    }

    switch (flags & VISIBILITY_MASK) {
    case IView::INVISIBLE:
        if (numFlags > 0) {
            sb += " ";
        }
        sb += "INVISIBLE";
        // USELESS HERE numFlags++;
        break;
    case IView::GONE:
        if (numFlags > 0) {
            sb += " ";
        }
        sb += "GONE";
        // USELESS HERE numFlags++;
        break;
    default:
        break;
    }
    *output = sb.ToString();
    return NOERROR;
}

/**
 * Build a human readable string representation of the specified private
 * view flags.
 *
 * @param privateFlags the private view flags to convert to a string
 * @return a String representing the supplied flags
 */
ECode View::PrintPrivateFlags(
    /* [in] */ Int32 privateFlags,
    /* [out] */ String* output)
{
    VALIDATE_NOT_NULL(output);

    StringBuilder sb("");
    Int32 numFlags = 0;

    if ((privateFlags & PFLAG_WANTS_FOCUS) == PFLAG_WANTS_FOCUS) {
        sb += "WANTS_FOCUS";
        numFlags++;
    }

    if ((privateFlags & PFLAG_FOCUSED) == PFLAG_FOCUSED) {
        if (numFlags > 0) {
            sb += " ";
        }
        sb += "FOCUSED";
        numFlags++;
    }

    if ((privateFlags & PFLAG_SELECTED) == PFLAG_SELECTED) {
        if (numFlags > 0) {
            sb += " ";
        }
        sb += "SELECTED";
        numFlags++;
    }

    if ((privateFlags & PFLAG_IS_ROOT_NAMESPACE) == PFLAG_IS_ROOT_NAMESPACE) {
        if (numFlags > 0) {
            sb += " ";
        }
        sb += "IS_ROOT_NAMESPACE";
        numFlags++;
    }

    if ((privateFlags & PFLAG_HAS_BOUNDS) == PFLAG_HAS_BOUNDS) {
        if (numFlags > 0) {
            sb += " ";
        }
        sb += "HAS_BOUNDS";
        numFlags++;
    }

    if ((privateFlags & PFLAG_DRAWN) == PFLAG_DRAWN) {
        if (numFlags > 0) {
            sb += " ";
        }
        sb += "DRAWN";
        // USELESS HERE numFlags++;
    }
    *output = sb.ToString();
    return NOERROR;
}

/**
 * <p>Indicates whether or not this view's layout will be requested during
 * the next hierarchy layout pass.</p>
 *
 * @return TRUE if the layout will be forced during next layout pass
 */
Boolean View::IsLayoutRequested()
{
    return (mPrivateFlags & PFLAG_FORCE_LAYOUT) == PFLAG_FORCE_LAYOUT;
}

/**
 * Return true if o is a ViewGroup that is laying out using optical bounds.
 * @hide
 */
Boolean View::IsLayoutModeOptical(
    /* [in] */ IInterface* o)
{
    Boolean isLayoutModeOptical;
    if (IViewGroup::Probe(o)) {
        ((ViewGroup*)(IViewGroup::Probe(o)))->IsLayoutModeOptical(&isLayoutModeOptical)
        if (isLayoutModeOptical) {
            return TRUE;
        }
    }
    return FALSE;
}

Boolean View::SetOpticalFrame(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    AutoPtr<IInsets> parentInsets;
    if (IView::Probe(mParent)) {
        (IView::Probe(mParent))->GetOpticalInsets((IInsets**)&parentInsets);
    } else {
        parentInsets = Insets::NONE;
    }
    AutoPtr<IInsets> childInsets;
    GetOpticalInsets((IInsets**)&childInsets);
    CInsets* tempParent = (CInsets*)parentInsets;
    CInsets* tempChild = (CInsets*)childInsets;
    return SetFrame(
            left   + tempParent->mLeft - tempChild->mLeft,
            top    + tempParent->mTop  - tempChild->mTop,
            right  + tempParent->mLeft + tempChild->mRight,
            bottom + tempParent->mTop  + tempChild->mBottom);
}

/**
 * Assign a size and position to a view and all of its
 * descendants
 *
 * <p>This is the second phase of the layout mechanism.
 * (The first is measuring). In this phase, each parent calls
 * layout on all of its children to position them.
 * This is typically done using the child measurements
 * that were stored in the measure pass().
 *
 * Derived classes with children should override
 * onLayout. In that method, they should
 * call layout on each of their their children.
 *
 * @param l Left position, relative to parent
 * @param t Top position, relative to parent
 * @param r Right position, relative to parent
 * @param b Bottom position, relative to parent
 */
ECode View::Layout(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    if ((mPrivateFlags3 & PFLAG3_MEASURE_NEEDED_BEFORE_LAYOUT) != 0) {
        OnMeasure(mOldWidthMeasureSpec, mOldHeightMeasureSpec);
        mPrivateFlags3 &= ~PFLAG3_MEASURE_NEEDED_BEFORE_LAYOUT;
    }

    Int32 oldL = mLeft;
    Int32 oldT = mTop;
    Int32 oldB = mBottom;
    Int32 oldR = mRight;

    Boolean changed = IsLayoutModeOptical(mParent) ?
        SetOpticalFrame(l, t, r, b) : SetFrame(l, t, r, b);
    if (changed || (mPrivateFlags & PFLAG_LAYOUT_REQUIRED) == PFLAG_LAYOUT_REQUIRED) {
        OnLayout(changed, l, t, r, b);
        mPrivateFlags &= ~PFLAG_LAYOUT_REQUIRED;

        AutoPtr<ListenerInfo> li = mListenerInfo;
        if (li != NULL && li->mOnLayoutChangeListeners.IsEmpty() == FALSE) {
            //TODO : clone
            // ArrayList<OnLayoutChangeListener> listenersCopy =
            //         (ArrayList<OnLayoutChangeListener>)li.mOnLayoutChangeListeners.clone();
            // int numListeners = listenersCopy.size();
            // for (int i = 0; i < numListeners; ++i) {
            //     listenersCopy.get(i).onLayoutChange(this, l, t, r, b, oldL, oldT, oldR, oldB);
            // }

            Int32 numListeners = li->mOnLayoutChangeListeners.GetSize();
            for (Int32 i = 0; i < numListeners; ++i) {
                li->mOnLayoutChangeListeners[i]->OnLayoutChange(IVIEW_PROBE(this),
                        l, t, r, b, oldL, oldT, oldR, oldB);
            }
        }
    }
    mPrivateFlags &= ~PFLAG_FORCE_LAYOUT;
    mPrivateFlags3 |= PFLAG3_IS_LAID_OUT;

    return NOERROR;
}

/**
 * Called from layout when this view should
 * assign a size and position to each of its children.
 *
 * Derived classes with children should override
 * this method and call layout on each of
 * their children.
 * @param changed This is a new size or position for this view
 * @param left Left position, relative to parent
 * @param top Top position, relative to parent
 * @param right Right position, relative to parent
 * @param bottom Bottom position, relative to parent
 */
void View::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
}

/**
 * Assign a size and position to this view.
 *
 * This is called from layout.
 *
 * @param left Left position, relative to parent
 * @param top Top position, relative to parent
 * @param right Right position, relative to parent
 * @param bottom Bottom position, relative to parent
 * @return TRUE if the new size and position are different than the
 *         previous ones
 * {@hide}
 */
Boolean View::SetFrame(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    //printf("ID=0x%08x, 0x%08x, View::SetFrame\n", mID, this);
    Boolean changed = FALSE;

    if (DBG) {
        Logger::D(VIEW_LOG_TAG, "this = 0x%08x View.setFrame(%d, %d, %d, %d)",
            left, top, right, bottom);
    }

    if (mLeft != left || mRight != right || mTop != top || mBottom != bottom) {
        //printf("left = %d, right = %d, top = %d, bottom = %d\n", left, right, top, bottom);
        changed = TRUE;

        // Remember our drawn bit
        Int32 drawn = mPrivateFlags & PFLAG_DRAWN;

        Int32 oldWidth = mRight - mLeft;
        Int32 oldHeight = mBottom - mTop;
        Int32 newWidth = right - left;
        Int32 newHeight = bottom - top;
        Boolean sizeChanged = (newWidth != oldWidth) || (newHeight != oldHeight);

        // Invalidate our old position
        Invalidate(sizeChanged);

        mLeft = left;
        mTop = top;
        mRight = right;
        mBottom = bottom;
        Boolean res;
        mRenderNode->SetLeftTopRightBottom(mLeft, mTop, mRight, mBottom, &res);

        mPrivateFlags |= PFLAG_HAS_BOUNDS;

        if (sizeChanged) {
            SizeChange(newWidth, newHeight, oldWidth, oldHeight);
        }

        if ((mViewFlags & VISIBILITY_MASK) == IView::VISIBLE) {
            // If we are visible, force the DRAWN bit to on so that
            // this invalidate will go through (at least to our parent).
            // This is because someone may have invalidated this view
            // before this call to setFrame came in, thereby clearing
            // the DRAWN bit.
            mPrivateFlags |= PFLAG_DRAWN;
            Invalidate(sizeChanged);
            // parent display list may need to be recreated based on a change in the bounds
            // of any child
            InvalidateParentCaches();
        }

        // Reset drawn bit to original value (invalidate turns it off)
        mPrivateFlags |= drawn;

        mBackgroundSizeChanged = TRUE;

        NotifySubtreeAccessibilityStateChangedIfNeeded();
    }
    return changed;
}

ECode View::SizeChange(
    /* [in] */ Int32 newWidth,
    /* [in] */ Int32 newHeight,
    /* [in] */ Int32 oldWidth,
    /* [in] */ Int32 oldHeight)
{
    OnSizeChanged(newWidth, newHeight, oldWidth, oldHeight);
    if (mOverlay != NULL) {
        AutoPtr<IViewGroup> group;
        mOverlay->GetOverlayView((IViewGroup**)&group);
        (IView::Probe(group))->SetRight(newWidth);
        (IView::Probe(group))->SetBottom(newHeight);
    }
    mPrivateFlags3 |= PFLAG3_OUTLINE_INVALID;
}

/**
 * Finalize inflating a view from XML.  This is called as the last phase
 * of inflation, after all child views have been added.
 *
 * <p>Even if the subclass overrides onFinishInflate, they should always be
 * sure to call the super method, so that we get called.
 */
ECode View::OnFinishInflate()
{
    return NOERROR;
}

/**
 * Returns the resources associated with this view.
 *
 * @return Resources object.
 */
AutoPtr<IResources> View::GetResources()
{
    return mResources;
}

/**
 * Invalidates the specified Drawable.
 *
 * @param drawable the drawable to invalidate
 */
ECode View::InvalidateDrawable(
    /* [in] */ IDrawable* drawable)
{
    if (VerifyDrawable(drawable)) {
        AutoPtr<IRect> dirty;
        drawable->GetBounds((IRect**)&dirty);
        Int32 scrollX = mScrollX;
        Int32 scrollY = mScrollY;

        CRect* dirtyObj = (CRect*)dirty.Get();
        Invalidate(dirtyObj->mLeft + scrollX, dirtyObj->mTop + scrollY,
                dirtyObj->mRight + scrollX, dirtyObj->mBottom + scrollY);

        mPrivateFlags3 |= PFLAG3_OUTLINE_INVALID;
    }
    return NOERROR;
}

/**
 * Schedules an action on a drawable to occur at a specified time.
 *
 * @param who the recipient of the action
 * @param what the action to run on the drawable
 * @param when the time at which the action must occur. Uses the
 *        {@link SystemClock#uptimeMillis} timebase.
 */
ECode View::ScheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what,
    /* [in] */ Int64 when)
{
    if (VerifyDrawable(who) && what != NULL) {
        Int64 delay = when - SystemClock::GetUptimeMillis();
        if (mAttachInfo != NULL) {
            //assert(0);
            //TODO
            AutoPtr<IChoreographerHelper> helper;
            CChoreographerHelper::AcquireSingleton((IChoreographerHelper**)&helper);
            Int64 delayTime = 0;
            helper->SubtractFrameDelay(delay, &delayTime);
            mAttachInfo->mViewRootImpl->mChoreographer->PostCallbackDelayed(
                IChoreographer::CALLBACK_ANIMATION, what, who, delayTime);
        }
        else {
            ViewRootImpl::GetRunQueue()->PostDelayed(what, delay);
        }
    }

    return NOERROR;
}

/**
 * Cancels a scheduled action on a drawable.
 *
 * @param who the recipient of the action
 * @param what the action to cancel
 */
ECode View::UnscheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what)
{
    if (VerifyDrawable(who) && what != NULL) {
        if (mAttachInfo != NULL) {
            mAttachInfo->mViewRootImpl->mChoreographer->RemoveCallbacks(
                IChoreographer::CALLBACK_ANIMATION, what, who);
        }
        ViewRootImpl::GetRunQueue()->RemoveCallbacks(what);
    }

    return NOERROR;
}

/**
 * Unschedule any events associated with the given Drawable.  This can be
 * used when selecting a new Drawable into a view, so that the previous
 * one is completely unscheduled.
 *
 * @param who The Drawable to unschedule.
 *
 * @see #drawableStateChanged
 */
ECode View::UnscheduleDrawable(
    /* [in] */ IDrawable* who)
{
    if (mAttachInfo != NULL && who != NULL) {
        mAttachInfo->mViewRootImpl->mChoreographer->RemoveCallbacks(
            IChoreographer::CALLBACK_ANIMATION, NULL, who);
    }

    return NOERROR;
}

void View::ResolveDrawables()
{
    // Drawables resolution may need to happen before resolving the layout direction (which is
    // done only during the measure() call).
    // If the layout direction is not resolved yet, we cannot resolve the Drawables except in
    // one case: when the raw layout direction has not been defined as LAYOUT_DIRECTION_INHERIT.
    // So, if the raw layout direction is LAYOUT_DIRECTION_LTR or LAYOUT_DIRECTION_RTL or
    // LAYOUT_DIRECTION_LOCALE, we can "cheat" and we don't need to wait for the layout
    // direction to be resolved as its resolved value will be the same as its raw value.
    Boolean isLayoutDirectionResolved;
    IsLayoutDirectionResolved(&isLayoutDirectionResolved);
    Int32 rawLayoutDirection;
    GetRawLayoutDirection(&rawLayoutDirection);
    if (!isLayoutDirectionResolved && rawLayoutDirection == IView::LAYOUT_DIRECTION_INHERIT) {
        return;
    }

    Int32 layoutDirection;
    if (isLayoutDirectionResolved) {
        GetLayoutDirection(&layoutDirection);
    } else {
        GetRawLayoutDirection(&layoutDirection);
    }

    Int32 direction;
    GetLayoutDirection(&direction);
    if (mBackground != NULL) {
        mBackground->SetLayoutDirection(direction);
    }
    mPrivateFlags2 |= PFLAG2_DRAWABLE_RESOLVED;
    OnResolveDrawables(direction);
}

ECode View::OnResolveDrawables(
    /* [in] */ Int64 layoutDirection)
{
    return NOERROR;
}

void View::ResetResolvedDrawables()
{
    mPrivateFlags2 &= ~PFLAG2_DRAWABLE_RESOLVED;
}

Boolean View::IsDrawablesResolved()
{
    return (mPrivateFlags2 & PFLAG2_DRAWABLE_RESOLVED) == PFLAG2_DRAWABLE_RESOLVED;
}

/**
 * If your view subclass is displaying its own Drawable objects, it should
 * override this function and return TRUE for any Drawable it is
 * displaying.  This allows animations for those drawables to be
 * scheduled.
 *
 * <p>Be sure to call through to the super class when overriding this
 * function.
 *
 * @param who The Drawable to verify.  Return TRUE if it is one you are
 *            displaying, else return the result of calling through to the
 *            super class.
 *
 * @return boolean If TRUE than the Drawable is being displayed in the
 *         view; else false and it is not allowed to animate.
 *
 * @see #unscheduleDrawable(android.graphics.drawable.Drawable)
 * @see #drawableStateChanged()
 */
Boolean View::VerifyDrawable(
    /* [in] */ IDrawable* who)
{
    return who == mBackground;
}

/**
 * This function is called whenever the state of the view changes in such
 * a way that it impacts the state of drawables being shown.
 *
 * <p>Be sure to call through to the superclass when overriding this
 * function.
 *
 * @see Drawable#setState
 */
ECode View::DrawableStateChanged()
{
    AutoPtr<IDrawable> d = mBackground;
    if (d != NULL) {
        Boolean stateful;
        d->IsStateful(&stateful);

        if (stateful) {
            AutoPtr<ArrayOf<Int32> > drawableState = GetDrawableState();
            d->SetState(drawableState, &stateful);
        }
    }

    if (mStateListAnimator != NULL) {
        AutoPtr< ArrayOf<Int32> > temp;
        GetDrawableState((ArrayOf<Int32>**)&temp);
        mStateListAnimator->SetState(temp);
    }
    return NOERROR;
}

/**
 * This function is called whenever the view hotspot changes and needs to
 * be propagated to drawables managed by the view.
 * <p>
 * Be sure to call through to the superclass when overriding this function.
 *
 * @param x hotspot x coordinate
 * @param y hotspot y coordinate
 */
ECode View::DrawableHotspotChanged(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    if (mBackground != NULL) {
        mBackground->SetHotspot(x, y);
    }
}

/**
 * Call this to force a view to update its drawable state. This will cause
 * drawableStateChanged to be called on this view. Views that are interested
 * in the new state should call getDrawableState.
 *
 * @see #drawableStateChanged
 * @see #getDrawableState
 */
ECode View::RefreshDrawableState()
{
    mPrivateFlags |= PFLAG_DRAWABLE_STATE_DIRTY;
    DrawableStateChanged();

    AutoPtr<IViewParent> parent = mParent;
    if (parent != NULL) {
        parent->ChildDrawableStateChanged(IVIEW_PROBE(this));
    }
    return NOERROR;
}

/**
 * Return an array of resource IDs of the drawable states representing the
 * current state of the view.
 *
 * @return The current drawable state
 *
 * @see Drawable#setState
 * @see #drawableStateChanged
 * @see #onCreateDrawableState
 */
ECode View::GetDrawableState(
    /* [out, callee] */ ArrayOf<Int32>** res)
{
    VALIDATE_NOT_NULL(res)
    if ((mDrawableState != NULL) && ((mPrivateFlags & PFLAG_DRAWABLE_STATE_DIRTY) == 0)) {
        *res = mDrawableState
        REFCOUNT_ADD(*res)
        return NOERROR;
    }
    else {
        AutoPtr<ArrayOf<Int32> > states;
        OnCreateDrawableState(0, (ArrayOf<Int32>**)&states);
        mDrawableState = states;
        mPrivateFlags &= ~PFLAG_DRAWABLE_STATE_DIRTY;
        *res = mDrawableState
        REFCOUNT_ADD(*res)
        return NOERROR;
    }
}

/**
 * Generate the new {@link android.graphics.drawable.Drawable} state for
 * this view. This is called by the view
 * system when the cached Drawable state is determined to be invalid.  To
 * retrieve the current state, you should use {@link #getDrawableState}.
 *
 * @param extraSpace if non-zero, this is the number of extra entries you
 * would like in the returned array in which you can place your own
 * states.
 *
 * @return Returns an array holding the current {@link Drawable} state of
 * the view.
 *
 * @see #mergeDrawableStates
 */
ECode View::OnCreateDrawableState(
    /* [in] */ Int32 extraSpace,
    /* [out] */ ArrayOf<Int32>** _drawableState)
{
    VALIDATE_NOT_NULL(_drawableState);
    if ((mViewFlags & DUPLICATE_PARENT_STATE) == DUPLICATE_PARENT_STATE &&
            mParent != NULL && IVIEW_PROBE(mParent) != NULL) {
        return VIEW_PROBE(mParent)->OnCreateDrawableState(extraSpace, _drawableState);
    }

    Int32 privateFlags = mPrivateFlags;

    Int32 viewStateIndex = 0;
    if ((privateFlags & PFLAG_PRESSED) != 0) viewStateIndex |= VIEW_STATE_PRESSED;
    if ((mViewFlags & ENABLED_MASK) == ENABLED) viewStateIndex |= VIEW_STATE_ENABLED;
    if (IsFocused()) viewStateIndex |= VIEW_STATE_FOCUSED;
    if ((privateFlags & PFLAG_SELECTED) != 0) viewStateIndex |= VIEW_STATE_SELECTED;
    if (HasWindowFocus()) viewStateIndex |= VIEW_STATE_WINDOW_FOCUSED;
    if ((privateFlags & PFLAG_ACTIVATED) != 0) viewStateIndex |= VIEW_STATE_ACTIVATED;

    // Boolean available = FALSE;
    // if (mAttachInfo != NULL && mAttachInfo->mHardwareAccelerationRequested &&
    //         (HardwareRenderer::IsAvailable(&available), available)) {
    //     // This is set if HW acceleration is requested, even if the current
    //     // process doesn't allow it.  This is just to allow app preview
    //     // windows to better match their app.
    //     viewStateIndex |= VIEW_STATE_ACCELERATED;
    // }
    if ((privateFlags & PFLAG_HOVERED) != 0) viewStateIndex |= VIEW_STATE_HOVERED;

    const Int32 privateFlags2 = mPrivateFlags2;
    if ((privateFlags2 & PFLAG2_DRAG_CAN_ACCEPT) != 0) viewStateIndex |= VIEW_STATE_DRAG_CAN_ACCEPT;
    if ((privateFlags2 & PFLAG2_DRAG_HOVERED) != 0) viewStateIndex |= VIEW_STATE_DRAG_HOVERED;

    AutoPtr<ArrayOf<Int32> > drawableState = (*VIEW_STATE_SETS)[viewStateIndex];

    //noinspection ConstantIfStatement
//    if (FALSE) {
//        Log.i(VIEW_LOG_TAG, "drawableStateIndex=" + viewStateIndex);
//        Log.i(VIEW_LOG_TAG, toString()
//                + " pressed=" + ((privateFlags & PFLAG_PRESSED) != 0)
//                + " en=" + ((mViewFlags & ENABLED_MASK) == ENABLED)
//                + " fo=" + hasFocus()
//                + " sl=" + ((privateFlags & PFLAG_SELECTED) != 0)
//                + " wf=" + hasWindowFocus
//                + ": " + Arrays.toString(drawableState));
//    }

    AutoPtr<ArrayOf<Int32> > fullState;
    if (drawableState != NULL) {
        const Int32 len = drawableState->GetLength();
        fullState = ArrayOf<Int32>::Alloc(len + extraSpace);
        fullState->Copy(drawableState, 0, len);
    }
    else {
        fullState = ArrayOf<Int32>::Alloc(extraSpace);
    }

    *_drawableState = fullState;
    REFCOUNT_ADD(*_drawableState);
    return NOERROR;
}

/**
 * Merge your own state values in <var>additionalState</var> into the base
 * state values <var>baseState</var> that were returned by
 * {@link #onCreateDrawableState}.
 *
 * @param baseState The base state values returned by
 * {@link #onCreateDrawableState}, which will be modified to also hold your
 * own additional state values.
 *
 * @param additionalState The additional state values you would like
 * added to <var>baseState</var>; this array is not modified.
 *
 * @return As a convenience, the <var>baseState</var> array you originally
 * passed into the function is returned.
 *
 * @see #onCreateDrawableState
 */
AutoPtr<ArrayOf<Int32> > View::MergeDrawableStates(
    /* [in] */ ArrayOf<Int32>* baseState,
    /* [in] */ ArrayOf<Int32>* additionalState)
{
    const Int32 N = baseState->GetLength();
    Int32 i = N - 1;
    while (i >= 0 && (*baseState)[i] == 0) {
        i--;
    }
    i++;
    const Int32 len = i + additionalState->GetLength() <= N?
            i + additionalState->GetLength() : N;
    for (Int32 j = 0; i < len; ++i, ++j) {
        (*baseState)[i] = (*additionalState)[j];
    }
    return baseState;
}

/**
 * Call {@link Drawable#jumpToCurrentState() Drawable.jumpToCurrentState()}
 * on all Drawable objects associated with this view.
 * <p>
 * Also calls {@link StateListAnimator#jumpToCurrentState()} if there is a StateListAnimator
 * attached to this view.
 */
ECode View::JumpDrawablesToCurrentState()
{
    if (mBackground != NULL) {
        mBackground->JumpToCurrentState();
    }

    if (mStateListAnimator != NULL) {
        mStateListAnimator->JumpToCurrentState();
    }

    return NOERROR;
}

/**
 * Sets the background color for this view.
 * @param color the color of the background
 */
ECode View::SetBackgroundColor(
    /* [in] */ Int32 color)
{
    if (IColorDrawable::Probe(mBackground) != NULL) {
        AutoPtr<IDrawable> drawable;
        mBackground->Mutate((IDrawable**)&drawable);
        assert(drawable != NULL);
        IColorDrawable::Probe(drawable)->SetColor(color);
        ComputeOpaqueFlags();
        mBackgroundResource = 0;
    }
    else {
        AutoPtr<IColorDrawable> colorDrawable;
        CColorDrawable::New(color, (IColorDrawable**)&colorDrawable);
        SetBackground(colorDrawable);
    }

    return NOERROR;
}

/**
 * Set the background to a given resource. The resource should refer to
 * a Drawable object or 0 to remove the background.
 * @param resid The identifier of the resource.
 * @attr ref android.R.styleable#View_background
 */
ECode View::SetBackgroundResource(
    /* [in] */ Int32 resid)
{
    if (resid != 0 && resid == mBackgroundResource) {
        return NOERROR;
    }

    AutoPtr<IDrawable> d;
    if (resid != 0) {
        mContext->GetDrawable(resid, (IDrawable**)&d);
    }
    SetBackground(d.Get());

    mBackgroundResource = resid;

    return NOERROR;
}

ECode View::SetBackground(
    /* [in] */ IDrawable* background)
{
    //noinspection deprecation
    return SetBackgroundDrawable(background);
}

/**
 * Set the background to a given Drawable, or remove the background. If the
 * background has padding, this View's padding is set to the background's
 * padding. However, when a background is removed, this View's padding isn't
 * touched. If setting the padding is desired, please use
 * {@link #setPadding(int, int, int, int)}.
 *
 * @param d The Drawable to use as the background, or NULL to remove the
 *        background
 */
ECode View::SetBackgroundDrawable(
    /* [in] */ IDrawable* background)
{
    ComputeOpaqueFlags();

    if (background == mBackground) {
        return NOERROR;
    }

    Boolean requestLayout = FALSE;

    mBackgroundResource = 0;

    /*
     * Regardless of whether we're setting a new background or not, we want
     * to clear the previous drawable.
     */
    if (mBackground != NULL) {
        mBackground->SetCallback(NULL);
        UnscheduleDrawable(mBackground);
    }

    if (background != NULL) {
        AutoPtr<CRect> padding = (CRect*)pthread_getspecific(sKey);

        if (padding == NULL) {
            CRect::NewByFriend((CRect**)&padding);
            pthread_setspecific(sKey, padding.Get());
            padding->AddRef();
        }

        ResetResolvedDrawables();
        background->SetLayoutDirection(GetLayoutDirection());
        Boolean result;
        background->GetPadding((IRect*)padding.Get(), &result);
        if (result) {
            ResetResolvedPadding();
            Int32 direction = 0;
            background->GetLayoutDirection(&direction);
            switch (direction) {
                case IView::LAYOUT_DIRECTION_RTL:
                    mUserPaddingLeftInitial = padding->mRight;
                    mUserPaddingRightInitial = padding->mLeft;
                    InternalSetPadding(padding->mRight, padding->mTop, padding->mLeft, padding->mBottom);
                    break;
                case IView::LAYOUT_DIRECTION_LTR:
                default:
                    mUserPaddingLeftInitial = padding->mLeft;
                    mUserPaddingRightInitial = padding->mRight;
                    InternalSetPadding(padding->mLeft, padding->mTop, padding->mRight, padding->mBottom);
            }

            mLeftPaddingDefined = FALSE;
            mRightPaddingDefined = FALSE;
        }

        // Compare the minimum sizes of the old Drawable and the new.  If there isn't an old or
        // if it has a different minimum size, we should layout again
        if (mBackground == NULL) {
            requestLayout = TRUE;
        }
        else {
            Int32 bWidth, dWidth, bHeight, dHeight;
            mBackground->GetMinimumHeight(&bHeight);
            mBackground->GetMinimumWidth(&bWidth);
            background->GetMinimumHeight(&dHeight);
            background->GetMinimumWidth(&dWidth);
            if (bHeight != dHeight || bWidth != dWidth) {
                requestLayout = TRUE;
            }
        }

        background->SetCallback(THIS_PROBE(IDrawableCallback));
        background->IsStateful(&result);
        if (result) {
            Boolean stateful;
            AutoPtr<ArrayOf<Int32> > drawableState = GetDrawableState();
            background->SetState(drawableState, &stateful);
        }
        background->SetVisible(GetVisibility() == IView::VISIBLE, FALSE, &result);
        mBackground = background;

        ApplyBackgroundTint();

        if ((mPrivateFlags & PFLAG_SKIP_DRAW) != 0) {
            mPrivateFlags &= ~PFLAG_SKIP_DRAW;
            mPrivateFlags |= PFLAG_ONLY_DRAWS_BACKGROUND;
            requestLayout = TRUE;
        }
    }
    else {
        /* Remove the background */
        mBackground = NULL;

        if ((mPrivateFlags & PFLAG_ONLY_DRAWS_BACKGROUND) != 0) {
            /*
                * This view ONLY drew the background before and we're removing
                * the background, so now it won't draw anything
                * (hence we PFLAG_SKIP_DRAW)
                */
            mPrivateFlags &= ~PFLAG_ONLY_DRAWS_BACKGROUND;
            mPrivateFlags |= PFLAG_SKIP_DRAW;
        }

        /*
         * When the background is set, we try to apply its padding to this
         * View. When the background is removed, we don't touch this View's
         * padding. This is noted in the Javadocs. Hence, we don't need to
         * requestLayout(), the invalidate() below is sufficient.
         */

        // The old background's minimum size could have affected this
        // View's layout, so let's requestLayout
        requestLayout = TRUE;
    }

    ComputeOpaqueFlags();

    if (requestLayout) {
        RequestLayout();
    }

    mBackgroundSizeChanged = TRUE;
    Invalidate(TRUE);

    return NOERROR;
}


/**
 * Gets the background drawable
 * @return The drawable used as the background for this view, if any.
 */
AutoPtr<IDrawable> View::GetBackground()
{
    return mBackground;
}

/**
 * Applies a tint to the background drawable. Does not modify the current tint
 * mode, which is {@link PorterDuff.Mode#SRC_IN} by default.
 * <p>
 * Subsequent calls to {@link #setBackground(Drawable)} will automatically
 * mutate the drawable and apply the specified tint and tint mode using
 * {@link Drawable#setTintList(ColorStateList)}.
 *
 * @param tint the tint to apply, may be {@code null} to clear tint
 *
 * @attr ref android.R.styleable#View_backgroundTint
 * @see #getBackgroundTintList()
 * @see Drawable#setTintList(ColorStateList)
 */
ECode View::SetBackgroundTintList(
    /* [in] */ IColorStateList tint)
{
    if (mBackgroundTint == NULL) {
        mBackgroundTint = new TintInfo();
    }
    mBackgroundTint->mTintList = tint;
    mBackgroundTint->mHasTintList = TRUE;

    ApplyBackgroundTint();
}

/**
 * Return the tint applied to the background drawable, if specified.
 *
 * @return the tint applied to the background drawable
 * @attr ref android.R.styleable#View_backgroundTint
 * @see #setBackgroundTintList(ColorStateList)
 */
ECode View::GetBackgroundTintList(
    /* [out] */ IColorStateList** res)
{
    VALIDATE_NOT_NULL(res);
    if (mBackgroundTint) {
        *res = mBackgroundTint->mTintList;
        REFCOUNT_ADD(*res)
    } else {
        *res = NULL;
    }
    return NOERROR;
}

/**
 * Specifies the blending mode used to apply the tint specified by
 * {@link #setBackgroundTintList(ColorStateList)}} to the background
 * drawable. The default mode is {@link PorterDuff.Mode#SRC_IN}.
 *
 * @param tintMode the blending mode used to apply the tint, may be
 *                 {@code null} to clear tint
 * @attr ref android.R.styleable#View_backgroundTintMode
 * @see #getBackgroundTintMode()
 * @see Drawable#setTintMode(PorterDuff.Mode)
 */
ECode View::SetBackgroundTintMode(
     /* [in] */ PorterDuffMode tintMode)
{
    if (mBackgroundTint == NULL) {
        mBackgroundTint = new TintInfo();
    }
    mBackgroundTint->mTintMode = tintMode;
    mBackgroundTint->mHasTintMode = TRUE;

    ApplyBackgroundTint();
}

/**
 * Return the blending mode used to apply the tint to the background
 * drawable, if specified.
 *
 * @return the blending mode used to apply the tint to the background
 *         drawable
 * @attr ref android.R.styleable#View_backgroundTintMode
 * @see #setBackgroundTintMode(PorterDuff.Mode)
 */
ECode View::GetBackgroundTintMode(
    /* [out] */ PorterDuffMode* res)
{
    VALIDATE_NOT_NULL(res)
    if (mBackgroundTint) {
        res = mBackgroundTint->mTintMode;
    } else {
        *res = NULL;
    }
    return NOERROR;
}

ECode View::ApplyBackgroundTint()
{
    if (mBackground != NULL && mBackgroundTint != NULL) {
        AutoPtr<TintInfo> tintInfo = mBackgroundTint;
        if (tintInfo->mHasTintList || tintInfo->mHasTintMode) {
            mBackground->Mutate((IDrawable**)&mBackground);

            if (tintInfo->mHasTintList) {
                mBackground->SetTintList(tintInfo->mTintList);
            }

            if (tintInfo->mHasTintMode) {
                mBackground->SetTintMode(tintInfo->mTintMode);
            }
        }
    }
}

/**
 * Sets the padding. The view may add on the space required to display
 * the scrollbars, depending on the style and visibility of the scrollbars.
 * So the values returned from {@link #getPaddingLeft}, {@link #getPaddingTop},
 * {@link #getPaddingRight} and {@link #getPaddingBottom} may be different
 * from the values set in this call.
 *
 * @attr ref android.R.styleable#View_padding
 * @attr ref android.R.styleable#View_paddingBottom
 * @attr ref android.R.styleable#View_paddingLeft
 * @attr ref android.R.styleable#View_paddingRight
 * @attr ref android.R.styleable#View_paddingTop
 * @param left the left padding in pixels
 * @param top the top padding in pixels
 * @param right the right padding in pixels
 * @param bottom the bottom padding in pixels
 */
ECode View::SetPadding(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    ResetResolvedPadding();

    mUserPaddingStart = UNDEFINED_PADDING;
    mUserPaddingEnd = UNDEFINED_PADDING;

    mUserPaddingLeftInitial = left;
    mUserPaddingRightInitial = right;

    mLeftPaddingDefined = TRUE;
    mRightPaddingDefined = TRUE;

    InternalSetPadding(left, top, right, bottom);
    return NOERROR;
}

void View::InternalSetPadding(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    mUserPaddingLeft = left;
    mUserPaddingRight = right;
    mUserPaddingBottom = bottom;

    const Int32 viewFlags = mViewFlags;
    Boolean changed = FALSE;

    // Common case is there are no scroll bars.
    if ((viewFlags & (SCROLLBARS_VERTICAL|SCROLLBARS_HORIZONTAL)) != 0) {
        if ((viewFlags & SCROLLBARS_VERTICAL) != 0) {
            const Int32 offset = (viewFlags & SCROLLBARS_INSET_MASK) == 0
                    ? 0 : GetVerticalScrollbarWidth();
            switch (mVerticalScrollbarPosition) {
                case IView::SCROLLBAR_POSITION_DEFAULT:
                    if (IsLayoutRtl()) {
                        left += offset;
                    }
                    else {
                        right += offset;
                    }
                    break;
                case IView::SCROLLBAR_POSITION_RIGHT:
                    right += offset;
                    break;
                case IView::SCROLLBAR_POSITION_LEFT:
                    left += offset;
                    break;
            }
        }
        if ((viewFlags & SCROLLBARS_HORIZONTAL) != 0) {
            bottom += (viewFlags & SCROLLBARS_INSET_MASK) == 0
                    ? 0 : GetHorizontalScrollbarHeight();
        }
    }

    if (mPaddingLeft != left) {
        changed = TRUE;
        mPaddingLeft = left;
    }
    if (mPaddingTop != top) {
        changed = TRUE;
        mPaddingTop = top;
    }
    if (mPaddingRight != right) {
        changed = TRUE;
        mPaddingRight = right;
    }
    if (mPaddingBottom != bottom) {
        changed = TRUE;
        mPaddingBottom = bottom;
    }

    if (changed) {
        RequestLayout();
    }
}

ECode View::SetPaddingRelative(
    /* [in] */ Int32 start,
    /* [in] */ Int32 top,
    /* [in] */ Int32 end,
    /* [in] */ Int32 bottom)
{
    ResetResolvedPadding();

    mUserPaddingStart = start;
    mUserPaddingEnd = end;

    mLeftPaddingDefined = TRUE;
    mRightPaddingDefined = TRUE;

    switch(GetLayoutDirection()) {
        case IView::LAYOUT_DIRECTION_RTL:
            mUserPaddingLeftInitial = end;
            mUserPaddingRightInitial = start;
            InternalSetPadding(end, top, start, bottom);
            break;
        case IView::LAYOUT_DIRECTION_LTR:
        default:
            mUserPaddingLeftInitial = start;
            mUserPaddingRightInitial = end;
            InternalSetPadding(start, top, end, bottom);
    }

    return NOERROR;
}

/**
 * Returns the top padding of this view.
 *
 * @return the top padding in pixels
 */
Int32 View::GetPaddingTop()
{
    return mPaddingTop;
}

/**
 * Returns the bottom padding of this view. If there are inset and enabled
 * scrollbars, this value may include the space required to display the
 * scrollbars as well.
 *
 * @return the bottom padding in pixels
 */
Int32 View::GetPaddingBottom()
{
    return mPaddingBottom;
}

/**
 * Returns the left padding of this view. If there are inset and enabled
 * scrollbars, this value may include the space required to display the
 * scrollbars as well.
 *
 * @return the left padding in pixels
 */
Int32 View::GetPaddingLeft()
{
    if (!IsPaddingResolved()) {
        ResolvePadding();
    }

    return mPaddingLeft;
}

Int32 View::GetPaddingStart() {
    if (!IsPaddingResolved()) {
        ResolvePadding();
    }
    return (GetLayoutDirection() == IView::LAYOUT_DIRECTION_RTL) ?
            mPaddingRight : mPaddingLeft;
}

/**
 * Returns the right padding of this view. If there are inset and enabled
 * scrollbars, this value may include the space required to display the
 * scrollbars as well.
 *
 * @return the right padding in pixels
 */
Int32 View::GetPaddingRight()
{
    if (!IsPaddingResolved()) {
        ResolvePadding();
    }

    return mPaddingRight;
}

Int32 View::GetPaddingEnd()
{
    if (!IsPaddingResolved()) {
        ResolvePadding();
    }
    return (GetLayoutDirection() == IView::LAYOUT_DIRECTION_RTL) ?
            mPaddingLeft : mPaddingRight;
}

Boolean View::IsPaddingRelative()
{
    return (mUserPaddingStart != UNDEFINED_PADDING || mUserPaddingEnd != UNDEFINED_PADDING);
}

ECode View::ComputeOpticalInsets(
    /* [out] */ Insets** res)
{
    VALIDATE_NOT_NULL(res)
    if (mBackground) {
        mBackground->etOpticalInsets(res);
    } else {
        *res = Insets::NONE;
        REFCOUNT_ADD(*res)
    }
    return NOERROR;
}

ECode View::ResetPaddingToInitialValues()
{
    if (IsRtlCompatibilityMode()) {
        mPaddingLeft = mUserPaddingLeftInitial;
        mPaddingRight = mUserPaddingRightInitial;
        return NOERROR;
    }
    if (IsLayoutRtl()) {
        mPaddingLeft = (mUserPaddingEnd >= 0) ? mUserPaddingEnd : mUserPaddingLeftInitial;
        mPaddingRight = (mUserPaddingStart >= 0) ? mUserPaddingStart : mUserPaddingRightInitial;
    }
    else {
        mPaddingLeft = (mUserPaddingStart >= 0) ? mUserPaddingStart : mUserPaddingLeftInitial;
        mPaddingRight = (mUserPaddingEnd >= 0) ? mUserPaddingEnd : mUserPaddingRightInitial;
    }
    return NOERROR;
}

AutoPtr<IInsets> View::GetOpticalInsets()
{
    if (mLayoutInsets == NULL) {
        ComputeOpticalInsets((Insets**)&mLayoutInsets);
    }
    return mLayoutInsets;
}

/**
 * Set this view's optical insets.
 *
 * <p>This method should be treated similarly to setMeasuredDimension and not as a general
 * property. Views that compute their own optical insets should call it as part of measurement.
 * This method does not request layout. If you are setting optical insets outside of
 * measure/layout itself you will want to call requestLayout() yourself.
 * </p>
 * @hide
 */
ECode View::SetOpticalInsets(
    /* [in] */ IInsets* insets)
{
    mLayoutInsets = insets;
    return NOERROR;
}

/**
 * Changes the selection state of this view. A view can be selected or not.
 * Note that selection is not the same as focus. Views are typically
 * selected in the context of an AdapterView like ListView or GridView;
 * the selected view is the view that is highlighted.
 *
 * @param selected TRUE if the view must be selected, FALSE otherwise
 */
ECode View::SetSelected(
    /* [in] */ Boolean selected)
{
    //noinspection DoubleNegation
    if (((mPrivateFlags & PFLAG_SELECTED) != 0) != selected) {
        mPrivateFlags = (mPrivateFlags & ~PFLAG_SELECTED) | (selected ? PFLAG_SELECTED : 0);
        if (!selected) ResetPressedState();
        Invalidate(TRUE);
        RefreshDrawableState();
        DispatchSetSelected(selected);

        NotifyViewAccessibilityStateChangedIfNeeded(
            IAccessibilityEvent::CONTENT_CHANGE_TYPE_UNDEFINED);
    }
    return NOERROR;
}

/**
 * Dispatch setSelected to all of this View's children.
 *
 * @see #setSelected(Boolean)
 *
 * @param selected The new selected state
 */
void View::DispatchSetSelected(
    /* [in] */ Boolean selected)
{
}

/**
 * Indicates the selection state of this view.
 *
 * @return TRUE if the view is selected, FALSE otherwise
 */
Boolean View::IsSelected()
{
    return (mPrivateFlags & PFLAG_SELECTED) != 0;
}

ECode View::SetActivated(
    /* [in] */ Boolean activated)
{
    //noinspection DoubleNegation
    if (((mPrivateFlags & PFLAG_ACTIVATED) != 0) != activated) {
        mPrivateFlags = (mPrivateFlags & ~PFLAG_ACTIVATED) | (activated ? PFLAG_ACTIVATED : 0);
        Invalidate(TRUE);
        RefreshDrawableState();
        DispatchSetActivated(activated);
    }

    return NOERROR;
}

void View::DispatchSetActivated(
    /* [in] */ Boolean activated)
{
}

Boolean View::IsActivated()
{
    return (mPrivateFlags & PFLAG_ACTIVATED) != 0;
}

/**
 * Returns the ViewTreeObserver for this view's hierarchy. The view tree
 * observer can be used to get notifications when global events, like
 * layout, happen.
 *
 * The returned ViewTreeObserver observer is not guaranteed to remain
 * valid for the lifetime of this View. If the caller of this method keeps
 * a long-lived reference to ViewTreeObserver, it should always check for
 * the return value of {@link ViewTreeObserver#isAlive()}.
 *
 * @return The ViewTreeObserver for this view's hierarchy.
 */
AutoPtr<IViewTreeObserver> View::GetViewTreeObserver()
{
    if (mAttachInfo != NULL) {
        return mAttachInfo->mTreeObserver;
    }
    if (mFloatingTreeObserver == NULL) {
        mFloatingTreeObserver = new ViewTreeObserver();
    }
    return mFloatingTreeObserver;
}

/**
 * <p>Finds the topmost view in the current view hierarchy.</p>
 *
 * @return the topmost view containing this view
 */
AutoPtr<IView> View::GetRootView()
{
    if (mAttachInfo != NULL) {
        AutoPtr<IView> v = mAttachInfo->mRootView.Get();
        if (v != NULL) {
            return v;
        }
    }

    AutoPtr<IView> me;
    AutoPtr<IViewParent> parent;
    me = IVIEW_PROBE(this);
    me->GetParent((IViewParent**)&parent);
    while (parent != NULL && parent->Probe(EIID_IView) != NULL) {
        me = IVIEW_PROBE(parent);
        parent = NULL;
        me->GetParent((IViewParent**)&parent);
    }

    return me;
}

/**
 * Transforms a motion event from view-local coordinates to on-screen
 * coordinates.
 *
 * @param ev the view-local motion event
 * @return false if the transformation could not be applied
 * @hide
 */
ECode View::ToGlobalMotionEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean res)
{
    VALIDATE_NOT_NULL(res)
    AutoPtr<AttachInfo> info = mAttachInfo;
    if (info == NULL) {
        *res = FALSE;
        return NOERROR;
    }

    AutoPtr<IMatrix> m = info->mTmpMatrix;
    assert(0 && "TODO");
    //m->Set(Matrix::IDENTITY_MATRIX);
    TransformMatrixToGlobal(m);
    ev->Transform(m);
    *res = TRUE;
    return NOERROR;
}

/**
 * Transforms a motion event from on-screen coordinates to view-local
 * coordinates.
 *
 * @param ev the on-screen motion event
 * @return false if the transformation could not be applied
 * @hide
 */
ECode View::ToLocalMotionEvent(
    /* [in] */ IMotionEvent* ev,
    /* [out] */ Boolean res)
{
    VALIDATE_NOT_NULL(res)
    AutoPtr<AttachInfo> info = mAttachInfo;
    if (info == NULL) {
        *res = FALSE;
        return NOERROR;
    }

    AutoPtr<IMatrix> m = info->mTmpMatrix;
    assert(0 && "TODO");
    //m->Set(Matrix::IDENTITY_MATRIX);
    TransformMatrixToLocal(m);
    ev->Transform(m);
    *res = TRUE;
    return NOERROR;
}

/**
 * Modifies the input matrix such that it maps view-local coordinates to
 * on-screen coordinates.
 *
 * @param m input matrix to modify
 * @hide
 */
ECode View::TransformMatrixToGlobal(
    /* [in] */ IMatrix* m)
{
    AutoPtr<IViewParent> parent = mParent;
    Boolean res;
    if (IView::Probe(parent)) {
        AutoPtr<IView> vp = IView::Probe(parent);
        vp->TransformMatrixToGlobal(m);
        m->PreTranslate(-((View*)vp)->mScrollX, -((View*)vp)->mScrollY, &res);
    } else if (ViewRootImpl::Probe(parent)) {
        AutoPtr<ViewRootImpl> vr =  ViewRootImpl::Probe(parent);
        vr->TransformMatrixToGlobal(m);
        m->PreTranslate(0, -vr->mCurScrollY, &res);
    }

    m->PreTranslate(mLeft, mTop, &res);

    if (!HasIdentityMatrix()) {
        AutoPtr<IMatrix> other;
        GetMatrix((IMatrix**)&other);
        m->PreConcat(other, &res);
    }
    return NOERROR;
}

/**
 * Modifies the input matrix such that it maps on-screen coordinates to
 * view-local coordinates.
 *
 * @param m input matrix to modify
 * @hide
 */
ECode View::TransformMatrixToLocal(
    /* [in] */ IMatrix* m)
{
    AutoPtr<IViewParent> parent = mParent;
    Boolean res;
    if (IView::Probe(parent)) {
        AutoPtr<IView> vp = IView::Probe(parent);
        vp->TransformMatrixToLocal(m);
        m->PostTranslate(((View*)vp)->mScrollX, ((View*)vp)->mScrollY, &res);
    } else if (ViewRootImpl::Probe(parent)) {
        AutoPtr<ViewRootImpl> vr =  ViewRootImpl::Probe(parent);
        vr->TransformMatrixToLocal(m);
        m->PostTranslate(0, vr->mCurScrollY, &res);
    }

    m->PostTranslate(-mLeft, -mTop, &res);

    if (!HasIdentityMatrix()) {
        AutoPtr<IMatrix> other;
        GetInverseMatrix((IMatrix**)&other);
        m->postConcat(other, &res);
    }
}

/**
 * @hide
 */
ECode View::GetLocationOnScreen(
    /* [out, callee] */ ArrayOf<Int32>** res)
{
    VALIDATE_NOT_NULL(res)
    AutoPtr< ArrayOf<Int32> > location = ArrayOf<Int32>::Alloc(2);
    GetLocationOnScreen(location);
    *res = location;
    REFCOUNT_ADD(*res)
    return NOERROR;
}

/**
 * <p>Computes the coordinates of this view on the screen. The argument
 * must be an array of two integers. After the method returns, the array
 * contains the x and y location in that order.</p>
 *
 * @param location an array of two integers in which to hold the coordinates
 */
ECode View::GetLocationOnScreen(
    /* [in] */ ArrayOf<Int32>* location)
{
    GetLocationInWindow(location);

    if (mAttachInfo != NULL) {
        (*location)[0] += mAttachInfo->mWindowLeft;
        (*location)[1] += mAttachInfo->mWindowTop;
    }

    return NOERROR;
}

/**
 * <p>Computes the coordinates of this view in its window. The argument
 * must be an array of two integers. After the method returns, the array
 * contains the x and y location in that order.</p>
 *
 * @param location an array of two integers in which to hold the coordinates
 */
ECode View::GetLocationInWindow(
    /* [in] */ ArrayOf<Int32>* location)
{
    Int32 locationLen;
    if (location == NULL || (location->GetLength(&locationLen), locationLen < 2)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (mAttachInfo == NULL) {
        // When the view is not attached to a window, this method does not make sense
        (*location)[0] = (*location)[1] = 0;
        return NOERROR;
    }

    ArrayOf<Float>& position = mAttachInfo->mTmpTransformLocation;
    position[0] = position[1] = 0.0f;

    if (!HasIdentityMatrix()) {
        AutoPtr<IMatrix> matrix = GetMatrix();
        assert(matrix != NULL);
        matrix->MapPoints(&position);
    }

    position[0] += mLeft;
    position[1] += mTop;

    AutoPtr<IViewParent> viewParent = mParent;
    while (viewParent && VIEW_PROBE(viewParent) != NULL) {
        View* view = VIEW_PROBE(viewParent);

        position[0] -= view->mScrollX;
        position[1] -= view->mScrollY;

        if (!view->HasIdentityMatrix()) {
            AutoPtr<IMatrix> matrix = view->GetMatrix();
            assert(matrix != NULL);
            matrix->MapPoints(&position);
        }

        position[0] += view->mLeft;
        position[1] += view->mTop;

        viewParent = view->mParent;
     }

    if (viewParent->Probe(EIID_ViewRootImpl)) {
        // *cough*
        ViewRootImpl* vr = reinterpret_cast<ViewRootImpl*>(viewParent->Probe(EIID_ViewRootImpl));
        position[1] -= vr->mCurScrollY;
    }

    (*location)[0] = (Int32) (position[0] + 0.5f);
    (*location)[1] = (Int32) (position[1] + 0.5f);

    return NOERROR;
}

/**
 * {@hide}
 * @param id the id of the view to be found
 * @return the view of the specified id, NULL if cannot be found
 */
AutoPtr<IView> View::FindViewTraversal(
    /* [in] */ Int32 id)
{
    if (id == mID) {
        return AutoPtr<IView>(IVIEW_PROBE(this));
    }
    return AutoPtr<IView>(NULL);
}

/**
 * {@hide}
 * @param tag the tag of the view to be found
 * @return the view of specified tag, NULL if cannot be found
 */
AutoPtr<IView> View::FindViewWithTagTraversal(
    /* [in] */ IInterface* tag)
{
    if (tag != NULL && tag == mTag.Get()) {
        return AutoPtr<IView>(IVIEW_PROBE(this));
    }
    return AutoPtr<IView>(NULL);
}

/**
 * {@hide}
 * @param predicate The predicate to evaluate.
 * @param childToSkip If not NULL, ignores this child during the recursive traversal.
 * @return The first view that matches the predicate or NULL.
 */
AutoPtr<IView> View::FindViewByPredicateTraversal(
    /* [in] */ IPredicate* predicate,
    /* [in] */ IView* childToSkip)
{
    assert(predicate != NULL);
    Boolean res = FALSE;
    predicate->Apply(this->Probe(EIID_IInterface), &res);
    if (res) {
        return IVIEW_PROBE(this);
    }

    return NULL;
}

/**
 * Look for a child view with the given id.  If this view has the given
 * id, return this view.
 *
 * @param id The id to search for.
 * @return The view that has the given id in the hierarchy or NULL
 */
AutoPtr<IView> View::FindViewById(
    /* [in] */ Int32 id)
{
    if (id < 0) {
        return NULL;
    }
    return FindViewTraversal(id);
}

AutoPtr<IView> View::FindViewByAccessibilityId(
    /* [in] */ Int32 accessibilityId)
{
    if (accessibilityId < 0) {
        return NULL;
    }
    return FindViewByAccessibilityIdTraversal(accessibilityId);
}

Ecode View::FindViewByAccessibilityIdTraversal(
    /* [in] */ Int32 accessibilityId,
    /* [out] */ IView** res)
{
    VALIDATE_NOT_NULL(res)
    Int32 id;
    GetAccessibilityViewId(&id);
    if (id == accessibilityId) {
        *res = IVIEW_PROBE(this);
        REFCOUNT_ADD(*res)
        return NOERROR;
    }
    *res = NULL;
    return NOERROR;
}

/**
    * Look for a child view with the given tag.  If this view has the given
    * tag, return this view.
    *
    * @param tag The tag to search for, using "tag.equals(getTag())".
    * @return The View that has the given tag in the hierarchy or NULL
    */
AutoPtr<IView> View::FindViewWithTag(
    /* [in] */ IInterface* tag)
{
    if (tag == NULL) {
        return AutoPtr<IView>(NULL);
    }
    return FindViewWithTagTraversal(tag);
}

AutoPtr<IView> View::FindViewByPredicate(
    /* [in] */ IPredicate* predicate)
{
    return FindViewByPredicateTraversal(predicate, NULL);
}

AutoPtr<IView> View::FindViewByPredicateInsideOut(
    /* [in] */ IView* start,
    /* [in] */ IPredicate* predicate)
{
    AutoPtr<IView> childToSkip;
    for (;;) {
        View* startView = VIEW_PROBE(start);
        AutoPtr<IView> view = startView->FindViewByPredicateTraversal(predicate, childToSkip);
        if (view != NULL || startView == this) {
            return view;
        }

        AutoPtr<IViewParent> parent = startView->GetParent();
        if (parent == NULL || !(IView::Probe(parent))) {
            return NULL;
        }

        childToSkip = start;
        start = IView::Probe(parent);
    }
}

/**
 * Sets the identifier for this view. The identifier does not have to be
 * unique in this view's hierarchy. The identifier should be a positive
 * number.
 *
 * @see #IView::NO_ID
 * @see #getId
 * @see #findViewById
 *
 * @param id a number used to identify the view
 *
 * @attr ref android.R.styleable#View_id
 */
ECode View::SetId(
    /* [in] */ Int32 id)
{
    mID = id;
    if (mID == IView::NO_ID && mLabelForId != IView::NO_ID) {
        mID = GenerateViewId();
    }

    return NOERROR;
}

/**
 * {@hide}
 *
 * @param isRoot TRUE if the view belongs to the root namespace, FALSE
 *        otherwise
 */
ECode View::SetIsRootNamespace(
    /* [in] */ Boolean isRoot)
{
    if (isRoot) {
        mPrivateFlags |= PFLAG_IS_ROOT_NAMESPACE;
    }
    else {
        mPrivateFlags &= ~PFLAG_IS_ROOT_NAMESPACE;
    }
    return NOERROR;
}

/**
 * {@hide}
 *
 * @return TRUE if the view belongs to the root namespace, FALSE otherwise
 */
Boolean View::IsRootNamespace()
{
    return (mPrivateFlags & PFLAG_IS_ROOT_NAMESPACE) != 0;
}

/**
 * Returns this view's identifier.
 *
 * @return a positive integer used to identify the view or {@link #IView::NO_ID}
 *         if the view has no ID
 *
 * @see #setId
 * @see #findViewById
 * @attr ref android.R.styleable#View_id
 */
Int32 View::GetId()
{
    return mID;
}

/**
 * Returns this view's tag.
 *
 * @return the Object stored in this view as a tag
 *
 * @see #setTag(Object)
 * @see #getTag(int)
 */
AutoPtr<IInterface> View::GetTag()
{
    return mTag;
}

/**
 * Sets the tag associated with this view. A tag can be used to mark
 * a view in its hierarchy and does not have to be unique within the
 * hierarchy. Tags can also be used to store data within a view without
 * resorting to another data structure.
 *
 * @param tag an Object to tag the view with
 *
 * @see #getTag()
 * @see #setTag(int, Object)
 */
ECode View::SetTag(
    /* [in] */ IInterface* tag)
{
    mTag = tag;
    return NOERROR;
}

/**
 * Returns the tag associated with this view and the specified key.
 *
 * @param key The key identifying the tag
 *
 * @return the Object stored in this view as a tag
 *
 * @see #setTag(int, Object)
 * @see #getTag()
 */
AutoPtr<IInterface> View::GetTag(
    /* [in] */ Int32 key)
{
    HashMap<Int32, AutoPtr<IInterface> >::Iterator find = mKeyedTags.Find(key);
    if (find != mKeyedTags.End())
        find->mSecond;
    return NULL;
}

/**
 * Sets a tag associated with this view and a key. A tag can be used
 * to mark a view in its hierarchy and does not have to be unique within
 * the hierarchy. Tags can also be used to store data within a view
 * without resorting to another data structure.
 *
 * The specified key should be an id declared in the resources of the
 * application to ensure it is unique (see the <a
 * href={@docRoot}guide/topics/resources/more-resources.html#Id">ID resource type</a>).
 * Keys identified as belonging to
 * the Android framework or not associated with any package will cause
 * an {@link IllegalArgumentException} to be thrown.
 *
 * @param key The key identifying the tag
 * @param tag An Object to tag the view with
 *
 * @throws IllegalArgumentException If they specified key is not valid
 *
 * @see #setTag(Object)
 * @see #getTag(int)
 */
ECode View::SetTag(
   /* [in] */ Int32 key,
   /* [in] */ IInterface* tag)
{
    // If the package id is 0x00 or 0x01, it's either an undefined package
    // or a framework id
    if (((key & 0x7FFFFFFF) >> 24) < 2) {
        Logger::E(VIEW_LOG_TAG, "The key must be an application-specific resource id.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    SetKeyedTag(key, tag);
    return NOERROR;
}

/**
 * Variation of {@link #setTag(int, Object)} that enforces the key to be a
 * framework id.
 *
 * @hide
 */
ECode View::SetTagInternal(
   /* [in] */ Int32 key,
   /* [in] */ IInterface* tag)
{
   if (((key & 0x7FFFFFFF) >> 24) != 0x1) {
        Logger::E(VIEW_LOG_TAG, "The key must be an framework-specific resource id.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
   }

   SetKeyedTag(key, tag);
   return NOERROR;
}

void View::SetKeyedTag(
    /* [in] */ Int32 key,
    /* [in] */ IInterface* tag)
{
    mKeyedTags[key] = tag;
}

ECode View::HackTurnOffWindowResizeAnim(
    /* [in] */ Boolean off)
{
    mAttachInfo->mTurnOffWindowResizeAnim = off;
    return NOERROR;
}

/**
 * This method returns a ViewPropertyAnimator object, which can be used to animate
 * specific properties on this View.
 *
 * @return ViewPropertyAnimator The ViewPropertyAnimator associated with this View.
 */
AutoPtr<IViewPropertyAnimator> View::Animate()
{
    if (mAnimator == NULL) {
        CViewPropertyAnimator::New(IVIEW_PROBE(this), (IViewPropertyAnimator**)&mAnimator);
    }
    return mAnimator;
}

/**
 * Sets the name of the View to be used to identify Views in Transitions.
 * Names should be unique in the View hierarchy.
 *
 * @param transitionName The name of the View to uniquely identify it for Transitions.
 */
ECode View::SetTransitionName(
    /* [in] */ const String& transitionName)
{
    mTransitionName = transitionName;
    return NOERROR;
}

/**
 * Returns the name of the View to be used to identify Views in Transitions.
 * Names should be unique in the View hierarchy.
 *
 * <p>This returns null if the View has not been given a name.</p>
 *
 * @return The name used of the View to be used to identify Views in Transitions or null
 * if no name has been given.
 */
ECode View::GetTransitionName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    *name = mTransitionName;
    return NOERROR;
}

/**
 * Prints information about this view in the log output, with the tag
 * {@link #VIEW_LOG_TAG}.
 *
 * @hide
 */
void View::Debug()
{
    //Debug(0);
}

/**
 * Prints information about this view in the log output, with the tag
 * {@link #VIEW_LOG_TAG}. Each line in the output is preceded with an
 * indentation defined by the <code>depth</code>.
 *
 * @param depth the indentation level
 *
 * @hide
 */
//void View::Debug(
//    /* [in] */ int32 depth)
//{
//    String output = debugIndent(depth - 1);
//
//    output += "+ " + this;
//    int id = getId();
//    if (id != -1) {
//        output += " (id=" + id + ")";
//    }
//    Object tag = getTag();
//    if (tag != NULL) {
//        output += " (tag=" + tag + ")";
//    }
//    Log.d(VIEW_LOG_TAG, output);
//
//    if ((mPrivateFlags & PFLAG_FOCUSED) != 0) {
//        output = debugIndent(depth) + " PFLAG_FOCUSED";
//        Log.d(VIEW_LOG_TAG, output);
//    }
//
//    output = debugIndent(depth);
//    output += "frame={" + mLeft + ", " + mTop + ", " + mRight
//            + ", " + mBottom + "} scroll={" + mScrollX + ", " + mScrollY
//            + "} ";
//    Log.d(VIEW_LOG_TAG, output);
//
//    if (mPaddingLeft != 0 || mPaddingTop != 0 || mPaddingRight != 0
//            || mPaddingBottom != 0) {
//        output = debugIndent(depth);
//        output += "padding={" + mPaddingLeft + ", " + mPaddingTop
//                + ", " + mPaddingRight + ", " + mPaddingBottom + "}";
//        Log.d(VIEW_LOG_TAG, output);
//    }
//
//    output = debugIndent(depth);
//    output += "mMeasureWidth=" + mMeasuredWidth +
//            " mMeasureHeight=" + mMeasuredHeight;
//    Log.d(VIEW_LOG_TAG, output);
//
//    output = debugIndent(depth);
//    if (mLayoutParams == NULL) {
//        output += "BAD! no layout params";
//    }
//    else {
//        output = mLayoutParams.debug(output);
//    }
//    Log.d(VIEW_LOG_TAG, output);
//
//    output = debugIndent(depth);
//    output += "flags={";
//    output += View.printFlags(mViewFlags);
//    output += "}";
//    Log.d(VIEW_LOG_TAG, output);
//
//    output = debugIndent(depth);
//    output += "privateFlags={";
//    output += View.printPrivateFlags(mPrivateFlags);
//    output += "}";
//    Log.d(VIEW_LOG_TAG, output);
//}

/**
 * Creates an string of whitespaces used for indentation.
 *
 * @param depth the indentation level
 * @return a String containing (depth * 2 + 3) * 2 white spaces
 *
 * @hide
 */
//static String debugIndent(
//    /* [in] */ Int32 depth)
//{
//    StringBuilder spaces = new StringBuilder((depth * 2 + 3) * 2);
//    for (int i = 0; i < (depth * 2) + 3; i++) {
//        spaces.append(' ').append(' ');
//    }
//    return spaces.toString();
//}

/**
 * <p>Return the offset of the widget's text baseline from the widget's top
 * boundary. If this widget does not support baseline alignment, this
 * method returns -1. </p>
 *
 * @return the offset of the baseline within the widget's bounds or -1
 *         if baseline alignment is not supported
 */
ECode View::GetBaseline(
    /* [out] */ Int32* baseline)
{
    assert(baseline);
    *baseline = -1;

    return NOERROR;
}

/**
 * Returns whether the view hierarchy is currently undergoing a layout pass. This
 * information is useful to avoid situations such as calling {@link #requestLayout()} during
 * a layout pass.
 *
 * @return whether the view hierarchy is currently undergoing a layout pass
 */
ECode View::IsInLayout(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    AutoPtr<ViewRootImpl> viewRoot;
    GetViewRootImpl((ViewRootImpl**)&viewRoot);
    *res = (viewRoot != NULL && viewRoot->IsInLayout());
    return NOERROR;
}

/**
 * Call this when something has changed which has invalidated the
 * layout of this view. This will schedule a layout pass of the view
 * tree. This should not be called while the view hierarchy is currently in a layout
 * pass ({@link #isInLayout()}. If layout is happening, the request may be honored at the
 * end of the current layout pass (and then layout will run again) or after the current
 * frame is drawn and the next layout occurs.
 *
 * <p>Subclasses which override this method should call the superclass method to
 * handle possible request-during-layout errors correctly.</p>
 */
ECode View::RequestLayout()
{
    mMeasureCache.Clear();

    if (mAttachInfo != NULL && mAttachInfo->mViewRequestingLayout == NULL) {
        // Only trigger request-during-layout logic if this is the view requesting it,
        // not the views in its parent hierarchy
        AutoPtr<ViewRootImpl> viewRoot;
        GetViewRootImpl((ViewRootImpl**)&viewRoot);
        if (viewRoot != NULL && viewRoot->IsInLayout()) {
            if (!viewRoot->RequestLayoutDuringLayout(THIS_PROBE(IView))) {
                return NOERROR;
            }
        }
        mAttachInfo->mViewRequestingLayout = THIS_PROBE(IView);
    }

    mPrivateFlags |= PFLAG_FORCE_LAYOUT;
    mPrivateFlags |= PFLAG_INVALIDATED;

    if (mParent != NULL) {
        Boolean requested = FALSE;
        mParent->IsLayoutRequested(&requested);
        if (!requested) {
            mParent->RequestLayout();
        }
    }

    if (mAttachInfo != NULL && mAttachInfo->mViewRequestingLayout == THIS_PROBE(IView)) {
        mAttachInfo->mViewRequestingLayout = NULL;
    }

    return NOERROR;
}

/**
 * Forces this view to be laid out during the next layout pass.
 * This method does not call requestLayout() or forceLayout()
 * on the parent.
 */
ECode View::ForceLayout()
{
    mMeasureCache.clear();

    mPrivateFlags |= PFLAG_FORCE_LAYOUT;
    mPrivateFlags |= PFLAG_INVALIDATED;
    return NOERROR;
}

/**
 * <p>
 * This is called to find out how big a view should be. The parent
 * supplies constraint information in the width and height parameters.
 * </p>
 *
 * <p>
 * The actual mesurement work of a view is performed in
 * {@link #onMeasure(int, int)}, called by this method. Therefore, only
 * {@link #onMeasure(int, int)} can and must be overriden by subclasses.
 * </p>
 *
 *
 * @param widthMeasureSpec Horizontal space requirements as imposed by the
 *        parent
 * @param heightMeasureSpec Vertical space requirements as imposed by the
 *        parent
 *
 * @see #onMeasure(int, int)
 */
ECode View::Measure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Boolean optical = IsLayoutModeOptical(THIS_PROBE(IInterface));
    if (optical != IsLayoutModeOptical((IInterface*)mParent->Probe(EIID_IInterface))) {
        AutoPtr<IInsets> insets;
        GetOpticalInsets((IInsets**)&insets);
        Cinsets* temp = (Cinsets*)insets;
        Int32 oWidth  = temp->mLeft + temp->mRight;
        Int32 oHeight = temp->mTop  + temp->mBottom;
        widthMeasureSpec  = MeasureSpec::Adjust(widthMeasureSpec,  optical ? -oWidth  : oWidth);
        heightMeasureSpec = MeasureSpec::Adjust(heightMeasureSpec, optical ? -oHeight : oHeight);
    }

    // Suppress sign extension for the low bytes
    Int64 key = (Int64) widthMeasureSpec << 32 | (Int64) heightMeasureSpec & 0xffffffffL;

    if ((mPrivateFlags & PFLAG_FORCE_LAYOUT) == PFLAG_FORCE_LAYOUT ||
            widthMeasureSpec != mOldWidthMeasureSpec ||
            heightMeasureSpec != mOldHeightMeasureSpec) {

        // first clears the measured dimension flag
        mPrivateFlags &= ~PFLAG_MEASURED_DIMENSION_SET;

        ResolveRtlPropertiesIfNeeded();

        HashMap<Int64, Int64>::Iterator it = mMeasureCache.Find(key);
        if (key == mMeasureCache.End() | sIgnoreMeasureCache) {
            OnMeasure(widthMeasureSpec, heightMeasureSpec);
            mPrivateFlags3 &= ~PFLAG3_MEASURE_NEEDED_BEFORE_LAYOUT;
        } else {
            Int64 value = *(it->mSecond);
            // Casting a long to int drops the high 32 bits, no mask needed
            SetMeasuredDimensionRaw((Int32) (value >> 32), (Int32) value);
            mPrivateFlags3 |= PFLAG3_MEASURE_NEEDED_BEFORE_LAYOUT;
        }

        // flag not set, setMeasuredDimension() was not invoked, we raise
        // an exception to warn the developer
        if ((mPrivateFlags & PFLAG_MEASURED_DIMENSION_SET) != PFLAG_MEASURED_DIMENSION_SET) {
            //throw new IllegalStateException("onMeasure() did not set the"
              //      + " measured dimension by calling"
                //    + " setMeasuredDimension()");
            return E_ILLEGAL_STATE_EXCEPTION;
        }

        mPrivateFlags |= PFLAG_LAYOUT_REQUIRED;
    }

    mOldWidthMeasureSpec = widthMeasureSpec;
    mOldHeightMeasureSpec = heightMeasureSpec;

    Int64 valueHeight = ((Int64)mMeasuredWidth) << 32 | ((Int64)mMeasuredHeight & 0xffffffffL);
    mMeasureCache.Insert(HashMap<Int64, Int64>::ValueType(key, valueHeight)); // suppress sign extension

    return NOERROR;
}

/**
 * <p>
 * Measure the view and its content to determine the measured width and the
 * measured height. This method is invoked by {@link #measure(int, int)} and
 * should be overriden by subclasses to provide accurate and efficient
 * measurement of their contents.
 * </p>
 *
 * <p>
 * <strong>CONTRACT:</strong> When overriding this method, you
 * <em>must</em> call {@link #setMeasuredDimension(int, int)} to store the
 * measured width and height of this view. Failure to do so will trigger an
 * <code>IllegalStateException</code>, thrown by
 * {@link #measure(int, int)}. Calling the superclass'
 * {@link #onMeasure(int, int)} is a valid use.
 * </p>
 *
 * <p>
 * The base class implementation of measure defaults to the background size,
 * unless a larger size is allowed by the MeasureSpec. Subclasses should
 * override {@link #onMeasure(int, int)} to provide better measurements of
 * their content.
 * </p>
 *
 * <p>
 * If this method is overridden, it is the subclass's responsibility to make
 * sure the measured height and width are at least the view's minimum height
 * and width ({@link #getSuggestedMinimumHeight()} and
 * {@link #getSuggestedMinimumWidth()}).
 * </p>
 *
 * @param widthMeasureSpec horizontal space requirements as imposed by the parent.
 *                         The requirements are encoded with
 *                         {@link android.view.View.MeasureSpec}.
 * @param heightMeasureSpec vertical space requirements as imposed by the parent.
 *                         The requirements are encoded with
 *                         {@link android.view.View.MeasureSpec}.
 *
 * @see #getMeasuredWidth()
 * @see #getMeasuredHeight()
 * @see #setMeasuredDimension(int, int)
 * @see #getSuggestedMinimumHeight()
 * @see #getSuggestedMinimumWidth()
 * @see android.view.View.MeasureSpec#getMode(int)
 * @see android.view.View.MeasureSpec#getSize(int)
 */
void View::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    SetMeasuredDimension(GetDefaultSize(GetSuggestedMinimumWidth(), widthMeasureSpec),
        GetDefaultSize(GetSuggestedMinimumHeight(), heightMeasureSpec));
}

/**
 * <p>This mehod must be called by {@link #onMeasure(int, int)} to store the
 * measured width and measured height. Failing to do so will trigger an
 * exception at measurement time.</p>
 *
 * @param measuredWidth The measured width of this view.  May be a complex
 * bit mask as defined by {@link #MEASURED_SIZE_MASK} and
 * {@link #MEASURED_STATE_TOO_SMALL}.
 * @param measuredHeight The measured height of this view.  May be a complex
 * bit mask as defined by {@link #MEASURED_SIZE_MASK} and
 * {@link #MEASURED_STATE_TOO_SMALL}.
 */
void View::SetMeasuredDimension(
    /* [in] */ Int32 measuredWidth,
    /* [in] */ Int32 measuredHeight)
{
    Boolean optical, parentOptical;
    IsLayoutModeOptical(THIS_PROBE(IInterface), &optical);
    IsLayoutModeOptical((IInterface*)mParent->Probe(EIID_IInterface), &parentOptical)
    if (optical != parentOptical) {
        AutoPtr<IInsets> insets;
        GetOpticalInsets((IInsets**)&insets);
        CInsets* temp = (CInsets*)insets;
        Int32 opticalWidth  = temp->mLeft + temp->mRight;
        Int32 opticalHeight = temp->mTop  + temp->mBottom;

        measuredWidth  += optical ? opticalWidth  : -opticalWidth;
        measuredHeight += optical ? opticalHeight : -opticalHeight;
    }
    SetMeasuredDimensionRaw(measuredWidth, measuredHeight);
}

/**
 * Sets the measured dimension without extra processing for things like optical bounds.
 * Useful for reapplying consistent values that have already been cooked with adjustments
 * for optical bounds, etc. such as those from the measurement cache.
 *
 * @param measuredWidth The measured width of this view.  May be a complex
 * bit mask as defined by {@link #MEASURED_SIZE_MASK} and
 * {@link #MEASURED_STATE_TOO_SMALL}.
 * @param measuredHeight The measured height of this view.  May be a complex
 * bit mask as defined by {@link #MEASURED_SIZE_MASK} and
 * {@link #MEASURED_STATE_TOO_SMALL}.
 */
ECode View::SetMeasuredDimensionRaw(
        /* [in] */ Int32 measuredWidth,
        /* [in] */ Int32 measuredHeight)
{
    mMeasuredWidth = measuredWidth;
    mMeasuredHeight = measuredHeight;

    mPrivateFlags |= PFLAG_MEASURED_DIMENSION_SET;
}

Int32 View::CombineMeasuredStates(
    /* [in] */ Int32 curState,
    /* [in] */ Int32 newState)
{
    return curState | newState;
}

/**
 * Version of {@link #resolveSizeAndState(int, int, int)}
 * returning only the {@link #MEASURED_SIZE_MASK} bits of the result.
 */
Int32 View::ResolveSize(
    /* [in] */ Int32 size,
    /* [in] */ Int32 measureSpec)
{
    return ResolveSizeAndState(size, measureSpec, 0) & IView::MEASURED_SIZE_MASK;
}

/**
 * Utility to reconcile a desired size and state, with constraints imposed
 * by a MeasureSpec.  Will take the desired size, unless a different size
 * is imposed by the constraints.  The returned value is a compound integer,
 * with the resolved size in the {@link #MEASURED_SIZE_MASK} bits and
 * optionally the bit {@link #MEASURED_STATE_TOO_SMALL} set if the resulting
 * size is smaller than the size the view wants to be.
 *
 * @param size How big the view wants to be
 * @param measureSpec Constraints imposed by the parent
 * @return Size information bit mask as defined by
 * {@link #MEASURED_SIZE_MASK} and {@link #MEASURED_STATE_TOO_SMALL}.
 */
Int32 View::ResolveSizeAndState(
    /* [in] */ Int32 size,
    /* [in] */ Int32 measureSpec,
    /* [in] */ Int32 childMeasuredState)
{
    Int32 result = size;
    Int32 specMode = MeasureSpec::GetMode(measureSpec);
    Int32 specSize =  MeasureSpec::GetSize(measureSpec);
    switch (specMode) {
    case MeasureSpec::UNSPECIFIED:
        result = size;
        break;
    case MeasureSpec::AT_MOST:
        if (specSize < size) {
            result = specSize | IView::MEASURED_STATE_TOO_SMALL;
        }
        else {
            result = size;
        }
        break;
    case MeasureSpec::EXACTLY:
        result = specSize;
        break;
    }
    return result | (childMeasuredState & IView::MEASURED_STATE_MASK);
}

/**
 * Utility to return a default size. Uses the supplied size if the
 * MeasureSpec imposed no contraints. Will get larger if allowed
 * by the MeasureSpec.
 *
 * @param size Default size for this view
 * @param measureSpec Constraints imposed by the parent
 * @return The size this view should be.
 */
Int32 View::GetDefaultSize(
    /* [in] */ Int32 size,
    /* [in] */ Int32 measureSpec)
{
    Int32 result = size;
    Int32 specMode = MeasureSpec::GetMode(measureSpec);
    Int32 specSize =  MeasureSpec::GetSize(measureSpec);

    switch (specMode) {
    case MeasureSpec::UNSPECIFIED:
        result = size;
        break;
    case MeasureSpec::AT_MOST:
    case MeasureSpec::EXACTLY:
        result = specSize;
        break;
    }

    return result;
}

/**
 * Returns the suggested minimum height that the view should use. This
 * returns the maximum of the view's minimum height
 * and the background's minimum height
 * ({@link android.graphics.drawable.Drawable#getMinimumHeight()}).
 * <p>
 * When being used in {@link #onMeasure(Int32, Int32)}, the caller should still
 * ensure the returned height is within the requirements of the parent.
 *
 * @return The suggested minimum height of the view.
 */
Int32 View::GetSuggestedMinimumHeight()
{
    Int32 tmpH = 0;
    return (mBackground == NULL) ? mMinHeight : Elastos::Core::Math::Max(mMinHeight, (mBackground->GetMinimumHeight(&tmpH), tmpH));
}

/**
 * Returns the suggested minimum width that the view should use. This
 * returns the maximum of the view's minimum width)
 * and the background's minimum width
 *  ({@link android.graphics.drawable.Drawable#getMinimumWidth()}).
 * <p>
 * When being used in {@link #onMeasure(Int32, Int32)}, the caller should still
 * ensure the returned width is within the requirements of the parent.
 *
 * @return The suggested minimum width of the view.
 */
Int32 View::GetSuggestedMinimumWidth()
{
    Int32 tmpW = 0;
    return (mBackground == NULL) ? mMinWidth : Elastos::Core::Math::Max(mMinWidth, (mBackground->GetMinimumWidth(&tmpW), tmpW));
}

Int32 View::GetMinimumHeight()
{
    return mMinHeight;
}

/**
 * Sets the minimum height of the view. It is not guaranteed the view will
 * be able to achieve this minimum height (for example, if its parent layout
 * constrains it with less available height).
 *
 * @param minHeight The minimum height the view will try to be.
 */
ECode View::SetMinimumHeight(
    /* [in] */ Int32 minHeight)
{
    mMinHeight = minHeight;
    RequestLayout();
    return NOERROR;
}

Int32 View::GetMinimumWidth()
{
    return mMinWidth;
}

/**
 * Sets the minimum width of the view. It is not guaranteed the view will
 * be able to achieve this minimum width (for example, if its parent layout
 * constrains it with less available width).
 *
 * @param minWidth The minimum width the view will try to be.
 */
ECode View::SetMinimumWidth(
    /* [in] */ Int32 minWidth)
{
    mMinWidth = minWidth;
    RequestLayout();
    return NOERROR;
}


/**
 * Get the animation currently associated with this view.
 *
 * @return The animation that is currently playing or
 *         scheduled to play for this view.
 */
AutoPtr<IAnimation> View::GetAnimation()
{
    return mCurrentAnimation;
}

/**
 * Start the specified animation now.
 *
 * @param animation the animation to start now
 */
ECode View::StartAnimation(
    /* [in] */ IAnimation* animation)
{
    animation->SetStartTime(IAnimation::START_ON_FIRST_FRAME);
    SetAnimation(animation);
    InvalidateParentCaches();
    Invalidate(TRUE);

    return NOERROR;
}

/**
 * Cancels any animations for this view.
 */
ECode View::ClearAnimation()
{
    if (mCurrentAnimation != NULL) {
        mCurrentAnimation->Detach();
    }
    mCurrentAnimation = NULL;
    InvalidateParentIfNeeded();

    return NOERROR;
}

ECode View::SetAnimation(
    /* [in] */ IAnimation* animation)
{
    mCurrentAnimation = animation;
    if (animation != NULL) {
        // If the screen is off assume the animation start time is now instead of
        // the next frame we draw. Keeping the START_ON_FIRST_FRAME start time
        // would cause the animation to start when the screen turns back on
        Int64 start = 0;
        animation->GetStartTime(&start);
        if (mAttachInfo != NULL &&  mAttachInfo.mDisplayState == IDisplay::STATE_OFF
            && start == IAnimation::START_ON_FIRST_FRAME) {
            animation->SetStartTime(AnimationUtils::CurrentAnimationTimeMillis());
        }

        animation->Reset();
    }

    return NOERROR;
}

/**
 * Invoked by a parent ViewGroup to notify the start of the animation
 * currently associated with this view. If you override this method,
 * always call super.onAnimationStart();
 *
 * @see #setAnimation(android.view.animation.Animation)
 * @see #getAnimation()
 */
void View::OnAnimationStart()
{
    mPrivateFlags |= PFLAG_ANIMATION_STARTED;
}

/**
 * Invoked by a parent ViewGroup to notify the end of the animation
 * currently associated with this view. If you override this method,
 * always call super.onAnimationEnd();
 *
 * @see #setAnimation(android.view.animation.Animation)
 * @see #getAnimation()
 */
void View::OnAnimationEnd()
{
    mPrivateFlags &= ~PFLAG_ANIMATION_STARTED;
}

/**
 * Invoked if there is a Transform that involves alpha. Subclass that can
 * draw themselves with the specified alpha should return TRUE, and then
 * respect that alpha when their onDraw() is called. If this returns FALSE
 * then the view may be redirected to draw into an offscreen buffer to
 * fulfill the request, which will look fine, but may be slower than if the
 * subclass handles it internally. The default implementation returns FALSE.
 *
 * @param alpha The alpha (0..255) to apply to the view's drawing
 * @return TRUE if the view can draw with the specified alpha.
 */
Boolean View::OnSetAlpha(
    /* [in] */ Int32 alpha)
{
    return FALSE;
}

Boolean View::GatherTransparentRegion(
    /* [in] */ IRegion* region)
{
    AttachInfo* attachInfo = mAttachInfo;
    if (region != NULL && attachInfo != NULL) {
        Int32 pflags = mPrivateFlags;
        if ((pflags & PFLAG_SKIP_DRAW) == 0) {
            // The PFLAG_SKIP_DRAW flag IS NOT set, so this view draws. We need to
            // remove it from the transparent region.
            Int32* location = attachInfo->mTransparentLocation;
            GetLocationInWindow(location, location + 1);
            Boolean result;
            region->Op(location[0], location[1], location[0] + mRight - mLeft,
                    location[1] + mBottom - mTop, RegionOp_DIFFERENCE, &result);
        }
        else if ((pflags & PFLAG_ONLY_DRAWS_BACKGROUND) != 0 && mBackground != NULL ) {
            Int32 opacity;
            mBackground->GetOpacity(&opacity);
            if (opacity != IPixelFormat::TRANSPARENT) {
                // The PFLAG_ONLY_DRAWS_BACKGROUND flag IS set and the background drawable
                // exists, so we remove the background drawable's non-transparent
                // parts from this transparent region.
                ApplyDrawableToTransparentRegion(mBackground, region);
            }
        }
    }
    return TRUE;
}

/**
 * Play a sound effect for this view.
 *
 * <p>The framework will play sound effects for some built in actions, such as
 * clicking, but you may wish to play these effects in your widget,
 * for instance, for internal navigation.
 *
 * <p>The sound effect will only be played if sound effects are enabled by the user, and
 * {@link #isSoundEffectsEnabled()} is TRUE.
 *
 * @param soundConstant One of the constants defined in {@link SoundEffectConstants}
 */
ECode View::PlaySoundEffect(
    /* [in] */ Int32 soundConstant)
{
    if (mAttachInfo == NULL || mAttachInfo->mRootCallbacks == NULL || !IsSoundEffectsEnabled()) {
        return NOERROR;
    }
    mAttachInfo->mRootCallbacks->PlaySoundEffect(soundConstant);

    return NOERROR;
}

/**
 * BZZZTT!!1!
 *
 * <p>Provide haptic feedback to the user for this view.
 *
 * <p>The framework will provide haptic feedback for some built in actions,
 * such as long presses, but you may wish to provide feedback for your
 * own widget.
 *
 * <p>The feedback will only be performed if
 * {@link #isHapticFeedbackEnabled()} is TRUE.
 *
 * @param feedbackConstant One of the constants defined in
 * {@link HapticFeedbackConstants}
 */
Boolean View::PerformHapticFeedback(
    /* [in] */ Int32 feedbackConstant)
{
    return PerformHapticFeedback(feedbackConstant, 0);
}

/**
 * BZZZTT!!1!
 *
 * <p>Like {@link #performHapticFeedback(int)}, with additional options.
 *
 * @param feedbackConstant One of the constants defined in
 * {@link HapticFeedbackConstants}
 * @param flags Additional flags as per {@link HapticFeedbackConstants}.
 */
Boolean View::PerformHapticFeedback(
    /* [in] */ Int32 feedbackConstant,
    /* [in] */ Int32 flags)
{
    if (mAttachInfo == NULL) {
        return FALSE;
    }

    //noinspection SimplifiableIfStatement
    if ((flags & IHapticFeedbackConstants::FLAG_IGNORE_VIEW_SETTING) == 0
            && !IsHapticFeedbackEnabled()) {
        return FALSE;
    }

    Boolean result;

    return mAttachInfo->mRootCallbacks->PerformHapticFeedback(
            feedbackConstant,
            (flags & IHapticFeedbackConstants::FLAG_IGNORE_GLOBAL_SETTING) != 0, &result);
}


ECode View::SetSystemUiVisibility(
    /* [in] */ Int32 visibility)
{
    if (visibility != mSystemUiVisibility) {
        mSystemUiVisibility = visibility;
        if (mParent != NULL && mAttachInfo != NULL && !mAttachInfo->mRecomputeGlobalAttributes) {
            mParent->RecomputeViewAttributes(IVIEW_PROBE(this));
        }
    }

    return NOERROR;
}

Int32 View::GetSystemUiVisibility()
{
    return mSystemUiVisibility;
}

Int32 View::GetWindowSystemUiVisibility()
{
    return mAttachInfo != NULL ? mAttachInfo->mSystemUiVisibility : 0;
}

ECode View::OnWindowSystemUiVisibilityChanged(
    /* [in] */ Int32 visible)
{
    return NOERROR;
}

ECode View::DispatchWindowSystemUiVisiblityChanged(
    /* [in] */ Int32 visible)
{
    return OnWindowSystemUiVisibilityChanged(visible);
}

ECode View::SetOnSystemUiVisibilityChangeListener(
    /* [in] */ IViewOnSystemUiVisibilityChangeListener* l)
{
    GetListenerInfo()->mOnSystemUiVisibilityChangeListener = l;
    if (mParent != NULL && mAttachInfo != NULL && !mAttachInfo->mRecomputeGlobalAttributes) {
        mParent->RecomputeViewAttributes(IVIEW_PROBE(this));
    }

    return NOERROR;
}

ECode View::DispatchSystemUiVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    AutoPtr<ListenerInfo> li = mListenerInfo;
    if (li != NULL && li->mOnSystemUiVisibilityChangeListener != NULL) {
        li->mOnSystemUiVisibilityChangeListener->OnSystemUiVisibilityChange(
                visibility & IView::PUBLIC_STATUS_BAR_VISIBILITY_MASK);
    }

    return NOERROR;
}

Boolean View::UpdateLocalSystemUiVisibility(
    /* [in] */ Int32 localValue,
    /* [in] */ Int32 localChanges)
{
    Int32 val = (mSystemUiVisibility&~localChanges) | (localValue&localChanges);
    if (val != mSystemUiVisibility) {
        SetSystemUiVisibility(val);
        return TRUE;
    }
    return FALSE;
}

ECode View::SetDisabledSystemUiVisibility(
    /* [in] */ Int32 flags)
{
    if (mAttachInfo != NULL) {
        if (mAttachInfo->mDisabledSystemUiVisibility != flags) {
            mAttachInfo->mDisabledSystemUiVisibility = flags;
            if (mParent != NULL) {
                mParent->RecomputeViewAttributes(IVIEW_PROBE(this));
            }
        }
    }

    return NOERROR;
}

Boolean View::StartDrag(
    /* [in] */ IClipData* data,
    /* [in] */ IDragShadowBuilder* shadowBuilder,
    /* [in] */ IInterface* myLocalState,
    /* [in] */ Int32 flags)
{
    // if (ViewDebug.DEBUG_DRAG) {
    //     Log.d(VIEW_LOG_TAG, "startDrag: data=" + data + " flags=" + flags);
    // }
    Boolean okay = FALSE;

    AutoPtr<IPoint> shadowSize;
    CPoint::New((IPoint**)&shadowSize);
    AutoPtr<IPoint> shadowTouchPoint;
    CPoint::New((IPoint**)&shadowTouchPoint);
    shadowBuilder->OnProvideShadowMetrics(shadowSize, shadowTouchPoint);

    Int32 shadowSizeX, shadowSizeY;
    shadowSize->GetX(&shadowSizeX);
    shadowSize->GetY(&shadowSizeY);

    Int32 shadowTouchPointX, shadowTouchPointY;
    shadowTouchPoint->GetX(&shadowTouchPointX);
    shadowTouchPoint->GetY(&shadowTouchPointY);

    if (shadowSizeX < 0 || shadowSizeY < 0 ||
        shadowTouchPointX < 0 || shadowTouchPointY < 0) {
        assert(0);
        //throw new IllegalStateException("Drag shadow dimensions must not be negative");
    }

    // if (ViewDebug.DEBUG_DRAG) {
    //     Log.d(VIEW_LOG_TAG, "drag shadow: width=" + shadowSize.x + " height=" + shadowSize.y
    //             + " shadowX=" + shadowTouchPoint.x + " shadowY=" + shadowTouchPoint.y);
    // }
    AutoPtr<ISurface> surface;
    //CSurface::New((ISurface**)&surface);//TODO:remove
    ECode ec = NOERROR;

    do {
        AutoPtr<IBinder> token;
        ec = mAttachInfo->mSession->PrepareDrag(mAttachInfo->mWindow,
                flags, shadowSizeX, shadowSizeY, (ISurface**)&surface, (IBinder**)&token);
        // if (ViewDebug.DEBUG_DRAG) Log.d(VIEW_LOG_TAG, "prepareDrag returned token=" + token
        //         + " surface=" + surface);
        if (FAILED(ec)) {
            break;
        }

        if (token != NULL) {
            AutoPtr<ICanvas> canvas;
            ec = surface->LockCanvas(NULL, (ICanvas**)&canvas);
            if (FAILED(ec)) {
                break;
            }

            ec = canvas->DrawColor(0, PorterDuffMode_CLEAR);
            if (FAILED(ec)) {
                surface->UnlockCanvasAndPost(canvas);
                break;
            }

            ec = shadowBuilder->OnDrawShadow(canvas);
            if (FAILED(ec)) {
                surface->UnlockCanvasAndPost(canvas);
                break;
            }

            surface->UnlockCanvasAndPost(canvas);

            AutoPtr<ViewRootImpl> root = GetViewRootImpl();

            // Cache the local state object for delivery with DragEvents
            root->SetLocalDragState(myLocalState);

            // repurpose 'shadowSize' for the last touch point
            root->GetLastTouchPoint(shadowSize);

            ec = mAttachInfo->mSession->PerformDrag(
                mAttachInfo->mWindow, token, shadowSizeX, shadowSizeY,
                shadowTouchPointX, shadowTouchPointY, data, &okay);
            if (FAILED(ec)) {
                break;
            }
            // if (ViewDebug.DEBUG_DRAG) Log.d(VIEW_LOG_TAG, "performDrag returned " + okay);

            // Off and running!  Release our local surface instance; the drag
            // shadow surface is now managed by the system process.
            surface->ReleaseSurface();
        }
    } while (0);

    if (FAILED(ec)) {
        Logger::E(VIEW_LOG_TAG, "Unable to initiate drag");
        surface->Destroy();
    }

    return okay;
}

Boolean View::OnDragEvent(
    /* [in] */ IDragEvent* event)
{
    return FALSE;
}

Boolean View::DispatchDragEvent(
    /* [in] */ IDragEvent* event)
{
    //noinspection SimplifiableIfStatement
    AutoPtr<ListenerInfo> li = mListenerInfo;
    Boolean res = FALSE;
    if (li != NULL && li->mOnDragListener != NULL && (mViewFlags & ENABLED_MASK) == ENABLED
            && (li->mOnDragListener->OnDrag(IVIEW_PROBE(this), event, &res), res)) {
        return TRUE;
    }
    return OnDragEvent(event);
}

Boolean View::CanAcceptDrag()
{
    return (mPrivateFlags2 & PFLAG2_DRAG_CAN_ACCEPT) != 0;
}

/**
 * This needs to be a better API (NOT ON VIEW) before it is exposed.  If
 * it is ever exposed at all.
 * @hide
 */
ECode View::OnCloseSystemDialogs(
    /* [in] */ const String& reason)
{
    return NOERROR;
}

/**
 * Given a Drawable whose bounds have been set to draw into this view,
 * update a Region being computed for {@link #gatherTransparentRegion} so
 * that any non-transparent parts of the Drawable are removed from the
 * given transparent region.
 *
 * @param dr The Drawable whose transparency is to be applied to the region.
 * @param region A Region holding the current transparency information,
 * where any parts of the region that are set are considered to be
 * transparent.  On return, this region will be modified to have the
 * transparency information reduced by the corresponding parts of the
 * Drawable that are not transparent.
 * {@hide}
 */
ECode View::ApplyDrawableToTransparentRegion(
    /* [in] */ IDrawable* dr,
    /* [in] */ IRegion* region)
{
    if (DBG) {
        //Log.i(VIEW_LOG_TAG, "Getting transparent region for: " + this);
    }
    AutoPtr<IRegion> r;
    dr->GetTransparentRegion((IRegion**)&r);
    AutoPtr<IRect> _db;
    dr->GetBounds((IRect**)&_db);
    AttachInfo* attachInfo = mAttachInfo;
    Boolean result;
    if (r != NULL && attachInfo != NULL) {
        Int32 w = GetRight() - GetLeft();
        Int32 h = GetBottom() - GetTop();
        CRect* db = (CRect*)_db.Get();
        if (db->mLeft > 0) {
            //Log.i("VIEW", "Drawable left " + db.left + " > view 0");
            r->Op(0, 0, db->mLeft, h, RegionOp_UNION, &result);
        }
        if (db->mRight < w) {
            //Log.i("VIEW", "Drawable right " + db.right + " < view " + w);
            r->Op(db->mRight, 0, w, h, RegionOp_UNION, &result);
        }
        if (db->mTop > 0) {
            //Log.i("VIEW", "Drawable top " + db.top + " > view 0");
            r->Op(0, 0, w, db->mTop, RegionOp_UNION, &result);
        }
        if (db->mBottom < h) {
            //Log.i("VIEW", "Drawable bottom " + db.bottom + " < view " + h);
            r->Op(0, db->mBottom, w, h, RegionOp_UNION, &result);
        }
        Int32* location = attachInfo->mTransparentLocation;
        GetLocationInWindow(location, location + 1);
        r->Translate(location[0], location[1]);
        region->Op(r, RegionOp_INTERSECT, &result);
    }
    else {
        region->Op(_db, RegionOp_DIFFERENCE, &result);
    }

    return NOERROR;
}

void View::CheckForLongClick(
    /* [in] */ Int32 delayOffset)
{
    if ((mViewFlags & LONG_CLICKABLE) == LONG_CLICKABLE) {
        mHasPerformedLongPress = FALSE;

        if (mPendingCheckForLongPress == NULL) {
            mPendingCheckForLongPress = new CheckForLongPress(this);
        }
        mPendingCheckForLongPress->RememberWindowAttachCount();
        PostDelayed(mPendingCheckForLongPress,
                CViewConfiguration::GetLongPressTimeout() - delayOffset);
    }
}

/**
 * Inflate a view from an XML resource.  This convenience method wraps the {@link
 * LayoutInflater} class, which provides a full range of options for view inflation.
 *
 * @param context The Context object for your activity or application.
 * @param resource The resource ID to inflate
 * @param root A view group that will be the parent.  Used to properly inflate the
 * layout_* parameters.
 * @see LayoutInflater
 */
ECode View::Inflate(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resource,
    /* [in] */ IViewGroup* root,
    /* [out] */ IView** view)
{
    AutoPtr<ILayoutInflater> factory;
    FAIL_RETURN(LayoutInflater::From(context, (ILayoutInflater**)&factory));
    return factory->Inflate(resource, root, view);
}

/**
 * Scroll the view with standard behavior for scrolling beyond the normal
 * content boundaries. Views that call this method should override
 * {@link #onOverScrolled(int, int, Boolean, Boolean)} to respond to the
 * results of an over-scroll operation.
 *
 * Views can use this method to handle any touch or fling-based scrolling.
 *
 * @param deltaX Change in X in pixels
 * @param deltaY Change in Y in pixels
 * @param scrollX Current X scroll value in pixels before applying deltaX
 * @param scrollY Current Y scroll value in pixels before applying deltaY
 * @param scrollRangeX Maximum content scroll range along the X axis
 * @param scrollRangeY Maximum content scroll range along the Y axis
 * @param maxOverScrollX Number of pixels to overscroll by in either direction
 *          along the X axis.
 * @param maxOverScrollY Number of pixels to overscroll by in either direction
 *          along the Y axis.
 * @param isTouchEvent TRUE if this scroll operation is the result of a touch event.
 * @return TRUE if scrolling was clamped to an over-scroll boundary along either
 *          axis, FALSE otherwise.
 */
Boolean View::OverScrollBy(
    /* [in] */ Int32 deltaX,
    /* [in] */ Int32 deltaY,
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Int32 scrollRangeX,
    /* [in] */ Int32 scrollRangeY,
    /* [in] */ Int32 maxOverScrollX,
    /* [in] */ Int32 maxOverScrollY,
    /* [in] */ Boolean isTouchEvent)
{
    Int32 overScrollMode = mOverScrollMode;
    Boolean canScrollHorizontal =
            ComputeHorizontalScrollRange() > ComputeHorizontalScrollExtent();
    Boolean canScrollVertical =
            ComputeVerticalScrollRange() > ComputeVerticalScrollExtent();
    Boolean overScrollHorizontal = overScrollMode == IView::OVER_SCROLL_ALWAYS ||
            (overScrollMode == IView::OVER_SCROLL_IF_CONTENT_SCROLLS && canScrollHorizontal);
    Boolean overScrollVertical = overScrollMode == IView::OVER_SCROLL_ALWAYS ||
            (overScrollMode == IView::OVER_SCROLL_IF_CONTENT_SCROLLS && canScrollVertical);

    Int32 newScrollX = scrollX + deltaX;
    if (!overScrollHorizontal) {
        maxOverScrollX = 0;
    }

    Int32 newScrollY = scrollY + deltaY;
    if (!overScrollVertical) {
        maxOverScrollY = 0;
    }

    // Clamp values if at the limits and record
    Int32 left = -maxOverScrollX;
    Int32 right = maxOverScrollX + scrollRangeX;
    Int32 top = -maxOverScrollY;
    Int32 bottom = maxOverScrollY + scrollRangeY;

    Boolean clampedX = FALSE;
    if (newScrollX > right) {
        newScrollX = right;
        clampedX = TRUE;
    }
    else if (newScrollX < left) {
        newScrollX = left;
        clampedX = TRUE;
    }

    Boolean clampedY = FALSE;
    if (newScrollY > bottom) {
        newScrollY = bottom;
        clampedY = TRUE;
    }
    else if (newScrollY < top) {
        newScrollY = top;
        clampedY = TRUE;
    }

    OnOverScrolled(newScrollX, newScrollY, clampedX, clampedY);

    return clampedX || clampedY;
}

/**
 * Called by {@link #overScrollBy(int, int, int, int, int, int, int, int, Boolean)} to
 * respond to the results of an over-scroll operation.
 *
 * @param scrollX New X scroll value in pixels
 * @param scrollY New Y scroll value in pixels
 * @param clampedX True if scrollX was clamped to an over-scroll boundary
 * @param clampedY True if scrollY was clamped to an over-scroll boundary
 */
void View::OnOverScrolled(
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Boolean clampedX,
    /* [in] */ Boolean clampedY)
{
    // Intentionally empty.
}

/**
 * Returns the over-scroll mode for this view. The result will be
 * one of {@link #OVER_SCROLL_ALWAYS} (default), {@link #OVER_SCROLL_IF_CONTENT_SCROLLS}
 * (allow over-scrolling only if the view content is larger than the container),
 * or {@link #OVER_SCROLL_NEVER}.
 *
 * @return This view's over-scroll mode.
 */
Int32 View::GetOverScrollMode()
{
    return mOverScrollMode;
}

/**
 * Set the over-scroll mode for this view. Valid over-scroll modes are
 * {@link #OVER_SCROLL_ALWAYS} (default), {@link #OVER_SCROLL_IF_CONTENT_SCROLLS}
 * (allow over-scrolling only if the view content is larger than the container),
 * or {@link #OVER_SCROLL_NEVER}.
 *
 * Setting the over-scroll mode of a view will have an effect only if the
 * view is capable of scrolling.
 *
 * @param overScrollMode The new over-scroll mode for this view.
 */
ECode View::SetOverScrollMode(
    /* [in] */ Int32 overScrollMode)
{
    if (overScrollMode != IView::OVER_SCROLL_ALWAYS &&
            overScrollMode != IView::OVER_SCROLL_IF_CONTENT_SCROLLS &&
            overScrollMode != IView::OVER_SCROLL_NEVER) {
//        throw new IllegalArgumentException("Invalid overscroll mode " + overScrollMode);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mOverScrollMode = overScrollMode;

    return NOERROR;
}


/**
 * Enable or disable nested scrolling for this view.
 *
 * <p>If this property is set to true the view will be permitted to initiate nested
 * scrolling operations with a compatible parent view in the current hierarchy. If this
 * view does not implement nested scrolling this will have no effect. Disabling nested scrolling
 * while a nested scroll is in progress has the effect of {@link #stopNestedScroll() stopping}
 * the nested scroll.</p>
 *
 * @param enabled true to enable nested scrolling, false to disable
 *
 * @see #isNestedScrollingEnabled()
 */
ECode View::SetNestedScrollingEnabled(
    /* [in] */ Boolean enabled)
{
    if (enabled) {
        mPrivateFlags3 |= PFLAG3_NESTED_SCROLLING_ENABLED;
    } else {
        StopNestedScroll();
        mPrivateFlags3 &= ~PFLAG3_NESTED_SCROLLING_ENABLED;
    }
}

/**
 * Returns true if nested scrolling is enabled for this view.
 *
 * <p>If nested scrolling is enabled and this View class implementation supports it,
 * this view will act as a nested scrolling child view when applicable, forwarding data
 * about the scroll operation in progress to a compatible and cooperating nested scrolling
 * parent.</p>
 *
 * @return true if nested scrolling is enabled
 *
 * @see #setNestedScrollingEnabled(boolean)
 */
ECode View::IsNestedScrollingEnabled(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = (mPrivateFlags3 & PFLAG3_NESTED_SCROLLING_ENABLED) ==
            PFLAG3_NESTED_SCROLLING_ENABLED;

    return NOERROR;
}

/**
 * Begin a nestable scroll operation along the given axes.
 *
 * <p>A view starting a nested scroll promises to abide by the following contract:</p>
 *
 * <p>The view will call startNestedScroll upon initiating a scroll operation. In the case
 * of a touch scroll this corresponds to the initial {@link MotionEvent#ACTION_DOWN}.
 * In the case of touch scrolling the nested scroll will be terminated automatically in
 * the same manner as {@link ViewParent#requestDisallowInterceptTouchEvent(boolean)}.
 * In the event of programmatic scrolling the caller must explicitly call
 * {@link #stopNestedScroll()} to indicate the end of the nested scroll.</p>
 *
 * <p>If <code>startNestedScroll</code> returns true, a cooperative parent was found.
 * If it returns false the caller may ignore the rest of this contract until the next scroll.
 * Calling startNestedScroll while a nested scroll is already in progress will return true.</p>
 *
 * <p>At each incremental step of the scroll the caller should invoke
 * {@link #dispatchNestedPreScroll(int, int, int[], int[]) dispatchNestedPreScroll}
 * once it has calculated the requested scrolling delta. If it returns true the nested scrolling
 * parent at least partially consumed the scroll and the caller should adjust the amount it
 * scrolls by.</p>
 *
 * <p>After applying the remainder of the scroll delta the caller should invoke
 * {@link #dispatchNestedScroll(int, int, int, int, int[]) dispatchNestedScroll}, passing
 * both the delta consumed and the delta unconsumed. A nested scrolling parent may treat
 * these values differently. See {@link ViewParent#onNestedScroll(View, int, int, int, int)}.
 * </p>
 *
 * @param axes Flags consisting of a combination of {@link #SCROLL_AXIS_HORIZONTAL} and/or
 *             {@link #SCROLL_AXIS_VERTICAL}.
 * @return true if a cooperative parent was found and nested scrolling has been enabled for
 *         the current gesture.
 *
 * @see #stopNestedScroll()
 * @see #dispatchNestedPreScroll(int, int, int[], int[])
 * @see #dispatchNestedScroll(int, int, int, int, int[])
 */
ECode View::StartNestedScroll(
    /* [in] */ Int32 axes,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    Boolean hasNestedScrollingParent;
    if (HasNestedScrollingParent(&hasNestedScrollingParent), hasNestedScrollingParent) {
        // Already in progress
        *res = TRUE;
        return NOERROR;
    }
    Boolean isNestedScrollingEnabled;
    if (IsNestedScrollingEnabled(&isNestedScrollingEnabled), isNestedScrollingEnabled) {
        AutoPtr<IViewParent> p;
        GetParent((IViewParent**)&p);
        AutoPtr<IView> child = THIS_PROBE(IView);
        while (p != NULL) {
            assert(0 && "TODO");
            /*try {
                if (p.onStartNestedScroll(child, this, axes)) {
                    mNestedScrollingParent = p;
                    p.onNestedScrollAccepted(child, this, axes);
                    return true;
                }
            } catch (AbstractMethodError e) {
                Log.e(VIEW_LOG_TAG, "ViewParent " + p + " does not implement interface " +
                        "method onStartNestedScroll", e);
                // Allow the search upward to continue
            }*/
            if (IView::Probe(p)) {
                child = IView::Probe(p);
            }
            p->GetParent((IViewParent**)&p);
        }
    }
    *res = FALSE;
    return NOERROR;
}

/**
 * Stop a nested scroll in progress.
 *
 * <p>Calling this method when a nested scroll is not currently in progress is harmless.</p>
 *
 * @see #startNestedScroll(int)
 */
ECode View::StopNestedScroll()
{
    if (mNestedScrollingParent != NULL) {
        mNestedScrollingParent->OnStopNestedScroll(THIS_PROBE(IView));
        mNestedScrollingParent = NULL;
    }
}

/**
 * Returns true if this view has a nested scrolling parent.
 *
 * <p>The presence of a nested scrolling parent indicates that this view has initiated
 * a nested scroll and it was accepted by an ancestor view further up the view hierarchy.</p>
 *
 * @return whether this view has a nested scrolling parent
 */
ECode View::HasNestedScrollingParent(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = mNestedScrollingParent != NULL;
    return NOERROR;
}

/**
 * Dispatch one step of a nested scroll in progress.
 *
 * <p>Implementations of views that support nested scrolling should call this to report
 * info about a scroll in progress to the current nested scrolling parent. If a nested scroll
 * is not currently in progress or nested scrolling is not
 * {@link #isNestedScrollingEnabled() enabled} for this view this method does nothing.</p>
 *
 * <p>Compatible View implementations should also call
 * {@link #dispatchNestedPreScroll(int, int, int[], int[]) dispatchNestedPreScroll} before
 * consuming a component of the scroll event themselves.</p>
 *
 * @param dxConsumed Horizontal distance in pixels consumed by this view during this scroll step
 * @param dyConsumed Vertical distance in pixels consumed by this view during this scroll step
 * @param dxUnconsumed Horizontal scroll distance in pixels not consumed by this view
 * @param dyUnconsumed Horizontal scroll distance in pixels not consumed by this view
 * @param offsetInWindow Optional. If not null, on return this will contain the offset
 *                       in local view coordinates of this view from before this operation
 *                       to after it completes. View implementations may use this to adjust
 *                       expected input coordinate tracking.
 * @return true if the event was dispatched, false if it could not be dispatched.
 * @see #dispatchNestedPreScroll(int, int, int[], int[])
 */
ECode View::DispatchNestedScroll(
    /* [in] */ Int32 dxConsumed,
    /* [in] */ Int32 dyConsumed,
    /* [in] */ Int32 dxUnconsumed,
    /* [in] */ Int32 dyUnconsumed,
    /* [in] */ ArrayOf<Int32>* offsetInWindow,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    Boolean isNestedScrollingEnabled;
    IsNestedScrollingEnabled(&isNestedScrollingEnabled);
    if (isNestedScrollingEnabled && mNestedScrollingParent) {
        if (dxConsumed != 0 || dyConsumed != 0 || dxUnconsumed != 0 || dyUnconsumed != 0) {
            Int32 startX = 0;
            Int32 startY = 0;
            if (offsetInWindow != NULL) {
                GetLocationInWindow(offsetInWindow);
                startX = (*offsetInWindow)[0];
                startY = (*offsetInWindow)[1];
            }

            mNestedScrollingParent->OnNestedScroll(THIS_PROBE(IView), dxConsumed, dyConsumed,
                    dxUnconsumed, dyUnconsumed);

            if (offsetInWindow != NULL) {
                GetLocationInWindow(offsetInWindow);
                (*offsetInWindow)[0] -= startX;
                (*offsetInWindow)[1] -= startY;
            }
            *res = TRUE;
            return NOERROR;
        } else if (offsetInWindow != NULL) {
            // No motion, no dispatch. Keep offsetInWindow up to date.
            (*offsetInWindow)[0] = 0;
            (*offsetInWindow)[1] = 0;
        }
    }
    *res = FALSE;
    return NOERROR;
}

/**
 * Dispatch one step of a nested scroll in progress before this view consumes any portion of it.
 *
 * <p>Nested pre-scroll events are to nested scroll events what touch intercept is to touch.
 * <code>dispatchNestedPreScroll</code> offers an opportunity for the parent view in a nested
 * scrolling operation to consume some or all of the scroll operation before the child view
 * consumes it.</p>
 *
 * @param dx Horizontal scroll distance in pixels
 * @param dy Vertical scroll distance in pixels
 * @param consumed Output. If not null, consumed[0] will contain the consumed component of dx
 *                 and consumed[1] the consumed dy.
 * @param offsetInWindow Optional. If not null, on return this will contain the offset
 *                       in local view coordinates of this view from before this operation
 *                       to after it completes. View implementations may use this to adjust
 *                       expected input coordinate tracking.
 * @return true if the parent consumed some or all of the scroll delta
 * @see #dispatchNestedScroll(int, int, int, int, int[])
 */
ECode View::DispatchNestedPreScroll(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy,
    /* [in] */ ArrayOf<Int32>* consumed,
    /* [in] */ ArrayOf<Int32>* offsetInWindow,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    Boolean isNestedScrollingEnabled;
    IsNestedScrollingEnabled(&isNestedScrollingEnabled);
    if (isNestedScrollingEnabled && mNestedScrollingParent != NULL) {
        if (dx != 0 || dy != 0) {
            Int32 startX = 0;
            Int32 startY = 0;
            if (offsetInWindow != NULL) {
                GetLocationInWindow(offsetInWindow);
                startX = (*offsetInWindow)[0];
                startY = (*offsetInWindow)[1];
            }

            if (consumed == NULL) {
                if (mTempNestedScrollConsumed == NULL) {
                    mTempNestedScrollConsumed = ArrayOf<Int32>::Allow(2);
                }
                consumed = mTempNestedScrollConsumed;
            }
            (*consumed)[0] = 0;
            (*consumed)[1] = 0;
            mNestedScrollingParent->OnNestedPreScroll(THIS_PROBE(IView), dx, dy, consumed);

            if (offsetInWindow != NULL) {
                GetLocationInWindow(offsetInWindow);
                (*offsetInWindow)[0] -= startX;
                (*offsetInWindow)[1] -= startY;
            }
            *res = (*consumed)[0] != 0 || (*consumed)[1] != 0;
            return NOERROR;
        } else if (offsetInWindow != NULL) {
            (*offsetInWindow)[0] = 0;
            (*offsetInWindow)[1] = 0;
        }
    }
    *res = FALSE;
    return NOERROR;
}

/**
 * Dispatch a fling to a nested scrolling parent.
 *
 * <p>This method should be used to indicate that a nested scrolling child has detected
 * suitable conditions for a fling. Generally this means that a touch scroll has ended with a
 * {@link VelocityTracker velocity} in the direction of scrolling that meets or exceeds
 * the {@link ViewConfiguration#getScaledMinimumFlingVelocity() minimum fling velocity}
 * along a scrollable axis.</p>
 *
 * <p>If a nested scrolling child view would normally fling but it is at the edge of
 * its own content, it can use this method to delegate the fling to its nested scrolling
 * parent instead. The parent may optionally consume the fling or observe a child fling.</p>
 *
 * @param velocityX Horizontal fling velocity in pixels per second
 * @param velocityY Vertical fling velocity in pixels per second
 * @param consumed true if the child consumed the fling, false otherwise
 * @return true if the nested scrolling parent consumed or otherwise reacted to the fling
 */
ECode View::DispatchNestedFling(
    /* [in] */ Float velocityX,
    /* [in] */ Float velocityY,
    /* [in] */ boolean consumed,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    Boolean isNestedScrollingEnabled;
    IsNestedScrollingEnabled(&isNestedScrollingEnabled);
    if (isNestedScrollingEnabled && mNestedScrollingParent != NULL) {
        return mNestedScrollingParent->OnNestedFling(THIS_PROBE(IView), velocityX, velocityY, consumed, res);
    }
    *res = FALSE;
    return NOERROR;
}

/**
 * Dispatch a fling to a nested scrolling parent before it is processed by this view.
 *
 * <p>Nested pre-fling events are to nested fling events what touch intercept is to touch
 * and what nested pre-scroll is to nested scroll. <code>dispatchNestedPreFling</code>
 * offsets an opportunity for the parent view in a nested fling to fully consume the fling
 * before the child view consumes it. If this method returns <code>true</code>, a nested
 * parent view consumed the fling and this view should not scroll as a result.</p>
 *
 * <p>For a better user experience, only one view in a nested scrolling chain should consume
 * the fling at a time. If a parent view consumed the fling this method will return false.
 * Custom view implementations should account for this in two ways:</p>
 *
 * <ul>
 *     <li>If a custom view is paged and needs to settle to a fixed page-point, do not
 *     call <code>dispatchNestedPreFling</code>; consume the fling and settle to a valid
 *     position regardless.</li>
 *     <li>If a nested parent does consume the fling, this view should not scroll at all,
 *     even to settle back to a valid idle position.</li>
 * </ul>
 *
 * <p>Views should also not offer fling velocities to nested parent views along an axis
 * where scrolling is not currently supported; a {@link android.widget.ScrollView ScrollView}
 * should not offer a horizontal fling velocity to its parents since scrolling along that
 * axis is not permitted and carrying velocity along that motion does not make sense.</p>
 *
 * @param velocityX Horizontal fling velocity in pixels per second
 * @param velocityY Vertical fling velocity in pixels per second
 * @return true if a nested scrolling parent consumed the fling
 */
ECode View::DispatchNestedPreFling(
    /* [in] */ Float velocityX,
    /* [in] */ Float velocityY,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    Boolean isNestedScrollingEnabled;
    IsNestedScrollingEnabled(&isNestedScrollingEnabled);
    if (isNestedScrollingEnabled && mNestedScrollingParent != NULL) {
        return mNestedScrollingParent->OnNestedPreFling(THIS_PROBE(IView), velocityX, velocityY, res);
    }
    *res = FALSE;
    return NOERROR;
}


Float View::GetVerticalScrollFactor()
{
    if (mVerticalScrollFactor == 0) {
        AutoPtr<ITypedValue> outValue;
        CTypedValue::New((ITypedValue**)&outValue);
        AutoPtr<IResourcesTheme> theme;
        mContext->GetTheme((IResourcesTheme**)&theme);
        assert(theme != NULL);

        Boolean res = FALSE;
        theme->ResolveAttribute(R::attr::listPreferredItemHeight, outValue, TRUE, &res);
        if (!res) {
            assert(0);
            // throw new IllegalStateException(
            //         "Expected theme to define listPreferredItemHeight.");
        }

        AutoPtr<IResources> resource;
        mContext->GetResources((IResources**)&resource);
        assert(resource != NULL);

        AutoPtr<IDisplayMetrics> display;
        resource->GetDisplayMetrics((IDisplayMetrics**)&display);
        outValue->GetDimension(display, &mVerticalScrollFactor);
    }

    return mVerticalScrollFactor;
}

Float View::GetHorizontalScrollFactor()
{
    // TODO: Should use something else.
    return GetVerticalScrollFactor();
}

Int32 View::GetRawTextDirection()
{
    return (mPrivateFlags2 & PFLAG2_TEXT_DIRECTION_MASK) >> PFLAG2_TEXT_DIRECTION_MASK_SHIFT;
}

ECode View::SetTextDirection(
    /* [in] */ Int32 textDirection)
{
    if (GetRawTextDirection() != textDirection) {
        // Reset the current text direction and the resolved one
        mPrivateFlags2 &= ~PFLAG2_TEXT_DIRECTION_MASK;
        ResetResolvedTextDirection();
        // Set the new text direction
        mPrivateFlags2 |= ((textDirection << PFLAG2_TEXT_DIRECTION_MASK_SHIFT)
            & PFLAG2_TEXT_DIRECTION_MASK);
        // Do resolution
        ResolveTextDirection();
        // Notify change
        OnRtlPropertiesChanged(GetLayoutDirection());
        // Refresh
        RequestLayout();
        Invalidate(TRUE);
    }

    return NOERROR;
}

Int32 View::GetTextDirection()
{
    return (mPrivateFlags2 & PFLAG2_TEXT_DIRECTION_RESOLVED_MASK)
        >> PFLAG2_TEXT_DIRECTION_RESOLVED_MASK_SHIFT;
}

ECode View::ResolveTextDirection(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    // Reset any previous text direction resolution
    mPrivateFlags2 &= ~(PFLAG2_TEXT_DIRECTION_RESOLVED | PFLAG2_TEXT_DIRECTION_RESOLVED_MASK);

    if (HasRtlSupport()) {
        // Set resolved text direction flag depending on text direction flag
        const Int32 textDirection = GetRawTextDirection();
        switch(textDirection) {
            case IView::TEXT_DIRECTION_INHERIT:
            {
                if (!CanResolveTextDirection()) {
                    // We cannot do the resolution if there is no parent, so use the default one
                    mPrivateFlags2 |= PFLAG2_TEXT_DIRECTION_RESOLVED_DEFAULT;
                    // Resolution will need to happen again later
                    return FALSE;
                }
                // Parent has not yet resolved, so we still return the default
                Boolean isTextDirectionResolved;
                ECode ec = mParent->IsTextDirectionResolved(&isTextDirectionResolved);
                if (ec == E_ABSTRACET_METHOD_ERROR) {

                    /*Log.e(VIEW_LOG_TAG, mParent.getClass().getSimpleName() +
                            " does not fully implement ViewParent", e);*/
                    mPrivateFlags2 |= PFLAG2_TEXT_DIRECTION_RESOLVED |
                            PFLAG2_TEXT_DIRECTION_RESOLVED_DEFAULT;
                    *res = TRUE;
                    return NOERROR;
                }

                FAIL_RETURN(ec)
                if (!isTextDirectionResolved) {
                    mPrivateFlags2 |= PFLAG2_TEXT_DIRECTION_RESOLVED_DEFAULT;
                    // Resolution will need to happen again later
                    *res = FALSE;
                    return NOERROR;
                }

                // Set current resolved direction to the same value as the parent's one
                Int32 parentResolvedDirection;
                ECode oc = mParent->GetTextDirection(&parentResolvedDirection);
                if (oc == E_ABSTRACET_METHOD_ERROR) {
                    /*Log.e(VIEW_LOG_TAG, mParent.getClass().getSimpleName() +
                            " does not fully implement ViewParent", e);*/
                    parentResolvedDirection = IView::TEXT_DIRECTION_LTR;
                }

                FAIL_RETURN(oc)

                switch (parentResolvedDirection) {
                    case IView::TEXT_DIRECTION_FIRST_STRONG:
                    case IView::TEXT_DIRECTION_ANY_RTL:
                    case IView::TEXT_DIRECTION_LTR:
                    case IView::TEXT_DIRECTION_RTL:
                    case IView::TEXT_DIRECTION_LOCALE:
                        mPrivateFlags2 |=
                                (parentResolvedDirection << PFLAG2_TEXT_DIRECTION_RESOLVED_MASK_SHIFT);
                        break;
                    default:
                        // Default resolved direction is "first strong" heuristic
                        mPrivateFlags2 |= PFLAG2_TEXT_DIRECTION_RESOLVED_DEFAULT;
                }
                break;
            }
            case IView::TEXT_DIRECTION_FIRST_STRONG:
            case IView::TEXT_DIRECTION_ANY_RTL:
            case IView::TEXT_DIRECTION_LTR:
            case IView::TEXT_DIRECTION_RTL:
            case IView::TEXT_DIRECTION_LOCALE:
                // Resolved direction is the same as text direction
                mPrivateFlags2 |= (textDirection << PFLAG2_TEXT_DIRECTION_RESOLVED_MASK_SHIFT);
                break;
            default:
                // Default resolved direction is "first strong" heuristic
                mPrivateFlags2 |= PFLAG2_TEXT_DIRECTION_RESOLVED_DEFAULT;
        }
    }
    else {
        // Default resolved direction is "first strong" heuristic
        mPrivateFlags2 |= PFLAG2_TEXT_DIRECTION_RESOLVED_DEFAULT;
    }

    // Set to resolved
    mPrivateFlags2 |= PFLAG2_TEXT_DIRECTION_RESOLVED;
    return TRUE;
}

ECode View::CanResolveTextDirection(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    Int32 direction;
    switch (GetRawTextDirection(&direction), direction) {
        case IView::TEXT_DIRECTION_INHERIT:
        {
            if (mParent != NULL) {
                Ecode ec = mParent->CanResolveTextDirection(res);
                if (ec == E_ABSTRACET_METHOD_ERROR) {
                    /*Log.e(VIEW_LOG_TAG, mParent.getClass().getSimpleName() +
                        " does not fully implement ViewParent", e);*/
                }

                return ec;
            }
            *res = FALSE;
            return NOERROR;
        }

        default:
            *res = TRUE;
            return NOERROR;
    }
}

ECode View::ResetResolvedTextDirection()
{
    // Reset any previous text direction resolution
    mPrivateFlags2 &= ~(PFLAG2_TEXT_DIRECTION_RESOLVED | PFLAG2_TEXT_DIRECTION_RESOLVED_MASK);
    // Set to default value
    mPrivateFlags2 |= PFLAG2_TEXT_DIRECTION_RESOLVED_DEFAULT;
    return NOERROR;
}

Boolean View::IsTextDirectionInherited()
{
    return (GetRawTextDirection() == IView::TEXT_DIRECTION_INHERIT);
}

ECode View::IsTextDirectionResolved(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = (mPrivateFlags2 & PFLAG2_TEXT_DIRECTION_RESOLVED) == PFLAG2_TEXT_DIRECTION_RESOLVED;
    return NOERROR;
}

Int32 View::GetRawTextAlignment()
{
    return (mPrivateFlags2 & PFLAG2_TEXT_ALIGNMENT_MASK) >> PFLAG2_TEXT_ALIGNMENT_MASK_SHIFT;
}

ECode View::SetTextAlignment(
    /* [in] */ Int32 textAlignment)
{
    if (textAlignment != GetRawTextAlignment()) {
        // Reset the current and resolved text alignment
        mPrivateFlags2 &= ~PFLAG2_TEXT_ALIGNMENT_MASK;
        ResetResolvedTextAlignment();
        // Set the new text alignment
        mPrivateFlags2 |=
                ((textAlignment << PFLAG2_TEXT_ALIGNMENT_MASK_SHIFT) & PFLAG2_TEXT_ALIGNMENT_MASK);
        // Do resolution
        ResolveTextAlignment();
        // Notify change
        OnRtlPropertiesChanged(GetLayoutDirection());
        // Refresh
        RequestLayout();
        Invalidate(TRUE);
    }

    return NOERROR;
}

Int32 View::GetTextAlignment()
{
    return (mPrivateFlags2 & PFLAG2_TEXT_ALIGNMENT_RESOLVED_MASK) >>
            PFLAG2_TEXT_ALIGNMENT_RESOLVED_MASK_SHIFT;
}

ECode View::ResolveTextAlignment(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    // Reset any previous text alignment resolution
    mPrivateFlags2 &= ~(PFLAG2_TEXT_ALIGNMENT_RESOLVED | PFLAG2_TEXT_ALIGNMENT_RESOLVED_MASK);

    if (HasRtlSupport()) {
        // Set resolved text alignment flag depending on text alignment flag
        const Int32 textAlignment = GetRawTextAlignment();
        switch (textAlignment) {
            case IView::TEXT_ALIGNMENT_INHERIT:
            {
                // Check if we can resolve the text alignment
                if (!CanResolveTextAlignment()) {
                    // We cannot do the resolution if there is no parent so use the default
                    mPrivateFlags2 |= PFLAG2_TEXT_ALIGNMENT_RESOLVED_DEFAULT;
                    // Resolution will need to happen again later
                    *res = FALSE;
                    return NOERROR;
                }

                // Parent has not yet resolved, so we still return the default
                Boolean isTextAlignmentResolved;
                ECode ec = mParent->IsTextAlignmentResolved(&isTextAlignmentResolved);
                if (ec == E_ABSTRACET_METHOD_ERROR) {
                    /*Log.e(VIEW_LOG_TAG, mParent.getClass().getSimpleName() +
                            " does not fully implement ViewParent", e);*/
                    mPrivateFlags2 |= PFLAG2_TEXT_ALIGNMENT_RESOLVED |
                            PFLAG2_TEXT_ALIGNMENT_RESOLVED_DEFAULT;
                    *res = TRUE;
                    return NOERROR;
                }

                FAIL_RETURN(ec)

                if (!isTextAlignmentResolved) {
                    mPrivateFlags2 |= PFLAG2_TEXT_ALIGNMENT_RESOLVED_DEFAULT;
                    // Resolution will need to happen again later
                    *res = FALSE;
                    return NOERROR;
                }

                Int32 parentResolvedTextAlignment;

                ECode oc = mParent->GetTextAlignment(&parentResolvedTextAlignment);
                if (oc == E_ABSTRACET_METHOD_ERROR) {
                    /*Log.e(VIEW_LOG_TAG, mParent.getClass().getSimpleName() +
                            " does not fully implement ViewParent", e);*/
                    parentResolvedTextAlignment = IView::TEXT_ALIGNMENT_GRAVITY;
                }
                FAIL_RETURN(oc)

                switch (parentResolvedTextAlignment) {
                    case IView::TEXT_ALIGNMENT_GRAVITY:
                    case IView::TEXT_ALIGNMENT_TEXT_START:
                    case IView::TEXT_ALIGNMENT_TEXT_END:
                    case IView::TEXT_ALIGNMENT_CENTER:
                    case IView::TEXT_ALIGNMENT_VIEW_START:
                    case IView::TEXT_ALIGNMENT_VIEW_END:
                        // Resolved text alignment is the same as the parent resolved
                        // text alignment
                        mPrivateFlags2 |=
                                (parentResolvedTextAlignment << PFLAG2_TEXT_ALIGNMENT_RESOLVED_MASK_SHIFT);
                        break;
                    default:
                        // Use default resolved text alignment
                        mPrivateFlags2 |= PFLAG2_TEXT_ALIGNMENT_RESOLVED_DEFAULT;
                }
                break;
            }
            case IView::TEXT_ALIGNMENT_GRAVITY:
            case IView::TEXT_ALIGNMENT_TEXT_START:
            case IView::TEXT_ALIGNMENT_TEXT_END:
            case IView::TEXT_ALIGNMENT_CENTER:
            case IView::TEXT_ALIGNMENT_VIEW_START:
            case IView::TEXT_ALIGNMENT_VIEW_END:
                // Resolved text alignment is the same as text alignment
                mPrivateFlags2 |= (textAlignment << PFLAG2_TEXT_ALIGNMENT_RESOLVED_MASK_SHIFT);
                break;
            default:
                // Use default resolved text alignment
                mPrivateFlags2 |= PFLAG2_TEXT_ALIGNMENT_RESOLVED_DEFAULT;
        }
    }
    else {
        // Use default resolved text alignment
        mPrivateFlags2 |= PFLAG2_TEXT_ALIGNMENT_RESOLVED_DEFAULT;
    }

    // Set the resolved
    mPrivateFlags2 |= PFLAG2_TEXT_ALIGNMENT_RESOLVED;
    return TRUE;
}

ECode View::CanResolveTextAlignment(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    switch (GetRawTextAlignment()) {
        case IView::TEXT_DIRECTION_INHERIT:
        {
            if (mParent != NULL) {
                ECode ec = mParent->CanResolveTextAlignment(res);
                if (ec == E_ABSTRACET_METHOD_ERROR) {
                    /*Log.e(VIEW_LOG_TAG, mParent.getClass().getSimpleName() +
                            " does not fully implement ViewParent", e);*/
                }
            }
            *res = FALSE
            return NOERROR;
        }

        default:
            *res = TRUE
            return NOERROR;
    }
}

ECode View::ResetResolvedTextAlignment()
{
    // Reset any previous text alignment resolution
    mPrivateFlags2 &= ~(PFLAG2_TEXT_ALIGNMENT_RESOLVED | PFLAG2_TEXT_ALIGNMENT_RESOLVED_MASK);
    // Set to default
    mPrivateFlags2 |= PFLAG2_TEXT_ALIGNMENT_RESOLVED_DEFAULT;
    return NOERROR;
}

Boolean View::IsTextAlignmentInherited()
{
    return (GetRawTextAlignment() == IView::TEXT_ALIGNMENT_INHERIT);
}

ECode View::IsTextAlignmentResolved(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    *res = (mPrivateFlags2 & PFLAG2_TEXT_ALIGNMENT_RESOLVED) == PFLAG2_TEXT_ALIGNMENT_RESOLVED;
    return NOERROR;
}

Int32 View::GenerateViewId()
{
    //for (;;) {
        AutoLock lock(sNextGeneratedIdLock);
        Int32 result = sNextGeneratedId;
        // aapt-generated IDs have the high byte nonzero; clamp to the range under that.
        Int32 newValue = result + 1;
        if (newValue > 0x00FFFFFF)
            newValue = 1; // Roll over to 1, not 0.
        // if (sNextGeneratedId->CompareAndSet(result, newValue)) {
        //     return result;
        // }
        sNextGeneratedId = newValue;
        return result;
    //}
}

/**
 * Gets the Views in the hierarchy affected by entering and exiting Activity Scene transitions.
 * @param transitioningViews This View will be added to transitioningViews if it is VISIBLE and
 *                           a normal View or a ViewGroup with
 *                           {@link android.view.ViewGroup#isTransitionGroup()} true.
 * @hide
 */
ECode View::CaptureTransitioningViews(
    /* [in] */ IList* transitioningViews)
{
    Int32 visibility;
    GetVisibility(&visibility);
    if (visibility == IView::VISIBLE) {
        transitioningViews->Add(THIS_PROBE(IView));
    }
}

/**
 * Adds all Views that have {@link #getTransitionName()} non-null to namedElements.
 * @param namedElements Will contain all Views in the hierarchy having a transitionName.
 * @hide
 */
ECode View::FindNamedViews(
    /* [in] */ IMap* namedElements)
{
    Int32 visibility;
    GetVisibility(&visibility);
    if (visibility || mGhostView != NULL) {
        String transitionName;
        GetTransitionName(&transitionName);
        if (!transitionName.IsNullOrEmpty) {
            AutoPtr<ICharSequence> csq;
            CStringWrapper::New(transitionName, (ICharSequence**)&csq);
            namedElements->Put(csq, THIS_PROBE(IView));
        }
    }
}

ECode View::Init(
    /* [in] */ IContext* context)
{
    mContext = context;
    if (context != NULL) {
        context->GetResources((IResources**)&mResources);
    }
    mViewFlags = IView::SOUND_EFFECTS_ENABLED | IView::HAPTIC_FEEDBACK_ENABLED;
    // Set some flags defaults
    mPrivateFlags2 =
            (LAYOUT_DIRECTION_DEFAULT << PFLAG2_LAYOUT_DIRECTION_MASK_SHIFT) |
            (TEXT_DIRECTION_DEFAULT << PFLAG2_TEXT_DIRECTION_MASK_SHIFT) |
            (PFLAG2_TEXT_DIRECTION_RESOLVED_DEFAULT) |
            (TEXT_ALIGNMENT_DEFAULT << PFLAG2_TEXT_ALIGNMENT_MASK_SHIFT) |
            (PFLAG2_TEXT_ALIGNMENT_RESOLVED_DEFAULT) |
            (IMPORTANT_FOR_ACCESSIBILITY_DEFAULT << PFLAG2_IMPORTANT_FOR_ACCESSIBILITY_SHIFT);

    CViewConfiguration::Get(context)->GetScaledTouchSlop(&mTouchSlop);
    SetOverScrollMode(IView::OVER_SCROLL_IF_CONTENT_SCROLLS);
    mUserPaddingStart = UNDEFINED_PADDING;
    mUserPaddingEnd = UNDEFINED_PADDING;

    mRenderNode = RenderNode::Create(String("View"), THIS_PROBE(IView));

    if (!sCompatibilityDone && context) {
        AutoPtr<IApplicationInfo> info;
        context->GetApplicationInfo((IApplicationInfo**)&info);
        Int32 targetSdkVersion;
        info->GetTargetSdkVersion(&targetSdkVersion);

        // Older apps may need this compatibility hack for measurement.
        sUseBrokenMakeMeasureSpec = targetSdkVersion <= Build::VERSION_CODES::JELLY_BEAN_MR1;

        // Older apps expect onMeasure() to always be called on a layout pass, regardless
        // of whether a layout was requested on that View.
        sIgnoreMeasureCache = targetSdkVersion < Build::VERSION_CODES::KITKAT;

        sCompatibilityDone = TRUE;
    }

    return NOERROR;
}

ECode View::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(context, attrs, 0);
}

ECode View::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return Init(context, attrs, defStyleAttr);
}


ECode View::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    Init(context);

    //printf("View::Init\n");
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
        const_cast<Int32 *>(R::styleable::View),
        ARRAY_SIZE(R::styleable::View));
    AutoPtr<ITypedArray> a;
    ASSERT_SUCCEEDED(context->ObtainStyledAttributes(
            attrs, attrIds, defStyleAttr, defStyleRes, (ITypedArray**)&a));
    if (mDebugViewAttributes) {
            SaveAttributeData(attrs, a);
        }

    AutoPtr<IDrawable> background;

    Int32 leftPadding = -1;
    Int32 topPadding = -1;
    Int32 rightPadding = -1;
    Int32 bottomPadding = -1;
    Int32 startPadding = UNDEFINED_PADDING;
    Int32 endPadding = UNDEFINED_PADDING;

    Int32 padding = -1;

    Int32 viewFlagValues = 0;
    Int32 viewFlagMasks = 0;

    Boolean setScrollContainer = FALSE;

    Int32 x = 0;
    Int32 y = 0;
    Float tx = 0;
    Float ty = 0;
    Float tz = 0;
    Float elevation = 0;
    Float rotation = 0;
    Float rotationX = 0;
    Float rotationY = 0;
    Float sx = 1.0f;
    Float sy = 1.0f;
    Boolean transformSet = FALSE;

    Int32 scrollbarStyle = IView::SCROLLBARS_INSIDE_OVERLAY;

    Int32 overScrollMode = mOverScrollMode;
    Boolean initializeScrollbars = FALSE;

    Boolean leftPaddingDefined = FALSE;
    Boolean rightPaddingDefined = FALSE;
    Boolean startPaddingDefined = FALSE;
    Boolean endPaddingDefined = FALSE;

    Int32 targetSdkVersion = 0;
    AutoPtr<IApplicationInfo> appInfo;
    context->GetApplicationInfo((IApplicationInfo**)&appInfo);
    assert(appInfo != NULL);
    appInfo->GetTargetSdkVersion(&targetSdkVersion);

    Int32 nTemp = 0;
    Float fTemp = 0.0f;
    Boolean value;
    Int32 N, attr;
    a->GetIndexCount(&N);
    for (Int32 i = 0; i < N; i++) {
        a->GetIndex(i, &attr);
        //printf("View::Init---Index = %d, attr = %d\n", i, attr);
        switch (attr) {
            case R::styleable::View_background:
                background = NULL;
                a->GetDrawable(attr, (IDrawable**)&background);
                //printf("Drawable = 0x%08x\n", (Int32)background.Get());
                break;
            case R::styleable::View_padding:
                a->GetDimensionPixelSize(attr, -1, &padding);
                mUserPaddingLeftInitial = padding;
                mUserPaddingRightInitial = padding;
                leftPaddingDefined = TRUE;
                rightPaddingDefined = TRUE;
                break;
             case R::styleable::View_paddingLeft:
                a->GetDimensionPixelSize(attr, -1, &leftPadding);
                mUserPaddingLeftInitial = leftPadding;
                leftPaddingDefined = TRUE;
                break;
            case R::styleable::View_paddingTop:
                a->GetDimensionPixelSize(attr, -1, &topPadding);
                break;
            case R::styleable::View_paddingRight:
                a->GetDimensionPixelSize(attr, -1, &rightPadding);
                mUserPaddingRightInitial = rightPadding;
                rightPaddingDefined = TRUE;
                break;
            case R::styleable::View_paddingBottom:
                a->GetDimensionPixelSize(attr, -1, &bottomPadding);
                break;
            case R::styleable::View_paddingStart:
                a->GetDimensionPixelSize(attr, UNDEFINED_PADDING, &startPadding);
                startPaddingDefined = (startPadding != UNDEFINED_PADDING);;
                break;
            case R::styleable::View_paddingEnd:
                a->GetDimensionPixelSize(attr, UNDEFINED_PADDING, &endPadding);
                endPaddingDefined = (endPadding != UNDEFINED_PADDING);
                break;
            case R::styleable::View_scrollX:
                a->GetDimensionPixelOffset(attr, 0, &x);
                break;
            case R::styleable::View_scrollY:
                a->GetDimensionPixelOffset(attr, 0, &y);
                break;
            case R::styleable::View_alpha:
                a->GetFloat(attr, 1.0f, &fTemp);
                SetAlpha(fTemp);
                break;
            case R::styleable::View_transformPivotX:
                a->GetDimensionPixelOffset(attr, 0, &nTemp);
                SetPivotX(nTemp);
                break;
            case R::styleable::View_transformPivotY:
                a->GetDimensionPixelOffset(attr, 0, &nTemp);
                SetPivotY(nTemp);
                break;
            case R::styleable::View_translationX:
                a->GetDimensionPixelOffset(attr, 0, &nTemp);
                tx = nTemp;
                transformSet = TRUE;
                break;
            case R::styleable::View_translationY:
                a->GetDimensionPixelOffset(attr, 0, &nTemp);
                ty = nTemp;
                transformSet = TRUE;
                break;
            case R::styleable::View_translationZ:
                a->GetDimensionPixelOffset(attr, 0, &nTemp);
                tz = nTemp;
                transformSet = TRUE;
                break;
            case R::styleable::View_elevation:
                a->GetDimensionPixelOffset(attr, 0, &nTemp);
                elevation = nTemp;
                transformSet = TRUE;
                break;
            case R::styleable::View_rotation:
                a->GetFloat(attr, 0, &rotation);
                transformSet = TRUE;
                break;
            case R::styleable::View_rotationX:
                a->GetFloat(attr, 0, &rotationX);
                transformSet = TRUE;
                break;
            case R::styleable::View_rotationY:
                a->GetFloat(attr, 0, &rotationY);
                transformSet = TRUE;
                break;
            case R::styleable::View_scaleX:
                a->GetFloat(attr, 1.0f, &sx);
                transformSet = TRUE;
                break;
            case R::styleable::View_scaleY:
                a->GetFloat(attr, 1.0f, &sy);
                transformSet = TRUE;
                break;
            case R::styleable::View_id:
                a->GetResourceId(attr, IView::NO_ID, &mID);
                break;
            case R::styleable::View_tag:
//                  mTag = a.getText(attr);
                break;
            case R::styleable::View_fitsSystemWindows:
                a->GetBoolean(attr, FALSE, &value);
                if (value) {
                    viewFlagValues |= FITS_SYSTEM_WINDOWS;
                    viewFlagMasks |= FITS_SYSTEM_WINDOWS;
                }
                break;
            case R::styleable::View_focusable:
                a->GetBoolean(attr, FALSE, &value);
                if (value) {
                    viewFlagValues |= FOCUSABLE;
                    viewFlagMasks |= FOCUSABLE_MASK;
                }
                break;
            case R::styleable::View_focusableInTouchMode:
                a->GetBoolean(attr, FALSE, &value);
                if (value) {
                    viewFlagValues |= FOCUSABLE_IN_TOUCH_MODE | FOCUSABLE;
                    viewFlagMasks |= FOCUSABLE_IN_TOUCH_MODE | FOCUSABLE_MASK;
                }
                break;
            case R::styleable::View_clickable:
                a->GetBoolean(attr, FALSE, &value);
                if (value) {
                    viewFlagValues |= CLICKABLE;
                    viewFlagMasks |= CLICKABLE;
                }
                break;
            case R::styleable::View_longClickable:
                a->GetBoolean(attr, FALSE, &value);
                if (value) {
                    viewFlagValues |= LONG_CLICKABLE;
                    viewFlagMasks |= LONG_CLICKABLE;
                }
                break;
            case R::styleable::View_saveEnabled:
                a->GetBoolean(attr, TRUE, &value);
                if (!value) {
                    viewFlagValues |= SAVE_DISABLED;
                    viewFlagMasks |= SAVE_DISABLED_MASK;
                }
                break;
            case R::styleable::View_duplicateParentState:
                a->GetBoolean(attr, FALSE, &value);
                if (value) {
                    viewFlagValues |= DUPLICATE_PARENT_STATE;
                    viewFlagMasks |= DUPLICATE_PARENT_STATE;
                }
                break;
            case R::styleable::View_visibility:
            {
                Int32 visibility;
                a->GetInt32(attr, 0, &visibility);
                if (visibility != 0) {
                    viewFlagValues |= VISIBILITY_FLAGS[visibility];
                    viewFlagMasks |= VISIBILITY_MASK;
                }
                break;
            }
            case R::styleable::View_layoutDirection:
            {
                // Clear any layout direction flags (included resolved bits) already set
                mPrivateFlags2 &=
                        ~(PFLAG2_LAYOUT_DIRECTION_MASK | PFLAG2_LAYOUT_DIRECTION_RESOLVED_MASK);
                // Set the layout direction flags depending on the value of the attribute
                Int32 layoutDirection = 0;
                a->GetInt32(attr, -1, &layoutDirection);
                Int32 value = (layoutDirection != -1) ?
                        LAYOUT_DIRECTION_FLAGS[layoutDirection] : LAYOUT_DIRECTION_DEFAULT;
                mPrivateFlags2 |= (value << PFLAG2_LAYOUT_DIRECTION_MASK_SHIFT);
                break;
            }
            case R::styleable::View_drawingCacheQuality:
            {
                Int32 cacheQuality;
                a->GetInt32(attr, 0, &cacheQuality);
                if (cacheQuality != 0) {
                    viewFlagValues |= DRAWING_CACHE_QUALITY_FLAGS[cacheQuality];
                    viewFlagMasks |= DRAWING_CACHE_QUALITY_MASK;
                }
                break;
            }
            case R::styleable::View_contentDescription:
            {
                String str;
                a->GetString(attr, &str);
                AutoPtr<ICharSequence> cs;
                CString::New(str, (ICharSequence**)&cs);
                SetContentDescription(cs);
                break;
            }
            case R::styleable::View_labelFor:
            {
                Int32 resId = 0;
                a->GetResourceId(attr, IView::NO_ID, &resId);
                SetLabelFor(resId);
                break;
            }
            case R::styleable::View_soundEffectsEnabled:
                a->GetBoolean(attr, TRUE, &value);
                if (!value) {
                    viewFlagValues &= ~IView::SOUND_EFFECTS_ENABLED;
                    viewFlagMasks |= IView::SOUND_EFFECTS_ENABLED;
                }
                break;
            case R::styleable::View_hapticFeedbackEnabled:
                a->GetBoolean(attr, TRUE, &value);
                if (!value) {
                    viewFlagValues &= ~IView::HAPTIC_FEEDBACK_ENABLED;
                    viewFlagMasks |= IView::HAPTIC_FEEDBACK_ENABLED;
                }
                break;
            case R::styleable::View_scrollbars:
            {
                Int32 scrollbars;
                a->GetInt32(attr, SCROLLBARS_NONE, &scrollbars);
                if (scrollbars != SCROLLBARS_NONE) {
                    viewFlagValues |= scrollbars;
                    viewFlagMasks |= SCROLLBARS_MASK;
                    initializeScrollbars = TRUE;
                }
                break;
            }
            //noinspection deprecation
            case R::styleable::View_fadingEdge:
                if (targetSdkVersion >= Build::VERSION_CODES::ICE_CREAM_SANDWICH) {
                    // Ignore the attribute starting with ICS
                    break;
                }
                // With builds < ICS, fall through and apply fading edges
            case R::styleable::View_requiresFadingEdge:
            {
                Int32 fadingEdge;
                a->GetInt32(attr, FADING_EDGE_NONE, &fadingEdge);
                if (fadingEdge != FADING_EDGE_NONE) {
                    viewFlagValues |= fadingEdge;
                    viewFlagMasks |= FADING_EDGE_MASK;
                    InitializeFadingEdgeInternal(a);
                }
                break;
            }
            case R::styleable::View_scrollbarStyle:
                a->GetInt32(attr, IView::SCROLLBARS_INSIDE_OVERLAY, &scrollbarStyle);
                if (scrollbarStyle != IView::SCROLLBARS_INSIDE_OVERLAY) {
                    viewFlagValues |= scrollbarStyle & SCROLLBARS_STYLE_MASK;
                    viewFlagMasks |= SCROLLBARS_STYLE_MASK;
                }
                break;
            case R::styleable::View_isScrollContainer:
                setScrollContainer = TRUE;
                a->GetBoolean(attr, FALSE, &value);
                if (value) {
                    SetScrollContainer(TRUE);
                }
                break;
            case R::styleable::View_keepScreenOn:
                a->GetBoolean(attr, FALSE, &value);
                if (value) {
                    viewFlagValues |= IView::KEEP_SCREEN_ON;
                    viewFlagMasks |= IView::KEEP_SCREEN_ON;
                }
                break;
            case R::styleable::View_filterTouchesWhenObscured:
                a->GetBoolean(attr, FALSE, &value);
                if (value) {
                    viewFlagValues |= FILTER_TOUCHES_WHEN_OBSCURED;
                    viewFlagMasks |= FILTER_TOUCHES_WHEN_OBSCURED;
                }
                break;
            case R::styleable::View_nextFocusLeft:
                a->GetResourceId(attr, IView::NO_ID, &mNextFocusLeftId);
                break;
            case R::styleable::View_nextFocusRight:
                a->GetResourceId(attr, IView::NO_ID, &mNextFocusRightId);
                break;
            case R::styleable::View_nextFocusUp:
                a->GetResourceId(attr, IView::NO_ID, &mNextFocusUpId);
                break;
            case R::styleable::View_nextFocusDown:
                a->GetResourceId(attr, IView::NO_ID, &mNextFocusDownId);
                break;
            case R::styleable::View_nextFocusForward:
                a->GetResourceId(attr, IView::NO_ID, &mNextFocusForwardId);
                break;
            case R::styleable::View_minWidth:
                a->GetDimensionPixelSize(attr, 0, &mMinWidth);
                break;
            case R::styleable::View_minHeight:
                a->GetDimensionPixelSize(attr, 0, &mMinHeight);
                break;
            case R::styleable::View_onClick:
//                if (context.isRestricted()) {
//                    throw new IllegalStateException("The android:onClick attribute cannot "
//                            + "be used within a restricted context");
//                }
//
//                final String handlerName = a.getString(attr);
//                if (handlerName != NULL) {
//                    setOnClickListener(new OnClickListener() {
//                        private Method mHandler;
//
//                        public void onClick(View v) {
//                            if (mHandler == NULL) {
//                                try {
//                                    mHandler = getContext().getClass().getMethod(handlerName,
//                                            View.class);
//                                } catch (NoSuchMethodException e) {
//                                    int id = getId();
//                                    String idText = id == IView::NO_ID ? "" : " with id '"
//                                            + getContext().getResources().getResourceEntryName(
//                                                id) + "'";
//                                    throw new IllegalStateException("Could not find a method " +
//                                            handlerName + "(View) in the activity "
//                                            + getContext().getClass() + " for onClick handler"
//                                            + " on view " + View.this.getClass() + idText, e);
//                                }
//                            }
//
//                             try {
//                                 mHandler.invoke(getContext(), View.this);
//                             } catch (IllegalAccessException e) {
//                                 throw new IllegalStateException("Could not execute non "
//                                         + "public method of the activity", e);
//                             } catch (InvocationTargetException e) {
//                                 throw new IllegalStateException("Could not execute "
//                                         + "method of the activity", e);
//                             }
//                         }
//                     });
//                }
                break;
            case R::styleable::View_overScrollMode:
                a->GetInt32(attr, IView::OVER_SCROLL_IF_CONTENT_SCROLLS, &overScrollMode);
                break;
            case R::styleable::View_verticalScrollbarPosition:
                a->GetInt32(attr, IView::SCROLLBAR_POSITION_DEFAULT, &mVerticalScrollbarPosition);
                break;
            case R::styleable::View_layerType:
                a->GetInt32(attr, IView::LAYER_TYPE_NONE, &nTemp);
                SetLayerType(nTemp, NULL);
                break;
            case R::styleable::View_textDirection:
            {
                // Clear any text direction flag already set
                mPrivateFlags2 &= ~PFLAG2_TEXT_DIRECTION_MASK;
                // Set the text direction flags depending on the value of the attribute
                Int32 textDirection = 0;
                a->GetInt32(attr, -1, &textDirection);
                if (textDirection != -1) {
                    mPrivateFlags2 |= PFLAG2_TEXT_DIRECTION_FLAGS[textDirection];
                }
                break;
            }
            case R::styleable::View_textAlignment:
            {
                // Clear any text alignment flag already set
                mPrivateFlags2 &= ~PFLAG2_TEXT_ALIGNMENT_MASK;
                // Set the text alignment flag depending on the value of the attribute
                Int32 textAlignment = 0;
                a->GetInt32(attr, TEXT_ALIGNMENT_DEFAULT, &textAlignment);
                mPrivateFlags2 |= PFLAG2_TEXT_ALIGNMENT_FLAGS[textAlignment];
                break;
            }
            case R::styleable::View_importantForAccessibility:
                a->GetInt32(attr, IMPORTANT_FOR_ACCESSIBILITY_DEFAULT, &nTemp);
                SetImportantForAccessibility(nTemp);
                break;
            case R::styleable::View_accessibilityLiveRegion:
                a->GetInt32(attr, ACCESSIBILITY_LIVE_REGION_DEFAULT, &nTemp);
                SetAccessibilityLiveRegion(nTemp);
                break;
            case R::styleable::View_transitionName:
            {
                String str;
                a->GetString(attr, &str);
                SetTransitionName(str);
                break;
            }
            case R::styleable::View_nestedScrollingEnabled:
            {
                Boolean temp;
                a->GetBoolean(attr, FALSE, &temp);
                SetNestedScrollingEnabled(temp);
                break;
            }
            case R::styleable::View_stateListAnimator:
            {
                Int32 temp;
                a->GetResourceId(attr, 0, &temp);
                AutoPtr<IStateListAnimator> animator;
                AnimatorInflater::LoadStateListAnimator(context, temp, (IStateListAnimator**)&animator);
                SetStateListAnimator(animator);
                break;
            }
            case R::styleable::View_backgroundTint:
            {
                // This will get applied later during setBackground().
                if (!mBackgroundTint) {
                    mBackgroundTint = new TintInfo();
                }
                a->GetColorStateList(R::styleable::View_backgroundTint, (IColorStateList**)&(mBackgroundTint->mTintList));
                mBackgroundTint->mHasTintList = TRUE;
                break;
            }
            case R::styleable::View_backgroundTintMode:
            {
                // This will get applied later during setBackground().
                if (!mBackgroundTint) {
                    mBackgroundTint = new TintInfo();
                }
                Int32 temp;
                a->GetInt32(R::styleable::View_backgroundTintMode, -1, &temp);
                mBackgroundTint->mTintMode = Drawable::RarseTintMode(temp, NULL);
                mBackgroundTint->mHasTintMode = TRUE;
                break;
            }
            case R::styleable::View_outlineProvider:
                a->GetInt32(R::styleable::View_outlineProvider, PROVIDER_BACKGROUND, &nTemp);
                SetOutlineProviderFromAttribute(nTemp);
                break;
            default:
                break;
        }
    }

    SetOverScrollMode(overScrollMode);
    // Cache start/end user padding as we cannot fully resolve padding here (we dont have yet
    // the resolved layout direction). Those cached values will be used later during padding
    // resolution.
    mUserPaddingStart = startPadding;
    mUserPaddingEnd = endPadding;

    if (background != NULL) {
        SetBackground(background);
    }

    // setBackground above will record that padding is currently provided by the background.
    // If we have padding specified via xml, record that here instead and use it.
    mLeftPaddingDefined = leftPaddingDefined;
    mRightPaddingDefined = rightPaddingDefined;

    if (padding >= 0) {
        leftPadding = padding;
        topPadding = padding;
        rightPadding = padding;
        bottomPadding = padding;
        mUserPaddingLeftInitial = padding;
        mUserPaddingRightInitial = padding;
    }

    if (IsRtlCompatibilityMode()) {
        // RTL compatibility mode: pre Jelly Bean MR1 case OR no RTL support case.
        // left / right padding are used if defined (meaning here nothing to do). If they are not
        // defined and start / end padding are defined (e.g. in Frameworks resources), then we use
        // start / end and resolve them as left / right (layout direction is not taken into account).
        // Padding from the background drawable is stored at this point in mUserPaddingLeftInitial
        // and mUserPaddingRightInitial) so drawable padding will be used as ultimate default if
        // defined.
        if (!mLeftPaddingDefined && startPaddingDefined) {
            leftPadding = startPadding;
        }

        mUserPaddingLeftInitial = (leftPadding >= 0) ? leftPadding : mUserPaddingLeftInitial;
        if (!mRightPaddingDefined && endPaddingDefined) {
            rightPadding = endPadding;
        }
        mUserPaddingRightInitial = (rightPadding >= 0) ? rightPadding : mUserPaddingRightInitial;
    }
    else {
        // Jelly Bean MR1 and after case: if start/end defined, they will override any left/right
        // values defined. Otherwise, left /right values are used.
        // Padding from the background drawable is stored at this point in mUserPaddingLeftInitial
        // and mUserPaddingRightInitial) so drawable padding will be used as ultimate default if
        // defined.

        Boolean hasRelativePadding = startPaddingDefined || endPaddingDefined;
        if (mLeftPaddingDefined && !hasRelativePadding) {
            mUserPaddingLeftInitial = leftPadding;
        }
        if (mRightPaddingDefined && !hasRelativePadding) {
            mUserPaddingRightInitial = rightPadding;
        }
    }

    InternalSetPadding(
            mUserPaddingLeftInitial,
            topPadding >= 0 ? topPadding : mPaddingTop,
            mUserPaddingRightInitial,
            bottomPadding >= 0 ? bottomPadding : mPaddingBottom);

    if (viewFlagMasks != 0) {
        SetFlags(viewFlagValues, viewFlagMasks);
    }

    if (initializeScrollbars) {
        InitializeScrollbarsInternal(a);
    }

    a->Recycle();

    // Needs to be called after mViewFlags is set
    if (scrollbarStyle != IView::SCROLLBARS_INSIDE_OVERLAY) {
        RecomputePadding();
    }

    if (x != 0 || y != 0) {
        ScrollTo(x, y);
    }

    if (transformSet) {
        SetTranslationX(tx);
        SetTranslationY(ty);
        SetTranslationZ(tz);
        SetElevation(elevation);
        SetRotation(rotation);
        SetRotationX(rotationX);
        SetRotationY(rotationY);
        SetScaleX(sx);
        SetScaleY(sy);
    }

    if (!setScrollContainer && (viewFlagValues&SCROLLBARS_VERTICAL) != 0) {
        SetScrollContainer(TRUE);
    }

    ComputeOpaqueFlags();

    //printf("View::Init----END 0x%08x, ID = 0x%08x\n", (Int32)this->Probe(EIID_IView), mID);
    return NOERROR;
}

HashMap<Int32, String > View::GetAttributeMap()
{
    return mAttributeMap;
}

void View::SaveAttributeData(
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ ITypedArray* a)
{
    Int32 length = 0;
    if (attrs) {
        attrs->GetAttributeCount(&length);
        Int32 temp;
        a->GetIndexCount(&temp);
        length += temp * 2;
    }
    mAttributes = ArrayOf<String>::Alloc(length);

    Int32 i = 0;
    if (attrs) {
        Int32 count = 0;
        attrs->GetAttributeCount(&count);
        for (i = 0; i < count; i += 2) {
            String str;
            attrs->GetAttributeName(i, &str);
            mAttributes[i] = str;
            attrs->GetAttributeValue(i, &str);
            mAttributes[i + 1] = str;
        }

    }
    Int32 ITypeLen;
    a->GetLength(&ITypeLen);
    for (Int32 j = 0; j < ITypeLen; ++j) {
        Boolean value;
        if (a->HasValue(j, &value), value) {
            //try {
            Int32 resourceId;
            a->GetResourceId(j, 0, &resourceId);
            if (resourceId == 0) {
                continue;
            }

            HashMap<Int32, String>::Iterator it = mAttributeMap.Find(resourceId);
            if (it != mAttributeMap.End() & it->mSecond != NULL) {
                String resourceName = it->mSecond;
            }
            if (resourceName.IsNullOrEmpty()) {
                AutoPtr<IResources> res;
                a->GetResources((IResources**)&res);
                res->GetResourceName(resourceId, &resourceName);
                mAttributeMap.Insert(HashMap<Int32, String>::ValueType(resourceId, resourceName));
            }

            mAttributes[i] = resourceName;
            AutoPtr<ICharSequence> text;
            a->GetText((ICharSequence**)&text);
            text->ToString((*mAttributes)[i+1]);
            i += 2;
            //} catch (Resources.NotFoundException e) {
                // if we can't get the resource name, we just ignore it
            //}
        }
    }
}

ECode View::HandleInvalidate()
{
    return Invalidate();
}

ECode View::HandleInvalidateRect(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    return Invalidate(left, top, right, bottom);
}

void View::Log()
{
#ifdef _DEBUG
    String name;
    AutoPtr<IResources> res = GetResources();
    if (res) res->GetResourceEntryName(mID, &name);
    Logger::D("View", "type = %s, id = 0x%08x, this = %p, view = %p",
        name.string(), mID, this, THIS_PROBE(IView));
#endif
}

ECode View::GetInflaterContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context);
    *context = mInflaterContext;
    REFCOUNT_ADD(*context);
    return NOERROR;
}

ECode View::SetInflaterContext(
    /* [in] */ IContext* context)
{
    mInflaterContext = context;
    return NOERROR;
}

ECode View::GetXmlPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = mXmlPath;
    return NOERROR;
}

ECode View::SetXmlPath(
    /* [in] */ const String& path)
{
    mXmlPath = path;
    return NOERROR;
}

Boolean View::InLiveRegion()
{
    Int32 region;
    GetAccessibilityLiveRegion(&region);
    if (region != IView::ACCESSIBILITY_LIVE_REGION_NONE) {
        return TRUE;
    }

    AutoPtr<IViewParent> parent;
    GetParent((IViewParent**)&parent);
    while (IView::Probe(parent)) {
        (IView::Probe(parent))->GetAccessibilityLiveRegion(&region);
        if (region != IView::ACCESSIBILITY_LIVE_REGION_NONE) {
            return TRUE;
        }
        (IView::Probe(parent))->GetParent((IViewParent**)&parent);
    }

    return FALSE;
}

/////////////////////////////////////////////////////////
//              View::AlpahFloatProperty
/////////////////////////////////////////////////////////

View::AlpahFloatProperty::AlpahFloatProperty()
    : FloatProperty(String("alpha"))
{}

ECode View::AlpahFloatProperty::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Float value)
{
    (IView::Probe(obj))->SetAlpha(value);
    return NOERROR;
}

ECode View::AlpahFloatProperty::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    VALIDATE_NOT_NULL(rst)
    Float alpha;
    (IView::Probe(obj))->GetAlpha(&alpha);
    AutoPtr<IFloat> temp;
    CFloat::New(alpha, (IFloat**)&temp);
    *rst = (IInterface*)temp->Probe(EIID_IInterface);
    REFCOUNT_ADD(*rst);
    return NOERROR;
}

/////////////////////////////////////////////////////////
//              View::TranslationXFloatProperty
/////////////////////////////////////////////////////////

View::TranslationXFloatProperty::TranslationXFloatProperty()
    : FloatProperty(String("translationX"))
{}

ECode View::TranslationXFloatProperty::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Float value)
{
    (IView::Probe(obj))->SetTranslationX(value);
    return NOERROR;
}

ECode View::TranslationXFloatProperty::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    VALIDATE_NOT_NULL(rst)
    Float translationX;
    (IView::Probe(obj))->GetTranslationX(&translationX);
    AutoPtr<IFloat> temp;
    CFloat::New(translationX, (IFloat**)&temp);
    *rst = (IInterface*)temp->Probe(EIID_IInterface);
    REFCOUNT_ADD(*rst);
    return NOERROR;
}

/////////////////////////////////////////////////////////
//              View::TranslationYFloatProperty
/////////////////////////////////////////////////////////

View::TranslationYFloatProperty::TranslationYFloatProperty()
    : FloatProperty(String("translationY"))
{}

ECode View::TranslationYFloatProperty::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Float value)
{
    (IView::Probe(obj))->SetTranslationY(value);
    return NOERROR;
}

ECode View::TranslationYFloatProperty::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    VALIDATE_NOT_NULL(rst)
    Float translationY;
    (IView::Probe(obj))->GetTranslationY(&translationY);
    AutoPtr<IFloat> temp;
    CFloat::New(translationY, (IFloat**)&temp);
    *rst = (IInterface*)temp->Probe(EIID_IInterface);
    REFCOUNT_ADD(*rst);
    return NOERROR;
}

/////////////////////////////////////////////////////////
//              View::TranslationZFloatProperty
/////////////////////////////////////////////////////////

View::TranslationZFloatProperty::TranslationZFloatProperty()
    : FloatProperty(String("translationZ"))
{}

ECode View::TranslationZFloatProperty::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Float value)
{
    (IView::Probe(obj))->SetTranslationZ(value);
    return NOERROR;
}

ECode View::TranslationZFloatProperty::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    VALIDATE_NOT_NULL(rst)
    Float translationZ;
    (IView::Probe(obj))->GetTranslationZ(&translationZ);
    AutoPtr<IFloat> temp;
    CFloat::New(translationZ, (IFloat**)&temp);
    *rst = (IInterface*)temp->Probe(EIID_IInterface);
    REFCOUNT_ADD(*rst);
    return NOERROR;
}

/////////////////////////////////////////////////////////
//              View::XFloatProperty
/////////////////////////////////////////////////////////

View::XFloatProperty::XFloatProperty()
    : FloatProperty(String("x"))
{}

ECode View::XFloatProperty::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Float value)
{
    (IView::Probe(obj))->SetX(value);
    return NOERROR;
}

ECode View::XFloatProperty::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    VALIDATE_NOT_NULL(rst)
    Float x;
    (IView::Probe(obj))->GetX(&x);
    AutoPtr<IFloat> temp;
    CFloat::New(x, (IFloat**)&temp);
    *rst = (IInterface*)temp->Probe(EIID_IInterface);
    REFCOUNT_ADD(*rst);
    return NOERROR;
}

/////////////////////////////////////////////////////////
//              View::YFloatProperty
/////////////////////////////////////////////////////////

View::YFloatProperty::YFloatProperty()
    : FloatProperty(String("y"))
{}

ECode View::YFloatProperty::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Float value)
{
    (IView::Probe(obj))->SetY(value);
    return NOERROR;
}

ECode View::YFloatProperty::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    VALIDATE_NOT_NULL(rst)
    Float y;
    (IView::Probe(obj))->GetY(&y);
    AutoPtr<IFloat> temp;
    CFloat::New(y, (IFloat**)&temp);
    *rst = (IInterface*)temp->Probe(EIID_IInterface);
    REFCOUNT_ADD(*rst);
    return NOERROR;
}

/////////////////////////////////////////////////////////
//              View::ZFloatProperty
/////////////////////////////////////////////////////////

View::ZFloatProperty::ZFloatProperty()
    : FloatProperty(String("z"))
{}

ECode View::ZFloatProperty::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Float value)
{
    (IView::Probe(obj))->SetZ(value);
    return NOERROR;
}

ECode View::ZFloatProperty::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    VALIDATE_NOT_NULL(rst)
    Float z;
    (IView::Probe(obj))->GetZ(&z);
    AutoPtr<IFloat> temp;
    CFloat::New(z, (IFloat**)&temp);
    *rst = (IInterface*)temp->Probe(EIID_IInterface);
    REFCOUNT_ADD(*rst);
    return NOERROR;
}

/////////////////////////////////////////////////////////
//              View::RotationFloatProperty
/////////////////////////////////////////////////////////

View::RotationFloatProperty::RotationFloatProperty()
    : FloatProperty(String("rotation"))
{}

ECode View::RotationFloatProperty::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Float value)
{
    (IView::Probe(obj))->SetRotation(value);
    return NOERROR;
}

ECode View::RotationFloatProperty::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    VALIDATE_NOT_NULL(rst)
    Float rotation;
    (IView::Probe(obj))->GetRotation(&rotation);
    AutoPtr<IFloat> temp;
    CFloat::New(rotation, (IFloat**)&temp);
    *rst = (IInterface*)temp->Probe(EIID_IInterface);
    REFCOUNT_ADD(*rst);
    return NOERROR;
}

/////////////////////////////////////////////////////////
//              View::RotationXFloatProperty
/////////////////////////////////////////////////////////

View::RotationXFloatProperty::RotationXFloatProperty()
    : FloatProperty(String("rotationX"))
{}

ECode View::RotationXFloatProperty::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Float value)
{
    (IView::Probe(obj))->SetRotationX(value);
    return NOERROR;
}

ECode View::RotationXFloatProperty::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    VALIDATE_NOT_NULL(rst)
    Float rotation;
    (IView::Probe(obj))->GetRotationX(&rotation);
    AutoPtr<IFloat> temp;
    CFloat::New(rotation, (IFloat**)&temp);
    *rst = (IInterface*)temp->Probe(EIID_IInterface);
    REFCOUNT_ADD(*rst);
    return NOERROR;
}

/////////////////////////////////////////////////////////
//              View::RotationYFloatProperty
/////////////////////////////////////////////////////////

View::RotationYFloatProperty::RotationYFloatProperty()
    : FloatProperty(String("rotationY"))
{}

ECode View::RotationYFloatProperty::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Float value)
{
    (IView::Probe(obj))->SetRotationY(value);
    return NOERROR;
}

ECode View::RotationYFloatProperty::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    VALIDATE_NOT_NULL(rst)
    Float rotation;
    (IView::Probe(obj))->GetRotationY(&rotation);
    AutoPtr<IFloat> temp;
    CFloat::New(rotation, (IFloat**)&temp);
    *rst = (IInterface*)temp->Probe(EIID_IInterface);
    REFCOUNT_ADD(*rst);
    return NOERROR;
}

/////////////////////////////////////////////////////////
//              View::ScaleXFloatProperty
/////////////////////////////////////////////////////////

View::ScaleXFloatProperty::ScaleXFloatProperty()
    : FloatProperty(String("scaleX"))
{}

ECode View::ScaleXFloatProperty::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Float value)
{
    (IView::Probe(obj))->SetScaleX(value);
    return NOERROR;
}

ECode View::ScaleXFloatProperty::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    VALIDATE_NOT_NULL(rst)
    Float rotation;
    (IView::Probe(obj))->GetScaleX(&rotation);
    AutoPtr<IFloat> temp;
    CFloat::New(rotation, (IFloat**)&temp);
    *rst = (IInterface*)temp->Probe(EIID_IInterface);
    REFCOUNT_ADD(*rst);
    return NOERROR;
}

/////////////////////////////////////////////////////////
//              View::ScaleYFloatProperty
/////////////////////////////////////////////////////////

View::ScaleYFloatProperty::ScaleYFloatProperty()
    : FloatProperty(String("scaleY"))
{}

ECode View::ScaleYFloatProperty::SetValue(
    /* [in] */ IInterface* obj,
    /* [in] */ Float value)
{
    (IView::Probe(obj))->SetScaleY(value);
    return NOERROR;
}

ECode View::ScaleYFloatProperty::Get(
    /* [in] */ IInterface* obj,
    /* [out] */ IInterface** rst)
{
    VALIDATE_NOT_NULL(rst)
    Float rotation;
    (IView::Probe(obj))->GetScaleY(&rotation);
    AutoPtr<IFloat> temp;
    CFloat::New(rotation, (IFloat**)&temp);
    *rst = (IInterface*)temp->Probe(EIID_IInterface);
    REFCOUNT_ADD(*rst);
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
