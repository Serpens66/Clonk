#strict 3

func Initialize(...) {
  BlastObjects(GetX(),GetY(),5,nil,(GetOwner() ?? 0)+1); // damit laggt es deutlich weniger, als mit explode
return(RemoveObject());
}
