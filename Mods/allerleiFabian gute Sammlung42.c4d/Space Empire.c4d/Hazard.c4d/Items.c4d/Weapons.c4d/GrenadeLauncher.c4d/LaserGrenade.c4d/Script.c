/*-- Lasergranate --*/

#strict

// Ist eine Granate
#include GREN

local alreadyhit; // was wir schon getroffen haben

func IsExplosiv() {return(0);}

func HitObject()
{
  if(ActIdle())
    CastLasers();
}

func CastLasers()
{
  Sound("LaserGrenade");
  SetSpeed(0,0);
  SetAction("Float");
  Schedule("RemoveObject();", 21, 0, this());

	alreadyhit = CreateObject(L_OS,0,0,NO_OWNER);

  var angle = Random(360);
  for(var i = 0 ; i < 10 ; i++)
  {
    CreateObject(LASR, 0,0, GetOwner())->Set(angle, 10, 150, 20, this(), 0, 2);
    angle += RandomX(20,50);
  }
}

func LaserStrike(object pObj)
{
	if(!pObj)
		return();

	if(alreadyhit->AddItem(pObj))
	  // Schaden!
  	DoDmg(iDamage, DMG_Energy, pObj);
  else
  	// Objekt wurde schon mal getroffen -> weniger schaden
  	DoDmg(iDamage/10, DMG_Energy, pObj);
  return(1);
}

func Destruction()
{
	if(alreadyhit)
		RemoveObject(alreadyhit);
}

func ExplodeDelay() { if(explodedelay) {return(explodedelay);};  return(45); }
