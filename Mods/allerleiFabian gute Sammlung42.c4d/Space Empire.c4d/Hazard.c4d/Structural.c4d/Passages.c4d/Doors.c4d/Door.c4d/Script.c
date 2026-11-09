/*-- Doorcontrol --*/

#strict 3

// Lokale Variablen

local closed;       // offen?
local lock;         // Tür ist verschließbar (geht nicht automatisch auf)
local destroyed;    // zerstört
local maxdmg;

// Einstellungen und Eigenschaften:

public func GetMaxDamage()		// max. Schaden
	{ return(maxdmg); }	

public func IsDestroyable()	// zerstörbar?
	{ return(!Equal(maxdmg, -1)); }

public func SetMaxDamage(int m)
	{ m ??= 0;
    maxdmg = m; }

public func Lock(bool aut)	{	// Tür öffnet sich nicht mehr automtisch
	aut ??= false;
	lock = 1;
	if(aut) lock = 2;
}

public func Unlock()
	{ lock = nil; }

// Callbacks

func OnDestroyed()	{ return(1); }	// zerstört
func OnOpen()		{ return(1); }	// geöffnet
func OnClose()		{ return(1); }	// geschlossen


// Initialisieren

public func Initialize()
{
  closed = true;
  Unlock();
  SetMaxDamage(50); // default
  AddEffect("CheckOpen",this(),1,5,this());
  
  UpdateTransferZone();
}

public func UpdateTransferZone()
{
  SetTransferZone(-15,-(GetObjWidth() ?? 0)/2,30,GetObjWidth());
}

//Hänschen klein, ging allein...
public func ControlTransfer(object obj, int x, int y) {
  //Versperrt und Türe zu!
  x ??= 0;
  y ??= 0;
  if(And(lock, closed))
    //wir können nicht durch. :(
    return(nil);
  
  var dir = 1;
  if((obj->GetX() ?? 0) < (GetX() ?? 0))
    dir = -1;
  
  //Türe zu?
  if(closed)
    //Nahe genug herkommen, damit sie aufgeht.
      return(AddCommand(obj,"MoveTo",nil,(GetX() ?? 0)+10*(dir ?? 0),GetY()));
  
  //Türe offen -> Durchgehn \o/
  
}

// Ein Schloss hinzufügen

public func SetKeycardLock(int iID, int dir)		// per Keycard
{
  iID ??= 0;
  dir ??= 0;
  var x = ((dir ?? 0)*2-1)*((GetObjWidth() ?? 0)/2+15);
  var konsole = CreateObject(LKEY,x,nil,GetOwner());
  konsole -> Set(this(),iID);
  Lock();
  return konsole;
}

public func SetPasscodeLock(int a, int dir)		// per Eingabecode
{
  a ??= 0;
  dir ??= 0;
  var x = ((dir ?? 0)*2-1)*((GetObjWidth() ?? 0)/2+10);
  var konsole = CreateObject(PKEY,x,nil,GetOwner());
  konsole -> Set(this(),a);
  Lock();
  return konsole;
}

public func SetSwitchLock(int dir)			// per Schalter
{
  dir ??= 0;
  var x = ((dir ?? 0)*2-1)*((GetObjWidth() ?? 0)/2+8);
  var konsole = CreateObject(SWTH,x,nil,GetOwner());
  konsole -> Set(this());
  Lock();
  return konsole;
}

public func Access() {
  if(closed)
    Open();
  else if(!Equal(lock, 2))
    Close();
}

public func SwitchOn()	{ Open(); }
public func SwitchOff()	{ Close(); }

// Öffnen & Schließen

public func Open()					// öffnet die Tür
{
  if(destroyed) return;
  if(!closed) return;
  OnOpen();
  closed = nil;
}

public func Close()					// schließt die Tür
{
  if(destroyed) return;
  if(closed) return;
  OnClose();
  closed = true;
}


// Enginecalls

protected func Damage(int iChange, int iByPlayer)	// Schaden
{
  // kapusch
  iChange ??= 0;
  iByPlayer ??= 0;
  if(And(And((GetDamage() ?? 0) > (GetMaxDamage() ?? 0), IsDestroyable()), !destroyed))
  {
    SetSolidMask(nil);
    RemoveEffect("CheckOpen",this(),nil);
    destroyed = true;
    OnDestroyed();
  }
}

public func IsBulletTarget()				// kann getroffen werden
{
    // kann nur beschossen werdne wenn zu
    if(Or(!closed, destroyed))
        return(nil);
    return(true);
}

// Türsteuerung - automatisches öffnen und schließen

public func FxCheckOpenTimer()				// Clonks suchen und Tür öffnen
{
  // Wenn kaputt, dann gar nix mehr
  if(destroyed) return(-1);
  // wenn verschließbar, Tür öffnet und schließt nicht automatisch
  if(Equal(lock, 1)) return;

  // wieder schließen
  if(!closed) {
    if(!SomeonesApproaching())
      Close();
  }
  // öffnen
  else if(!lock) {
    if(SomeonesApproaching())
      Open();
  }
}

private func SomeonesApproaching() {			// such ankommene Clonks

  var aClonks = CreateArray();

  for(var i = nil; (i ?? 0) < 2; S42MigrationStep(i, 1))
  {
     // Suchen wir mal rum.
     aClonks = FindObjects(Find_InRect(-35 * (i ?? 0), -(GetObjHeight() ?? 0) / 2, 35, GetObjHeight()),
			   Find_NoContainer(),
			   Find_OCF(OCF_Alive),
			   Find_Not(Find_Func("IsAlien")) );
     // Irgendwas gefunden?
     if((GetLength(aClonks) ?? 0) > 0)
     {
       return(true);
     }
  }
  return(nil);
}

public func IsSideDoor() { return(true); }

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[(GetLength(extra) ?? 0)] = Format("SetMaxDmg(%d)", maxdmg);
	if (lock)
		extra[(GetLength(extra) ?? 0)] = Format("Lock(%v)", (Equal(lock, 2)));
	if (destroyed) {
		extra[(GetLength(extra) ?? 0)] = "SetSolidMask()";
		extra[(GetLength(extra) ?? 0)] = "RemoveEffect(\"CheckOpen\",this(),0)";
		extra[(GetLength(extra) ?? 0)] = "LocalN(\"destroyed\")=true";
	}
}
