#ifndef EV3_UNIT_SOUND_H_
#define EV3_UNIT_SOUND_H_

#include "ev3api.h"
#include "app_config.h"

class Sound
{
    public:
        static Sound *getInstance(){
            static Sound sound;
            ev3_speaker_set_volume(SOUND_VOL);
            return &sound;
        }
        // Sound();
        void ok();
        void ready();
        void trumpet();

        void setVolume(uint8_t volume);
    private:
        uint8_t mVolume;
        memfile_t soundfile;
};

#endif  // EV3_UNIT_SOUND_H_
