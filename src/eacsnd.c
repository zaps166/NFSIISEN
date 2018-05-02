#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define STDCALL __attribute__((stdcall))
#define REGPARM __attribute__((regparm(2))) //First two arguments are compatible with Watcom fastcall

static void (REGPARM *getSamples)(void *samples, uint32_t num_samples_per_chn);

typedef void (*FadeInOut)();
static FadeInOut fadeInOut;

#include <SDL2/SDL.h>

#define CHN_CNT 2

static SDL_AudioDeviceID audioDevice;
static uint32_t unPaused, canGetSamples;
static uint32_t buffer_pos;
static uint8_t *buffer;

static void audioCallback(void *userdata, uint8_t *stream, int32_t len)
{
	if (!buffer)
	{
		int32_t i;
		for (i = 0; i < len; i += 256 * CHN_CNT * sizeof(int16_t))
			getSamples(stream + i, 256);
	}
	else
	{
		while (buffer_pos < len)
		{
			getSamples(buffer + buffer_pos, 256);
			buffer_pos += 256 * CHN_CNT * sizeof(int16_t);
		}
		memcpy(stream, buffer, len);
		memcpy(buffer, buffer + len, buffer_pos -= len);
	}
}

/**/

uint32_t iSNDdllversion_(void)
{
	return 0x60002;
}

STDCALL uint32_t iSNDdirectsetfunctions(void (REGPARM *arg1)(), void (*arg2)(), void (*arg3)(), FadeInOut arg4, void (*arg5)())
{
	getSamples = arg1;
	fadeInOut  = arg4;
	return 0;
}
REGPARM uint32_t iSNDdirectcaps_(void *hWnd)
{
	return 0x23E0F; //?
}
REGPARM uint32_t iSNDdirectstart_(uint32_t arg1, void *hWnd)
{
	if (SDL_WasInit(SDL_INIT_AUDIO) != SDL_INIT_AUDIO)
		SDL_InitSubSystem(SDL_INIT_AUDIO);

	SDL_AudioSpec audioSpecIn =
	{
		22050,
		AUDIO_S16,
		CHN_CNT,
		0,
		1024,
		0,
		0,
		audioCallback,
		NULL
	};
	SDL_AudioSpec audioSpecOut;
	audioDevice = SDL_OpenAudioDevice(NULL, 0, &audioSpecIn, &audioSpecOut, 0);
	if (!audioDevice)
		buffer = (uint8_t *)malloc(256 * CHN_CNT * sizeof(int16_t));
	else
	{
		uint32_t bufferSize = (audioSpecOut.samples + 255) & ~255; //Aligned to 256
		if (bufferSize != audioSpecOut.samples)
		{
			bufferSize += 256;
			bufferSize *= CHN_CNT * sizeof(int16_t);
			buffer = (uint8_t *)malloc(bufferSize);
		}
	}
	canGetSamples = 1;
	return 0;
}
void iSNDdirectserve_()
{
	if (canGetSamples)
	{
		if (!unPaused && audioDevice)
		{
			SDL_PauseAudioDevice(audioDevice, 0);
			unPaused = 1;
		}
		fadeInOut();
		if (!audioDevice)
			getSamples(buffer, 256);
	}
}
uint32_t iSNDdirectstop_(void)
{
	canGetSamples = 0;
	if (audioDevice)
	{
		SDL_CloseAudioDevice(audioDevice);
		unPaused = 0;
		audioDevice = 0;
	}
	if (SDL_WasInit(SDL_INIT_AUDIO) == SDL_INIT_AUDIO)
		SDL_QuitSubSystem(SDL_INIT_AUDIO);
	buffer_pos = 0;
	free(buffer);
	buffer = NULL;
	return 0;
}
