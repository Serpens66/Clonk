/*--- Energieleitung ---*/

#strict

local fTransferStop;

local pTgt1,pTgt2;

protected func Initialize()
{
  Local(0)=68; 
  Local(1)=68;  
  Local(2)=30;
  Local(3)=30;

  //ChangeDef(PWR2,this());
  fTransferStop = 0;
  SetAction("Connect");  
  SetVertex(0,0,GetX()); SetVertex(0,1,GetY());
  SetVertex(1,0,GetX()); SetVertex(1,1,GetY());
  SetPosition(0, 0, this() );
}

func ChangeTransfer()
{
Sound();
if( fTransferStop) {fTransferStop=0; Sound("Energize",0,this()); return(0);}
if(!fTransferStop) {fTransferStop=1; Sound("DeEnergize",0,this()); return(1);}
}

func LostConnection(pBuilding) 
{
    // Log(Format("LostConnection pBuilding %v , ActionTarget0: %v, ActionTarget1 %v",pBuilding,GetActionTarget(0),GetActionTarget(1)));
// Umspannwerk als Leitungsursprung  
if(Format("%i",GetID(pBuilding)) eq "_USW") if(GetID(GetActionTarget(0))!=LNKT) pBuilding->LostTarget(GetActionTarget(0),0);
if(Format("%i",GetID(pBuilding)) eq "_USW") if(GetID(GetActionTarget(1))!=LNKT) pBuilding->LostTarget(GetActionTarget(1),1);
// Umspannwerk als Leitungsziel
if(Format("%i",GetID(GetActionTarget(1))) eq "_USW") GetActionTarget(1)->LostTarget(pBuilding,0);
if(Format("%i",GetID(GetActionTarget(0))) eq "_USW") GetActionTarget(0)->LostTarget(pBuilding,1);

}

private func Transfer()
{
  //Message("ActionTarget0: %s|ActionTarget1: %s",this(),GetName(GetActionTarget(0)),GetName(GetActionTarget(1)));
  pTgt1 = GetActionTarget(0);
  pTgt2 = GetActionTarget(1);
  if(fTransferStop) return(0);
  var energy;
  if(GetEnergy(GetActionTarget())<=3)
      energy = GetEnergy(GetActionTarget());
  else
  {
      if(GetPlrKnowledge(GetOwner(GetActionTarget()),IMLO))
          energy = GetEnergy(GetActionTarget()) / 2; // mit verbesserten Leitungen wird statt nem drittel, die hälfte übertragen
      else
        energy = GetEnergy(GetActionTarget()) / 3; // dies ist das eigentliche Limit, maximal 1/3 von dem was auf LAger ist, alle 10 Frames übertragen. da 100 das maximum ist, also maximal 33,33... was teilweise ganz schön wenig ist, zumindest fuer zukunftsgebäude
  }
  var limit = 100 - GetEnergy(GetActionTarget(1) ); // nicht mehr übertragen als vorhanden
  energy = Min(Min(energy, limit), 20);
  DoEnergy(-energy, GetActionTarget() );
  DoEnergy(energy, GetActionTarget(1) );
}

protected func LineBreak(bool fNoMsg)
{
  
  
  Sound("LineBreak");
  if(!fNoMsg) BreakMessage();
  
  if(Format("%i",GetID(pTgt1)) eq "_USW") {pTgt1->LostTarget(pTgt2,1); return(0);}
  if(Format("%i",GetID(pTgt2)) eq "_USW") {pTgt2->LostTarget(pTgt1,0); return(1);}
}
  
private func BreakMessage()
{
  // Meldung bei Leitungsbausatz am einen oder anderen Ende ausgeben
  var line = GetActionTarget(0);
  if(GetID(line) != LNKT) line = GetActionTarget(1);

  Message("$TxtLinebroke$", line);
}

func IsStopped() {return(fTransferStop);}
/*func SetLocalVar(iIndex, iValue) 
{
Local(iIndex)=iValue;
}*/
