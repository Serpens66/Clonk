#strict 3

/* Unstuck-Effect */
/*Effekt der Objekte augenfreundlich aus Materialien herauszieht.*/
global func FxIntUnstuck4KStart(object pTarget, int iEffectNumber, int iTemp,iXDir,iYDir)
{
  iEffectNumber ??= 0;
  iTemp ??= 0;
  if(And(!iXDir, !iYDir))
  {
    for(var j = 10;(j ?? 0) < 60;S42MigrationAssign(j, "+=", 10))
    {
      for(var i = -90;(i ?? 0) < 360-90;S42MigrationAssign(i, "+=", 45))//In 45° Schritten testen.
      {
        iXDir = Cos(i,j);
        iYDir = Sin(i,j);
        if(!GBackSolid(iXDir,iYDir))
        {
          Normalize4K(iXDir,iYDir);
          EffectVar (nil,pTarget,iEffectNumber) = iXDir;
          EffectVar (1,pTarget,iEffectNumber) = iYDir;
          return(1);
        }
      }
    }
    
    EffectVar (nil,pTarget,iEffectNumber) =  nil;
    EffectVar (1,pTarget,iEffectNumber) = -1;
  }
  else
  {
    EffectVar (nil,pTarget,iEffectNumber) = iXDir;
    EffectVar (1,pTarget,iEffectNumber) = iYDir;
  }
  return(1);
}

global func FxIntUnstuck4KTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  if(!Stuck(pTarget))
    return(-1);

  if((iEffectTime ?? 0) > 10)
    return(-1);

  SetPosition((GetX(pTarget) ?? 0)  -  (EffectVar (nil,pTarget,iEffectNumber) ?? 0),
              (GetY(pTarget) ?? 0)  -  (EffectVar (1,pTarget,iEffectNumber) ?? 0));

  return(nil);
}

global func AutoUnstuck(pTarget,iXDir,iYDir)
{
  AutoUnstuck4K(pTarget,iXDir,iYDir);
}

global func AutoUnstuck4K(pTarget,iXDir,iYDir)
{
  if(!GetEffect("IntUnstuck4K",pTarget))
    AddEffect("IntUnstuck4K",pTarget,1,1,pTarget,nil,iXDir,iYDir);
}


/* Rauch-Effect */
/*Effekt der Wraks schön rauchen und brennen lässt.*/
global func FxIntWreckSmoke4KStart(object pTarget, int iEffectNumber, int iTemp,iP,iX,iY,bF)
{
  iEffectNumber ??= 0;
  iTemp ??= 0;
  EffectVar (nil,pTarget,iEffectNumber) = iP;
  EffectVar (1,pTarget,iEffectNumber) = iX;
  EffectVar (2,pTarget,iEffectNumber) = iY;
  EffectVar (3,pTarget,iEffectNumber) = bF;
  return(1);
}

global func FxIntWreckSmoke4KTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  if(!pTarget)
    return(-1);

  var iP = EffectVar (nil,pTarget,iEffectNumber);
  var iX = EffectVar (1,pTarget,iEffectNumber);
  var iY = EffectVar (2,pTarget,iEffectNumber);
  
  GetOffset4K(iX,iY);
  
  var vel = (Abs(GetXDir(pTarget)) ?? 0)+(Abs(GetYDir(pTarget)) ?? 0);
  var alpha = Max(nil,30-(vel ?? 0));
  var black = 40 + (Random(40) ?? 0);
  
  if(EffectVar(3,pTarget,iEffectNumber))
    CreateParticle("Thrust",iX,iY,(GetXDir() ?? 0)/2,(GetYDir() ?? 0)/2,(iP ?? 0) + (Random((iP ?? 0)/2) ?? 0) + (vel ?? 0)/3 + 40,RGBa(255,200,200,60));
  
  CreateParticle("Smoke3",iX,iY,RandomX(-1,1),-(Random(5) ?? 0),(iP ?? 0) + (Random((iP ?? 0)/2) ?? 0) + (vel ?? 0)/3,RGBa(black,black,black,alpha));

  return(nil);
}

global func AddSmokeEffect4K(iStrength,iX,iY,bBurning,pTarget)
{
  if(!pTarget) pTarget = this();
  if(!pTarget) return(nil);
  AddEffect ("IntWreckSmoke4K",pTarget,10,2,pTarget,nil,iStrength,iX,iY,bBurning);
  return(1);
}


/*SpeedSecure®-Effekt*/
//Das Objekt kann bei hohen Geschwindigkeiten nicht eingesammelt werden.

global func FxIntSpeedSecure4KStart(object pTarget, int iEffectNumber, int iTemp,max_speed)
{
  iEffectNumber ??= 0;
  iTemp ??= 0;
  EffectVar (nil,pTarget,iEffectNumber) = max_speed;
  return(1);
}

global func FxIntSpeedSecure4KTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  if((Distance(GetXDir(pTarget),GetYDir(pTarget)) ?? 0) > (EffectVar (nil,pTarget,iEffectNumber) ?? 0))
    SetCategory(C4D_Vehicle,pTarget);
  else
    SetCategory(C4D_Object,pTarget);
  return(nil);
}

global func FxIntSpeedSecure4KStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  iEffectNumber ??= 0;
  iReason ??= 0;
  fTemp ??= false;
  SetCategory(C4D_Object,pTarget);
  return(-1);
}

global func SetSpeedSecure(iMaxSpeed,pTarget)
{
  if(!pTarget) pTarget = this();
  if(!pTarget) return(nil);
  
  if(!iMaxSpeed) iMaxSpeed = 20;
  
  AddEffect ("IntSpeedSecure4K",pTarget,200,5,pTarget,nil,iMaxSpeed);
}

/* Neue Aus- und Einfade-Effekte (Original gabs im Hazard-Pack.) */
global func FxFadeOut4KStart(target, no, temp, speed)
{
  EffectVar(1, target, no) = Max(speed,1);
}

global func FxFadeOut4KTimer(target, no)
{
  EffectVar(nil, target, no) = BoundBy((EffectVar(nil, target, no) ?? 0) + (EffectVar(1, target, no) ?? 0), nil, 255);
  SetClrModulation(RGBa(255,255,255, EffectVar(nil, target, no)), target);
  
  if((EffectVar(nil, target, no) ?? 0) >= 255)
  {
    RemoveObject(target);
    return(-1);
  }
}

global func FadeOut4K(int iSpeed, object pObject)
{
  iSpeed ??= 0;
  if(!pObject) pObject = this();
  if(!pObject) return;

  if(GetEffect("*FadeOut*", pObject)) return;
  return(AddEffect("FadeOut4K", pObject, 101, 2,nil,nil,iSpeed));
}

global func FxFadeIn4KStart(target, no, temp, speed)
{
  EffectVar(nil, target, no) = 255;
  EffectVar(1, target, no) = Max(speed,1);
  SetClrModulation(RGBa(255,255,255, EffectVar(nil, target, no)), target);
}

global func FxFadeIn4KTimer(target, no)
{
  EffectVar(nil, target, no) = BoundBy((EffectVar(nil, target, no) ?? 0) - (EffectVar(1, target, no) ?? 0), nil, 255);
  SetClrModulation(RGBa(255,255,255, EffectVar(nil, target, no)), target);
  
  if((EffectVar(nil, target, no) ?? 0) <= 0)
  {
    return(-1);
  }
}

global func FadeIn4K(int iSpeed, object pObject)
{
  iSpeed ??= 0;
  if(!pObject) pObject = this();
  if(!pObject) return;

  if(GetEffect("*FadeIn*", pObject)) return;
  return(AddEffect("FadeIn4K", pObject, 101, 2,nil,nil,iSpeed));
}


/* TimedMessage-Effekt */
//Wie Message(), nur das ein Parameter die Anzeigezeit festlegt.

global func FxIntTimedMessage4KStart(object pTarget, int iEffectNumber, int iTemp,szMessage)
{
  iEffectNumber ??= 0;
  iTemp ??= 0;
  Message("@%s",pTarget,szMessage);
}

global func FxIntTimedMessage4KTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  Message(" ",pTarget);
  return(-1);
}

global func FxIntTimedMessage4KStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  //...
iEffectNumber ??= 0;
iReason ??= 0;
fTemp ??= false;
}

global func TimedMessage4K(string szMessage, object pTarget, int iTime)
{
  iTime ??= 0;
  if(!iTime) iTime = 35*3;
  
  var iEffect;
  var i = GetEffectCount("IntTimedMessage4K",pTarget);
  while(S42MigrationStep(i, -1, true))
    if(iEffect = GetEffect("IntTimedMessage4K",pTarget,i))
      RemoveEffect(nil, pTarget, iEffect);
  
  AddEffect("IntTimedMessage4K",pTarget,10,iTime,pTarget,nil,Format(szMessage,...));
}

/* TimedPlayerMessage-Effekt */
//Wie Message(), nur das ein Parameter die Anzeigezeit festlegt.

global func FxIntTimedPlayerMessage4KStart(object pTarget, int iEffectNumber, int iTemp,szMessage,iPlr)
{
  iEffectNumber ??= 0;
  iTemp ??= 0;
  PlayerMessage(iPlr,"@%s",pTarget,szMessage);
  EffectVar(nil,pTarget,iEffectNumber) = iPlr;
}

global func FxIntTimedPlayerMessage4KTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  PlayerMessage(EffectVar(nil,pTarget,iEffectNumber)," ",pTarget);
  return(-1);
}

global func FxIntTimedPlayerMessage4KStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  //...
iEffectNumber ??= 0;
iReason ??= 0;
fTemp ??= false;
}

global func TimedPlayerMessage4K(int iPlr, string szMessage, object pTarget, int iTime)
{
  iPlr ??= 0;
  iTime ??= 0;
  if(!iTime) iTime = 35*3;
  
  var iEffect;
  var i = GetEffectCount("IntTimedPlayerMessage4K",pTarget);
  while(S42MigrationStep(i, -1, true))
    if(iEffect = GetEffect("IntTimedPlayerMessage4K",pTarget,i))
      RemoveEffect(nil, pTarget, iEffect);
  
  AddEffect("IntTimedPlayerMessage4K",pTarget,10,iTime,pTarget,nil,Format(szMessage,...),iPlr);
}