/*-- Overlay-Zeiger --*/

#strict 2

public func Initialize()
{
  SetObjectLayer(this(),this());
  return(SetAction("Float"));
}

public func SetGraphicShape(int iLength, int iHeight, int iXOff, int iYOff)
{
  SetCon(Max(Max(iLength,iHeight),1),this());
  SetShape(-iLength/2+iXOff,-iHeight/2+iYOff,iLength,iHeight,this());
  return(this());
}