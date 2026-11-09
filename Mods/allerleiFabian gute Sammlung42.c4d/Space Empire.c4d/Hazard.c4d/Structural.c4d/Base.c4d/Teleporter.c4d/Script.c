/*-- Teleporter --*/

#strict

local pCur_Target;  //Zweiter Teleporter (Ziel)
local ar_Tele; //Array zur Zielgebung
local pLight;
local basement;
local pfeil;  // zur Markierung des ausgewählten Teleporters

func IsRepa(){return(1);}

func Construction()
{
  basement = CreateObject(BS13,0,+8,GetOwner());
}

func Initialize() 
{
  //Startaktion setzen
  SetAction("Activated");
  SetEntrance(0);
  ar_Tele = CreateArray();

  pLight = AddLight(100,RGBa(100,220,255));
}

func ActivateEntrance(object pObj)
{
  //Nur für Clonks
  //Aufruf löschen
  SetCommand(pObj, "None");
  //Nicht aktiv? Objekt soll springen
  if( GetAction() ne "Activated")
  {
    pObj->Jump();
    return();
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
    return();
  }
  //Ziel nicht aktiv?
  if( pCur_Target->GetAction() ne "Activated")
  {
    Message("$TargetInactive$", this());
    Sound("Error");
    return();
  }
  //Beamen
  BeamObjects();
}

func BeamObjects()
{
    var pObj;
    while(pObj = FindObject(0, -11, -13, 22, 43, 0, 0, 0, NoContainer(), pObj) )
    {
        if(pObj->GetCategory() & C4D_Structure() || pObj->GetCategory() & C4D_StaticBack() || pObj->GetCategory() & C4D_Vehicle)
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
  if(!pObj) return();
  AddEffect("Beam", pObj, 180, 1, 0, TELE, pTarget);
  return();
}

func Activity()
{
  //Tolle Effekte bitte hierher.
  // kk!
  var x,y,xdir,ydir;
  for(var i = -7; i < 9; i += 3)
  {
    x = Cos(GetR()+90+i, 23);
    y = Sin(GetR()+90+i, 23);
    xdir = -x/3;
    ydir = -y/3;
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
  for(var i = 0; i < 2; ++i)
  {
    //Position des Objektes ermitteln und Sparks casten
    var cur_X = pTarget->GetX() - GetX();
    var cur_Y = pTarget->GetY() - GetY();
    var x = RandomX( cur_X - pTarget->GetObjWidth() / 2, cur_X + pTarget->GetObjWidth() / 2);
    var y = RandomX( cur_Y - pTarget->GetObjHeight() / 2, cur_Y + pTarget->GetObjHeight() / 2);
    
    CreateParticle("LsrSprk",x,y,0,-2,170,RGBa(188, 127, 220, 50));
  }
  var new_Visibility = 0;
  //Ausfaden
    var TargetRGB = pTarget->GetClrModulation();
    new_Visibility = GetRGBaValue(TargetRGB, 0) + 5;
    pTarget -> SetClrModulation(RGBa(255, 255, 255, new_Visibility));

  //Scheinbar sind wir weg. :O
  if(new_Visibility > 230)
  {
    //Umsetzen und wiederherstellen
    var obj = EffectVar(2, pTarget, iEffectNumber);
    if( obj->GetCategory() & C4D_Structure)
    {
      Enter(obj, pTarget);
      var r = obj->GetObjHeight()/2-pTarget->GetObjHeight()/2;
      var a = obj->GetR();
      //EffectVar(2, pTarget, iEffectNumber)->Exit(pTarget, Sin(a,r), -Cos(a,r), a);
      EffectVar(2, pTarget, iEffectNumber)->Exit(pTarget, 0, pTarget->GetObjHeight()/2, a);
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
        if( pTargetTeleporter->GetID() != TELE)
            return(false);
        pCur_Target = pTargetTeleporter; 
        ClearArray();
        return(true);
    }
    if( pSecTele)
    {
        if(pSecTele->GetID() != TELE)
            return(false);
        pCur_Target = pSecTele;
        ClearArray();
        return(true);
    }
    
    //Kein Ziel?
    return(false);       
}

func ClearArray()
{
  //Lokale Variablen löschen
  var l = GetLength(ar_Tele);
  for(var i = 0; i < l; ++i)
      ar_Tele[i] = 0;
}

//Ziel bestimmen
func SearchTargets(object pClonk)
{
  //Gibt es jemanden, der die Konsole bedient?
  if(! pClonk) return(0);
  //Menü bauen
  CreateMenu(HSGN, pClonk, this());

  var pTele, i = -1;
  for(pTele in FindObjects(Find_ID(TELE), Find_Exclude(this()),Find_Allied(GetOwner()) ))
  {
    //Teleporter lokal speichern zum anzeigen
    ar_Tele[++i] = pTele;
    AddMenuItem("$ChooseTarget$", "SetTarget", GetID(), pClonk, 0, pTele);
  }
  // Effekt für tolles Feature
  AddEffect("TeleMenu", pClonk, 101, 5, this(), 0, ar_Tele);
}

//Effekt zur Anzeige der Teleporter, die zur Auswahl stehen
func FxTeleMenuStart(object target, int no, int temp, array targets)
{
  if(temp) return();
  EffectVar(0, target, no) = CreateArray();
  EffectVar(0, target, no) = targets;
 // zusätzlich das Ziel noch markieren
 if(pfeil) RemoveObject(pfeil);
 pfeil=CreateObject(PFEI,-30,-80,-1);
 SetPhase(1,pfeil);
}

func FxTeleMenuTimer(object target, int no)
{
  // Menü weg?
  if(GetMenu(target) != HSGN) return(-1);

  var i = GetMenuSelection(target);
  SetPlrView(target->GetOwner(), EffectVar(0, target, no)[i]);
 // zusätzlich Markierung auf Ziel anpassen
 SetPosition(GetX(EffectVar(0, target, no)[i])-30, GetY(EffectVar(0, target, no)[i])-80,pfeil);
}

func FxTeleMenuStop()
{
if(pfeil) RemoveObject(pfeil);
}


//Konsolenkontrolle
public func ConsoleControl(int i)
{
    if(i == 1)
        return("$BeamMeUp$");
    if(i == 2)
    {
        if(GetAction() eq "Deactivate")
            return("$TurnOn$");
        else
            return("$TurnOff$");
    }
    if(i == 3)
        return("$SearchTarget$");
}

public func ConsoleControlled(int i, int Clonk, int Console)
{
    if(i == 1)
    {
        CheckBeam();
    }
    if(i == 2)
    {
        if(GetAction() eq "Deactivate")
            {
            Activate();
            }
        else
            {
            Deactivate();
            }
    }
    if(i == 3 && Object(Clonk))
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
		extra[GetLength(extra)] = ["SetTarget(%d)", pCur_Target];
	if (GetAction() eq "Activated")
		extra[GetLength(extra)] = "Activate()";
}
