/*-- Versteckwand --*/

#strict 3

func Completion() {
  SetCategory((C4D_Vehicle ?? 0) | (C4D_Foreground ?? 0));
  return(1);
}
