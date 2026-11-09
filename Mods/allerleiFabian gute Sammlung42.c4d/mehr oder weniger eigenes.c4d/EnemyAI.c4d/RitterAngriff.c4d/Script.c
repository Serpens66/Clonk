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
  if (ObjectCount2(Find_ID(KNIG), Find_OCF(OCF_Alive))>=ObjectCount(RI_N)+1) return(1);
  var pMonster = CreateObject(KNIG, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pMonster);

// globale Variable, sorgt dafür, dass Skriptspieler zu allen anderen Spielern feindlich sind
if(EA69_HostileAll!=1) EA69_HostileAll=1;

     pMonster -> SetAI(); // standard reichweite von 400
     MakeCrewMember(pMonster, GetPlayerByIndex(0, C4PT_Script));
     SetOwner(GetPlayerByIndex(0, C4PT_Script),pMonster);
// bekommt durch AI skript Waffen
 pMonster -> AI_Equip();

// standardschwierigkeit ist easy. Eventuell noch Angriffregeln mit anderer Schwierigkeit machen? 

  return(1);
}
