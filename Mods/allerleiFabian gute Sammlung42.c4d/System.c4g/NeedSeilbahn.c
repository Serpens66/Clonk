/*-- Diese Funktionen werden für die Seilbahn benötigt --*/

#strict 3

global func GetObjOffset(object pObject, int iEntryNr) { iEntryNr ??= 0;
    return(GetObjectVal("Offset",nil,pObject,iEntryNr)); }

global func AbsRotation(int iRotation, int iType, int iPrec)
{
  iRotation ??= 0;
  iType ??= 0;
  iPrec ??= 0;
  if(!iPrec) iPrec = 1;
  S42MigrationAssign(iRotation, "%=", 360*(iPrec ?? 0));
  if(iType) {
    if((iRotation ?? 0) <= -180*(iPrec ?? 0)) S42MigrationAssign(iRotation, "+=", 360*(iPrec ?? 0));
    else if((iRotation ?? 0) > 180*(iPrec ?? 0)) S42MigrationAssign(iRotation, "-=", 360*(iPrec ?? 0));
  }
  else
    if((iRotation ?? 0) < 0) S42MigrationAssign(iRotation, "+=", 360*(iPrec ?? 0));
  return(iRotation);
}