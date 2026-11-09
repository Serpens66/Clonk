/*-- Jagd --*/

#strict
#include GOAL

public func IsFulfilled()
{
  // jede Gattung von Viechzeug überprüfen
  if (FindObject(BIRD)) return(0);
  if (FindObject(FMNS)) return(0);
  if (FindObject(FISH)) return(0);
  if (FindObject(MONS)) return(0);
  if (FindObject(SHRK)) return(0);
  if (FindObject(SNKE)) return(0);
  if (FindObject(WIPF)) return(0);
  if (FindObject(BEAR)) return(0);
  if (FindObject(QFSH)) return(0);
  if (FindObject(_CHP)) return(0);
  if (FindObject(BBRD)) return(0);
  if (FindObject(DRGN)) return(0);
  if (FindObject(HICP)) return(0);
  if (FindObject(PIRA)) return(0);
  if (FindObject(SEAL)) return(0);
  if (FindObject(SPFL)) return(0);
  if (FindObject(HORS)) return(0);
  if (FindObject(_IOZ)) return(0);
  if (FindObject(ICCR)) return(0);
  if (FindObject(FSKE)) return(0);
  if (FindObject(SARN)) return(0);
  if (FindObject(PHNX)) return(0);
  if (FindObject(GRFN)) return(0);
  if (FindObject(BAT1)) return(0);
  return(1);
}

protected func Activate(player)
{
  // schon fertig
  if (IsFulfilled())
    return(MessageWindow("Mission completed, you are victorious!", player));
  // Nachricht ausgeben, was noch zu tun ist
  return(MessageWindow(Get2DoList(), player));
}

private func Get2DoList()
{
  // Liste aufstellen
  var list = "Creatures left:";
  var i;
  // Monster & Eier
  if (i = ObjectCount(BIRD)) 
               list = Format("Birds Left: %d", i);
  if (i = ObjectCount(FMNS)) 
               list = Format("Fire Mosters Left: %d", i);
  if (i = ObjectCount(FISH)) 
               list = Format("Fishes Left: %d", i);
  if (i = ObjectCount(MONS)) 
               list = Format("Monsters Left: %d", i);
  if (i = ObjectCount(SHRK)) 
               list = Format("Sharks Left: %d", i);
  if (i = ObjectCount(SNKE)) 
               list = Format("Snakes Left: %d", i);
  if (i = ObjectCount(WIPF)) 
               list = Format("Wipfs Left: %d", i);
  if (i = ObjectCount(BEAR)) 
               list = Format("Bears Left: %d", i);
  if (i = ObjectCount(QFSH)) 
               list = Format("Black Fishes Left: %d", i);
  if (i = ObjectCount(_CHP)) 
               list = Format("Chappies Left: %d", i);
  if (i = ObjectCount(BBRD)) 
               list = Format("Death Crowns Left: %d", i);
  if (i = ObjectCount(DRGN)) 
               list = Format("Dragons Left: %d", i);
  if (i = ObjectCount(HICP)) 
               list = Format("Seahorses Left: %d", i);
  if (i = ObjectCount(PIRA)) 
               list = Format("Phiranias Left: %d", i);
  if (i = ObjectCount(SEAL)) 
               list = Format("Seals Left: %d", i);
  if (i = ObjectCount(SPFL)) 
               list = Format("Swamp Flies Left: %d", i);
  if (i = ObjectCount(HORS)) 
               list = Format("Horses Left: %d", i);
  if (i = ObjectCount(_IOZ)) 
               list = Format("Ice Ozarks Left: %d", i);
  if (i = ObjectCount(ICCR)) 
               list = Format("Ice Crowns Left: %d", i);
  if (i = ObjectCount(FSKE)) 
               list = Format("Fire Snakes Left: %d", i);
  if (i = ObjectCount(SARN)) 
               list = Format("Saurians Left: %d", i);
  if (i = ObjectCount(PHNX)) 
               list = Format("Phoenixes Left: %d", i);
  if (i = ObjectCount(GRFN)) 
               list = Format("Griffons Left: %d", i);
  if (i = ObjectCount(BAT1)) 
               list = Format("Bats Left: %d", i);
  return(list);
}