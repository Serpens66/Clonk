#strict 3
#include SHT1

local glow;

/* Schuss */

//Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {
  //glow = AddLight(100,RGB(128,64,255),this(),GLOW);
  
  iSize ??= 0;
  iTrail ??= 0;
  }


private func Traveling()
{
  // effect
  if(glow)
    glow->ChangeColor(Color(GetActTime()));
  CreateParticle("PSpark",nil,nil,-(GetXDir() ?? 0)/4,-(GetYDir() ?? 0)/4,RandomX(100,200),
                 RGBa(255,32,64,100+50*(GetActTime() ?? 0)/(iTime ?? 0)),this());
  return(_inherited());
}

/* Treffer */

private func HitObject(object pObject) {
  if(BulletStrike(pObject)) {
    LaserLight(RandomX(30,50), Color(GetActTime()),nil,nil,10);
    Remove();
  }
}

public func BulletStrike(object pObj) {
  if(pObj)  {
    var clnk=CreateObject(CLNK);
    clnk->GrabContents(pObj);
    clnk->RemoveObject(nil,1);
    Fling(pObj,(GetXDir() ?? 0)/100,-2);
    Sound("Electric");   
    }
  return(1);
}

private func Color(int iATime) {
  iATime ??= 0;
  var iPrg = 100*(iATime ?? 0)/(iTime ?? 0);
  return(RGBa(255,127-(iPrg ?? 0),64,(iPrg ?? 0)*2));
}

private func GlowColor(int iATime) {
  iATime ??= 0;
  var iPrg = 100*(iATime ?? 0)/(iTime ?? 0);
  return(RGBa(255-(iPrg ?? 0),50,64,50));
}

public func TrailColor(int iATime) { iATime ??= 0;
    return(Color(iATime)); }


func TurnLightOn()
{
  var iColor;
  iColor = RGBa(150,50,190,50);
  mlight = AddLight3(300, iColor, this());
  return(1);
}