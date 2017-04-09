//-----------------------------------------------------------------------------
// File: Matrices.cpp
//
// Desc: Now that we know how to create a device and render some 2D vertices,
//       this tutorial goes the next step and renders 3D geometry. To deal with
//       3D geometry we need to introduce the use of 4x4 Matrices to transform
//       the geometry with translations, rotations, scaling, and setting up our
//       camera.
//
//       Geometry is defined in model space. We can move it (translation),
//       rotate it (rotation), or stretch it (scaling) using a world transform.
//       The geometry is then said to be in world space. Next, we need to
//       position the camera, or eye point, somewhere to look at the geometry.
//       Another transform, via the view matrix, is used, to position and
//       rotate our view. With the geometry then in view space, our last
//       transform is the projection transform, which "projects" the 3D scene
//       into our 2D viewport.
//
//       Note that in this tutorial, we are introducing the use of D3DX, which
//       is a set of helper utilities for D3D. In this case, we are using some
//       of D3DX's useful matrix initialization functions. To use D3DX, simply
//       include <d3dx9.h> and link with d3dx9.lib.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//-----------------------------------------------------------------------------
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 )


float A = 0;
float B = 0;

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER9 g_pVB01 = NULL; // Buffer to hold vertices
LPDIRECT3DVERTEXBUFFER9 g_pVB02 = NULL;

// A structure for our custom vertex type
struct CUSTOMVERTEX
{
    FLOAT x, y, z;      // The untransformed, 3D position for the vertex
    DWORD color;        // The vertex color
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)




//-----------------------------------------------------------------------------
// Name: InitD3D()
// Desc: Initializes Direct3D
//-----------------------------------------------------------------------------
HRESULT InitD3D( HWND hWnd )
{
    // Create the D3D object.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof( d3dpp ) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    // Create the D3DDevice
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }

    // Turn off culling, so we see the front and back of the triangle
    g_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

    // Turn off D3D lighting, since we are providing our own vertex colors
    g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: InitGeometry()
// Desc: Creates the scene geometry
//-----------------------------------------------------------------------------
HRESULT InitGeometry()
{
    // Initialize three vertices for rendering a triangle
    CUSTOMVERTEX g_Vertices01[] =
    {
        {-0.224513988289793f, 0.309016994374947f, 0.0f, 0xffff0000, },
        { 0.0f, 0.0f, 0.0f, 0xffffffff, },
		{ 0.224513988289793f, 0.309016994374947f, 0.0f, 0xffff0000, },

		{ 0.0f, 1.0f, 0.0f, 0xffffffff, },
		{-0.224513988289793f, 0.309016994374947f, 0.0f, 0xffff0000, },
		{ 0.224513988289793f, 0.309016994374947f, 0.0f, 0xffff0000, },

		{ 0.224513988289793f, 0.309016994374947f, 0.0f, 0xffff0000, },
		{ 0.0f, 0.0f, 0.0f, 0xffffffff, },
		{ 0.36327126400268f, -0.118033988749895f, 0.0f, 0xffff0000, },

		{ 0.951056516295154f, 0.3090169943374947f, 0.0f, 0xffffffff, },
		{ 0.224513988289793f, 0.309016994374947f, 0.0f, 0xffff0000, },
		{ 0.36327126400268f, -0.118033988749895f, 0.0f, 0xffff0000, },

		{ 0.36327126400268f, -0.118033988749895f, 0.0f, 0xffff0000, },
		{ 0.0f, 0.0f, 0.0f, 0xffffffff, },
		{ 0.0f, -0.381966011250105f, 0.0f, 0xffff0000, },

		{ 0.587785252292473f, -0.809016994374948f, 0.0f, 0xffffffff, },
		{ 0.36327126400268f, -0.118033988749895f, 0.0f, 0xffff0000, },
		{ 0.0f, -0.381966011250105f, 0.0f, 0xffff0000, },
		
		{ 0.0f, -0.381966011250105f, 0.0f, 0xffff0000, },
		{ 0.0f, 0.0f, 0.0f, 0xffffffff, },
		{-0.36327126400268f, -0.118033988749895f, 0.0f, 0xffff0000, },

		{-0.587785252292473f, -0.809016994374947f, 0.0f, 0xffffffff, },
		{ 0.0f, -0.381966011250105f, 0.0f, 0xffff0000, },
		{-0.36327126400268f, -0.118033988749895f, 0.0f, 0xffff0000, },

		{-0.36327126400268f, -0.118033988749895f, 0.0f, 0xffff0000, },
		{ 0.0f, 0.0f, 0.0f, 0xffffffff, },
		{-0.224513988289793f, 0.309016994374947f, 0.0f, 0xffff0000, },

		{-0.951056516295154f, 0.309016994374948f, 0.0f, 0xffffffff, },
		{-0.36327126400268f, -0.118033988749895f, 0.0f, 0xffff0000, },
		{-0.224513988289793f, 0.309016994374947f, 0.0f, 0xffff0000, },
    };

	CUSTOMVERTEX g_Vertices02[] =
	{
		{ -0.224513988289793f, 0.309016994374947f, 0.0f, 0xff0000ff, },
		{ 0.0f, 0.0f, 0.0f, 0xffffffff, },
		{ 0.224513988289793f, 0.309016994374947f, 0.0f, 0xff0000ff, },

		{ 0.0f, 1.0f, 0.0f, 0xffffffff, },
		{ -0.224513988289793f, 0.309016994374947f, 0.0f, 0xff0000ff, },
		{ 0.224513988289793f, 0.309016994374947f, 0.0f, 0xff0000ff, },

		{ 0.224513988289793f, 0.309016994374947f, 0.0f, 0xff0000ff, },
		{ 0.0f, 0.0f, 0.0f, 0xffffffff, },
		{ 0.36327126400268f, -0.118033988749895f, 0.0f, 0xff0000ff, },

		{ 0.951056516295154f, 0.3090169943374947f, 0.0f, 0xffffffff, },
		{ 0.224513988289793f, 0.309016994374947f, 0.0f, 0xff0000ff, },
		{ 0.36327126400268f, -0.118033988749895f, 0.0f, 0xff0000ff, },

		{ 0.36327126400268f, -0.118033988749895f, 0.0f, 0xff0000ff, },
		{ 0.0f, 0.0f, 0.0f, 0xffffffff, },
		{ 0.0f, -0.381966011250105f, 0.0f, 0xff0000ff, },

		{ 0.587785252292473f, -0.809016994374948f, 0.0f, 0xffffffff, },
		{ 0.36327126400268f, -0.118033988749895f, 0.0f, 0xff0000ff, },
		{ 0.0f, -0.381966011250105f, 0.0f, 0xff0000ff, },

		{ 0.0f, -0.381966011250105f, 0.0f, 0xff0000ff, },
		{ 0.0f, 0.0f, 0.0f, 0xffffffff, },
		{ -0.36327126400268f, -0.118033988749895f, 0.0f, 0xff0000ff, },

		{ -0.587785252292473f, -0.809016994374947f, 0.0f, 0xffffffff, },
		{ 0.0f, -0.381966011250105f, 0.0f, 0xff0000ff, },
		{ -0.36327126400268f, -0.118033988749895f, 0.0f, 0xff0000ff, },

		{ -0.36327126400268f, -0.118033988749895f, 0.0f, 0xff0000ff, },
		{ 0.0f, 0.0f, 0.0f, 0xffffffff, },
		{ -0.224513988289793f, 0.309016994374947f, 0.0f, 0xff0000ff, },

		{ -0.951056516295154f, 0.309016994374948f, 0.0f, 0xffffffff, },
		{ -0.36327126400268f, -0.118033988749895f, 0.0f, 0xff0000ff, },
		{ -0.224513988289793f, 0.309016994374947f, 0.0f, 0xff0000ff, },
	};

    // Create the vertex buffer.
    if( FAILED( g_pd3dDevice->CreateVertexBuffer( 30 * sizeof( CUSTOMVERTEX ),
                                                  0, D3DFVF_CUSTOMVERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB01, NULL ) ) )
    {
        return E_FAIL;
    }

	if (FAILED(g_pd3dDevice->CreateVertexBuffer(30 * sizeof(CUSTOMVERTEX),
												0, D3DFVF_CUSTOMVERTEX,
												D3DPOOL_DEFAULT, &g_pVB02, NULL)))
	{
		return E_FAIL;
	}

    // Fill the vertex buffer.
    VOID* pVertices01;
    if( FAILED( g_pVB01->Lock( 0, sizeof( g_Vertices01 ), ( void** )&pVertices01, 0 ) ) )
        return E_FAIL;
    memcpy( pVertices01, g_Vertices01, sizeof( g_Vertices01 ) );
    g_pVB01->Unlock();

	VOID* pVertices02;
	if (FAILED(g_pVB02->Lock(0, sizeof(g_Vertices02), (void**)&pVertices02, 0)))
		return E_FAIL;
	memcpy(pVertices02, g_Vertices02, sizeof(g_Vertices02));
	g_pVB02->Unlock();

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: Cleanup()
// Desc: Releases all previously initialized objects
//-----------------------------------------------------------------------------
VOID Cleanup()
{
    if( g_pVB01 != NULL )
        g_pVB01->Release();

	if (g_pVB02 != NULL)
		g_pVB02->Release();

    if( g_pd3dDevice != NULL )
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )
        g_pD3D->Release();
}



//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform Matrices.
//-----------------------------------------------------------------------------
VOID SetupMatrices01()
{
    // For our world matrix, we will just rotate the object about the y-axis.
    D3DXMATRIXA16 matWorld;

    // Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
    // every 1000 ms. To avoid the loss of precision inherent in very high 
    // floating point numbers, the system time is modulated by the rotation 
    // period before conversion to a radian angle.
    
	D3DXMatrixScaling(&matWorld, A/10, A/10, A/10 );

    g_pd3dDevice->SetTransform( D3DTS_WORLD, &matWorld );
	

    // Set up our view matrix. A view matrix can be defined given an eye point,
    // a point to lookat, and a direction for which way is up. Here, we set the
    // eye five units back along the z-axis and up three units, look at the
    // origin, and define "up" to be in the y-direction.
    D3DXVECTOR3 vEyePt( 0.0f, 3.0f,-5.0f );
    D3DXVECTOR3 vLookatPt( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vUpVec( 0.0f, 1.0f, 0.0f );
    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookatPt, &vUpVec );
    g_pd3dDevice->SetTransform( D3DTS_VIEW, &matView );

    // For the projection matrix, we set up a perspective transform (which
    // transforms geometry from 3D view space to 2D viewport space, with
    // a perspective divide making objects smaller in the distance). To build
    // a perpsective transform, we need the field of view (1/4 pi is common),
    // the aspect ratio, and the near and far clipping planes (which define at
    // what distances geometry should be no longer be rendered).
    D3DXMATRIXA16 matProj;
    D3DXMatrixPerspectiveFovLH( &matProj, D3DX_PI / 2, 1.0f, 1.0f, 100.0f );
    g_pd3dDevice->SetTransform( D3DTS_PROJECTION, &matProj );
}

VOID SetupMatrices02()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld01;
	D3DXMATRIXA16 matWorld02;
	D3DXMATRIXA16 matWorld03;

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.

	D3DXMatrixTranslation(&matWorld01, B, 0.0f, 0.0f);
	D3DXMatrixRotationX(&matWorld02, A);
	D3DXMatrixRotationY(&matWorld03, A);
	D3DXMatrixMultiply(&matWorld02, &matWorld02, &matWorld03);
	D3DXMatrixMultiply(&matWorld01, &matWorld01, &matWorld02);
	
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld01);


	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, 1.0f, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

VOID SetupMatrices03()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld01;
	D3DXMATRIXA16 matWorld02;
	D3DXMATRIXA16 matWorld03;

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.

	D3DXMatrixTranslation(&matWorld01, -B, 0.0f, 0.0f);
	D3DXMatrixRotationX(&matWorld02, A);
	D3DXMatrixRotationZ(&matWorld03, A);
	D3DXMatrixMultiply(&matWorld02, &matWorld02, &matWorld03);
	D3DXMatrixMultiply(&matWorld01, &matWorld01, &matWorld02);
	
	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld01);


	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, 1.0f, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

VOID SetupMatrices04()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld01;
	D3DXMATRIXA16 matWorld02;
	D3DXMATRIXA16 matWorld03;

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.

	D3DXMatrixTranslation(&matWorld01, 0.0f, 0.0f, B);
	D3DXMatrixRotationX(&matWorld02, -A);
	D3DXMatrixRotationZ(&matWorld03, -A);
	D3DXMatrixMultiply(&matWorld02, &matWorld02, &matWorld03);
	D3DXMatrixMultiply(&matWorld01, &matWorld01, &matWorld02);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld01);


	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, 1.0f, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

VOID SetupMatrices05()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld01;
	D3DXMATRIXA16 matWorld02;
	D3DXMATRIXA16 matWorld03;

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.

	D3DXMatrixTranslation(&matWorld01, 0.0f, 0.0f, -B );
	D3DXMatrixRotationX(&matWorld02, -A);
	D3DXMatrixRotationZ(&matWorld03, -A);
	D3DXMatrixMultiply(&matWorld02, &matWorld02, &matWorld03);
	D3DXMatrixMultiply(&matWorld01, &matWorld01, &matWorld02);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld01);


	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, 1.0f, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

VOID SetupMatrices06()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld01;
	D3DXMATRIXA16 matWorld02;
	D3DXMATRIXA16 matWorld03;

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.

	D3DXMatrixTranslation(&matWorld01, 0.0f, B, 0.0f);
	D3DXMatrixRotationY(&matWorld02, A);
	D3DXMatrixRotationZ(&matWorld03, A);
	D3DXMatrixMultiply(&matWorld02, &matWorld02, &matWorld03);
	D3DXMatrixMultiply(&matWorld01, &matWorld01, &matWorld02);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld01);


	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, 1.0f, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

VOID SetupMatrices07()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld01;
	D3DXMATRIXA16 matWorld02;
	D3DXMATRIXA16 matWorld03;

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.

	D3DXMatrixTranslation(&matWorld01, 0.0f, -B, 0.0f);
	D3DXMatrixRotationY(&matWorld02, -A);
	D3DXMatrixRotationZ(&matWorld03, -A);
	D3DXMatrixMultiply(&matWorld02, &matWorld02, &matWorld03);
	D3DXMatrixMultiply(&matWorld01, &matWorld01, &matWorld02);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld01);


	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, 1.0f, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

VOID SetupMatrices08()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld01;
	D3DXMATRIXA16 matWorld02;
	D3DXMATRIXA16 matWorld03;

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.

	D3DXMatrixTranslation(&matWorld01, 0.0f, 0.0f, -B);
	D3DXMatrixRotationY(&matWorld02, A);
	D3DXMatrixRotationZ(&matWorld03, A);
	D3DXMatrixMultiply(&matWorld02, &matWorld02, &matWorld03);
	D3DXMatrixMultiply(&matWorld01, &matWorld01, &matWorld02);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld01);


	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, 1.0f, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

VOID SetupMatrices09()
{
	// For our world matrix, we will just rotate the object about the y-axis.
	D3DXMATRIXA16 matWorld01;
	D3DXMATRIXA16 matWorld02;
	D3DXMATRIXA16 matWorld03;

	// Set up the rotation matrix to generate 1 full rotation (2*PI radians) 
	// every 1000 ms. To avoid the loss of precision inherent in very high 
	// floating point numbers, the system time is modulated by the rotation 
	// period before conversion to a radian angle.

	D3DXMatrixTranslation(&matWorld01, 0.0f, 0.0f, B);
	D3DXMatrixRotationY(&matWorld02, -A);
	D3DXMatrixRotationZ(&matWorld03, -A);
	D3DXMatrixMultiply(&matWorld02, &matWorld02, &matWorld03);
	D3DXMatrixMultiply(&matWorld01, &matWorld01, &matWorld02);

	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld01);


	// Set up our view matrix. A view matrix can be defined given an eye point,
	// a point to lookat, and a direction for which way is up. Here, we set the
	// eye five units back along the z-axis and up three units, look at the
	// origin, and define "up" to be in the y-direction.
	D3DXVECTOR3 vEyePt(0.0f, 3.0f, -5.0f);
	D3DXVECTOR3 vLookatPt(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vUpVec(0.0f, 1.0f, 0.0f);
	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &vEyePt, &vLookatPt, &vUpVec);
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);

	// For the projection matrix, we set up a perspective transform (which
	// transforms geometry from 3D view space to 2D viewport space, with
	// a perspective divide making objects smaller in the distance). To build
	// a perpsective transform, we need the field of view (1/4 pi is common),
	// the aspect ratio, and the near and far clipping planes (which define at
	// what distances geometry should be no longer be rendered).
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 2, 1.0f, 1.0f, 100.0f);
	g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: Draws the scene
//-----------------------------------------------------------------------------
VOID Render()
{
    // Clear the backbuffer to a black color
    g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB( 0, 0, 0 ), 1.0f, 0 );

    // Begin the scene
    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
        // Setup the world, view, and projection Matrices
        SetupMatrices01();
        // Render the vertex buffer contents
        g_pd3dDevice->SetStreamSource( 0, g_pVB02, 0, sizeof( CUSTOMVERTEX ) );
        g_pd3dDevice->SetFVF( D3DFVF_CUSTOMVERTEX );
        g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 10 );

		SetupMatrices02();
		// Render the vertex buffer contents
		g_pd3dDevice->SetStreamSource(0, g_pVB01, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 10);

		SetupMatrices03();
		// Render the vertex buffer contents
		g_pd3dDevice->SetStreamSource(0, g_pVB01, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 10);

		SetupMatrices04();
		// Render the vertex buffer contents
		g_pd3dDevice->SetStreamSource(0, g_pVB01, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 10);

		SetupMatrices05();
		// Render the vertex buffer contents
		g_pd3dDevice->SetStreamSource(0, g_pVB01, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 10);

		SetupMatrices06();
		// Render the vertex buffer contents
		g_pd3dDevice->SetStreamSource(0, g_pVB01, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 10);

		SetupMatrices07();
		// Render the vertex buffer contents
		g_pd3dDevice->SetStreamSource(0, g_pVB01, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 10);

		SetupMatrices08();
		// Render the vertex buffer contents
		g_pd3dDevice->SetStreamSource(0, g_pVB01, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 10);

		SetupMatrices09();
		// Render the vertex buffer contents
		g_pd3dDevice->SetStreamSource(0, g_pVB01, 0, sizeof(CUSTOMVERTEX));
		g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
		g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 10);

        // End the scene
        g_pd3dDevice->EndScene();
    }

    // Present the backbuffer contents to the display
    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}




//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
		case WM_KEYDOWN:
			A = A + 0.1f;
			B = B + 0.01f;
			return 0;
        case WM_DESTROY:
            Cleanup();
            PostQuitMessage( 0 );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
    UNREFERENCED_PARAMETER( hInst );

    // Register the window class
    WNDCLASSEX wc =
    {
        sizeof( WNDCLASSEX ), CS_CLASSDC, MsgProc, 0L, 0L,
        GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
        L"D3D Tutorial", NULL
    };
    RegisterClassEx( &wc );

    // Create the application's window
    HWND hWnd = CreateWindow( L"D3D Tutorial", L"D3D Tutorial 03: Matrices",
                              WS_OVERLAPPEDWINDOW, 100, 100, 800, 800,
                              NULL, NULL, wc.hInstance, NULL );

    // Initialize Direct3D
    if( SUCCEEDED( InitD3D( hWnd ) ) )
    {
        // Create the scene geometry
        if( SUCCEEDED( InitGeometry() ) )
        {
            // Show the window
            ShowWindow( hWnd, SW_SHOWDEFAULT );
            UpdateWindow( hWnd );

            // Enter the message loop
            MSG msg;
            ZeroMemory( &msg, sizeof( msg ) );
            while( msg.message != WM_QUIT )
            {
                if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
                {
                    TranslateMessage( &msg );
                    DispatchMessage( &msg );
                }
                else
                    Render();
            }
        }
    }

    UnregisterClass( L"D3D Tutorial", wc.hInstance );
    return 0;
}



