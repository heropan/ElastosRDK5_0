
#ifndef __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_INPUT_DATETIMESUGGESTIONLISTADAPTER_H__
#define __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_INPUT_DATETIMESUGGESTIONLISTADAPTER_H__

// import android.content.Context;
// import android.view.LayoutInflater;
// import android.view.View;
// import android.view.ViewGroup;
// import android.widget.ArrayAdapter;
// import android.widget.TextView;

// import org.chromium.content.R;

// import java.util.List;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

/**
 * Date/time suggestion adapter for the suggestion dialog.
 */
class DateTimeSuggestionListAdapter
    : public Object
    , public IArrayAdapter
{
public:
    DateTimeSuggestionListAdapter(
        /* [in] */ IContext* context,
        /* [in] */ List<DateTimeSuggestion> objects);

    //@Override
    CARAPI GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** outView)

    //@Override
    CARAPI GetCount(
        /* [out] */ Int32* count);

private:
    const AutoPtr<IContext> mContext;
};

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_INPUT_DATETIMESUGGESTIONLISTADAPTER_H__
