/*-- Verschmutzung erzeugen --*/

#strict 3

protected func Initialize()
{
  // Eigene Position festlegen
SetPosition();
  return(1);
}

/* Eventuell Nebel erzeugen */
private func DoNebel()
{
  if((ObjectCount2(Find_ID(VM9Z)) ?? 0)>=5) return(1);
else CreateObject(VM9Z);return(1);
}
