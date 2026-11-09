/*--- Energieleitung ---*/

#strict 3

local fTransferStop;

local pTgt1,pTgt2;

protected func Initialize()
{
  Local(nil)=68;
  Local(1)=68;  
  Local(2)=30;
  Local(3)=30;

  //ChangeDef(PWR2,this());
  fTransferStop = nil;
  SetAction("Connect");  
  SetVertex(nil,nil,GetX()); SetVertex(nil,1,GetY());
  SetVertex(1,nil,GetX()); SetVertex(1,1,GetY());
  SetPosition(nil, nil, this() );
}

func ChangeTransfer()
{
Sound();
if( fTransferStop) {fTransferStop=nil; Sound("Energize",nil,this()); return(nil);}
if(!fTransferStop) {fTransferStop=1; Sound("DeEnergize",nil,this()); return(1);}
}

func LostConnection(pBuilding) 
{
    // Log(Format("LostConnection pBuilding %v , ActionTarget0: %v, ActionTarget1 %v",pBuilding,GetActionTarget(0),GetActionTarget(1)));
// Umspannwerk als Leitungsursprung  
if(!!SEqual(Format("%i",GetID(pBuilding)), "_USW")) if(!Equal(GetID(GetActionTarget(nil)), LNKT)) pBuilding->LostTarget(GetActionTarget(nil),nil);
if(!!SEqual(Format("%i",GetID(pBuilding)), "_USW")) if(!Equal(GetID(GetActionTarget(1)), LNKT)) pBuilding->LostTarget(GetActionTarget(1),1);
// Umspannwerk als Leitungsziel
if(!!SEqual(Format("%i",GetID(GetActionTarget(1))), "_USW")) GetActionTarget(1)->LostTarget(pBuilding,nil);
if(!!SEqual(Format("%i",GetID(GetActionTarget(nil))), "_USW")) GetActionTarget(nil)->LostTarget(pBuilding,1);

}

private func Transfer()
{
  //Message("ActionTarget0: %s|ActionTarget1: %s",this(),GetName(GetActionTarget(0)),GetName(GetActionTarget(1)));
  pTgt1 = GetActionTarget(nil);
  pTgt2 = GetActionTarget(1);
  if(fTransferStop) return(nil);
  var energy;
  if((GetEnergy(GetActionTarget()) ?? 0)<=3)
      energy = GetEnergy(GetActionTarget());
  else
  {
      if(GetPlrKnowledge(GetOwner(GetActionTarget()),IMLO))
          energy = (GetEnergy(GetActionTarget()) ?? 0) / 2; // mit verbesserten Leitungen wird statt nem drittel, die hälfte übertragen
      else
        energy = (GetEnergy(GetActionTarget()) ?? 0) / 3; // dies ist das eigentliche Limit, maximal 1/3 von dem was auf LAger ist, alle 10 Frames übertragen. da 100 das maximum ist, also maximal 33,33... was teilweise ganz schön wenig ist, zumindest fuer zukunftsgebäude
  }
  var limit = 100 - (GetEnergy(GetActionTarget(1) ) ?? 0); // nicht mehr übertragen als vorhanden
  energy = Min(Min(energy, limit), 20);
  DoEnergy(-(energy ?? 0), GetActionTarget() );
  DoEnergy(energy, GetActionTarget(1) );
}

protected func LineBreak(bool fNoMsg)
{
  
  
  fNoMsg ??= false;
  Sound("LineBreak");
  if(!fNoMsg) BreakMessage();
  
  if(!!SEqual(Format("%i",GetID(pTgt1)), "_USW")) {pTgt1->LostTarget(pTgt2,1); return(nil);}
  if(!!SEqual(Format("%i",GetID(pTgt2)), "_USW")) {pTgt2->LostTarget(pTgt1,nil); return(1);}
}
  
private func BreakMessage()
{
  // Meldung bei Leitungsbausatz am einen oder anderen Ende ausgeben
  var line = GetActionTarget(nil);
  if(!Equal(GetID(line), LNKT)) line = GetActionTarget(1);

  Message("$TxtLinebroke$", line);
}

func IsStopped() {return(fTransferStop);}
/*func SetLocalVar(iIndex, iValue) 
{
Local(iIndex)=iValue;
}*/
