/*-- Jetbelt --*/

#strict
#include GEAR

local user, hud, buffer, slow;

/* Einstellungen */
private func MaxSpeed(){return(20);}
private func MaxAccel(){return(100);}//iPrec = 100
private func Consumption(){return(30);}//iPrec = 100

/* Hazard-Gear */
public func GetGearType()	{ return(GEAR_Backpack); }
public func IsEquipment()	{ return(true); }

public func Hit(){Sound("ClonkHit*");}
public func Hit3(){if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(40); else FakeExplode(40,0,0,40);}//:°

public func GearEffect(object pClonk)
{
  user = pClonk;
  SetClonkOverlay();
  SetAction("StandBy");
  hud = CreateObject(VHUD,0,0,GetOwner());
  hud->Set(this(),pClonk);
  UpdateHUD(hud);
}

public func GearEffectReset(object pClonk)
{
  GearUnbckle();
}

public func GearUnbckle()
{
  ResetClonkOverlay();
  user = 0;
  if(hud)
    RemoveObject(hud);
  Stop();//Ausschalten.
  SetAction("Idle");//Ganz aus!
}

public func IsActive()
{
  if(!user) return();
  if(GetAction() eq "Active") return(true);
}

/* Steuerung */
public func UpDouble(){return(Switch(COMD_Up));}
public func DownDouble(){return(Switch(COMD_Down));}
public func LeftDouble(){return(Switch(COMD_Left));}
public func RightDouble(){return(Switch(COMD_Right));}

public func Up(){if(IsActive())return(Start(COMD_Up));}
public func Down(){if(IsActive())return(Start(COMD_Down));}
public func Left(){if(IsActive())return(Start(COMD_Left));}
public func Right(){if(IsActive())return(Start(COMD_Right));}

public func Slow(){if(IsActive())return(Start(COMD_Stop));}

public func Update(int comdir){if(IsActive())return(Start(comdir));}

private func Switch(int iDir)
{
  if(IsActive())
  {
    if(iDir == COMD_Up)
      Stop();
  }
  else
  {
    Start(iDir);
  }
}

private func Stop()
{
  if(IsActive())
  {
    Sound("Jetbelt",0,user,0,0,-1);
    Sound("JetbeltStop",0,user);
    if(hud)
      hud->Recharge();
  }
    
  SetAction("StandBy");
  SetComDir(COMD_None);
  
  return();
}

private func Start(int iDir)
{
  if(!IsReady()) return();

  var jnr = GetPlrCoreJumpAndRunControl(user->GetController());
  
  if(iDir == COMD_Stop)
  {
    if(!jnr)
      slow = !slow;
    else
      slow = true;
      
    return();
  }
  else
  {
    if(jnr)
      slow = false;
  
    iDir = DoDirection(iDir);
  }
  
  SetComDir(iDir);
  
  if(!IsActive())
  {
    SetOwner(GetOwner(user));
    SetController(GetController(user));
    
    Sound("Jetbelt",0,user,0,0,+1);
    SetUserAction();
    SetAction("Active");
  }
}

public func UpdateHUD(object pHUD,pClonk)
{
  pHUD->Charge(GetAmmo(GSAM,user)*100/GSAM->MaxAmmo(user));
  pHUD->Fuel(GetAmmo(GSAM,user),GSAM->MaxAmmo(user),GetName());
  pHUD->MasterFuel(GSAM,GetAmmo(GSAM,user));//2. Parameter ist doof/überflüssig.
  
  if((GetFuel() <= 5000) && IsActive())
    pHUD->SetFlash(true);
  else
    pHUD->SetFlash(false);
}

private func GetFuel()
{
  if(NoAmmo()) return(10000);
  return(GetAmmo(GSAM,user)*100+buffer);
}

private func GetBuffer()
{
  if(NoAmmo()) return(100);
  return(buffer);
}

private func DoBuffer(int iAmount)
{
  if(NoAmmo()) return();
  buffer = BoundBy(buffer+iAmount,0,100);
}

private func RefillBuffer()
{
  if(NoAmmo()) return(true);

  var fuel = DoAmmo(GSAM,-1,user);
  if(!fuel) return(false);
  
  buffer = 100;
  return(true);
}

private func DoDirection(int comdir)
{
  return(comdir);
  var cur_comdir = GetComDir();
  //if(ComDir2Angle(cur_comdir)-ComDir2Angle(comdir)) cur_comdir = COMD_Stop;

  cur_comdir = ComDirTransform(cur_comdir, comdir);
  return(cur_comdir);
}

private func UpdateTimer()
{
  if(!user) return();
  
  //GetProcedure ist hier Performance schonender als GetAction. :D
  if(user->GetProcedure() eq "NONE"
  || user->GetProcedure() eq "KNEEL"
  || user->GetProcedure() eq "SWIM"
  || user->GetProcedure() eq "FIGHT"
  || user->GetProcedure() eq "DIG"
  || user->GetProcedure() eq "BRIDGE"
  || user->GetAction() eq "BackFlip"
  || user->GetAction() eq "Tumble"
  || user->GetAction() eq "Dive")
    ResetClonkOverlay();
  else
    SetClonkOverlay();
}

private func ActiveTimer()
{
  if(!IsReady()) return(Stop());
  if(user->GetAction() eq "Tumble") return(Stop());
  
  UpdateTimer();
  
  var r = ComDir2Angle(GetComDir());
  
  var xmax = MaxSpeed();
  var ymax = MaxSpeed();
  
  if(slow)
  {
    xmax /= 4;
    ymax /= 4;
  }
  
  var xdir = BoundBy(GetXDir(user)*100+Sin(r,MaxAccel()),-xmax*100,+xmax*100);
  var ydir = BoundBy(GetYDir(user)*100-Cos(r,MaxAccel()),-ymax*100,+ymax*100);
  
  
  /*var xdir = GetXDir(user)*100;
  if(Abs(xdir) < xmax*100)
    xdir = BoundBy(xdir+Sin(r,MaxAccel()),-xmax*100,+xmax*100);
  else
  {
    xdir *= 10/11;
    if(xdir < 0)
    {
      if(xdir > +xmax*100)
        xdir = +xmax*100;
    }
    else
    {
      if(xdir < -xmax*100)
        xdir = -xmax*100;
    }
  }
  
  var ydir = GetYDir(user)*100;
  if(Abs(ydir) < ymax*100)
    ydir = BoundBy(ydir-Cos(r,MaxAccel()),-ymax*100,+ymax*100);
  else
  {
    ydir *= 10/11;
    if(ydir < 0)
    {
      if(ydir > +ymax*100)
        ydir = +ymax*100;
    }
    else
    {
      if(ydir < -ymax*100)
        ydir = -ymax*100;
    }
  }*/
  

  SetXDir(xdir,user,1000);
  SetYDir(ydir-GetGravityAccel4K(1000),user,1000);//4K-Lib sorgt für Pseudo-Schwerelosigkeit. ;D
  
  if(!GetBuffer()) RefillBuffer();
  DoBuffer(-Consumption());
  
  hud->Recharge(Min(Distance(xdir/100,ydir/100),MaxSpeed())*100/MaxSpeed());//^^
    
  r-=180;
  
  var dist = Distance(0,0,GetXDir(user),GetYDir(user));
  var maxx = +Sin(r,dist/10);
  var maxy = -Cos(r,dist/10);
  var ptrdist = 50;

  for(var i=0; i<dist; i+=ptrdist)
  {
    var x = -maxx*i/dist+GetX(user)-GetDir(user)*12+6;
    var y = -maxy*i/dist+GetY(user)+3;

    var rand = RandomX(-30,30);
    var xdir = +Sin(r+rand,20);
    var ydir = -Cos(r+rand,20);

    CreateParticle("Thrust",x,y,GetXDir(user)/2,GetYDir(user)/2,RandomX(50,70),RGBa(255,200,200),user,true);
    CreateParticle("Smoke2",x,y,xdir,ydir,RandomX(40,60),RGBa(220,200,180),user,true);
  }
}

public func IsReady()
{
  if(!GetFuel()) return();

  if(user->IsJumping()
  || user->GetAction() eq "Tumble"
  || user->GetAction() eq "BackFlip")
    return(1);
}

private func SetUserAction()
{
 if(user->~IsArmed())
 {
   if(user->GetAction() ne "JumpArmed")
     user->SetAction("JumpArmed");
 }
 else
   if(user->GetAction() ne "Jump")
     user->SetAction("Jump");
}

private func Departure()
{
  Stop();
  SetAction("Idle");
}

private func Destruction()
{
  Stop();
  SetAction("Idle");
}

private func ComDir2Angle(int iDir)
{
  if(iDir == COMD_Down) return(180);
  if(iDir == COMD_Left) return(270);
  if(iDir == COMD_Right) return(90);
  if(iDir == COMD_DownLeft) return(225);
  if(iDir == COMD_DownRight) return(135);
  if(iDir == COMD_UpLeft) return(315);
  if(iDir == COMD_UpRight) return(45);
  //if(COMD_Up) return(0);
  return(0);
}

private func ComDir2String(int iDir)//:D Debug!
{
  if(iDir == COMD_Stop) return("COMD_Stop");
  if(iDir == COMD_Down) return("COMD_Down");
  if(iDir == COMD_Left) return("COMD_Left");
  if(iDir == COMD_Right) return("COMD_Right");
  if(iDir == COMD_DownLeft) return("COMD_DownLeft");
  if(iDir == COMD_DownRight) return("COMD_DownRight");
  if(iDir == COMD_UpLeft) return("COMD_UpLeft");
  if(iDir == COMD_UpRight) return("COMD_UpRight");
  return("COMD_Up");
}

//Grafik-Extras
public func ResetClonkOverlay()
{
  if(!user) return();
  SetGraphics(0,user,0,WeaponDrawLayer+1);
}

public func SetClonkOverlay()
{
  if(!user) return();

  //SetGraphics(0,user,GetID(),WeaponDrawLayer+1,GFXOV_MODE_Base);
  SetGraphics(0,user,GetID(),WeaponDrawLayer+1,GFXOV_MODE_Action,"BagPack");

  var x = -1;
  if(GetDir(user) == DIR_Left)
    x = 1;
    
  //SetObjDrawTransform(650*x,0,x*5500,0,900,-3*1000,user,WeaponDrawLayer+1);
  SetObjDrawTransform(1000*x,0,x*5000,0,1000,-1000,user,WeaponDrawLayer+1);
}