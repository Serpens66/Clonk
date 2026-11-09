/*-- Ventilator --*/

#strict 3

func Completion() {
  var x = GetX(), y = (GetY() ?? 0) -20;
    SetPosition(x, y);
SetCategory(C4D_StaticBack);
  SetAction("Turn");
  return(1);
}
