package com.virgilsecurity.voip;

import android.media.AudioManager;
import android.content.Context;
import android.util.Log;

import org.webrtc.ContextUtils;


public class PlatformAudio {
    private static final String TAG = PlatformAudio.class.getName();
    private AudioManager audioManager;

    //
    //  Construction.
    //
    PlatformAudio() {
        Context context = ContextUtils.getApplicationContext();
        if (context == null) {
            throw new NullPointerException("PlatformAudio: can not get android context.");
        }

        this.audioManager = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
        if (this.audioManager == null) {
            throw new NullPointerException("PlatformAudio: can not get AudioManager service.");
        }

        this.audioManager.setMode(AudioManager.MODE_IN_COMMUNICATION);
    }

    //
    // Speaker control.
    //
    boolean hasSpeaker() {
        Log.d(TAG, "hasSpeaker()");
        return true;
    }

    void setSpeakerOn(boolean on) {
        Log.d(TAG, "setSpeakerOn(): " + on);
        audioManager.setSpeakerphoneOn(on);
    }
}
