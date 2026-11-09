#strict 3

local zaehler, lebenspunkte, prozent;

func Initialize()
{
zaehler=nil;
lebenspunkte=1000;
//Log("Schild aktiviert");//SetLocal(1,FindObject2(Find_Not(Find_Func("IsBasement")),Find_AtRect(-5,-5,10,20),Find_Func("GetDefCoreVal", "Construction", "DefCore"),Find_Or(Find_Owner(GetOwner()), Find_Owner(NO_OWNER))));
PlayerMessage(GetOwner(),"Groﬂes Schild aufgestellt",nil);
  return(1);
}

func Check()
{
  //if(!FindObject(GetID(Local(1)))) return(RemoveObject()); 
  while(Var() = FindObject(nil,-150,-100,300,200,nil,nil,nil,NoContainer(),Var())) // irgendein Objekt in N‰he
   if ((GetCategory(Var()) ?? 0) & (C4D_Object ?? 0))  // nur wenn es ein Objekt ist, also keine Fahrzeuge oder Clonks
    if((Abs(GetXDir(Var())) ?? 0)+(Abs(GetYDir(Var())) ?? 0) > 75)  // wenn es schneller als 75 ist
      if(!Equal(GetOwner(Var()), GetOwner())) // wenn es nicht zu einem selbst gehˆrt
        {WallCall(); S42MigrationStep(zaehler, 1); prozent = (((lebenspunkte ?? 0)-(zaehler ?? 0))*100/(lebenspunkte ?? 0));
PlayerMessage(GetOwner(),"%v%",this(),prozent);};
if ((zaehler ?? 0) >=(lebenspunkte ?? 0)) {RemoveObject();};



  return(1);
}


func WallCall(...) {
  if(Not(!!SEqual(GetAction(), "Defense")))
    if(Not(!!SEqual(GetAction(), "DefenseWall"))) SetAction("Defense");
  if(!!SEqual(GetAction(), "DefenseWall")) SetAction("DefenseWall");
  return(1);
}

func SolidMaskUpdate(...) {
  SetSolidMask((GetPhase() ?? 0)*300+300,200,300,200);
  return(1);
}

func SolidMaskWall(...) {
  SetSolidMask(1800,200,300,200);
  return(1);
}

func SolidMaskKill(...) {
  SetSolidMask(nil,200,300,200);
  return(1);
}


func Destruction()
{
//Log("Schild entfernt");
PlayerMessage(GetOwner(),"Groﬂes Schild entfernt",nil);
}

func IsShield()
{return(1);}


//public func IsBulletTarget(){ return(true); }
//public func IsBulletExtraTarget(){ return(true); }