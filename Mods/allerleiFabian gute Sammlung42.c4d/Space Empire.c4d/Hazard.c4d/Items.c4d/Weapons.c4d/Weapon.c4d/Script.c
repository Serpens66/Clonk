/*-- Waffe --*/

#strict 3

/* Konstanten */
//Feuermodi-Konstanten
static const FM_Name      = 1;  // Name des Feuermodus'

static const FM_AmmoID    = 2;  // ID der benutzten Munition
static const FM_AmmoLoad  = 3;  // Menge der eingeladenen Munition
static const FM_AmmoUsage = 4;  // Munitionsverbrauch pro AmmoRate Schüsse
static const FM_AmmoRate  = 5;  // s.o.

static const FM_Reload    = 6;  // Nachladezeit der Waffe in Frames.
static const FM_Recharge  = 7;  // Zeitabstand zwischen zwei Schüssen in Frames.

static const FM_Auto      = 8;  // Automatische Waffe.
static const FM_Aim       = 9;  // Waffe zielt
static const FM_Condition =10;  // Vorraussetzung für den Schussmodus

static const FM_Icon      =11;  // Icon das im Schussmodimenü angezeigt wird

static const FM_Damage    =12;  // Schaden, den der Schussmodus anrichtet

// Bot-Konstanten für Feuermodi
static const BOT_Range    = 100;  // Reichweite des Modus
static const BOT_DmgType  = 101;  // Schadenstyp, den der Modus hauptsächlich anrichtet

static const BOT_Ballistic= 102;  // Der Modus schießt ballistische Geschosse

static const BOT_Power    = 103;  // Stärke der Waffe
  static const BOT_Power_1 =  1;  // geringe Stärke (Pistole, Pistolen-Granatwerfer, Pistole-Laser, Pistole-Schleim, EMP)
  static const BOT_Power_2 =  2;  // mittlere Stärke (Pumpgun, Energiegewehr, Flammenwerfer, Napalmgranate, Lasergranate)
  static const BOT_Power_3 =  3;  // hohe Stärke (Granatwerfer: Normal, Cluster & Schleim, Motorsäge, Energiegewehr mit Laser, Biowerfer, Clusterpumpgun, Feuerball)
  static const BOT_Power_LongLoad = 4;
                                  // Waffen, mit einer langen Nachladezeit (Partikelk., Bazooka, Minigun)
                                  // Diese Waffen werden nur leer geschossen und dann erstmal nicht benutzt
static const BOT_EMP      = 104;  // EMP-Waffe. Wird nur gegen Maschinen eingesetzt

public func Default(int data)    // Standardeinstellungen
{
  data ??= 0;
  if(Equal(data, FM_Name))      return("Standard");
  if(Equal(data, FM_AmmoID))    return(STAM);
  if(Equal(data, FM_AmmoUsage)) return(1);
  if(Equal(data, FM_AmmoRate))  return(1);
  if(Equal(data, FM_Auto))      return(nil);
  if(Equal(data, FM_Aim))       return(nil);
  if(Equal(data, FM_Condition)) return(true);
  if(Equal(data, FM_AmmoLoad))  return(5);
  if(Equal(data, FM_Reload))    return(100);
  if(Equal(data, FM_Recharge))  return(10);
  if(Equal(data, FM_Icon))      return(GetID());
  if(Equal(data, FM_Damage))    return(nil);
  if(Equal(data, BOT_Range))    return(100);
  if(Equal(data, BOT_DmgType))  return(DMG_Projectile);
  if(Equal(data, BOT_Ballistic))return(nil);
  if(Equal(data, BOT_Power))    return(BOT_Power_1);
  if(Equal(data, BOT_EMP))      return(nil);
}

public func HandX()    { return(nil); }    // X-Position in der Hand
public func HandY()    { return(nil); }    // Y-Position in der Hand
public func HandSize() { return(1000); } // Größe in der Hand, Standard: 1000
public func HandBarrel(){return(nil); }    // Y-Offset des Laufs

public func CanAim() { return(true); }   // mit diesem Gegenstand kann man zielen

/* Callbacks an Objekte die WEPN includieren */
// i ist dabei firemode

public func OnEmpty(int i)     { i ??= 0;
    } // wird aufgerufen wenn Waffe leer (keine Munition)
public func OnSelect(int i)    { i ??= 0;
    } // wird bei Anwahl im Inventar aufgerufen
public func OnDeselect(int i)  { i ??= 0;
    } // wird bei Abwahl im Inventar aufgerufen

public func OnReload(int i)    { i ??= 0;
    } // wird bei Start des Nachladens aufgerufen
public func OnReloaded(int i)  { i ??= 0;
    } // wird nach erfolgreichem Nachladen aufgerufen

public func OnAutoStart(int i) { i ??= 0;
    } // wird beim Starten einer autom. Waffe aufgerufen (erster Schuss)
public func OnAutoStop(int i)  { i ??= 0;
    } // wird beim Stoppen einer atuom. Waffe aufgerufen

public func OnAimStart(int i)  { i ??= 0;
    } // Anfangen zu zielen
public func OnAimStop(int i)   { i ??= 0;
    } // Stoppen zu zielen

public func IsUpgradeable(id i){ } // Upgradebar?
public func OnUpgrade(id i)    { } // Beim Upgraden
public func OnDowngrade(id i)  { } // Beim Downgraden

/* Lokale Variablen */

local firemode;             // aktueller Feuermodus

local ratecount, stopauto, shooting;  // intern...

local controller;           // the object which controls the weapon (the hazard clonk)

local upgrades;							// Welche Upgrades wir haben

protected func Initialize() {
  firemode = 1;
  ratecount = GetFMData(FM_AmmoRate);
  stopauto = nil;
  upgrades = CreateArray();
  SetUser();
}

/* Feuern */

public func SetUser(object pObj) { controller = pObj; }
public func GetUser() { return(controller); }

public func ControlUpdate(object caller, int comdir, bool dig, bool throw)
{
  comdir ??= 0;
  dig ??= false;
  throw ??= false;
  SetUser(caller);

  // autom. Schuss beenden, wenn Werfen losgelassen (JumpAndRun)
  if(And(IsRecharging(), !throw))
    StopAutoFire();
  // Nicht nachladen, wenn die Munition ausging und der Controller rechtzeitig
  // werfen wieder loslässt.  
  else if(!throw)
    ClearScheduleCall(this(), "Reload");
}

public func ControlThrow(caller)
{
  SetUser(caller);
  
  // autom. Schuss beenden, wenn erneut Werfen gedrückt (klassisch)
  if(IsRecharging())
  {
    if(!GetPlrCoreJumpAndRunControl(caller->GetController()))
      StopAutoFire();

    return(1);
  }

  // Unterstützt der Schussmodus Zielen, aber wir tuns nicht?
  if(And(And((GetFMData(FM_Aim) ?? 0)>0, !(GetUser()->~IsAiming())), !(GetUser()->~AimOverride())))
  {
    // Können wir?
    if(GetUser()->~ReadyToAim())
      // Auf gehts:
      GetUser()->StartAiming();
  
    // Nachladen?
    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);
    // weil keine Muni mehr da war? -> nachladen wenn geht
    if(!CheckAmmo(ammoid,ammousage,this()))
      if(CheckAmmo(ammoid,ammousage,GetUser()))
      {
        Reload();
      }
      // nicht genügend Munition
      else {
        PlayerMessage(GetOwner(caller), "$NotEnoughAmmo$", caller, ammoid);
      }
    // Sonst nix.
    return(1);
  }

  // Feuern! Fehlgeschlagen?
  if(!Fire()) {

    var ammoid = GetFMData(FM_AmmoID);
    var ammousage = GetFMData(FM_AmmoUsage);
    // weil keine Muni mehr da war? -> nachladen wenn geht
    if(!CheckAmmo(ammoid,ammousage,this()))
      if(CheckAmmo(ammoid,ammousage,GetUser())) {
        Reload();
      }
      // nicht genügend Munition
      else {
        PlayerMessage(GetOwner(caller), "$NotEnoughAmmo$", caller, ammoid);
      }
  }

  return(1);
}

public func Fire()
{

  // bereit zum Schießen
  if(!(GetUser()->~ReadyToFire())) return(nil);

  // lädt grad nach... bitte nicht stören
  if(IsReloading()) return(nil);

  var ammoid = GetFMData(FM_AmmoID);
  var ammousage = GetFMData(FM_AmmoUsage);
  // leer?
  if(!CheckAmmo(ammoid,ammousage,this())) {
    OnEmpty(firemode);
    return(nil);
  }

  // Callback
  if(GetFMData(FM_Auto))
    OnAutoStart(firemode);

  // FEUAAA!!!1 OMGOMG
  Shoot();

  return(true);
}

/* Waffenmenü */

public func ControlDigDouble(caller)
{
  SetUser(caller);

  var name;
  // Das Menü für die Schussmodiauswahl
  CreateMenu(GetID(), GetUser(), nil, nil, "$FireMode$", C4MN_Extra_Info, nil, true);
  // Solang suchen, bis keine Schussmodi mehr da sind
  for(var i = 1; name=GetFMData(FM_Name, i); S42MigrationStep(i, 1, true) )
  {
    // Vorraussetzung muss erfüllt sein
    if(GetFMData(FM_Condition, i))
    {
      var id = GetFMData(FM_AmmoID, i);
      var iconid = GetFMData(FM_Icon, i);
      var obj = CreateObject(iconid);
      // Wir haben ein Objekt fürs Menü und legen die Ammografik drauf
      SetGraphics(nil, obj, id, 1, GFXOV_MODE_Picture);
      SetObjDrawTransform(650,nil,5000,nil,650,5000, obj, 1);
      
      // Eintragstext
      var entryname=Format("%s (%s)", name, GetName(nil, id));
      
      // Genug Ammo für zumindest einen Schuss vorhanden sein
      if(!CheckAmmo(id,GetFMData(FM_AmmoUsage,i),GetUser(),this()))
      {
        // Name des Eintrags wird grau
        entryname=Format("<c %x>%s</c>", RGB(100,100,100), entryname);
        // Eintrag grau
        obj->SetClrModulation(RGB(100,100,100));
      }
      // Eintrag hinzufügen.
      AddMenuItem(entryname, "ChangeFireMode", GetID(this()), GetUser(), nil, i, Format("$DescFireMode$", name), 4, obj);
      // Objekte natürlich wieder löschen
      RemoveObject(obj);
    }
  }
  return(true);
}

/* Nachladen/Feuermodus ändern */

private func Failure(dummy, i)
{
  // Geht nicht: Bescheidsagen
  var id2=GetFMData(FM_AmmoID, i);
  PlayerMessage(GetOwner(GetUser()), "$NotEnoughAmmo$", GetUser(),id2);
  OnEmpty(i);
}

public func GetFireMode() { return(firemode); }

private func ChangeFireMode(dummy, i)
{
  // Immer noch genügend Munition da?
  if(!CheckAmmo(GetFMData(FM_AmmoID, i),GetFMData(FM_AmmoUsage,i),GetUser(),this()))
  {
    // Nein. Mecker.
    Failure(dummy, i);
    // Menü will geupdatet werden, wahrscheinlich
    var item=GetMenuSelection(GetUser());
    CloseMenu(GetUser());
    ControlDigDouble(GetUser());
    SelectMenuItem (item, GetUser());
    // Fertig.
    return;
  }
  CloseMenu(GetUser());
  var old = GetFireMode();
  // Schussmodus umstellen
  if(SetFireMode(i))
  {
    OnAutoStop(old);
    // Danach neue Munition einladen.
    Reload();
  }
}

private func SetFireMode(int i) {
  i ??= 0;
  if(Equal(i, nil)) return;

  // Gleicher Modus: Nur nachladen wenn nicht mehr voll und lädt nicht nach
  if(Equal(i, firemode)) {
    if(CheckAmmo(GetFMData(FM_AmmoID, i),GetFMData(FM_AmmoLoad),this())) return;
    if(IsReloading()) return;
  }
  // Alte Munition ausladen
  Empty();
  // Schussmodus umstellen
  firemode=i;
  stopauto=nil;
  ratecount = GetFMData(FM_AmmoRate, i);
  
  // Helpmessage
  if(GetUser())
    HelpMessage(GetUser()->GetOwner(),"$FireModeChanged$",GetUser(),GetFMData(FM_Name),GetFMData(FM_AmmoID));
  
  return(1);
}

public func Reload()    // Waffe nachladen
{
  ResumeReload();
  if(IsReloading()) return(nil);

  // Verzögerung? Abbrechen
  if(IsRecharging()) RemoveEffect("Recharge", this());

  // nicht genug Ammo um nachzuladen
  var ammoid = GetFMData(FM_AmmoID);
  var ammousage = GetFMData(FM_AmmoUsage);
  if(!CheckAmmo(ammoid,ammousage,GetUser()))
    return(nil);

  // entleeren falls noch voll
  if(CheckAmmo(ammoid,ammousage,this()))
    Empty();

  if(GetFMData(FM_Auto))
    OnAutoStop(firemode);
  OnReload(firemode);
  var reloadtime = (GetFMData(FM_Reload) ?? 0)*(MaxReloadAmount(GetUser()) ?? 0)/(GetFMData(FM_AmmoLoad) ?? 0);

  // Mit dem Nachladen beginnen
  AddEffect("Reload", this(), 1, 1, this(), nil, Max(1, reloadtime));
  return(true);
}

public func CancelReload()  // Nachladen abbrechen
{
  if(!IsReloading()) return(nil);
  RemoveEffect("Reload", this(),nil,true);
  return(true);
}

public func PauseReload() // Nachladen pausieren
{
  if(!IsReloading()) return(nil);
  if(Equal(EffectVar(1,this(),GetEffect("Reload",this())), nil)) return(nil);
  EffectVar(1,this(),GetEffect("Reload",this())) = nil;
  return(true);
}

public func ResumeReload()  // Nachladen wiederaufnehmen (wenn pausiert)
{
  if(!IsReloading()) return(nil);
  if(Equal(EffectVar(1,this(),GetEffect("Reload",this())), 1)) return(nil);
  EffectVar(1,this(),GetEffect("Reload",this())) = 1;
  return(true);
}

private func Recharge()    // Wartezeit zwischen zwei SchÃ¼ssen
{
  var rechargetime = GetFMData(FM_Recharge);
  AddEffect("Recharge", this(), 1, 1+(Max(1, rechargetime) ?? 0), this());
}

public func Empty()    // Waffe ausleeren
{
  // Laden wir nach? Abbrechen.
  if(IsReloading()) RemoveEffect("Reload", this());
  if(IsRecharging()) RemoveEffect("Recharge", this());

  // Munitionsart wo raus muss
  var ammoid = GetFMData(FM_AmmoID);
  // Zu schiebende Munitionsmenge
  var ammoamount = GetAmmo(ammoid, this());
  // Clonk wiedergeben 
  DoAmmo(ammoid, ammoamount, GetUser());
  // Hier entfernen
  DoAmmo(ammoid, -(ammoamount ?? 0), this());
}

private func Reloaded(caller)   // Waffe nachgeladen
{

  // Munitionsart wo rein muss
  var ammoid = GetFMData(FM_AmmoID);
  var ammoamount = MaxReloadAmount(caller);

  // zwischendurch Ammo zuviel ammo verbraucht? Fehlschlag.
  if(!CheckAmmo(ammoid,ammoamount,caller))
    return(nil);

  // Hier einfügen
  DoAmmo(ammoid, ammoamount, this());
  // Dem Clonk abziehen
  DoAmmo(ammoid, -(ammoamount ?? 0), caller);

  HelpMessage(caller->GetOwner(),"$Reloaded$",caller,ammoamount,ammoid);


  // Callback
  OnReloaded(firemode);
}

/* Upgradezeug */

// ‬Waffe mit uid Upgraden
public func Upgrade(id uid)
{
  // ist die Waffe damit überhaupt upgradebar?
  if(!IsUpgradeable(uid))
    // nichts tun
    return(nil);
  
  upgrades[(GetLength(upgrades) ?? 0)] = uid;
  OnUpgrade(uid);
  return(true);
}

// Upgrade uid entfernen
public func Downgrade(id uid)
{
  // haben wir das Upgrade überhaupt?
  if(!GetUpgrade(uid))
    return(nil);
  
  for(var i = nil; (i ?? 0) < (GetLength(upgrades) ?? 0); S42MigrationStep(i, 1, true))
    if(Equal(upgrades[(i ?? 0)], uid))
    {
      upgrades[(i ?? 0)] = nil;
      for(var j = i; (j ?? 0) < (GetLength(upgrades) ?? 0)-1; S42MigrationStep(j, 1, true))
        upgrades[(j ?? 0)] = upgrades[(j ?? 0)+1];
      
      OnDowngrade(uid);
      if(Contained())
        CreateContents(uid,Contained());
      else
        CreateObject(uid);
      return(true);
    }
  
  return(nil);
}

// Ist waffe mit uid upgegradet?
public func GetUpgrade(id uid)
{
  for(var upg in upgrades)
    if(Equal(upg, uid))
      return(true);
      
  return(nil);
} 

/* Feuern */

private func Shoot(object caller) {          // Feuern mit Feuermodus
  //User da? :S
  if(!GetUser())
    return(stopauto=true);

  var ammoid = GetFMData(FM_AmmoID);
  //soll er zielen, zielt aber nicht?
  if(And(And(!(GetUser()->~IsAiming()), (GetFMData(FM_Aim) ?? 0)>0), !(GetUser()->~AimOverride())))
    return(stopauto=true); //abbrechen
  // user hat sich irgendwo reinbewegt 
  if(And(GetUser()->Contained(), Equal(GetUser(), Contained())))
    return(stopauto=true);
  // Feuern...
  Call(Format("Fire%d",firemode));
  if(GetFMData(FM_Auto))
    shooting = true;

  if(!GetUser()) return;
	
  // Munition abziehen
  if(Equal(ratecount, 1)) {
    var muni = GetFMData(FM_AmmoUsage);
    DoAmmo(ammoid,-(muni ?? 0));
    ratecount = GetFMData(FM_AmmoRate);
  }
  else { S42MigrationStep(ratecount, -1); }

  // Pause nach einem Schuss
  if(GetAmmo(ammoid)) Recharge();
  else {
    shooting = nil;
    OnEmpty();
    if(GetFMData(FM_Auto)) OnAutoStop(firemode);

    // Automatisch nachladen, wenn die Feuertaste nach 5 Frames noch gedrückt ist
    if(GetPlrCoreJumpAndRunControl(GetUser()->GetController()))
    	if(!IsReloading())
      	ScheduleCall(this(), "Reload", 5);
  }
  // HZCK soll Munition doch bitte neu anschauen
  GetUser()->~UpdateCharge();
}

public func IsShooting() { return(shooting); }

public func GetFMData(int data, int i)
{
  // Vom Feuermodus i Information holen
  // i nicht angegeben? Muss nicht, ist aktueller dann
  data ??= 0;
  i ??= 0;
  if(!i) i=firemode;
  var value = ObjectCall(this(), Format("FMData%d",i), data);
  // Modifikationen
  var effect,user = GetUser(),j;
  while(effect = Or(GetEffect("*Bonus*",this(),j), Equal(j, nil))) {
    S42MigrationStep(j, 1, true);
    if(!GetEffect("*Bonus*",this(),effect,1))
      continue;
    var tval = EffectCall(this(),effect,"FMData",data,value);
    if(tval)
      value = tval;
  }
  if(user)
  {
    for(var i; (i ?? 0) <= (GetEffectCount("*Bonus*",user) ?? 0); S42MigrationStep(i, 1, true)) {
      var tval = EffectCall(user,GetEffect("*Bonus*",user,(i ?? 0)-1),"FMData",data,value,i);
      if(tval)
        value = tval;
    }
    /*
    while(effect = GetEffect("*Bonus*", user, j) || j == 0)
    {
      j++;
      if(!GetEffect("*Bonus*",user,effect,1))
       continue;
      Log("%s",GetEffect("*Bonus*",user,effect,1));
      var tval = EffectCall(user,effect,"FMData",data,value);
      if(tval)
        value = tval;
    }*/
  }
  return(value);
}

// Informationen für Bots (je nach Feuermodus)
public func GetBotData(int data, int i)
{
  // Vom Feuermodus i Information holen
  data ??= 0;
  i ??= 0;
  if(!i) i=firemode;
  return(ObjectCall(this(), Format("BotData%d",i), data));
}

protected func Deselection(object pContainer)
{
  // Callback
  OnDeselect(firemode);
  // Laden stoppen
  PauseReload();
  //Automatischen Schuss stoppen
  if(And(GetFMData(FM_Auto), IsRecharging())) stopauto=true;
}

protected func Selection(object pContainer)
{
  // Callback
  OnSelect(firemode);
  // Laden wiederaufnehmen
  ResumeReload();
}

public func StopAutoFire() {
  stopauto = true;
}

public func AimStart() { OnAimStart(firemode); }

public func AimStop() { 
  // automatisch feuern aus
  if(IsRecharging())
    StopAutoFire();
  OnAimStop(firemode);
}

protected func Departure()  // Waffe weggeworfen: Nachladen abbrechen. :(
{
  if(GetFMData(FM_Auto))
    if(!stopauto)
      OnAutoStop(firemode);
  if(IsReloading())
    RemoveEffect("Reload", this(), nil, true);

  SetUser();
}

protected func Entrance(object pContainer)
{
  SetUser(pContainer);
}

public func IsWeapon() { return(true); }
public func NoWeaponChoice() { return(Equal(GetID(), WEPN)); }

public func GetCharge() {   // wie voll ist die Waffe
  var charge;
  var ammoid = GetFMData(FM_AmmoID);
  // lädt nach: Nachladestatus anzeigen
  if(IsReloading()) {
    // Nachladestatus in %
    charge = 1000*((EffectVar(nil, this(), GetEffect("Reload", this()))) ?? 0)/(GetFMData(FM_Reload) ?? 0);
  }
  // ansonsten: Ladestand anzeigen
  else {
    charge = 1000*(GetAmmo(ammoid) ?? 0)/(GetFMData(FM_AmmoLoad) ?? 0);
  }

  return(charge);
}

private func MaxReloadAmount(object pAmmostore) {
  var ammoid = GetFMData(FM_AmmoID);
  var ammoload = GetFMData(FM_AmmoLoad);
  var ammousage = GetFMData(FM_AmmoUsage);
  // Zu erwartende Munitionsmenge errechnen...
  if(!CheckAmmo(ammoid,ammoload,pAmmostore))
    ammoload = GetAmmo(ammoid,pAmmostore);
  S42MigrationAssign(ammoload, "/=", (ammousage ?? 0));
  S42MigrationAssign(ammoload, "*=", (ammousage ?? 0));

  return(ammoload);
}

public func IsReloading() { return(GetEffect("Reload",this())); }
public func IsRecharging() { return(GetEffect("Recharge", this())); }

public func GetRecharge() {
  if(!IsRecharging())
    return(100);
  var time = GetEffect("Recharge",this(),nil,6);
  return((100*(time ?? 0))/(GetFMData(FM_Recharge) ?? 0));
}

/* Effekte */

public func FxReloadStart(object pTarget, int iNumber, int iTemp, int iTime) {
  iNumber ??= 0;
  iTemp ??= 0;
  iTime ??= 0;
  if(iTemp) return;

  // Nachladen aktiv:
  EffectVar(1,pTarget,iNumber) = 1;

  // Zeit speichern
  EffectVar(2,pTarget,iNumber) = iTime;
}

public func FxReloadTimer(object pTarget, int iNumber, int iTime) {

  iNumber ??= 0;
  iTime ??= 0;
  if(EffectVar(1,pTarget,iNumber))
  {
    GetUser()->~UpdateCharge();
    if(S42MigrationStep(EffectVar(nil,pTarget,iNumber), 1, true) >= (EffectVar(2,pTarget,iNumber) ?? 0)) return(-1);
  }
}

public func FxReloadStop(object pTarget, int iNumber, int iReason, bool fTemp) {
  // nicht nachladen
  iNumber ??= 0;
  iReason ??= 0;
  fTemp ??= false;
  if(And(!GetAlive(GetUser()), (GetCategory(GetUser()) ?? 0)&(C4D_Living ?? 0))) return(nil);
  // temporäre Aufrufe ignorieren
  if(fTemp) return;
  Message(" ",pTarget);
  // nur erfolgreich nachgeladen wenn Timer abgelaufen
  if(Equal(iReason, nil))
    pTarget->Reloaded(GetUser());
}

public func FxRechargeStop(object pTarget, int iNumber, int iReason, bool fTemp) {
  // Waffenträger weg?
  iNumber ??= 0;
  iReason ??= 0;
  fTemp ??= false;
  if(!GetUser()) return(nil);
  if(And(!GetAlive(GetUser()), (GetCategory(GetUser()) ?? 0)&(C4D_Living ?? 0))) return(nil);
  // automatisch weiterschießen, mit JnR-Control auch bei normalen Waffen
  if(Or(GetFMData(FM_Auto), GetPlrCoreJumpAndRunControl(pTarget->GetController()))) {
    // ... außer wenn abgebrochen oder keine Munition mehr (!)
    if(Or(Or(Or(stopauto, (And(!Equal(GetUser()->Contents(), this()), Equal(GetUser(), Contained(this()))))), !(GetUser()->ReadyToFire())), !CheckAmmo(GetFMData(FM_AmmoID), GetFMData(FM_AmmoUsage)))) {
      // Callback bei AutoFire
      if(GetFMData(FM_Auto))
        OnAutoStop(firemode);

      stopauto = nil;
      shooting = nil;
    }
    else {
      Shoot(pTarget);
    }
  }
}


/* globale Funktionen */

global func HelpMessage(int iPlr, string szMsg, object pTarget, a,b,c,d,e,f,g)
{
  // Nur ausgeben, wenn auch erwünscht
  iPlr ??= 0;
  if(GetPlrExtraData(iPlr, "Hazard_NoHelpMsg")) return(nil);
  // Hallo Neuling! Hier deine Info!
  return(PlayerMessage(iPlr, szMsg, pTarget, a,b,c,d,e,f,g));
}

/*  Waffenrotationszeugs und so :)   */

/* Von Sven kopiertes und geändertes Zeugs */
// Edit: Inzwischen selbst neu gecodet. Sven2->RemoveObject();
// mit Extravielen Kommentaren für Nachtschatten aka Clonk Nukem!!!!
//   jede Zeile nen Kommentar?? Hälst du CN für doof, boni? ^^ - newton
//     er hat gesagt er will viele Kommentare, also bekommt er sie. ;) - boni

// EffectVars:
//  0 - ID der Waffe
//  1 - Aktuelle Drehung
//  2 - X-Position des Anfangs der Waffe
//  3 - Y-Position des Anfangs der Waffe
//  4 - X-Position des Ende des Laufs
//  5 - Y-Position des Ende des Laufs
//  6 - Aktuelle Waffe

global func FxShowWeaponStart(object pTarget, int iNumber, int iTemp)
{
  iNumber ??= 0;
  iTemp ??= 0;
  if (iTemp) return(FX_OK);
  // Erste Grafikaktualisierung
  FxShowWeaponTimer(pTarget, iNumber);
  return (FX_OK);
}

global func FxShowWeaponTimer(object pTarget, int iNumber, int iTime)
{
  // Waffe aktualisieren:
  iNumber ??= 0;
  iTime ??= 0;
  var xoff, yoff, r;  // Offset, Winkel
  // kein Inventar oder falsche Aktion
  if(!Contents(nil,pTarget)) {
    EffectVar(nil, pTarget, iNumber) = nil;
    if(EffectVar(6, pTarget, iNumber)) {
      SetObjDrawTransform(1000,nil,nil,nil,1000,nil,EffectVar(6,pTarget,iNumber));
      EffectVar(6, pTarget, iNumber) = nil;
    }
    SetGraphics(nil, pTarget, nil, WeaponDrawLayer);
    return(FX_OK);
  }
  //Die Waffe momentan überhaupt anzeigen?
  if(!(pTarget->~WeaponAt(xoff, yoff, r))) {
    EffectVar(nil, pTarget, iNumber) = nil;
    if(EffectVar(6, pTarget, iNumber)) {
      SetObjDrawTransform(1000,nil,nil,nil,1000,nil,EffectVar(6,pTarget,iNumber));
      EffectVar(6, pTarget, iNumber) = nil;
    }
    SetGraphics(nil, pTarget, nil, WeaponDrawLayer);
    return(FX_OK);
  }
  var obj = Contents(nil,pTarget), id=GetID(obj);
  // Waffe nicht mehr aktuell
  if(!Equal(EffectVar(nil, pTarget, iNumber), id)) {
    // neues Objekt ist Waffe, oder ein Objekt, das gezeichnet werden soll
    if(Or(obj->~IsWeapon(), obj->~IsDrawable())) {
      EffectVar(nil, pTarget, iNumber) = id;
      EffectVar(6, pTarget, iNumber) = obj;
      SetGraphics(nil, pTarget,id, WeaponDrawLayer, GFXOV_MODE_Object,nil,GFX_BLIT_Parent,obj);
    }
    // neues Objekt ist keine Waffe
    else {
      EffectVar(nil, pTarget, iNumber) = nil;
      if(EffectVar(6, pTarget, iNumber)) {
        SetObjDrawTransform(1000,nil,nil,nil,1000,nil,EffectVar(6,pTarget,iNumber));
        EffectVar(6, pTarget, iNumber) = nil;
      }
      SetGraphics(nil, pTarget, nil, WeaponDrawLayer);
      return(FX_OK);
    }
  }

  id = EffectVar(nil, pTarget, iNumber);
  obj = EffectVar(6, pTarget, iNumber);

  // Ausrichtung nach Blickrichtung des Clonks
  // Variablen für die Transformation

  var width, height;  // Breiten- und Höhenverzerrung der Waffe
  var xskew, yskew;   // Zerrung der Waffe, wird zur Rotation gebraucht
  var size;           // Größe der Waffe in der Hand: 1000 = 100%
  // Variablen für die Position
  var xaim, yaim;     // Offset, dass sich durch zielen ergibt
  var dir;            // Richtung in die das Objekt schaut
  
  //schnell noch Rotation dazurechnen oder so!
  S42MigrationAssign(r, "+=", (ObjectCall(obj,"HandR") ?? 0));
  
  // Variablen mit Werten versehen
  width = height = xskew = yskew = 1;
  size = id->~HandSize();
  if(!size) size = 1000;
  dir  = (GetDir() ?? 0)*2-1;
  if(Or((r ?? 0) > 180, (r ?? 0) < -180))
    S42MigrationAssign(dir, "*=", -1);
  S42MigrationAssign(r, "*=", (dir ?? 0));

  var xfact = (size ?? 0) * (ObjectCall(obj,"HandX") ?? 0);    // Attachpunkte dazurechnen
  var yfact = (size ?? 0) * (ObjectCall(obj,"HandY") ?? 0);

  S42MigrationAssign(xoff, "+=", (Cos(r,xfact) ?? 0)/1000 + (dir ?? 0)*(Sin(r,yfact) ?? 0)/1000);
  S42MigrationAssign(yoff, "-=", (Cos(r,yfact) ?? 0)/1000 - (dir ?? 0)*(Sin(r,xfact) ?? 0)/1000);

  if(Equal(dir, 1))
  {
    height = -1;
    xskew = -1;
    yskew = -1;
  }

  r = -90*(dir ?? 0)-(r ?? 0)-90;
  S42MigrationAssign(height, "*=", (S42MigrationAssign(width, "*=", (Cos(r, size) ?? 0)) ?? 0));
  S42MigrationAssign(xskew, "*=", (Sin(r, size) ?? 0));
  S42MigrationAssign(yskew, "*=", -(xskew ?? 0));
  S42MigrationAssign(xoff, "*=", (dir ?? 0));

  SetObjDrawTransform(1000,xskew,xoff,yskew,1000,yoff, pTarget, WeaponDrawLayer); //position
  SetObjDrawTransform(width,xskew,nil,yskew,height,nil, obj); //Größe und Rotation

  // abspeichern, damit abrufbar
  r = -(r ?? 0)-90;
  var w = (GetDefCoreVal("Width",nil,id) ?? 0)/2;
  var brly = DefinitionCall(id,"BarrelYOffset");
  var brlx = DefinitionCall(id,"BarrelXOffset");
  var r2 = ((Angle(nil,nil,(w ?? 0)-(brlx ?? 0)/1000,(brly ?? 0)/1000) ?? 0)-90)*(dir ?? 0);
  var dist = Distance(nil,nil,(w ?? 0)*1000-(brlx ?? 0),brly);
  //Log("%d - %d - %d - %d",w,brl,r2,dist);
  EffectVar(1, pTarget, iNumber) = r;
  EffectVar(2, pTarget, iNumber) = (xoff ?? 0)-(Sin(r,(size ?? 0)*(w ?? 0)) ?? 0);
  EffectVar(3, pTarget, iNumber) = (yoff ?? 0)+(Cos(r,(size ?? 0)*(w ?? 0)) ?? 0);
  EffectVar(4, pTarget, iNumber) = (xoff ?? 0)+(Sin((r ?? 0)+(r2 ?? 0),(size ?? 0)*((dist) ?? 0)) ?? 0)/1000;
  EffectVar(5, pTarget, iNumber) = (yoff ?? 0)-(Cos((r ?? 0)+(r2 ?? 0),(size ?? 0)*((dist) ?? 0)) ?? 0)/1000;
  //EffectVar(4, pTarget, iNumber) = xoff+Sin(r,size*(w));	
  //EffectVar(5, pTarget, iNumber) = yoff-Cos(r,size*(w));
  //Log("%d / %d",EffectVar(4, pTarget, iNumber),EffectVar(5, pTarget, iNumber));
}
  
global func FxShowWeaponStop(object pTarget, int iNumber, int iReason, bool fTemp)
{
  iNumber ??= 0;
  iReason ??= 0;
  fTemp ??= false;
  if (fTemp) return(FX_OK);
  // Grafik entfernen
  SetGraphics(nil, pTarget, nil, WeaponDrawLayer);
  
  if(EffectVar(6,pTarget,iNumber))
	SetObjDrawTransform(1000,nil,nil,nil,1000,nil,EffectVar(6,pTarget,iNumber));
}  

public func SetUpgradeGraphic(int bitsWithGraphic, aIgnore)
{
	bitsWithGraphic ??= 0;
	if(!aIgnore) aIgnore=[]; // eine Liste von Upgrade IDs, die nicht berücksichtigt werden sollen (zb. weil dafuer keine Grafik exisitiert)
    var myBits = GetUpgradeGraphicBitmask(aIgnore);
	if(bitsWithGraphic) S42MigrationAssign(myBits, "&=", (bitsWithGraphic ?? 0));
	
	var graphicsString = nil;
	if(myBits) graphicsString = Format("Upgrade%d",myBits);
	
	SetGraphics(graphicsString);
}

public func GetUpgradeGraphicBitmask(aIgnore)
{
	var bitmask = nil;
	for(var upgradeId in upgrades)
	{
		if(InArray(aIgnore, upgradeId)) continue;
        S42MigrationAssign(bitmask, "+=", (upgradeId->~GetUpgradeGraphicsIDBit() ?? 0));
	}	
	return bitmask;
}
