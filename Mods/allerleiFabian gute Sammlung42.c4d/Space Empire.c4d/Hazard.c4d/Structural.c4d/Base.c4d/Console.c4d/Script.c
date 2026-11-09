/*-- Konsole --*/

#strict

local smashed, target, ar_Tele,derclonk,Ziel,basement, pfeil;

func Initialize()
{
  ar_Tele = CreateArray();
}

protected func Construction()
{
  basement = CreateObject(BS06,0,+8,GetOwner());
}

protected func Destruction()
{
  if (basement) RemoveObject(basement); 
  return(_inherited());
}


// Effekte
protected func Lights()
{
  if(smashed || GetEffect("EMPShock", this()))
    {
    if(!Random(10)) CastObjects(SPRK, 1, 30, RandomX(-15,15));
    }
  else
    {
    if(!Random(8)) CreateParticle("NoGravSpark", 3, 1, 0, 0, 50, RGBa(250, 10, 10, 50));
    if(!Random(8)) CreateParticle("NoGravSpark", 3, 3, 0, 0, 50, RGBa(10, 10, 250, 50));
    }
}

// Bumm :o
protected func Damage()
{
  if(smashed) return(CastObjects(SPRK, 1, 30, RandomX(-15,15)));
  if(GetDamage() >= 40)
    {
    CastObjects(SPRK, 4, 50);
    Sparks(15, RGB(210, 210, 0));
    CreateSmokeTrail(RandomX(15,20), Random(360), 0,0, this());
    CreateSmokeTrail(RandomX(15,20), Random(360), 0,0, this());
    Sound("Blast1");
    Sound("CrystalHit*");
    SetGraphics("Broken");
    smashed = true;
    }
}

/* Bedienung */







// Konsole während des spiels auf Ziele einstellen

protected func ControlThrow(pClonk)
{
var obj, i = -1;


CreateMenu(GetID(),pClonk , 0, 0, "Konsole konfigurieren", 0, 1);


derclonk=pClonk;


for (var obj in FindObjects(Find_Func("IsConsoleTarget"),Find_Owner(GetOwner())))
{
ar_Tele[++i] = obj;
  AddMenuItem("Konsole auf %s einstellen","Set",GetID(obj),pClonk,0,obj);
}
  // Effekt für tolles Feature
  AddEffect("TeleMenu", pClonk, 101, 5, this(), 0, ar_Tele);
  AddMenuItem("Zurück","Nothing",GetID(),pClonk);
}


//Effekt zur Anzeige der Ziele, die zur Auswahl stehen

func FxTeleMenuStart(object ziel, int no, int temp, array targets)
{
  if(temp) return();
  EffectVar(0, ziel, no) = CreateArray();
  EffectVar(0, ziel, no) = targets;
 // zusätzlich das Ziel noch markieren
 if(pfeil) RemoveObject(pfeil);
 pfeil=CreateObject(PFEI,-30,-80,-1);
 SetPhase(1,pfeil);
}

func FxTeleMenuTimer(object ziel, int no)
{
  // Menü weg?
  if(GetMenu(ziel) != CONS && GetMenu(ziel) != CNS2 ) return(-1);

  var i = GetMenuSelection(ziel);
  SetPlrView(ziel->GetOwner(), EffectVar(0, ziel, no)[i]);
 // zusätzlich Markierung auf Ziel anpassen
 SetPosition(GetX(EffectVar(0, ziel, no)[i])-30, GetY(EffectVar(0, ziel, no)[i])-80,pfeil);
}

func FxTeleMenuStop()
{
if(pfeil) RemoveObject(pfeil);
}









protected func ControlDig(pClonk)
{
  [$Control$]
  if(smashed || GetEffect("EMPShock", this()))
    {
    Sound("Electric");
    Fling(pClonk, RandomX(-2,2), -2);
    return();
    }
    
 


if(!target) return();


PlayerMessage(GetOwner(),Format("Eingestellt auf: %v",GetName(Ziel)),derclonk);

  CreateMenu(GetID(target), pClonk, target, 0, Format("$Control$: %s", GetName(target)), 0, 1);
  for(var i = 1, desc ; desc = target->~ConsoleControl(i) ; i++)
  {
    //Aufrufenden Clonk und uns selbst dem Zielobjekt übergeben. Dies kann es in ConsoleController abrufen
//    target->~ConsoleController(this(), pClonk);
    // Zielobjekt muss für ConsoleControl(i) jeweils den Namen der Aktion zurückgeben, wichtig: i beginnt bei 1
    AddMenuItem(desc, Format("ConsoleControlled(%d, %d, %d)", i, ObjectNumber(pClonk), ObjectNumber(this())), GetID(), pClonk, 0, 0, "$Control$");
    // Zielobjekt bekommt Aufruf: ConsoleControll(i) und muss das dann verarbeiten
  }
  return(1);
}

func Nothing()
{}

//EMP-Angriff
public func EMPShock()
{
  //Nock Schockbar?
  if(GetEffect("EMPShock", this()) || smashed) return(0);
  
  //Ansonsten knallt's
  AddEffect("EMPShock", this(), 180, 650, this(), GetID());
}

public func IsMachine() { return(1); }

public func Set(par0,pTarget)
{
  target = pTarget;
Ziel=target;
PlayerMessage(GetOwner(),Format("Konsole erfolgreich auf %v eingestellt",GetName(target)),derclonk);

//if(obj) target = obj;
ClearArray();
}


func ClearArray()
{
  //Lokale Variablen löschen
  var l = GetLength(ar_Tele);
  for(var i = 0; i < l; ++i)
      ar_Tele[i] = 0;
}


/* Serialisierung */

public func Serialize(array& extra)
{
	if (target)
		extra[GetLength(extra)] = ["Set(Object(%d))", target];
	if (smashed)
		extra[GetLength(extra)] = "LocalN(\"smashed\")=true";
}
