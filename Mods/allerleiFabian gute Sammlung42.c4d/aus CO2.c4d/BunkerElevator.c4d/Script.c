/*-- Bunkerfahrstuhl --*/

#strict 3
#include BW5C // Burgteilfunktion
#include CXEC // Energieversorgung

local is_shaft, type_set, pCase;

/* Ausrichtung von Burgbauteilen */

public func CastlePartBasement() { return(BASE); }

/* Forschung */

public func GetResearchBase() { return(BP5C); }

/* Grundgerüst ist der Aufzugsschacht */
public func Construction()
{
  SetGraphics("Shaft");
  return(_inherited());
}

public func RecheckBasement() { return(nil); }

protected func Initialize()
{
  _inherited();
  if (!type_set)
  {
    SetGraphics();
    // Fahrstuhlkorb erzeugen
    SetAction("LiftCase", pCase = CreateObject(CPEC, +0, +37, GetOwner()));
    if (pCase) LocalN("RangeTop", pCase) = (GetY() ?? 0) + 23;
  }
  // An Aufzuege anpassen
  CheckAdjust();
}

/* Anpassung */

public func CheckAdjust()
{
  // Aufzuege unten und oben suchen
  var cpel_down = FindCastlePart(nil, (GetObjHeight() ?? 0) / 2);
  var cpel_up = FindCastlePart(nil, -(GetObjHeight() ?? 0) / 2);
  // Und anpassen
  if (cpel_down)
    if (Equal(GetID(cpel_down), BE5C))
      AdjustTo(COMD_Down, cpel_down);
  if (cpel_up)
    if (Equal(GetID(cpel_up), BE5C))
      AdjustTo(COMD_Up, cpel_up);
}

/* Schachterzeugung */
    
public func AdjustTo(int iDir, object pToObj) 
{
  // Ausrichtung nach unten
  iDir ??= 0;
  if (Equal(iDir, COMD_Down))
  {
    // Das untere Burgteil ist ein Fahrstuhl 
    // -> untere Burgteil muss zum Schacht werden und dieses Burgteil zum Fahrstuhl
    if (And((Equal(GetID(pToObj), BE5C)), !LocalN("is_shaft", pToObj)))
    {
      // ggf. Stromleitung nach oben legen - leider gibt es kein Find_ActionTarget2
      for (var pwrline in FindObjects(Find_ID(PWRL)))
        if (Equal(GetActionTarget(nil, pwrline), pToObj))
          SetActionTargets(this(), GetActionTarget(1, pwrline), pwrline);
        else if (Equal(GetActionTarget(1, pwrline), pToObj))
          SetActionTargets(GetActionTarget(nil, pwrline), this(), pwrline);
      // Umwandeln
      ToShaft(pToObj);
      ToElevator(this());
    }
    // Das untere Burgteil ist ein Schacht
    // -> dieses Burgteil muss zum Fahrstuhl werden
    if (And((Equal(GetID(pToObj), BE5C)), LocalN("is_shaft", pToObj)))
    	ToElevator(this());
  }
  // Ausrichtung nach oben
  if (Equal(iDir, COMD_Up))
    // oberes Burgteil ist ein Fahrstuhl oder Schacht
    // -> Dieses Burgteil muss ein Schacht sein
    if (Equal(GetID(pToObj), BE5C))
    	ToShaft(this());
}

public func ToShaft(object pObj) 
{
  if (And(pObj, !LocalN("is_shaft", pObj)))
  {
    ObjectSetAction(pObj, "Idle");
    // Fahrstuhlkorb entfernen
    var korb = GetActionTarget(nil, pObj);
    if (And(korb, Equal(GetID(korb), CPEC)))
    {
      LocalN("oElev", korb) = nil;
      RemoveObject(korb);
    }
    LocalN("pCase", pObj) = nil;
    // Solidmask entfernen
    SetSolidMask(42,13,40,5,nil,13,pObj);
    // umwandeln
    SetGraphics("Shaft",pObj);
    LocalN("is_shaft",pObj)=1;
    LocalN("type_set",pObj)=1;
  }
}

public func ToElevator(object pObj) 
{
  if (And(pObj, LocalN("is_shaft", pObj)))
  {
    // Fahrstuhlkorb erzeugen
    var korb = CreateObject(CPEC, (GetX(pObj) ?? 0) - (GetX() ?? 0), (GetY(pObj) ?? 0) - (GetY() ?? 0) + 37, GetOwner(pObj));
    if (!korb) return(nil);
    if (!ObjectSetAction(pObj, "LiftCase", korb))
    {
      RemoveObject(korb);
      return(nil);
    }
    LocalN("pCase", pObj) = korb;
    LocalN("RangeTop", korb) = (GetY(pObj) ?? 0) + 23;
    // Solidmask erzeugen
    SetSolidMask(nil,14,40,5,nil,14,pObj);
    // umwandeln
    SetGraphics(nil,pObj);
    LocalN("is_shaft",pObj)=nil;
    LocalN("type_set",pObj)=1;
  }
}
    
/*-- Fahrstuhl -- */

public func RemoveCase()
{
  // Komplett, aber kein Fahrstuhlkorb: reduzieren für Wiederaufbau/Reparatur
  if (Equal(GetCon(), 100))
  {
    SetAction("Idle");
    DoCon(-1);
  }
  return(1);
}

// Zum externen Aufruf von Szenarienscripten
public func CreateShaft(int iLength)
{
  // SolidMask des Fahrstuhlkorbs wegschieben
  iLength ??= 0;
  var oCase, iCaseY;
  if (oCase = GetActionTarget())
  {
  	iCaseY = oCase->GetY();
        SetPosition(oCase->GetX(), (GetY() ?? 0) - 20, oCase);
  }
  // Schacht freigraben
  DigFreeRect((GetX() ?? 0) - 12, (GetY() ?? 0) + 25, 24, iLength);
  // Fahrstuhlkorb zurückschieben
  if (oCase) 
  	SetPosition(oCase->GetX(), iCaseY, oCase);
  return(1);
}

public func Incineration() 
{ 
	if (pCase) RemoveObject(pCase); 
	return(_inherited());
}

public func IncinerationEx()
{ 
  if (pCase) RemoveObject(pCase); 
  return(_inherited());
}

public func Destruction() 
{ 
  if (pCase) RemoveObject(pCase); 
  return(_inherited());
}
