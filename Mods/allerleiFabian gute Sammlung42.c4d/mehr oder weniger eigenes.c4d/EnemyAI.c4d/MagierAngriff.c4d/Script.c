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
  if (ObjectCount2(Find_ID(SCLK), Find_OCF(OCF_Alive),Find_Func("IsAI"))>=ObjectCount(MA_N)+1) return(1);
  var pMonster = CreateObject(SCLK, Random(LandscapeWidth()), 50, -1);
  SetR(-180, pMonster);

// globale Variable, sorgt dafür, dass Skriptspieler zu allen anderen Spielern feindlich sind
if(EA69_HostileAll!=1) EA69_HostileAll=1;

     pMonster -> SetAI();
     MakeCrewMember(pMonster, GetPlayerByIndex(0, C4PT_Script));
     SetOwner(GetPlayerByIndex(0, C4PT_Script),pMonster);
// bekommt durch AI skript Waffen, wenn auch vom magier nicht unbedingt gebraucht, aber egal
 pMonster -> AI_Equip();

/*
var zufall=Random(5);
if(zufall==0)  {CreateContents(FLNT,pMonster); } 
if(zufall==1)  {CreateContents(EFLN,pMonster); }  
if(zufall==2)  {CreateContents(STFN,pMonster); } 
if(zufall==3)  {CreateContents(SFLN,pMonster); } 
if(zufall==4)  {CreateContents(FBMP,pMonster); } 
*/

  return(1);
}
