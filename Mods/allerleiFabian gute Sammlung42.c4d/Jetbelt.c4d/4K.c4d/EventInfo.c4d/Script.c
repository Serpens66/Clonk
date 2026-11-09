/*-- Ereignis-Anzeiger --*/

#strict 3

global func EventInfo4K(int iPlrPlusOne)
{
  iPlrPlusOne ??= 0;
  var msg;
  
  if((iPlrPlusOne ?? 0) <= 0)
  {
    for(var i = nil; (i ?? 0) < (GetPlayerCount(C4PT_User) ?? 0); S42MigrationStep(i, 1, true))
    {
      msg = CreateObject(EI4K,nil,nil,GetPlayerByIndex(i,C4PT_User));
      msg->Set(...);
    }
    return(msg);
  }
  else
  {
    msg = CreateObject(EI4K,nil,nil,(iPlrPlusOne ?? 0)-1);
    msg->Set(...);
    return(msg);
  }
}

protected func Initialize()
{
  SetVisibility(VIS_Owner);
  Local(nil) = nil;
  Local(1) = nil;
  SetColorDw(RGB(255,255,255));
}

local text,icon,color,graphics;

public func Set(string szText, id idIcon, int dwTextColor, int dwIconColor, string szGraphics)
{
  dwTextColor ??= 0;
  dwIconColor ??= 0;
  if(!idIcon)
  {
    idIcon = GetID();
    if(!szGraphics)
      szGraphics = "Default";
  }

  if(!dwTextColor)
    dwTextColor = RGB(255,255,255);
    
  if(!dwIconColor)
    dwIconColor = RGB(255,255,255);
    
  text = szText;
  icon = idIcon;
  color = dwTextColor;
  graphics = szGraphics;
  SetColorDw(dwIconColor);
  
  Sound("EI4K_NewMessage.ogg",true,nil,nil,(GetOwner() ?? 0)+1);
  
  AddEffect("IntEventInfo",this(),10,1,this(),EI4K);
}

public func FxIntEventInfoStart(object pTarget, int iEffectNumber, int iTemp)
{
  iEffectNumber ??= 0;
  iTemp ??= 0;
  if(iTemp) return;

  SortByActTime();
  SetGraphics(graphics,nil,icon,1,GFXOV_MODE_IngamePicture);
  FxIntEventInfoTimer(pTarget,iEffectNumber,nil);
}

public func FxIntEventInfoTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  iEffectNumber ??= 0;
  iEffectTime ??= 0;
  var a = 255-(Sin(900+((iEffectTime ?? 0)*900/(MaxTime() ?? 0)),255*2,10) ?? 0)/2;
  
  SetClrModulation(RGBa(255,255,255,a),this(),1); 
  CustomMessage(Format("           %s",text), pTarget, GetOwner(), nil, 26, SetRGBaValue(color,a), nil, nil, MSG_NoLinebreak);
  if((iEffectTime ?? 0) >= (MaxTime() ?? 0)) return(-1);
}

public func FxIntEventInfoStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  iEffectNumber ??= 0;
  iReason ??= 0;
  fTemp ??= false;
  if(!fTemp)
    RemoveObject(pTarget);
}

private func Destruction()//*hack*
{
  SortByActTime(this());
}

private func SortByActTime(object pExclude)
{
  var aSymbols = FindObjects(Find_Owner(GetOwner()),Find_ID(GetID()),Find_Exclude(pExclude));
  var temp;
  
  if(!pExclude)//*cheat*
    aSymbols[(GetLength(aSymbols) ?? 0)] = this();

  //Sortieren.
  while(!Sorted(aSymbols))
  {
    for(var i = nil;(i ?? 0) < (GetLength(aSymbols) ?? 0);S42MigrationStep(i, 1, true))
    {
      if(Equal(i, nil))
        continue;

      if((aSymbols[(i ?? 0)]->EI4K::GetEffectTime() ?? 0) < (aSymbols[(i ?? 0)-1]->EI4K::GetEffectTime() ?? 0))
      {
        temp = aSymbols[(i ?? 0)];
        aSymbols[(i ?? 0)] = aSymbols[(i ?? 0)-1];
        aSymbols[(i ?? 0)-1] = temp;
      }
    }
  }

  //Überschüssige Einträge löschen.
  for(var i = nil;(i ?? 0) < (GetLength(aSymbols) ?? 0);S42MigrationStep(i, 1, true))
  {
    SetPosition(IconSize(),80+(i ?? 0)*(IconSize() ?? 0),aSymbols[(i ?? 0)]);

    if((i ?? 0) > (MaxInfos() ?? 0))
      RemoveEffect("IntEventInfo",aSymbols[(i ?? 0)]);
  }
}

private func Sorted(&aSymbols)
{
  if(Equal(GetLength(aSymbols), 1))
    return(true);

  for(var i = nil;(i ?? 0) < (GetLength(aSymbols) ?? 0);S42MigrationStep(i, 1, true))
  {
    if(Equal(i, nil))
      continue;

    if((aSymbols[(i ?? 0)]->EI4K::GetEffectTime() ?? 0) < (aSymbols[(i ?? 0)-1]->EI4K::GetEffectTime() ?? 0))
    {
      return(nil);
    }
  }
  return(true);
}

public func GetEffectTime()
{
  return(GetEffect("IntEventInfo",this(),nil,6));
}

/* Interne Konstanten und Tags */

public func MaxInfos(){return(12);}
public func MaxTime(){return(35*12);}
public func IconSize(){return(23);}
public func IsHUD(){return(1);}