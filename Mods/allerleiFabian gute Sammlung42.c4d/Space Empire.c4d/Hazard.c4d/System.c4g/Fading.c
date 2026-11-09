/*-- Fading --*/
// fading aus neuster hazard version 2.5

#strict 3

global func SetObjAlpha(byAlpha, pObj,iLayer)
{
  byAlpha = BoundBy(byAlpha,nil,255);
  var dwClrMod=GetClrModulation(pObj,iLayer);
  if(!dwClrMod)
    dwClrMod=RGB(255,255,255);

  dwClrMod=SetRGBaValue(dwClrMod,byAlpha);
  if(iLayer)
   return SetClrModulation(dwClrMod, pObj,iLayer);
  else
   return SetClrModulation(dwClrMod, pObj);
}

global func FxFadeOutStart(target, no, temp, int level, int speed)
{
  level ??= 0;
  speed ??= 0;
  if(!level)
    EffectVar(1, target, no) = 255;
  else
    EffectVar(1, target, no) = level;
	
	if(!speed) speed = 1;
	EffectVar(2, target, no) = speed;
}

global func FxFadeOutTimer(target, no)
{
  S42MigrationAssign(EffectVar(nil, target, no), "+=", (EffectVar(2, target, no) ?? 0));
  
  var alpha = EffectVar(nil, target, no);
  var maxAlpha = EffectVar(1, target, no);
  SetObjAlpha(alpha, target);
  if((alpha ?? 0) >= (maxAlpha ?? 0))
  {
    if((maxAlpha ?? 0) >= 255) RemoveObject(target);
    return -1;
  }
}

global func FadeOut(object pObject, int level, int speed)
{
  level ??= 0;
  speed ??= 0;
  if(!pObject)
    pObject = this();
    
  if(GetEffect("*FadeOut*", pObject)) return;
  return(AddEffect("FadeOut", pObject, 101, 2,nil,nil,level,speed));
}

global func FxFadeInStart(target, no, temp, int level, int speed)
{
	level ??= 0;
	speed ??= 0;
	if(!speed) speed = 1;
	
  EffectVar(nil, target, no) = 255;
  EffectVar(1, target, no) = level;
	EffectVar(2, target, no) = speed;
}

global func FxFadeInTimer(target, no)
{
  S42MigrationAssign(EffectVar(nil, target, no), "-=", (EffectVar(2, target, no) ?? 0));

  var alpha = EffectVar(nil, target, no);
  var maxAlpha = EffectVar(1, target, no);

  SetObjAlpha(alpha, target);
	
  if((alpha ?? 0) <= (maxAlpha ?? 0)) return -1;
}

global func FadeIn(object pObject, int level, int speed)
{
  level ??= 0;
  speed ??= 0;
  if(!pObject)
    pObject = this();
  
  if(GetEffect("*FadeIn*", pObject)) return;
  SetObjAlpha(255, pObject);
  return(AddEffect("FadeIn", pObject, 101, 2,nil,nil,level,speed));
}

global func StopFading(int color, object pObject) {
  color ??= 0;
  if(!pObject)
    pObject = this();
  
  var eff = GetEffectCount("*Fade*",pObject);
  for(eff; eff; S42MigrationStep(eff, -1, true))
    RemoveEffect("*Fade*",pObject);
  
  SetClrModulation(color,pObject);
}
