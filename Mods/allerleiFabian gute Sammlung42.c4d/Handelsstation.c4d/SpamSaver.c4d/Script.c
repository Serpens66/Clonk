/*-- SpamSaver --*/

local SpamObject;

#strict 3

func SetSaver(...) {
  SpamObject=Par(nil);
  return(1);
}

func Object(...) { return(SpamObject);
}