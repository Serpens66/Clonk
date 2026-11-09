/*-- Verteidigungsstellung --*/

#strict
public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }
public func IsMachine()     { return(true); }

local target, rocky, gun, maxdist;

func Initialize()
{
  SetAction("Off");
  maxdist=900;
  //Automatikkanone Erstellen
  gun=CreateObject(DFG_,0,0,GetOwner());
  ObjectSetAction(gun,"Attaching",this());
  SetActionData(256 , gun);
  gun->Rotinit(-20,-110,110);
  gun->Bezug(this());

  //Rocketeer Erstellen
  rocky=CreateObject(DFR_,0,0,GetOwner());
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
  if(ObjectDistance(this(),target)>maxdist) return(Loose());
  if(!(GetOwner(this())==-1) && (!Hostile(GetOwner(this()),GetOwner(target)) && !(GetOwner(target)==-1))) 	Loose();
}

func Loose()
{
  target=0;
}

func FindTarget()
{
  var pT;
  for(pT in FindObjects( Find_Distance(maxdist),
                         Find_And(
                           Find_OCF(OCF_Alive),
                           Find_Func("CheckEnemy",this())))) {
    // Sichtlinie?
    if(!(GetOwner(this())==-1) && (!Hostile(GetOwner(this()),GetOwner(pT)) && !(GetOwner(pT)==-1))) continue;
    if(!PathFree(GetX(),GetY(),GetX(pT),GetY(pT))) continue;
	//Lass ma die armen Tiere in frieden und so...
	if(GetID(pT)==SHRK) continue;
    if(GetID(pT)==FISH) continue;
	if(GetID(pT)==BIRD) continue;
    if(GetID(pT)==SNKE) continue;
	if(GetID(pT)==WIPF) continue;
	rocky->Target(pT);	
	if(ObjectDistance(this(),target)>600) continue;
    gun->Target(pT);
	}
	target=pT;
}

public func OnDmg(int iDmg, int iType)
{ 
  //Mögliche Resistenzen berücksichtigen
  if(iType & DMG_Projectile)
    return(90);
  if(iType & DMG_Fire)
    return(80);
  if(iType & DMG_Explosion)
    return(80);
  if(iType & DMG_Energy)
    return(40);
  if(iType & DMG_Bio)
    return(50);
}

func Damage()
{
  if(Wrecked()&&GetDamage()<250) SetAction("Off");
  if(GetDamage()<300) return(0);
  SetAction("Wrecked");
  SetOwner(-1);
  if(GetDamage()>500) Explosion();
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
  CreateParticle("Smoke2",10,5,RandomX(-4,4),-15,50,RGBa(220,200,180,0),0,0); 
  CreateParticle("Smoke2",-12,8,RandomX(-4,4),-15,40,RGBa(220,200,180,0),0,0);
  return(1);
}

func Wrecked() {return(SEqual(GetAction(),"Wrecked"));}
















