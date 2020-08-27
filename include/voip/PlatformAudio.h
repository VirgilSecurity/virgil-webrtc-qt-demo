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

    virtual void
    setSpeakerOn(bool on) = 0;

    static std::unique_ptr<PlatformAudio>
    createDefault();

    virtual ~PlatformAudio() noexcept = default;
};

} // namespace voip
} // namespace virgil


#endif /* VIRGIL_VOIP_PLATFORM_AUDIO_H_INCLUDED */
