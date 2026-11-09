/*-- Plasmaschuss --*/

#strict


local XDir, YDir, count;

/* Initialisierung */

protected func Initialize()
{  
  return(1);
}

public func Launch()
{
  Sound("PlasmaShot");
  
  if (Par(0) == DIR_Left()) XDir = -125;
  else (XDir = +125);
  YDir = 0;
  
  SetXDir(XDir);
  SetYDir(YDir);
  SetAction("Flying");
  return(1);
}

protected func Hit()
{
  Smoke(0,0,8);
  RemoveObject();
  return(1);
}

protected func Flight()
{
  var obj = FindObject(0,-15,-10,30,20,OCF_Alive(),"",0,NoContainer());
   if(!obj) 
   obj = FindObject2(Find_AtPoint(),Find_NoContainer(),Find_Func("IsBulletTarget")); // alle bullettargets
     if(!(GetOCF(obj) & OCF_Living && !(GetOCF(obj) & OCF_Alive))) { if(!FindObject(NOFF))  obj=obj; else obj=0; if(FindObject(NOFF)) if(Hostile(GetOwner(obj), GetOwner())) obj=obj; else obj=0; }
    else  obj=0;
  if (GetActTime() > 12) RemoveObject();
  SetXDir(XDir);
  SetYDir(YDir);
  
  if (!obj) return(1);  
  //DoEnergy(-(1+Random(2)),obj);
  DoDmg(1,DMG_Energy,obj);
  RemoveObject();  
  return(1);
}
