
#pragma comment(lib, "fmod\\fmodvc.lib")

#define STRICT
#include "audio.h"
using namespace std;


Audio::Audio()
{
	FSOUND_Init(44100, 32, 0);
}

Audio::~Audio()
{
	for (unsigned int i = 0; i < sound.size(); i++)
		delete sound[i].first;

	FSOUND_Close();
}

int Audio::load(const char filename[])
{
	for (unsigned int n = 0; n < sound.size(); n++)
		if (!sound[n].second.compare(filename))
			return n;

	Sound * snd = NULL;

	string name(filename);
	name.erase(0, name.find_last_of('.'));

	for (unsigned int i = 0; i < name.size(); i++)
		name[i] = tolower(name[i]);

	if (!name.compare(".wav"))
		snd = new Sample;
	else if (!name.compare(".mod") || !name.compare(".xm") || !name.compare(".s3m"))
		snd = new Module;
	else if (!name.compare(".mp3") || !name.compare(".ogg"))
		snd = new Stream;

	snd->load(filename);

	int handle = sound.size();
	sound.push_back(pair<Sound *, string>(snd, filename));
	return handle;
}

void Audio::play(const int handle)
{
	sound[handle].first->play();
}

void Audio::Sample::load(const char filename[])
{
	sample = FSOUND_Sample_Load(FSOUND_FREE, filename, FSOUND_NORMAL, 0);
	FSOUND_Sample_SetLoopMode(sample, FSOUND_LOOP_OFF);
}

void Audio::Sample::play()
{
	FSOUND_PlaySound(FSOUND_FREE, sample);
}

void Audio::Sample::stop()
{
}

Audio::Module::~Module()
{
	FMUSIC_FreeSong(module);
}

void Audio::Module::load(const char filename[])
{
	module = FMUSIC_LoadSong(filename);
}

void Audio::Module::play()
{
	FMUSIC_PlaySong(module);
}

void Audio::Module::stop()
{
}

void Audio::Stream::load(const char filename[])
{
	stream = FSOUND_Stream_OpenFile(filename, FSOUND_LOOP_NORMAL, 0);
}

void Audio::Stream::play()
{
	FSOUND_SetPan(FSOUND_Stream_Play(FSOUND_FREE, stream), FSOUND_STEREOPAN);
}

void Audio::Stream::stop()
{
}

void Audio::startFFT(bool start)
{
	FSOUND_DSP_SetActive(FSOUND_DSP_GetFFTUnit(), start);
}

float * Audio::getSpectrum()
{
	return FSOUND_DSP_GetSpectrum();
}
