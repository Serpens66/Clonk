/*--Berserker --*/

#strict 3

/* Initialisierung */

protected func Initialize()
{
  SetAction("Walk");
  SetDir(Random(2));
  // Gruppen-KI
  AddEffect("GroupKI", this(), 1, nil, this());
  // Schadenseffekt
  AddEffect("DmgCheck",this(),1,nil);
}

/* Aktivität */

protected func Activity()
{
  // Tot?
  if(!GetAlive()) {return;}
  // Gerade verhindert?
  if(!Ready()) return;

  // Rumangreifen
  if(ReadyToAttack()) {
    // Nur wenn möglich
    for(var prey in FindTargets(this, 200)) {
        Attack(prey);
		return;
  }
}

    // Auftauchen, wenn der Atem ausgeht
  if(And(And(InLiquid(), (GetBreath() ?? 0) <= 25), !Equal(GetComDir(), COMD_Up)))
    SetComDir(COMD_Up);

// wieder deaktivert, weil sonst alles angegriffen wird
/*
	 if(!prey) {   for(var prey2 in FindObjects(Find_Distance(200),Find_Exclude(this()),Find_NoContainer(),Find_Or(
                Find_Func("IsBulletTarget",GetID(this()),0,this()),Find_OCF(OCF_Alive)),Sort_Distance() ))
 {
        Attack(prey2);}}

*/

  // Kommando?
  if(GetCommand()) return;

  // Brüllen!
  if(!Random(500)) return(Roar());

  // Rumlaufen
  if(!!SEqual(GetAction(), "Walk"))
    {
    if(Random(5)) return;
    if(Random(2)) SetComDir(COMD_Left);
    else SetComDir(COMD_Right);
    }
}



public func Ready() // Wenn das Alien gerade nichts tun kann
{
  if(!!SEqual(GetAction(), "Jump")) return;
  if(!!SEqual(GetAction(), "KneelDown")) return;
  if(!!SEqual(GetAction(), "Death")) return;

  return(1);
}

/* Aktionen */

// Unglaublich schrecklicher Schrei!
public func Roar()
{
  if(!!SEqual(GetAction(), "Roar")) return;
  SetAction("Roar");
  Sound("BerserkerRoar");
}

// Angreifen oder Hinlaufen
public func Attack(object pPrey, bool noKI)
{
  // Gruppe benachrichtigen
  noKI ??= false;
  if(!noKI)
    GroupAttack(pPrey);
  // Geth nicht?!
  if(!ReadyToAttack()) return;
  // Nah genug zum krassen Angriff?
  if((ObjectDistance(pPrey) ?? 0) <= 64)
    // RAAR!
    return(ClawAttack(pPrey));
  // Hinrennen
  SetCommand(this(), "MoveTo", pPrey);
}

// Klauenangriff
private func ClawAttack(object pPrey)
{
  // Puh, jetzt wirds kompliziert. Richtige Attackanimation auswählen :D
  
  // Ist oben Platz?
  if(!GBackSolid(nil,-60))
    // Wir haben genug Platz für alle Angriffe. Random!
    SetAction(Format("Attack%d",(Random(3) ?? 0)+1)); // *hax*
    
  // Genug Platz für Angriff von der Seite? 
  else if(!GBackSolid(nil,-45))
    // Attack! :D
    SetAction(Format("Attack%d",(Random(2) ?? 0)+2));
    
  // Okay, bleibt nur noch Beißen. >:(
  else
    SetAction("Attack3");

  // Geräusch
  Sound("BerserkerAttack");
}

// Beißen
private func EndAttack()
{
  // Feind suchen
  var pPrey;
  for(pPrey in FindTargets(this,64)) { break; }

/*
  if (!pPrey) {for(var pPrey2 in FindObjects(Find_Distance(64),Find_Exclude(this()),Find_NoContainer(),Find_Or(
                Find_Func("IsBulletTarget",GetID(this()),0,this()),Find_OCF(OCF_Alive)),Sort_Distance() )) {break;}}
*/

  // Kein Feind
  if(!pPrey
// && !pPrey2
) return;
  // Hauen!!1
if (pPrey)  DoDmg(25+(Random(40) ?? 0)*(GetCon() ?? 0)/100, DMG_Melee, pPrey);
//if (pPrey2) DoDmg(35+Random(50)*GetCon()/100, DMG_Melee, pPrey2);
  Sound("BerserkerStab");
  // Hacksperre, damit man nicht dauerhackt
  AddEffect("NoAttack", this(), 99, 30, this());
}

public func Birth()
{
  // Miniwinzig!
  SetCon(50);
}




protected func Death()
{
  // Sterben
  SetAction("Death");
  Sparks(20, RGBa(25, 255, 25, 20));
  // Tönen!
  Sound("BerserkerDeath");

  GameCallEx("Zaehler10");
}

func CheckLiquid()
{
  return; // kein Tod in Wasser mehr.
  if(!GBackLiquid(nil,-3))
    return;
  if(Equal(GetMaterial(nil,-3), Material("Water")))
  {
    ExtractMaterialAmount(nil,nil, Material("Water"), GetMass());
    CastPXS("Acid",GetMass(),GetMass());
    Death();
    FadeOut();
  }
}

/* Gruppen-KI */

protected func GroupAttack(object pPrey)
{
  // Andere Aliens benachrichtigen
  for(var alien in FindObjects(Find_Distance(100), Find_Func("IsAlien"), Find_Exclude(this())))
    if(!CheckEnemy(alien))
      alien->~Attack(pPrey, true);
}

/* Schaden */

public func OnDmg(int iDmg, int iType) {
  //Melee
  iDmg ??= 0;
  iType ??= 0;
  if(Equal(iType, DMG_Melee))
    return(90);
  //Projectile
  if(Equal(iType, DMG_Projectile))
    return(nil);
  //Flame
  if(Equal(iType, DMG_Fire)) {
    AddFireEffect(this(),(5*(iDmg ?? 0)*(105))/100);
    return(-50);
  }
  //Explosion
  if(Equal(iType, DMG_Explosion))
    return(-10);
  //Energy
  if(Equal(iType, DMG_Energy))
    return(50);
  //Bio
  if(Equal(iType, DMG_Bio)) {
    return(90);
  }
}

/* Status */

public func ReadyToAttack()
{
  if(GetEffect("NoAttack", this())) return;

  return(!!SEqual(GetAction(), "Walk"));
}

public func IsAlien() { return(!SEqual(GetAction(), "Death")); }
public func IsBulletTarget() { return(true); }

/* Effekte */

public func FxGroupKIDamage(object target, int no, int iDmg, int iCause)
{
  // Angeschossen?
  no ??= 0;
  iDmg ??= 0;
  iCause ??= 0;
  if(Or(Or(Equal(iCause, 32), Equal(iCause, 33)), Equal(iCause, 35)))
    // Feinde in weiter Ferne suchen
    for(var prey in FindTargets(this,400))
          {
          Attack(prey);
          return(iDmg);
          }

/*
 if(!prey) {   for(var prey2 in FindObjects(Find_Distance(400),Find_Exclude(this()),Find_NoContainer(),Find_Or(
                Find_Func("IsBulletTarget",GetID(this()),0,this()),Find_OCF(OCF_Alive)),Sort_Distance() )) {
        Attack(prey2);return(iDmg);}}
*/

  return(iDmg);
}


public func Incineration() {
  // -- brennen nicht wirklich ;)
  Extinguish();
  Schedule("DoDmg(1,DMG_Fire,0,1)",1,20,this());
  AddFireEffect(this(),30,FIRE_Red,1);
}

public func IgnoreFriendlyFire() { return(1); }
public func IsThreat() { return(true); }
