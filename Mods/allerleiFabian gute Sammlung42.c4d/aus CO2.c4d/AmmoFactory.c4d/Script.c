/*-- Kleiner Hochofen --*/

#strict 3

#include BAS2

public func IsWaste(){ return(2);} // bei jeder produktion wird dieser verschmutzungsswert an die umwelt gegeben, der sich nur langsam wieder abbaut.

/* Aufnahme */


// besser doch nicht für munition verwenden, da zu umständlich und man keine lore usw reinpacken kann und man schon eher viel munition braucht.. 


protected func Collection() { return(Sound("Clonk")); }
  
/* Brennvorgang */

protected func Burning()
{
  // Effekte
  Smoke(nil, 10, 6);
  // Noch nicht fertig
  if ((GetActTime() ?? 0) < 150) return(1);
  // Aktion beenden
  SetAction("Idle");
  // Effekt
  CastParticles("PxSpark",6,10,nil,10,15,30,RGB(255,223,127),RGB(255,223,127));
  // Erz in Metall umwandeln
  var pOre;
  if (pOre = FindContents(METL))
  {
    RemoveObject(pOre);
    Exit(CreateContents(STAP), -19,10,nil, -1);
  }
  Sound("Pshshsh");
  // Fertig
  // Prüfen, ob der Ofen weiterlaufen kann
  return(ContentsCheck());
}

protected func Burning2()
{
  // Effekte
  Smoke(nil, 10, 6);
  // Noch nicht fertig
  if ((GetActTime() ?? 0) < 150) return(1);
  // Aktion beenden
  SetAction("Idle");
  // Effekt
  CastParticles("PxSpark",6,10,nil,10,15,30,RGB(255,223,127),RGB(255,223,127));
  // Erz in Metall umwandeln
  var pROC;
  if (pROC = FindContents(CRYS))
  {
    RemoveObject(pROC);
    Exit(CreateContents(ENAP), -19,10,nil, -1);
  }
  Sound("Pshshsh");
  // Fertig
  // Prüfen, ob der Ofen weiterlaufen kann
  return(ContentsCheck());
}

protected func Burning3()
{
  // Effekte
  Smoke(nil, 10, 6);
  // Noch nicht fertig
  if ((GetActTime() ?? 0) < 150) return(1);
  // Aktion beenden
  SetAction("Idle");
  // Effekt
  CastParticles("PxSpark",6,10,nil,10,15,30,RGB(255,223,127),RGB(255,223,127));
  // Erz in Metall umwandeln
  var pROC;
  if (pROC = FindContents(SFLN))
  {
    RemoveObject(pROC);
    Exit(CreateContents(GRAP), -19,10,nil, -1);
    Exit(CreateContents(GRAP), -19,10,nil, -1);
}
  Sound("Pshshsh");
  // Fertig
  // Prüfen, ob der Ofen weiterlaufen kann
  return(ContentsCheck());
}

protected func Burning4()
{
  // Effekte
  Smoke(nil, 10, 6);
  // Noch nicht fertig
  if ((GetActTime() ?? 0) < 150) return(1);
  // Aktion beenden
  SetAction("Idle");
  // Effekt
  CastParticles("PxSpark",6,10,nil,10,15,30,RGB(255,223,127),RGB(255,223,127));
  // Erz in Metall umwandeln
  var pROC;
  if (pROC = FindContents(OBRL))
  {
    RemoveObject(pROC);
    Exit(CreateContents(GSAP), -19,10,nil, -1);
    Exit(CreateContents(GSAP), -19,10,nil, -1);
    Exit(CreateContents(GSAP), -19,10,nil, -1);
  }
  Sound("Pshshsh");
  // Fertig
  // Prüfen, ob der Ofen weiterlaufen kann
  return(ContentsCheck());
}

protected func Burning5()
{
  // Effekte
  Smoke(nil, 10, 6);
  // Noch nicht fertig
  if ((GetActTime() ?? 0) < 150) return(1);
  // Aktion beenden
  SetAction("Idle");
  // Effekt
  CastParticles("PxSpark",6,10,nil,10,15,30,RGB(255,223,127),RGB(255,223,127));
  // Erz in Metall umwandeln
  var pROC;
  if (pROC = FindContents(GUNP))
  {
    RemoveObject(pROC);
    Exit(CreateContents(MIAP), -19,10,nil, -1);
  }
  Sound("Pshshsh");
  // Fertig
  // Prüfen, ob der Ofen weiterlaufen kann
  return(ContentsCheck());
}

/*
protected func Burning6()
{
  // Effekte
  Smoke(0, 10, 6);
  // Noch nicht fertig
  if (GetActTime() < 150) return(1);
  // Aktion beenden
  SetAction("Idle");
  // Effekt
  CastParticles("PxSpark",6,10,0,10,15,30,RGB(255,223,127),RGB(255,223,127));
  // Erz in Metall umwandeln
  var pROC;
  if (pROC = FindContents(ROCK))
  {
    RemoveObject(pROC);
    Exit(CreateContents(GOLD), -19,10,0, -1);
    Exit(CreateContents(FLAG), -19,10,0, -1);
  }
  Sound("Pshshsh");
  // Fertig
  // Prüfen, ob der Ofen weiterlaufen kann
  return(ContentsCheck());
}
*/


/* Brennmaterial suchen */

protected func Collection2()
{
  // Neuer Inhalt: Prüfen (aber nicht sofort wieder auswerfen; sieht blöd aus)
  if (ActIdle()) SetAction("Wait");
  return(1);
}

protected func ContentsCheck()
{
  // Unpassende Gegenstände auswerfen
  var i, pContents, idContents;
  while (pContents = Contents(S42MigrationStep(i, 1, true)))
  {
    idContents = GetID(pContents);
    if (And(And(And(And(!Equal(idContents, METL), !Equal(idContents, CRYS)), !Equal(idContents, SFLN)), !Equal(idContents, OBRL)), !Equal(idContents, GUNP))
// && idContents != ROCK
 )
      Exit(pContents, -20,13,nil, -1);
  }
  // Ofen läuft schon?
  if (!ActIdle()) return(1);


 // noch neue munition zufuegen
  // Nur starten, wenn Erz drin ist
  if (And(FindContents(METL), GetPlrKnowledge (GetOwner(),STAP))) SetAction("Burning");
else if (And(FindContents(CRYS), GetPlrKnowledge (GetOwner(),ENAP))) SetAction("Burning2");
else if (And(FindContents(SFLN), GetPlrKnowledge (GetOwner(),GRAP))) SetAction("Burning3");
else if (And(FindContents(OBRL), GetPlrKnowledge (GetOwner(),GSAP))) SetAction("Burning4");
else if (And(FindContents(GUNP), GetPlrKnowledge (GetOwner(),MIAP))) SetAction("Burning5");
//else if (FindContents(ROCK)) SetAction("Burning6"); 
if (!ActIdle()) // wenn nun produziert wird, erstelle evlt muell
{
    if (!Random(10)) CreateContents(MTWS,this(),1);
    var verbrannt;
    verbrannt= CreateObject(VBWS);
    verbrannt->Verbrannt(GetID(),IsWaste());
}    


  // Ofen anschmeißen
//  SetAction("Burning");
 // return(1);
}

/*
public func HasFuel()
{
  if (FindContents(COAL)) return(1);
  if (ContentsCount(WOOD) >= 2) return(1);
  if (FindContents(OBRL)) return(1);
  return();
}
*/


/* Status */

protected func CalcValue() // Geringer Verkaufswert
{
  if (Contained()) return(25);
  return(50);
}
