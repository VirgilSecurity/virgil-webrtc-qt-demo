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

    boolean setSpeakerOn() {
        Log.d(TAG, "setSpeakerOn()");
        return true;
    }

    boolean setSpeakerOff() {
        Log.d(TAG, "setSpeakerOff()");
        return true;
    }

    //
    // Microphone control.
    //
    boolean hasMicrophone() {
        Log.d(TAG, "hasMicrophone()");

        try {
            return audioManager.getMicrophones().size() > 0;
        } catch (Exception e) {
            Log.e(TAG, e.getMessage());
            return false;
        }
    }

    boolean setMicrophoneOn() {
        Log.d(TAG, "setMicrophoneOn()");

        if (!hasMicrophone()) {
            return false;
        }

        if (audioManager.isMicrophoneMute()) {
            audioManager.setMicrophoneMute(false);
        }

        return true;
    }

    boolean setMicrophoneOff() {
        Log.d(TAG, "setMicrophoneOff()");

        if (!hasMicrophone()) {
            return false;
        }

        if (audioManager.isMicrophoneMute()) {
            audioManager.setMicrophoneMute(true);
        }

        return true;
    }
}
