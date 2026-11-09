#strict 3
#include SHT1

local glow;

/* Schuss */

public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize)
{
  // Standardwerte setzen
  iAngle ??= 0;
  iSpeed ??= 0;
  iDist ??= 0;
  iSize ??= 0;
  iTrail ??= 0;
  iDmg ??= 0;
  iDmgPrec ??= 0;
  iGlowSize ??= 0;
  if(!iSize)    iSize = 8;
  if(!iGlowSize)
    iGlowSize = iSize;
  if(!iTrail)   iTrail = 300;
  if(!iDmg)     iDamage = 3;
  else iDamage = iDmg;

  // und zuweisen
  iSize = Min((iSize ?? 0)+2,GetDefWidth());
  iPrec = iDmgPrec;

  // Positionieren
  SetPosition(GetX(),(GetY() ?? 0)+(GetDefWidth() ?? 0)/2);

  DoCon(100*(iSize ?? 0)/(GetDefWidth() ?? 0)-100);

  lx = GetX();
  ly = GetY();

  iTime = 10*(iDist ?? 0)/(iSpeed ?? 0);
  
  if(!iTime)
    return(RemoveObject());

  var self = this();

  if(!self) return;   // Kleiner Sicherheitscheck, ob die Kugel nicht sofort verschwindet

  SetXDir(+(Sin(iAngle,iSpeed) ?? 0));
  SetYDir(-(Cos(iAngle,iSpeed) ?? 0));
  SetR(+(iAngle ?? 0));

  if((GetXDir() ?? 0)>0)  SetAction("TravelRight");
  else SetAction("TravelRight");
  // Trail erzeugen
  CreateTrail(iSize, iTrail);
  // Tolles Leuchten erzeugen
  
  SetObjDrawTransform(100+35*(iGlowSize ?? 0),nil,nil, nil,100+35*(iGlowSize ?? 0),nil, this(),1);
  SetClrModulation(GlowColor(1),this(),1);

  AddEffect("HitCheck", this(), 1,1, nil, GetID(),shooter);
}

//Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {
  //glow = AddLight(100,RGB(128,64,255),this(),GLOW);
  
  iSize ??= 0;
  iTrail ??= 0;
  pTrail = CreateObject(TRL5,nil,nil,-1);
  if(pTrail) {
    pTrail->Set((iSize ?? 0)+2,iTrail,this());
    pTrail->SetAction("Travel2");
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

private func Traveling()
{
  // effect
  if(glow)
    glow->ChangeColor(Color(GetActTime()));
  CreateParticle("PSpark",nil,nil,-(GetXDir() ?? 0)/4,-(GetYDir() ?? 0)/4,RandomX(100,200),
                 RGBa(64,32,255,100+50*(GetActTime() ?? 0)/(iTime ?? 0)),this());
  return(_inherited());
}

/* Treffer */

private func HitObject(object pObject) {
  if(Equal(pObject, shooter)) return;
  if(BulletStrike(pObject)) {
    LaserLight(RandomX(30,50), Color(GetActTime()),nil,nil,10);
    Remove();
  }
}

public func BulletStrike(object pObj) {
  if(pObj)
  if(!Equal(pObj, shooter))  // nicht sich selbst treffen!
 {
    Fling(pObj,(GetXDir() ?? 0)/50,-4); Sound("DePressurize"); }
  return(1);
}

private func Color(int iATime) {
  iATime ??= 0;
  var iPrg = 100*(iATime ?? 0)/(iTime ?? 0);
  return(RGBa(127,127-(iPrg ?? 0),255,(iPrg ?? 0)*2));
}

private func GlowColor(int iATime) {
  iATime ??= 0;
  var iPrg = 100*(iATime ?? 0)/(iTime ?? 0);
  return(RGBa(70,50,255-(iPrg ?? 0),50));
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