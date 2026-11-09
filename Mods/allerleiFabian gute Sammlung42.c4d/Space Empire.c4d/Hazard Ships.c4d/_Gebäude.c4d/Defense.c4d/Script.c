/*-- Verteidigungsstellung --*/

#strict 3
public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }
public func IsMachine()     { return(true); }

local target, rocky, gun, maxdist;

func Initialize()
{
  SetAction("Off");
  maxdist=900;
  //Automatikkanone Erstellen
  gun=CreateObject(DFG_,nil,nil,GetOwner());
  ObjectSetAction(gun,"Attaching",this());
  SetActionData(256 , gun);
  gun->Rotinit(-20,-110,110);
  gun->Bezug(this());

  //Rocketeer Erstellen
  rocky=CreateObject(DFR_,nil,nil,GetOwner());
  ObjectSetAction(rocky,"Attaching",this());
  SetActionData(256 + 1, rocky);
  rocky->Rotinit(30,-110,+110);
  rocky->Bezug(this());
}

func SetUser(user)
{
  var owner=GetOwner(user);
  SetOwner(owner,this());
  SetOwner(owner,gun);
  SetOwner(owner,rocky);
}

func Off()
{
  SetAction("Off");
  rocky->Loose();
  gun->Loose();
}

private func Search()
{
  if(!target) (target = FindTarget());
  if(SEqual(GetAction(target),"Dead")) return(Loose());
  if(SEqual(GetAction(),"Wrecked")) return(Loose());
  if(!PathFree(GetX(),GetY(),GetX(target),GetY(target))) return(Loose());
  if((ObjectDistance(this(),target) ?? 0)>(maxdist ?? 0)) return(Loose());
  if(And(!(Equal(GetOwner(this()), -1)), (And(!Hostile(GetOwner(this()),GetOwner(target)), !(Equal(GetOwner(target), -1)))))) 	Loose();
}

func Loose()
{
  target=nil;
}

func FindTarget()
{
  var pT;
  for(pT in FindObjects( Find_Distance(maxdist),
                         Find_And(
                           Find_OCF(OCF_Alive),
                           Find_Func("CheckEnemy",this())))) {
    // Sichtlinie?
    if(And(!(Equal(GetOwner(this()), -1)), (And(!Hostile(GetOwner(this()),GetOwner(pT)), !(Equal(GetOwner(pT), -1)))))) continue;
    if(!PathFree(GetX(),GetY(),GetX(pT),GetY(pT))) continue;
	//Lass ma die armen Tiere in frieden und so...
	if(Equal(GetID(pT), SHRK)) continue;
    if(Equal(GetID(pT), FISH)) continue;
	if(Equal(GetID(pT), BIRD)) continue;
    if(Equal(GetID(pT), SNKE)) continue;
	if(Equal(GetID(pT), WIPF)) continue;
	rocky->Target(pT);	
	if((ObjectDistance(this(),target) ?? 0)>600) continue;
    gun->Target(pT);
	}
	target=pT;
}

public func OnDmg(int iDmg, int iType)
{ 
  //Mögliche Resistenzen berücksichtigen
  iDmg ??= 0;
  iType ??= 0;
  if((iType ?? 0) & (DMG_Projectile ?? 0))
    return(90);
  if((iType ?? 0) & (DMG_Fire ?? 0))
    return(80);
  if((iType ?? 0) & (DMG_Explosion ?? 0))
    return(80);
  if((iType ?? 0) & (DMG_Energy ?? 0))
    return(40);
  if((iType ?? 0) & (DMG_Bio ?? 0))
    return(50);
}

func Damage()
{
  if(And(Wrecked(), (GetDamage() ?? 0)<250)) SetAction("Off");
  if((GetDamage() ?? 0)<300) return(nil);
  SetAction("Wrecked");
  SetOwner(-1);
  if((GetDamage() ?? 0)>500) Explosion();
  return(1);
}

func Explosion()
{
  Removal();
  Explode(50);
}

func Removal()
{
  if(gun) RemoveObject(gun);
  if(rocky) RemoveObject(rocky);
}

func Burn()
{
  CreateParticle("Smoke2",10,5,RandomX(-4,4),-15,50,RGBa(220,200,180,nil),nil,nil);
  CreateParticle("Smoke2",-12,8,RandomX(-4,4),-15,40,RGBa(220,200,180,nil),nil,nil);
  return(1);
}

func Wrecked() {return(SEqual(GetAction(),"Wrecked"));}
















