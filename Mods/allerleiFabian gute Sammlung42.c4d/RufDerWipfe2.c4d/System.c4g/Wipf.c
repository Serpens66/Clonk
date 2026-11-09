/*-- Wipfe --*/

#strict
#appendto WIPF

protected func Activity()
{
  // Geräusche machen
  if (!Random(15)) Sound("Snuff*");

  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return();
  
  // Köder fressen
  if(Bait)
  {
    if(ObjectDistance(Bait, this()) <= 35)
      Bait->~RejectEat();
    if(ObjectDistance(Bait, this()) <= 15)
      if(GetAction() eq "Walk")
      {
        SetAction("Eat");
        SetXDir();
      }
  }

  var pBait;
  // Soll nicht zu einfach sein...
  if (!Random(GetEnergy()/3*2))
    // Etwas zu essen in der Nähe?
    for(pBait in FindObjects(Find_Distance(500), Find_NoContainer(), Find_Func("IsAnimalFoodstuff"),Find_Not(Find_OCF(OCF_InLiquid | OCF_InSolid))))
      {
        // ...hinrennen
        SetCommand(this(), "Follow", pBait);
        // Und Köder merken
        Bait = pBait;
      }

  // Nur im Gehen oder Schwimmen
  if (GetAction() ne "Walk" && GetAction() ne "Swim") return();

  // Auftauchen, wenn der Atem ausgeht
  if (InLiquid() && GetBreath() <= 25 && GetComDir() != COMD_Up())
   SetComDir(COMD_Up());

  // Brennenden Objekten ausweichen
  var pBurning;
  if(pBurning = FindObject(0, -!GetDir()*80,-30,80,35, OCF_OnFire()))
  {
   if(!Random(5) || !SpaceToFlee(pBurning))
   {
    if(GetDir()) SetComDir(COMD_Right());
    else SetComDir(COMD_Left());
    Jump();
   }
   else
   {
    if(GetComDir()==COMD_Left()) SetComDir(COMD_Right());
    else SetComDir(COMD_Left());
   }
  }

  // Nichts machen
  if (Random(2)) return();

  // Freigraben, falls er feststeckt
  if(Stuck()) return(DigFree());

  // Fortpflanzung
  if (!Random(ReproductionRate())) 
    Reproduction();

  // Springen
  if (GetAction() eq "Walk")
   if (!Random(3)) return(Jump());

  // Hinsetzen
  if (GetAction() eq "Walk")
    if (!Random(3)) return(SetAction("Sit"));

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}

public func Eating()
{
  if(!Bait)return 0;
  Sound("Munch*");
  Schedule("Sound(\"Snuff*\")",5,1+Random(2),this());
  Bait->~Eat(this());
  return(1);
}

public func Feed(iVal)
{
  DoEnergy(iVal/2);
}
