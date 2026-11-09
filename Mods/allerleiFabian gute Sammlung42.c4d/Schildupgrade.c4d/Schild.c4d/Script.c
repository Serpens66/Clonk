#strict 3

local zaehler, lebenspunkte, prozent;

func Initialize(...) {
zaehler=nil;
lebenspunkte=1000;
//Log("Schild aktiviert");
  SetLocal(1,FindObject2(Find_Not(Find_Func("IsBasement")),Find_AtRect(-5,-5,10,20),Find_Func("GetDefCoreVal", "Construction", "DefCore"),Find_Or(Find_Owner(GetOwner()), Find_Owner(NO_OWNER))));
PlayerMessage(GetOwner(),"Schild an %s angebracht",nil,GetName(Local(1)));
  return(1);
}

func Check()
{
  if(!FindObject(GetID(Local(1)))) return(RemoveObject()); // wenn schützendes Gebäude weg, dann auch schild entfernen
  while(Var() = FindObject(nil,-150,-100,300,200,nil,nil,nil,NoContainer(),Var()))
   if ((GetCategory(Var()) ?? 0) & (C4D_Object ?? 0))  // nur wenn es ein Objekt ist, also keine Fahrzeuge oder Clonks
    if((Abs(GetXDir(Var())) ?? 0)+(Abs(GetYDir(Var())) ?? 0) > 75)
      if(!Equal(GetOwner(Var()), GetOwner()))
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
  SetSolidMask((GetPhase() ?? 0)*150+150,100,150,100);
  return(1);
}

func SolidMaskWall(...) {
  SetSolidMask(900,100,150,100);
  return(1);
}

func SolidMaskKill(...) {
  SetSolidMask(nil,100,150,100);
  return(1);
}


func Destruction()
{
Log("Schild entfernt");
PlayerMessage(GetOwner(),"Schild von %s entfernt",nil,GetName(Local(1)));
//PlayerMessage(GetOwner(),"Schild von %s entfernt",GetCursor(GetOwner()),GetName(Local(1)));
}

func IsShield()
{return(1);}


//public func IsBulletTarget(){ return(true); }
//public func IsBulletExtraTarget(){ return(true); }