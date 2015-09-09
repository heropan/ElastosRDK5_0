
#include "Context.h"

namespace Elastos {
namespace Droid {
namespace Content {


    CARAPI RegisterComponentCallbacks(
        /* [in] */ IComponentCallbacks* componentCallback)
    {
        AutoPtr<IContext> context;
        FAIL_RETURN(GetApplicationContext((IContext**) &context));
        return context->RegisterComponentCallbacks(componentCallback);
    }

    CARAPI UnregisterComponentCallbacks(
        /* [in] */ IComponentCallbacks* componentCallback)
    {
        AutoPtr<IContext> context;
        FAIL_RETURN(GetApplicationContext((IContext**) &context));
        return context->UnregisterComponentCallbacks(componentCallback);
    }

    CARAPI GetText(
        /* [in] */ Int32 resId,
        /* [out] */ ICharSequence** text);

    CARAPI GetString(
        /* [in] */ Int32 resId,
        /* [out] */ String* str)
    {
        VALIDATE_NOT_NULL(str)
        *str = String(NULL);
        AutoPtr<IResources> resources;
        FAIL_RETURN(GetResources((IResources**) &resources));
        return resources->GetString(resId, str);
    }

    CARAPI GetString(
        /* [in] */ Int32 resId,
        /* [in] */ ArrayOf<IInterface*>* formatArgs,
        /* [out] */ String* str)
    {
        VALIDATE_NOT_NULL(str)
        *str = String(NULL);
        AutoPtr<IResources> resources;
        FAIL_RETURN(GetResources((IResources**) &resources));
//***        return resources->GetString(resId, formatArgs, str);
        return E_NOT_IMPLEMENTED;
    }

    CARAPI GetDrawable(
        /* [in] */ Int32 id,
        /* [out] */ IDrawable** drawable)
    {
        return getResources().getDrawable(id, getTheme());
    }

    CARAPI GetThemeResId(
        /* [out] */ Int32* resId)
    {
        VALIDATE_NOT_NULL(resId)
        *resId = 0;
        return NOERROR;
    }

    CARAPI ObtainStyledAttributes(
        /* [in] */ ArrayOf<Int32>* attrs,
        /* [out] */ ITypedArray** styles)
    {
        VALIDATE_NOT_NULL(styles)
        *styles = NULL;
        AutoPtr<IResourcesTheme> theme;
        FAIL_RETURN(GetTheme((IResourcesTheme**) &theme));
        return theme->ObtainStyledAttributes(attrs, styles);
    }

    CARAPI ObtainStyledAttributes(
        /* [in] */ Int32 resid,
        /* [in] */ ArrayOf<Int32>* attrs,
        /* [out] */ ITypedArray** styles)
    {
        VALIDATE_NOT_NULL(styles)
        *styles = NULL;
        AutoPtr<IResourcesTheme> theme;
        FAIL_RETURN(GetTheme((IResourcesTheme**) &theme));
        return theme->ObtainStyledAttributes(resid, attrs, styles);
    }

    CARAPI ObtainStyledAttributes(
        /* [in] */ IAttributeSet* set,
        /* [in] */ ArrayOf<Int32>* attrs,
        /* [out] */ ITypedArray** styles)
    {
        VALIDATE_NOT_NULL(styles)
        *styles = NULL;
        AutoPtr<IResourcesTheme> theme;
        FAIL_RETURN(GetTheme((IResourcesTheme**) &theme));
        return theme->ObtainStyledAttributes(set, attrs, 0, 0, styles);
    }

    CARAPI ObtainStyledAttributes(
        /* [in] */ IAttributeSet* set,
        /* [in] */ ArrayOf<Int32>* attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes,
        /* [out] */ ITypedArray** styles)
    {
        VALIDATE_NOT_NULL(styles)
        *styles = NULL;
        AutoPtr<IResourcesTheme> theme;
        FAIL_RETURN(GetTheme((IResourcesTheme**) &theme));
        return theme->ObtainStyledAttributes(set, attrs, defStyleAttr, defStyleRes, styles);
    }

    CARAPI StartActivityAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user)
    {
        //Not implemented. Must override in a subclass.
        return E_RUNTIME_EXCEPTION;
    }

    CARAPI StartActivityAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IBundle* options,
        /* [in] */ IUserHandle* userId)
    {
        //Not implemented. Must override in a subclass.
        return E_RUNTIME_EXCEPTION;
    }

    CARAPI IsRestricted(
        /* [out] */ Boolean* isRestricted)
    {
        VALIDATE_NOT_NULL(isRestricted)
        *isRestricted = FALSE;
        return NOERROR;
    }

} //Content
} //Droid
} //Elastos
