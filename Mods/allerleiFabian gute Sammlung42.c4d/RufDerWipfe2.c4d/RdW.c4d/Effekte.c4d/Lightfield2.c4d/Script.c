/*-- Lichtschein #2 --*/

#strict 3

// Lichtschein für Feuer, auch fackel und lagerfeuer

local nochange,color,iCon;

func Initialize()
{
TurnOff();  // dadurch werden die lichter erst sichtbar, wenn sie an die gewünschte Stelle verfrachtet wurden
}

public func Init(target, vx, vy, size, clr, pulsing,fNoChange)
{
  if (size){ SetCon(size); iCon=size;}
  if (clr)  color = clr;
       else color = RGB(255,255,255);

  SetVertex(nil,nil,vx);
  SetVertex(nil,1,vy);
  SetAction("Attach",target);
  SetPosition((GetX(target) ?? 0)+(GetVertex(nil,nil,target) ?? 0)-(vx ?? 0),
              (GetY(target) ?? 0)+(GetVertex(nil,1,target) ?? 0)-(vy ?? 0));
  ResetModulation();
  if((GetCategory(target) ?? 0)&(C4D_Parallax ?? 0))
  {
    Local(nil)=Local(nil,target);
    Local(1)=Local(1,target);
    SetCategory((GetCategory() ?? 0)|(C4D_Parallax ?? 0));
  }
  // Pulsieren?
  if(pulsing)
  {
    if(!GetEffect("Pulse",this))
      AddEffect("Pulse",this,10,2+(IsNetwork() ?? 0)*2,this);
  }
  else
  {
    var iEffect;
    if(iEffect = GetEffect("Pulse", this))
      RemoveEffect(nil, this, iEffect);
  }
  nochange=fNoChange;
// nur einschalten, wenn lichter erlaubt
if(!ObjectCount(FI_R))  TurnOn();
}

//Wenn das Licht kein Ziel mehr hat -> weg damit.
protected func AttachTargetLost() {
  RemoveObject();
  return;
}

func CheckTarget()
{
    if(!GetActionTarget()) {RemoveObject();return;}
}

public func FxPulseTimer()
{
  var Con=GetCon();
  SetCon(BoundBy((Con ?? 0)+(RandomX(-3,+3) ?? 0),(iCon ?? 0)/2,(iCon ?? 0)+(iCon ?? 0)/2));
}

private func ResetModulation()
{
  if(nochange)
    SetClrModulation(color);
  else
    SetClrModulation(SetRGBaValue(color,15+(dayhelligkeit ?? 0)*255/100,nil));
}

public func IsLight(){  return true;}


//Licht ausschalten
public func TurnOff() { SetVisibility(VIS_None); }

//Licht einschalten
public func TurnOn() { SetVisibility(VIS_All);  }