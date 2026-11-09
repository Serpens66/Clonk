/* Dragon-Rettungskapsel */

#strict

public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }


//Anfangswerte
protected func Initialize()
{
  local pilot, count, dirb, heat;
  count=0;
  dirb=0;
  heat=0;
  SetEntrance(1);
  return(1);
}

//Nichts Aufnehmen
protected func RejectCollect()
{
  return(0);
}

//Schaden & Aufprall
func ContactTop()
{
  if (GetXDir()>5)  return(Explosion());
  if (GetYDir()>5)  return(Explosion());
  return(1);
}

func ContactBottom()
{
  if (GetXDir()>40)  return(Explosion());
  if (GetYDir()>40)  return(Explosion());
  if (SEqual(GetAction(),"Gelandet")) return(1);
  count=0;
  SetAction("Gelandet");
  return(1);
}


func Explosion()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Exit(pilot,0,5);
  if(heat) RemoveObject(heat);
  Explode(25);
  return(1);
}

public func OnDmg(int iDmg, int iType)
{ 
  //Mögliche Resistenzen berücksichtigen
  if(iType & DMG_Projectile)
    return(70);
  if(iType & DMG_Fire)
    return(90);
  if(iType & DMG_Explosion)
    return(-10);
  if(iType & DMG_Energy)
    return(-40);
  if(iType & DMG_Bio)
    return(50);
}

func Damage()
{
  if(GetDamage()<70) return(0);
  Explosion();
  return(1);
}


func Wasser()
{
  SetRDir(0);
  if(heat) RemoveObject(heat);
  Bubble(RandomX(-10,10),RandomX(-10,10));
  return(1);
}


//Steuerung
func ContainedUp()
{
  SetComDir(COMD_Up());
  return(1);
}


func ContainedDown()
{
  if (!SEqual(GetAction(),"Gelandet")) return(1);
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Exit(pilot,0,5);
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
  heat=CreateObject(PRQ_,Cos( GetR() + 270 , -20),Sin( GetR() + 270, -20),GetOwner());
  ObjectSetAction(heat,"Attaching",this());
  }
  count++;
  CreateParticle("Thrust",Cos( GetR() + 270 , -10),Sin( GetR() + 270, -10),GetXDir()/2,GetYDir()/2,RandomX(80,100),RGBa(255,200,200,60),0,0);
  CreateParticle("Smoke2",Cos( GetR() + 270 , -10),Sin( GetR() + 270, -10),GetXDir()/2,GetYDir()/2,RandomX(100,120),RGBa(220,200,180,0),0,0);
  if(count>30) SetAction("Fliegen"); 
  return(1);
}


func Aufrichten()
{
  if(GetR()>0) SetRDir(-5);
  if(GetR()<0) SetRDir(5);
  CreateParticle("Thrust",Cos( GetR() + 270 , -10),Sin( GetR() + 270, -10),GetXDir()/2,GetYDir()/2,RandomX(80,100),RGBa(255,200,200,60),0,0);
  if(Inside(GetR(),-3,3))
  {
  SetRDir(0);
  SetAction("Bremsflug");
  }
  return(1);
}


func Bremsflug()
{
  if(GetYDir()>30) SetYDir(GetYDir()-4);
  if(GetXDir()>dirb)  SetXDir(GetXDir()-2);
  if(GetXDir()<dirb)  SetXDir(GetXDir()+2);
  CreateParticle("Thrust",Cos( GetR() + 270 , -10),Sin( GetR() + 270, -10),GetXDir()/2,GetYDir()/2,RandomX(80,100),RGBa(255,200,200,60),0,0);
  return(1);
}

func Gelandet()
{
  if(heat) RemoveObject(heat);
  count++;
  if(count<6) return(Sound("Bip"));
  if(count==6)return(Sound("BipBipBip"));
  if(count>8) Explosion();
  return(1);
}

func Gewassert()
{
  if(heat) RemoveObject(heat);
  Bubble(RandomX(-10,10),RandomX(-10,10));
  count++;
  if(count<6) return(Sound("Bip"));
  if(count==6)return(Sound("BipBipBip"));
  if(count>8) Explosion();
  return(1);
}

