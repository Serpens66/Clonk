/*-- Verbrannte Holzhütte --*/

#strict 3

/* Fundament (Local 9) */
 
protected func Destruction(...) {
  if (Local(9)) RemoveObject(Local(9));
  return(1);
}
