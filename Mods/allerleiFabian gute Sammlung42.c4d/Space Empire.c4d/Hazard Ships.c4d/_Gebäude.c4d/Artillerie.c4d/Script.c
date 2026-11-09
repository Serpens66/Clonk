/*-- Artilleriestellung --*/

#strict
public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }
public func IsMachine()     { return(true); }

local gun;

func Initialize()
{
  SetAction("Be");
  //Mörser
  gun=CreateObject(ARG_,0,0,GetOwner());
  ObjectSetAction(gun,"Attaching",this());
  SetActionData(256 , gun);
  gun->Rotinit(-20,-80,80);
  gun->Bezug(this());
}

func GetGun() {return(gun);}

func SetUser(user)
{
  var owner=GetOwner(user);
  SetOwner(owner,this());
  SetOwner(owner,gun);
}

func Be()
{
  SetAction("Be");
}

private func Search()
{

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
  if(Wrecked()&&GetDamage()<250) SetAction("Be");
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
}

func Burn()
{
  CreateParticle("Smoke2",10,5,RandomX(-4,4),-15,50,RGBa(220,200,180,0),0,0); 
  CreateParticle("Smoke2",-12,8,RandomX(-4,4),-15,40,RGBa(220,200,180,0),0,0);
  return(1);
}

func Wrecked() {return(SEqual(GetAction(),"Wrecked"));}
















