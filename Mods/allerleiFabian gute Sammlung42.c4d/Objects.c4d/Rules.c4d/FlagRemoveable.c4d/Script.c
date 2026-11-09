/*-- Flagge abnehmbar --*/

#strict 3

protected func Activate(iByPlayer)
{
  MessageWindow(GetDesc(), iByPlayer);
  return(1);
}
