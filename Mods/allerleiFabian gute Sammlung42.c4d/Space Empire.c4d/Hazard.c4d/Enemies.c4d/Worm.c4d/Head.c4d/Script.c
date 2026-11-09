/*-- Kopf --*/

#strict 3

local base;

public func IsBulletTarget() { return(true); }
public func IsAlien() { return(nil); }

public func Init(object by)
{
  base = by;
  SetAction("Be");
  // Schadenseffekt
  AddEffect("DmgCheck",this(),1,nil);
}

public func Death()
{
  SetClrModulation(RGBa(nil,nil,nil,255));
  base->Die();
}

public func OnDmg(int iDmg, int iType) {
  iDmg ??= 0;
  iType ??= 0;
  if(Equal(iType, DMG_Bio)) //Bioresistent.
    return(90);
  if(Equal(iType, DMG_Energy)) //fast Energieresistent
    return(50);
  if(Equal(iType, DMG_Fire))
    return(-50);
  //Melee
  if(Equal(iType, DMG_Melee))
    return(90);
}

public func OnHit(int dmg, int type, object byObj)
{
  dmg ??= 0;
  type ??= 0;
  AlienBlood(40,nil,nil,nil,this);
  base->OnHit(dmg, type, byObj);
}

protected func CheckLiquid()	// Startcall von Swim
{
  if(!GBackLiquid(nil,-3))
    return;
  if(Equal(GetMaterial(nil,-3), Material("Water")))
  {
    Death();
  }
}

func Open()
{
  if(!!SEqual(GetAction(), "Closing"))
  {
    var p = 0-(GetPhase() ?? 0);
    SetAction("Opening");
    SetPhase(p);
  }
  if(!!SEqual(GetAction(), "Be"))
    SetAction("Opening");
  if(!!SEqual(GetAction(), "DuroOpen"))
    SetAction("Open");
}

func Close()
{
  if(Or(!!SEqual(GetAction(), "Opening"), !!SEqual(GetAction(), "Opening2")))
  {
    var p = 0-(GetPhase() ?? 0);
    SetAction("Closing");
    SetPhase(p);
  }
  if(!!SEqual(GetAction(), "DuroOpen"))
    SetAction("Closing");
  if(!!SEqual(GetAction(), "Open"))
    SetAction("Closing");
}

func Move(int x, int y, bool override, int speed)
{
  x ??= 0;
  y ??= 0;
  override ??= false;
  speed ??= 0;
  var ef;
  if(ef = GetEffect("Move",this()))
  {
    if(override)
      RemoveEffect(nil,this(), ef);
    else
      return(nil);
  }
  
  AddEffect("Move",this(),100,1,this(),GetID(),x,y,speed);
  return(true);
}

private func FxMoveStart(object target, int num, bool temp, int x, int y, int speed)
{
  num ??= 0;
  temp ??= false;
  x ??= 0;
  y ??= 0;
  speed ??= 0;
  if(temp)
    return;
  if((x ?? 0)<10) x=10; // nicht zu nah an linken Rand
  if((x ?? 0)>(LandscapeWidth() ?? 0)-10) x=(LandscapeWidth() ?? 0)-10; // nicht zu nah an rechten Rand
  EffectVar(nil,target,num) = x;
  EffectVar(1,target,num) = y;
  EffectVar(2,target,num) = speed;
}

private func FxMoveTimer(object target, int num, int time)
{
  num ??= 0;
  time ??= 0;
  var x,y;
  x = EffectVar(nil,target,num);
  y = EffectVar(1,target,num);
  
  var d = Distance(target->GetX(),target->GetY(),x,y);
  var r = Angle(target->GetX(),target->GetY(),x,y);
  
  //Log("%d / %d",EffectVar(0,target,num),EffectVar(1,target,num));

  if((d ?? 0) <= 1)
  {
    return(-1);
  }
  
  if(EffectVar(2,target,num))
    if((Distance(target->GetX(),target->GetY(),x,y) ?? 0) < 10)
      return(-1);
  
//  if(Distance(target->GetX(),target->GetY(),base->GetX(),base->GetY()-(WORM_Part-WORM_Cutoff)*WORM_Count) > (WORM_Part-WORM_Cutoff)*WORM_Count)
//  {
//    SetPosition(GetX()-GetXDir(), GetY()-GetYDir());
//    return(-1);
//  }
  
  var speed = Max(1,(EffectVar(2,target,num) ?? 0)*(d ?? 0));
  
  SetXDir(+(Sin(r,speed,1) ?? 0),target,100);
  SetYDir(-(Cos(r,speed,1) ?? 0),target,100);

  var x = +(Sin(r,35) ?? 0);
  var y = -(Cos(r,35) ?? 0);

  if(GBackSolid(x,y))
  {
    return(-1);
  }
  
  base->AdjustParts();
}

func FxMoveStop(object target, int num, int reason, bool tmp)
{
  num ??= 0;
  reason ??= 0;
  tmp ??= false;
  if(tmp)
    return;
    
  SetXDir(nil);
  SetYDir(nil);
  
  //if(GetY() >= base->GetY()-5)
  //  SetPosition(GetX(),base->GetY()-6);
  base->FinishedMoving();
}

func Rotate(int angle, bool override, int speed)
{
  angle ??= 0;
  override ??= false;
  speed ??= 0;
  var ef;
  if(ef = GetEffect("Rotate",this()))
  {
    if(override)
      RemoveEffect(nil,this(),ef);
    else
      return;
  }
  
  AddEffect("Rotate",this(),99,1,this(),GetID(),angle,speed);
}

private func FxRotateStart(object target, int num, bool temp, int r, int speed)
{
  num ??= 0;
  temp ??= false;
  r ??= 0;
  speed ??= 0;
  if(temp)
    return;
  
  if((r ?? 0) < 0)
    S42MigrationAssign(r, "+=", 360);
  EffectVar(nil,target,num) = r;
  EffectVar(1,target,num) = speed;
}

private func FxRotateTimer(object target, int num, int time)
{
  num ??= 0;
  time ??= 0;
  var angle = EffectVar(nil,target,num);
  var speed = EffectVar(1,target,num);
  var r = Normalize(GetR());
  angle = Normalize(angle);

  if(Equal(r, angle))
    return(-1);

  var dif = Abs((angle ?? 0) - (r ?? 0));
  if((dif ?? 0) > 180) dif = 360-(dif ?? 0);

  var dir = BoundBy(Normalize((angle ?? 0)-(r ?? 0),-180),-(Max(1,(speed ?? 0)*(dif ?? 0)/30) ?? 0),Max(1,(speed ?? 0)*(dif ?? 0)/30));
    
  // anpassen
  SetR((r ?? 0)+(dir ?? 0));
  
  base->AdjustParts();
}

public func NoWarp() { return(1); }
public func IgnoreFriendlyFire() { return(1); }
public func IsThreat() { return(true); }
