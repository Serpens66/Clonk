/*-- Seilbahn --*/

#strict 3

#include CXEC
#include BAS1

// Der Seilbahnkorb und das Gegenüber: Beides gilt für die rechte Seite der linken Seilbahn
local pCase, pLeadTo;
// Damit der Draht auch im kleineren Bildschirm immer zu sehen ist
local pWire;

// Sollten weitere ähnliche Seilbahnen erstellt werden (z.B. Burgteil)
public func GetCaseOffset(int iDir, int iCoordinate)
{
  // iDir: 0 = Nach Links; 1 = Nach Rechts
  // iCoordinate: X- oder Y-Achse
  iDir ??= 0;
  iCoordinate ??= 0;
  if(!iCoordinate) return((+26 ?? 0)*((iDir ?? 0)*2-1));
  return(+16);
}

public func GetWheelOffset(int iDir, int iCoordinate)
{
  // iDir: 0 = Nach Links; 1 = Nach Rechts
  // iCoordinate: X- oder Y-Achse
  iDir ??= 0;
  iCoordinate ??= 0;
  if(!iCoordinate) return((+8 ?? 0)*((iDir ?? 0)*2-1));
  return(-22);
}
// Der Radius des Rades sollte aber immer ~5-6 Pixel betragen

public func LineConnectType()
{
  // Sind beide Seiten bereits belegt?
  if(pLeadTo && FindObject2(Find_ID(RW4V),Find_Local("pLeadTo",this())))
    // Stromleitung
    return(PWRL);
  // Sonst Seilbahnleitung
  return(RL4V);
}

protected func Initialize()
{
  SetAction("Ready");
  return(1);
}

public func ConnectWith(object pToConnect, int iCaseSide)
{
  // Liegt die Station mit welcher diese verbunden wird weiter links davon?
  iCaseSide ??= 0;
  if((AbsX(GetX(pToConnect)) ?? 0) < 0)
    // Die Funktion muss in ihr aufgerufen werden
    return(pToConnect->~ConnectWith(this()));
  // Station speichern
  pLeadTo = pToConnect;
  // Draht erstellen & Graphik einrichten
  CreateLineGraphic(pToConnect);
  // Seilbahnkorb erstellen, abhängig
  // iCaseSide: 0 = Links; 1 = Rechts; Sonst: Mitte
  var iX, iY;
  if(S42MigrationEqual(iCaseSide, nil)) {
    iX = GetCaseOffset(1,nil);
    iY = GetCaseOffset(1,1);
  }
  else if(S42MigrationEqual(iCaseSide, 1)) {
    iX = (AbsX(GetX(pToConnect)) ?? 0)+(pToConnect->~GetCaseOffset(nil,nil) ?? 0);
    iY = (AbsY(GetY(pToConnect)) ?? 0)+(pToConnect->~GetCaseOffset(nil,1) ?? 0);
  }
  else {
    iX = ((AbsX(GetX(pToConnect)) ?? 0)+(GetCaseOffset(1,nil) ?? 0)+(pToConnect->~GetCaseOffset(nil,nil) ?? 0))/2;
    iY = ((AbsY(GetY(pToConnect)) ?? 0)+(GetCaseOffset(1,1) ?? 0)+(pToConnect->~GetCaseOffset(nil,1) ?? 0))/2;
  }
  if(!pCase)
    pCase = CreateObject(ELC2,iX,(iY ?? 0)-(GetDefOffset(ELC2,1) ?? 0),GetOwner(this()));
  else {
    SetPosition((GetX(this()) ?? 0)+(iX ?? 0),(GetY(this()) ?? 0)+(iY ?? 0),pCase);
    SetOwner(GetOwner(this()),pCase);
  }
  SetObjectOrder(this(),pCase);
  SetObjectOrder(pToConnect,pCase);
  LocalN("pLeft",pCase) = this();
  LocalN("pRight",pCase) = pToConnect;
  // Fertig
  return(true);
}

// Erstellt die Graphik des Drahtes
public func CreateLineGraphic(object pToConnect)
{
  if(!pToConnect) pToConnect = pLeadTo;
  if(!pToConnect) pToConnect = FindObject2(Find_ID(RW4V),Find_Local("pLeadTo",this()));
  if(!pToConnect) return(nil);
  // Liegt die Station mit welcher diese verbunden wird weiter links davon?
  if((AbsX(GetX(pToConnect)) ?? 0) < 0)
    // Die Funktion muss in ihr aufgerufen werden
    return(pToConnect->~CreateLineGraphic(this()));
  var iMoveX, iMoveY, iX, iY, iAngle, i, iCount, iLength;
  iX        = AbsX(GetX(pToConnect));
  iY        = AbsY(GetY(pToConnect));
  iMoveX    = (pToConnect->~GetWheelOffset(nil,nil) ?? 0)-(GetWheelOffset(1,nil) ?? 0);
  iMoveY    = (pToConnect->~GetWheelOffset(nil,1) ?? 0)-(GetWheelOffset(1,1) ?? 0);
  iAngle    = Angle(   nil,nil,(iX ?? 0)+(iMoveX ?? 0)-2,(iY ?? 0)+(iMoveY ?? 0),10);

/*
  // Draht erstellen + Graphik einrichten
  if(!pWire) pWire = CreateObject(OD4V);
  pWire->~SetGraphicShape(Abs(iX+iMoveX)+2,Abs(iY+iMoveY)+2);
  SetGraphics(0,pWire,GetID(this()),1,2,"Line");
  SetObjDrawTransform( Sin(iAngle,iDistance*200,10),Cos(iAngle,1000,10),0,
                      -Cos(iAngle,iDistance*200,10),Sin(iAngle,1000,10),0,pWire,1);
  // Position einrichten
  iMoveY += 2*GetWheelOffset(1,1);
  SetPosition(GetX(this())+(iX       -Cos(iAngle,6,10))/2,
	      GetY(this())+(iY+iMoveY-Sin(iAngle,6,10))/2-1,pWire);
*/
  // So wurde es früher mal geregelt.
  // Weil dies aber Fehler mit der Kombination vom FoW hatte, wurde es durch das Folgende ausgetauscht:

  if(pWire)
    // Zur Einfachkeit
    RemoveObject(pWire);
  // Größe & Position einrichten
  pWire = CreateObject(OD4V);
  pWire->~SetGraphicShape((Abs((iX ?? 0)+(iMoveX ?? 0)) ?? 0)+2,(Abs((iY ?? 0)+(iMoveY ?? 0)) ?? 0)+2);
  SetPosition((GetX(this()) ?? 0)+((iX ?? 0)				   -(Cos(iAngle,6,10) ?? 0))/2,
	      (GetY(this()) ?? 0)+((iY ?? 0)+(iMoveY ?? 0)+2*(GetWheelOffset(1,1) ?? 0)-(Sin(iAngle,6,10) ?? 0))/2-1,pWire);
  // Graphik einrichten
  // Möglichst 30 Pixel pro Drahtstück
  i = Distance(nil,nil,(iX ?? 0)+(iMoveX ?? 0),(iY ?? 0)+(iMoveY ?? 0));
  iCount = (i ?? 0)/30+1;
  iLength = (i ?? 0)*200/(iCount ?? 0)+25;
  S42MigrationAssign(iMoveX, "+=", (iX ?? 0));
  S42MigrationAssign(iMoveY, "+=", (iY ?? 0));
  for(i = nil; (i ?? 0) < (iCount ?? 0); S42MigrationStep(i, 1, true)) {
    // i = 0 ist der Anfang; i = iCount-1 ist das Ende
    SetGraphics(nil,pWire,GetID(this()),(i ?? 0)+1,2,"Line");
    iX = 500*(iMoveX ?? 0)*(2*(i ?? 0)-((iCount ?? 0)-1))/(iCount ?? 0);
    iY = 500*(iMoveY ?? 0)*(2*(i ?? 0)-((iCount ?? 0)-1))/(iCount ?? 0);
    SetObjDrawTransform( Sin(iAngle,iLength,10),Cos(iAngle,1000,10),iX,
			-(Cos(iAngle,iLength,10) ?? 0),Sin(iAngle,1000,10),iY,pWire,(i ?? 0)+1);
  }
  return(pWire);
}

public func RemoveCase(int iSide, int iFire)
{
  iSide ??= 0;
  iFire ??= 0;
  var pRopeway;
  // iSide: -1 = Links; 1 = Rechts; Sonst: Beide
  if((iSide ?? 0) >= 0) {
    if(pWire) RemoveObject(pWire);
    if(pCase) {
      LocalN("pLeft",pCase) = nil;
      if(iFire)
        Incinerate(pCase);
      else
        RemoveObject(pCase);
    }
    pLeadTo = nil;
  }
  if((iSide ?? 0) <= 0)
    if(pRopeway = FindObject2(Find_ID(RW4V),Find_Local("pLeadTo",this())))
      pRopeway->~RemoveCase(1,iFire);
  return(true);
}

protected func Incineration()   { RemoveCase(nil,1); return(_inherited(...)); }
protected func IncinerationEx() { RemoveCase(nil,1); return(_inherited(...)); }
protected func Destruction()    { RemoveCase(nil); return(_inherited(...)); }