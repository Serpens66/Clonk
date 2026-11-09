/*-- Plasmaschuss --*/

#strict


local XDir, YDir, count;

/* Initialisierung */

protected func Initialize()
{  
  Sound("Launch");  
  return(1);
}

public func Launch()
{
  if (Par(0) == DIR_Left()) { XDir = -150; SetR(270); }
  else { XDir = +150;  SetR(90); }
  YDir = 0;
  
  SetXDir(XDir);
  SetYDir(YDir);
  SetAction("Flying");
  return(1);
}

protected func Hit()
{
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))Explode(6);
  else FakeExplode(6,0,0,6);
  return(1);
}

protected func Flight()
{
  var target = FindObject(0,-15,-10,30,20,OCF_Alive(),"",0,NoContainer());
  if (!target) target=FindObject(0,-20,-15,40,30,OCF_Entrance(),"",0,NoContainer());
   if(!target) 
   target = FindObject2(Find_AtPoint(),Find_NoContainer(),Find_Func("IsBulletTarget")); // alle bullettargets
     if(!(GetOCF(target) & OCF_Living && !(GetOCF(target) & OCF_Alive))) { if(!FindObject(NOFF))  target=target; else target=0; if(FindObject(NOFF)) if(Hostile(GetOwner(target), GetOwner())) target=target; else target=0; }
    else  target=0;
  if (!Random(6)) Smoke(0,0,8);
  if (GetActTime() > 50) RemoveObject();
  
  SetXDir(XDir);
  SetYDir(YDir);
  if (!target) return(1); 
  DoDmg(5,DMG_Explosion,target); 
  if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))Explode(3);
  else FakeExplode(3,0,0,3);
  //if (GetCategory(target) && C4D_Structure()) DoDamage(-5,target);  
  return(1);
}
