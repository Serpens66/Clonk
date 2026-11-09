#strict

/* Schuss */

local iTime, lx, ly, pTrail, iDamage, iPrec;

public func Initialize() {
  SetObjectBlitMode(1);
  SetGraphics(0,this(),LIGH,1,GFXOV_MODE_Base, 0, 1);
}

public func Launch(int iAngle, int iSpeed, int iDist, int iSize, int iTrail, int iDmg, int iDmgPrec, int iGlowSize)
{
  // Standardwerte setzen
  if(!iSize)    iSize = 8;
  if(!iGlowSize)
    iGlowSize = iSize;
  if(!iTrail)   iTrail = 300;
  if(!iDmg)     iDamage = 3;
  else iDamage = iDmg;

  // und zuweisen
  iSize = Min(iSize+2,GetDefWidth());
  iPrec = iDmgPrec;

  // Positionieren
  SetPosition(GetX(),GetY()+GetDefWidth()/2);

  DoCon(100*iSize/GetDefWidth()-100);

  lx = GetX();
  ly = GetY();

  iTime = 10*iDist/iSpeed;
  
  if(!iTime)
    return(RemoveObject());

  var self = this();
  SetAction("Travel");
  if(!self) return();   // Kleiner Sicherheitscheck, ob die Kugel nicht sofort verschwindet

  SetXDir(+Sin(iAngle,iSpeed));
  SetYDir(-Cos(iAngle,iSpeed));
  SetR(+iAngle);

  // Trail erzeugen
  CreateTrail(iSize, iTrail);
  // Tolles Leuchten erzeugen
  
  SetObjDrawTransform(100+35*iGlowSize,0,0, 0,100+35*iGlowSize,0, this(),1);
  SetClrModulation(GlowColor(1),this(),1);

  AddEffect("HitCheck", this(), 1,1, 0, GetID());
}

// Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {
  pTrail = CreateObject(MOT_,0,0,-1);
  if(pTrail) {
    pTrail->Set(iSize-2,iTrail,this());
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

/* Timer */

private func Traveling()
{
  var iATime = GetActTime();

  // ausfaden
  SetClrModulation(Color(iATime));
  SetClrModulation(GlowColor(iATime),0,1);
  // löschen
  if(iATime >= iTime) return(Remove());

  // außerhalb der Landschaft: löschen
  if(GetY()<0) return(Remove());
}
  
/* Treffer */

private func Hit() {

  // umliegende Objekte beschädigen.
  var objs = FindObjects(Find_AtPoint(),Find_Func("IsBulletTarget",GetID()),Find_NoContainer());
  for(var pTarget in objs)
    HitObject(pTarget);

  HitObject();
}

private func HitObject(object pObject) {

  if(BulletStrike(pObject)) {
    Sparks(70*GetCon()/100,Color(GetActTime()));
    Remove();
  }
}

public func BulletStrike(object pObj) {
  if(pObj)
    DoDmg(iDamage,DMG_Projectile,pObj,iPrec);
  return(1);
}

private func Color(int iATime) {
  var iPrg = 100*iATime/iTime;
  return(RGBa(255,255-iPrg*2,255-iPrg*2,iPrg*2));
}

public func TrailColor(int iATime) {
  var iPrg = 100*iATime/iTime;
  return(RGBa(255,255-iPrg*2,255-iPrg*2,iPrg*2));
}

public func GlowColor(int iATime) {
  return(RGBa(255,190,0,50));
}

public func Remove() {
  if(pTrail) {
    pTrail->SetPosition(GetX(),GetY());
    pTrail->Remove();
  }
  RemoveObject();
}

public func FxHitCheckStart(object target, int effect, int temp)
{
  if(temp) return();
  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
}

/* Effekt für Trefferüberprüfung */
public func FxHitCheckTimer(object target, int effect, int time)
{
  var obj;
  // Oh man. :O
  var oldx = EffectVar(0, target, effect);
  var oldy = EffectVar(1, target, effect);
  var newx = GetX(target);
  var newy = GetY(target);
  var dist = Distance(oldx, oldy, newx, newy);
  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
  
  if(dist < 10) // Normal checken
  {
    for(obj in FindObjects(Find_AtPoint(newx,newy),
    					             Find_Exclude(target),
                           Find_NoContainer(),
                           Find_Func("IsBulletTarget",GetID(target),target),
                           Find_Func("CheckEnemy",target))) {
      return(target-> ~HitObject(obj));
    }
 
    return();
  }

  var steps = Abs(Distance(oldx, oldy, newx, newy))/5; // Anzahl der Schritte, wenn wir 5 Pixel wollen

  var curx, cury;
  // Schleife
  for(var i = 1; i <= steps; ++i)
  {
    curx=oldx+(newx-oldx)*i/steps;
    cury=oldy+(newy-oldy)*i/steps;
    for(obj in FindObjects(Find_AtPoint(curx,cury),
    					             Find_Exclude(target),
                           Find_NoContainer(),
                           Find_Func("IsBulletTarget",GetID(target),target),			 
                           Find_Func("CheckEnemy",target)))
    {
      SetPosition(curx, cury, target);
      return(target-> ~HitObject(obj));
    }
  }
  EffectVar(0, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
//  while(obj=target->FindObject(0, 1, 1, 0, 0, 0, 0, 0, NoContainer(), obj))
//    if(obj ->~ IsBulletTarget(GetID(target)) || GetOCF(obj) & OCF_Living)
//      return(target-> ~HitObject(obj));
}

// Ist ein Schuss! Nicht warpen!
func NoWarp() { return(true); }
// Ist ein Schuss!
func IsBullet() { return(true); }
