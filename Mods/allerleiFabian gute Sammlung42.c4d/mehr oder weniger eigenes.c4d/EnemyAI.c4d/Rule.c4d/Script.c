/*-- Gegner KI --*/

#strict 3

protected func Activate(int iByPlayer)
  {
  iByPlayer ??= 0;
  MessageWindow(GetDesc(), iByPlayer);
  return true;
  }
