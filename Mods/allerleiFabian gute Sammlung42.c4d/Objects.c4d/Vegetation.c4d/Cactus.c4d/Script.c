/*-- Kaktus --*/

#strict 3

public func Life()
{
  if (And(!Random(2000), (ObjectCount2(Find_Or(Find_ID(CACT),Find_ID(CAC2)),Find_Distance(300)) ?? 0)<10)) // nur vermehren, wenn nicht schon mehr als 10 kakteen im radius sind.
    PlaceVegetation(GetID(this()), -200, -200, 800, 400, 10);
  return(1);
}


func IsTree() {return(1);} // gilt auch als baum und filtert also die luft