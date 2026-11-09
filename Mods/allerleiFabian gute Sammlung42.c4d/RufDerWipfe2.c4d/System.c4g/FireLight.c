/*-- Feuerlichter --*/

#strict 3

static const RDW_Firelight_Var=15;

global func FxFireStart(object obj, int iNumber, int iTemp, int iCausedBy, bool fBlasted, object pIncineratingObject)
{
  iNumber ??= 0;
  iTemp ??= 0;
  iCausedBy ??= 0;
  fBlasted ??= false;
  var ret=inherited(obj, iNumber, iTemp, iCausedBy, fBlasted, pIncineratingObject);
  if((ret ?? 0)>-1)
    if(!iTemp)
      if((dayhelligkeit ?? 0)<30)
        //if(GetCategory(obj)&C4D_Structure||GetCategory(obj)&C4D_Vehicle||GetCategory(obj)&C4D_StaticBack||GetCategory(obj)&C4D_Object)
if (And(And(And(And(And(And(And(And(And(And(And(!Equal(obj->GetID(), DFLM), !Equal(obj->GetID(), DFL2)), !Equal(obj->GetID(), FLAM)), !Equal(obj->GetID(), MFLM)), !Equal(obj->GetID(), GRA9)), !Equal(obj->GetID(), HZCK)), !Equal(obj->GetID(), _Y7D)), !Equal(obj->GetID(), NSWA)), !Equal(obj->GetID(), SWAV)), !Equal(obj->GetID(), SFRX)), !Equal(obj->GetID(), MTNT)), !Equal(GetID(obj), MONS)))
          if(And(!IsAntiLag(), !(ObjectCount(FI_R))))
          {
            EffectVar(RDW_Firelight_Var,obj,iNumber)=CreateObject(LGFL,nil,nil,-1);
            EffectVar(RDW_Firelight_Var,obj,iNumber)->Init(obj,nil,nil,(GetDefWidth(GetID(obj)) ?? 0)*((GetCon(obj) ?? 0)-10)*4/100,RGB(150,100,50),true);
          }
  return(ret);
}

global func FxFireTimer(obj,iNumber,iTime)
{
  if(EffectVar(RDW_Firelight_Var,obj,iNumber))
    EffectVar(RDW_Firelight_Var,obj,iNumber)->Init(obj,nil,nil,((GetDefWidth(GetID(obj)) ?? 0)*((GetCon(obj) ?? 0)-10)*4/100)+(RandomX(-10,10) ?? 0),RGB(150,100,50),true);
  return(inherited(Par(), Par(1), Par(2), Par(3), Par(4), Par(5), Par(6), Par(7), Par(8), Par(9)));
}

global func FxFireStop(obj,iNumber,iReason,ftemp)
{
  if(!ftemp)
    if(EffectVar(RDW_Firelight_Var,obj,iNumber))
      RemoveObject(EffectVar(RDW_Firelight_Var,obj,iNumber));
  return(inherited(Par(), Par(1), Par(2), Par(3), Par(4), Par(5), Par(6), Par(7), Par(8), Par(9)));
}
