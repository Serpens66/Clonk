/*-- Granate --*/

#strict 3

local iDamage;
local shooter;
local explodedelay;

func Construction(object byObj) {
  // nichts? :C
  if(!byObj)
    return;
  // Waffe?
  shooter = GetShooter(byObj);
}

func Launch(int xdir, int ydir, int iDmg)
{
  xdir ??= 0;
  ydir ??= 0;
  iDmg ??= 0;
  AddEffect("HitCheck", this(), 1, 1, nil, SHT1,shooter);
  AddEffect("Grenade", this(), 1, 1, this());
  SetSpeed(xdir, ydir);
  iDamage = iDmg;
  if(!iDamage)
    // Standardexplosion
    iDamage = 30;
}

// Aufprall
func Hit()
{
  var x, y;
  // Rechts angestoßen
  if((GetContact(this(), -1) ?? 0) & (CNAT_Right ?? 0) )
    x=-((Abs((GetXDir() ?? 0)/4) ?? 0)+(Random(3) ?? 0));
  // Links angestoßen
  else if((GetContact(this(), -1) ?? 0) & (CNAT_Left ?? 0) ) x=(Abs((GetXDir() ?? 0)/4) ?? 0)+(Random(3) ?? 0);
  // Nichts davon: Alte YDir
  else x=(GetXDir() ?? 0)/2;
  
  // Oben angestoßen
  if((GetContact(this(), -1) ?? 0) & (CNAT_Top ?? 0)) y=Abs((GetYDir() ?? 0)/3);
  // Unten angestoßen
  // Warum verlieren Objekte auf glattem Boden ihre YDir..?
  // Hacken wir halt ein bisschen.. Zufall drauf.
  // Ich kann nicht mehr so recht lesen was da steht und ich denke das hat einen gewissen Vorteil.
  else if((GetContact(this(), -1) ?? 0) & (CNAT_Bottom ?? 0)) y=-((Max(Abs((GetYDir() ?? 0)/4), (Abs(GetXDir()) ?? 0)/4) ?? 0)+(Random(3) ?? 0));
  // Nichts davon: Alte YDir
  else y=(GetYDir() ?? 0)/2;
  // Rumdopsen.. Komplizierter als man meint
  SetXDir(x);
  SetYDir(y);
  // Letztendlich hört man den Aufprall auch noch
  Sound("GrenadeHit*");
}

local noExplode;

func HitObject()
{
  RemoveEffect("Grenade", this(),nil,1);
//  CastObjects(BRNH,Random(3)+6,30);
  BlastObjects(GetX(), GetY(), (iDamage ?? 0)/4);
if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID())))  Explode(iDamage); else FakeExplode(iDamage,nil,nil,iDamage);
}

func ChangeExplodeDelay(delay) {explodedelay = delay;}

func ExplodeDelay() { if(explodedelay) {return(explodedelay);}; return(110); }

func IsBulletTarget(id id)
{
  // Kann von anderen Geschossen getroffen werden
  return(true);
}

public func NoDecoDamage() { return(true); }

func FxGrenadeTimer(object target, int effect, int time)
{
  effect ??= 0;
  time ??= 0;
  if((time ?? 0) > (ExplodeDelay() ?? 0)) return(HitObject());
  var vel=(Abs(GetXDir()) ?? 0)+(Abs(GetYDir()) ?? 0);
  var alpha=Max(nil,100-(vel ?? 0));
  
  CreateParticle("Smoke2", -(GetXDir() ?? 0)/6, -(GetYDir() ?? 0)/6, RandomX(-15, 15), -5,
                       (vel ?? 0)/2+(RandomX(20, 60) ?? 0), RGBa(100,100,100,50+(alpha ?? 0)));
}

func FxGrenadeDamage()
{
  HitObject();
}

func IsBouncy() { return(true); }
