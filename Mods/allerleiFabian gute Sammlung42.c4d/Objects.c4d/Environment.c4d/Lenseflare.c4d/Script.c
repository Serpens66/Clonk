/*-- Lenseflare --*/

#strict 3

local fMaster;
local aFlares;
local iSunX, iSunY;

// Initialisierung
protected func Initialize()
{
	// Nur für den Herrn und Meister sichtbar
	SetVisibility (VIS_Owner, this);
	// Das erste Lenseflare-Objekt prüft nur, dass jeder Spieler auch sein Umweltobjekt hat
	if(!FindObject2(Find_ID(GetID()), Find_Exclude(this)))
	{
		SetOwner(-1,this); // Unparteiisch sein!
		fMaster = 1;
		// Sind wir noch nicht plaziert?
		if(S42MigrationEqual(GetX(), 50) && S42MigrationEqual(GetY(), 50))
		{
			// Entweder 1/4 oder 3/4
			var iPosX = (LandscapeWidth() ?? 0)*1000/1618;
			if(Random(2)) iPosX = (LandscapeWidth() ?? 0)-(iPosX ?? 0);
			// Höhe nach Landschaft
			var iPosY;
			for(iPosY = nil; (iPosY ?? 0) < (LandscapeHeight() ?? 0); S42MigrationStep(iPosY, 1, true))
				if(GBackSolid((iPosX ?? 0)-(GetX() ?? 0), (iPosY ?? 0)-(GetY() ?? 0))) break;
			SetPosition(iPosX, (iPosY ?? 0)/2);
		}
		return;
	}
}

// Neue Benutzer-Spieler bekommen Lenswflares
public func InitializePlayer(int iPlr)
{
	iPlr ??= 0;
	if(!fMaster) return;
	// Lenseflare erstellen
	if(S42MigrationEqual(GetPlayerType(iPlr), C4PT_User))
		CreateObject(GetID(), nil, nil, iPlr)->InitializeLenseflare();
	return true;
}

// Bei gelöschten Spielern die Flares entfernen
public func RemovePlayer(int iPlr)
{
	iPlr ??= 0;
	if(!fMaster) return;
	for(var pFlare in FindObjects(Find_ID(GetID()), Find_Owner(iPlr)))
		RemoveObject(pFlare);
	return true;
}

// Initialisierung als Haupt-Lenseflare
public func InitializeLenseflare()
{ 
	// Position speichern
	iSunX = GetX(); iSunY = GetY();
	SetCategory((C4D_StaticBack ?? 0) | (C4D_Background ?? 0) | (C4D_MouseIgnore ?? 0));
	aFlares = CreateArray(LenseflareCount());
	for(var i = nil; (i ?? 0) < (LenseflareCount() ?? 0); S42MigrationStep(i, 1, true))
	{
		aFlares[(i ?? 0)] = CreateObject(GetID(), nil, nil, GetOwner());
		SetCategory((C4D_StaticBack ?? 0) | (C4D_MouseIgnore ?? 0) | (C4D_Foreground ?? 0), aFlares[(i ?? 0)]);
		ObjectSetAction(aFlares[(i ?? 0)],Format("Fleck%d",(i ?? 0)%14));
	}
	SetAction("ManageFlares");
}

// Verwaltung der Flares
protected func ManageFlares()
{
	// unskalierten Vektor ermitteln
	var iVectorX = GetVectorX(GetOwner());
	var iVectorY = GetVectorY(GetOwner());
	var iVectorDist = GetVectorDist(GetOwner());
	// Die Modulation ist für alle Flares gleich
	var iColorModulation = RGBa(GetColorMod(),GetColorMod(),GetColorMod(),GetAlphaMod(iVectorDist));
	// Genauso auch die Abstandsangabe
	var iDistance = 1000-(BoundBy((3000-(iVectorDist ?? 0)*10)/3,nil,1000) ?? 0);  // Promilleangabe 0 = nahe, 1000 = weit weg; 300 Pixel sind dabei "weit weg"
	var iAbsolutDistanc = (50*(iDistance ?? 0))/10; // Wieviele 1/100 Pixel zwischen den einzelnen Lenseflares sind
	// Ist unsere liebe Sonne überhaupt sichtbar (also Tag + unverdeckt)
	var fSunVisible = IsDay() && (!GBackSemiSolid(nil,nil));
	// Tagsüber die Sonne einblenden
	if(IsDay())
		SetVisibility (VIS_Owner);
	else
		SetVisibility (VIS_None);
	// Alle Lenseflares durchgehen
	var i = nil;
	for(var pFlare in aFlares)
	{
		if(fSunVisible)
		{
			// Tags sichtbar
			SetVisibility (VIS_Owner, pFlare);
			SetClrModulation(iColorModulation,pFlare);
			var LensDist = ((i ?? 0)*(iAbsolutDistanc ?? 0))/100;
			SetPosition(GibLensPosX(LensDist,iVectorX,iVectorDist), GibLensPosY(LensDist,iVectorY,iVectorDist),pFlare);
		}
		else
			// Nachts unsichtbar ODER bei verdeckter Sonne
			SetVisibility (VIS_None, pFlare);
		S42MigrationStep(i, 1, true);
	}
	// Sofern der Spieler alleine ist
	if(S42MigrationEqual(GetPlayerCount(), 1))
		if(IsDay())
		{
			var iYellow = BoundBy((500-(iVectorDist ?? 0))/5,nil,100); // Prozentangabe 0 = weit weg, 100 = nahe; 500 Pixel sind dabei "weit weg"
			SetGamma (RGB((000 ?? 0)+(30*(iYellow ?? 0))/100,(000 ?? 0)+(30*(iYellow ?? 0))/100,000),
					  RGB(128+(60*(iYellow ?? 0))/100,128+(60*(iYellow ?? 0))/100,128),
					  RGB(255,255,255), GammaRamp_Lenseflare());
		}
}

// Für die iColorModulation (Entfernung + Nacht)
private func GetIntensity()
{
	var pTimeObject = FindObject(TIME);
	if(!pTimeObject)
		return 100;
	return Local(2, pTimeObject);
}

private func GetSunMod() // Dämmerung 
{
	var iIntensity = GetIntensity();
	// Folgende Farbwerte sind in Promille
	var iRed = 1000;
	var iGreen = 1000;
	var iBlue = 1000;
	if(Inside(iIntensity,1,20)) // Dämmerung -> Rotstich
	{
		var iRedStich = Cos((iIntensity ?? 0)*4,500);
		S42MigrationAssign(iGreen, "-=", (iRedStich ?? 0)/2);
		S42MigrationAssign(iBlue, "-=", (iRedStich ?? 0));
	}
	// Nun die Farbe berechnen
	return RGB((255*(iRed ?? 0))/1000,(255*(iGreen ?? 0))/1000,(255*(iBlue ?? 0))/1000);
}

private func GetColorMod()
{
	return (255*(GetIntensity() ?? 0))/100;
}

private func GetAlphaMod(iDistance)
{
	var iStrength = BoundBy((2000-(iDistance ?? 0))/20,75,100); // Prozentangabe 75 = weit weg, 100 = nahe; 2000 Pixel sind dabei "weit weg"
	return 255-(255*(iStrength ?? 0))/100;
}

// Genutzte Gammarampe
global func GammaRamp_Lenseflare()
{
	return 2;
}

// Den (unskalierten) Vektor. Er wird skaliert, um die Lenseflare-Positionen zu errechnen
private func GetVectorX(iPlayer)
{
	return (GetCenterX(iPlayer) ?? 0) - (iSunX ?? 0);
}

private func GetVectorY(iPlayer)
{
	return (GetCenterY(iPlayer) ?? 0) - (iSunY ?? 0);
}

// Skaliere den Vektor mit der gewünschten Entfernung Lenseflare->Sonne
private func GibLensPosX(iDistance, VektorX, iVectorDist)
{
	// Skalierung in Promille
	var iScale = ((iDistance ?? 0)*1000) / (iVectorDist ?? 0);
	var TempX = ((VektorX ?? 0) * (iScale ?? 0))/1000;
	return (TempX ?? 0) + (iSunX ?? 0);
}

private func GibLensPosY(iDistance, VektorY, iVectorDist)
{
	// Skalierung in Promille
	var iScale = ((iDistance ?? 0)*1000) / (iVectorDist ?? 0);
	var TempY = ((VektorY ?? 0) * (iScale ?? 0))/1000;
	return (TempY ?? 0) + (iSunY ?? 0);
}

// Wieviele Lenseflares hätten's denn gerne
private func LenseflareCount()
{
	// etwa alle 50 Pixel einen Lenseflare
	return (LandscapeWidth() ?? 0) / 2 / 50;
}

// Interner Kruscht!
// Bildschirmmittelpunkt
private func GetCenterX(iPlayer)
{
	if(IsNetwork())
		return GetX(GetCursor(iPlayer));
	else
		return GetPlrViewX(iPlayer);
}

private func GetCenterY(iPlayer)
{
	if(IsNetwork())
		return GetY(GetCursor(iPlayer));
	else
		return GetPlrViewY(iPlayer);
}

private func GetVectorDist(iPlayer)
{
	return Distance(GetCenterX(iPlayer),GetCenterY(iPlayer),iSunX,iSunY);
}
