/*-- Jetbelt --*/

#strict 3
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
public func Hit3(){if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(40); else FakeExplode(40,nil,nil,40);}//:°

public func GearEffect(object pClonk)
{
  user = pClonk;
  SetClonkOverlay();
  SetAction("StandBy");
  hud = CreateObject(VHUD,nil,nil,GetOwner());
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
  user = nil;
  if(hud)
    RemoveObject(hud);
  Stop();//Ausschalten.
  SetAction("Idle");//Ganz aus!
}

public func IsActive()
{
  if(!user) return;
  if(!!SEqual(GetAction(), "Active")) return(true);
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

public func Update(int comdir){comdir ??= 0;
    if(IsActive())return(Start(comdir));}

private func Switch(int iDir)
{
  iDir ??= 0;
  if(IsActive())
  {
    if(Equal(iDir, COMD_Up))
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
    Sound("Jetbelt",nil,user,nil,nil,-1);
    Sound("JetbeltStop",nil,user);
    if(hud)
      hud->Recharge();
  }
    
  SetAction("StandBy");
  SetComDir(COMD_None);
  
  return;
}

private func Start(int iDir)
{
  iDir ??= 0;
  if(!IsReady()) return;

  var jnr = GetPlrCoreJumpAndRunControl(user->GetController());
  
  if(Equal(iDir, COMD_Stop))
  {
    if(!jnr)
      slow = !slow;
    else
      slow = true;
      
    return;
  }
  else
  {
    if(jnr)
      slow = nil;
  
    iDir = DoDirection(iDir);
  }
  
  SetComDir(iDir);
  
  if(!IsActive())
  {
    SetOwner(GetOwner(user));
    SetController(GetController(user));
    
    Sound("Jetbelt",nil,user,nil,nil,+1);
    SetUserAction();
    SetAction("Active");
  }
}

public func UpdateHUD(object pHUD,pClonk)
{
  pHUD->Charge((GetAmmo(GSAM,user) ?? 0)*100/(GSAM->MaxAmmo(user) ?? 0));
  pHUD->Fuel(GetAmmo(GSAM,user),GSAM->MaxAmmo(user),GetName());
  pHUD->MasterFuel(GSAM,GetAmmo(GSAM,user));//2. Parameter ist doof/überflüssig.
  
  if(And(((GetFuel() ?? 0) <= 5000), IsActive()))
    pHUD->SetFlash(true);
  else
    pHUD->SetFlash(nil);
}

private func GetFuel()
{
  if(NoAmmo()) return(10000);
  return((GetAmmo(GSAM,user) ?? 0)*100+(buffer ?? 0));
}

private func GetBuffer()
{
  if(NoAmmo()) return(100);
  return(buffer);
}

private func DoBuffer(int iAmount)
{
  iAmount ??= 0;
  if(NoAmmo()) return;
  buffer = BoundBy((buffer ?? 0)+(iAmount ?? 0),nil,100);
}

private func RefillBuffer()
{
  if(NoAmmo()) return(true);

  var fuel = DoAmmo(GSAM,-1,user);
  if(!fuel) return(nil);
  
  buffer = 100;
  return(true);
}

private func DoDirection(int comdir)
{
  comdir ??= 0;
  return(comdir);
  var cur_comdir = GetComDir();
  //if(ComDir2Angle(cur_comdir)-ComDir2Angle(comdir)) cur_comdir = COMD_Stop;

  cur_comdir = ComDirTransform(cur_comdir, comdir);
  return(cur_comdir);
}

private func UpdateTimer()
{
  if(!user) return;
  
  //GetProcedure ist hier Performance schonender als GetAction. :D
  if(Or(Or(Or(Or(Or(Or(Or(Or(!!SEqual(user->GetProcedure(), "NONE")
  , !!SEqual(user->GetProcedure(), "KNEEL"))
  , !!SEqual(user->GetProcedure(), "SWIM"))
  , !!SEqual(user->GetProcedure(), "FIGHT"))
  , !!SEqual(user->GetProcedure(), "DIG"))
  , !!SEqual(user->GetProcedure(), "BRIDGE"))
  , !!SEqual(user->GetAction(), "BackFlip"))
  , !!SEqual(user->GetAction(), "Tumble"))
  , !!SEqual(user->GetAction(), "Dive")))
    ResetClonkOverlay();
  else
    SetClonkOverlay();
}

private func ActiveTimer()
{
  if(!IsReady()) return(Stop());
  if(!!SEqual(user->GetAction(), "Tumble")) return(Stop());
  
  UpdateTimer();
  
  var r = ComDir2Angle(GetComDir());
  
  var xmax = MaxSpeed();
  var ymax = MaxSpeed();
  
  if(slow)
  {
    S42MigrationAssign(xmax, "/=", 4);
    S42MigrationAssign(ymax, "/=", 4);
  }
  
  var xdir = BoundBy((GetXDir(user) ?? 0)*100+(Sin(r,MaxAccel()) ?? 0),-(xmax ?? 0)*100,(+(xmax ?? 0) ?? 0)*100);
  var ydir = BoundBy((GetYDir(user) ?? 0)*100-(Cos(r,MaxAccel()) ?? 0),-(ymax ?? 0)*100,(+(ymax ?? 0) ?? 0)*100);
  
  
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
  SetYDir((ydir ?? 0)-(GetGravityAccel4K(1000) ?? 0),user,1000);//4K-Lib sorgt für Pseudo-Schwerelosigkeit. ;D
  
  if(!GetBuffer()) RefillBuffer();
  DoBuffer(-(Consumption() ?? 0));
  
  hud->Recharge((Min(Distance((xdir ?? 0)/100,(ydir ?? 0)/100),MaxSpeed()) ?? 0)*100/(MaxSpeed() ?? 0));//^^
    
  S42MigrationAssign(r, "-=", 180);
  
  var dist = Distance(nil,nil,GetXDir(user),GetYDir(user));
  var maxx = +(Sin(r,(dist ?? 0)/10) ?? 0);
  var maxy = -(Cos(r,(dist ?? 0)/10) ?? 0);
  var ptrdist = 50;

  for(var i=nil; (i ?? 0)<(dist ?? 0); S42MigrationAssign(i, "+=", (ptrdist ?? 0)))
  {
    var x = -(maxx ?? 0)*(i ?? 0)/(dist ?? 0)+(GetX(user) ?? 0)-(GetDir(user) ?? 0)*12+6;
    var y = -(maxy ?? 0)*(i ?? 0)/(dist ?? 0)+(GetY(user) ?? 0)+3;

    var rand = RandomX(-30,30);
    var xdir = +(Sin((r ?? 0)+(rand ?? 0),20) ?? 0);
    var ydir = -(Cos((r ?? 0)+(rand ?? 0),20) ?? 0);

    CreateParticle("Thrust",x,y,(GetXDir(user) ?? 0)/2,(GetYDir(user) ?? 0)/2,RandomX(50,70),RGBa(255,200,200),user,true);
    CreateParticle("Smoke2",x,y,xdir,ydir,RandomX(40,60),RGBa(220,200,180),user,true);
  }
}

public func IsReady()
{
  if(!GetFuel()) return;

  if(Or(Or(user->IsJumping()
  , !!SEqual(user->GetAction(), "Tumble"))
  , !!SEqual(user->GetAction(), "BackFlip")))
    return(1);
}

private func SetUserAction()
{
 if(user->~IsArmed())
 {
   if(!SEqual(user->GetAction(), "JumpArmed"))
     user->SetAction("JumpArmed");
 }
 else
   if(!SEqual(user->GetAction(), "Jump"))
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
  iDir ??= 0;
  if(Equal(iDir, COMD_Down)) return(180);
  if(Equal(iDir, COMD_Left)) return(270);
  if(Equal(iDir, COMD_Right)) return(90);
  if(Equal(iDir, COMD_DownLeft)) return(225);
  if(Equal(iDir, COMD_DownRight)) return(135);
  if(Equal(iDir, COMD_UpLeft)) return(315);
  if(Equal(iDir, COMD_UpRight)) return(45);
  //if(COMD_Up) return(0);
  return(nil);
}

private func ComDir2String(int iDir)//:D Debug!
{
  iDir ??= 0;
  if(Equal(iDir, COMD_Stop)) return("COMD_Stop");
  if(Equal(iDir, COMD_Down)) return("COMD_Down");
  if(Equal(iDir, COMD_Left)) return("COMD_Left");
  if(Equal(iDir, COMD_Right)) return("COMD_Right");
  if(Equal(iDir, COMD_DownLeft)) return("COMD_DownLeft");
  if(Equal(iDir, COMD_DownRight)) return("COMD_DownRight");
  if(Equal(iDir, COMD_UpLeft)) return("COMD_UpLeft");
  if(Equal(iDir, COMD_UpRight)) return("COMD_UpRight");
  return("COMD_Up");
}

//Grafik-Extras
public func ResetClonkOverlay()
{
  if(!user) return;
  SetGraphics(nil,user,nil,(WeaponDrawLayer ?? 0)+1);
}

public func SetClonkOverlay()
{
  if(!user) return;

  //SetGraphics(0,user,GetID(),WeaponDrawLayer+1,GFXOV_MODE_Base);
  SetGraphics(nil,user,GetID(),(WeaponDrawLayer ?? 0)+1,GFXOV_MODE_Action,"BagPack");

  var x = -1;
  if(Equal(GetDir(user), DIR_Left))
    x = 1;
    
  //SetObjDrawTransform(650*x,0,x*5500,0,900,-3*1000,user,WeaponDrawLayer+1);
  SetObjDrawTransform(1000*(x ?? 0),nil,(x ?? 0)*5000,nil,1000,-1000,user,(WeaponDrawLayer ?? 0)+1);
}