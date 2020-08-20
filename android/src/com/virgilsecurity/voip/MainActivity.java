/**
 * Custom Activity is used to:
 *   - pass application context to the WebRTC;
 *   - request voice recording permissions.
 *
 * Code to request permissions is taken from:
 *   https://developer.here.com/documentation/android-premium/3.15/dev_guide/topics/request-android-permissions.html
 */
package com.virgilsecurity.voip;

import android.os.Bundle;
import android.content.Context;
import android.content.pm.PackageManager;
import android.Manifest;
import android.util.Log;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.core.content.ContextCompat;
import androidx.core.app.ActivityCompat;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import org.qtproject.qt5.android.bindings.QtActivity;
import org.webrtc.ContextUtils;

public class MainActivity extends QtActivity {
    private static final String TAG = MainActivity.class.getName();

    /**
     * permissions request code
     */
    private final static int REQUEST_CODE_ASK_PERMISSIONS = 1;

    /**
     * Permissions that need to be explicitly requested from end user.
     */
    private static final String[] REQUIRED_PERMISSIONS = new String[] {
        Manifest.permission.RECORD_AUDIO
    };

    @Override
    public void onCreate(Bundle savedInstanceState) {
        Log.d(TAG, "Initialize MainActivity");

        ContextUtils.initialize(getApplicationContext());

        super.onCreate(savedInstanceState);

        checkPermissions();
    }

    /**
     * Checks the dynamically-controlled permissions and requests missing permissions from end user.
     */
    protected void checkPermissions() {
        final List < String > missingPermissions = new ArrayList < String > ();
        // check all required dynamic permissions
        for (final String permission: REQUIRED_PERMISSIONS) {
            final int result = ContextCompat.checkSelfPermission(this, permission);
            if (result != PackageManager.PERMISSION_GRANTED) {
                missingPermissions.add(permission);
            }
        }
        if (!missingPermissions.isEmpty()) {
            // request all missing permissions
            final String[] permissions = missingPermissions
                .toArray(new String[missingPermissions.size()]);
            ActivityCompat.requestPermissions(this, permissions, REQUEST_CODE_ASK_PERMISSIONS);
        } else {
            final int[] grantResults = new int[REQUIRED_PERMISSIONS.length];
            Arrays.fill(grantResults, PackageManager.PERMISSION_GRANTED);
            onRequestPermissionsResult(REQUEST_CODE_ASK_PERMISSIONS, REQUIRED_PERMISSIONS, grantResults);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String permissions[],
        @NonNull int[] grantResults) {
        switch (requestCode) {
            case REQUEST_CODE_ASK_PERMISSIONS:
                for (int index = permissions.length - 1; index >= 0; --index) {
                    if (grantResults[index] != PackageManager.PERMISSION_GRANTED) {
                        // exit the app if one permission is not granted
                        Toast.makeText(this, "Required permission '" + permissions[index] +
                            "' not granted, exiting", Toast.LENGTH_LONG).show();
                        finish();
                        return;
                    }
                }
                Log.d(TAG, "All permissions were granted.");
                break;
        }
    }

}
