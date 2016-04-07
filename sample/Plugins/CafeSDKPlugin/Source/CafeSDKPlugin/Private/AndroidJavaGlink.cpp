#include "CafeSDKPluginPrivatePCH.h"
#include "AndroidJavaGlink.h"

#if PLATFORM_ANDROID

FAndroidJavaGlink::FAndroidJavaGlink()
{
    Class = FAndroidApplication::FindJavaClass(GetClassName().GetPlainANSIString());
    
    InitMethod = GetClassStaticMethod("init", "(Ljava/lang/String;Ljava/lang/String;I)V");
    StartHomeMethod = GetClassStaticMethod("startHome", "(Landroid/app/Activity;)V");
    StartNoticeMethod = GetClassStaticMethod("startNotice", "(Landroid/app/Activity;)V");
    StartEventMethod = GetClassStaticMethod("startEvent", "(Landroid/app/Activity;)V");
    StartMenuMethod = GetClassStaticMethod("startMenu", "(Landroid/app/Activity;)V");
    StartMenuByIdMethod = GetClassStaticMethod("startMenu", "(Landroid/app/Activity;I)V");
    StartProfileMethod = GetClassStaticMethod("startProfile", "(Landroid/app/Activity;)V");
    IsShowMethod = GetClassStaticMethod("isShowGlink", "(Landroid/app/Activity;)Z");
    SyncGameUserIdMethod = GetClassStaticMethod("syncGameUserId", "(Landroid/app/Activity;Ljava/lang/String;)V");
}

void FAndroidJavaGlink::Init(FString ClientId, FString ClientSecret, int32 CafeId) const
{
    JNIEnv* JEnv = FAndroidApplication::GetJavaEnv();
    JEnv->CallStaticVoidMethod(Class, InitMethod.Method, FJavaClassObject::GetJString(ClientId), FJavaClassObject::GetJString(ClientSecret), CafeId);
}

void FAndroidJavaGlink::StartHome() const
{
    StartTab(StartHomeMethod);
}

void FAndroidJavaGlink::StartNotice() const
{
    StartTab(StartNoticeMethod);
}

void FAndroidJavaGlink::StartEvent() const
{
    StartTab(StartEventMethod);
}

void FAndroidJavaGlink::StartMenu() const
{
    StartTab(StartMenuMethod);
}

void FAndroidJavaGlink::StartMenuById(int32 MenuId) const
{
    JNIEnv* JEnv = FAndroidApplication::GetJavaEnv();
    JEnv->CallStaticVoidMethod(Class, StartMenuByIdMethod.Method, FJavaWrapper::GameActivityThis, MenuId);
}

void FAndroidJavaGlink::StartProfile() const
{
    StartTab(StartProfileMethod);
}

void FAndroidJavaGlink::StartTab(const FJavaClassMethod& JavaClassMethod) const
{
    JNIEnv* JEnv = FAndroidApplication::GetJavaEnv();
    JEnv->CallStaticVoidMethod(Class, JavaClassMethod.Method, FJavaWrapper::GameActivityThis);
}

bool FAndroidJavaGlink::IsShow() const
{
    JNIEnv* JEnv = FAndroidApplication::GetJavaEnv();
    return JEnv->CallStaticBooleanMethod(Class, IsShowMethod.Method, FJavaWrapper::GameActivityThis);
}

void FAndroidJavaGlink::SyncGameUserId(FString GameUserId) const
{
    JNIEnv* JEnv = FAndroidApplication::GetJavaEnv();
    JEnv->CallStaticVoidMethod(Class, SyncGameUserIdMethod.Method, FJavaWrapper::GameActivityThis, FJavaClassObject::GetJString(GameUserId));
}

FJavaClassMethod FAndroidJavaGlink::GetClassStaticMethod(const char* MethodName, const char* FuncSig) const
{
    JNIEnv* JEnv = FAndroidApplication::GetJavaEnv();
    FJavaClassMethod Method;
    Method.Method = JEnv->GetStaticMethodID(Class, MethodName, FuncSig);
    Method.Name = MethodName;
    Method.Signature = FuncSig;
    // Is method valid?
    checkf(Method.Method, TEXT("Unable to find Java Method %s with Signature %s"), UTF8_TO_TCHAR(MethodName), UTF8_TO_TCHAR(FuncSig));
    return Method;
}

FName FAndroidJavaGlink::GetClassName()
{
    if (FAndroidMisc::GetAndroidBuildVersion() >= 1)
    {
        return FName("com.naver.glink.android.sdk.Glink");
    }
    else
    {
        return FName("");
    }
}

#endif // PLATFORM_ANDROID
