/*-- Wipfe --*/

#strict 3
#appendto WIPF

protected func Activity()
{
  // Geräusche machen
  if (!Random(15)) Sound("Snuff*");

  // Die KI-Steuerung wird bei Besessenheit nicht gebraucht
  if (GetEffect("PossessionSpell", this())) return;
  
  // Köder fressen
  if(Bait)
  {
    if((ObjectDistance(Bait, this()) ?? 0) <= 35)
      Bait->~RejectEat();
    if((ObjectDistance(Bait, this()) ?? 0) <= 15)
      if(!!SEqual(GetAction(), "Walk"))
      {
        SetAction("Eat");
        SetXDir();
      }
  }

  var pBait;
  // Soll nicht zu einfach sein...
  if (!Random((GetEnergy() ?? 0)/3*2))
    // Etwas zu essen in der Nähe?
    for(pBait in FindObjects(Find_Distance(500), Find_NoContainer(), Find_Func("IsAnimalFoodstuff"),Find_Not(Find_OCF((OCF_InLiquid ?? 0) | (OCF_InSolid ?? 0)))))
      {
        // ...hinrennen
        SetCommand(this(), "Follow", pBait);
        // Und Köder merken
        Bait = pBait;
      }

  // Nur im Gehen oder Schwimmen
  if (And(!SEqual(GetAction(), "Walk"), !SEqual(GetAction(), "Swim"))) return;

  // Auftauchen, wenn der Atem ausgeht
  if (And(And(InLiquid(), (GetBreath() ?? 0) <= 25), !Equal(GetComDir(), COMD_Up)))
   SetComDir(COMD_Up);

  // Brennenden Objekten ausweichen
  var pBurning;
  if(pBurning = FindObject(nil, -(!GetDir() ?? 0)*80,-30,80,35, OCF_OnFire))
  {
   if(Or(!Random(5), !SpaceToFlee(pBurning)))
   {
    if(GetDir()) SetComDir(COMD_Right);
    else SetComDir(COMD_Left);
    Jump();
   }
   else
   {
    if(Equal(GetComDir(), COMD_Left)) SetComDir(COMD_Right);
    else SetComDir(COMD_Left);
   }
  }

  // Nichts machen
  if (Random(2)) return;

  // Freigraben, falls er feststeckt
  if(Stuck()) return(DigFree());

  // Fortpflanzung
  if (!Random(ReproductionRate())) 
    Reproduction();

  // Springen
  if (!!SEqual(GetAction(), "Walk"))
   if (!Random(3)) return(Jump());

  // Hinsetzen
  if (!!SEqual(GetAction(), "Walk"))
    if (!Random(3)) return(SetAction("Sit"));

  // Umdrehen
  if (Random(2)) return(TurnRight());
  return(TurnLeft());
}

public func Eating()
{
  if(!Bait)return nil;
  Sound("Munch*");
  Schedule("Sound(\"Snuff*\")",5,1+(Random(2) ?? 0),this());
  Bait->~Eat(this());
  return(1);
}

public func Feed(iVal)
{
  DoEnergy((iVal ?? 0)/2);
}
