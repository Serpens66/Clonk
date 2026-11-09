/*-- Farbhilfsfunktionen --*/

//Hellt Farbe color um value auf oder dunkelt um den negativen Wert ab.

#strict 3


global func DoColorBrightness(int color, int value)
{
  color ??= 0;
  value ??= 0;
  if((value ?? 0) > 0)
  {
    color = DoRGBaValue(color, Min(255-(GetRGBaValue(color, 1) ?? 0), value), 1);
    color = DoRGBaValue(color, Min(255-(GetRGBaValue(color, 2) ?? 0), value), 2);
    color = DoRGBaValue(color, Min(255-(GetRGBaValue(color, 3) ?? 0), value), 3);
  }
  if((value ?? 0) < 0)
  {
    color = DoRGBaValue(color, -(Min(GetRGBaValue(color, 1), -(value ?? 0)) ?? 0), 1);
    color = DoRGBaValue(color, -(Min(GetRGBaValue(color, 2), -(value ?? 0)) ?? 0), 2);
    color = DoRGBaValue(color, -(Min(GetRGBaValue(color, 3), -(value ?? 0)) ?? 0), 3);
  }
  return color;
}