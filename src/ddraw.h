#include <ddraw.h>

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#define STDCALL __attribute__((stdcall))

typedef struct
{
	uint8_t r, g, b;
} Palette;

typedef struct DirectDrawPalette
{
	/*** IUnknown methods ***/
	uint32_t (STDCALL *QueryInterface)(struct DirectDrawPalette **this, REFIID riid, void **ppvObject);
	uint32_t (STDCALL *AddRef)(struct DirectDrawPalette **this);
	uint32_t (STDCALL *Release)(struct DirectDrawPalette **this);
	/*** IDirectDrawPalette methods ***/
	uint32_t (STDCALL *GetCaps)(struct DirectDrawPalette **this, LPDWORD lpdwCaps);
	uint32_t (STDCALL *GetEntries)(struct DirectDrawPalette **this, DWORD dwFlags, DWORD dwBase, DWORD dwNumEntries, LPPALETTEENTRY lpEntries);
	uint32_t (STDCALL *Initialize)(struct DirectDrawPalette **this, LPDIRECTDRAW lpDD, DWORD dwFlags, LPPALETTEENTRY lpDDColorTable);
	uint32_t (STDCALL *SetEntries)(struct DirectDrawPalette **this, DWORD dwFlags, DWORD dwStartingEntry, DWORD dwCount, LPPALETTEENTRY lpEntries);
	/* my vars */
	uint32_t ref;
	Palette *palette;
} DirectDrawPalette;

typedef struct DirectDrawSurface
{
    /*** IUnknown methods ***/
	uint32_t (STDCALL *QueryInterface)(struct DirectDrawSurface **this, REFIID riid, void **ppvObject);
	uint32_t (STDCALL *AddRef)(struct DirectDrawSurface **this);
	uint32_t (STDCALL *Release)(struct DirectDrawSurface **this);
    /*** IDirectDrawSurface2 methods ***/
    uint32_t (STDCALL *AddAttachedSurface)(struct DirectDrawSurface **this, LPDIRECTDRAWSURFACE2 lpDDSAttachedSurface);
    uint32_t (STDCALL *AddOverlayDirtyRect)(struct DirectDrawSurface **this, LPRECT lpRect);
    uint32_t (STDCALL *Blt)(struct DirectDrawSurface **this, LPRECT lpDestRect, LPDIRECTDRAWSURFACE2 lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwFlags, LPDDBLTFX lpDDBltFx);
    uint32_t (STDCALL *BltBatch)(struct DirectDrawSurface **this, LPDDBLTBATCH lpDDBltBatch, DWORD dwCount, DWORD dwFlags);
    uint32_t (STDCALL *BltFast)(struct DirectDrawSurface **this, DWORD dwX, DWORD dwY, LPDIRECTDRAWSURFACE2 lpDDSrcSurface, LPRECT lpSrcRect, DWORD dwTrans);
    uint32_t (STDCALL *DeleteAttachedSurface)(struct DirectDrawSurface **this, DWORD dwFlags, LPDIRECTDRAWSURFACE2 lpDDSAttachedSurface);
    uint32_t (STDCALL *EnumAttachedSurfaces)(struct DirectDrawSurface **this, LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback);
    uint32_t (STDCALL *EnumOverlayZOrders)(struct DirectDrawSurface **this, DWORD dwFlags, LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpfnCallback);
    uint32_t (STDCALL *Flip)(struct DirectDrawSurface **this, LPDIRECTDRAWSURFACE2 lpDDSurfaceTargetOverride, DWORD dwFlags);
    uint32_t (STDCALL *GetAttachedSurface)(struct DirectDrawSurface **this, LPDDSCAPS lpDDSCaps, LPDIRECTDRAWSURFACE2 **lplpDDAttachedSurface);
    uint32_t (STDCALL *GetBltStatus)(struct DirectDrawSurface **this, DWORD dwFlags);
    uint32_t (STDCALL *GetCaps)(struct DirectDrawSurface **this, LPDDSCAPS lpDDSCaps);
    uint32_t (STDCALL *GetClipper)(struct DirectDrawSurface **this, LPDIRECTDRAWCLIPPER *lplpDDClipper);
    uint32_t (STDCALL *GetColorKey)(struct DirectDrawSurface **this, DWORD dwFlags, LPDDCOLORKEY lpDDColorKey);
    uint32_t (STDCALL *GetDC)(struct DirectDrawSurface **this, HDC *lphDC);
    uint32_t (STDCALL *GetFlipStatus)(struct DirectDrawSurface **this, DWORD dwFlags);
    uint32_t (STDCALL *GetOverlayPosition)(struct DirectDrawSurface **this, LPLONG lplX, LPLONG lplY);
    uint32_t (STDCALL *GetPalette)(struct DirectDrawSurface **this, LPDIRECTDRAWPALETTE *lplpDDPalette);
    uint32_t (STDCALL *GetPixelFormat)(struct DirectDrawSurface **this, LPDDPIXELFORMAT lpDDPixelFormat);
    uint32_t (STDCALL *GetSurfaceDesc)(struct DirectDrawSurface **this, LPDDSURFACEDESC lpDDSurfaceDesc);
    uint32_t (STDCALL *Initialize)(struct DirectDrawSurface **this, LPDIRECTDRAW lpDD, LPDDSURFACEDESC lpDDSurfaceDesc);
    uint32_t (STDCALL *IsLost)(struct DirectDrawSurface **this);
    uint32_t (STDCALL *Lock)(struct DirectDrawSurface **this, LPRECT lpDestRect, LPDDSURFACEDESC lpDDSurfaceDesc, DWORD dwFlags, HANDLE hEvent);
    uint32_t (STDCALL *ReleaseDC)(struct DirectDrawSurface **this, HDC hDC);
    uint32_t (STDCALL *Restore)(struct DirectDrawSurface **this);
    uint32_t (STDCALL *SetClipper)(struct DirectDrawSurface **this, LPDIRECTDRAWCLIPPER lpDDClipper);
    uint32_t (STDCALL *SetColorKey)(struct DirectDrawSurface **this, DWORD dwFlags, LPDDCOLORKEY lpDDColorKey);
    uint32_t (STDCALL *SetOverlayPosition)(struct DirectDrawSurface **this, LONG lX, LONG lY);
    uint32_t (STDCALL *SetPalette)(struct DirectDrawSurface **this, LPDIRECTDRAWPALETTE lpDDPalette);
    uint32_t (STDCALL *Unlock)(struct DirectDrawSurface **this, LPVOID lpSurfaceData);
    uint32_t (STDCALL *UpdateOverlay)(struct DirectDrawSurface **this, LPRECT lpSrcRect, LPDIRECTDRAWSURFACE2 lpDDDestSurface, LPRECT lpDestRect, DWORD dwFlags, LPDDOVERLAYFX lpDDOverlayFx);
    uint32_t (STDCALL *UpdateOverlayDisplay)(struct DirectDrawSurface **this, DWORD dwFlags);
    uint32_t (STDCALL *UpdateOverlayZOrder)(struct DirectDrawSurface **this, DWORD dwFlags, LPDIRECTDRAWSURFACE2 lpDDSReference);
    /* added in v2 */
    uint32_t (STDCALL *GetDDInterface)(struct DirectDrawSurface **this, LPVOID *lplpDD);
    uint32_t (STDCALL *PageLock)(struct DirectDrawSurface **this, DWORD dwFlags);
    uint32_t (STDCALL *PageUnlock)(struct DirectDrawSurface **this, DWORD dwFlags);
	/* my vars */
	uint32_t ref;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	uint8_t *imgData; // For 8 BPP
	Palette *palette;
	BOOL isAttached;
} DirectDrawSurface;

typedef struct DirectDraw
{
	/*** IUnknown methods ***/
	uint32_t (STDCALL *QueryInterface)(struct DirectDraw **this, REFIID riid, void **ppvObject);
	uint32_t (STDCALL *AddRef)(struct DirectDraw **this);
	uint32_t (STDCALL *Release)(struct DirectDraw **this);
	/*** IDirectDraw2 methods ***/
	uint32_t (STDCALL *Compact)(struct DirectDraw **this);
	uint32_t (STDCALL *CreateClipper)(struct DirectDraw **this, DWORD dwFlags, LPDIRECTDRAWCLIPPER *lplpDDClipper, IUnknown *pUnkOuter);
	uint32_t (STDCALL *CreatePalette)(struct DirectDraw **this, DWORD dwFlags, LPPALETTEENTRY lpColorTable, LPDIRECTDRAWPALETTE **lplpDDPalette, IUnknown *pUnkOuter);
	uint32_t (STDCALL *CreateSurface)(struct DirectDraw **this, LPDDSURFACEDESC lpDDSurfaceDesc, LPDIRECTDRAWSURFACE **lplpDDSurface, IUnknown *pUnkOuter);
	uint32_t (STDCALL *DuplicateSurface)(struct DirectDraw **this, LPDIRECTDRAWSURFACE lpDDSurface, LPDIRECTDRAWSURFACE *lplpDupDDSurface);
	uint32_t (STDCALL *EnumDisplayModes)(struct DirectDraw **this, DWORD dwFlags, LPDDSURFACEDESC lpDDSurfaceDesc, LPVOID lpContext, LPDDENUMMODESCALLBACK lpEnumModesCallback);
	uint32_t (STDCALL *EnumSurfaces)(struct DirectDraw **this, DWORD dwFlags, LPDDSURFACEDESC lpDDSD, LPVOID lpContext, LPDDENUMSURFACESCALLBACK lpEnumSurfacesCallback);
	uint32_t (STDCALL *FlipToGDISurface)(struct DirectDraw **this);
	uint32_t (STDCALL *GetCaps)(struct DirectDraw **this, LPDDCAPS lpDDDriverCaps, LPDDCAPS lpDDHELCaps);
	uint32_t (STDCALL *GetDisplayMode)(struct DirectDraw **this, LPDDSURFACEDESC lpDDSurfaceDesc);
	uint32_t (STDCALL *GetFourCCCodes)(struct DirectDraw **this, LPDWORD lpNumCodes, LPDWORD lpCodes);
	uint32_t (STDCALL *GetGDISurface)(struct DirectDraw **this, LPDIRECTDRAWSURFACE *lplpGDIDDSurface);
	uint32_t (STDCALL *GetMonitorFrequency)(struct DirectDraw **this, LPDWORD lpdwFrequency);
	uint32_t (STDCALL *GetScanLine)(struct DirectDraw **this, LPDWORD lpdwScanLine);
	uint32_t (STDCALL *GetVerticalBlankStatus)(struct DirectDraw **this, BOOL *lpbIsInVB);
	uint32_t (STDCALL *Initialize)(struct DirectDraw **this, GUID *lpGUID);
	uint32_t (STDCALL *RestoreDisplayMode)(struct DirectDraw **this);
	uint32_t (STDCALL *SetCooperativeLevel)(struct DirectDraw **this, HWND hWnd, DWORD dwFlags);
	uint32_t (STDCALL *SetDisplayMode)(struct DirectDraw **this, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP, WORD dwRefreshRate, DWORD dwFlags);
	uint32_t (STDCALL *WaitForVerticalBlank)(struct DirectDraw **this, DWORD dwFlags, HANDLE hEvent);
	/* added in v2 */
	uint32_t (STDCALL *GetAvailableVidMem)(struct DirectDraw **this, LPDWORD lpdwTotal, LPDWORD lpdwFree);
	/* my vars */
	uint32_t ref;
	SDL_Window *win;
	SDL_Renderer *renderer;
	DirectDrawPalette *palette;
} DirectDraw;
