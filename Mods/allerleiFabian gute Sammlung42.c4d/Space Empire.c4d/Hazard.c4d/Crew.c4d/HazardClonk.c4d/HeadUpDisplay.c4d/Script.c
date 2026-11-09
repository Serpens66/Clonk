/*-- HUD --*/

#strict 2

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
  if(owner <= -1) return;
  var bar = FindObjectOwner(theID,owner);
  if(!bar)
  {
    bar = CreateObject(theID, 0,0, owner);
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
  info = CreateObject(2HUD, 0,0, GetOwner());
  info->SetPosition(168, -23);
  
  // Ammobag
  InitAmmoBagHUD();
}

protected func Destruction()
{
	if(info) info->RemoveObject();
	for(var i=0; i < GetLength(ammoobjs); ++i) {
		if(ammoobjs[i]) ammoobjs[i]->RemoveObject();
	}
}

private func InitAmmoBagHUD() {
	// remove ammobag display
	var def = 0;
	allAmmo = CreateArray();
	var arrI = 0;
	// get all ammo
	for(var i=0; def = GetDefinition(i,C4D_StaticBack); ++i) {
		if(DefinitionCall(def,"IsAmmo"))
			allAmmo[arrI++] = def;
	}
	ammoobjs = CreateArray();
	
	// place all ammo objects
	for(var i=0; i < GetLength(allAmmo); ++i) {
	
		// ...now
		ammoobjs[i] = CreateObject(2HUD,0,0,GetOwner());
		ammoobjs[i]->SetPosition(70,-85-i*28);
		ammoobjs[i]->SetGraphics(0, 0, allAmmo[i],1,4);
		ammoobjs[i]->SetObjDrawTransform(750,0,-50000,0,750,-62000,0,1);
		ammoobjs[i]->SetVisibility(VIS_None);
	}

}

/* Timer */

protected func Timer()
{
  // Spieler weg?
  if(GetOwner() == NO_OWNER || !GetPlayerName(GetOwner())) return(RemoveObject());
  // Kein Cursor?
  var pCursor = GetCursor(GetOwner());
  if(!pCursor) return;
  
  for(var i=0; i < GetLength(ammoobjs); ++i) {
		var aobj = ammoobjs[i];
	
		// Munitionsanzeige unsichtbar bei nicht hazard
		if(GetID(pCursor)!=HZCK) {
			aobj->SetVisibility(VIS_None);
			aobj->Message("",aobj);
            SetVisibility(VIS_None);
            Message("",info);
            Message("",this);
		}
  }
  
  // Anzeige
  SetObjDrawTransform(charge, 0, -1000 * (1000-charge)/20,0, 1000, 0, this(), 1);
  
  // Recharge: roter Balken wandert von links nach rechts
  if(recharge)
  {
    SetGraphics("Red", this, GetID(), 2, 2, "Red");
    SetObjDrawTransform(1000, 0, 50000 -1000 * (100-recharge),0, 1000, 0, this(), 2);
  }
  else {
    SetGraphics(0,0,0, 2);
  }
}

/* Aufrufe */

public func Update(object weapon, object ammobag, object who)
{
	// Nur den Cursor updaten
	if(who)
		if(GetCursor(GetOwner(who)) == who) {
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
				recharge = 0;
				
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
	charge = 0;
	recharge = 0;
}

private func UpdateAmmoBag(object ammobag, object weapon,who) {

	var ammoid = 0;
	if(weapon)
		if(weapon->~IsWeapon())
			ammoid = weapon->GetFMData(FM_AmmoID);

	for(var i=0; i < GetLength(ammoobjs); ++i) {
		var aobj = ammoobjs[i];
	
		// unsichtbar bei NoAmmo, oder wenn clonk ein arbeiter ist!
		if(NoAmmo() || !ammobag || who->~IstBauarbeiter()) {
			aobj->SetVisibility(VIS_None);
			aobj->Message("",aobj);
			continue;
		}
		// ansonsten Munitionsstand anzeigen
		aobj->SetVisibility(VIS_Owner);
			
		var ammo = 0;
		if(ammobag)
			ammo = ammobag->GetAmmo(allAmmo[i]);
		
		var color = "eeeeee";
		if(!ammo) color = "777777";
		if(allAmmo[i] == ammoid) {
			color = "ffff00";
			if(!ammo) color = "ff0000";
		}
		aobj->Message("@<c %s>%d</c>", aobj, color, ammo);
	}
}
