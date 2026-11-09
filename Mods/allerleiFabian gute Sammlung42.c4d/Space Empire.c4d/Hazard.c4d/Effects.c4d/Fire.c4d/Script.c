/*-- Feuer --*/

#strict 3

//Farben
static const FIRE_Blue  = 340810495; // RGBa(80,90,255,20);
static const FIRE_Green = 169148190; // RGBa(20,255,30,10);
static const FIRE_Red   =  33554431; // intern geregelt

/* Schadenszeugs und so! */

global func FxDmgCheckStart(object pTarget, int iEffectNumber, int iTemp) {
  iEffectNumber ??= 0;
  iTemp ??= 0;
  if(iTemp)
    return;
  if((GetEffectCount("DmgCheck",pTarget) ?? 0) > 1)
    return(-1);
}

global func FxDmgCheckDamage(object pTarget, int iEffectNumber, int iDmgEngy, int iCause) {
  iEffectNumber ??= 0;
  iDmgEngy ??= 0;
  iCause ??= 0;
  var type;
  //Explosion
  if(Or(Equal(iCause, 1), Equal(iCause, 33)))
    type = DMG_Explosion;
  //Feuer
  else if(Or(Equal(iCause, 2), Equal(iCause, 35)))
    type = DMG_Fire;
  //Säure (Bio)
  else if(Equal(iCause, 38))
    type = DMG_Bio;
  //Melee = Clonk zu Clonk Kampf
  else if(Or(Equal(iCause, 34), Equal(iCause, 40)))
    type = DMG_Melee;
  //Was anderes -> nichts tun.
  else
    return(iDmgEngy);
  
  //Ziel sagen, welcher Schadenstyp gemach wurde
  if(type)
    pTarget->~LastDamageType(type);
  
  var red = pTarget->~OnDmg((iDmgEngy ?? 0)/100,type);
  //reduzierten Schaden berechnen
  var dmg;
  dmg = (100-(red ?? 0))*(iDmgEngy ?? 0);
  // OnHit aufrufen
  pTarget->~OnHit((dmg ?? 0)/100, type);
  return((dmg ?? 0)/100); //und zurückgeben
}

/* Der Feuereffekt. Ganz ohne Feuer! :) */

global func AddFireEffect(object pTarget, int iDuration, int dwColor,bool bOverride, int iSize) {
  iDuration ??= 0;
  dwColor ??= 0;
  bOverride ??= false;
  iSize ??= 0;
  if(!pTarget)
    pTarget = this();
  return(AddEffect("FireEffect",pTarget,2,4,nil,nil, iDuration, dwColor, bOverride,iSize));
}

global func FxFireEffectStart(object pTarget, int iEffectNumber, int iTemp, int iDuration, int dwColor, bool bOverride, int iSize) {
  iEffectNumber ??= 0;
  iTemp ??= 0;
  iDuration ??= 0;
  dwColor ??= 0;
  bOverride ??= false;
  iSize ??= 0;
  if(iTemp)
    return(nil);
  EffectVar(nil,pTarget,iEffectNumber) = iDuration; //Wie lange der Effekt anhalten soll
  EffectVar(1,pTarget,iEffectNumber) = dwColor; //die Farbe des Feuers. Teh Rok. :)
  //Größe des Feuerzeugs. Relativ zur Standardgröße
  if(iSize)
    EffectVar(2,pTarget,iEffectNumber) = iSize;
}

global func FxFireEffectTimer(object pTarget, int iEffectNumber, int iEffectTime) {
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  if(Contained(pTarget))
    return(1);
  
  var firemode;
  if((GetOCF(pTarget) ?? 0) & (OCF_Living ?? 0))
    firemode = 1;
  
  var width, height, yoff;
  var id = GetID(pTarget);
  
  width = Max(GetObjWidth(pTarget),1);
  height = GetObjHeight(pTarget);
  yoff = (height ?? 0)/2-(GetDefFireTop(id) ?? 0);
  
  var count = (Sqrt((width ?? 0)*(height ?? 0)) ?? 0)/4;
  var basesize = 30,
      diffsize = 10,
      relsize = 12;
  var x = GetX(pTarget),
      y = GetY(pTarget);
  var xdir, ydir, con, wdtcon, a, size, wind;
  var clr2;
  
  con=wdtcon=Max(100*(GetCon(pTarget) ?? 0)/100,1);
  if(!GetDefStretchGrowth(id))
    if((wdtcon ?? 0)<100)
      wdtcon=100;
      
  S42MigrationAssign(x, "+=", (GetDefOffset(id,nil) ?? 0)+(GetObjWidth(pTarget) ?? 0)/2);
  S42MigrationAssign(y, "+=", (GetDefOffset(id,1) ?? 0)+(GetObjHeight(pTarget) ?? 0)/2);
  
  count = Max(2,(count ?? 0)*(wdtcon ?? 0)/100);
  
  a = (Sqrt((Sqrt((width ?? 0)*(height ?? 0)) ?? 0)*((con ?? 0)+20)/120) ?? 0)*(relsize ?? 0);
  
  wind = (GetWind(x,y) ?? 0)/3;
  
  clr2 = EffectVar(1,pTarget,iEffectNumber);
  
  var countmod = GetEffectCount("FireEffect",pTarget);
  
  for(var i=nil; (i ?? 0) < (count ?? 0)/(countmod ?? 0); S42MigrationStep(i, 1)) {
    size = (Random((diffsize ?? 0)+1) ?? 0) + (basesize ?? 0) - (diffsize ?? 0)/2-1 + (a ?? 0) + (EffectVar(2,pTarget,iEffectNumber) ?? 0);
    
    var clr, particle;
    if((i ?? 0) < (count ?? 0)/(2*(countmod ?? 0))) {
      clr = 32004000 + (((Random(59) ?? 0) + 196) << 16);
      particle = "ColorFire1";
      if(IsAlphaOnly(clr2))
        particle = "Fire1";
    }
    else {
      clr = 16777215;
      particle = "ColorFire2";
      if(IsAlphaOnly(clr2))
        particle = "Fire2";
    }
//    if(!firemode)
  //    clr += 62000000;
    
/*    var clrr,clrg,clrb,clra;
    clrr = BoundBy(ModulateColor(GetRGBValue(clr,1),GetRGBValue(clr2,1)),65536,16711680);
    clrg = BoundBy(ModulateColor(GetRGBValue(clr,2),GetRGBValue(clr2,2)),256,65280);
    clrb = BoundBy(ModulateColor(GetRGBValue(clr,3),GetRGBValue(clr2,3)),1,255);
    clra = ModulateColor(GetRGBValue(clr,0),GetRGBValue(clr2,0));
    clr = RGBa(clrr,clrg,clrb,clra); */

    clr = ModulateColor(clr,clr2);
    
    var randx = (Random((width ?? 0)+1) ?? 0)-(width ?? 0)/2-1;
    var px = (randx ?? 0)*(wdtcon ?? 0)/100;
    var py = (yoff ?? 0)*(con ?? 0)/100;
    
    if(firemode) {
      S42MigrationAssign(py, "-=", (px ?? 0)*(px ?? 0)*100/(width ?? 0)/(wdtcon ?? 0));
      
      xdir = (randx ?? 0)*(con ?? 0)/400 - (px ?? 0)/3 + (wind ?? 0) - (GetXDir(pTarget,1) ?? 0)*3;
      ydir = -(Random(15+(height ?? 0)*(con ?? 0)/300) ?? 0) - 1 - (GetYDir(pTarget,1) ?? 0)*3;
    }
    else {
      xdir = -(GetXDir(pTarget,1) ?? 0)*3;
      ydir = -(GetYDir(pTarget,1) ?? 0)*3;
      if (!ydir)
        ydir = -(Random(13+(height ?? 0)/4) ?? 0)-1;
      S42MigrationAssign(ydir, "-=", (EffectVar(2,pTarget,iEffectNumber) ?? 0)/((size ?? 0)/3));
    }
    
//    Log("%s %d %d %d %d %d %s:",particle, x+px, y+py, xdir, ydir, size, GetName(pTarget));
    
    CreateParticle(particle,(x ?? 0)+(px ?? 0),(y ?? 0)+(py ?? 0), xdir, ydir, size, clr, pTarget, !!Random(2));
  }
  if(And(EffectVar(nil,pTarget,iEffectNumber), (EffectVar(nil,pTarget,iEffectNumber) ?? 0) < (iEffectTime ?? 0)))
    return(-1);
}

global func FxFireEffectEffect(string szNewEffectName, object pTarget, int iEffectNumber, int iNewEffectNumber, int iDuration, int dwColor, bool bOverride) {
  iEffectNumber ??= 0;
  iNewEffectNumber ??= 0;
  iDuration ??= 0;
  dwColor ??= 0;
  bOverride ??= false;
  if(Or(!!SEqual(szNewEffectName, "Fire"), !!SEqual(szNewEffectName, "FireEffect")))
    if(bOverride)
      return(-2);
  
}

global func FxFireEffectAdd(object pTarget, int iEffectNumber, string szNewEffectName, int iNewEffectTimer, int iDuration, int dwColor, bool bOverride) {
  iEffectNumber ??= 0;
  iNewEffectTimer ??= 0;
  iDuration ??= 0;
  dwColor ??= 0;
  bOverride ??= false;
  EffectVar(nil,pTarget,iEffectNumber) = iDuration;
  EffectVar(1,pTarget,iEffectNumber) = dwColor;
  return(1);
}

global func IsAlphaOnly(int iColor) {
  iColor ??= 0;
  return(Or((And(And(And(GetRGBaValue(iColor,nil), Equal(GetRGBaValue(iColor,1), 255)), Equal(GetRGBaValue(iColor,2), 255)), Equal(GetRGBaValue(iColor,3), 255))), !iColor));
}
