/*-- Monsterattacken --*/

#strict

// Kann mittels des Spielzielauswählers ausgewählt werden
public func IsChooseable() { return(1); }

protected func Initialize()
{
  // Eigene Position festlegen
  SetPosition();
  return(1);
}

/* Eventuell Monster erstellen */
private func DoMonsters()
{
  if (ObjectCount2(Find_ID(HZCK), Find_OCF(OCF_Alive),Find_Func("IsAI"),Find_Func("IsHazardAI"))>=ObjectCount(HZ_N)+1) return(1);
  var pMonster = CreateObject(HZCK, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pMonster);

    // globale Variable, sorgt dafür, dass Skriptspieler zu allen anderen Spielern feindlich sind
    if(EA69_HostileAll!=1) EA69_HostileAll=1;

     pMonster -> SetAI();
     MakeCrewMember(pMonster, GetPlayerByIndex(0, C4PT_Script));
     pMonster->~IsNunHazardAI();  // als HazardAI markieren, damit Regel ihn erkennt.(und zb von assasinen usw unterschheiden kann)
     SetOwner(GetPlayerByIndex(0, C4PT_Script),pMonster);
     pMonster -> AI_Equip(6); // standard hazardwaffen vergeben

  return(1);
}
