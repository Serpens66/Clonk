/*-- Overlay-Zeiger --*/

#strict 3

public func Initialize()
{
  SetObjectLayer(this(),this());
  return(SetAction("Float"));
}

public func SetGraphicShape(int iLength, int iHeight, int iXOff, int iYOff)
{
  iLength ??= 0;
  iHeight ??= 0;
  iXOff ??= 0;
  iYOff ??= 0;
  SetCon(Max(Max(iLength,iHeight),1),this());
  SetShape(-(iLength ?? 0)/2+(iXOff ?? 0),-(iHeight ?? 0)/2+(iYOff ?? 0),iLength,iHeight,this());
  return(this());
}