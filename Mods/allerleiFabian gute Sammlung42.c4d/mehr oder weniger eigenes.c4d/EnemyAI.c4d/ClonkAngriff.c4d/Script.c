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
  if ((ObjectCount2(Find_ID(CLNK), Find_OCF(OCF_Alive),Find_Func("IsAI"),Find_Func("IsClonkAI")) ?? 0)>=(ObjectCount(CLAN) ?? 0)+1) return(1);
  var pMonster = CreateObject(CLNK, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pMonster);

// globale Variable, sorgt dafür, dass Skriptspieler zu allen anderen Spielern feindlich sind
if(!Equal(EA69_HostileAll, 1)) EA69_HostileAll=1;

     pMonster -> SetAI();
     MakeCrewMember(pMonster, GetPlayerByIndex(nil, C4PT_Script));
     pMonster -> IsNunClonkAI();
     SetOwner(GetPlayerByIndex(nil, C4PT_Script),pMonster);
// bekommt durch AI skript Waffen
 pMonster -> AI_Equip();


// wenn "angepasste Physics" aktiv ist, dann haben alle CLNK zu durch initialize schlechtere Physics. Daher hier rückgängig machen:

if(FindObject2(Find_ID(PPP2)))
{
Physicswiederanpssen(pMonster);
}

  return(1);
}


func Physicswiederanpssen(pMonster)
{
SetPhysical("Energy", 50000, PHYS_StackTemporary, pMonster);
SetPhysical("Breath", 50000, PHYS_StackTemporary, pMonster);
SetPhysical("Walk", 70000, PHYS_StackTemporary, pMonster);
SetPhysical("Jump", 30000, PHYS_StackTemporary, pMonster);
SetPhysical("Scale", 30000, PHYS_StackTemporary, pMonster);
SetPhysical("Hangle", 30000, PHYS_StackTemporary, pMonster);
SetPhysical("Dig", 40000, PHYS_StackTemporary, pMonster);
SetPhysical("Swim", 60000, PHYS_StackTemporary, pMonster);
SetPhysical("Throw", 50000, PHYS_StackTemporary, pMonster);
SetPhysical("Push", 40000, PHYS_StackTemporary, pMonster);
SetPhysical("Fight", 50000, PHYS_StackTemporary, pMonster);
}
