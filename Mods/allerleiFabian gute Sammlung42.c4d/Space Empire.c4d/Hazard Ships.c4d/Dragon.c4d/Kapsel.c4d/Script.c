/* Dragon-Rettungskapsel */

#strict 3

public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }


//Anfangswerte
protected func Initialize()
{
  local pilot, count, dirb, heat;
  count=nil;
  dirb=nil;
  heat=nil;
  SetEntrance(1);
  return(1);
}

//Nichts Aufnehmen
protected func RejectCollect()
{
  return(nil);
}

//Schaden & Aufprall
func ContactTop()
{
  if ((GetXDir() ?? 0)>5)  return(Explosion());
  if ((GetYDir() ?? 0)>5)  return(Explosion());
  return(1);
}

func ContactBottom()
{
  if ((GetXDir() ?? 0)>40)  return(Explosion());
  if ((GetYDir() ?? 0)>40)  return(Explosion());
  if (SEqual(GetAction(),"Gelandet")) return(1);
  count=nil;
  SetAction("Gelandet");
  return(1);
}


func Explosion()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Exit(pilot,nil,5);
  if(heat) RemoveObject(heat);
  Explode(25);
  return(1);
}

public func OnDmg(int iDmg, int iType)
{ 
  //Mögliche Resistenzen berücksichtigen
  iDmg ??= 0;
  iType ??= 0;
  if((iType ?? 0) & (DMG_Projectile ?? 0))
    return(70);
  if((iType ?? 0) & (DMG_Fire ?? 0))
    return(90);
  if((iType ?? 0) & (DMG_Explosion ?? 0))
    return(-10);
  if((iType ?? 0) & (DMG_Energy ?? 0))
    return(-40);
  if((iType ?? 0) & (DMG_Bio ?? 0))
    return(50);
}

func Damage()
{
  if((GetDamage() ?? 0)<70) return(nil);
  Explosion();
  return(1);
}


func Wasser()
{
  SetRDir(nil);
  if(heat) RemoveObject(heat);
  Bubble(RandomX(-10,10),RandomX(-10,10));
  return(1);
}


//Steuerung
func ContainedUp()
{
  SetComDir(COMD_Up);
  return(1);
}


func ContainedDown()
{
  if (!SEqual(GetAction(),"Gelandet")) return(1);
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Exit(pilot,nil,5);
  Sound("Airlock1");
  return(1);
}

func ContainedLeft()
{
  if (!SEqual(GetAction(),"Bremsflug")) return(1);
  dirb=-10;
  return(1);
}

func ContainedRight()
{
  if (!SEqual(GetAction(),"Bremsflug")) return(1);
  dirb=10;
  return(1);
}

func ContainedDigDouble()
{
  Explosion();
  return(1);
}

//Abfangsystem

func Start()
{
  if(!heat)
  {
  heat=CreateObject(PRQ_,Cos( (GetR() ?? 0) + 270 , -20),Sin( (GetR() ?? 0) + 270, -20),GetOwner());
  ObjectSetAction(heat,"Attaching",this());
  }
  S42MigrationStep(count, 1, true);
  CreateParticle("Thrust",Cos( (GetR() ?? 0) + 270 , -10),Sin( (GetR() ?? 0) + 270, -10),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(80,100),RGBa(255,200,200,60),nil,nil);
  CreateParticle("Smoke2",Cos( (GetR() ?? 0) + 270 , -10),Sin( (GetR() ?? 0) + 270, -10),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(100,120),RGBa(220,200,180,nil),nil,nil);
  if((count ?? 0)>30) SetAction("Fliegen");
  return(1);
}


func Aufrichten()
{
  if((GetR() ?? 0)>0) SetRDir(-5);
  if((GetR() ?? 0)<0) SetRDir(5);
  CreateParticle("Thrust",Cos( (GetR() ?? 0) + 270 , -10),Sin( (GetR() ?? 0) + 270, -10),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(80,100),RGBa(255,200,200,60),nil,nil);
  if(Inside(GetR(),-3,3))
  {
  SetRDir(nil);
  SetAction("Bremsflug");
  }
  return(1);
}


func Bremsflug()
{
  if((GetYDir() ?? 0)>30) SetYDir((GetYDir() ?? 0)-4);
  if((GetXDir() ?? 0)>(dirb ?? 0))  SetXDir((GetXDir() ?? 0)-2);
  if((GetXDir() ?? 0)<(dirb ?? 0))  SetXDir((GetXDir() ?? 0)+2);
  CreateParticle("Thrust",Cos( (GetR() ?? 0) + 270 , -10),Sin( (GetR() ?? 0) + 270, -10),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(80,100),RGBa(255,200,200,60),nil,nil);
  return(1);
}

func Gelandet()
{
  if(heat) RemoveObject(heat);
  S42MigrationStep(count, 1, true);
  if((count ?? 0)<6) return(Sound("Bip"));
  if(Equal(count, 6))return(Sound("BipBipBip"));
  if((count ?? 0)>8) Explosion();
  return(1);
}

func Gewassert()
{
  if(heat) RemoveObject(heat);
  Bubble(RandomX(-10,10),RandomX(-10,10));
  S42MigrationStep(count, 1, true);
  if((count ?? 0)<6) return(Sound("Bip"));
  if(Equal(count, 6))return(Sound("BipBipBip"));
  if((count ?? 0)>8) Explosion();
  return(1);
}

