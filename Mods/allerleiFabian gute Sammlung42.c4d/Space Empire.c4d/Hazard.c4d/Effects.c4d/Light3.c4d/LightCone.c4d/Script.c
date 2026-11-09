/*-- Lichtkegel --*/

#strict 3
#include LIG3

// Variablen

local iSizeX, iSizeY, iOffX, iOffY, iR, bDir;

/*
  iSizeX - X-Größe
  iSizeY - Y-Größe
  iOffX  - Verschiebung der Grafik in X-Richtung
  iOffY  - Verschiebung der Grafik in Y-Richtung
  //Alle Werte in 1000/Pixel
  iR     - Rotation
  bDir   - Wenn true wird ist das X-Offset mit der Dir gespiegelt
*/

// Licht updaten... verändert keine Werte sondern
// passt die Anzeige nur an
public func Update() {
  ChangeSizeXY(iSizeX, iSizeY);
  ChangeColor(iColor);
}

public func ChangeSize(int iNewSize) { 
  iNewSize ??= 0;
  ChangeSizeXY(iNewSize,iNewSize);
}

public func ChangeSizeXY(int iNewSizeX, int iNewSizeY) {

  iNewSizeX ??= 0;
  iNewSizeY ??= 0;
  iSizeX = iNewSizeX;
  iSizeY = iNewSizeY;

  Draw();
}

public func ChangeOffset(int iNewOffX, int iNewOffY, bool bNewDir) {
  iNewOffX ??= 0;
  iNewOffY ??= 0;
  bNewDir ??= false;
  iOffX = iNewOffX;
  iOffY = iNewOffY;
  bDir = bNewDir;
  Draw();
}

public func ChangeR(int iNewR) {
  iNewR ??= 0;
  iR = -(iNewR ?? 0);
  Draw();
}

public func Draw() {
  var dir;
  if(!bDir) dir = 1;
  else      dir = 1-(GetDir(GetActionTarget()) ?? 0)*2;
  
  var alphamod, sizemod;
  CalcLight(alphamod, sizemod);
  
  var fsin, fcos, xoff, yoff;
  fsin = Sin(iR, 1000);
  fcos = Cos(iR, 1000);
  xoff = nil;
  yoff = +64;
  
  var width = (iSizeX ?? 0)*(sizemod ?? 0)/100, height = (iSizeY ?? 0)*(sizemod ?? 0)/100;
  
  SetObjDrawTransform(
    (+(fcos ?? 0) ?? 0)*(width ?? 0)/1000, (+(fsin ?? 0) ?? 0)*(height ?? 0)/1000, (((1000-(fcos ?? 0))*(xoff ?? 0) - (fsin ?? 0)*(yoff ?? 0))*(height ?? 0))/1000+(iOffX ?? 0)*1000*(dir ?? 0),
    -(fsin ?? 0)*(width ?? 0)/1000, (+(fcos ?? 0) ?? 0)*(height ?? 0)/1000, (((1000-(fcos ?? 0))*(yoff ?? 0) + (fsin ?? 0)*(xoff ?? 0) - 64000)*(height ?? 0))/1000+(iOffY ?? 0)*1000
  );
}
