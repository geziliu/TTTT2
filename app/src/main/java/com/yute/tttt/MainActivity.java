package com.yute.tttt;
import me.weishu.reflection.Reflection;
import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import java.lang.reflect.Method;

public class MainActivity extends AppCompatActivity {
    static {
        System.loadLibrary("native-lib");
    }
    public native String CountMD5(String str);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Log.d("MainActivity", CountMD5("test"));

/*        try {
            Method forName = Class.class.getDeclaredMethod("forName", String.class);
            Method getDeclaredMethod = Class.class.getDeclaredMethod("getDeclaredMethod", String.class, Class[].class);
            Class<?> vmRuntimeClass = (Class<?>) forName.invoke(null, "dalvik.system.VMRuntime");
            Method getRuntime = (Method) getDeclaredMethod.invoke(vmRuntimeClass, "getRuntime", null);
            Method setHiddenApiExemptions = (Method) getDeclaredMethod.invoke(vmRuntimeClass, "setHiddenApiExemptions", new Class[]{String[].class});
            Object sVmRuntime = getRuntime.invoke(null);
            setHiddenApiExemptions.invoke(sVmRuntime, new Object[]{new String[]{"L"}});
        } catch (Throwable e) {
            Log.e("[error]", "reflect bootstrap failed:", e);
        }*/
        Reflection.unseal(getApplicationContext());
        try {
            Class <?> clazz1 = Class.forName("com.android.internal.widget.LockPatternUtils");
            Object lockUtils = clazz1.getConstructor(Context.class).newInstance(this);
            Class <?> lockUtilsClazz = lockUtils.getClass();
            Method getSaltM = lockUtilsClazz.getDeclaredMethod("getSalt", int.class);
            getSaltM.setAccessible(true);
            Object obj = getSaltM.invoke(lockUtils, 0);
            Log.d("MainActivity", "getSalt: "+obj);
        }catch (Exception e) {
            e.printStackTrace();
        }
    }
}