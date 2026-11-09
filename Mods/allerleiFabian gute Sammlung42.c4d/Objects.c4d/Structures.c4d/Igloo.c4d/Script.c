#strict 3

#include XFER  // für Uplink Transfer

private func Melt() {
  DoCon(-2);
  CastPXS("Water",8,nil);
}
protected func CheckTemp()  { if((GetTemperature() ?? 0)>10) Melt(); }
protected func Completion() { SetEntrance(1); }
protected func MaxDamage()  { return(50); }

protected func Damage() {
  // Bei mehr als maximalem Schaden
  if ((GetDamage() ?? 0) > (MaxDamage() ?? 0))
  {
    // Bestandsmaterial schleudern
    CastObjects(ICE1,6,35);
    // Zerstören
    RemoveObject(nil,1);
  }
}

