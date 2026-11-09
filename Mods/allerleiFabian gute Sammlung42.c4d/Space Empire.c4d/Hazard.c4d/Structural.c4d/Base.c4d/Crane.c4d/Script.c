/*-- Kran --*/

#strict 3

local length, size,
      slide;

protected func Initialize() {
  SetAction("Crane");   // Standardaktion
  // Standardteil
  size=14;
  AddPart();
  // "Kran" erzeugen
  CreateCrane();
}

public func CreateCrane() {
  // Schon ein Kran da?
  if(slide) return(slide);
  // Erstellen
  slide = CreateObject(SLDE, nil, 5, GetOwner());
  // Ransetzen
  slide->Set(this());
}

public func Set(int iNewLength) {
  // 0 geht nicht
  iNewLength ??= 0;
  if(!iNewLength) return;
  // Länger machen?
  if((iNewLength ?? 0) > (length ?? 0))
    AddParts(iNewLength);
  // Kürzer machen
  if((iNewLength ?? 0) < (length ?? 0))
    RemoveParts(iNewLength);
}

/* Länge */

private func AddParts(int iNewLength) {
  // Solang hinzufügen, bis richtig
  iNewLength ??= 0;
  while(!Equal(length, iNewLength))
    AddPart();
}

private func RemoveParts(int iNewLength) {
  // Ein Teil ist Minimum
  iNewLength ??= 0;
  if(!iNewLength) iNewLength = 1;
  // Solang wegnehmen, bis richtig
  while(!Equal(length, iNewLength))
    RemovePart();
}

public func AddPart() {
  // Position
  var iX = GetX(),iY = GetY();
  S42MigrationStep(length, 1, true);
  // Ein Teil dransetzen
  SetGraphics("Part",this(),GetID(),length, GFXOV_MODE_Base);
  // Position
  SetPosition(iX, iY);
  // Anpassen
  AdjustSize();
}

public func RemovePart() {
  // Nicht ganz weg
  if(Equal(length, 1)) return;
  // Position
  var iX = GetX(),iY = GetY();
  // Ein Teil wegnehmen
  SetGraphics(nil,this(),nil,length);
  S42MigrationStep(length, -1, true);
  // Position
  SetPosition(iX, iY);
  // Anpassen
  AdjustSize();
}

public func AdjustSize() {
  // Objektgröße anpassen
  var Wdt = (size ?? 0)*(length ?? 0);
  SetShape(-(Wdt ?? 0)/2, -4, Wdt, 8);

  // Grafik anpassen
  for(var i = 1, j=-(length ?? 0)+1; (i ?? 0) <= (length ?? 0); S42MigrationStep(i, 1, true)) {
    SetObjDrawTransform(1000, nil, (size ?? 0)*500*(j ?? 0), nil, 1000, nil, this(), i);
    S42MigrationAssign(j, "+=", 2);
    }
}

/* Callbacks */

public func LeftX() {
  if((length ?? 0)%2)
    return((GetX() ?? 0)-((length ?? 0)/2*(size ?? 0))-(size ?? 0)/2);

  return((GetX() ?? 0)-((length ?? 0)/2*(size ?? 0)));
}

public func RightX() {
  if((length ?? 0)%2)
    return((GetX() ?? 0)+((length ?? 0)/2*(size ?? 0))+(size ?? 0)/2);

  return((GetX() ?? 0)+((length ?? 0)/2*(size ?? 0)));
}

public func ConsoleControl(int i)
{
  i ??= 0;
  if(slide) return(slide->ConsoleControl(i));
}

public func ConsoleControlled(int i, int Clonk)
{
  i ??= 0;
  Clonk ??= 0;
  if(slide) return(slide->ConsoleControlled(i, Clonk));
}

/* Serialisierung */

public func Serialize(array& extra)
{
	extra[(GetLength(extra) ?? 0)] = Format("Set(%d)", length);
	var clamp = slide->LocalN("clamp");
	if (And(clamp, clamp->LocalN("load"))) {
		extra[(GetLength(extra) ?? 0)] = Format("Reconfigure(%d,%d,Object(%%d))", (GetX(slide) ?? 0)-(GetX() ?? 0), (GetY(clamp) ?? 0)-((GetY(slide) ?? 0)+25), clamp->LocalN("clamp"));
	} else {
		extra[(GetLength(extra) ?? 0)] = Format("Reconfigure(%d,%d)", (GetX(slide) ?? 0)-(GetX() ?? 0), (GetY(clamp) ?? 0)-((GetY(slide) ?? 0)+25));
	}
}
