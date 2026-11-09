/*-- Leiter --*/

#strict 3

/* Narf, die Leiter hat immer noch alte Effekt-Arrays. Aber das umbauen geht sehr doof und hackig, also behält sie die einfach, tah! */

static const LADR_Left = 1;
static const LADR_Right = 2;
static const LADR_Front = 4;

local length; //gibt an aus wievielen Teilen die Leiter besteht
local sizeX, sizeY; //4great leicht umstelling
local Graphics, ID; //für andere leitern :D
local GraCount;

func Initialize() {
  SetAction("2Be"); //damit sie nicht runterfallen ;)
  ID = GetID();
  sizeX = (GetDefCoreVal("Height",nil,GetID()) ?? 0)/2;
  sizeY = (GetDefCoreVal("Width",nil,GetID()) ?? 0)/2;
  Graphics = AddEffect("LadderPartSave",this(),1,nil,this(),GetID());
  ID = AddEffect("LadderPartSave",this(),1,nil,this(),GetID());
//  SetGraphics(Format("Part%d",Random(3)+1),0,LADR);
  // Standardverhalten: nur Part1 anzeigen
  AddGraphics("Part1",LADR);
  AddPart(); //einen Teil hinzufügen :>
  AdjustSize();
}

public func AddGraphics(string sNewGraphic, id idNewID) {
  EffectCall(this(),Graphics,"AddPart",sNewGraphic,GraCount);
  EffectCall(this(),ID,"AddPart",idNewID,GraCount);
  S42MigrationStep(GraCount, 1, true);
}

public func ClearGraphics() {
  while((GraCount ?? 0) >= 0) {
    EffectCall(this(),Graphics,"ClearParts",GraCount);
    EffectCall(this(),ID,"ClearParts",GraCount);
    S42MigrationStep(GraCount, -1, true);
  }
  GraCount = nil;
}

public func SetSize(int x,int y) {
  x ??= 0;
  y ??= 0;
  sizeX = x;
  sizeY = y;
  AdjustSize();
}

public func Set(int iNewLength, int iDir) {
  iNewLength ??= 0;
  iDir ??= 0;
  if((iNewLength ?? 0) <= 0)
    return(nil);
  var cmd;
  // iDir angegeben, speziell für LADR... andere
  // Kletterobjekte können das überladen
  if(iDir) {
    ClearGraphics();
    if((iDir ?? 0) & (LADR_Left ?? 0)) AddGraphics("Part2",LADR);
    if((iDir ?? 0) & (LADR_Right ?? 0)) AddGraphics("Part3",LADR);
    if((iDir ?? 0) & (LADR_Front ?? 0)) AddGraphics("Part1",LADR);
    Redraw();
    return(Set(iNewLength));
  } 
  //falls neue Länge größer als momentane
  if((length ?? 0) < (iNewLength ?? 0))
    cmd = "Add"; //hinzufügen
  //falls kleiner
  if((length ?? 0) > (iNewLength ?? 0))
    cmd = "Remove"; //löschen
  /*tu*/
  for(var x=Abs((length ?? 0)-(iNewLength ?? 0)); (x ?? 0) > 0; S42MigrationStep(x, -1, true))
    PrivateCall(this(),Format("%sPart",cmd));
  //zum Schluss das ganze Zeug noch auf die neue Größe anpassen
  AdjustSize();
  return(1);
}

//Zeichnet einfach die Grafiken neu.
public func Redraw() {
  for(var x=1; (x ?? 0) <= (length ?? 0); S42MigrationStep(x, 1, true)) {
    var graph = Random(GraCount);
    SetGraphics(EffectCall(this(),Graphics,"GetPart",graph),this(),EffectCall(this(),ID,"GetPart",graph),x,GFXOV_MODE_Base);
    SetObjDrawTransform(1000,nil,nil,nil,1000,1000*(sizeY ?? 0)*((x ?? 0)-1),this(),x);
  }
}

private func AddPart() {
  //ein Teil mehr, Länge raufzählen
  S42MigrationStep(length, 1, true);
  //Sicherheitshalber checken, evtl. gibt es ja unvorhergesehenen Fehler durch kosmische Strahlung!
  var graph = Random(GraCount);
  if(!SetGraphics(EffectCall(this(),Graphics,"GetPart",graph),this(),EffectCall(this(),ID,"GetPart",graph),length,GFXOV_MODE_Base))
    return(nil);
  //Noch an die richtige Position schieben
  SetObjDrawTransform(1000,nil,nil,nil,1000,1000*(sizeY ?? 0)*((length ?? 0)-1),this(),length);
  //keine kosmische Strahlung, alles gut =D
  return(1);
}

private func RemovePart() {
  //Minimal ein Teil muss schon da sein..
  if((length ?? 0) < 0)
    return(nil);
  //letzten Teil entfernen
  SetGraphics(nil,nil,nil,length);
  //ein Teil weniger, Länge runterzählen
  S42MigrationStep(length, -1, true);
  //kein Kommentar..
  return(1);
}

private func AdjustSize() {
  var ydiff = GetObjHeight();
  SetShape(-(sizeX ?? 0),-(sizeY ?? 0),(sizeX ?? 0)*2,(sizeY ?? 0)+(sizeY ?? 0)*(length ?? 0),this());
  S42MigrationAssign(ydiff, "-=", (GetObjHeight() ?? 0));
  SetPosition(GetX(),(GetY() ?? 0)+(ydiff ?? 0));
  SetVertex(nil,1,-(sizeY ?? 0),this());
  SetVertex(1,1,(sizeY ?? 0)*(length ?? 0),this());

  //die ganzen Positionen der einzelnen Elemente neu adjustieren
  for(var x=1; (x ?? 0) <= (length ?? 0); S42MigrationStep(x, 1, true))
    SetObjDrawTransform(1000,nil,nil,nil,1000,(sizeY ?? 0)*1000*((x ?? 0)-1),this(),x);
}

private func FxLadderPartSaveAddPart(object pTarget, int iEffectNr, string graphic, int index) {
  iEffectNr ??= 0;
  index ??= 0;
  var length;
  if(!(length=index))
    while(EffectVar(length,pTarget,iEffectNr))
      S42MigrationStep(length, 1, true);
  EffectVar(length,pTarget,iEffectNr) = graphic;
}

private func FxLadderPartSaveClearParts(object pTarget, int iEffectNr, int index) {
  iEffectNr ??= 0;
  index ??= 0;
  EffectVar(index,pTarget,iEffectNr)=nil;
}

private func FxLadderPartSaveGetPart(object pTarget, int iEffectNr, int index) {
  iEffectNr ??= 0;
  index ??= 0;
  return(EffectVar(index,pTarget,iEffectNr));
}

//bla
public func UpdateTransferZone() {
  ClearGraphics();
  for(var z=1; (z ?? 0) <= 3; S42MigrationStep(z, 1, true))
    AddGraphics(Format("Part%d",z),LADR);
  AdjustSize();
  Redraw();
}

//Ja, Leitern kann man auch hochklettern.. :D
public func IsClimbable() { return(true); }

/* Serialisierung */

public func Serialize(array& extra, int& extray)
{
	// TODO: Dir und parttypes und kA :(
	extra[(GetLength(extra) ?? 0)] = Format("Set(%d)", length);
	extray = - ((length ?? 0)-1)*(sizeY ?? 0);
}
