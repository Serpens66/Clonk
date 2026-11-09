/*-- Neues Script --*/

#strict 3

#appendto WIPF

//Normalowipf zum Killerwipf machen

public func MakeKillerWipf() {
 Local(1)=1;
 //Killerwipf Färbung initialisieren
 SetClrModulation(RGB(RandomX(200,220),RandomX(200, 220),RandomX(200,220)));
 return(1); }
 

public func IsThreat() {if(Equal(Local(1), 1)) return(true); return(nil);}  // nur wenn killerwipf

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
  if(Equal(Local(1), 1))  return(1); // killerwipfe lassen sich nicht einsammeln
  return (_inherited(pNewCont, a,b,c));
  }

protected func Activity()
{
  if(Equal(Local(1), 1)) {
  //Gefährlichkeitsstufe hochzählen
  //irgendwann reichts aber auch :o 
  if((Local(5) ?? 0) <= 80) S42MigrationStep(Local(5), 1, true);
 
  // Monster!! (Wir sind die Beatsteakes aus Berlin! Berlin!)
  if (!Random(50)) Sound("DeadRowl");
  
  //Zum Angriffsziel!
      //die vorherige Überprüfung ist hochgradig essentziell!
 if(FindObject2(Find_OCF(OCF_CrewMember), Find_InRect(-450,  -50, 900, 100),Find_NoContainer()))
  SetCommand(this(), "MoveTo",FindObject2(Find_OCF(OCF_CrewMember), Find_InRect(-450,  -50, 900, 100),Find_NoContainer()));
   
  //Mach den Angriffssprung!
   if(FindObject2(Find_OCF(OCF_CrewMember), Find_InRect(-54,  -50, nil, 100),Find_NoContainer())) {SetXDir(-21); SetYDir(-25); }
   if(FindObject2(Find_OCF(OCF_CrewMember), Find_InRect(-255,  -50, nil, 100),Find_NoContainer())) {SetXDir(21); SetYDir(-25); }
   
  //Bissattacke!
  var ptarget = FindObject2(Find_OCF(OCF_CrewMember), Find_InRect(-10,  -15, 20, 30),Find_NoContainer());
 if(ptarget)
    {  
    Punch(ptarget, 2 +(Local(5) ?? 0) / 4 );
    if(ptarget) ptarget->CastParticles("PxSpark", 5 + (Local(5) ?? 0)/2, 50, nil, nil, 10, 50, RGB(128, nil, nil), RGB(255, nil, nil));
    }
 
  //Mutation(GRÖßER!!!111)
  SetObjDrawTransform(1000 +(Local(5) ?? 0)*4, nil,nil,nil,1000 +(Local(5) ?? 0)*4);
  }

//ab Jetz normal (mit inherited geth nich wegen graben)


  if (GetEffect("PossessionSpell", this())) return;

  if(Bait)
    if((ObjectDistance(Bait, this()) ?? 0) <= 15)
      if(!!SEqual(GetAction(), "Walk"))
        {
        SetAction("Eat");
        SetXDir();
        }

  var pBait;
  if (Equal(GetCon(), 100))
   if (!Random(5))
     while (pBait = FindObject( nil, -250,-250,500,500, nil,nil,nil,NoContainer(), pBait))
      if(WildcardMatch(GetAction(pBait), "*MeatBait*"))
       if ((Random(100) ?? 0) < (pBait->~IsBait() ?? 0))
        {
         SetCommand(this(), "Follow", pBait);
         Bait = pBait;
        }

  if(And(!!SEqual(GetAction(), "Dig"), Equal(GetComDir(), COMD_None)))
    SetAction("Walk");

  if (Contained()) return;

  if (And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Swim"))) return;

  if (And(And(InLiquid(), (GetBreath() ?? 0) <= 25), !Equal(GetComDir(), COMD_Up)))
   SetComDir(COMD_Up);

  var pBurning;
  if(pBurning = FindObject(nil, -(!GetDir() ?? 0)*80,-30,80,35, OCF_OnFire))
  {
   if(Or(!Random(5), !SpaceToFlee(pBurning)))
   {
    if(GetDir()) SetComDir(COMD_Right);
    else SetComDir(COMD_Left);
    Jump();
   }
   else
   {
    if(Equal(GetComDir(), COMD_Left)) SetComDir(COMD_Right);
    else SetComDir(COMD_Left);
   }
  }

  if (Random(2)) return;

  if(Stuck()) return(DigFree());

  if (!Random(ReproductionRate())) 
    Reproduction();


  if (!!SEqual(GetAction(), "Walk"))
   if (!Random(3)) return(Jump());

  if (!!SEqual(GetAction(), "Walk"))
    if (!Random(3)) return(SetAction("Sit"));

//KEIN GRABEN!

  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}
