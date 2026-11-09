/* Predator-Raumjäger */

#strict 3

public func IsHazardShip()  { return(true); }
public func IsBulletTarget(){ return(true); }
public func IsMachine()     { return(true); }
public func ReadyToFire()   { return(true); }

//Anfangswerte
protected func Initialize()
{
  local pilot, EMPDamaged;
  SetAction("Gelandet");
  SetEntrance(1);
  return(1);
}

//Nichts Aufnehmen
protected func RejectCollect()
{
  return(nil);
}

//Schaden & Aufprall
func ContactTop()
{
  if ((GetXDir() ?? 0)>15)  return(Explosion());
  if ((GetYDir() ?? 0)>15)  return(Explosion());
  return(1);
}

func ContactBottom()
{
  if ((GetXDir() ?? 0)>30)  return(Explosion());
  if ((GetYDir() ?? 0)>30)  return(Explosion());
  return(1);
}

func ContactLeft()
{
  if ((GetYDir() ?? 0)>40)  return(Explosion());
  if ((GetXDir() ?? 0)>75)  return(Explosion());
  if ((GetXDir() ?? 0)<-75) return(Explosion());
  if ((GetXDir() ?? 0)>10)  return(Funken());
  if ((GetXDir() ?? 0)<-10) return(Funken());
  return(1);
}

func ContactRight()
{
  if ((GetYDir() ?? 0)>40)  return(Explosion());
  if ((GetXDir() ?? 0)>75)  return(Explosion());
  if ((GetXDir() ?? 0)<-75) return(Explosion());
  if ((GetXDir() ?? 0)>10)  return(Funken());
  if ((GetXDir() ?? 0)<-10) return(Funken());
  return(1);
}

func Funken()
{
  CreateParticle("Funken",RandomX(-3,3),13,RandomX(-10,10),RandomX(-10,-20),RandomX(40,60));
  return(1);
}

func Explosion()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Exit(pilot);
 if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(40);
 else
  FakeExplode(40,nil,nil,40); //eine explosion, die kein schaden auf der erde macht
  return(1);
}

public func OnDmg(int iDmg, int iType)
{ 
  //Mögliche Resistenzen berücksichtigen
  iDmg ??= 0;
  iType ??= 0;
  if((iType ?? 0) & (DMG_Projectile ?? 0))
    return(80);
  if((iType ?? 0) & (DMG_Fire ?? 0))
    return(90);
  if((iType ?? 0) & (DMG_Explosion ?? 0))
    return(-10);
  if((iType ?? 0) & (DMG_Energy ?? 0))
    return(-40);
  if((iType ?? 0) & (DMG_Bio ?? 0))
    return(50);
}

func Damage()
{
  if((GetDamage() ?? 0)<100) return(nil);
  Explosion();
  return(1);
}

public func EMPShock()
{
  //Nicht zweimal.
  if( GetEffect("Damaged", this())) return(nil);
  EMPDamaged = 1;
  AddEffect("Damaged", this(), 190, 20, this(), GetID());
  SetEntrance(nil);
  return(1);
}

public func FxDamagedStart(object pTarget, int iEffectNumber, int iTemp)
{
iEffectNumber ??= 0;
iTemp ??= 0;
}

public func FxDamagedTimer(object pTarget, int iEffectNumber)
{
  iEffectNumber ??= 0;
  if(! Contained())
    CreateParticle("PxSpark", RandomX(-2, 2), RandomX(2, 2), RandomX(-2, 2), 4, 30, RGB(187, 214, 224));
}

public func FxDamagedStop(object pTarget, int iEffectNumber)
{
  iEffectNumber ??= 0;
  EMPDamaged = nil;
  SetEntrance(1);
}

func Wasser()
{
  SetRDir(nil);
  Bubble(RandomX(-10,10),RandomX(-10,10));
  return(1);
}

func Gelandet()
{
//Schaden sichtbar machen
  if((GetDamage() ?? 0)>=50)
  CreateParticle("Smoke2",Cos( (GetR() ?? 0) + 270 , -10+(Random(20) ?? 0)),Sin( (GetR() ?? 0) + 270, -10+(Random(20) ?? 0)),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,GetDamage(),RGBa(220,200,180,nil),nil,nil);
  if((GetDamage() ?? 0)>=80)
  {
  CreateParticle("Smoke2",Cos( (GetR() ?? 0) + 270 , Random(10)),Sin( (GetR() ?? 0) + 270, Random(10)),(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,RandomX(30,50),RGBa(220,200,180,nil),nil,nil);
  }
//Besitzer zuweisen
  if(pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this()))
  {
  SetOwner(GetOwner(pilot));
  SetEntrance(nil);
  }
  return(1);
}

//Steuerung & Schubregelung
func ContainedUp()
{
  return(1);
}

func ContainedDown()
{
  return(1);
}

func ContainedLeft()
{
  return(1);
}

func ContainedRight()
{
  return(1);
}

//Waffensysteme
func ContainedThrow() 
{
  return(1);
}

//Menüsteuerung
func ContainedDig()
{
  if(EMPDamaged) 
  {
    Message("EMP-Schaden",this());
    Sound("Error");
    return(1);
  }
  CreateMenu(PRE_,Par(nil),nil,nil,nil,nil);
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s","Waffenmodi",M16G,Par(nil));  //Waffenmodi
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s","Fahrgestell",M36G,Par(nil)); //Fahrgestell
  AddMenuItem("%s","Triebwerk",M26G,Par(nil));   //Triebwerk
  AddMenuItem("%s","Schadenskontrolle",M56G,Par(nil)); //Ausstieg
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s",nil,M66G,Par(nil));  //Freiraum
  AddMenuItem("%s","Ausstieg",M46G,Par(nil));    //Ausstieg
  SelectMenuItem(7,Par(nil));
  return(1);
}

func Waffenmodi()
{
  Message("Mit ausgefahrenen Kuven nicht verfügbar.",this());
  return(1);
}

func Triebwerk()
{
  Sound("Airlock1");
  ChangeDef(PRE_);
  this()->SetAction("Gelandet");
  this()->Triebwerk();
  this()->Speed();
  return(1);
}

func Fahrgestell()
{
  Sound("Airlock1");
  ChangeDef(PRE_);
  this()->SetAction("Gelandet");
  return(1);
}

func Ausstieg()
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Exit(pilot);
  pilot=nil;
  Sound("Airlock1");
  SetEntrance(1);
  return(1);
}

func Tucan(tucan)
{
  pilot=FindObject(nil, nil,nil,-1,-1, OCF_CrewMember,nil,nil,this());
  Enter(tucan,pilot);
  pilot=nil;
  return(1);
}

func Schadenskontrolle()
{
  if (!GetDamage())
  {
  Message("Kein Schaden.",this());
  return(1);
  }
  Message("Schaden: %d%.",this(),GetDamage());
  return(1);
}
