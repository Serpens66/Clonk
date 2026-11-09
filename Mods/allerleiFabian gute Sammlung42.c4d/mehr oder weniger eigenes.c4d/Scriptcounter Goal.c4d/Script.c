/*-- Scriptziel --*/

#strict
#include GOAL

public func IsFulfilled()
{
if(ScriptCounter()>=999998)
  return(1);
return(0);
}

protected func Activate(player)
{
  // schon fertig
  if (IsFulfilled())
    return(MessageWindow("Mission completed, you are victorious!", player));
  // Nachricht ausgeben, was noch zu tun ist
  return(MessageWindow("Erledige die vom Szenario vorgegebenen Aufgaben", player));
}

