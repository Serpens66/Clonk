/*-- Brückensegment --*/

#strict

protected func Initialize() {
SetAction("Closed");
SetMask();
Local(1)=1;
return(1);
}

private func SetMask(){
SetSolidMask(0,24,48,8,0,0);
  return(1);
}

private func RemoveMask(){
if(Local(1)==1){
SetSolidMask(48,24,48,8,0,0);}
  return(1);
}



private func Timer(){
if(Local(1)==1){
//Kommt ein Fahrstuhlkorb???
Local(0)=GetPhase();
// if(FindObject(ELEC,-15,-15,30,30)||FindObject(CPEC,-15,-15,30,30))
if(FindObject2(Find_InRect(-15,-32,30,64),Find_Or(Find_ID(ELEC),Find_ID(ELE_),Find_ID(CPEC))))
{
    if(GetAction()S="Closed") {SetAction("Open"); }
    if(GetAction()S="Close")
    SetAction("Open"); 
    SetPhase(Local(0));
    return(1);
} 
else {
if(GetAction()S="Opened") {SetAction("Close"); }
if(GetAction()S="Open") {
SetAction("Close"); 
SetPhase(Local(0));}
}
SetSolidMask(0,24,48,8,0,0);
//Ist da ein zug? 
if(FindObject(STMG,-20,-20,40,40)){
//Dann ganz dicht machen
SetSolidMask(96,24,48,8,0,0);
return(1);
}
return(1);
}}
