package com.mad.project.team3.places_near_me;

import java.security.NoSuchAlgorithmException;

import android.app.Application;
        import android.content.pm.PackageInfo;
        import android.content.pm.PackageManager;
        import android.content.pm.Signature;
        import android.util.Base64;
        import android.util.Log;

        import java.security.MessageDigest;
        import java.security.NoSuchAlgorithmException;

/**
 * Created by Vishwanath on 3/11/2016.
 */
public class MyApplication extends Application {
    @Override
    public void onCreate() {
        super.onCreate();
        printhaskkey();
    }

    public void printhaskkey()
    {
        // Add code to print out the key hash
        try {
            PackageInfo info = getPackageManager().getPackageInfo(
                    "com.mad.project.team3.places_near_me",
                    PackageManager.GET_SIGNATURES);
            for (Signature signature : info.signatures) {
                MessageDigest md = MessageDigest.getInstance("SHA");
                md.update(signature.toByteArray());
                Log.d("Vishwanath:", Base64.encodeToString(md.digest(), Base64.DEFAULT));
            }
        } catch (PackageManager.NameNotFoundException e) {

        } catch (NoSuchAlgorithmException e) {

        }
    }
}
