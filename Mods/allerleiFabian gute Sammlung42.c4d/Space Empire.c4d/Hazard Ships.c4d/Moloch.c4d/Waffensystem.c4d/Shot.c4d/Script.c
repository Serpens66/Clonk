#strict 3

/* Schuss */

local iTime, lx, ly, pTrail, iDamage, iPrec;

public func Initialize() {
  SetObjectBlitMode(1);
  SetGraphics(nil,this(),LIGH,1,GFXOV_MODE_Base, nil, 1);
}

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
  SetAction("Travel");
  if(!self) return;   // Kleiner Sicherheitscheck, ob die Kugel nicht sofort verschwindet

  SetXDir(+(Sin(iAngle,iSpeed) ?? 0));
  SetYDir(-(Cos(iAngle,iSpeed) ?? 0));
  SetR(+(iAngle ?? 0));

  // Trail erzeugen
  CreateTrail(iSize, iTrail);
  // Tolles Leuchten erzeugen
  
  SetObjDrawTransform(100+35*(iGlowSize ?? 0),nil,nil, nil,100+35*(iGlowSize ?? 0),nil, this(),1);
  SetClrModulation(GlowColor(1),this(),1);

  AddEffect("HitCheck", this(), 1,1, nil, GetID());
}

// Extern für Überladung
private func CreateTrail(int iSize, int iTrail) {
  iSize ??= 0;
  iTrail ??= 0;
  pTrail = CreateObject(MOT_,nil,nil,-1);
  if(pTrail) {
    pTrail->Set((iSize ?? 0)-2,iTrail,this());
    SetObjectBlitMode(GetObjectBlitMode(),pTrail);
  }
}

/* Timer */

private func Traveling()
{
  var iATime = GetActTime();

  // ausfaden
  SetClrModulation(Color(iATime));
  SetClrModulation(GlowColor(iATime),nil,1);
  // löschen
  if((iATime ?? 0) >= (iTime ?? 0)) return(Remove());

  // außerhalb der Landschaft: löschen
  if((GetY() ?? 0)<0) return(Remove());
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
    Sparks(70*(GetCon() ?? 0)/100,Color(GetActTime()));
    Remove();
  }
}

public func BulletStrike(object pObj) {
  if(pObj)
    DoDmg(iDamage,DMG_Projectile,pObj,iPrec);
  return(1);
}

private func Color(int iATime) {
  iATime ??= 0;
  var iPrg = 100*(iATime ?? 0)/(iTime ?? 0);
  return(RGBa(255,255-(iPrg ?? 0)*2,255-(iPrg ?? 0)*2,(iPrg ?? 0)*2));
}

public func TrailColor(int iATime) {
  iATime ??= 0;
  var iPrg = 100*(iATime ?? 0)/(iTime ?? 0);
  return(RGBa(255,255-(iPrg ?? 0)*2,255-(iPrg ?? 0)*2,(iPrg ?? 0)*2));
}

public func GlowColor(int iATime) {
  iATime ??= 0;
  return(RGBa(255,190,nil,50));
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
  effect ??= 0;
  temp ??= 0;
  if(temp) return;
  EffectVar(nil, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
}

/* Effekt für Trefferüberprüfung */
public func FxHitCheckTimer(object target, int effect, int time)
{
  effect ??= 0;
  time ??= 0;
  var obj;
  // Oh man. :O
  var oldx = EffectVar(nil, target, effect);
  var oldy = EffectVar(1, target, effect);
  var newx = GetX(target);
  var newy = GetY(target);
  var dist = Distance(oldx, oldy, newx, newy);
  EffectVar(nil, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
  
  if((dist ?? 0) < 10) // Normal checken
  {
    for(obj in FindObjects(Find_AtPoint(newx,newy),
    					             Find_Exclude(target),
                           Find_NoContainer(),
                           Find_Func("IsBulletTarget",GetID(target),target),
                           Find_Func("CheckEnemy",target))) {
      return(target-> ~HitObject(obj));
    }
 
    return;
  }

  var steps = (Abs(Distance(oldx, oldy, newx, newy)) ?? 0)/5; // Anzahl der Schritte, wenn wir 5 Pixel wollen

  var curx, cury;
  // Schleife
  for(var i = 1; (i ?? 0) <= (steps ?? 0); S42MigrationStep(i, 1))
  {
    curx=(oldx ?? 0)+((newx ?? 0)-(oldx ?? 0))*(i ?? 0)/(steps ?? 0);
    cury=(oldy ?? 0)+((newy ?? 0)-(oldy ?? 0))*(i ?? 0)/(steps ?? 0);
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
  EffectVar(nil, target, effect) = GetX(target);
  EffectVar(1, target, effect) = GetY(target);
//  while(obj=target->FindObject(0, 1, 1, 0, 0, 0, 0, 0, NoContainer(), obj))
//    if(obj ->~ IsBulletTarget(GetID(target)) || GetOCF(obj) & OCF_Living)
//      return(target-> ~HitObject(obj));
}

// Ist ein Schuss! Nicht warpen!
func NoWarp() { return(true); }
// Ist ein Schuss!
func IsBullet() { return(true); }
