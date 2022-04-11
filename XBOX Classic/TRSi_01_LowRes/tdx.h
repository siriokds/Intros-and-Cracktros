#ifndef _TDX_H
#define _TDX_H


// Deadzone for the gamepad inputs
const SHORT XINPUT_DEADZONE = (SHORT)( 0.24f * FLOAT(0x7FFF) );


class	tdx
{
protected:
    // Main objects used for creating and rendering the 3D scene
    D3DPRESENT_PARAMETERS m_d3dpp;
    LPDIRECT3D8           m_pD3D;              // The D3D enumerator object
    LPDIRECT3DDEVICE8     m_pd3dDevice;        // The D3D rendering device
    LPDIRECT3DSURFACE8    m_pBackBuffer;       // The back buffer
    LPDIRECT3DSURFACE8    m_pDepthBuffer;      // The depth buffer

    // Variables for timing
    FLOAT      m_fTime;             // Current absolute time in seconds
    FLOAT      m_fElapsedTime;      // Elapsed absolute time since last frame
    FLOAT      m_fAppTime;          // Current app time in seconds
    FLOAT      m_fElapsedAppTime;   // Elapsed app time since last frame
    BOOL       m_bPaused;           // Whether app time is paused by user
    FLOAT      m_fFPS;              // instantaneous frame rate
    WCHAR      m_strFrameRate[20];  // Frame rate written to a string
    HANDLE     m_hFrameCounter;     // Handle to frame rate perf counter

    // Members to init the XINPUT devices.
    XDEVICE_PREALLOC_TYPE* m_InputDeviceTypes;
    DWORD                  m_dwNumInputDeviceTypes;
    XBGAMEPAD*             m_Gamepad;
    XBGAMEPAD              m_DefaultGamepad;

	HRESULT	Create(void);
	
public:
	void	Open(void);

};


#endif