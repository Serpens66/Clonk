/*-- Pfad - Visualisierung --*/

#strict 3

local so,eo, sx,sy, ex,ey, flag, jtpk, path;

protected func Timer() // Schaut nach Änderungen
{
  if(!so) return(RemoveObject());
  if(!(so->GetPathTarget(path))) return(RemoveObject());
  // Irgendeine klitzekleine Änderung?
  if(Or(Or(Or(Or(Or(Or(!Equal(so->GetX(), sx),
     !Equal(so->GetY(), sy)),
     !Equal(eo->GetX(), ex)),
     !Equal(eo->GetY(), ey)),
     !Equal(so->GetPathFlag(path), flag)),
     !Equal(so->GetPathJetpack(path), jtpk)),
     !Equal(so->GetPathTarget(path), eo)))

       Set(so, so->GetPathTarget(path), so->GetPathFlag(path), so->GetPathJetpack(path), path);
}

public func Set(object start, object end, int flags, int jetpack, int iPath) {
  flags ??= 0;
  jetpack ??= 0;
  iPath ??= 0;
  so = start;
  eo = end;
  sx = start->GetX();
  sy = start->GetY();
  ex = end->GetX();
  ey = end->GetY();
  flag = flags;
  jtpk = jetpack;
  path = iPath;

  SetObjectBlitMode(GFX_BLIT_Additive);
  SetClrModulation(MyColor(flags,jetpack,start));

  var sx = GetX(start);
  var sy = GetY(start);
  var ex = GetX(end);
  var ey = GetY(end);
  var cl = 1000*(Distance(sx,sy,ex,ey) ?? 0)/(GetActMapVal("Facet",GetAction(),nil,3) ?? 0);
  var w = 650;
  var r = (Angle(sx,sy,ex,ey) ?? 0)-180;

  var fsin = -(Sin(r, 1000) ?? 0), fcos = Cos(r, 1000);

  var xoff = -4;
  var yoff = nil;

  var width = (+(fcos ?? 0) ?? 0)*(w ?? 0)/1000, height = (+(fcos ?? 0) ?? 0)*(cl ?? 0)/1000;
  var xskew = (+(fsin ?? 0) ?? 0)*(cl ?? 0)/1000, yskew = -(fsin ?? 0)*(w ?? 0)/1000;

  var xadjust = (+(fcos ?? 0) ?? 0)*(xoff ?? 0) + (fsin ?? 0)*(yoff ?? 0);
  var yadjust = -(fsin ?? 0)*(xoff ?? 0) + (fcos ?? 0)*(yoff ?? 0);

  // set matrix values
  SetObjDrawTransform (
    width, xskew, xadjust,
    yskew, height, yadjust
  );

}

private func MyColor(int flags, int jetpack, object start) {
  flags ??= 0;
  jetpack ??= 0;
  SetAction("Vis",start);
  if(jetpack) {
    SetAction("Vis3",start);
    return(RGB(255,nil,nil));
  }
  else {
    if(Equal(flags, Path_MoveTo)) return(RGB(nil,255,nil));
    if(Equal(flags, Path_Jump)) return(RGB(255,255,nil));
    if(Equal(flags, Path_Backflip)) {
      SetAction("Vis3",start);
      return(RGB(255,255,nil));
    }
    SetAction("Vis2",start);
    if(Equal(flags, Path_Lift)) return(RGB(255,nil,255));
  }
}
