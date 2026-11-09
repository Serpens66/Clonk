/* Identifizierer*/

#strict 3


global func IdentifyShip(id idObj)
{
  if(Equal(idObj, MAM_)) return(1);
  if(Equal(idObj, DFB_)) return(1);
  if(Equal(idObj, DRA_)) return(1);
  if(Equal(idObj, CCD_)) return(1);
  if(Equal(idObj, CGC_)) return(1);
  if(Equal(idObj, TCN_)) return(1);
  if(Equal(idObj, CAN_)) return(1);
  if(Equal(idObj, CAB_)) return(1);
  if(Equal(idObj, CGE_)) return(1);
  if(Equal(idObj, CAG_)) return(1);
  if(Equal(idObj, CTE_)) return(1);
  if(Equal(idObj, CBR_)) return(1);
  if(Equal(idObj, CCF_)) return(1);
  if(Equal(idObj, CCG_)) return(1);
  if(Equal(idObj, MLC_)) return(1);
  if(Equal(idObj, CGG_)) return(1);
  if(Equal(idObj, TAS_)) return(1);
  if(Equal(idObj, TBR_)) return(1);
  if(Equal(idObj, TCH_)) return(1);
  if(Equal(idObj, MOO_)) return(1);
  if(Equal(idObj, PRE_)) return(1);
  if(Equal(idObj, PRL_)) return(1);
  if(Equal(idObj, SCR_)) return(1);
  if(Equal(idObj, AVP_)) return(1);
  return(nil);
}

global func IdentifyFighter(id idObj)
{
  if(Equal(idObj, DRA_)) return(1);
  if(Equal(idObj, MOO_)) return(1);
  if(Equal(idObj, PRE_)) return(1);
  if(Equal(idObj, PRL_)) return(1);
  if(Equal(idObj, SCR_)) return(1);
  return(nil);
}

global func IdentifyStippel(id idObj)
{
  if(Equal(idObj, ST5B)) return(1);
  if(Equal(idObj, KG5B)) return(1);
  if(Equal(idObj, MI5B)) return(1);
  if(Equal(idObj, MF5B)) return(1);
  if(Equal(idObj, MOO_)) return(1);
  return(nil);
}
