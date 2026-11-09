/*-- Monsterattacken --*/

#strict 3

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
  if ((ObjectCount2(Find_ID(KNIG), Find_OCF(OCF_Alive)) ?? 0)>=(ObjectCount(RI_N) ?? 0)+1) return(1);
  var pMonster = CreateObject(KNIG, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pMonster);

// globale Variable, sorgt dafür, dass Skriptspieler zu allen anderen Spielern feindlich sind
if(!Equal(EA69_HostileAll, 1)) EA69_HostileAll=1;

     pMonster -> SetAI(); // standard reichweite von 400
     MakeCrewMember(pMonster, GetPlayerByIndex(nil, C4PT_Script));
     SetOwner(GetPlayerByIndex(nil, C4PT_Script),pMonster);
// bekommt durch AI skript Waffen
 pMonster -> AI_Equip();

// standardschwierigkeit ist easy. Eventuell noch Angriffregeln mit anderer Schwierigkeit machen? 

  return(1);
}
