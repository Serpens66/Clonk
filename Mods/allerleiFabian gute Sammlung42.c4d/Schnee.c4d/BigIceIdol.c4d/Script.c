/* Große Eisstatue */

#strict

protected func Initialize() {
  SetCategory(C4D_Vehicle());
  return(1);
}

protected func CheckTemp() { if(GetTemperature()>0) Melt(); }

private func Melt() {
  DoCon(-2);
  CastPXS("Water",8,0);
}