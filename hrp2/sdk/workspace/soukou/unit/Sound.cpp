#include "Sound.h"


/**
 * コンストラクタ
 */
// Sound::Sound()
//     	{
// 			ev3_speaker_set_volume(10);
// }
// static *Sound Sound::getInstance(){
//     static Sound sound;
//     ev3_speaker_set_volume(10);
//     return &sound;
// }

void Sound::setVolume(uint8_t volume){
	ev3_speaker_set_volume(volume);
}

void Sound::ready()
{
	ev3_memfile_load("/ev3rt/sounds/ready.wav", &soundfile);
	ev3_speaker_play_file(&soundfile,SOUND_MANUAL_STOP);
}
void Sound::ok()
{
	ev3_memfile_load("/ev3rt/sounds/ok.wav", &soundfile);
	ev3_speaker_play_file(&soundfile,SOUND_MANUAL_STOP);
}
void Sound::trumpet()
{
	ev3_memfile_load("/ev3rt/sounds/trumpet.wav", &soundfile);
	ev3_speaker_play_file(&soundfile,SOUND_MANUAL_STOP);
}
