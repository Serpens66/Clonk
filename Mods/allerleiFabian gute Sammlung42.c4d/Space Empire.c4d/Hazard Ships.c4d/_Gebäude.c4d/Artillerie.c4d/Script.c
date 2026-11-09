/*-- Artilleriestellung --*/

#strict 3
public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }
public func IsMachine()     { return(true); }

local gun;

func Initialize()
{
  SetAction("Be");
  //Mörser
  gun=CreateObject(ARG_,nil,nil,GetOwner());
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
  if(And(Wrecked(), (GetDamage() ?? 0)<250)) SetAction("Be");
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
}

func Burn()
{
  CreateParticle("Smoke2",10,5,RandomX(-4,4),-15,50,RGBa(220,200,180,nil),nil,nil);
  CreateParticle("Smoke2",-12,8,RandomX(-4,4),-15,40,RGBa(220,200,180,nil),nil,nil);
  return(1);
}

func Wrecked() {return(SEqual(GetAction(),"Wrecked"));}
















