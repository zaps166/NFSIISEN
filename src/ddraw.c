#include "ddraw.h"

#ifndef NDEBUG
	#warning DEBUG MODE

	static FILE *f;

	#define retVal() \
		(*(uint32_t *)(((void *)&this) - 4))

	#define trace() \
		fprintf(f, "%s from 0x%p\n", __PRETTY_FUNCTION__, retVal()); \
		fflush(f);

	#define dprintf(...) \
		fprintf(f, __VA_ARGS__); \
		fflush(f);
#else
	#define trace()
	#define dprintf(...)
#endif

static const uint8_t g_displayMode8[108] = { // 640x480 8 BPP (Indexed)
	0x6C, 0x00, 0x00, 0x00, 0x0E, 0x10, 0x04, 0x00, 0xE0, 0x01,
	0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x80, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t g_displayMode16[108] = { // 640x480 16 BPP (RGB565)
	0x6C, 0x00, 0x00, 0x00, 0x0E, 0x10, 0x04, 0x00, 0xE0, 0x01,
	0x00, 0x00, 0x80, 0x02, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0xF8,
	0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const uint8_t *g_currentDisplayMode = g_displayMode16;

/**/

static void copyPalette(Palette *dest, LPPALETTEENTRY src, DWORD count)
{
	for (DWORD i = 0; i != count; ++i)
	{
		dest[i].r = src[i].peRed;
		dest[i].g = src[i].peGreen;
		dest[i].b = src[i].peBlue;
	}
}

static inline void renderSDL(SDL_Renderer *renderer, SDL_Texture *texture)
{
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

/**/

static STDCALL uint32_t Palette_QueryInterface(DirectDrawPalette **this, REFIID riid, void **ppvObject)
{
	trace();
	*ppvObject = this;
	return 0;
}
static STDCALL uint32_t Palette_AddRef(DirectDrawPalette **this)
{
	trace();
	return 0;
}
static STDCALL uint32_t Palette_Release(DirectDrawPalette **this)
{
	trace();
	if (--(*this)->ref == 0)
	{
		free((*this)->palette);
		free(*this);
		free(this);
	}
	return 0;
}

static STDCALL uint32_t Palette_GetCaps(struct DirectDrawPalette **this, LPDWORD lpdwCaps)
{
	trace();
	return 0;
}
static STDCALL uint32_t Palette_GetEntries(struct DirectDrawPalette **this, DWORD dwFlags, DWORD dwBase, DWORD dwNumEntries, LPPALETTEENTRY lpEntries)
{
	trace();
	return 0;
}
static STDCALL uint32_t Palette_Initialize(struct DirectDrawPalette **this, LPDIRECTDRAW lpDD, DWORD dwFlags, LPPALETTEENTRY lpDDColorTable)
{
	trace();
	return 0;
}
static STDCALL uint32_t Palette_SetEntries(struct DirectDrawPalette **this, DWORD dwFlags, DWORD dwStartingEntry, DWORD dwCount, LPPALETTEENTRY lpEntries)
{
	trace();
	assert(dwFlags == 0);
	assert(dwStartingEntry == 0);
	assert(dwCount <= 256);
	copyPalette((*this)->palette, lpEntries, dwCount);
	return 0;
}

/**/

static STDCALL uint32_t Surface_QueryInterface(DirectDrawSurface **this, REFIID riid, void **ppvObject)
{
	trace();
	*ppvObject = this;
	return 0;
}
static STDCALL uint32_t Surface_AddRef(DirectDrawSurface **this)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_Release(DirectDrawSurface **this)
{
	trace();
	if (--(*this)->ref == 0)
	{
		dprintf("	Is attached: %d\n", (*this)->isAttached);
		SDL_DestroyTexture((*this)->texture);
		free(*this);
		free(this);
	}
	return 0;
}

static STDCALL uint32_t Surface_AddAttachedSurface(DirectDrawSurface **this, LPDIRECTDRAWSURFACE2 lpDDSAttachedSurface)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_AddOverlayDirtyRect(DirectDrawSurface **this, LPRECT lpRect)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_Blt(DirectDrawSurface **this, LPRECT lpDestRect, LPDIRECTDRAWSURFACE2 lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_BltBatch(DirectDrawSurface **this, LPDDBLTBATCH lpDDBltBatch, DWORD dwCount, DWORD dwFlags)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_BltFast(DirectDrawSurface **this, DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE2 lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwTrans)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_DeleteAttachedSurface(DirectDrawSurface **this, DWORD dwFlags, LPDIRECTDRAWSURFACE2 lpDDSAttachedSurface)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_EnumAttachedSurfaces(DirectDrawSurface **this, LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_EnumOverlayZOrders(DirectDrawSurface **this, DWORD dwFlags, LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpfnCallback)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_Flip(DirectDrawSurface **this, LPDIRECTDRAWSURFACE2 lpDDSurfaceTargetOverride, DWORD dwFlags)
{
	trace();
	assert((*this)->isAttached == 0);

	renderSDL((*this)->renderer, (*this)->texture);

	return 0;
}
static STDCALL uint32_t Surface_GetAttachedSurface(DirectDrawSurface **this, LPDDSCAPS lpDDSCaps, LPDIRECTDRAWSURFACE2 **lplpDDAttachedSurface)
{
	trace();
	assert(lpDDSCaps->dwCaps == DDSCAPS_BACKBUFFER);

	DirectDrawSurface *surface = malloc(sizeof(DirectDrawSurface));
	memcpy(surface, *this, sizeof(DirectDrawSurface));
	surface->ref = 1;
	surface->isAttached = 1;

	*lplpDDAttachedSurface = malloc(sizeof(void *));
	**lplpDDAttachedSurface = (LPDIRECTDRAWSURFACE2)surface;

	return 0;
}
static STDCALL uint32_t Surface_GetBltStatus(DirectDrawSurface **this, DWORD dwFlags)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_GetCaps(DirectDrawSurface **this, LPDDSCAPS lpDDSCaps)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_GetClipper(DirectDrawSurface **this, LPDIRECTDRAWCLIPPER *lplpDDClipper)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_GetColorKey(DirectDrawSurface **this, DWORD dwFlags, LPDDCOLORKEY lpDDColorKey)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_GetDC(DirectDrawSurface **this, HDC *lphDC)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_GetFlipStatus(DirectDrawSurface **this, DWORD dwFlags)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_GetOverlayPosition(DirectDrawSurface **this, LPLONG lplX, LPLONG lplY)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_GetPalette(DirectDrawSurface **this, LPDIRECTDRAWPALETTE *lplpDDPalette)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_GetPixelFormat(DirectDrawSurface **this, LPDDPIXELFORMAT lpDDPixelFormat)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_GetSurfaceDesc(DirectDrawSurface **this, LPDDSURFACEDESC lpDDSurfaceDesc)
{
	trace();

	assert(lpDDSurfaceDesc->dwSize == sizeof(DDSURFACEDESC));
	memcpy(lpDDSurfaceDesc, g_currentDisplayMode, sizeof(DDSURFACEDESC));
	assert(lpDDSurfaceDesc->dwSize == sizeof(DDSURFACEDESC));

	return 0;
}
static STDCALL uint32_t Surface_Initialize(DirectDrawSurface **this, LPDIRECTDRAW lpDD, LPDDSURFACEDESC lpDDSurfaceDesc)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_IsLost(DirectDrawSurface **this)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_Lock(DirectDrawSurface **this, LPRECT lpDestRect, LPDDSURFACEDESC lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent)
{
	trace();

	assert(lpDDSurfaceDesc->dwSize == sizeof(DDSURFACEDESC));
	memcpy(lpDDSurfaceDesc, g_currentDisplayMode, sizeof(DDSURFACEDESC));
	assert(lpDDSurfaceDesc->dwSize == sizeof(DDSURFACEDESC));

	assert(dwFlags == DDLOCK_WAIT);
	assert(!lpDestRect);

	if ((*this)->imgData)
	{
		// 8 BPP
		lpDDSurfaceDesc->lpSurface = (*this)->imgData;
	}
	else
	{
		// 16 BPP
		int pitch = 0;
		SDL_LockTexture((*this)->texture, NULL, &lpDDSurfaceDesc->lpSurface, &pitch);
		assert(pitch == 640 * 2);
	}

	return 0;
}
static STDCALL uint32_t Surface_ReleaseDC(DirectDrawSurface **this, HDC hDC)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_Restore(DirectDrawSurface **this)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_SetClipper(DirectDrawSurface **this, LPDIRECTDRAWCLIPPER lpDDClipper)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_SetColorKey(DirectDrawSurface **this, DWORD dwFlags, LPDDCOLORKEY lpDDColorKey)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_SetOverlayPosition(DirectDrawSurface **this, LONG lX, LONG lY)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_SetPalette(DirectDrawSurface **this, LPDIRECTDRAWPALETTE lpDDPalette)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_Unlock(DirectDrawSurface **this, LPVOID lpSurfaceData)
{
	trace();

	if ((*this)->imgData)
	{
		int pitch = 0;
		uint8_t *rgb = 0;
		SDL_LockTexture((*this)->texture, NULL, (void **)&rgb, &pitch);
		assert(pitch == 640 * 3);
		for (DWORD i = 0, j = 0; i < 640 * 480; i += 1, j += 3)
		{
			memcpy(rgb + j, &(*this)->palette[(*this)->imgData[i]], 3);
		}
	}

	SDL_UnlockTexture((*this)->texture);

	if ((*this)->isAttached == 0)
		renderSDL((*this)->renderer, (*this)->texture);

	return 0;
}
static STDCALL uint32_t Surface_UpdateOverlay(DirectDrawSurface **this, LPRECT lpSrcRect, LPDIRECTDRAWSURFACE2 lpDDDestSurface, LPRECT lpDestRect, DWORD dwFlags, LPDDOVERLAYFX lpDDOverlayFx)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_UpdateOverlayDisplay(DirectDrawSurface **this, DWORD dwFlags)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_UpdateOverlayZOrder(DirectDrawSurface **this, DWORD dwFlags, LPDIRECTDRAWSURFACE2 lpDDSReference)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_GetDDInterface(DirectDrawSurface **this, LPVOID *lplpDD)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_PageLock(DirectDrawSurface **this, DWORD dwFlags)
{
	trace();
	return 0;
}
static STDCALL uint32_t Surface_PageUnlock(DirectDrawSurface **this, DWORD dwFlags)
{
	trace();
	return 0;
}

/**/

static STDCALL uint32_t DDraw_QueryInterface(DirectDraw **this, REFIID riid, void **ppvObject)
{
	trace();
	++(*this)->ref;
	*ppvObject = this;
	return 0;
}
static STDCALL uint32_t DDraw_AddRef(DirectDraw **this)
{
	trace();
	return 0;
}
static STDCALL uint32_t DDraw_Release(DirectDraw **this)
{
	trace();
	if (--(*this)->ref == 0)
	{
		SDL_DestroyRenderer((*this)->renderer);
		SDL_DestroyWindow((*this)->win);
		if ((*this)->palette)
		{
			// Game doesn't free the palette, so do it here
			(*this)->palette->Release(&(*this)->palette);
		}
		free(*this);
		free(this);
		assert(SDL_WasInit(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == SDL_INIT_VIDEO | SDL_INIT_EVENTS);
		SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	}
	return 0;
}

static STDCALL uint32_t DDraw_Compact(DirectDraw **this)
{
	trace();
	return 0;
}
static STDCALL uint32_t DDraw_CreateClipper(DirectDraw **this, DWORD dwFlags, LPDIRECTDRAWCLIPPER *lplpDDClipper, IUnknown *pUnkOuter)
{
	trace();
	return 0;
}
static STDCALL uint32_t DDraw_CreatePalette(DirectDraw **this, DWORD dwFlags, LPPALETTEENTRY lpColorTable, LPDIRECTDRAWPALETTE **lplpDDPalette, IUnknown *pUnkOuter)
{
	trace();
	assert(dwFlags == (DDPCAPS_8BIT | DDPCAPS_ALLOW256));

	DirectDrawPalette *palette = calloc(1, sizeof(DirectDrawPalette));
	palette->ref = 1;

	*lplpDDPalette = malloc(sizeof(void *));
	**lplpDDPalette = (LPDIRECTDRAWPALETTE)palette;

	palette->QueryInterface = Palette_QueryInterface;
	palette->AddRef = Palette_AddRef;
	palette->Release = Palette_Release;

	palette->GetCaps = Palette_GetCaps;
	palette->GetEntries = Palette_GetEntries;
	palette->Initialize = Palette_Initialize;
	palette->SetEntries = Palette_SetEntries;

	palette->palette = malloc(256 * sizeof(Palette));
	copyPalette(palette->palette, lpColorTable, 256);

	(*this)->palette = palette;

	return 0;
}
static STDCALL uint32_t DDraw_CreateSurface(DirectDraw **this, LPDDSURFACEDESC lpDDSurfaceDesc, LPDIRECTDRAWSURFACE **lplpDDSurface, IUnknown *pUnkOuter)
{
	trace();
	assert(lpDDSurfaceDesc->dwSize == sizeof(DDSURFACEDESC));

	DirectDrawSurface *surface = calloc(1, sizeof(DirectDrawSurface));
	surface->ref = 1;

	*lplpDDSurface = malloc(sizeof(void *));
	**lplpDDSurface = (LPDIRECTDRAWSURFACE)surface;

	surface->QueryInterface = Surface_QueryInterface;
	surface->AddRef = Surface_AddRef;
	surface->Release = Surface_Release;

	surface->AddAttachedSurface = Surface_AddAttachedSurface;
	surface->AddOverlayDirtyRect = Surface_AddOverlayDirtyRect;
	surface->Blt = Surface_Blt;
	surface->BltBatch = Surface_BltBatch;
	surface->BltFast = Surface_BltFast;
	surface->DeleteAttachedSurface = Surface_DeleteAttachedSurface;
	surface->EnumAttachedSurfaces = Surface_EnumAttachedSurfaces;
	surface->EnumOverlayZOrders = Surface_EnumOverlayZOrders;
	surface->Flip = Surface_Flip;
	surface->GetAttachedSurface = Surface_GetAttachedSurface;
	surface->GetBltStatus = Surface_GetBltStatus;
	surface->GetCaps = Surface_GetCaps;
	surface->GetClipper = Surface_GetClipper;
	surface->GetColorKey = Surface_GetColorKey;
	surface->GetDC = Surface_GetDC;
	surface->GetFlipStatus = Surface_GetFlipStatus;
	surface->GetOverlayPosition = Surface_GetOverlayPosition;
	surface->GetPalette = Surface_GetPalette;
	surface->GetPixelFormat = Surface_GetPixelFormat;
	surface->GetSurfaceDesc = Surface_GetSurfaceDesc;
	surface->Initialize = Surface_Initialize;
	surface->IsLost = Surface_IsLost;
	surface->Lock = Surface_Lock;
	surface->ReleaseDC = Surface_ReleaseDC;
	surface->Restore = Surface_Restore;
	surface->SetClipper = Surface_SetClipper;
	surface->SetColorKey = Surface_SetColorKey;
	surface->SetOverlayPosition = Surface_SetOverlayPosition;
	surface->SetPalette = Surface_SetPalette;
	surface->Unlock = Surface_Unlock;
	surface->UpdateOverlay = Surface_UpdateOverlay;
	surface->UpdateOverlayDisplay = Surface_UpdateOverlayDisplay;
	surface->UpdateOverlayZOrder = Surface_UpdateOverlayZOrder;

	surface->GetDDInterface = Surface_GetDDInterface;
	surface->PageLock = Surface_PageLock;
	surface->PageUnlock = Surface_PageUnlock;

	surface->renderer = (*this)->renderer;
	if (g_currentDisplayMode == g_displayMode8)
	{
		surface->texture = SDL_CreateTexture((*this)->renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING, 640, 480);
		surface->imgData = malloc(640 * 480);
	}
	else if (g_currentDisplayMode == g_displayMode16)
	{
		surface->texture = SDL_CreateTexture((*this)->renderer, SDL_PIXELFORMAT_RGB565, SDL_TEXTUREACCESS_STREAMING, 640, 480);
	}
	surface->palette = (*this)->palette->palette;

	assert(surface->palette); // Palette is created earlier

	return 0;
}
static STDCALL uint32_t DDraw_DuplicateSurface(DirectDraw **this, LPDIRECTDRAWSURFACE lpDDSurface, LPDIRECTDRAWSURFACE *lplpDupDDSurface)
{
	trace();
	return 0;
}
static STDCALL uint32_t DDraw_EnumDisplayModes(DirectDraw **this, DWORD dwFlags, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMMODESCALLBACK lpEnumModesCallback)
{
	trace();

	lpEnumModesCallback((LPDDSURFACEDESC)g_displayMode16, lpContext);
	lpEnumModesCallback((LPDDSURFACEDESC)g_displayMode8, lpContext);

	return 0;
}
static STDCALL uint32_t DDraw_EnumSurfaces(DirectDraw **this, DWORD dwFlags, LPDDSURFACEDESC lpDDSD, LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback)
{
	trace();
	return 0;
}
static STDCALL uint32_t DDraw_FlipToGDISurface(DirectDraw **this)
{
	trace();
	return 0;
}
static STDCALL uint32_t DDraw_GetCaps(DirectDraw **this, LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps)
{
	trace();

	assert(lpDDDriverCaps->dwSize == sizeof(DDCAPS));
	assert(lpDDHELCaps->dwSize == sizeof(DDCAPS));

	memset(lpDDDriverCaps, 0, sizeof(DDCAPS));
	memset(lpDDHELCaps, 0, sizeof(DDCAPS));

	return 0;
}
static STDCALL uint32_t DDraw_GetDisplayMode(DirectDraw **this, LPDDSURFACEDESC lpDDSurfaceDesc)
{
	trace();

	assert(lpDDSurfaceDesc->dwSize == sizeof(DDSURFACEDESC));
	memcpy(lpDDSurfaceDesc, g_currentDisplayMode, sizeof(DDSURFACEDESC));
	assert(lpDDSurfaceDesc->dwSize == sizeof(DDSURFACEDESC));

	return 0;
}
static STDCALL uint32_t DDraw_GetFourCCCodes(DirectDraw **this, LPDWORD lpNumCodes, LPDWORD lpCodes)
{
	trace();
	return 0;
}
static STDCALL uint32_t DDraw_GetGDISurface(DirectDraw **this, LPDIRECTDRAWSURFACE *lplpGDIDDSurface)
{
	trace();
	return 0;
}
static STDCALL uint32_t DDraw_GetMonitorFrequency(DirectDraw **this, LPDWORD lpdwFrequency)
{
	trace();
	return 0;
}
static STDCALL uint32_t DDraw_GetScanLine(DirectDraw **this, LPDWORD lpdwScanLine)
{
	trace();
	return 0;
}
static STDCALL uint32_t DDraw_GetVerticalBlankStatus(DirectDraw **this, BOOL *lpbIsInVB)
{
	trace();
	return 0;
}
static STDCALL uint32_t DDraw_Initialize(DirectDraw **this, GUID *lpGUID)
{
	trace();
	return 0;
}
static STDCALL uint32_t DDraw_RestoreDisplayMode(DirectDraw **this)
{
	trace();
	return 0;
}
static STDCALL uint32_t DDraw_SetCooperativeLevel(DirectDraw **this, HWND hWnd, DWORD dwFlags)
{
	trace();

	(*this)->win = SDL_CreateWindowFrom(hWnd);
	(*this)->renderer = SDL_CreateRenderer((*this)->win, -1, 0);

	return 0;
}
static STDCALL uint32_t DDraw_SetDisplayMode(DirectDraw **this, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, WORD dwRefreshRate, DWORD dwFlags)
{
	trace();
	assert(dwBPP == 16 || dwBPP == 8);

	if (dwBPP == 16)
		g_currentDisplayMode = g_displayMode16;
	else if (dwBPP == 8)
		g_currentDisplayMode = g_displayMode8;

	return 0;
}
static STDCALL uint32_t DDraw_WaitForVerticalBlank(DirectDraw **this, DWORD dwFlags, HANDLE hEvent)
{
	trace();
	return 0;
}
static uint32_t STDCALL DDraw_GetAvailableVidMem(struct DirectDraw **this, LPDWORD lpdwTotal, LPDWORD lpdwFree)
{
	trace();
	return 0;
}

STDCALL uint32_t DirectDrawCreNFS(void *lpGUID, DirectDraw ***lplpDD, void *pUnkOuter)
{
	assert(!f);
#ifndef NDEBUG
// 	f = stdout;
	f = fopen("nfstrace.txt", "w");
#endif
	dprintf("DirectDrawCreate: 0x%p, 0x%p\n", lpGUID, lplpDD);

	SetProcessAffinityMask(GetCurrentProcess(), 1); // Game can't run properly on multicore CPUs

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
		ExitProcess(100);

	DirectDraw *ddraw = calloc(1, sizeof(DirectDraw));
	ddraw->ref = 1;

	*lplpDD = malloc(sizeof(void *));
	**lplpDD = ddraw;

	ddraw->QueryInterface = DDraw_QueryInterface;
	ddraw->AddRef = DDraw_AddRef;
	ddraw->Release = DDraw_Release;

	ddraw->Compact = DDraw_Compact;
	ddraw->CreateClipper = DDraw_CreateClipper;
	ddraw->CreatePalette = DDraw_CreatePalette;
	ddraw->CreateSurface = DDraw_CreateSurface;
	ddraw->DuplicateSurface = DDraw_DuplicateSurface;
	ddraw->EnumDisplayModes = DDraw_EnumDisplayModes;
	ddraw->EnumSurfaces = DDraw_EnumSurfaces;
	ddraw->FlipToGDISurface = DDraw_FlipToGDISurface;
	ddraw->GetCaps = DDraw_GetCaps;
	ddraw->GetDisplayMode = DDraw_GetDisplayMode;
	ddraw->GetFourCCCodes = DDraw_GetFourCCCodes;
	ddraw->GetGDISurface = DDraw_GetGDISurface;
	ddraw->GetMonitorFrequency = DDraw_GetMonitorFrequency;
	ddraw->GetScanLine = DDraw_GetScanLine;
	ddraw->GetVerticalBlankStatus = DDraw_GetVerticalBlankStatus;
	ddraw->Initialize = DDraw_Initialize;
	ddraw->RestoreDisplayMode = DDraw_RestoreDisplayMode;
	ddraw->SetCooperativeLevel = DDraw_SetCooperativeLevel;
	ddraw->SetDisplayMode = DDraw_SetDisplayMode;
	ddraw->WaitForVerticalBlank = DDraw_WaitForVerticalBlank;
	ddraw->GetAvailableVidMem = DDraw_GetAvailableVidMem;

	return 0;
}
STDCALL uint32_t DirectDrawEnumeraNFS(void *lpCallback, void *lpContext)
{
	dprintf("DirectDrawEnumerateA\n");
	return 0;
}
