/* Predator-Raumjäger */

#strict

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
  return(0);
}

//Schaden & Aufprall
func ContactTop()
{
  if (GetXDir()>15)  return(Explosion());
  if (GetYDir()>15)  return(Explosion());
  return(1);
}

func ContactBottom()
{
  if (GetXDir()>30)  return(Explosion());
  if (GetYDir()>30)  return(Explosion());
  return(1);
}

func ContactLeft()
{
  if (GetYDir()>40)  return(Explosion());
  if (GetXDir()>75)  return(Explosion());
  if (GetXDir()<-75) return(Explosion());
  if (GetXDir()>10)  return(Funken());
  if (GetXDir()<-10) return(Funken());
  return(1);
}

func ContactRight()
{
  if (GetYDir()>40)  return(Explosion());
  if (GetXDir()>75)  return(Explosion());
  if (GetXDir()<-75) return(Explosion());
  if (GetXDir()>10)  return(Funken());
  if (GetXDir()<-10) return(Funken());
  return(1);
}

func Funken()
{
  CreateParticle("Funken",RandomX(-3,3),13,RandomX(-10,10),RandomX(-10,-20),RandomX(40,60));
  return(1);
}

func Explosion()
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Exit(pilot);
 if(HashGet(hashExplosivErdschaden,Format("Erdschaden %v",GetID()))) Explode(40);
 else
  FakeExplode(40,0,0,40); //eine explosion, die kein schaden auf der erde macht
  return(1);
}

public func OnDmg(int iDmg, int iType)
{ 
  //Mögliche Resistenzen berücksichtigen
  if(iType & DMG_Projectile)
    return(80);
  if(iType & DMG_Fire)
    return(90);
  if(iType & DMG_Explosion)
    return(-10);
  if(iType & DMG_Energy)
    return(-40);
  if(iType & DMG_Bio)
    return(50);
}

func Damage()
{
  if(GetDamage()<100) return(0);
  Explosion();
  return(1);
}

public func EMPShock()
{
  //Nicht zweimal.
  if( GetEffect("Damaged", this())) return(0);
  EMPDamaged = 1;
  AddEffect("Damaged", this(), 190, 20, this(), GetID());
  SetEntrance(0);
  return(1);
}

public func FxDamagedStart(object pTarget, int iEffectNumber, int iTemp)
{
}

public func FxDamagedTimer(object pTarget, int iEffectNumber)
{
  if(! Contained())
    CreateParticle("PxSpark", RandomX(-2, 2), RandomX(2, 2), RandomX(-2, 2), 4, 30, RGB(187, 214, 224));
}

public func FxDamagedStop(object pTarget, int iEffectNumber)
{
  EMPDamaged = false;
  SetEntrance(1);
}

func Wasser()
{
  SetRDir(0);
  Bubble(RandomX(-10,10),RandomX(-10,10));
  return(1);
}

func Gelandet()
{
//Schaden sichtbar machen
  if(GetDamage()>=50)
  CreateParticle("Smoke2",Cos( GetR() + 270 , -10+Random(20)),Sin( GetR() + 270, -10+Random(20)),GetXDir()/2,GetYDir()/2,GetDamage(),RGBa(220,200,180,0),0,0); 
  if(GetDamage()>=80)
  {
  CreateParticle("Smoke2",Cos( GetR() + 270 , Random(10)),Sin( GetR() + 270, Random(10)),GetXDir()/2,GetYDir()/2,RandomX(30,50),RGBa(220,200,180,0),0,0);
  }
//Besitzer zuweisen
  if(pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()))
  {
  SetOwner(GetOwner(pilot));
  SetEntrance(0); 
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
  CreateMenu(PRE_,Par(0),0,0,0,0);
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s","Waffenmodi",M16G,Par(0));  //Waffenmodi
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s","Fahrgestell",M36G,Par(0)); //Fahrgestell
  AddMenuItem("%s","Triebwerk",M26G,Par(0));   //Triebwerk
  AddMenuItem("%s","Schadenskontrolle",M56G,Par(0)); //Ausstieg
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s",0,M66G,Par(0));  //Freiraum
  AddMenuItem("%s","Ausstieg",M46G,Par(0));    //Ausstieg
  SelectMenuItem(7,Par(0)); 
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
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Exit(pilot);
  pilot=0;
  Sound("Airlock1");
  SetEntrance(1);
  return(1);
}

func Tucan(tucan)
{
  pilot=FindObject(0, 0,0,-1,-1, OCF_CrewMember(),0,0,this()); 
  Enter(tucan,pilot);
  pilot=0;
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
