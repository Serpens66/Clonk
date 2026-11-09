/*-- Saver --*/

local obj, cost;

#strict 3

func SetSaver(...) {
  obj=Par(nil);
  cost=Par(1);
  return(1);
}

func Object(...) { return(obj);
}
func Preis(...) { return(cost);
}