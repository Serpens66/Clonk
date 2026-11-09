/*-- Schroff --*/

#strict 3

local countter;

func IsBulletTarget() { return(1); }
public func IsThreat() {return(true);}

private func Flying() { // wird alle 3 Frames aufgerufen
  if (!Random(2)) CastParticles("PSpark",(Random(10) ?? 0)+(countter ?? 0),RandomX(10,30),nil,nil,nil,Random(150),RGB(150,10,21));
  
  // Auftauchen, wenn der Atem ausgeht
  if(And(InLiquid(), (GetBreath() ?? 0) <= 35))
  {SetCommand(this(),"None");return(SetYDir(-15));}//SetComDir(COMD_Up());
  
  // Rückwärtsfliegen? umdrehen
  if(And((GetXDir() ?? 0) > 1, Equal(GetDir(), DIR_Left)))  return(SetDir(DIR_Right));
  if(And((GetXDir() ?? 0) < -1, Equal(GetDir(), DIR_Right))) return(SetDir(DIR_Left));
  
 for (var obj in FindObjects(Find_Distance(140),Find_NoContainer(),Find_OCF(OCF_Alive),Find_OCF(OCF_CrewMember), Find_Not(Find_Owner(-1)),
//Find_Func("IsAttackable"),
Sort_Distance()))
{
  if ((ObjectDistance(this(),obj) ?? 0)<20) if (!SEqual(GetAction(), "Attack")) return(SetAction("Attack"));
    // return(SetCommand(this(),"Follow",obj)); // follow is unendlich lang
    return(SetCommand(this(),"MoveTo",obj));
}

 // Nichts machen
  if(Random(30)) return(nil);
  
  // Nach oben oder unten fliegen
  if(Random(2)) SetYDir(RandomX(-5,20)); 
  
  // Nichts machen
  // if(Random(10)) return(0);
  
  // Umdrehen
  if(Random(2)) TurnRight();
  else TurnLeft();
 
}

func Attack()
{
 for (var obj in FindObjects(Find_Distance(20),Find_OCF(OCF_Alive),Find_NoContainer(), Find_OCF(OCF_CrewMember),Find_Not(Find_Owner(-1)),
//Find_Func("IsAttackable"),
Sort_Distance()))
  if((Abs(GetXDir()) ?? 0)>25) {Fling(obj,(GetXDir() ?? 0)/10+3,(GetYDir() ?? 0)/10-(RandomX(nil,3) ?? 0)); DoDmg(RandomX(2,10), DMG_Melee, obj);}
  else S42MigrationStep(countter, 1, true);
  if((countter ?? 0) >=12) Death(); // wenn zu oft zu langsam, dann explodieren, um clonk zu schaden.
 return(1);
}

protected func Initialize() {
  SetAction("Fly");
  countter = nil;
  SetYDir(100); // zu anfang zu boden stuerzen
}


/* Kontakte */

protected func ContactLeft()
{
  Log("ContactLeft");
  return(TurnRight());
}

protected func ContactRight()
{
  Log("ContactRight");
  return(TurnLeft());
}

protected func ContactTop()
{
  Log("ContactTop");
  SetYDir(RandomX(2,20)); 
}

protected func ContactBottom()
{
  Log("ContactBottom");
  SetYDir(RandomX(-1,-3)); 
}

/* Aktionen */

public func TurnRight()
{
    Log("TurnRight");
  // if (GetXDir() < 0) SetXDir(0);
  // SetDir(DIR_Right());
  SetComDir(COMD_Right);
  return(1);
}

public func TurnLeft()
{
    Log("TurnLeft");
  // if (GetXDir() > 0) SetXDir(0);
  // SetDir(DIR_Left());
  SetComDir(COMD_Left);
  return(1);
}

 
public func CatchBlow()	{ SetAction("Hurt"); }
public func Damage()	{ SetAction("Hurt"); }
protected func Death()	{ GameCallEx("Zaehler20");if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(Random(30)); else FakeExplode(30,nil,nil,30); }