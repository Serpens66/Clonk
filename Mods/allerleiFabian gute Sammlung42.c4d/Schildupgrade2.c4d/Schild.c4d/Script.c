#strict

local zaehler, lebenspunkte, prozent;

func Initialize()
{
zaehler=0;
lebenspunkte=1000;
//Log("Schild aktiviert");//SetLocal(1,FindObject2(Find_Not(Find_Func("IsBasement")),Find_AtRect(-5,-5,10,20),Find_Func("GetDefCoreVal", "Construction", "DefCore"),Find_Or(Find_Owner(GetOwner()), Find_Owner(NO_OWNER))));
PlayerMessage(GetOwner(),"Groﬂes Schild aufgestellt",0);
  return(1);
}

func Check()
{
  //if(!FindObject(GetID(Local(1)))) return(RemoveObject()); 
  while(Var() = FindObject(0,-150,-100,300,200,0,0,0,NoContainer(),Var())) // irgendein Objekt in N‰he
   if (GetCategory(Var()) & C4D_Object())  // nur wenn es ein Objekt ist, also keine Fahrzeuge oder Clonks
    if(Abs(GetXDir(Var()))+Abs(GetYDir(Var())) > 75)  // wenn es schneller als 75 ist
      if(GetOwner(Var()) != GetOwner()) // wenn es nicht zu einem selbst gehˆrt
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
  SetSolidMask(GetPhase()*300+300,200,300,200);
  return(1);

SolidMaskWall:
  SetSolidMask(1800,200,300,200);
  return(1);

SolidMaskKill:
  SetSolidMask(0,200,300,200);
  return(1);


func Destruction()
{
//Log("Schild entfernt");
PlayerMessage(GetOwner(),"Groﬂes Schild entfernt",0);
}

func IsShield()
{return(1);}


//public func IsBulletTarget(){ return(true); }
//public func IsBulletExtraTarget(){ return(true); }