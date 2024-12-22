#pragma once
//-----------------------------------------------------------------------
// CGame
// 
// Erklärung : Eine weitestgehend leere Klasse, 
//             sinnfällig in eine Windows-Applikation eingebettet,
//             um ein Spiel bzw. eine 3D-Simulation zu erzeugen
// Autor     : Prof. Dr. Tobias Breiner
// Ort       : Pfungstadt
// Zeit      : seit Aug. 2011 (mit seitdem ständigen Verbesserungs-Updates)
// Copyright : Tobias Breiner  
// Disclaimer: Nutzung auf eigene Gefahr, keine Gewähr, no warranty!
//------------------------------------------------------------------------


#ifdef _WIN64
#ifdef _DEBUG
#pragma comment (lib, "Vektoria_Debug64.lib")
#pragma comment (lib, "VektoriaMath_Debug64.lib")
#else
#pragma comment (lib, "Vektoria_Release64.lib")
#pragma comment (lib, "VektoriaMath_Release64.lib")
#endif
#else
#ifdef _DEBUG
#pragma comment (lib, "Vektoria_Debug.lib")
#pragma comment (lib, "VektoriaMath_Debug.lib")
#else
#pragma comment (lib, "Vektoria_Release.lib")
#pragma comment (lib, "VektoriaMath_Release.lib")
#endif
#endif


#include "Vektoria\Root.h"

using namespace Vektoria;


class CGame
{
public:
	// Wird vor Begin einmal aufgerufen (Konstruktor):
	CGame(void);																				

	// Wird nach Ende einmal aufgerufen (Destruktor):
	~CGame(void);																				

	// Wird zu Begin einmal aufgerufen:
	void Init(HWND hwnd, void(*procOS)(HWND hwnd, unsigned int uWndFlags), CSplash * psplash);	

	// Wird während der Laufzeit bei jedem Bildaufbau aufgerufen:
	void Tick(float fTime, float fTimeDelta);						

	// Wird am Ende einmal aufgerufen:
	void Fini();																				

	// Wird immer dann aufgerufen, wenn der Benutzer die Fenstergröße verändert hat:
	void WindowReSize(int iNewWidth, int iNewHeight);											

	// Holt das minimale Zeitdelta zur eventuellen Frame-Rate-Beschränkung:
	float GetTimeDeltaMin();																	

	// Holt die Versionsnummer:
	float GetVersion();


private:

	// Member_ZockKlasse --> m_zr

	// Die Wurzel des Szenengrafen: 
	CRoot m_zr; 

	// Hier ist Platz für Deine weiteren Vektoriaobjekte:	--> Knotenobjekte des Szenengraphen
	CCamera m_zc; // Kamera

	CFrame m_zf; // Fenster, in das wir reinrendern
	
	CViewport m_zv;
	
	COverlay m_zo;

	CScene m_zs; // 3D-Raum/Szene
	
	CPlacement m_zpCamera;


	CLightParallel m_zlp; // Licht

	CBackground m_zb; // Gegenteil vom Overlay, ist ein Objekt, das immer hinter der Szene ist --> vgl. Overlay: ist immer über Szene

	CDeviceKeyboard m_zdk;

	// Cursor
	CDeviceCursor m_zCursor;
	CImage m_ziCursorCat;
	COverlay m_zoCatCursor;
	CImage m_ziCursorPaw;
	COverlay m_zoPawCursor;

	CGeos m_zgPick;
	CMaterial m_zmRedMaterial;

	// Startbildschirm
	COverlay m_zoStartbildschirm;
	CMaterial m_zmStartbildschirm;

	// Buttons
	COverlay m_zoButtonSinglePlayer;
	COverlay m_zoButtonMultiPlayer;
	CMaterial m_zmSingleplayer;
	CMaterial m_zmMultiplayer;
	COverlays m_zosButtons;
	bool m_bMultiplayer = false;

	// Splitscreen
	CViewport m_zvPlayerOne;
	CViewport m_zvPlayerTwo;
	CCamera m_zcPlayerOne;
	CCamera m_zcPlayerTwo;
	CPlacement m_zpCameraPlayerOne;
	CPlacement m_zpCameraPlayerTwo;
	CPlacement m_zpRocketPlayerOne;
	CPlacement m_zpRocketPlayerTwo;

	CGeoCube m_zgCubeRocketPlayerOne;
	CPlacement m_zpCubeRocketPlayerOne;
	CGeoCone m_zgConeRocketPlayerOne;
	CPlacement m_zpConeRocketPlayerOne;

	CGeoCube m_zgCubeRocketPlayerTwo;
	CPlacement m_zpCubeRocketPlayerTwo;
	CGeoCone m_zgConeRocketPlayerTwo;
	CPlacement m_zpConeRocketPlayerTwo;

	// Rakete Übung 5
	CPlacement m_zpRocket5;
	CGeoCube m_zgCubeRocket5;
	CPlacement m_zpCubeRocket5;
	CGeoCone m_zgConeRocket5;
	CPlacement m_zpConeRocket5;

	// Meine Bullets
	CPlacements m_zpBullets;
	CPlacement m_zpBulletTemplate;
	CGeoSphere m_zgBullet;
	CMaterial m_zmBullet;
	CHVector m_zavBulletsLastPos[15];

	// Bullets für Deathstar
	CPlacements m_zpBulletsDeathstar;
	bool isDeathstarAlive = true;

	// Erde
	CMaterial m_zm;
	CGeoSphere m_zgEarth;
	CPlacement m_zpEarth; // Platzierungsobjekte um die Objekte zu platzieren

	// Wolken
	CPlacement m_zpClouds;
	CGeoSphere m_zgClouds;
	CMaterial m_zmClouds;

	// Nebel
	CPlacement m_azpNebel[5];
	CGeoQuad m_zgNebel;
	CMaterial m_zmNebel;

	// Himmel
	CGeoSphere m_zgSky;
	CMaterial m_zmSky;
	CPlacement m_zpSky;

	// Sonne
	CPlacement m_zpSun;
	CGeoSphere m_zgSun;
	CMaterial m_zmSun;

	// Asteroidengürtel um Sonne
	CPlacement m_azpAsteroids[500];
	CGeoQuad m_zgAsteroid;
	CMaterial m_zmAsteroid;
	CHVector m_avAsteroidRandom[500];

	// Mond
	CPlacement m_zpMoon;
	CGeoSphere m_zgMoon;
	CMaterial m_zmMoon;

	// EMS
	CPlacement m_zpEMS; // EMS = Earth-Moon-System

	// Rakete
	CPlacement m_zpRocket;
	CGeoCube m_zgCube;
	CPlacement m_zpCube;
	CGeoCone m_zgCone;
	CPlacement m_zpCone;

	CGeos m_zagDeathstars[11];

	// Audio
	CAudio m_azaShootSound[15];
	CAudio m_ExplosionSound;

	// Todesstern 1
	CPlacement m_zpDeathstar;
	CGeoSphere m_zgDeathstar;
	CMaterial m_zmDeathstar;

	// Todesstern 2
	CPlacement m_zpDeathstar2;
	CGeoSphere m_zgDeathstar2;

	// Todesstern 3
	CPlacement m_zpDeathstar3;
	CGeoSphere m_zgDeathstar3;

	// Todesstern 4
	CPlacement m_zpDeathstar4;
	CGeoSphere m_zgDeathstar4;

	// Todesstern 5
	CPlacement m_zpDeathstar5;
	CGeoSphere m_zgDeathstar5;

	// Todesstern 6
	CPlacement m_zpDeathstar6;
	CGeoSphere m_zgDeathstar6;

	// Todesstern 7
	CPlacement m_zpDeathstar7;
	CGeoSphere m_zgDeathstar7;

	// Todesstern 8
	CPlacement m_zpDeathstar8;
	CGeoSphere m_zgDeathstar8;

	// Todesstern 9
	CPlacement m_zpDeathstar9;
	CGeoSphere m_zgDeathstar9;

	// Todesstern 10
	CPlacement m_zpDeathstar10;
	CGeoSphere m_zgDeathstar10;

	// Todesstern 11
	CPlacement m_zpDeathstar11;
	CGeoSphere m_zgDeathstar11;
};


