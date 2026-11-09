/*-- Seilbahn --*/

#strict 2

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
  if(!iCoordinate) return(+26*(iDir*2-1));
  return(+16);
}

public func GetWheelOffset(int iDir, int iCoordinate)
{
  // iDir: 0 = Nach Links; 1 = Nach Rechts
  // iCoordinate: X- oder Y-Achse
  if(!iCoordinate) return(+8*(iDir*2-1));
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
  if(AbsX(GetX(pToConnect)) < 0)
    // Die Funktion muss in ihr aufgerufen werden
    return(pToConnect->~ConnectWith(this()));
  // Station speichern
  pLeadTo = pToConnect;
  // Draht erstellen & Graphik einrichten
  CreateLineGraphic(pToConnect);
  // Seilbahnkorb erstellen, abhängig
  // iCaseSide: 0 = Links; 1 = Rechts; Sonst: Mitte
  var iX, iY;
  if(iCaseSide == 0) {
    iX = GetCaseOffset(1,0);
    iY = GetCaseOffset(1,1);
  }
  else if(iCaseSide == 1) {
    iX = AbsX(GetX(pToConnect))+pToConnect->~GetCaseOffset(0,0);
    iY = AbsY(GetY(pToConnect))+pToConnect->~GetCaseOffset(0,1);
  }
  else {
    iX = (AbsX(GetX(pToConnect))+GetCaseOffset(1,0)+pToConnect->~GetCaseOffset(0,0))/2;
    iY = (AbsY(GetY(pToConnect))+GetCaseOffset(1,1)+pToConnect->~GetCaseOffset(0,1))/2;
  }
  if(!pCase)
    pCase = CreateObject(ELC2,iX,iY-GetDefOffset(ELC2,1),GetOwner(this()));
  else {
    SetPosition(GetX(this())+iX,GetY(this())+iY,pCase);
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
  if(!pToConnect) return(false);
  // Liegt die Station mit welcher diese verbunden wird weiter links davon?
  if(AbsX(GetX(pToConnect)) < 0)
    // Die Funktion muss in ihr aufgerufen werden
    return(pToConnect->~CreateLineGraphic(this()));
  var iMoveX, iMoveY, iX, iY, iAngle, i, iCount, iLength;
  iX        = AbsX(GetX(pToConnect));
  iY        = AbsY(GetY(pToConnect));
  iMoveX    = pToConnect->~GetWheelOffset(0,0)-GetWheelOffset(1,0);
  iMoveY    = pToConnect->~GetWheelOffset(0,1)-GetWheelOffset(1,1);
  iAngle    = Angle(   0,0,iX+iMoveX-2,iY+iMoveY,10);

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
  pWire->~SetGraphicShape(Abs(iX+iMoveX)+2,Abs(iY+iMoveY)+2);
  SetPosition(GetX(this())+(iX				   -Cos(iAngle,6,10))/2,
	      GetY(this())+(iY+iMoveY+2*GetWheelOffset(1,1)-Sin(iAngle,6,10))/2-1,pWire);
  // Graphik einrichten
  // Möglichst 30 Pixel pro Drahtstück
  i = Distance(0,0,iX+iMoveX,iY+iMoveY);
  iCount = i/30+1;
  iLength = i*200/iCount+25;
  iMoveX += iX;
  iMoveY += iY;
  for(i = 0; i < iCount; i++) {
    // i = 0 ist der Anfang; i = iCount-1 ist das Ende
    SetGraphics(0,pWire,GetID(this()),i+1,2,"Line");
    iX = 500*iMoveX*(2*i-(iCount-1))/iCount;
    iY = 500*iMoveY*(2*i-(iCount-1))/iCount;
    SetObjDrawTransform( Sin(iAngle,iLength,10),Cos(iAngle,1000,10),iX,
			-Cos(iAngle,iLength,10),Sin(iAngle,1000,10),iY,pWire,i+1);
  }
  return(pWire);
}

public func RemoveCase(int iSide, int iFire)
{
  var pRopeway;
  // iSide: -1 = Links; 1 = Rechts; Sonst: Beide
  if(iSide >= 0) {
    if(pWire) RemoveObject(pWire);
    if(pCase) {
      LocalN("pLeft",pCase) = 0;
      if(iFire)
        Incinerate(pCase);
      else
        RemoveObject(pCase);
    }
    pLeadTo = 0;
  }
  if(iSide <= 0)
    if(pRopeway = FindObject2(Find_ID(RW4V),Find_Local("pLeadTo",this())))
      pRopeway->~RemoveCase(1,iFire);
  return(true);
}

protected func Incineration()   { RemoveCase(0,1); return(_inherited(...)); }
protected func IncinerationEx() { RemoveCase(0,1); return(_inherited(...)); }
protected func Destruction()    { RemoveCase(0); return(_inherited(...)); }