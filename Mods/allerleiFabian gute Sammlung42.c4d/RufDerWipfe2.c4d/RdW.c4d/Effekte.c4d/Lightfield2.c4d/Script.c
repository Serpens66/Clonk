/*-- Lichtschein #2 --*/

#strict

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

  SetVertex(0,0,vx);
  SetVertex(0,1,vy);
  SetAction("Attach",target);
  SetPosition(GetX(target)+GetVertex(0,0,target)-vx,
              GetY(target)+GetVertex(0,1,target)-vy);
  ResetModulation();
  if(GetCategory(target)&C4D_Parallax)
  {
    Local(0)=Local(0,target);
    Local(1)=Local(1,target);
    SetCategory(GetCategory()|C4D_Parallax);
  }
  // Pulsieren?
  if(pulsing)
  {
    if(!GetEffect("Pulse",this))
      AddEffect("Pulse",this,10,2+IsNetwork()*2,this);
  }
  else
  {
    var iEffect;
    if(iEffect = GetEffect("Pulse", this))
      RemoveEffect(0, this, iEffect);
  }
  nochange=fNoChange;
// nur einschalten, wenn lichter erlaubt
if(!ObjectCount(FI_R))  TurnOn();
}

//Wenn das Licht kein Ziel mehr hat -> weg damit.
protected func AttachTargetLost() {
  RemoveObject();
  return();
}

func CheckTarget()
{
    if(!GetActionTarget()) {RemoveObject();return();}
}

public func FxPulseTimer()
{
  var Con=GetCon();
  SetCon(BoundBy(Con+RandomX(-3,+3),iCon/2,iCon+iCon/2));
}

private func ResetModulation()
{
  if(nochange)
    SetClrModulation(color);
  else
    SetClrModulation(SetRGBaValue(color,15+dayhelligkeit*255/100,0));
}

public func IsLight(){  return true;}


//Licht ausschalten
public func TurnOff() { SetVisibility(VIS_None); }

//Licht einschalten
public func TurnOn() { SetVisibility(VIS_All);  }