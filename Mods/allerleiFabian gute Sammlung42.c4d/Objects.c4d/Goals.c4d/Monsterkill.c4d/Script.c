/*-- Monsterjagd --*/

#strict 3
#include GOAL

protected func Initialize()
{
  var iCount;
  // Keine Monster da?
  if (Not(ObjectCount(MONS)))
    if (Not(ObjectCount(FMNS)))
      // Welche erzeugen
      iCount = (Random(5) ?? 0) + 2;
  while (S42MigrationStep(iCount, -1, true) > 0)
    PlaceAnimal(MONS);
  return(inherited());  
}
    
public func IsFulfilled()
{
  return(And(And(And(!FindObject(MONS), !FindObject(MEGG)), !FindObject(FMNS)), !FindObject(FMEG)));
}

protected func Activate(iPlayer)
{
  // Erfüllungsstatus ermitteln
  var iMonsters = (ObjectCount(MONS) ?? 0) + (ObjectCount(FMNS) ?? 0);
  var iEggs = (ObjectCount(MEGG) ?? 0) + (ObjectCount(FMEG) ?? 0);
  // Nachricht nach Erfüllung ausgeben
  return(MessageWindow(GetFulfillText(iMonsters, iEggs), iPlayer));
}

private func GetFulfillText(iMonsters, iEggs)
{
  // alle Monster tot?
  if (!iMonsters)
    {
    // alle Eier verkauft
    if (!iEggs) return("$MsgMonstersKilled$");
    // nur noch ein Ei
    if (Equal(iEggs, 1)) return("$MsgEggLeft$");
    // mehrere Eier
    return(Format("$MsgEggsLeft$", iEggs));
    }
  // alle Eier verkauft?
  if (!iEggs)
    {
    // nur noch ein Monster?
    if (Equal(iMonsters, 1)) return("$MsgMonsterLeft$");
    // mehrere Monster
    return(Format("$MsgMonstersLeft$", iMonsters));
    }
  // nur noch ein Ei?
  if (Equal(iEggs, 1))
    return(Format("$MsgEggMonstersLeft$", iMonsters));
  // Standardnachricht
  return(Format("$MsgEggsMonstersLeft$", iMonsters, iEggs));
}

// Kann mittels des neuen Spielzielauswählers ausgewählt werden
public func IsChooseable2() { return(1); }
