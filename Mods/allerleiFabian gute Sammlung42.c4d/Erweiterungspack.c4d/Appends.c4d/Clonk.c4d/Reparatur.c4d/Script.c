#strict

local iAlpha,iChange,pTarget,iMaxDamage,iYPos;

protected func Initialize()
{
  if(GetOwner() != NO_OWNER()) SetVisibility(VIS_Owner());
  SetAction("Warn");
  iChange= -20;
}

private func Fading()
{
  if(!pTarget || !GetDamage(pTarget) || OnFire(pTarget)) {RemoveObject() ; RemoveEffect("IntRepairMarker",pTarget) ; return(1);};
  if(WildcardMatch(GetName(pTarget),"Verbrannt*") || 
     WildcardMatch(GetName(pTarget),"Abgebrannt*") ||
     WildcardMatch(GetName(pTarget),"Burned*"))
{RemoveObject() ; RemoveEffect("IntRepairMarker",pTarget) ; return(1);};
  SetPosition(GetX(pTarget),GetY(pTarget)+iYPos);
  var value= Min(GetDamage(pTarget),iMaxDamage)*255/iMaxDamage;
  iAlpha= BoundBy(iAlpha+iChange,0,255);
  SetClrModulation(RGBa(255,255-value,255-value,iAlpha));
  if(iAlpha == 0) return(iChange= +20);
  if(iAlpha == 255) return(iChange= -20);
}

global func Damage(a,b,c,d,e,f,g,h,i,j)
{

/*
  if(FindObject2(Find_Distance(90), Find_NoContainer(), Find_Func("GetDefCoreVal", "BlastIncinerate", "DefCore"),  Find_Or(Find_Func("GetDefCoreVal", "Construction", "DefCore"), Find_Func("IsRepa")),Find_Not(Find_Func("IsBasement")), Find_Not(Find_OCF(OCF_Alive)) ,Find_Or(Find_Owner(GetOwner()), Find_Owner(NO_OWNER)), Find_Func("GetDamage")))
*/

if(GetDefCoreVal("BlastIncinerate","DefCore",GetID()))
if(this()->~IsRepa() || GetDefCoreVal("Construction","DefCore",GetID()) )
 if(GetDamage())
  if(GetOCF() & OCF_NotContained())
   if(!(this()->~IsBasement()))
    if(!GetEffect("IntRepairMarker",this()))
     if(!WildcardMatch(GetName(),"Verbrannt*"))
      if(!WildcardMatch(GetName(),"Abgebrannt*"))
       if(!WildcardMatch(GetName(),"Burned*"))
          {
          var sign= CreateObject(REPA,0,0,GetOwner());
          LocalN("pTarget",sign)= this();
          LocalN("iMaxDamage",sign)= GetDefCoreVal("BlastIncinerate","DefCore",GetID());
          LocalN("iYPos",sign)= GetDefCoreVal("Offset","DefCore",GetID(),1)-20;
          AddEffect("IntRepairMarker",this(),1);
          sign-> Fading();
          }
  return(_inherited(a,b,c,d,e,f,g,h,i,j));
}