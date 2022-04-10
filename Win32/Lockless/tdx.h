#ifndef TDX_H_
#define TDX_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Types.h"
#include <windows.h>

HRESULT	__stdcall tdxOpen(DWORD hWnd);			// Open System and change resolution to 320x240x8
							// hWnd = Window Handle 	es: GetForegroundWindow()
void	__stdcall tdxClose(void);			// Close System and rechange resolution
HRESULT	__stdcall tdxUpdate(UBYTE *src, int pitch);	// Update FrameBuffer And Flip...
void	__stdcall tdxSetPal(UBYTE *pal);		// Update Palette
void	__stdcall tdxWait4VBlank(int end);			// Wait 4 vertical retrace

#ifdef __cplusplus
}
#endif

#endif
