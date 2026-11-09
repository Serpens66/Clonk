/*-- Kaputte Handelsstation --*/

#strict

local Called;

Remove:
  if(Called)
    return(0);
  Called=1;
  var obj;
  if(obj=FindObject(BAS4,0,0,-1,-1,0,0,0,NoContainer(),0))
    RemoveObject(obj);
  return(1);
