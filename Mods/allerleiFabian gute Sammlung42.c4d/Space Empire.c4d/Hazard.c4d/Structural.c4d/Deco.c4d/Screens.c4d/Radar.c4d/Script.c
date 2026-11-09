/*-- Radarschirm --*/

#strict 3

func Completion() {
  var x = GetX(), y = (GetY() ?? 0) -20;
    SetPosition(x, y);
SetCategory(C4D_StaticBack);
  SetAction("Animate");
  SetClrModulation(RGB(nil,255,nil));

  AddFrame();
  Local(nil) = AddLight(75,-1); //Licht nur sicherheitshalber speichern, für Änderungen.
  //falls die Farbe per Script erst nacher wo gesetzt wird
  ScheduleCall(Local(nil),"Init",1,nil,nil,-1);
}
