#strict 3

local fRemove, iSpeed, pShot, w, l, r, x, y;

public func Set(int iWidth, int iLength, object pSht)
{
  iWidth ??= 0;
  iLength ??= 0;
  pShot = pSht;  

  w = 1000*(iWidth ?? 0)/20;
  l = 1000*(iLength ?? 0)/100;

  var iXDir = GetXDir(pShot,100);
  var iYDir = GetYDir(pShot,100);

  iSpeed = Sqrt((iXDir ?? 0)*(iXDir ?? 0)/100+(iYDir ?? 0)*(iYDir ?? 0)/100);

  SetAction("Travel2");
  SetXDir(iXDir,nil,100);
  SetYDir(iYDir,nil,100);

  r = Angle(nil,nil,iXDir,iYDir);
  x = GetX();
  y = GetY();

  // richtig positionieren
  SetPosition(GetX(pShot),GetY(pShot));
  Traveling();
}

/* Timer */

private func Traveling()
{
  // The shot is gone -> remove
  if(!fRemove)
    if(!pShot)
      Remove();

  // Display
  DrawTransform();
  if(pShot) 
    if(pShot->~TrailColor())
      SetClrModulation(pShot->~TrailColor(GetActTime()));

  if(Equal(l, nil)) return(RemoveObject());
}

public func Remove() {
  SetXDir();
  SetYDir();
  l = Min(l,10*(Distance(x,y,GetX(),GetY()) ?? 0));
  fRemove = true;
}

public func DrawTransform() {

  // skip because nothing has to be transformed
  if(And(!fRemove, (l ?? 0) < 10*(Distance(x,y,GetX(),GetY()) ?? 0))) return(nil);

  // stretch >-<
  if(fRemove) l = Max(nil,(l ?? 0)-(iSpeed ?? 0));

  // stretch <->
  var h = Min(l,10*(Distance(x,y,GetX(),GetY()) ?? 0));

  var fsin = -(Sin(r, 1000) ?? 0), fcos = Cos(r, 1000);

  var xoff = -((GetActMapVal("Facet",GetAction(),nil,2) ?? 0)*(w ?? 0)/1000)/2-1;
  var yoff = nil;

  var width = (+(fcos ?? 0) ?? 0)*(w ?? 0)/1000, height = (+(fcos ?? 0) ?? 0)*(h ?? 0)/1000;
  var xskew = (+(fsin ?? 0) ?? 0)*(h ?? 0)/1000, yskew = -(fsin ?? 0)*(w ?? 0)/1000;

  var xadjust = (+(fcos ?? 0) ?? 0)*(xoff ?? 0) + (fsin ?? 0)*(yoff ?? 0);
  var yadjust = -(fsin ?? 0)*(xoff ?? 0) + (fcos ?? 0)*(yoff ?? 0);

  // set matrix values
  SetObjDrawTransform (
    width, xskew, xadjust,
    yskew, height, yadjust
  );
}

func NoWarp() { return(true); }
