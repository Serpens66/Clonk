/*-- Lasergranate --*/

#strict 3

// Ist eine Granate
#include GREN

local alreadyhit; // was wir schon getroffen haben

func IsExplosiv() {return(nil);}

func HitObject()
{
  if(ActIdle())
    CastLasers();
}

func CastLasers()
{
  Sound("LaserGrenade");
  SetSpeed(nil,nil);
  SetAction("Float");
  Schedule("RemoveObject();", 21, nil, this());

	alreadyhit = CreateObject(L_OS,nil,nil,NO_OWNER);

  var angle = Random(360);
  for(var i = nil ; (i ?? 0) < 10 ; S42MigrationStep(i, 1, true))
  {
    CreateObject(LASR, nil,nil, GetOwner())->Set(angle, 10, 150, 20, this(), nil, 2);
    S42MigrationAssign(angle, "+=", (RandomX(20,50) ?? 0));
  }
}

func LaserStrike(object pObj)
{
	if(!pObj)
		return;

	if(alreadyhit->AddItem(pObj))
	  // Schaden!
  	DoDmg(iDamage, DMG_Energy, pObj);
  else
  	// Objekt wurde schon mal getroffen -> weniger schaden
        DoDmg((iDamage ?? 0)/10, DMG_Energy, pObj);
  return(1);
}

func Destruction()
{
	if(alreadyhit)
		RemoveObject(alreadyhit);
}

func ExplodeDelay() { if(explodedelay) {return(explodedelay);};  return(45); }
