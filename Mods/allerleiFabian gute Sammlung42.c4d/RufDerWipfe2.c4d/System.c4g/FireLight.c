/*-- Feuerlichter --*/

#strict

static const RDW_Firelight_Var=15;

global func FxFireStart(object obj, int iNumber, int iTemp, int iCausedBy, bool fBlasted, object pIncineratingObject)
{
  var ret=inherited(obj, iNumber, iTemp, iCausedBy, fBlasted, pIncineratingObject);
  if(ret>-1)
    if(!iTemp)
      if(dayhelligkeit<30)
        //if(GetCategory(obj)&C4D_Structure||GetCategory(obj)&C4D_Vehicle||GetCategory(obj)&C4D_StaticBack||GetCategory(obj)&C4D_Object)
if (obj->GetID()!= DFLM && obj->GetID()!= DFL2 && obj->GetID()!= FLAM && obj->GetID()!= MFLM && obj->GetID()!= GRA9 && obj->GetID()!= HZCK&& obj->GetID()!= _Y7D && obj->GetID()!= NSWA && obj->GetID()!= SWAV && obj->GetID()!= SFRX && obj->GetID()!= MTNT && GetID(obj)!=MONS)
          if(!IsAntiLag() && !(ObjectCount(FI_R)))
          {
            EffectVar(RDW_Firelight_Var,obj,iNumber)=CreateObject(LGFL,0,0,-1);
            EffectVar(RDW_Firelight_Var,obj,iNumber)->Init(obj,0,0,GetDefWidth(GetID(obj))*(GetCon(obj)-10)*4/100,RGB(150,100,50),true);
          }
  return(ret);
}

global func FxFireTimer(obj,iNumber,iTime)
{
  if(EffectVar(RDW_Firelight_Var,obj,iNumber))
    EffectVar(RDW_Firelight_Var,obj,iNumber)->Init(obj,0,0,(GetDefWidth(GetID(obj))*(GetCon(obj)-10)*4/100)+RandomX(-10,10),RGB(150,100,50),true);
  return(inherited(Par(), Par(1), Par(2), Par(3), Par(4), Par(5), Par(6), Par(7), Par(8), Par(9)));
}

global func FxFireStop(obj,iNumber,iReason,ftemp)
{
  if(!ftemp)
    if(EffectVar(RDW_Firelight_Var,obj,iNumber))
      RemoveObject(EffectVar(RDW_Firelight_Var,obj,iNumber));
  return(inherited(Par(), Par(1), Par(2), Par(3), Par(4), Par(5), Par(6), Par(7), Par(8), Par(9)));
}
