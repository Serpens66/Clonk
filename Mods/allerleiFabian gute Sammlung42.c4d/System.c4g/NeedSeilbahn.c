/*-- Diese Funktionen werden für die Seilbahn benötigt --*/

#strict 2

global func GetObjOffset(object pObject, int iEntryNr) { return(GetObjectVal("Offset",0,pObject,iEntryNr)); }

global func AbsRotation(int iRotation, int iType, int iPrec)
{
  if(!iPrec) iPrec = 1;
  iRotation %= 360*iPrec;
  if(iType) {
    if(iRotation <= -180*iPrec) iRotation += 360*iPrec;
    else if(iRotation > 180*iPrec) iRotation -= 360*iPrec;
  }
  else
    if(iRotation < 0) iRotation += 360*iPrec;
  return(iRotation);
}