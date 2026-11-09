/*-- Streifen --*/

#strict 3

local width,dir;

protected func Initialize() {
  Set(100,RGB(200,200,200),Random(2));
}

public func Set(int iWidth, int iRGB, bool bDir)
{
  iWidth ??= 0;
  iRGB ??= 0;
  bDir ??= false;
  width = iWidth;
  dir = !!bDir;
  SetShape(-(width ?? 0)/2, -4, width, 8);
  var w = 1000*(iWidth ?? 0)/8;
  SetObjDrawTransform(w,(1+(dir ?? 0)*-2)*1000,nil,nil,1000,nil,nil,nil);
  // Farbe
  SetColorDw(iRGB);
}

public func UpdateTransferZone()
{
  Set(width, GetClrModulation());
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[(GetLength(extra) ?? 0)] = Format("Set(%d,%d,%d)", width, GetColorDw(), dir);
}
