/*-- Monsterattacken --*/
// Assassinenschützen mit Scharfschützengewehr und Pistole, kann sich verstecken

#strict 3

local WaffenRange;


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
  if ((ObjectCount2(Find_ID(HZCK), Find_OCF(OCF_Alive),Find_Func("IsAI"),Find_Func("IsAssasinAI")) ?? 0)>=(ObjectCount(AS_N) ?? 0)+1) return(1);
  var pMonster = CreateObject(HZCK, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pMonster);

WaffenRange=1000; 

// globale Variable, sorgt dafür, dass Skriptspieler zu allen anderen Spielern feindlich sind
if(!Equal(EA69_HostileAll, 1)) EA69_HostileAll=1;

     MakeCrewMember(pMonster, GetPlayerByIndex(nil, C4PT_Script));
     SetOwner(GetPlayerByIndex(nil, C4PT_Script),pMonster);
     SetGraphics("Assassin",pMonster);   // Assassinengrafik
     pMonster->~IsNunAssasinAI();  // als Assasinen markieren, damit Regel ihn erkennt.
     pMonster ->~ SetAI(nil,WaffenRange,1); // als AI einstellen mit WaffenRage als Reichweit und kann sich verstecken.
     pMonster ->~ AI_Equip(RandomX(1,2)); //spezielle Assassinausrüstung geben

  return(1);
}
