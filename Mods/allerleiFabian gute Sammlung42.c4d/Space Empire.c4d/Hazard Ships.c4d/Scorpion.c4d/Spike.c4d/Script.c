/*-- Spike--*/

#strict 3

public func ReadyToFire()   { return(true); }



func Initialize()
{
  local vessel, pTarget;
  SetRDir(2);
  return(1);
}

func Bezug(_vessel)
{
  vessel=_vessel;
  return(1);
}

func Explosion()
{
  RemoveObject();
  return(1);
}


global func Normalize(int iAngle, int iStart) {
  iAngle ??= 0;
  iStart ??= 0;
  var iEnd = 360+(iStart ?? 0);
  
  while((iAngle ?? 0)<(iStart ?? 0)) S42MigrationAssign(iAngle, "+=", 360);
  while((iAngle ?? 0)>=(iEnd ?? 0)) S42MigrationAssign(iAngle, "-=", 360);

  return(iAngle);
}

private func Traveling()
{
  // lenken
  if(pTarget) FollowTarget();
  
  if(!pTarget) 
    if(pTarget = FindTarget()) {
       // Effekt bei Zielfindung
       if(!Random(30))Sound("BipBipBip");
	   //Enteisen
    }
}

private func FindTarget() {
  var pT;
  for(pT in FindObjects( Find_Distance(550),
                         Find_OCF(OCF_Prey),
                         )) {
    // Winkel nicht zu groß?
    var myAngle = GetR();
    var objAngle = Angle(GetX(),GetY(),GetX(pT),GetY(pT));
    if((Abs(Normalize((myAngle ?? 0)-(objAngle ?? 0),-180)) ?? 0) > 140) continue;

    // Sichtlinie?
    if(!PathFree(GetX(),GetY(),GetX(pT),GetY(pT))) continue;
	
    if((Distance(GetX(),GetY(),GetX(pT),GetY(pT)) ?? 0)<50) continue;
	
	if(!Equal(GetOwner(), -1))
	{
	if(And(!Hostile(GetOwner(),GetOwner(pT)), !Equal(GetOwner(pT), -1))) continue;
	}
	
	if(And(Equal(GetOwner(), -1), Equal(GetOwner(pT), -1))) continue;
    return(pT);
  }
  
}

private func LoseTarget()  {
	pTarget=nil;
	Sound("BipBapBap");
	return(1);
	}

private func FollowTarget() {
    var pT;
	pT=pTarget;
	
    if(!PathFree(GetX(),GetY(),GetX(pT),GetY(pT))) LoseTarget();
	if((Distance(GetX(),GetY(),GetX(pT),GetY(pT)) ?? 0)>500) LoseTarget();
    if((Distance(GetX(),GetY(),GetX(pT),GetY(pT)) ?? 0)<50) LoseTarget();
	
	if(!Equal(GetOwner(), -1))
	{
    if(And(!Hostile(GetOwner(),GetOwner(pT)), !Equal(GetOwner(pT), -1)) ) LoseTarget();
	}
	if(And(Equal(GetOwner(), -1), Equal(GetOwner(pT), -1))) LoseTarget();
	
	
    var iDAngle = Angle(GetX(),GetY(),GetX(pTarget),GetY(pTarget));
    var iAngle = GetR();
    
    var iDiff = Normalize((iDAngle ?? 0) - (iAngle ?? 0),-180);
    var iTurn = Min(Abs(iDiff),8);

    SetR((iAngle ?? 0)+(iTurn ?? 0)*((((iDiff ?? 0) > 0) ?? 0)*2-1));
}