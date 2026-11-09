/*-- Ventilator --*/

#strict

func Completion() {
  var x = GetX(), y = GetY() -20;
    SetPosition(x, y);
SetCategory(C4D_StaticBack());
  SetAction("Turn");
  return(1);
}
