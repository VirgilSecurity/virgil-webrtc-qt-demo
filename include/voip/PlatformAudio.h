#ifndef VIRGIL_VOIP_PLATFORM_AUDIO_H_INCLUDED
#define VIRGIL_VOIP_PLATFORM_AUDIO_H_INCLUDED

#include <memory>

namespace virgil {
namespace voip {

//
// Handles platform dependent interactions with an audio device.
// All methods are internally syncronized.
//
class PlatformAudio {
public:
    //
    // Speaker control.
    //
    virtual bool
    hasSpeaker() const = 0;
    virtual bool
    setSpeakerOn() = 0;
    virtual bool
    setSpeakerOff() = 0;

    //
    // Microphone control.
    //
    virtual bool
    hasMicrophone() const = 0;
    virtual bool
    setMicrophoneOn() = 0;
    virtual bool
    setMicrophoneOff() = 0;

    static std::unique_ptr<PlatformAudio>
    createDefault();

    virtual ~PlatformAudio() noexcept = default;
};

} // namespace voip
} // namespace virgil


#endif /* VIRGIL_VOIP_PLATFORM_AUDIO_H_INCLUDED */
