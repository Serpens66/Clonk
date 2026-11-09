#strict

local zaehler, lebenspunkte, prozent;

Initialize:
zaehler=0;
lebenspunkte=1000;
//Log("Schild aktiviert");
  SetLocal(1,FindObject2(Find_Not(Find_Func("IsBasement")),Find_AtRect(-5,-5,10,20),Find_Func("GetDefCoreVal", "Construction", "DefCore"),Find_Or(Find_Owner(GetOwner()), Find_Owner(NO_OWNER))));
PlayerMessage(GetOwner(),"Schild an %s angebracht",0,GetName(Local(1)));
  return(1);

func Check()
{
  if(!FindObject(GetID(Local(1)))) return(RemoveObject()); // wenn schützendes Gebäude weg, dann auch schild entfernen
  while(Var() = FindObject(0,-150,-100,300,200,0,0,0,NoContainer(),Var()))
   if (GetCategory(Var()) & C4D_Object())  // nur wenn es ein Objekt ist, also keine Fahrzeuge oder Clonks
    if(Abs(GetXDir(Var()))+Abs(GetYDir(Var())) > 75)
      if(GetOwner(Var()) != GetOwner())
        {WallCall(); ++zaehler; prozent = ((lebenspunkte-zaehler)*100/lebenspunkte);
PlayerMessage(GetOwner(),"%v%",this(),prozent);};
if (zaehler >=lebenspunkte) {RemoveObject();};



  return(1);
}


WallCall:
  if(Not(GetAction() S= "Defense"))
    if(Not(GetAction() S= "DefenseWall")) SetAction("Defense");
  if(GetAction() S= "DefenseWall") SetAction("DefenseWall");
  return(1);

SolidMaskUpdate:
  SetSolidMask(GetPhase()*150+150,100,150,100);
  return(1);

SolidMaskWall:
  SetSolidMask(900,100,150,100);
  return(1);

SolidMaskKill:
  SetSolidMask(0,100,150,100);
  return(1);


func Destruction()
{
Log("Schild entfernt");
PlayerMessage(GetOwner(),"Schild von %s entfernt",0,GetName(Local(1)));
//PlayerMessage(GetOwner(),"Schild von %s entfernt",GetCursor(GetOwner()),GetName(Local(1)));
}

func IsShield()
{return(1);}


//public func IsBulletTarget(){ return(true); }
//public func IsBulletExtraTarget(){ return(true); }