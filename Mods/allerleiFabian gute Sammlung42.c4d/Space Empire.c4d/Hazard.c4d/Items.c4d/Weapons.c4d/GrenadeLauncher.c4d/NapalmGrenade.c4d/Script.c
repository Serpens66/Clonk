/*-- Napalmgranate --*/

#strict 3

// Ist eine Granate
#include GREN

func HitObject(object pTarget)
{

  for(var i=RandomX(5,6); (i ?? 0)>0; S42MigrationStep(i, -1))
  {
    var obj=CreateObject(NGFR, nil, nil, GetController());
    var xdir = (GetXDir() ?? 0)+(RandomX(-10,10) ?? 0);
    var ydir = (GetYDir() ?? 0)+(RandomX(-10,10) ?? 0);

    SetSpeed(xdir, ydir, obj);
  }
	
  return(_inherited());
}

func ExplodeDelay() { if(explodedelay) {return(explodedelay);}; return(40); }
