/*-- Jetpack --*/

#strict 3

#include GEAR

//wieviel Zeit bis zum nächsten Boost
local timeleft;
local pHazard;

public func GetGearType()	{ return(GEAR_Backpack); }
public func IsEquipment()	{ return(true); }


public func GearEffect(object pH) {
  pHazard = pH;
}

public func GearEffectReset(object pH) {
  pHazard = nil;
}
/*
public func GetFMData(int d) {
  if(d == FM_AmmoLoad) return(5);
  if(d == FM_AmmoID)   return(GSAM);
}
*/
//Commands, die vom Clonk übergeben werden.

public func ControlUpDouble(object pCon)
{
  // nur als Gear
  if(!( pCon->~HasGear(nil,JTPK))) return(nil);
  
  if(!CheckStart(pCon)) return(nil);
  //Clonk wird hochgechubst (Doppelsprung)
  //Schub
  SetJetpackAction(pCon);
  pCon->SetXDir((GetXDir() ?? 0)/3-10 + 20 * (pCon->GetDir() ?? 0));
  pCon->SetYDir((GetYDir() ?? 0)/3-60);
//  AddEffect("Jumping", pCon, 1, 2, pCon);
  pCon->JumpStart();
  
  StartToFly(pCon);
  
  return(1);
}

public func ControlRightDouble(object pCon)
{
  // nur als Gear
  if(!( pCon->~HasGear(nil,JTPK))) return(nil);
  
  if(!CheckStart(pCon)) return(nil);
  //Nun noch herausfinden, ob rechts unsere Blickrichtung ist
  if(Equal(pCon->GetDir(), DIR_Right))
    FlyForward(pCon);
  else
    return(nil);

  return(1);
}

public func ControlLeftDouble(object pCon)
{
  // nur als Gear
  if(!( pCon->~HasGear(nil,JTPK))) return(nil);
  
  if(!CheckStart(pCon)) return(nil);
  //Nun noch herausfinden, ob links unsere Blickrichtung ist
  if(Equal(pCon->GetDir(), DIR_Left))
    FlyForward(pCon);
  else
    return(nil);

  return(1);	
}

public func FlyForward(object pClonk)
{
  //Aktion setzen
  SetJetpackAction(pClonk);
  pClonk->SetXDir((pClonk->GetXDir() ?? 0)/3-50 + 100 * (pClonk->GetDir() ?? 0));
  pClonk->SetYDir((pClonk->GetYDir() ?? 0)/3-15);
  
  pClonk->JumpStart();

  StartToFly(pClonk);
}

private func StartToFly(object pClonk) {
  //Zeugs fürs Fliegen hinzufügen
  AddEffect("JetpackDelay", pClonk, 170, 5, this());
  AddEffect("Thrust", pClonk, 170, 1, this());
  timeleft = 15;
  //DoAmmo(GetFMData(FM_AmmoID),-GetFMData(FM_AmmoLoad),this());
}

public func CheckStart(object pCon) {
  if(!ReadyToFly(pCon)) return(nil);
/*  var ammoload = GetFMData(FM_AmmoLoad);
  var ammoid = GetFMData(FM_AmmoID);
  if(!CheckAmmo(ammoid,ammoload))
    if(!Reload())
      return(0);*/
  return(1);
}
/*
public func Reload(bool nomsg) {
  if(!pHazard) return(false);
  if(reload) return(false);
  
  //checken ob wir genug Sprit zum nochmal fliegen haben
  if(!CheckAmmo(GetFMData(FM_AmmoID),GetFMData(FM_AmmoLoad),pHazard)) {
    var ammoid = GetFMData(FM_AmmoID);
    if(!nomsg) PlayerMessage(GetOwner(pHazard),"$NotEnoughAmmo$",pHazard,ammoid);
    return(false);
  }
  //doch, haben wir, also Tank auffüllen:
  DoAmmo(GetFMData(FM_AmmoID),-GetFMData(FM_AmmoLoad),pHazard);
  DoAmmo(GetFMData(FM_AmmoID),+GetFMData(FM_AmmoLoad),this());
  reload=true;
  return(true);
}
*/
public func ReadyToFly(object pClonk)
{
  // So ist der Clonk in der Luft?
  if(Or(Or(Or(!!SEqual(pClonk->GetAction(), "JumpArmed")
  , !!SEqual(pClonk->GetAction(), "Jump"))
  , !!SEqual(pClonk->GetAction(), "Dive"))
  , !!SEqual(pClonk->GetAction(), "JetpackFlight"))) {
    // Dürfen wir überhaupt?
    if(GetEffect("JetpackDelay", pClonk)) return(nil);
  
    return(true);
  }
  return(nil);
}

public func FxThrustStart(object pTarget, int iEffectNumber) {
  iEffectNumber ??= 0;
  EffectVar(nil,pTarget,iEffectNumber) = AddLight(75,RGB(255,127,nil),pTarget,GLOW);
  EffectVar(nil,pTarget,iEffectNumber)->SetVertex(nil,nil,(GetDir(pTarget) ?? 0)*10-5,nil,2);
  Sound("Flamer", nil, pTarget, 100,nil,+1);
}

public func FxThrustTimer(object pTarget, int iEffectNumber, int iTime) {

  /* halb kopiert aus Missile.c4d -- */
  iEffectNumber ??= 0;
  iTime ??= 0;
  var dist = Distance(nil,nil,GetXDir(pTarget),GetYDir(pTarget));
  var angle = Angle(nil,nil,GetXDir(pTarget),GetYDir(pTarget));
  var maxx = +(Sin(angle,(dist ?? 0)/10) ?? 0);
  var maxy = -(Cos(angle,(dist ?? 0)/10) ?? 0);
  var ptrdist = 66;

  for(var i=nil; (i ?? 0)<(dist ?? 0); S42MigrationAssign(i, "+=", (ptrdist ?? 0))) {

    var x = -(maxx ?? 0)*(i ?? 0)/(dist ?? 0)+(GetX(pTarget) ?? 0)-(GetDir(pTarget) ?? 0)*20+10;
    var y = -(maxy ?? 0)*(i ?? 0)/(dist ?? 0)+(GetY(pTarget) ?? 0);

    var rand = RandomX(-30,30);
    var xdir = +(Sin((angle ?? 0)+(rand ?? 0),20) ?? 0);
    var ydir = -(Cos((angle ?? 0)+(rand ?? 0),20) ?? 0);

    CreateParticle("Thrust",x,y,(GetXDir(pTarget) ?? 0)/2,(GetYDir(pTarget) ?? 0)/2,RandomX(80,100),RGBa(255,200,200,60),nil,nil);
    CreateParticle("Smoke2",x,y,xdir,ydir,RandomX(100,120),RGBa(220,200,180,nil),nil,nil);
  }
  /* -- halb kopiert aus Missile.c4d */
  
  if((iTime ?? 0) > 20)
    return(-1);
}

public func FxThrustStop(object pTarget, int iEffectNumber) {
  iEffectNumber ??= 0;
  Sound("Flamer", nil, pTarget, 100,nil,-1);
  if(EffectVar(nil,pTarget,iEffectNumber))
    RemoveObject(EffectVar(nil,pTarget,iEffectNumber));
    
  // reload
  //Reload(true);
}

public func FxJetpackDelayTimer(object pTarget, int iEffectNumber)
{
  iEffectNumber ??= 0;
  S42MigrationStep(timeleft, -1, true);
  if((timeleft ?? 0) <= 0) return(-1);
/*
  if(pTarget->GetAction() ne "JetpackFlight"
  && pTarget->GetAction() ne "BackFlip"
  && pTarget->GetAction() ne "JumpArmed"
  && pTarget->GetAction() ne "Jump")
  {
    return(-1);
  }*/
} 

public func FxJetpackDelayStop(object pTarget, int iEffectNumber) {
iEffectNumber ??= 0;
}

protected func Update() {
  // nicht angelegt
  if(!pHazard) return;
  /*
  if(reload) {
    timeleft -= 15;
    if(timeleft <= 0) {
      timeleft = 0;
      reload = false;
      // echt nervig
      //var ammoload = GetFMData(FM_AmmoLoad);
      //var ammoid = GetFMData(FM_AmmoID);
      //PlayerMessage(GetOwner(pHazard),"$Reloaded$",pHazard,ammoload,ammoid);
    }
  }*/
}

private func SetJetpackAction(object pCon) {
  if(GetActMapVal("Name","JetpackFlight",GetID(pCon)))
	pCon->SetAction("JetpackFlight");
  // ansonsten so lassen wie is.
}


public func IsRefineryProduct() { return(1); }