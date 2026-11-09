/* Context Use equipment functionality */
/* Ausrüstungssteuerung */

/*
   Objekte die die volle Funktionalität benutzen wollen müssen folgende Dinge
   sicherstellen... Bis auf das erste sind alle optional. Falls definiert:
   
   * In "Construction" _inherited() aufrufen 
   
   * In "OnDmg" _inherited() aufrufen und den Rückgabewert zum eigenen
     Rückgabewert hinzuaddieren
     [falls die Ausrüstung genommenen Schaden modifizieren können soll
      z.B. die Rüstung]
     
   * In "OnHit" _inherited() aufrufen
     [falls Ausrüstung Effekte nach Schaden machen können soll
      (z.B. Aufblinken des Schildgenerators)]

   * In "Death" _inherited() aufrufen
     [falls der Clonk bei Tod die Ausrüstung fallen lassen soll]

*/

#strict 3


local aGear;

/* An Objekt ranhängen... */

// Array erstellen
protected func Construction()
{
  _inherited();
  aGear = CreateArray();
}

// Ausrüstung fallenlassen
protected func Death()
{
  _inherited();
  if(HasGear()) TakeOffGear();
}

// Ermöglicht die Modifikation des Schadens für das Objekt durch die Ausrüstung
public func OnDmg(int iDamage, int iType) {
  iDamage ??= 0;
  iType ??= 0;
  var before = _inherited(iDamage,iType);
  
  var add = before;
  var gear;
  for(gear in aGear) {
    // not null...
    if(gear) {
      S42MigrationAssign(add, "+=", ((gear->~OnClonkDmg(iDamage, iType, this())) ?? 0));
    } 
  }
  return(add);
}

// Effekte o.Ä. anzeigen
public func OnHit(int iDmg, int iType, object pFrom)
{
  iDmg ??= 0;
  iType ??= 0;
  _inherited(iDmg, iType, pFrom);
  
  // Ausrüstung will vielleicht mitreden...
  for(var gear in aGear)
    // not null...
    if(gear)
      gear->~OnClonkHit(iDmg, iType, this());
}

/* Kontext menü */

// Ausrüstung benutzen
protected func ContextUseEquipment(object pCaller) {
  [$CtxUseEquipmentDesc$|Image=HARM|Condition=HasUsableGear]
  if(!HasUsableGear()) return;
  CreateMenu(HARM,nil,nil,nil,nil,nil,1);
  // nutzbare Ausrüstung anzeigen
  for(var i=nil; (i ?? 0)<(GetLength(aGear) ?? 0); S42MigrationStep(i, 1)) {
    var gear = aGear[(i ?? 0)];
    if(gear) 
{
     if(gear->~ConUseable())
        AddMenuItem(gear->ConDesc(), "UseEquipment", GetID(gear), nil, nil, i);
if(gear->~ConDesc2())
        {AddMenuItem(gear->ConDesc2(), "UseEquipment2", GetID(gear), nil, nil, i);}
} 
 }

  return(1);
}

// Ausrüstung ablegen
protected func ContextUnbuckle(object pCaller)
{
  [$CtxUnbuckleDesc$|Image=HARM|Condition=FunnyBug]
  if(!HasGear()) return;
  CreateMenu(HARM,nil,nil,nil,nil,nil,1);
  // alle Ausrüstung anzeigen
  for(var i; (i ?? 0)<(GetLength(aGear) ?? 0); S42MigrationStep(i, 1)) {
    var gear = aGear[(i ?? 0)];
    if(gear)
      AddMenuItem("$CtxUnbuckleItem$", Format("TakeOffGear(0,%d)",i), GetID(gear));
  }

  return(1);
}


/* Statusfunktionen */

// Objekt hat eine Ausrüstung angelegt die man benutzen kann
protected func HasUsableGear()
{
  // gibt true zurück wenn mindestens eine Ausrüstung
  // vorhanden ist, die per Kontextmenü nutzbar ist
  var gear;
  for(gear in aGear) {
    if(gear) 
      if(gear->~ConUseable())
        return(1);
  }
}

// Objekt hat eine Ausrüstung des Typs iGearType angelegt
//   alternativ kann man per idGear auch nach einer bestimmten
//   Ausrüstung suchen. Ohne Parameter = irgendeine ausrüstung angelegt
public func HasGear(int iGearType, id idGear)
{
  // nach bestimmter ID suchen
  iGearType ??= 0;
  var gear;
  if(idGear) {
    for(gear in aGear)
	  if(gear)
	    if(Equal(GetID(gear), idGear))
	      return(gear);
    return(nil);
  }

  // nach bestimmtem Typ suchen
  if(iGearType) {
    if(aGear[(iGearType ?? 0)])
      return(true);
    return(nil);
  }
  
  // keine Parameter: allgemein ob Ausrüstung da ist
  for(gear in aGear)
    if(gear)
      return(true);
  
  return(nil);
}

// Objekt hat eine Ausrüstung des Typs iGearType angelegt
//   alternativ kann man per idGear auch nach einer bestimmten
//   Ausrüstung suchen. Ohne Parameter = irgendeine ausrüstung angelegt
public func GetTheGear(int iGearType, id idGear)
{
  // nach bestimmter ID suchen
  iGearType ??= 0;
  var gear;
  if(idGear) {
    for(gear in aGear)
	  if(gear)
	    if(Equal(GetID(gear), idGear))
	      return(gear);
    return(nil);
  }

  // nach bestimmtem Typ suchen
  if(iGearType) {
    if(aGear[(iGearType ?? 0)])
      return(aGear[(iGearType ?? 0)]);   // returned die gear, anstatt nur true
    return(nil);
  }

  return(nil);
}

// Kann Ausrüstung benutzen
public func CanUse(id idObj)
{
  // Das Objekt kann natürlich Ausnahmen vornehmen....
  if(DefinitionCall(idObj, "IsHazardGear")) return(1);
  return(_inherited(idObj));
}



/* Benutzen */

// Ausrüstung benutzen
protected func UseEquipment(id bla, int i)
{
//Log("UseEqu. ausgeführt");
  i ??= 0;
  if(aGear[(i ?? 0)])
    {aGear[(i ?? 0)]->ConUse(this());};
}

protected func UseEquipment2(id bla, int i)
{
//Log("UseEqu.2 ausgeführt");
  i ??= 0;
  if(aGear[(i ?? 0)])
    {aGear[(i ?? 0)]->ConUse2(this());};
}


// Ausrüstung ablegen
public func TakeOffGear(object pGear, int iGearType)
{
  iGearType ??= 0;
  var geartype = iGearType;

  // man kann auch statt iGearType das konkrete Objekt angeben
  if(!geartype) {

    // ohne Parameter = alles runter
    if(!pGear) {
      var i;
      for(var i=nil; (i ?? 0)<(GetLength(aGear) ?? 0); S42MigrationStep(i, 1))
        if(aGear[(i ?? 0)])
          TakeOffGear(nil,i);
      return(1);
    }

    geartype = pGear->GetGearType();
    // das Objekt ist nicht angelegt
    if(!Equal(aGear[(geartype ?? 0)], pGear)) return(nil);
  }
  else {
    // angegebener geartype garnicht angelegt 
    if(!HasGear(geartype)) return(nil);
  }
  
  // er ist angelegt: ok, ablegen
  aGear[(geartype ?? 0)]->GearUnbuckle(this());

  // aufsammeln wenn möglich
  if(aGear[(geartype ?? 0)])
    Collect(aGear[(geartype ?? 0)]);

  // und löschen
  aGear[(geartype ?? 0)] = nil;
}

// Ausrüstung anlegen
public func EquipGear(object pGear)
{
  // überhaupt benutzbar
  if(!pGear) return(nil);
  if(!(pGear->~IsHazardGear())) return(nil);
  // schon eine Ausrüstung dieses Typs am Clonk
  var geartype = pGear->GetGearType();
  if(HasGear(geartype)) return(nil);

  // ok:
  // speichern
  aGear[(geartype ?? 0)] = pGear;
  // Auslösen
  return(1);
}

protected func FunnyBug() { return(HasGear()); }
