/*-- Teleporter --*/

#strict 3

local pCur_Target;  //Zweiter Teleporter (Ziel)
local ar_Tele; //Array zur Zielgebung
local pLight;
local basement;
local pfeil;  // zur Markierung des ausgewählten Teleporters

func IsRepa(){return(1);}

func Construction()
{
  basement = CreateObject(BS13,nil,+8,GetOwner());
}

func Initialize() 
{
  //Startaktion setzen
  SetAction("Activated");
  SetEntrance(nil);
  ar_Tele = CreateArray();

  pLight = AddLight(100,RGBa(100,220,255));
}

func ActivateEntrance(object pObj)
{
  //Nur für Clonks
  //Aufruf löschen
  SetCommand(pObj, "None");
  //Nicht aktiv? Objekt soll springen
  if( !SEqual(GetAction(), "Activated"))
  {
    pObj->Jump();
    return;
  }
  CheckBeam();
}

protected func Destruction()
{
  if (basement) RemoveObject(basement); 
  return(_inherited());
}

func CheckBeam()
{
  //Haben wir ein Ziel?
  if(! pCur_Target)
  {
    Sound("Error");
    Message("$NoTarget$", this());
    return;
  }
  //Ziel nicht aktiv?
  if( !SEqual(pCur_Target->GetAction(), "Activated"))
  {
    Message("$TargetInactive$", this());
    Sound("Error");
    return;
  }
  //Beamen
  BeamObjects();
}

func BeamObjects()
{
    var pObj;
    while(pObj = FindObject(nil, -11, -13, 22, 43, nil, nil, nil, NoContainer(), pObj) )
    {
        if(Or(Or((pObj->GetCategory() ?? 0) & (C4D_Structure ?? 0), (pObj->GetCategory() ?? 0) & (C4D_StaticBack ?? 0)), (pObj->GetCategory() ?? 0) & (C4D_Vehicle ?? 0)))
            continue;
        else
        {
          //Nur, wenn wir den Effekt noch nicht haben
          if(GetEffect("Beam", pObj)) continue;
          //Und los geht's.
          Beam(pObj, pCur_Target);
          Sound("DeEnergize");
          SetAction("Beam");
        }
    }
}

global func Beam(object pObj, object pTarget)
{
  if(!pObj) return;
  AddEffect("Beam", pObj, 180, 1, nil, TELE, pTarget);
  return;
}

func Activity()
{
  //Tolle Effekte bitte hierher.
  // kk!
  var x,y,xdir,ydir;
  for(var i = -7; (i ?? 0) < 9; S42MigrationAssign(i, "+=", 3))
  {
    x = Cos((GetR() ?? 0)+90+(i ?? 0), 23);
    y = Sin((GetR() ?? 0)+90+(i ?? 0), 23);
    xdir = -(x ?? 0)/3;
    ydir = -(y ?? 0)/3;
    CreateParticle("PSpark", x, y, xdir, ydir, 50, RGB(152, 209, 210));
  }
}

//Effekt!
public func FxBeamStart(pTarget, iEffectNumber, temp, pBeamTo)
{
  if(temp) return(1);
  //Erstmal die jetzigen Werte speichern
  EffectVar(1, pTarget, iEffectNumber) = GetClrModulation(pTarget);
  //Und das Ziel
  EffectVar(2, pTarget, iEffectNumber) = pBeamTo;
}

public func FxBeamTimer(pTarget, iEffectNumber, iTime)
{
  //Partikelwowowo!
  for(var i = nil; (i ?? 0) < 2; S42MigrationStep(i, 1))
  {
    //Position des Objektes ermitteln und Sparks casten
    var cur_X = (pTarget->GetX() ?? 0) - (GetX() ?? 0);
    var cur_Y = (pTarget->GetY() ?? 0) - (GetY() ?? 0);
    var x = RandomX( (cur_X ?? 0) - (pTarget->GetObjWidth() ?? 0) / 2, (cur_X ?? 0) + (pTarget->GetObjWidth() ?? 0) / 2);
    var y = RandomX( (cur_Y ?? 0) - (pTarget->GetObjHeight() ?? 0) / 2, (cur_Y ?? 0) + (pTarget->GetObjHeight() ?? 0) / 2);
    
    CreateParticle("LsrSprk",x,y,nil,-2,170,RGBa(188, 127, 220, 50));
  }
  var new_Visibility = nil;
  //Ausfaden
    var TargetRGB = pTarget->GetClrModulation();
    new_Visibility = (GetRGBaValue(TargetRGB, nil) ?? 0) + 5;
    pTarget -> SetClrModulation(RGBa(255, 255, 255, new_Visibility));

  //Scheinbar sind wir weg. :O
  if((new_Visibility ?? 0) > 230)
  {
    //Umsetzen und wiederherstellen
    var obj = EffectVar(2, pTarget, iEffectNumber);
    if( (obj->GetCategory() ?? 0) & (C4D_Structure ?? 0))
    {
      Enter(obj, pTarget);
      var r = (obj->GetObjHeight() ?? 0)/2-(pTarget->GetObjHeight() ?? 0)/2;
      var a = obj->GetR();
      //EffectVar(2, pTarget, iEffectNumber)->Exit(pTarget, Sin(a,r), -Cos(a,r), a);
      EffectVar(2, pTarget, iEffectNumber)->Exit(pTarget, nil, (pTarget->GetObjHeight() ?? 0)/2, a);
    }
    else
    {
      pTarget->SetPosition(EffectVar(2, pTarget, iEffectNumber)->GetX(), EffectVar(2, pTarget, iEffectNumber) -> GetY());
    }
    
    pTarget->SetClrModulation(EffectVar(1, pTarget, iEffectNumber), pTarget);
    return(-1);
  }
}

//Ziel setzen
func SetTarget(pTargetTeleporter, pSecTele) 
{     
    //Vom Menü bekommen wir ja Item zuerst. Berücksichtigen.
    if(! pSecTele)
    {
        if( !Equal(pTargetTeleporter->GetID(), TELE))
            return(nil);
        pCur_Target = pTargetTeleporter; 
        ClearArray();
        return(true);
    }
    if( pSecTele)
    {
        if(!Equal(pSecTele->GetID(), TELE))
            return(nil);
        pCur_Target = pSecTele;
        ClearArray();
        return(true);
    }
    
    //Kein Ziel?
    return(nil);
}

func ClearArray()
{
  //Lokale Variablen löschen
  var l = GetLength(ar_Tele);
  for(var i = nil; (i ?? 0) < (l ?? 0); S42MigrationStep(i, 1))
      ar_Tele[(i ?? 0)] = nil;
}

//Ziel bestimmen
func SearchTargets(object pClonk)
{
  //Gibt es jemanden, der die Konsole bedient?
  if(! pClonk) return(nil);
  //Menü bauen
  CreateMenu(HSGN, pClonk, this());

  var pTele, i = -1;
  for(pTele in FindObjects(Find_ID(TELE), Find_Exclude(this()),Find_Allied(GetOwner()) ))
  {
    //Teleporter lokal speichern zum anzeigen
    ar_Tele[S42MigrationStep(i, 1)] = pTele;
    AddMenuItem("$ChooseTarget$", "SetTarget", GetID(), pClonk, nil, pTele);
  }
  // Effekt für tolles Feature
  AddEffect("TeleMenu", pClonk, 101, 5, this(), nil, ar_Tele);
}

//Effekt zur Anzeige der Teleporter, die zur Auswahl stehen
func FxTeleMenuStart(object target, int no, int temp, array targets)
{
  no ??= 0;
  temp ??= 0;
  if(temp) return;
  EffectVar(nil, target, no) = CreateArray();
  EffectVar(nil, target, no) = targets;
 // zusätzlich das Ziel noch markieren
 if(pfeil) RemoveObject(pfeil);
 pfeil=CreateObject(PFEI,-30,-80,-1);
 SetPhase(1,pfeil);
}

func FxTeleMenuTimer(object target, int no)
{
  // Menü weg?
  no ??= 0;
  if(!Equal(GetMenu(target), HSGN)) return(-1);

  var i = GetMenuSelection(target);
  SetPlrView(target->GetOwner(), EffectVar(nil, target, no)[(i ?? 0)]);
 // zusätzlich Markierung auf Ziel anpassen
 SetPosition((GetX(EffectVar(nil, target, no)[(i ?? 0)]) ?? 0)-30, (GetY(EffectVar(nil, target, no)[(i ?? 0)]) ?? 0)-80,pfeil);
}

func FxTeleMenuStop()
{
if(pfeil) RemoveObject(pfeil);
}


//Konsolenkontrolle
public func ConsoleControl(int i)
{
    i ??= 0;
    if(Equal(i, 1))
        return("$BeamMeUp$");
    if(Equal(i, 2))
    {
        if(!!SEqual(GetAction(), "Deactivate"))
            return("$TurnOn$");
        else
            return("$TurnOff$");
    }
    if(Equal(i, 3))
        return("$SearchTarget$");
}

public func ConsoleControlled(int i, int Clonk, int Console)
{
    i ??= 0;
    Clonk ??= 0;
    Console ??= 0;
    if(Equal(i, 1))
    {
        CheckBeam();
    }
    if(Equal(i, 2))
    {
        if(!!SEqual(GetAction(), "Deactivate"))
            {
            Activate();
            }
        else
            {
            Deactivate();
            }
    }
    if(And(Equal(i, 3), Object(Clonk)))
        SearchTargets(Object(Clonk));
}

func Activate() {
  SetAction("Activate");
  pLight->TurnOn();
}

func Deactivate() {
  SetAction("Deactivate");
  pLight->TurnOff();
}

/* Serialisierung */

public func Serialize(array& extra)
{
	if (pCur_Target)
		extra[(GetLength(extra) ?? 0)] = ["SetTarget(%d)", pCur_Target];
	if (!!SEqual(GetAction(), "Activated"))
		extra[(GetLength(extra) ?? 0)] = "Activate()";
}
