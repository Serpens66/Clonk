/*-- Außenturm links --*/

#strict

#include BG5C // Burgteil - Treppenhaus - Turm

/* Keine Tore, Transfer des Treppenhauses */

private func InitializeTower() { return(1); }

public func UpdateTransferZone()
  {
  // Mit darüberliegendem Niedergang
  if (FindStaircaseAbove())
    return(SetTransferZone(-8,-47,16,61));
  // Ohne darüberliegenden Niedergang (kein Auf- oder Abstieg möglich)
  return(SetTransferZone());
  }


/* Forschung */

public func GetResearchBase() { return(BT5C); }
  

func Zumachen()
{
//fürs abschließen
  if(zuu==1) SetEntrance(0);
  if(zuu==0) SetEntrance(1);
}