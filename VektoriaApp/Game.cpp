#include "Game.h"
#include <iostream>

CGame::CGame(void)
{
}

CGame::~CGame(void)
{
}

void CGame::Init(HWND hwnd, void(*procOS)(HWND hwnd, unsigned int uWndFlags), CSplash * psplash)
{
	//---------------------------------
	// Initialisiere die Knotenobjekte:
	//---------------------------------

	// Zuallererst muss die Root mit dem Splash-Screen initialisiert werden, 
	// damit die Engine freigeschaltet wird:
	m_zr.Init(psplash);	// muss immer als erstes stehen

	// Hier kommen alle weiteren Initialisierungen hinein: 

	m_zf.Init(hwnd, procOS);
	m_zf.SetColorBackground(CColor(0.1f, 0.1f, 1.0f));
	m_zc.Init(0.4f);
	m_zv.InitFull(&m_zc);	// Viewport über ganze Größe initialisieren
	m_zo.InitFull(&m_zm);
	m_zlp.Init(CHVector(-1, -1, -1));	// Licht initialisieren
	m_zb.InitFull("textures//stars_image.jpg");


	// Vernetzung:
	m_zr.AddFrame(&m_zf);	// Frame an Root hängen
	m_zf.AddViewport(&m_zv);	// Viewport an Frame hängen
	//m_zv.AddOverlay(&m_zo);	// Overlay an Viewport hängen
	m_zv.AddBackground(&m_zb);
	m_zr.AddScene(&m_zs);	// Szene an Root hängen
	m_zs.AddLightParallel(&m_zlp);
	m_zs.AddPlacement(&m_zpCamera);	// Placement an Szene hängen
	m_zpCamera.AddCamera(&m_zc);	// Kamera an Placement hängen

	//m_zv.SetWireframeOn();	// Wireframe-Modus anschalten
	m_zpCamera.TranslateZ(10);
		
	// Erde
	m_zgEarth.Init(1.0, &m_zm, 36, 24);	// 1.0 = 1m Radius, Material, longitude: Vertices um Äquator, lattitude: Vertices vom Nordpol zum Südpol
	m_zpEarth.AddGeo(&m_zgEarth);
	m_zm.MakeTextureDiffuse("textures//earth_image.jpg");
	m_zm.MakeTextureSpecular("textures//earth_reflection.jpg");	// ist überall hell, wo Glanzlichter sein sollen
	m_zm.SetSpecularStrength(0.5f);	// Glanz runterstellen
	m_zm.MakeTextureBump("textures//earth_bump.jpg"); // Höhen und Tiefen simulieren durch Normals --> blau = tief, rot = hoch --> rot zeigt Tangente, grün zeigt Bitangente, blau zeigt Normal --> durch RGB-Kanal Bumpmap darstellen
	m_zm.MakeTextureGlow("textures//earth_glow.jpg");	// Leuchtet nur dort wo dunkel
	m_zpEarth.AddGeo(&m_zgEarth);

	// Wolken
	m_zpEarth.AddPlacement(&m_zpClouds);
	m_zpClouds.AddGeo(&m_zgClouds);
	m_zgClouds.Init(1.05f, &m_zmClouds, 36, 24);
	m_zmClouds.MakeTextureDiffuse("textures//clouds_alpha_l1.gif");
	m_zmClouds.SetTransparencyOn();

	// Tastatur
	m_zf.AddDeviceKeyboard(&m_zdk);

	// Buttons
	m_zoButtonSinglePlayer.Init(&m_zmSingleplayer, C2dRect(0.1f, 0.2f, 0.87f, 0.5f));
	m_zoButtonMultiPlayer.Init(&m_zmMultiplayer, C2dRect(0.1f, 0.2f, 0.7f, 0.5f));
	m_zmSingleplayer.MakeTextureDiffuse("textures//singlebutton.png");
	m_zmMultiplayer.MakeTextureDiffuse("textures//multibutton.png");
	m_zv.AddOverlay(&m_zoButtonSinglePlayer);
	m_zv.AddOverlay(&m_zoButtonMultiPlayer);
	m_zoButtonMultiPlayer.SetLayer(0.15f);
	m_zoButtonSinglePlayer.SetLayer(0.15f);
	m_zosButtons.Add(&m_zoButtonMultiPlayer);
	m_zosButtons.Add(&m_zoButtonSinglePlayer);

	// Startbildschirm
	m_zoStartbildschirm.InitFull(&m_zmStartbildschirm);
	m_zmStartbildschirm.MakeTextureDiffuse("textures//startbildschirm.bmp");
	m_zv.AddOverlay(&m_zoStartbildschirm);
	m_zoStartbildschirm.SetLayer(0.2f);

	// Cursor
	m_ziCursorCat.Init("textures//cat.png");
	m_zoCatCursor.Init(&m_ziCursorCat, C2dRect(0.1f, 0.2f), true);
	m_ziCursorPaw.Init("textures//paw.png");
	m_zoPawCursor.Init(&m_ziCursorPaw, C2dRect(0.1f, 0.2f), true);
	m_zf.AddDeviceCursor(&m_zCursor);
	m_zv.AddOverlay(&m_zoCatCursor);
	m_zv.AddOverlay(&m_zoPawCursor);
	m_zoCatCursor.SetLayer(0.01f);
	m_zoPawCursor.SetLayer(0.01f);

	// Himmel
	m_zgSky.Init(10000, &m_zmSky);
	m_zgSky.Flip();
	m_zpSky.AddGeo(&m_zgSky);
	m_zs.AddPlacement(&m_zpSky);	// Textur liegt außen auf Sphere
	//m_zmSky.LoadPreset("NasaStars");
	m_zmSky.MakeTextureSky("textures//stars_image.jpg");

	// Nebel
	m_zmNebel.MakeTextureBillboard("textures//nebula.tif");
	m_zmNebel.SetTransparencyOn();
	m_zgNebel.Init(1.0f, &m_zmNebel);
	m_zs.AddPlacement(&m_azpNebel[0]);
	m_azpNebel[0].SetBillboard();
	m_azpNebel[0].TranslateY(7.0f);
	for (int i = 1; i < 5; i++) {	// 0ter Nebel das Überplacement, die anderen hängen daran
		m_azpNebel[0].AddPlacement(&m_azpNebel[i]);
		m_azpNebel[i].AddGeo(&m_zgNebel);
		m_azpNebel[i].TranslateZ(i / 5.0f);
	}

	// Sonne
	m_zs.AddPlacement(&m_zpSun);
	m_zpSun.AddPlacement(&m_zpEMS);
	m_zpSun.AddGeo(&m_zgSun);
	m_zmSun.MakeTextureDiffuse("textures//sun_image.jpg");
	m_zgSun.Init(3.0f, &m_zmSun);

	// Asteroidengürtel um Sonne
	m_zmAsteroid.MakeTextureBillboard("textures//asteroid_image_0.bmp");
	m_zmAsteroid.SetChromaKeyingOn();
	m_zgAsteroid.Init(0.1f, &m_zmAsteroid);

	for (int i = 0; i < 500; i++) {
		m_zpSun.AddPlacement(&m_azpAsteroids[i]);
		m_azpAsteroids[i].SetBillboard(eBillboardType_Parallel);
		m_azpAsteroids[i].AddGeo(&m_zgAsteroid);
		m_avAsteroidRandom[i] = CHVector(
			rand() % 1000 / 1000.0f + 3.5f,
			(rand() % 1000 / 1000.0f - 0.5f) * 0.8f,
			0,
			rand() % 1000 / 1000.0f
		);
	}

	// Mond
	m_zpMoon.AddGeo(&m_zgMoon);
	m_zgMoon.Init(0.4f, &m_zmMoon);
	m_zmMoon.MakeTextureDiffuse("textures//Moon_Image.jpg");
	m_zmMoon.MakeTextureBump("textures//moon_bump.jpg");

	// EMS
	m_zpEMS.AddPlacement(&m_zpMoon);
	m_zpEMS.AddPlacement(&m_zpEarth);

	// Rakete
	m_zpEarth.AddPlacement(&m_zpRocket);
	m_zpRocket.AddPlacement(&m_zpCube);
	m_zpRocket.AddPlacement(&m_zpCone);
	m_zgCube.Init(0.2f, &m_zmSky);
	m_zpRocket.AddGeo(&m_zgCube);
	m_zgCone.Init(0.1f, 0.5f, &m_zmSky);
	m_zpRocket.AddGeo(&m_zgCone);
	m_zpRocket.RotateX(90);

	// Rakete Übung 5
	m_zpRocket5.AddPlacement(&m_zpCubeRocket5);
	m_zpRocket5.AddPlacement(&m_zpConeRocket5);
	m_zgCubeRocket5.Init(0.1f, &m_zmDeathstar);
	m_zpRocket5.AddGeo(&m_zgCubeRocket5);
	m_zgConeRocket5.Init(0.5f, 0.1f, &m_zmDeathstar);
	m_zpRocket5.AddGeo(&m_zgConeRocket5);
	m_zpRocket5.TranslateZ(-5.0f);
	m_zpRocket5.TranslateYDelta(-0.5f);
	m_zpCamera.AddPlacement(&m_zpRocket5);
	m_zpRocket5.SwitchOff();


	// Template Bullet erzeugen
	m_zmBullet.MakeTextureDiffuse("textures//red_image.jpg");
	m_zgBullet.Init(0.1f, &m_zmBullet);
	m_zpBulletTemplate.AddGeo(&m_zgBullet);
	m_zpBulletTemplate.SetPhysics(2.0f, 0.1f, 0.0f, 200000.1f, true);
	m_zpBulletTemplate.SwitchOff();
	// Template an Szene hängen
	m_zs.AddPlacement(&m_zpBulletTemplate);

	//m_zs.SetPhysicsSpace();

	// Ring für Bullets erzeugen
	m_zpBullets.RingMake(15, m_zpBulletTemplate);
	m_zs.AddPlacements(m_zpBullets);

	for (int i = 0; i < 15; i++) {
		m_zs.AddAudio(&m_azaShootSound[i]);
		m_azaShootSound[i].Init("sounds//Gun.wav");
	}

	m_zs.AddAudio(&m_ExplosionSound);
	m_ExplosionSound.Init("sounds//Explosion.wav");

	m_zpBulletsDeathstar.RingMake(2, m_zpBulletTemplate);
	m_zs.AddPlacements(m_zpBulletsDeathstar);

	// Deathstar Material
	m_zmDeathstar.MakeTextureDiffuse("textures/deathstar_image.jpg");
	m_zmDeathstar.MakeTextureBump("textures/deathstar_bump.jpg");
	m_zmRedMaterial.MakeTextureDiffuse("textures/deathstar_image_red.png");

	// Splitscreen
	m_zcPlayerOne.Init(0.4f);
	m_zcPlayerTwo.Init(0.4f);

	m_zf.AddViewport(&m_zvPlayerOne);
	m_zf.AddViewport(&m_zvPlayerTwo);

	m_zpRocketPlayerOne.AddPlacement(&m_zpCubeRocketPlayerOne);
	m_zpRocketPlayerOne.AddPlacement(&m_zpConeRocketPlayerOne);
	m_zgCubeRocketPlayerOne.Init(0.1f, &m_zmDeathstar);
	m_zpRocketPlayerOne.AddGeo(&m_zgCubeRocketPlayerOne);
	m_zgConeRocketPlayerOne.Init(0.5f, 0.1f, &m_zmDeathstar);
	m_zpRocketPlayerOne.AddGeo(&m_zgConeRocketPlayerOne);
	m_zpRocketPlayerOne.TranslateZ(-5.0f);
	m_zpRocketPlayerOne.TranslateYDelta(-0.5f);

	m_zpRocketPlayerTwo.AddPlacement(&m_zpCubeRocketPlayerTwo);
	m_zpRocketPlayerTwo.AddPlacement(&m_zpConeRocketPlayerTwo);
	m_zgCubeRocketPlayerTwo.Init(0.1f, &m_zmRedMaterial);
	m_zpRocketPlayerTwo.AddGeo(&m_zgCubeRocketPlayerTwo);
	m_zgConeRocketPlayerTwo.Init(0.5f, 0.1f, &m_zmRedMaterial);
	m_zpRocketPlayerTwo.AddGeo(&m_zgConeRocketPlayerTwo);
	m_zpRocketPlayerTwo.TranslateZ(-5.0f);
	m_zpRocketPlayerTwo.TranslateYDelta(-0.5f);

	m_zvPlayerOne.InitHalfLeft(&m_zcPlayerOne);
	m_zvPlayerTwo.InitHalfRight(&m_zcPlayerTwo);

	m_zpCameraPlayerOne.AddCamera(&m_zcPlayerOne);
	m_zpCameraPlayerTwo.AddCamera(&m_zcPlayerTwo);

	m_zpRocketPlayerOne.SwitchOff();
	m_zpRocketPlayerTwo.SwitchOff();

	m_zs.AddPlacement(&m_zpCameraPlayerOne);
	m_zs.AddPlacement(&m_zpCameraPlayerTwo);
	m_zvPlayerOne.SwitchOff();
	m_zvPlayerTwo.SwitchOff();

	m_zpCameraPlayerOne.TranslateZ(10);
	m_zpCameraPlayerTwo.TranslateZ(10);

	m_zpCameraPlayerOne.AddPlacement(&m_zpRocketPlayerOne);
	m_zpCameraPlayerOne.TranslateXDelta(-2.0f);
	m_zpCameraPlayerTwo.AddPlacement(&m_zpRocketPlayerTwo);
	m_zpCameraPlayerTwo.TranslateXDelta(2.0f);

	m_zpCameraPlayerOne.SetTranslationSensitivity(50);
	m_zpCameraPlayerTwo.SetTranslationSensitivity(50);

	// Deathstars
	//m_zpEarth.AddPlacement(&m_zpDeathstar);
	m_zs.AddPlacement(&m_zpDeathstar);
	m_zpDeathstar.AddGeo(&m_zgDeathstar);
	m_zgDeathstar.Init(0.8f, &m_zmDeathstar);
	m_zgPick.Add(&m_zgDeathstar);
	m_zagDeathstars->Add(&m_zgDeathstar);

	m_zpEarth.AddPlacement(&m_zpDeathstar2);
	m_zpDeathstar2.AddGeo(&m_zgDeathstar2);
	m_zgDeathstar2.Init(0.4f, &m_zmDeathstar);
	m_zgPick.Add(&m_zgDeathstar2);
	m_zagDeathstars->Add(&m_zgDeathstar2);

	m_zpEarth.AddPlacement(&m_zpDeathstar3);
	m_zpDeathstar3.AddGeo(&m_zgDeathstar3);
	m_zgDeathstar3.Init(0.4f, &m_zmDeathstar);
	m_zgPick.Add(&m_zgDeathstar3);
	m_zagDeathstars->Add(&m_zgDeathstar3);

	m_zpEarth.AddPlacement(&m_zpDeathstar4);
	m_zpDeathstar4.AddGeo(&m_zgDeathstar4);
	m_zgDeathstar4.Init(0.4f, &m_zmDeathstar);
	m_zgPick.Add(&m_zgDeathstar4);
	m_zagDeathstars->Add(&m_zgDeathstar4);

	m_zpEarth.AddPlacement(&m_zpDeathstar5);
	m_zpDeathstar5.AddGeo(&m_zgDeathstar5);
	m_zgDeathstar5.Init(0.4f, &m_zmDeathstar);
	m_zgPick.Add(&m_zgDeathstar5);
	m_zagDeathstars->Add(&m_zgDeathstar5);

	m_zpEarth.AddPlacement(&m_zpDeathstar6);
	m_zpDeathstar6.AddGeo(&m_zgDeathstar6);
	m_zgDeathstar6.Init(0.4f, &m_zmDeathstar);
	m_zgPick.Add(&m_zgDeathstar6);
	m_zagDeathstars->Add(&m_zgDeathstar6);

	m_zpEarth.AddPlacement(&m_zpDeathstar7);
	m_zpDeathstar7.AddGeo(&m_zgDeathstar7);
	m_zgDeathstar7.Init(0.4f, &m_zmDeathstar);
	m_zgPick.Add(&m_zgDeathstar7);
	m_zagDeathstars->Add(&m_zgDeathstar7);

	m_zpEarth.AddPlacement(&m_zpDeathstar8);
	m_zpDeathstar8.AddGeo(&m_zgDeathstar8);
	m_zgDeathstar8.Init(0.4f, &m_zmDeathstar);
	m_zgPick.Add(&m_zgDeathstar8);
	m_zagDeathstars->Add(&m_zgDeathstar8);

	m_zpEarth.AddPlacement(&m_zpDeathstar9);
	m_zpDeathstar9.AddGeo(&m_zgDeathstar9);
	m_zgDeathstar9.Init(0.4f, &m_zmDeathstar);
	m_zgPick.Add(&m_zgDeathstar9);
	m_zagDeathstars->Add(&m_zgDeathstar9);

	m_zpEarth.AddPlacement(&m_zpDeathstar10);
	m_zpDeathstar10.AddGeo(&m_zgDeathstar10);
	m_zgDeathstar10.Init(0.4f, &m_zmDeathstar);
	m_zgPick.Add(&m_zgDeathstar10);
	m_zagDeathstars->Add(&m_zgDeathstar10);

	m_zpEarth.AddPlacement(&m_zpDeathstar11);
	m_zpDeathstar11.AddGeo(&m_zgDeathstar11);
	m_zgDeathstar11.Init(0.4f, &m_zmDeathstar);
	m_zgPick.Add(&m_zgDeathstar11);
	m_zagDeathstars->Add(&m_zgDeathstar11);

	m_zpCamera.SetTranslationSensitivity(50);	// Standardmäßig auf eine Einheit pro Sekunde, hier 100 Einheiten pro Sekunde
}

void CGame::Tick(float fTime, float fTimeDelta)
{
	//-------------------------------------------
	// Veränderungen während der Simulationszeit:
	//-------------------------------------------

	// Hier kommen die Veränderungen pro Renderschritt hinein: 
	if (m_bMultiplayer == false) {
		m_zdk.PlaceWASD(m_zpCamera, fTimeDelta, false);
		m_zpRocket5.SwitchOn();
	}
	else {
		m_zdk.PlaceWASD(m_zpCameraPlayerOne, fTimeDelta, false);
		m_zpRocket5.SwitchOff();
		m_zpRocketPlayerOne.SwitchOn();
		m_zpRocketPlayerTwo.SwitchOn();
	}
	
	COverlay* oPickedOverlay = m_zCursor.PickOverlayPreselected(m_zosButtons);
	if (oPickedOverlay == &m_zoButtonSinglePlayer && m_zCursor.ButtonPressed(0)) {
		m_bMultiplayer = false;
		m_zoStartbildschirm.SwitchOff();
		m_zoButtonSinglePlayer.SwitchOff();
		m_zoButtonMultiPlayer.SwitchOff();
	}
	else if (oPickedOverlay == &m_zoButtonMultiPlayer && m_zCursor.ButtonPressed(0)) {
		m_bMultiplayer = true;
		m_zoStartbildschirm.SwitchOff();
		m_zoButtonSinglePlayer.SwitchOff();
		m_zoButtonMultiPlayer.SwitchOff();
	}

	if (m_bMultiplayer == true) {
		m_zvPlayerOne.SwitchOn();
		m_zvPlayerTwo.SwitchOn();
		m_zv.SwitchOff();
	}

	if(m_zdk.KeyPressed(DIK_RETURN)) {
		m_zoStartbildschirm.SwitchOff();
		m_zoButtonSinglePlayer.SwitchOff();
		m_zoButtonMultiPlayer.SwitchOff();
	}

	// Bullets
	if (m_zdk.KeyDown(DIK_SPACE)) {
		// Bullets schießen
		if (m_zpBullets.RingIsNotFull()) {
			CPlacement* pzp_BulletInRing = m_zpBullets.RingInc();
			CHVector vector;
			vector.Normal();
			if (m_bMultiplayer == true) {
				pzp_BulletInRing->SetMat(m_zpRocketPlayerOne.GetMatGlobal()); 
				vector = m_zpCameraPlayerOne.GetDirectionGlobal() + CHVector(0.0f, 0.08f, 0.0f);
			}
			else {
				pzp_BulletInRing->SetMat(m_zpRocket5.GetMatGlobal()); // Matrix des Placements der Rakete holen und bei Bullet setzen --> Bullets haben dann die gleiche Startposition wie die Rakete
				vector = m_zpCamera.GetDirectionGlobal() + CHVector(0.0f, 0.08f, 0.0f);
			}
			pzp_BulletInRing->SetPhysicsVelocity(vector * 50.0f);
			m_azaShootSound[m_zpBullets.m_uRingStart].Start();
		}
	}

	CPlacement* pzp_LastBullet = m_zpBullets.RingAskLast();

	if (pzp_LastBullet) {
		if (pzp_LastBullet->GetPos().Length() > 100.0f) {
			m_zpBullets.RingDec();
		}
	}

	// Positionen meiner Bullets speichern
	for (int i = 0; i < 15; i++) {
		if (m_zpBullets.m_applacement[i]->IsOn()) { // nur den aktiven Teil der Bullets betrachten
			m_zavBulletsLastPos[i] = m_zpBullets.m_applacement[i]->GetPosGlobal(); // Position der letzten Bullets merken
		}
	}

	if (isDeathstarAlive == true) {
		if (m_zpBulletsDeathstar.RingIsNotFull()) {
			CPlacement* pzp_DeathstarBulletInRing = m_zpBulletsDeathstar.RingInc();
			pzp_DeathstarBulletInRing->SetMat(m_zpDeathstar.GetMatGlobal());

			CHVector vector;
			if (m_bMultiplayer == true) {
				vector = m_zpCameraPlayerOne.GetPosGlobal() - m_zpDeathstar.GetPosGlobal();
			}
			else {
				vector = m_zpCamera.GetPosGlobal() - m_zpDeathstar.GetPosGlobal();
			}
			vector += CHVector(0.0f, 0.0f, 2.0f, 0.0f);
			pzp_DeathstarBulletInRing->SetPhysicsVelocity(vector * 2.0f);

		}

		CPlacement* pzp_LastBulletDeathstar = m_zpBulletsDeathstar.RingAskLast();

		if (pzp_LastBulletDeathstar) {
			if (pzp_LastBulletDeathstar->GetPos().Length() > 100.0f) {
				m_zpBulletsDeathstar.RingDec();
			}
		}
	}


	// Cursor
	float fx, fy;
	m_zCursor.GetFractional(fx, fy, true);
	if (m_zCursor.ButtonPressedLeft()) {
		m_zoPawCursor.SwitchOff();
		m_zoCatCursor.SwitchOn();
		m_zoCatCursor.SetPos(fx - 0.01f, fy-0.01f);
	}
	else {
		m_zoCatCursor.SwitchOff();
		m_zoPawCursor.SwitchOn();
		m_zoPawCursor.SetPos(fx - 0.01f, fy - 0.01f);
	}

	// Asteroidenring
	for (int i = 0; i < 500; i++)
	{
		m_azpAsteroids[i].Translate(m_avAsteroidRandom[i]);
		m_azpAsteroids[i].RotateYDelta(m_avAsteroidRandom[i].w * fTime);
	}

	// Nebel
	for (int i = 0; i < 5; i++) {
		m_azpNebel[i].SetBillboardAngle(fTime * i * 0.05f);
		m_azpNebel[i].SetBillboardScaling((1.5f + sinf(fTime * i * 0.7f), (1.5f + sinf(fTime * i * 0.7f), (1.0f + sinf(fTime * i * 0.7f)))));
	}

	CGeo* gPickedGeo = m_zCursor.PickGeoPreselected(m_zgPick);
	m_zgDeathstar.SetMaterial(&m_zmRedMaterial);	// Material muss an Root ran
	if (gPickedGeo != nullptr && m_zCursor.ButtonPressedLeft()) {
		gPickedGeo->SetMaterial(&m_zmRedMaterial);
	}
	
	m_zpEarth.RotateY(fTime);	// Rotation um Y-Achse, fTime ist Zeit die seit dem Beginn des Spieles vergangen ist
	m_zpEarth.RotateZDelta(UM_DEG2RAD(24));	// Reihenfolge der Rotation ist relevant

	m_zpClouds.RotateY(fTime*0.125f);	// kleiner Verschub zur Erdrotation

	m_zpMoon.TranslateX(5);
	m_zpMoon.RotateYDelta(fTime*0.5f);

	m_zpSun.RotateY(fTime*0.125f);

	m_zpEMS.TranslateX(25);

	m_zpRocket.TranslateX(2);
	m_zpRocket.RotateZDelta(fTime*2);

	// Deathstars
	m_zpDeathstar.TranslateX(10);
	//m_zpDeathstar.RotateZDelta(fTime*0.5f);
	//m_zpDeathstar.RotateYDelta(fTime);

	m_zpDeathstar2.TranslateX(2);
	m_zpDeathstar2.RotateYDelta(fTime);
	m_zpDeathstar2.RotateZDelta(fTime*0.3f);

	m_zpDeathstar3.TranslateX(3);
	m_zpDeathstar3.RotateXDelta(fTime*0.2f);
	m_zpDeathstar3.RotateYDelta(fTime);

	m_zpDeathstar4.TranslateZ(4);
	m_zpDeathstar4.RotateXDelta(fTime * 2);
	m_zpDeathstar4.RotateYDelta(fTime);

	m_zpDeathstar5.TranslateY(5);
	m_zpDeathstar5.RotateYDelta(fTime);
	m_zpDeathstar5.RotateZDelta(fTime);
	m_zpDeathstar5.RotateXDelta(fTime*4);

	m_zpDeathstar6.TranslateX(7);
	m_zpDeathstar6.RotateXDelta(fTime);
	m_zpDeathstar6.RotateZDelta(fTime * 2);

	m_zpDeathstar7.TranslateZ(2);
	m_zpDeathstar7.RotateYDelta(fTime);
	m_zpDeathstar7.RotateZDelta(fTime * 6);

	m_zpDeathstar8.TranslateY(3);
	m_zpDeathstar8.RotateZDelta(fTime);
	m_zpDeathstar8.RotateYDelta(fTime);

	m_zpDeathstar9.TranslateX(4);
	m_zpDeathstar9.RotateYDelta(fTime * 2);

	m_zpDeathstar10.TranslateZ(5);
	m_zpDeathstar10.RotateXDelta(fTime);
	m_zpDeathstar10.RotateZDelta(fTime);
	m_zpDeathstar10.RotateYDelta(fTime * 10);

	m_zpDeathstar11.TranslateY(6);
	m_zpDeathstar11.RotateZDelta(fTime * 2);

	// Traversiere am Schluss den Szenengraf und rendere:
	m_zr.Tick(fTimeDelta);

	// Prüfen, ob Bullets etwas getroffen haben
// nach Tick berechnen, weil in Tick die Physik berechnet wird

	for (int i = 0; i < 15; i++) {
		if (m_zpBullets.m_applacement[i]->IsOn()) { // nur den aktiven Teil der Bullets betrachten
			CRay r;
			r.InitFromTo(m_zavBulletsLastPos[i], m_zpBullets.m_applacement[i]->GetPosGlobal()); // Strahl von Punkt A bis Punkt B

			CHitPoint hitpoint;

			if (m_zgDeathstar.Intersects(r, hitpoint)) {
				m_zgDeathstar.SwitchOff();
				isDeathstarAlive = false;
				m_ExplosionSound.Start();
			}
		}
	}
}

void CGame::Fini()
{
	//-------------------------------
	// Finalisiere die Knotenobjekte:
	//-------------------------------

	// Hier die Finalisierung Deiner Vektoria-Objekte einfügen:

	// Auch die Root sollte finalisiert werden:   
	m_zr.Fini();
}

void CGame::WindowReSize(int iNewWidth, int iNewHeight)
{
	// Windows ReSize wird immer automatisch aufgerufen, wenn die Fenstergröße verändert wurde.
	// Hier kannst Du dann die Auflösung des Viewports neu einstellen:
	// z.B. mittels: m_zf.ReSize(iNewWidth, iNewHeight);
}

float CGame::GetTimeDeltaMin()
{
	return m_zr.GetTimeDeltaMin();
}

float CGame::GetVersion()
{
	return m_zr.GetVersion();
}


