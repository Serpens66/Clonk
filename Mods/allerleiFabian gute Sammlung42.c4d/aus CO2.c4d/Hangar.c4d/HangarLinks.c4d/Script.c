/*-- Hangar Links --*/

#strict

#include BP5C // Burgteilfunktion

local pSwitch;
local pGate,zerstoert;

func Initialize()
{
  ScheduleCall(this(),"SetModules",1);
  SetCategory(C4D_StaticBack());
  CastlePartAdjust();
  CastlePartInitialize();
  return(1);
}

func SetModules()
{
  pGate = CreateObject(HN0D,-57,68,GetOwner(this()));
  pSwitch = CreateObject(SI0D,40,68,GetOwner(this()));
  pSwitch->~Set(this());  //damit der Schalter weiß, welches Hangar zu ihm gehört
  return(1);
}


// Der Aufruf von CastleChange muss verzögert erfolgen, damit das Teil zum Aufruf auch wirklich weg ist
public func CastlePartDestruction()
{
if(pSwitch)RemoveObject(pSwitch);
if(pGate)RemoveObject(pGate);
  // Fundament?
  if (basement) 
  	RemoveObject(basement);
  // Globaler Temporäreffekt, wenn nicht schon vorhanden
  if (!GetEffect("IntCPW2CastleChange"))
  	AddEffect("IntCPW2CastleChange", 0, 1, 2, 0, CPW2);
  return(1);
}


func Damage(a,b,c,d,e,f,g,h,i,j)
{
var hp;
var schaden;
hp = GetDefCoreVal("BlastIncinerate","DefCore");
schaden = GetDamage();

  if (schaden*100 > hp*70 && zerstoert==0)  // bei 70% Schaden
  {
    if(pGate != 0)
      {PlayerMessage(GetOwner(),"Das Hangartor | wurde zerstört!",this()); Sound("Ding");RemoveObject(pGate);zerstoert=1;}
  }
  if (schaden*100 < hp*70)
{zerstoert=0;}
  return(_inherited(a,b,c,d,e,f,g,h,i,j));
} 


/* Ausrichtung von Burgbauteilen */

public func CastlePartWidth() { return (120); }
public func CastlePartHeight() { return (136); }
public func CastlePartTop() { return (-13); }
public func CastlePartBasement() { return(BA0D); }

/* Forschung */

public func GetResearchBase() { return(BPW2); }

