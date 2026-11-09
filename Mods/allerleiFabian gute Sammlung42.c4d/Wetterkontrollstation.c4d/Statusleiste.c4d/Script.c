/*-- Statusleiste --*/

#strict 3

public func Start(...) {
  SetAction("Start");
  return(1);
}

public func Ende(...) {
  return(RemoveObject(this()));
}