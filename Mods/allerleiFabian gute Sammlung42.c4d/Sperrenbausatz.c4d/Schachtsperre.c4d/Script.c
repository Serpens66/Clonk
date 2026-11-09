/*-- Brückensegment --*/

#strict 3

protected func Initialize() {
SetAction("Closed");
SetMask();
Local(1)=1;
return(1);
}

private func SetMask(){
SetSolidMask(nil,24,48,8,nil,nil);
  return(1);
}

private func RemoveMask(){
if(Equal(Local(1), 1)){
SetSolidMask(48,24,48,8,nil,nil);}
  return(1);
}



private func Timer(){
if(Equal(Local(1), 1)){
//Kommt ein Fahrstuhlkorb???
Local(nil)=GetPhase();
// if(FindObject(ELEC,-15,-15,30,30)||FindObject(CPEC,-15,-15,30,30))
if(FindObject2(Find_InRect(-15,-32,30,64),Find_Or(Find_ID(ELEC),Find_ID(ELE_),Find_ID(CPEC))))
{
    if(!!SEqual(GetAction(), "Closed")) {SetAction("Open"); }
    if(!!SEqual(GetAction(), "Close"))
    SetAction("Open"); 
    SetPhase(Local(nil));
    return(1);
} 
else {
if(!!SEqual(GetAction(), "Opened")) {SetAction("Close"); }
if(!!SEqual(GetAction(), "Open")) {
SetAction("Close"); 
SetPhase(Local(nil));}
}
SetSolidMask(nil,24,48,8,nil,nil);
//Ist da ein zug? 
if(FindObject(STMG,-20,-20,40,40)){
//Dann ganz dicht machen
SetSolidMask(96,24,48,8,nil,nil);
return(1);
}
return(1);
}}
