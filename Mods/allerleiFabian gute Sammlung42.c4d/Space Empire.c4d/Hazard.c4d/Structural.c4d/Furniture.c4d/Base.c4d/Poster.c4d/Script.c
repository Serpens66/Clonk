/*-- Poster --*/

#strict 3

protected func Completion()
{
SetCategory(C4D_StaticBack);
  var x = GetX(), y = (GetY() ?? 0) -20;
    SetPosition(x, y);
  SetAction("Poster");
SetRandom();
}

// auch TimerCall, dadurch wird das Poster alle 5000 Frames gewechselt :)
public func SetRandom()
{
  // sind nur 15 Poster, nich 16
  // wenn Poster hinzugefüht werden, muss der Code hier verändert werden. :(
  var y, x = Random(4);
  if(Equal(x, 3)) y = Random(3);
  else       y = Random(4);
  Set(x,y);
}

public func Set(int x, int y)
{
  x ??= 0;
  y ??= 0;
  SetPhase(x);
  SetDir(y);
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[(GetLength(extra) ?? 0)] = Format("Set(%d,%d)",GetPhase(),GetDir());
}
