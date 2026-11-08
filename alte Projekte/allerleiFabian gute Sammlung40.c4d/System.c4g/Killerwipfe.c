/*-- Neues Script --*/

#strict

#appendto WIPF

//Normalowipf zum Killerwipf machen

public func MakeKillerWipf() {
 Local(1)=1;
 //Killerwipf Färbung initialisieren
 SetClrModulation(RGB(RandomX(200,220),RandomX(200, 220),RandomX(200,220)));
 return(1); }
 

public func IsThreat() {if(Local(1)==1) return(true); return(0);}  // nur wenn killerwipf

protected func Initialize() {

//Local(1) = Killerwipf? (1=ja; 2=nein;)
 Local(1) = 2;
 return(Birth());}
 
func Death(a,b,c,d,e,f)
{
    if(IsThreat())   GameCallEx("Zaehler18");
    return(_inherited(a,b,c,d,e,f));
}
 
 
protected func RejectEntrance(object pNewCont, a,b,c) 
  {
  if(Local(1) == 1)  return(1); // killerwipfe lassen sich nicht einsammeln
  return (_inherited(pNewCont, a,b,c));
  }

protected func Activity()
{
  if(Local(1) == 1) {
  //Gefährlichkeitsstufe hochzählen
  //irgendwann reichts aber auch :o 
  if(Local(5) <= 80) Local(5)++; 
 
  // Monster!! (Wir sind die Beatsteakes aus Berlin! Berlin!)
  if (!Random(50)) Sound("DeadRowl");
  
  //Zum Angriffsziel!
      //die vorherige Überprüfung ist hochgradig essentziell!
 if(FindObject2(Find_OCF(OCF_CrewMember), Find_InRect(-450,  -50, 900, 100),Find_NoContainer()))
  SetCommand(this(), "MoveTo",FindObject2(Find_OCF(OCF_CrewMember), Find_InRect(-450,  -50, 900, 100),Find_NoContainer()));
   
  //Mach den Angriffssprung!
   if(FindObject2(Find_OCF(OCF_CrewMember), Find_InRect(-54,  -50, 0, 100),Find_NoContainer())) {SetXDir(-21); SetYDir(-25); }
   if(FindObject2(Find_OCF(OCF_CrewMember), Find_InRect(-255,  -50, 0, 100),Find_NoContainer())) {SetXDir(21); SetYDir(-25); }
   
  //Bissattacke!
  var ptarget = FindObject2(Find_OCF(OCF_CrewMember), Find_InRect(-10,  -15, 20, 30),Find_NoContainer());
 if(ptarget)
    {  
    Punch(ptarget, 2 +Local(5) / 4 );
    if(ptarget) ptarget->CastParticles("PxSpark", 5 + Local(5)/2, 50, 0, 0, 10, 50, RGB(128, 0, 0), RGB(255, 0, 0));
    }
 
  //Mutation(GRÖßER!!!111)
  SetObjDrawTransform(1000 +Local(5)*4, 0,0,0,1000 +Local(5)*4);
  }

//ab Jetz normal (mit inherited geth nich wegen graben)


  if (GetEffect("PossessionSpell", this())) return();

  if(Bait)
    if(ObjectDistance(Bait, this()) <= 15)
      if(GetAction() eq "Walk")
        {
        SetAction("Eat");
        SetXDir();
        }

  var pBait;
  if (GetCon()==100)
   if (!Random(5))
     while (pBait = FindObject( 0, -250,-250,500,500, 0,0,0,NoContainer(), pBait))
      if(WildcardMatch(GetAction(pBait), "*MeatBait*"))
       if (Random(100) < pBait->~IsBait())
        {
         SetCommand(this(), "Follow", pBait);
         Bait = pBait;
        }

  if(GetAction() S= "Dig" && GetComDir() == COMD_None)
    SetAction("Walk");

  if (Contained()) return();

  if (GetAction() ne "Walk" && GetAction() ne "Swim") return();

  if (InLiquid() && GetBreath() <= 25 && GetComDir() != COMD_Up())
   SetComDir(COMD_Up());

  var pBurning;
  if(pBurning = FindObject(0, -!GetDir()*80,-30,80,35, OCF_OnFire()))
  {
   if(!Random(5) || !SpaceToFlee(pBurning))
   {
    if(GetDir()) SetComDir(COMD_Right());
    else SetComDir(COMD_Left());
    Jump();
   }
   else
   {
    if(GetComDir()==COMD_Left()) SetComDir(COMD_Right());
    else SetComDir(COMD_Left());
   }
  }

  if (Random(2)) return();

  if(Stuck()) return(DigFree());

  if (!Random(ReproductionRate())) 
    Reproduction();


  if (GetAction() eq "Walk")
   if (!Random(3)) return(Jump());

  if (GetAction() eq "Walk")
    if (!Random(3)) return(SetAction("Sit"));

//KEIN GRABEN!

  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}
