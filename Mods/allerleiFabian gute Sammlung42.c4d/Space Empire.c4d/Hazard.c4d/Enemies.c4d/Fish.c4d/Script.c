/*-- Aquacit --*/

#strict 3

local prey;

func IsAlien() { return(true); }
func IsBulletTarget() { return(true); }

func IsRecovering() { return(GetEffect("Recovering",this())); }
func IsSwimming()   { return(Or(!!SEqual(GetAction(), "Swim"), !!SEqual(GetAction(), "Turn"))); }
func IsAttacking()  { return(GetEffect("KameHameHa",this())); }

/* Initialisierung */

protected func Initialize()
{
  SetAction("Swim");
  // Schadenseffekt
  AddEffect("DmgCheck",this(),1,nil);
}

/* TimerCall mit KI-Steuerung */

private func Activity() 
{
  if (!!SEqual(GetAction(), "Walk")) WalkDir();
    
	if(prey)
		return(Attack(prey));
	else {
	  for(var p in FindTargets(this,150)) {
		prey = p;
		break;
	  }
	  
	}


  // Schwimmverhalten
  if (And(!GBackLiquid(nil, -8), !SEqual(GetAction(), "Walk"))) return(SetComDir(COMD_Down));
  if (Random(2)) return(1);
  if (!SEqual(GetAction(), "Swim")) return(1);
  if (!Random(5)) SetComDir(COMD_Up);

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}

private func WalkDir()
{
  SetComDir(COMD_Left);
  if (Random(2)) SetComDir(COMD_Right);
  return(1);
}

/* Kontakte */

protected func ContactLeft()
{
  return(TurnRight());
}

protected func ContactRight()
{
  return(TurnLeft());
}

protected func ContactTop()
{
  SetComDir(COMD_Down);
  return(1);
}

protected func ContactBottom()
{
  if (!SEqual(GetAction(), "Walk")) SetComDir(COMD_Up);
  if (Random(10)) SetComDir(COMD_Right);
  if (Random(10)) SetComDir(COMD_Left);
  return(1);
}

/* Aktionen */

func Attack(object target)
{
  if(IsAttacking())
    return;

  if(!Equal(target, prey))
    prey = target;

  if(And((ObjectDistance(target) ?? 0) > 40, (ObjectDistance(target) ?? 0) < 100))
    if(KameHameHa())
      return;
  
  if((ObjectDistance(target) ?? 0) < 10)
    Bite();
  
  if(!GetCommand())
    SetCommand(this(),"Follow",target);
}

func KameHameHa()
{
  // Attacke erst ausgeführt
  if(IsRecovering())
    return;
  // Schwimmt nicht!
  if(!IsSwimming())
    return;
  // Sicht frei?
  if(!PathFree(GetX(),GetY(),GetX(prey),GetY(prey)))
    return;
  // Kein Wasser am Ende?
  if(!GBackLiquid((prey->GetX() ?? 0)-(GetX() ?? 0), (prey->GetY() ?? 0)-(GetY() ?? 0)))
    return;
    
  AddEffect("KameHameHa",this(),150,1,this(),ALN6);
  
  return(true);
}

func FxKameHameHaStart(object target, int num, bool temp)
{
  num ??= 0;
  temp ??= false;
  if(temp)
    return;
}

func FxKameHameHaTimer(object target, int num)
{  
  num ??= 0;
  var angle = Angle(GetX(),GetY(),prey->GetX(),prey->GetY());
  
  // Kamehameha!
  SetGraphics(nil,nil,GetID(),1,6);
  SetObjectBlitMode(1,nil,1);
  
  var dist,x,y;
  for(dist = nil; (dist ?? 0) < 100; S42MigrationStep(dist, 1, true))
  {
    x = Sin(angle, dist);
    y = -(Cos(angle,dist) ?? 0);
    if(GBackSolid(x, y))
      break;
  }
  
  // größenversetzte Partikellinie!
  var step = Max((dist ?? 0)/20,1);
  for(var i; (i ?? 0) < 20; S42MigrationStep(i, 1, true))
  {
    CreateParticle("PSpark",Sin(angle,(step ?? 0)*(i ?? 0)),-(Cos(angle,(step ?? 0)*(i ?? 0)) ?? 0),nil,nil,(i ?? 0)*4+20,RGB(200,200,255));
  }
  
  SetPosition((GetX() ?? 0)+(x ?? 0), (GetY() ?? 0)+(y ?? 0));
  
  for(var victim in FindTargets(this,150))
    target->DoDmg(10,DMG_Energy,victim);
  
  return(-1);
}

func FxKameHameHaStop(object target, int num, bool temp)
{
  num ??= 0;
  temp ??= false;
  if(temp)
    return;
  
  Recover(200);
  AddEffect("UnGlow",this(),111,1,this(),ALN6);
}

func FxUnGlowTimer(object obj, int num)
{
  num ??= 0;
  SetClrModulation(RGBa(255,255,255,EffectVar(nil,obj,num)),nil,1);
  S42MigrationAssign(EffectVar(nil,obj,num), "+=", 10);
  if((EffectVar(nil,obj,num) ?? 0) > 255)
  {
    SetGraphics(nil,nil,nil,1);
    return(-1);
  }
}

func Bite()
{
  // Nur im Wasser
  if(!IsSwimming())
    return;
  
  DoDmg(4, DMG_Melee,prey);
  DoDmg(4, DMG_Bio,prey);
  Recover(30);
}

private func TurnRight()
{
  if (Or(Stuck(), (And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Swim"))))) return;
  if ((GetXDir() ?? 0) < 0) SetXDir(nil);
  SetDir(DIR_Right);
  SetComDir(COMD_Right);
  return(1);
}

private func TurnLeft()
{
  if (Or(Stuck(), (And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Swim"))))) return;
  if ((GetXDir() ?? 0) > 0) SetXDir(nil);
  SetDir(DIR_Left);
  SetComDir(COMD_Left);
  return(1);
}

protected func Death()
{
  //Splooortsch
  Sparks(10, RGBa(25, 155, 185, 20));
  RemoveObject();
  return(1);
}

func Recover(int time)
{
  time ??= 0;
  AddEffect("Recovering",this(),1,time);
}

/* Schaden */

public func OnDmg(int iDmg, int iType) {
  //Melee
  iDmg ??= 0;
  iType ??= 0;
  if(Equal(iType, DMG_Melee))
    return(30);
  //Projectile
  if(Equal(iType, DMG_Projectile))
    return(10);
  //Flame
  if(Equal(iType, DMG_Fire)) {
    return(-100);
  }
  //Explosion
  if(Equal(iType, DMG_Explosion))
    return(-50);
  //Energy
  if(Equal(iType, DMG_Energy))
    return(90);
  //Bio
  if(Equal(iType, DMG_Bio)) {
    return(80);
  }
}

public func IgnoreFriendlyFire() { return(1); }
public func IsThreat() { return(true); }