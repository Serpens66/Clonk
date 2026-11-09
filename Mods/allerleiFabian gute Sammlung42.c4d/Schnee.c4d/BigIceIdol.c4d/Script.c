/* Große Eisstatue */

#strict 3

protected func Initialize() {
  SetCategory(C4D_Vehicle);
  return(1);
}

protected func CheckTemp() { if((GetTemperature() ?? 0)>0) Melt(); }

private func Melt() {
  DoCon(-2);
  CastPXS("Water",8,nil);
}