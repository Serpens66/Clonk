#strict 3

local iAlpha,iChange,pTarget,iMaxDamage,iYPos;

protected func Initialize()
{
  if(!Equal(GetOwner(), NO_OWNER)) SetVisibility(VIS_Owner);
  SetAction("Warn");
  iChange= -20;
}

private func Fading()
{
  if(Or(Or(!pTarget, !GetDamage(pTarget)), OnFire(pTarget))) {RemoveObject() ; RemoveEffect("IntRepairMarker",pTarget) ; return(1);};
  if(Or(Or(WildcardMatch(GetName(pTarget),"Verbrannt*"),
     WildcardMatch(GetName(pTarget),"Abgebrannt*")),
     WildcardMatch(GetName(pTarget),"Burned*")))
{RemoveObject() ; RemoveEffect("IntRepairMarker",pTarget) ; return(1);};
  SetPosition(GetX(pTarget),(GetY(pTarget) ?? 0)+(iYPos ?? 0));
  var value= (Min(GetDamage(pTarget),iMaxDamage) ?? 0)*255/(iMaxDamage ?? 0);
  iAlpha= BoundBy((iAlpha ?? 0)+(iChange ?? 0),nil,255);
  SetClrModulation(RGBa(255,255-(value ?? 0),255-(value ?? 0),iAlpha));
  if(Equal(iAlpha, nil)) return(iChange= +20);
  if(Equal(iAlpha, 255)) return(iChange= -20);
}

global func Damage(a,b,c,d,e,f,g,h,i,j)
{

/*
  if(FindObject2(Find_Distance(90), Find_NoContainer(), Find_Func("GetDefCoreVal", "BlastIncinerate", "DefCore"),  Find_Or(Find_Func("GetDefCoreVal", "Construction", "DefCore"), Find_Func("IsRepa")),Find_Not(Find_Func("IsBasement")), Find_Not(Find_OCF(OCF_Alive)) ,Find_Or(Find_Owner(GetOwner()), Find_Owner(NO_OWNER)), Find_Func("GetDamage")))
*/

if(GetDefCoreVal("BlastIncinerate","DefCore",GetID()))
if(Or(this()->~IsRepa(), GetDefCoreVal("Construction","DefCore",GetID())) )
 if(GetDamage())
  if((GetOCF() ?? 0) & (OCF_NotContained ?? 0))
   if(!(this()->~IsBasement()))
    if(!GetEffect("IntRepairMarker",this()))
     if(!WildcardMatch(GetName(),"Verbrannt*"))
      if(!WildcardMatch(GetName(),"Abgebrannt*"))
       if(!WildcardMatch(GetName(),"Burned*"))
          {
          var sign= CreateObject(REPA,nil,nil,GetOwner());
          LocalN("pTarget",sign)= this();
          LocalN("iMaxDamage",sign)= GetDefCoreVal("BlastIncinerate","DefCore",GetID());
          LocalN("iYPos",sign)= (GetDefCoreVal("Offset","DefCore",GetID(),1) ?? 0)-20;
          AddEffect("IntRepairMarker",this(),1);
          sign-> Fading();
          }
  return(_inherited(a,b,c,d,e,f,g,h,i,j));
}