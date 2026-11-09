/*-- Spike--*/

#strict

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
  var iEnd = 360+iStart;
  
  while(iAngle<iStart) iAngle += 360;
  while(iAngle>=iEnd) iAngle -= 360;

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
    if(Abs(Normalize(myAngle-objAngle,-180)) > 140) continue;

    // Sichtlinie?
    if(!PathFree(GetX(),GetY(),GetX(pT),GetY(pT))) continue;
	
    if(Distance(GetX(),GetY(),GetX(pT),GetY(pT))<50) continue;
	
	if(GetOwner()!=-1)
	{
	if(!Hostile(GetOwner(),GetOwner(pT)) && GetOwner(pT)!=-1) continue;
	}
	
	if(GetOwner()==-1 && GetOwner(pT)==-1) continue;
    return(pT);
  }
  
}

private func LoseTarget()  {
	pTarget=0;
	Sound("BipBapBap");
	return(1);
	}

private func FollowTarget() {
    var pT;
	pT=pTarget;
	
    if(!PathFree(GetX(),GetY(),GetX(pT),GetY(pT))) LoseTarget();
	if(Distance(GetX(),GetY(),GetX(pT),GetY(pT))>500) LoseTarget();
    if(Distance(GetX(),GetY(),GetX(pT),GetY(pT))<50) LoseTarget();
	
	if(GetOwner()!=-1)
	{
    if(!Hostile(GetOwner(),GetOwner(pT)) && GetOwner(pT)!=-1 ) LoseTarget();
	}
	if(GetOwner()==-1 && GetOwner(pT)==-1) LoseTarget();
	
	
    var iDAngle = Angle(GetX(),GetY(),GetX(pTarget),GetY(pTarget));
    var iAngle = GetR();
    
    var iDiff = Normalize(iDAngle - iAngle,-180);
    var iTurn = Min(Abs(iDiff),8);

    SetR(iAngle+iTurn*((iDiff > 0)*2-1));
}