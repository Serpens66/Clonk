/* Identifizierer*/

#strict


global func IdentifyShip(id idObj)
{
  if(idObj==MAM_) return(1);
  if(idObj==DFB_) return(1);
  if(idObj==DRA_) return(1);
  if(idObj==CCD_) return(1);
  if(idObj==CGC_) return(1);
  if(idObj==TCN_) return(1);
  if(idObj==CAN_) return(1);
  if(idObj==CAB_) return(1);
  if(idObj==CGE_) return(1);
  if(idObj==CAG_) return(1);
  if(idObj==CTE_) return(1);
  if(idObj==CBR_) return(1);
  if(idObj==CCF_) return(1);
  if(idObj==CCG_) return(1);
  if(idObj==MLC_) return(1);
  if(idObj==CGG_) return(1);
  if(idObj==TAS_) return(1);
  if(idObj==TBR_) return(1);
  if(idObj==TCH_) return(1);  
  if(idObj==MOO_) return(1);
  if(idObj==PRE_) return(1);
  if(idObj==PRL_) return(1);
  if(idObj==SCR_) return(1);
  if(idObj==AVP_) return(1);
  return(0);
}

global func IdentifyFighter(id idObj)
{
  if(idObj==DRA_) return(1);
  if(idObj==MOO_) return(1);
  if(idObj==PRE_) return(1);
  if(idObj==PRL_) return(1);
  if(idObj==SCR_) return(1);
  return(0);
}

global func IdentifyStippel(id idObj)
{
  if(idObj==ST5B) return(1);
  if(idObj==KG5B) return(1);
  if(idObj==MI5B) return(1);
  if(idObj==MF5B) return(1);
  if(idObj==MOO_) return(1);
  return(0);
}
