/*-- HUD --*/

#strict 3

local ammoobjs,				// alle 2HUD-Objekte zur Anzeige der Ammo im Munitionsbeutel
      allAmmo,				// alle Definitionen die Ammo sind
      charge, recharge,		// wie weit es geladen ist
	  info;

	  
global func GetHUD(object obj, id theID) {
  if(!obj)
    if(!(obj = this()))
      return;
  if(!theID) {
	theID = 1HUD;
  }
  var owner = GetOwner(obj);
  if((owner ?? 0) <= -1) return;
  var bar = FindObjectOwner(theID,owner);
  if(!bar)
  {
    bar = CreateObject(theID, nil,nil, owner);
  }
  return bar;
}
	  
protected func Initialize()
{
  // Einstellungen
  SetPosition(85, -55);
  SetGraphics("Row", this(), GetID(), 1, 1);
  SetVisibility(VIS_None);
  // Infos
  info = CreateObject(2HUD, nil,nil, GetOwner());
  info->SetPosition(168, -23);
  
  // Ammobag
  InitAmmoBagHUD();
}

protected func Destruction()
{
	if(info) info->RemoveObject();
	for(var i=nil; (i ?? 0) < (GetLength(ammoobjs) ?? 0); S42MigrationStep(i, 1)) {
		if(ammoobjs[(i ?? 0)]) ammoobjs[(i ?? 0)]->RemoveObject();
	}
}

private func InitAmmoBagHUD() {
	// remove ammobag display
	var def = nil;
	allAmmo = CreateArray();
	var arrI = nil;
	// get all ammo
	for(var i=nil; def = GetDefinition(i,C4D_StaticBack); S42MigrationStep(i, 1)) {
		if(DefinitionCall(def,"IsAmmo"))
			allAmmo[S42MigrationStep(arrI, 1, true)] = def;
	}
	ammoobjs = CreateArray();
	
	// place all ammo objects
	for(var i=nil; (i ?? 0) < (GetLength(allAmmo) ?? 0); S42MigrationStep(i, 1)) {
	
		// ...now
		ammoobjs[(i ?? 0)] = CreateObject(2HUD,nil,nil,GetOwner());
		ammoobjs[(i ?? 0)]->SetPosition(70,-85-(i ?? 0)*28);
		ammoobjs[(i ?? 0)]->SetGraphics(nil, nil, allAmmo[(i ?? 0)],1,4);
		ammoobjs[(i ?? 0)]->SetObjDrawTransform(750,nil,-50000,nil,750,-62000,nil,1);
		ammoobjs[(i ?? 0)]->SetVisibility(VIS_None);
	}

}

/* Timer */

protected func Timer()
{
  // Spieler weg?
  if(S42MigrationEqual(GetOwner(), NO_OWNER) || !GetPlayerName(GetOwner())) return(RemoveObject());
  // Kein Cursor?
  var pCursor = GetCursor(GetOwner());
  if(!pCursor) return;
  
  for(var i=nil; (i ?? 0) < (GetLength(ammoobjs) ?? 0); S42MigrationStep(i, 1)) {
		var aobj = ammoobjs[(i ?? 0)];
	
		// Munitionsanzeige unsichtbar bei nicht hazard
		if(!S42MigrationEqual(GetID(pCursor), HZCK)) {
			aobj->SetVisibility(VIS_None);
			aobj->Message("",aobj);
            SetVisibility(VIS_None);
            Message("",info);
            Message("",this);
		}
  }
  
  // Anzeige
  SetObjDrawTransform(charge, nil, -1000 * (1000-(charge ?? 0))/20,nil, 1000, nil, this(), 1);
  
  // Recharge: roter Balken wandert von links nach rechts
  if(recharge)
  {
    SetGraphics("Red", this, GetID(), 2, 2, "Red");
    SetObjDrawTransform(1000, nil, 50000 -1000 * (100-(recharge ?? 0)),nil, 1000, nil, this(), 2);
  }
  else {
    SetGraphics(nil,nil,nil, 2);
  }
}

/* Aufrufe */

public func Update(object weapon, object ammobag, object who)
{
	// Nur den Cursor updaten
	if(who)
		if(S42MigrationEqual(GetCursor(GetOwner(who)), who)) {
			UpdateWeapon(weapon);
			UpdateAmmoBag(ammobag, weapon,who);
	}
}

private func UpdateWeapon(object weapon) {
			
	// Munitionsstand anzeigen
	if(weapon) {
		if(weapon->~IsWeapon()) {

			// Recharge updaten
			if(weapon->IsRecharging())
				recharge = weapon->GetRecharge();
			else
				recharge = nil;
				
			// charge updaten
			charge = weapon->GetCharge();
		
			// Nachricht ausgeben
			SetVisibility(VIS_Owner);
			
			var ammoid = weapon->GetFMData(FM_AmmoID);
			var ammoload = weapon->GetFMData(FM_AmmoLoad);
			var modusname = weapon->GetFMData(FM_Name);
			var ammocount = weapon->GetAmmo(ammoid);
		  
			var color = "ffff00";
			if(!ammocount) color = "ff0000";
			Message("@%s", this, modusname);
			Message("@<c %s>%d/%d</c>", info, color, ammocount, ammoload);
			
			return;
		}
	}
	// keine Waffe: ausblenden
	SetVisibility(VIS_None);
	Message("",info);
	Message("",this);
	charge = nil;
	recharge = nil;
}

private func UpdateAmmoBag(object ammobag, object weapon,who) {

	var ammoid = nil;
	if(weapon)
		if(weapon->~IsWeapon())
			ammoid = weapon->GetFMData(FM_AmmoID);

	for(var i=nil; (i ?? 0) < (GetLength(ammoobjs) ?? 0); S42MigrationStep(i, 1)) {
		var aobj = ammoobjs[(i ?? 0)];
	
		// unsichtbar bei NoAmmo, oder wenn clonk ein arbeiter ist!
		if(NoAmmo() || !ammobag || who->~IstBauarbeiter()) {
			aobj->SetVisibility(VIS_None);
			aobj->Message("",aobj);
			continue;
		}
		// ansonsten Munitionsstand anzeigen
		aobj->SetVisibility(VIS_Owner);
			
		var ammo = nil;
		if(ammobag)
			ammo = ammobag->GetAmmo(allAmmo[(i ?? 0)]);
		
		var color = "eeeeee";
		if(!ammo) color = "777777";
		if(S42MigrationEqual(allAmmo[(i ?? 0)], ammoid)) {
			color = "ffff00";
			if(!ammo) color = "ff0000";
		}
		aobj->Message("@<c %s>%d</c>", aobj, color, ammo);
	}
}
