
#pragma once
#include "fmod\\fmod.h"
#pragma warning (disable:4786)
#include <vector>
#include <string>


class Audio
{
	public:
		Audio();
		~Audio();

		int load(const char filename[]);
		void play(const int handle);
		void stop(const int handle);

		void startFFT(bool start);
		float * getSpectrum();

	//protected:
		class Sound
		{
			public:
				virtual ~Sound() {}
				virtual void load(const char filename[]) = 0;
				virtual void play() = 0;
				virtual void stop() = 0;
		};

		class Sample : public Sound
		{
			public:
				void load(const char filename[]);
				void play();
				void stop();

			//protected:
				struct FSOUND_SAMPLE * sample;
		};

		class Module : public Sound
		{
			public:
				~Module();
				void load(const char filename[]);
				void play();
				void stop();

			//protected:
				struct FMUSIC_MODULE * module;
		};

		class Stream : public Sound
		{
			public:
				void load(const char filename[]);
				void play();
				void stop();

			//protected:
				struct FSOUND_STREAM * stream;
		};

		std::vector< std::pair<Sound *, std::string> > sound;
};
